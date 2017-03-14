#include "StdAfx.h"

#include <iostream>
#include <deque>

#include <boost/graph/graphviz.hpp>

#include "graph_constructor.h"
#include "node.h"
#include "edge.h"

void GraphConstructor::MakeGraph()
{
	unsigned long initialNode = -1;

	BOOST_FOREACH(const Node& vertex, std::get<0>(m_activity))
	{
		const unsigned long LocalId = vertex.GetLocalId();
		boost::add_vertex(LocalId, m_graph);
		m_graph[LocalId] = std::make_shared<Node>(vertex);
		std::cout << m_graph[LocalId]->ToString() << std::endl;
		if (m_graph[LocalId]->ToString() == "xmi:type=\"uml:InitialNode\"\n")
		{
			initialNode = LocalId;
		}
	}

	BOOST_FOREACH(const Edge edge, std::get<1>(m_activity))
	{
		boost::add_edge_by_label(edge.GetSource().GetLocalId(), edge.GetTarget().GetLocalId(), m_graph);
	}

	boost::write_graphviz(std::cout, m_graph);
}
