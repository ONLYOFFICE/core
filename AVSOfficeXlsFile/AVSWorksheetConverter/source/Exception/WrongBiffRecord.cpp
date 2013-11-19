#include "stdafx.h"
#include "WrongBiffRecord.h"


namespace EXCEPT
{;
namespace RT
{;

WrongBiffRecord::WrongBiffRecord(const std::string& message, const XLS::CFRecordType::TypeString& biff_tag)
	: CompoundFileFormatError(message + " Record name: \"" + biff_tag + "\"", S_OK)
{
}

} // namespace RT
} // namespace EXCEPT
