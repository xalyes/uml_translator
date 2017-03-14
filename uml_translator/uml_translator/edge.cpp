#include "StdAfx.h"
#include "edge.h"

const std::string Edge::ToString() const
{
	return
		"Edge:\n"
		"\t" + m_type.ToString() +
		"\t" + m_id.ToString() +
		"\t" + m_visibility.ToString() +
		"\t" + "source: " + m_source.ToString() +
		"\t" + "target: " + m_target.ToString();
}

const Id Edge::GetSource() const
{
	return m_source;
}

const Id Edge::GetTarget() const
{
	return m_target;
}

const EdgeType Edge::GetType() const
{
	return m_type;
}

const std::string Guard::ToString() const
{
	return
		"Guard:\n"
		"\t" + m_type.ToString() +
		"\t" + m_id.ToString() +
		"\t" + m_body;
}