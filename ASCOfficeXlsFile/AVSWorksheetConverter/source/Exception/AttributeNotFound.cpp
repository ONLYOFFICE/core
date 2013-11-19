#include "stdafx.h"
#include "AttributeNotFound.h"


namespace EXCEPT
{;
namespace LE
{;

AttributeNotFound::AttributeNotFound(const std::string& attrib_name, const std::string& tag_name)
	: WrongIntermediateXMLFormat("Attribute @" + attrib_name + " not found in <" + tag_name + "> tag of the intermediate XML file.", "no matter")
{
}


} // namespace LE
} // namespace EXCEPT
