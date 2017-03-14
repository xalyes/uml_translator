#include <iostream>
#include <string>
#include <fstream>
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <locale>
#include <unordered_map>
#include <list>

#include "uml_enums.h"
#include "node.h"
#include "edge.h"

typedef std::tuple<std::vector<Node>, std::vector<Edge>> Activity;
