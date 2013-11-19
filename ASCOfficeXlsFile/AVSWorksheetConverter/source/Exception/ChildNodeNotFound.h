#pragma once

#include "WrongIntermediateXMLFormat.h"

namespace EXCEPT
{;
namespace LE
{;

class ChildNodeNotFound : public WrongIntermediateXMLFormat
{
public:
	explicit ChildNodeNotFound(const std::string& tag_name, const std::string& parent_tag_name);
};


} // namespace LE
} // namespace EXCEPT

