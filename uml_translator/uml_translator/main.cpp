#include "StdAfx.h"
#include "structures.h"

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

	void display(void)
	{
		//display(0, pt);
		print_tree(pt, 0);
	};

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

	void fillAttribute(const ptree& pt, structures::Class& newClass)
	{
		string name;
		structures::Properties props;
		BOOST_FOREACH(const ptree::value_type &v, pt) 
		{
			if (v.first == "name")
				name = v.second.get<string>("");
			if (v.first == "visibility")
				props.m_visibility = v.second.get<string>("");
			if (v.first == "type")
				props.m_type = v.second.get<string>("");
			if (v.first == "<xmlattr>")
				fillAttribute(v.second, newClass);
		}
		newClass.m_members.insert(std::make_pair<string, structures::Properties>(name, props));
	}

	void fillFunction(const ptree& pt, structures::Class& newClass)
	{
		static string name;
		static string type;
		structures::Function func;
		unordered_map<string, structures::Properties> props;
		BOOST_FOREACH(const ptree::value_type &v, pt) 
		{
			if (v.first == "name")
				name = v.second.get<string>("");
			if (v.first == "xmi.id")
				func.m_id = v.second.get<string>("");
			if (v.first == "UML:BehavioralFeature.parameter")
				type = fillParameters(v.second, props);
			if (v.first == "<xmlattr>")
				fillFunction(v.second, newClass);
		}
		func.m_name = name;
		func.m_type = type;
		func.m_params = props;
		newClass.m_functions.push_back(func);
	}

	string fillParameters(const ptree& pt, unordered_map<string, structures::Properties>& props)
	{
		string return_type;
		BOOST_FOREACH(const ptree::value_type &v, pt) 
		{
			string name, kind, type;
			if (v.first == "<xmlattr>")
				return_type = fillParameters(v.second, props);
			if (v.first == "UML:Parameter")
			{
				fillParameter(v.second, name, kind, type);
				structures::Properties prop;
				prop.m_type = type;
				if (kind == "return")
					return_type = type;
				else
					props.insert(make_pair<string, structures::Properties>(name, prop));
			}
		}
		return return_type;
	}

	void fillParameter(const ptree& pt, string& name, string& kind, string& type)
	{
		BOOST_FOREACH(const ptree::value_type &v, pt) 
		{
			if (v.first == "<xmlattr>")
				fillParameter(v.second, name, kind, type);
			if (v.first == "name")
				name = v.second.get<string>("");
			if (v.first == "kind")
				kind = v.second.get<string>("");
			if (v.first == "type")
				type = v.second.get<string>("");
		}
	}

	void fillClass(const ptree& pt, structures::Class& newClass)
	{
		BOOST_FOREACH(const ptree::value_type &v, pt) 
		{
			if (v.first == "xmi.id")
				newClass.m_id = v.second.get<string>("");
			if (v.first == "name")
				newClass.m_name = v.second.get<string>("");
			if (v.first == "UML:Attribute")
				fillAttribute(v.second, newClass);
			if (v.first == "UML:Operation")
				fillFunction(v.second, newClass);
			fillClass(v.second, newClass);
		}

	}

	void print_tree(const ptree& pt, int level)
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
	}

	void display(const int depth, const ptree& tree)
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
	};  
};  

#define DEFAULT_CALORIES 0  
   
int main(void)  
{  
	configuration cfg("Untitled.xml");  
	cfg.display();  
	//cfg.save("test_output.xml");  
	  
	// example to grab data from property tree  
	int calories = cfg.property_tree().get<int>("collection.recipe.nutrition.<xmlattr>.calories",DEFAULT_CALORIES);  
	cout << "Calories = " << calories << endl;  
	 
	return 0; 
}  