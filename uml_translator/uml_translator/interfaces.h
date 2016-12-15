#pragma once
#include "id.h"
#include <string>

enum Visibility
{
	Public,
	Protected,
	Private
};

static std::string VisibilityStrings[] =
{
	"Public",
	"Protected",
	"Private"
};

std::string GetVisibilityString(Visibility vsbl)
{
	return "Visibility: " + VisibilityStrings[vsbl] + "\n";
}

class IType
{
public:
	IType() {};

	virtual std::string ToString() const
	{
		return "";
	};
};

class IElement
{
public:
	IElement() {}

	void AddChild(const std::shared_ptr<IElement> elem)
	{
		m_childs.push_back(elem);
	}

	const std::vector<std::shared_ptr<IElement> > GetChilds() const
	{
		return m_childs;
	}

	virtual const std::string ToString() const = 0;

private:
	std::vector<std::shared_ptr<IElement> > m_childs;
};

class ITypedElement : public virtual IElement
{
public:
	ITypedElement(const std::shared_ptr<IType> type)
		: m_type(type)
	{}

protected:
	const std::shared_ptr<IType> m_type;
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