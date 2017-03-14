#pragma once

#include "interfaces.h"

class Edge : public INumerableElement, public IVisibleElement
{
public:
	Edge(
		const std::string& id,
		const EdgeType& type,
		VisibilityType visibility,
		const std::string& source,
		const std::string& target
	)
		: INumerableElement(id),
		m_type(type),
		IVisibleElement(visibility), 
		m_source(source),
		m_target(target)
	{}

	const std::string ToString() const;

	const Id GetSource() const;
	const Id GetTarget() const;

	const EdgeType GetType() const;

private:
	const Id m_source;
	const Id m_target;
	const EdgeType m_type;
};

class Guard : public INumerableElement
{
public:
	Guard(const std::string& id, GuardType type, const std::string& body)
		: INumerableElement(id), m_type(type), m_body(body)
	{}

	const std::string ToString() const;

private:
	const std::string m_body;
	const GuardType m_type;
};