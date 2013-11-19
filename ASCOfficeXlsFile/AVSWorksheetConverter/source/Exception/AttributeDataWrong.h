#pragma once

#include "WrongIntermediateXMLFormat.h"

namespace EXCEPT
{;
namespace LE
{;

class AttributeDataWrong : public WrongIntermediateXMLFormat
{
public:
	explicit AttributeDataWrong(const _bstr_t& attrib_name, const _bstr_t& tag_name, const _bstr_t& value);
};


} // namespace LE
} // namespace EXCEPT

