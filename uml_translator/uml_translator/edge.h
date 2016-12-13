#pragma once

#include "interfaces.h"

class EdgeType : public IType
{
private:
	enum
	{
		ControlFlow
	} m_type;

public:
	EdgeType(const std::string& type)
	{
		if (type == "uml:ControlFlow")
			m_type = ControlFlow;
		else
			throw(new std::runtime_error("Bad xmi:type of Edge"));
	}
};

class Edge : public INumerableElement, public ITypedElement, public IVisibleElement
{
public:
	Edge(
		const std::string& id,
		const EdgeType& type,
		Visibility visibility,
		const std::string& source,
		const std::string& target
	)
		: INumerableElement(id),
		ITypedElement(type),
		IVisibleElement(visibility), 
		m_source(source),
		m_target(target)
	{}

private:
	const Id m_source;
	const Id m_target;
};

class GuardType : public IType
{
private:
	enum
	{
		OpaqueExpression
	} m_type;

public:
	GuardType(const std::string& type)
	{
		if (type == "uml:OpaqueExpression")
			m_type = OpaqueExpression;
		else
			throw(new std::runtime_error("Bad xmi:type of Guard"));
	}
};

class Guard : public INumerableElement, public ITypedElement
{
public:
	Guard(const std::string& id, GuardType type, const std::string& body)
		: INumerableElement(id), ITypedElement(type), m_body(body)
	{}

private:
	std::string m_body;
};