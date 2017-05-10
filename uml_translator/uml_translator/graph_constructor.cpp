#include "StdAfx.h"

#include <iostream>
#include <unordered_map>
#include <stack>
#include <algorithm>

#include <boost/format.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/depth_first_search.hpp>

#include "graph_constructor.h"
#include "node.h"
#include "edge.h"
#include "uml_enums.h"

template <class T>
inline T PopFromStack(std::stack<T>& stack)
{
	const T topElement = stack.top();
	stack.pop();
	return topElement;
}

class DfsCodeGenerator : public boost::default_dfs_visitor
{
public:
	DfsCodeGenerator(std::string& outputString) : m_output(outputString), m_currentThread(0)
	{
		std::stack<std::pair<boost::format, Graph::vertex_descriptor>> stack;
		auto pair = std::pair<boost::format, Graph::vertex_descriptor>(boost::format("%s"), -1);
		stack.push(pair);
		m_threads.push_back(stack);
	}

	void initialize_vertex(const Graph::vertex_descriptor &s, const Graph &g) const 
	{
		std::cout << "Initialize: " << s << std::endl;
	}

	void start_vertex(const Graph::vertex_descriptor &s, const Graph &g) const
	{
		std::cout << "Start: " << s << std::endl;
	}

	void discover_vertex(const Graph::vertex_descriptor &v, const Graph &g) 
	{
		using namespace boost;
		std::cout << "Discover: " << v << std::endl;
		m_history.push_front(v);
	}

	void examine_edge(const Graph::edge_descriptor &e, const Graph &g) const
	{
		std::cout << "Examine edge: " << e << std::endl;
	}

	void tree_edge(const Graph::edge_descriptor &e, const Graph &g) const
	{
		std::cout << "Tree edge: " << e << std::endl;
	}

	void back_edge(const Graph::edge_descriptor &e, const Graph &g)
	{
		using namespace boost;
		std::cout << "Back edge: " << e << std::endl;

		if (g[e.m_target]->GetType().ToString() == "DecisionNode")
		{
			m_currentBlocks.push(While);
			boost::format fmt("%s");
			m_currentThread++;
			if (m_threads.size() <= m_currentThread)
			{
				std::stack<std::pair<boost::format, Graph::vertex_descriptor>> stack;
				m_threads.push_back(stack);
			}

			m_threads[m_currentThread].push(std::pair<boost::format, Graph::vertex_descriptor>(fmt, e.m_source));

			auto findIter = std::find_if(m_history.begin(), m_history.end(),
				[&](Graph::vertex_descriptor v)
			{
				graph_traits<Graph>::out_edge_iterator ei, edge_end;
				for (tie(ei, edge_end) = out_edges(e.m_target, g); ei != edge_end; ++ei)
					if (v == target(*ei, g))
						return 1;
				return 0;
			}
			);
			m_blockDescriptors.push(std::pair<Graph::vertex_descriptor, Graph::vertex_descriptor>(e.m_target, *findIter));
		}
		else if (g[e.m_source]->GetType().ToString() == "DecisionNode")
		{
			m_currentBlocks.push(DoWhile);
			boost::format fmt("%s");
			m_threads[m_currentThread].push(std::pair<boost::format, Graph::vertex_descriptor>(fmt, e.m_source));
			m_blockDescriptors.push(std::pair<Graph::vertex_descriptor, Graph::vertex_descriptor>(e.m_target, e.m_source));
		}
		else
		{
			throw(new std::runtime_error("Back edge without DecisionNode!!!"));
		}
	}

	void forward_or_cross_edge(const Graph::edge_descriptor &e, const Graph &g)
	{
		std::cout << "Forward or cross edge: " << e << std::endl;

		if (m_currentBlocks.top() != IfElse)
			throw(new std::runtime_error("Forward or cross edge without IfElse!"));

		boost::format fmt("%s");
		m_currentThread++;
		if (m_threads.size() <= m_currentThread)
		{
			std::stack<std::pair<boost::format, Graph::vertex_descriptor>> stack;
			m_threads.push_back(stack);
		}

		m_threads[m_currentThread].push(std::pair<boost::format, Graph::vertex_descriptor>(fmt, e.m_source));
	}

	void finish_vertex(const Graph::vertex_descriptor &v, const Graph &g) 
	{
		using namespace boost;
		std::cout << "Finish vertex: " << v << std::endl;

		if (g[v]->GetType().ToString() == "Action")
		{
			BOOST_FOREACH(const Effect& effect, g[v]->GetEffects())
			{
				boost::format fmt("%s" + (m_threads[m_currentThread].top().first % "").str());
				fmt % (effect.GetBody() + "\n");

				const auto pair = std::pair<boost::format, Graph::vertex_descriptor>(boost::format("%s" + fmt.str()), v);

				m_threads[m_currentThread].top() = pair;
			}
		}

		if (!m_currentBlocks.empty() && (m_currentBlocks.top() == DoWhile))
		{
			if (m_blockDescriptors.top().first == v)
			{
				// Do {} while ($guard)
				boost::format fmt("do\n{\n%s}\nwhile(%s);\n%s");
				Guard guard = get(edge_all, g)[edge(m_blockDescriptors.top().second, v, g).first]->GetGuards()[0];
				const std::string loopBody = (PopFromStack(m_threads[m_currentThread]).first % "").str();
				const std::string loop = "%s" +
					(fmt % loopBody % guard.GetBody() % (PopFromStack(m_threads[m_currentThread]).first % "").str()).str();
				const auto pair = std::pair<boost::format, Graph::vertex_descriptor>(boost::format(loop), v);

				m_threads[m_currentThread].push(pair);
				m_blockDescriptors.pop();
				m_currentBlocks.pop();
				return;
			}
		}
		else if (!m_currentBlocks.empty() && (m_currentBlocks.top() == While))
		{
			if (m_blockDescriptors.top().second == v)
			{
				// While ($guard) { }
				boost::format fmt("while(%s)\n{\n%s}\n");
				Guard guard = get(edge_all, g)[edge(m_blockDescriptors.top().first, v, g).first]->GetGuards()[0];
				const std::string loop = (fmt % guard.GetBody() % (PopFromStack(m_threads[m_currentThread]).first % "").str()).str() + "%s";

				const auto pair = std::pair<boost::format, Graph::vertex_descriptor>(boost::format(loop), v);

				m_threads[m_currentThread].push(pair);
			}
		}

		if (g[v]->GetType().ToString() == "ActivityFinalNode")
		{
			if (out_degree(v, g))
				throw(new std::runtime_error("ActivityFinalNode must have no output edges!"));

			
			format fmt((m_threads[m_currentThread].top().first % "%sreturn 0;\n}\n").str());
			auto pair = std::pair<format, size_t>(fmt, v);
			m_threads[m_currentThread].top() = pair;

		}
		else if (g[v]->GetType().ToString() == "InitialNode")
		{
			if (in_degree(v, g))
				throw(new std::runtime_error("InitialNode must have no input edges!"));

			m_output = (m_threads[m_currentThread].top().first % "int main()\n{\n").str();
		}
		else if (g[v]->GetType().ToString() == "DecisionNode")
		{
			const size_t CountOfOutEdges = out_degree(v, g);
			if (CountOfOutEdges != 2)
				throw(new std::runtime_error("More than two output edges is not supported"));

			if (CountOfOutEdges == 1)
				throw(new std::runtime_error("Decision node must have at least two output edges!"));

			if (!m_currentBlocks.empty() && (m_currentBlocks.top() == While))
			{
				if (!m_blockDescriptors.empty() && (m_threads[m_currentThread].top().second == m_blockDescriptors.top().first))
				{
					const std::string notLoop = (m_threads[m_currentThread - 1].top().first % "").str();
					format fmt("%s" + (m_threads[m_currentThread].top().first % notLoop).str());
					m_currentThread--;
					auto pair = std::pair<format, size_t>(fmt, v);
					m_threads[m_currentThread].top() = pair;
					m_blockDescriptors.pop();
					m_currentBlocks.pop();
					return;
				}
				else if (!m_blockDescriptors.empty() && (m_threads[m_currentThread - 1].top().second == m_blockDescriptors.top().first))
				{
					const std::string notLoop = (m_threads[m_currentThread].top().first % "").str();
					format fmt("%s" + (m_threads[m_currentThread - 1].top().first % notLoop).str());
					m_currentThread--;
					auto pair = std::pair<format, size_t>(fmt, v);
					m_threads[m_currentThread].top() = pair;
					m_blockDescriptors.pop();
					m_currentBlocks.pop();
					return;
				}
			}
			else if (!m_currentBlocks.empty() && (m_currentBlocks.top() == IfElse))
			{
				const std::string Thread1 = (m_threads[m_currentThread].top().first % "").str();
				const Guard Guard1 = get(edge_all, g)[edge(v, m_threads[m_currentThread].top().second, g).first]->GetGuards()[0];

				const std::string Thread2 = (m_threads[m_currentThread - 1].top().first % "").str();
				const Guard Guard2 = get(edge_all, g)[edge(v, m_threads[m_currentThread - 1].top().second, g).first]->GetGuards()[0];

				format fmt("if (%s)\n{\n%s}\nelse if (%s)\n{\n%s}\n%s");
				m_threads[m_currentThread - 1].pop();
				const std::string Block = "%s" + (fmt % Guard1.GetBody() % Thread1 % Guard2.GetBody() % Thread2 % (m_threads[m_currentThread - 1].top().first % "").str()).str();
				m_threads[m_currentThread].pop();
				m_currentThread--;

				auto pair = std::pair<format, size_t>(boost::format(Block), v);
				m_threads[m_currentThread].top() = pair;
				m_blockDescriptors.pop();
				m_currentBlocks.pop();
			}
		}

		if (in_degree(v, g) == 2)
		{
			if (m_blockDescriptors.empty() || m_blockDescriptors.top().first != v)
			{
				m_currentBlocks.push(IfElse);

				boost::format fmt("%s");
				m_threads[m_currentThread].push(std::pair<boost::format, Graph::vertex_descriptor>(fmt, v));
				m_blockDescriptors.push(std::pair<Graph::vertex_descriptor, Graph::vertex_descriptor>(v, v));
			}
		}
	}

private:
	enum BlockType
	{
		DoWhile,
		While,
		IfElse
	};

	std::stack<BlockType> m_currentBlocks;
	std::stack<std::pair<Graph::vertex_descriptor, Graph::vertex_descriptor>> m_blockDescriptors;

	unsigned int m_currentThread;
	std::vector<std::stack<std::pair<boost::format, Graph::vertex_descriptor>>> m_threads;

	std::list<Graph::vertex_descriptor> m_history;

	std::string& m_output;
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
	boost::write_graphviz(std::cout, m_graph);//, boost::make_label_writer(labels));

	std::string code;
	DfsCodeGenerator visitor(code);

	boost::depth_first_search(m_graph, boost::visitor(visitor).root_vertex(initialNode));

	std::cout << code;
}