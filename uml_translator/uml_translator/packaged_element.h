#pragma once
#include <vector>
#include <memory>

#include "interfaces.h"

class PackagedElementTypeClass : public IType
{
private:
	enum
	{
		Package,
		Activity,
		Class
	} m_type;

public:
	PackagedElementTypeClass(const std::string& type)
	{
		if (type == "uml:Package")
			m_type = Package;
		else if (type == "uml:Activity")
			m_type = Activity;
		else if (type == "uml:Class")
			m_type = Class;
		else
			throw(new std::runtime_error("Bad xmi:type"));
	}
};

class PackagedElement : public INumerableElement, public ITypedElement, public IVisibleElement
{
public:
	PackagedElement(const std::string& id, const PackagedElementTypeClass& type, Visibility visibility, const std::string& name)
		: INumerableElement(id), ITypedElement(type), IVisibleElement(visibility), m_name(name)
	{}

private:
	const std::string m_name;
};