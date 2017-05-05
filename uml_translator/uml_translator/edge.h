#pragma once

#include "interfaces.h"

class Guard : public INumerableElement
{
public:
	Guard(const std::string& id, GuardType type, const std::string& body)
		: INumerableElement(id), m_type(type), m_body(body)
	{}

	const std::string ToString() const;
	Guard& operator=(const Guard&);

	const std::string GetBody() const { return m_body; }

private:
	const std::string m_body;
	const GuardType m_type;
};

class Edge : public INumerableElement, public IVisibleElement
{
public:
	Edge(
		const std::string& id,
		const EdgeType& type,
		VisibilityType visibility,
		const std::string& source,
		const std::string& target,
		const std::vector<Guard>& guards = std::vector<Guard>()
	)
		: INumerableElement(id),
		m_type(type),
		IVisibleElement(visibility), 
		m_source(source),
		m_target(target),
		m_guards(guards)
	{}

	void AddGuard(const Guard& guard);

	const std::string ToString() const;

	const Id GetSource() const;
	const Id GetTarget() const;

	const EdgeType GetType() const;

	std::vector<Guard> GetGuards() const { return m_guards; }
	void SetGuards(std::vector<Guard> val) { m_guards = val; }

private:
	const Id m_source;
	const Id m_target;
	const EdgeType m_type;
	std::vector<Guard> m_guards;
};