#pragma once

#include "CompoundFileFormatError.h"
#include <XLS_bin/CFRecordType.h>

namespace EXCEPT
{;
namespace RT
{;

class WrongBiffRecord : public CompoundFileFormatError
{
public:
	explicit WrongBiffRecord(const std::string& message, const XLS::CFRecordType::TypeString& biff_tag);
};

} // namespace RT
} // namespace EXCEPT

