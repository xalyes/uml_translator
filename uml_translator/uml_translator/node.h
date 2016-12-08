#pragma once

#include "element.h"

class NodeTypes : public IType
{
public:
	enum
	{
		DecisionNode,
		ActivityFinalNode,
		InitialNode,
		Action
	};
};

class Node : public IElement
{
public:
	Node(const std::string& id, NodeTypes type, Visibility visibility)
		: IElement(id, type, visibility)
	{}

private:
	std::string m_name;
};

class Incoming
{
private:
	unsigned long m_ref;
};

class Outgoing
{
private:
	unsigned long m_ref;
};

class EffectTypes : public IType
{
public:
	enum
	{
		OpaqueBehavior
	};
};

class Effect : public IElement
{
public:
	Effect(const std::string& id, EffectTypes type, Visibility visibility)
		: IElement(id, type, visibility)
	{}

public:
	std::string body;
};