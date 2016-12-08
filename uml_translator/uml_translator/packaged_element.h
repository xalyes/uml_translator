#pragma once
#include <vector>
#include <memory>

#include "element.h"

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

class PackagedElement : public IElement
{
public:
	PackagedElement(const std::string& id, const PackagedElementTypeClass& type, Visibility visibility, const std::string& name)
		: IElement(id, type, visibility), m_name(name)
	{}

private:
	const std::string m_name;
};