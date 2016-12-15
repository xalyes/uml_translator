#include "StdAfx.h"
#include "packaged_element.h"
#include "node.h"
#include "edge.h"

using boost::property_tree::ptree;

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

class Parser
{
public:
	Parser(const boost::property_tree::ptree& tree) : m_root(tree)
	{}

	PackagedElement Parse()
	{
		boost::property_tree::ptree PackageTree = m_root.get_child("xmi:XMI.uml:Model.packagedElement");
		PackagedElement Package = FillPackage(PackageTree);
		std::cout << Package.ToString();

		// iterate all diagram
		BOOST_FOREACH(const ptree::value_type& diagramTree, PackageTree.get_child(""))
		{
			if (diagramTree.first != "<xmlattr>")
			{
				PackagedElement diagram = FillPackage(diagramTree.second);
				std::cout << diagram.ToString();

				// iterate nodes or edges
				BOOST_FOREACH(const ptree::value_type& elem, diagramTree.second.get_child(""))
				{
					if (elem.first != "<xmlattr>")
					{
						if (elem.first == "node")
							diagram.AddChild(std::make_shared<Node>(FillNode(elem.second)));
						else if (elem.first == "edge")
							diagram.AddChild(std::make_shared<Edge>(FillEdge(elem.second)));
						else
							throw(new std::runtime_error("Element " + elem.first + " is not supported. Please call me (no)"));
					}
				}

				Package.AddChild(std::make_shared<PackagedElement>(diagram));
			}
		}

		return Package;
	}

private:
	PackagedElement FillPackage(const ptree& pt)
	{
		const std::string Id = pt.get<std::string>("<xmlattr>.xmi:id");
		const std::string StrType = pt.get<std::string>("<xmlattr>.xmi:type");
		const std::string StrVisibility = pt.get<std::string>("<xmlattr>.visibility");
		const std::string Name = pt.get<std::string>("<xmlattr>.name");

		const PackagedElementTypeClass Type(StrType);
		const Visibility visibility = GetVisibility(StrVisibility);

		return PackagedElement(Id, Type, visibility, Name);
	}

	Node FillNode(const ptree& pt)
	{
		const std::string Id = pt.get<std::string>("<xmlattr>.xmi:id");
		const std::string StrType = pt.get<std::string>("<xmlattr>.xmi:type");
		const std::string StrVisibility = pt.get<std::string>("<xmlattr>.visibility");

		const NodeType Type(StrType);

		const Visibility visibility = GetVisibility(StrVisibility);

		Node node(Id, Type, visibility);
		std::cout << node.ToString();

		BOOST_FOREACH(const ptree::value_type& elem, pt.get_child(""))
		{
			if (elem.first != "<xmlattr>")
			{
				if (elem.first == "incoming")
					node.AddChild(std::make_shared<Incoming>(Incoming(elem.second.get<std::string>("<xmlattr>.xmi:idref"))));
				else if (elem.first == "outgoing")
					node.AddChild(std::make_shared<Outgoing>(Outgoing(elem.second.get<std::string>("<xmlattr>.xmi:idref"))));
				else if (elem.first == "effect")
				{
					const std::string StrType = elem.second.get<std::string>("<xmlattr>.xmi:type");
					const std::string Id = elem.second.get<std::string>("<xmlattr>.xmi:id");
					const std::string Body = elem.second.get<std::string>("<xmlattr>.body");
					node.AddChild(std::make_shared<Effect>(Effect(Id, EffectType(StrType), Body)));
				}
				else
					throw(new std::runtime_error("Element " + elem.first + " is not supported. Please call me (no)"));
			}
		}

		return node;
	}

	Edge FillEdge(const ptree& pt)
	{
		const std::string Id = pt.get<std::string>("<xmlattr>.xmi:id");
		const std::string StrType = pt.get<std::string>("<xmlattr>.xmi:type");
		const std::string StrVisibility = pt.get<std::string>("<xmlattr>.visibility");
		const std::string Source = pt.get<std::string>("<xmlattr>.source");
		const std::string Target = pt.get<std::string>("<xmlattr>.target");

		const EdgeType Type(StrType);

		const Visibility visibility = GetVisibility(StrVisibility);

		Edge edge = Edge(Id, Type, visibility, Source, Target);
		std::cout << edge.ToString();

		const boost::optional<std::string> GuardId = pt.get_optional<std::string>("guard.xmi:id");

		const boost::optional<std::string> Body = pt.get_optional<std::string>("guard.body");

		const boost::optional<std::string> GuardStrType = pt.get_optional<std::string>("guard.xmi:type");
		if (GuardStrType.is_initialized())
		{
			const GuardType guardType(GuardStrType.get());
			edge.AddChild(std::make_shared<Guard>(Guard(GuardId.get(), guardType, Body.get())));
		}

		return edge;
	}

	boost::property_tree::ptree m_root;
};

int main(void)  
{
	ptree pt;
	std::ifstream input("fact.xml");
	read_xml(input, pt);

	Parser parser(pt);
	const PackagedElement package = parser.Parse();

	std::cout << "===========" << std::endl;
	const std::vector<std::shared_ptr<IElement> > elements = package.GetChilds();
	std::cout << elements[0]->ToString() << std::endl;

	return 0; 
}