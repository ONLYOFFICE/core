#pragma once

#include "CompoundFileFormatError.h"
#include <XLS_bin/CFRecordType.h>

namespace EXCEPT
{;
namespace RT
{;

class RequestedRecordNotFound : public CompoundFileFormatError
{
public:
	explicit RequestedRecordNotFound(const XLS::CFRecordType::TypeId req, const XLS::CFRecordType::TypeId got);

};

} // namespace RT
} // namespace EXCEPT

