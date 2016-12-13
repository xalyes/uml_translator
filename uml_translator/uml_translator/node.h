#pragma once

#include "interfaces.h"

class NodeType : public IType
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
};

class Node : public INumerableElement, public ITypedElement, public IVisibleElement
{
public:
	Node(const std::string& id, const NodeType& type, Visibility visibility)
		: INumerableElement(id), ITypedElement(type), IVisibleElement(visibility)
	{}
};

class Incoming : public IElement
{
public:
	Incoming(const std::string& id) : m_ref(id)
	{}

private:
	const Id m_ref;
};

class Outgoing : public IElement
{
public:
	Outgoing(const std::string& id) : m_ref(id)
	{}

private:
	const Id m_ref;
};

class EffectType : public IType
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
};

class Effect : public INumerableElement, public ITypedElement
{
public:
	Effect(const std::string& id, EffectType type, const std::string& body)
		: INumerableElement(id), ITypedElement(type), m_body(body)
	{}

public:
	const std::string m_body;
};