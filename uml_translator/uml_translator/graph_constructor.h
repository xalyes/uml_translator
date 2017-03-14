#pragma once

#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/labeled_graph.hpp"

typedef boost::labeled_graph<boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
	std::shared_ptr<Node>, std::shared_ptr<Edge> >, unsigned long>
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