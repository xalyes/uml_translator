#pragma once
#include "id.h"
#include <string>

enum Visibility
{
	Public,
	Protected,
	Private
};

class IType
{
public:
	IType() {};
};

class IElement
{
public:
	IElement(const std::string& id, const IType& type, Visibility visibility)
		: m_id(id), m_type(type), m_visibility(visibility)
	{}

protected:
	const Id m_id;
	const IType m_type;
	const Visibility m_visibility;
	std::vector<std::shared_ptr<IElement> > m_childs;
};