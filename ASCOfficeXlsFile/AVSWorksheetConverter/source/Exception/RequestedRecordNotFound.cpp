#include "stdafx.h"
#include "RequestedRecordNotFound.h"


namespace EXCEPT
{;
namespace RT
{;

RequestedRecordNotFound::RequestedRecordNotFound(const XLS::CFRecordType::TypeId req, const XLS::CFRecordType::TypeId got)
	: CompoundFileFormatError("The extracted record is not of requested type.\nRequested: \"" + 
	XLS::CFRecordType::getStringById(req) + "\" Extracted: \"" + XLS::CFRecordType::getStringById(got) + "\"", S_OK)
{
}

} // namespace RT
} // namespace EXCEPT
