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

	virtual std::string ToString() const
	{
		if (m_type == Package)
			return "xmi:type=\"uml:Package\"\n";
		else if (m_type == Activity)
			return "xmi:type=\"uml:Activity\"\n";
		else if (m_type == Class)
			return "xmi:type=\"uml:Class\"\n";
		else
			return "";
	}
};

class PackagedElement : public INumerableElement, public ITypedElement, public IVisibleElement
{
public:
	PackagedElement(const std::string& id, const PackagedElementTypeClass& type, Visibility visibility, const std::string& name)
		: INumerableElement(id), ITypedElement(std::make_shared<PackagedElementTypeClass>(type)), IVisibleElement(visibility), m_name(name)
	{}

	const std::string ToString() const
	{
		return
			"Packaged element:\n"
			"\t" + m_type->ToString() +
			"\t" + m_id.ToString() +
			"\t" + GetVisibilityString(m_visibility) +
			"\t" + "name: " + m_name + "\n";
	}

private:
	const std::string m_name;
};