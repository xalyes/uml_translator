#pragma once

#include "interfaces.h"

/*class NodeType : public IType
{
private:
	enum
	{
		DecisionNode,
		ActivityFinalNode,
		InitialNode,
		Action
	} m_type;

public:
	NodeType(const std::string& type)
	{
		if (type == "uml:DecisionNode")
			m_type = DecisionNode;
		else if (type == "uml:ActivityFinalNode")
			m_type = ActivityFinalNode;
		else if (type == "uml:InitialNode")
			m_type = InitialNode;
		else if (type == "uml:Action")
			m_type = Action;
		else
			throw(new std::runtime_error("Bad xmi:type of Node"));
	}

	std::string ToString() const
	{
		if (m_type == DecisionNode)
			return "xmi:type=\"uml:DecisionNode\"\n";
		else if (m_type == ActivityFinalNode)
			return "xmi:type=\"uml:ActivityFinalNode\"\n";
		else if (m_type == InitialNode)
			return "xmi:type=\"uml:InitialNode\"\n";
		else if (m_type == Action)
			return "xmi:type=\"uml:Action\"\n";
		else
			return "";
	}
};*/

class Node : public INumerableElement, public IVisibleElement
{
public:
	Node(const std::string& id, NodeType type, VisibilityType visibility)
		: INumerableElement(id), m_type(type), IVisibleElement(visibility)
	{}

	const std::string ToString() const
	{
		return
			"Node:\n"
			"\t" + m_type.ToString() +
			"\t" + m_id.ToString();
			/*"\t" + GetVisibilityString(m_visibility);*/
	}

	const NodeType GetType() const
	{
		return m_type;
	}

private:
	const NodeType m_type;
};

class Incoming : public IElement
{
public:
	Incoming(const std::string& id) : m_ref(id)
	{}

	const std::string ToString() const
	{
		return
			"Incoming:\n"
			"\t" + m_ref.ToString();
	}

private:
	const Id m_ref;
};

class Outgoing : public IElement
{
public:
	Outgoing(const std::string& id) : m_ref(id)
	{}

	const std::string ToString() const
	{
		return
			"Outgoing:\n"
			"\t" + m_ref.ToString();
	}

private:
	const Id m_ref;
};

/*class EffectType : public IType
{
private:
	enum
	{
		OpaqueBehavior
	} m_type;

public:
	EffectType(const std::string& type)
	{
		if (type == "uml:OpaqueBehavior")
			m_type = OpaqueBehavior;
		else
			throw(new std::runtime_error("Bad xmi:type of Effect"));
	}

	std::string ToString() const
	{
		if (m_type == OpaqueBehavior)
			return "xmi:type=\"uml:OpaqueBehavior\"\n";
		else
			return "";
	}
};*/

class Effect : public INumerableElement
{
public:
	Effect(const std::string& id, EffectType type, const std::string& body)
		: INumerableElement(id), m_type(type), m_body(body)
	{}

	const std::string ToString() const
	{
		return
			"Effect:\n"
			/*"\t" + m_type->ToString() +*/
			"\t" + m_id.ToString() +
			"\tBody: " + m_body;
	}

	const EffectType GetType() const
	{
		return m_type;
	}

private:
	const EffectType m_type;
	const std::string m_body;
};