#include "StdAfx.h"
using namespace std;

namespace structures
{

class UmlObject
{
public:
	string m_id;
};

enum Modifier
{
	CONST
};

class Properties
{
public:
	string m_type;
	string m_visibility;
	vector<Modifier> m_modifiers;
};

class Function : public UmlObject
{
public:
	string m_name;
	string m_type;
	unordered_map<string, Properties> m_params; // first - arg name
};

class Class : public UmlObject
{
public:
	string m_name;
	vector<Function> m_functions;
	unordered_map<string, Properties> m_members; // first - member name
};

}