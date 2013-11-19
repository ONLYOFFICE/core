#include "stdafx.h"
#include "ChildNodeNotFound.h"


namespace EXCEPT
{;
namespace LE
{;

ChildNodeNotFound::ChildNodeNotFound(const std::string& tag_name, const std::string& parent_tag_name)
	: WrongIntermediateXMLFormat("Child tag <" + tag_name + "> not found in <" + parent_tag_name + "> tag of the intermediate XML file.", "no matter")
{
}


} // namespace LE
} // namespace EXCEPT
