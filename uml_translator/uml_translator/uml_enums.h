#pragma once

#include <boost/preprocessor.hpp>

#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data,  elem) \
    case elem : return BOOST_PP_STRINGIZE(elem);

#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOENUM_IF(r, data, elem) \
    if (BOOST_PP_SEQ_TAIL(data) ==                                     \
            "uml:" BOOST_PP_STRINGIZE(elem)) m_type =                            \
            elem; else

#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)         \
class name {                       \
private:                               \
    enum {                                                  \
        BOOST_PP_SEQ_ENUM(enumerators)                                 \
    } m_type;    \
                                         \
public:                                                                                         \
                                                                       \
    std::string ToString() const                          \
    {                                                                  \
        switch (m_type)                                                     \
        {                                                              \
            BOOST_PP_SEQ_FOR_EACH(                                     \
                X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE,   \
                name,                                                  \
                enumerators                                            \
            )                                                          \
            default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";  \
        }                                                              \
    }                                                                  \
                                                                       \
    name(const std::string& s)                                   \
    {                                                                  \
        BOOST_PP_SEQ_FOR_EACH(                                         \
                X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOENUM_IF,       \
                (name)(s),                                             \
                enumerators                                            \
            )                                                          \
        throw(new std::runtime_error("Bad xmi:type of " BOOST_PP_STRINGIZE(name)));                                           \
    } \
};

//DEFINE_ENUM_WITH_STRING_CONVERSIONS(OS_type, (Linux)(Apple)(Windows));
/*DEFINE_ENUM_CLASS_WITH_ToString_METHOD(Elements,
(Element1)
(Element2, "string representation for Element2 ")
(Element3, "Element3 string representation", 1000)
(Element4, "Element 4 string repr")
(Element5, "Element5", 1005)
(Element6, "Element6 ")
)*/
// Generates the following:
//      enum class Elements {
//          Element1, Element2, Element3 = 1000, Element4, Element5 = 1005, Element6,
//      };
//      inline const char* ToString(const Elements element) {
//          switch (element) {
//              case Elements::Element1: return "Element1";
//              case Elements::Element2: return "string representation for Element2 ";
//              case Elements::Element3: return "Element3 string representation";
//              case Elements::Element4: return "Element 4 string repr";
//              case Elements::Element5: return "Element5";
//              case Elements::Element6: return "Element6 ";
//              default: return "[Unknown " "Elements" "]";
//          }
//      }

DEFINE_ENUM_WITH_STRING_CONVERSIONS(EdgeType, (ControlFlow))

DEFINE_ENUM_WITH_STRING_CONVERSIONS(GuardType, (OpaqueExpression))

DEFINE_ENUM_WITH_STRING_CONVERSIONS(NodeType,
	(DecisionNode)
	(ActivityFinalNode)
	(InitialNode)
	(Action)
)

DEFINE_ENUM_WITH_STRING_CONVERSIONS(EffectType, (OpaqueBehavior))

DEFINE_ENUM_WITH_STRING_CONVERSIONS(PackagedElementType,
	(Package)
	(Activity)
	(Class)
)

class VisibilityType
{
private:
	enum
	{
		Public,
		Protected,
		Private
	} m_type;

public:
	VisibilityType(const std::string& type)
	{
		if (type == "public")
			m_type = Public;
		else if (type == "protected")
			m_type = Protected;
		else if (type == "private")
			m_type = Private;
		else
			throw(new std::runtime_error("Bad xmi:type of visibility"));
	}

	std::string ToString() const
	{
		if (m_type == Public)
			return "public";
		else if (m_type == Protected)
			return "protected";
		else if (m_type == Private)
			return "private";
		else
			return "";
	}
};