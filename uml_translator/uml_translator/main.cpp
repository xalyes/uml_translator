#include "StdAfx.h"
#include "structures.h"
#include "packaged_element.h"

using boost::property_tree::ptree;  
using namespace std;

list<structures::Class> classes;
list<structures::Function> functions;
  
class configuration 
{
private:
	ptree pt;
public:
	configuration(const string& filename)
	{
		load(filename);
	};

	ptree load(const string& filename) 
	{
		return import_xml(filename);
	};
	
	void save(const string& filename)
	{
		export_xml(filename,pt);
	};

	ptree property_tree(void)
	{
		return pt;
	};

	/*void display(void)
	{
		//display(0, pt);
		print_tree(pt, 0);
	};*/

private:  
	ptree import_xml(const string& filename) 
	{
		ifstream input(filename.c_str());
		read_xml(input, pt);
		return pt;
	};

	void export_xml(const string& filename, const ptree& pt)
	{
	  //boost::property_tree::xml_parser::xml_writer_settings<char> w(' ',2);  
	  boost::property_tree::write_xml(filename, pt, locale()); 
	};

	std::string q(const std::string& s) { return "\"" + s + "\""; }

	/*void fillAttribute(const ptree& pt, structures::Class& newClass)
	{
		structures::Properties props;
		string name = pt.get<string>("<xmlattr>.name");
		props.m_type = pt.get<string>("<xmlattr>.type");
		props.m_visibility = pt.get<string>("<xmlattr>.visibility");

		newClass.m_members.insert(std::make_pair<string, structures::Properties>(name, props));
	}*/

	/*void fillFunction(const ptree& pt, structures::Class& newClass)
	{
		static string type;
		structures::Function func;
		func.m_id = pt.get<string>(ptree::path_type("<xmlattr>/xmi.id", '/'));
		func.m_name = pt.get<string>("<xmlattr>.name");

		unordered_map<string, structures::Properties> props;
		BOOST_FOREACH(const ptree::value_type &v, pt.get_child(ptree::path_type("UML:BehavioralFeature.parameter", '/'))) 
		{
			type = fillParameters(v.second, props);
		}
		func.m_type = type;
		func.m_params = props;
		newClass.m_functions.push_back(func);
	}*/

	/*string fillParameters(const ptree& pt, unordered_map<string, structures::Properties>& props)
	{
		string return_type;
		BOOST_FOREACH(const ptree::value_type &v, pt) 
		{
			string name, kind, type;
			fillParameter(v.second, name, kind, type);
			structures::Properties prop;
			prop.m_type = type;
			if (kind == "return")
				return_type = type;
			else
				props.insert(make_pair<string, structures::Properties>(name, prop));
		}
		return return_type;
	}*/

	/*void fillParameter(const ptree& pt, string& name, string& kind, string& type)
	{
		name = pt.get<string>("name");
		kind = pt.get<string>("kind");
		type = pt.get<string>("type");
	}*/

	/*void fillClass(const ptree& pt, structures::Class& newClass)
	{
		newClass.m_id = pt.get<string>(ptree::path_type("<xmlattr>/xmi.id", '/'));
		newClass.m_name = pt.get<string>("<xmlattr>.name");

		BOOST_FOREACH(const ptree::value_type &v, pt.get_child(ptree::path_type("UML:Classifier.feature", '/'))) 
		{
			if (v.first == "UML:Attribute")
				fillAttribute(v.second, newClass);
			if (v.first == "UML:Operation")
				fillFunction(v.second, newClass);
		}

	}*/

	/*void print_tree(const ptree& pt, int level)
	{
		const std::string sep(2 * level, ' ');
		BOOST_FOREACH(const ptree::value_type &v, pt)
		{
			if (v.first == "UML:Class")
			{
				structures::Class *newClass = new structures::Class();
				fillClass(v.second, *newClass);
			}
			std::cout
				<< sep
				<< q(v.first) << " : " << q(v.second.data()) << "\n";
			print_tree(v.second, level + 1);
		}
	}*/

	/*void display(const int depth, const ptree& tree)
	{
		BOOST_FOREACH( ptree::value_type const&v, tree.get_child("") )
		{
			//if (v.first != "<xmlattr>")
			//{
				ptree subtree = v.second;
				string nodestr = tree.get<string>(v.first);
				cout << string("").assign(depth*2,' ') << "* ";
				cout << v.first;
				if ( nodestr.length() > 0 )
					cout << "=\"" << tree.get<string>(v.first) << "\"";
				cout << endl;
				display(depth+1,subtree);
			//}
		}
	}*/
};

#define DEFAULT_CALORIES 0

Visibility GetVisibility(const std::string& visibility)
{
	if (visibility == "public")
		return Public;
	else if (visibility == "private")
		return Private;
	else if (visibility == "protected")
		return Protected;
	else
		throw(new std::runtime_error("Bad visibility"));
}

PackagedElement FillPackage(const ptree& pt)
{
	const std::string Id = pt.get<std::string>("packagedElement.<xmlattr>.xmi:id");
	std::cout << "Id: " << Id << std::endl;
	const std::string StrType = pt.get<std::string>("packagedElement.<xmlattr>.xmi:type");
	std::cout << "Type: " << StrType << std::endl;
	const std::string StrVisibility = pt.get<std::string>("packagedElement.<xmlattr>.visibility");
	std::cout << "Visibility: " << StrVisibility << std::endl;
	const std::string Name = pt.get<std::string>("packagedElement.<xmlattr>.name");
	std::cout << "Name: " << Name << std::endl;

	const PackagedElementTypeClass Type(StrType);

	const Visibility Visibl = GetVisibility(StrVisibility);

	return PackagedElement(Id, Type, Visibl, Name);
}

int main(void)  
{
	configuration cfg("fact.xml");  
	const PackagedElement Package = FillPackage(cfg.property_tree().get_child("xmi:XMI.uml:Model"));

	return 0; 
}