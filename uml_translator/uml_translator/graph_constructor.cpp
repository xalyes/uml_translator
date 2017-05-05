#include "StdAfx.h"

#include <iostream>
#include <unordered_map>
#include <stack>

#include <boost/format.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/depth_first_search.hpp>

#include "graph_constructor.h"
#include "node.h"
#include "edge.h"
#include "uml_enums.h"

class DfsCodeGenerator : public boost::default_dfs_visitor
{
public:
	DfsCodeGenerator(std::stack<std::string>& stack) : m_layers(stack)
	{
		m_layers.emplace("");
	}

	template <class T>
	inline T PopFromStack(std::stack<T>& stack)
	{
		const T topElement = stack.top();
		stack.pop();
		return topElement;
	}

	template <typename Vertex, typename Graph>
	void discover_vertex(Vertex vertex, const Graph& graph)
	{
		using namespace boost;
		static std::stack<Vertex> loppIds = 
		[]() -> std::stack<Vertex>
		{
			std::stack<Vertex> stack;
			stack.emplace(-1);
			return stack;
		}();

		static int counter = 0;
		static std::vector<int> coloredVertices(graph.vertex_set().size(), -1);

		coloredVertices[vertex] = counter++;

		if (graph[vertex]->GetType().ToString() == "InitialNode")
		{
			if (in_degree(vertex, graph))
				throw(new std::runtime_error("Initial node must have no input edges!"));

			m_layers.top().append("int main()\n{\n");
		}
		else if (graph[vertex]->GetType().ToString() == "Action")
		{
			if (in_degree(vertex, graph) == 2)
			{
				m_layers.top().append("do\n{\n");
				graph_traits<Graph>::in_edge_iterator ei, edge_end;
				for (tie(ei, edge_end) = in_edges(vertex, graph); ei != edge_end; ++ei)
					if (coloredVertices[source(*ei, graph)] == -1)
						loppIds.push(source(*ei, graph));
			}
			BOOST_FOREACH(const Effect& effect, graph[vertex]->GetEffects())
			{
				m_layers.top().append(effect.GetBody() + "\n");
			}
			if (loppIds.top() == vertex)
			{
				graph_traits<Graph>::out_edge_iterator ei, edge_end;
				tie(ei, edge_end) = out_edges(vertex, graph);
				if (ei == edge_end)
					throw(new std::runtime_error("Out edge is not found"));
				Vertex ver = target(*ei, graph);

				graph_traits<Graph>::out_edge_iterator ei2, edge_end2;
				int max = -1;
				int current = -1;
				for (tie(ei2, edge_end2) = out_edges(target(*ei, graph), graph); ei2 != edge_end2; ++ei2)
					if (coloredVertices[target(*ei2, graph)] > max)
					{
						max = coloredVertices[target(*ei2, graph)];
						current = target(*ei2, graph);
					}

				Guard guard = get(edge_all, graph)[edge(target(*ei, graph), current, graph).first]->GetGuards()[0];
				std::string loopBody(PopFromStack(m_layers));
				std::string loopHeader(PopFromStack(m_layers));
				format fmt(loopHeader);
				fmt % guard.GetBody() % loopBody;
				m_layers.top().append(fmt.str());
				loppIds.pop();
			}
		}
		else if (graph[vertex]->GetType().ToString() == "DecisionNode")
		{
			if (out_degree(vertex, graph) != 2)
				throw(new std::runtime_error("More than two output edges is not supported"));

			if (out_degree(vertex, graph) == 1)
				throw(new std::runtime_error("Decision node must have at least two output edges!"));

			if (in_degree(vertex, graph) == 2)
			{
				graph_traits<Graph>::in_edge_iterator ei, edge_end;
				for (tie(ei, edge_end) = in_edges(vertex, graph); ei != edge_end; ++ei)
					if (coloredVertices[source(*ei, graph)] == -1)
					{
						loppIds.push(source(*ei, graph));
						std::cout << loppIds.top() << std::endl;
					}
				m_layers.emplace("while (%s)\n{\n\t%s\n}\n");
				m_layers.emplace(""); // body
			}
		}
		else if (graph[vertex]->GetType().ToString() == "ActivityFinalNode")
		{
			if (out_degree(vertex, graph))
				throw(new std::runtime_error("ActivityFinalNode must have no output edges!"));

			m_layers.top().append("return 0;\n}");
		}
	}

	void PrintSourceCode()
	{
		std::cout << m_layers.top() << std::endl;
	}

private:
	std::stack<std::string>& m_layers;
};

void GraphConstructor::MakeGraph()
{
	size_t initialNode;
	std::unordered_map<size_t, unsigned long> idMap;

	BOOST_FOREACH(const Node& vertex, std::get<0>(m_activity))
	{
		const unsigned long LocalId = vertex.GetLocalId();
		const size_t VertexDescriptor = boost::add_vertex(std::make_shared<Node>(vertex), m_graph);
		idMap[VertexDescriptor] = LocalId;
		std::cout << LocalId << " inserted" << std::endl;
		if (m_graph[VertexDescriptor]->GetType().ToString() == "InitialNode")
		{
			initialNode = VertexDescriptor;
		}
	}

	BOOST_FOREACH(const Edge& edge, std::get<1>(m_activity))
	{
		const std::unordered_map<size_t, unsigned long>::const_iterator itSource = std::find_if(
			idMap.begin(), idMap.end(), boost::bind(&std::unordered_map<size_t, unsigned long>::value_type::second, _1) == edge.GetSource().GetLocalId()
		);
		const std::unordered_map<size_t, unsigned long>::const_iterator itTarget = std::find_if(
			idMap.begin(), idMap.end(), boost::bind(&std::unordered_map<size_t, unsigned long>::value_type::second, _1) == edge.GetTarget().GetLocalId()
		);

		if ((itSource != idMap.end()) && (itTarget != idMap.end()))
			boost::add_edge(itSource->first, itTarget->first, std::make_shared<Edge>(edge), m_graph);
	}

	const char* labels[] = {"DecisionNode", "ActivityFinalNode","InitialNode", "Action", "Action"};
	boost::write_graphviz(std::cout, m_graph, boost::make_label_writer(labels));

	std::stack<std::string> SourceCode;
	DfsCodeGenerator visitor(SourceCode);

	boost::depth_first_search(m_graph, boost::visitor(visitor).root_vertex(initialNode));

	visitor.PrintSourceCode();
}
