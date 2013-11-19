#pragma once

#include "WrongIntermediateXMLFormat.h"

namespace EXCEPT
{;
namespace LE
{;

class AttributeNotFound : public WrongIntermediateXMLFormat
{
public:
	explicit AttributeNotFound(const std::string& attrib_name, const std::string& tag_name);
};


} // namespace LE
} // namespace EXCEPT

