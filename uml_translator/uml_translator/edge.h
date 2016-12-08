#pragma once

#include "element.h"

class EdgeTypes : public IType
{
public:
	enum
	{
		ControlFlow
	};
};

class Edge : public IElement
{
public:
	Edge(const std::string& id, EdgeTypes type, Visibility visibility)
		: IElement(id, type, visibility)
	{}

private:
	unsigned long source;
	unsigned long target;
};

class GuardTypes : public IType
{
public:
	enum
	{
		OpaqueExpression
	};
};

class Guard : public IElement
{
public:
	Guard(const std::string& id, GuardTypes type, Visibility visibility)
		: IElement(id, type, visibility)
	{}

private:
	std::string body;
};