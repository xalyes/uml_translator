#include "StdAfx.h"
#include "node.h"
#include "edge.h"
#include "graph_constructor.h"

#include "boost/program_options.hpp"

using boost::property_tree::ptree;

class Parser
{
public:
	Parser(const boost::property_tree::ptree& tree) : m_root(tree)
	{}

	Activity Parse()
	{
		boost::property_tree::ptree PackageTree = m_root.get_child("xmi:XMI.uml:Model.packagedElement");

		// iterate all diagram
		BOOST_FOREACH(const ptree::value_type& diagramTree, PackageTree.get_child(""))
		{
			if (diagramTree.first != "<xmlattr>")
			{
				// iterate nodes or edges
				BOOST_FOREACH(const ptree::value_type& elem, diagramTree.second.get_child(""))
				{
					if (elem.first != "<xmlattr>")
					{
						if (elem.first == "node")
						{
							Node node = FillNode(elem.second);
							m_nodes.push_back(node);
						}
						else if (elem.first == "edge")
						{
							Edge edge = FillEdge(elem.second);
							m_edges.push_back(edge);
						}
						else
							throw(new std::runtime_error("Element " + elem.first + " is not supported. Please call me (no)"));
					}
				}
			}
		}

		return GetActivity();
	}

	std::vector<Node> GetNodes()
	{
		return m_nodes;
	}

	std::vector<Edge> GetEdges()
	{
		return m_edges;
	}

	Activity GetActivity()
	{
		static Activity activity = std::make_tuple(m_nodes, m_edges);
		return activity;
	}

private:
	Node FillNode(const ptree& pt)
	{
		const std::string Id = pt.get<std::string>("<xmlattr>.xmi:id");
		const std::string StrType = pt.get<std::string>("<xmlattr>.xmi:type");
		const std::string StrVisibility = pt.get<std::string>("<xmlattr>.visibility");
		std::cout << StrType;
		const NodeType Type(StrType);

		const VisibilityType visibility(StrVisibility);

		Node node(Id, Type, visibility);
		std::cout << node.ToString();

		BOOST_FOREACH(const ptree::value_type& elem, pt.get_child(""))
		{
			if (elem.first != "<xmlattr>")
			{
				if (elem.first == "incoming")
					node.AddIncoming(Incoming(elem.second.get<std::string>("<xmlattr>.xmi:idref")));
				else if (elem.first == "outgoing")
					node.AddOutgoing(Outgoing(elem.second.get<std::string>("<xmlattr>.xmi:idref")));
				else if (elem.first == "effect")
				{
					const std::string StrType = elem.second.get<std::string>("<xmlattr>.xmi:type");
					const std::string Id = elem.second.get<std::string>("<xmlattr>.xmi:id");
					const std::string Body = elem.second.get<std::string>("<xmlattr>.body");
					Effect effect = Effect(Id, EffectType(StrType), Body);
					std::cout << effect.ToString();
					node.AddEffect(effect);
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

		const VisibilityType visibility(StrVisibility);

		Edge edge = Edge(Id, Type, visibility, Source, Target);
		std::cout << edge.ToString();

		const boost::optional<std::string> GuardId = pt.get_optional<std::string>("guard.<xmlattr>.xmi:id");

		const boost::optional<std::string> Body = pt.get_optional<std::string>("guard.<xmlattr>.body");

		const boost::optional<std::string> GuardStrType = pt.get_optional<std::string>("guard.<xmlattr>.xmi:type");
		if (GuardStrType.is_initialized())
		{
			const GuardType guardType(GuardStrType.get());
			edge.AddGuard(Guard(GuardId.get(), guardType, Body.get()));
		}

		return edge;
	}

	boost::property_tree::ptree m_root;
	std::vector<Node> m_nodes;
	std::vector<Edge> m_edges;
};

int main(int argc, char** argv)
{
	std::cout << "UML translator App" << std::endl;
	try
	{
		/** Define and parse the program options*/

		namespace po = boost::program_options;
		po::options_description desc("Options");
		desc.add_options()
			("help,h", "Print help messages")
			("path,p", po::value<std::string>()->required(), "path to XMI-file");

		po::variables_map vm;
		try
		{
			po::store(po::parse_command_line(argc, argv, desc), vm); // can throw

			/* --help option*/
			if (vm.count("help"))
			{
				std::cout << desc << std::endl;
				return 0;
			}

			po::notify(vm); // throws on error, so do after help in case 
							// there are any problems 
		}
		catch (po::error& e)
		{
			std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
			std::cerr << desc << std::endl;
			return -1;
		}

		if (vm.count("path"))
		{
			ptree pt;
			std::ifstream input(vm["path"].as<std::string>());
			read_xml(input, pt);

			Parser parser(pt);
			const Activity ActivityDiagram = parser.Parse();

			GraphConstructor graphConstructor(ActivityDiagram);
			std::cin.ignore();
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Unhandled Exception reached the top of main: "
			<< e.what() << ", application will now exit" << std::endl;
		return -2;

	}

	return 0; 
}