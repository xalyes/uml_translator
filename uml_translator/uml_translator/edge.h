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

	std::string ToString() const
	{
		if (m_type == ControlFlow)
			return "xmi:type=\"uml:ControlFlow\"\n";
		else
			return "";
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
		ITypedElement(std::make_shared<EdgeType>(type)),
		IVisibleElement(visibility), 
		m_source(source),
		m_target(target)
	{}

	const std::string ToString() const
	{
		return
			"Edge:\n"
			"\t" + m_type->ToString() +
			"\t" + m_id.ToString() +
			"\t" + GetVisibilityString(m_visibility) +
			"\t" + "source: " + m_source.ToString() +
			"\t" + "target: " + m_target.ToString();
	}

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

	std::string ToString() const
	{
		if (m_type == OpaqueExpression)
			return "xmi:type=\"uml:OpaqueExpression\"\n";
		else
			return "";
	}
};

class Guard : public INumerableElement, public ITypedElement
{
public:
	Guard(const std::string& id, GuardType type, const std::string& body)
		: INumerableElement(id), ITypedElement(std::make_shared<IType>(type)), m_body(body)
	{}

	const std::string ToString() const
	{
		return
			"Guard:\n"
			"\t" + m_type->ToString() +
			"\t" + m_id.ToString() +
			"\t" + m_body;
	}

private:
	std::string m_body;
};