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
	IElement() {}

	void AddChild(const IElement& elem)
	{
		m_childs.push_back(std::make_shared<IElement>(elem));
	}

private:
	std::vector<std::shared_ptr<IElement> > m_childs;
};

class ITypedElement : public virtual IElement
{
public:
	ITypedElement(const IType& type)
		: m_type(type)
	{}

protected:
	const IType m_type;
};

class INumerableElement : public virtual IElement
{
public:
	INumerableElement(const std::string& id)
		: m_id(id)
	{}

protected:
	const Id m_id;
};

class IVisibleElement : public virtual IElement
{
public:
	IVisibleElement(Visibility visibility)
		: m_visibility(visibility)
	{}

protected:
	const Visibility m_visibility;
};