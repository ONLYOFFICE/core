#include "stdafx.h"
#include "AttributeDataWrong.h"


namespace EXCEPT
{;
namespace LE
{;

AttributeDataWrong::AttributeDataWrong(const _bstr_t& attrib_name, const _bstr_t& tag_name, const _bstr_t& value)
: WrongIntermediateXMLFormat(std::string("Attribute $") + static_cast<char*>(attrib_name) + " of <" + static_cast<char*>(tag_name) + 
							 "> tag  has incorrect format. Value got: \"" + static_cast<char*>(value) + "\".", "no matter")
{
}


} // namespace LE
} // namespace EXCEPT
