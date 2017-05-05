#pragma once

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/labeled_graph.hpp"

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
	std::shared_ptr<Node>, std::shared_ptr<Edge> >
	Graph;

class GraphConstructor
{
public:
	GraphConstructor(const Activity& activity) : m_activity(activity)
	{
		MakeGraph();
	}

	void MakeGraph();
	const Graph GetGraph() const;
	const std::string GraphViz() const;

private:
	const Activity m_activity;
	Graph m_graph;
};