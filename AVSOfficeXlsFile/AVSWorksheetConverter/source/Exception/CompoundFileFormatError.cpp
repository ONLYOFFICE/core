#include "stdafx.h"
#include "CompoundFileFormatError.h"


namespace EXCEPT
{;
namespace RT
{;

CompoundFileFormatError::CompoundFileFormatError(const std::string& message, HRESULT hres)
: RuntimeException(message + (S_OK != hres ? (std::string(" HRESULT: 0x") + STR::int2str(hres, 16) + " (" + STR::hres2str(hres) + ")") : ""))
{
}


} // namespace RT
} // namespace EXCEPT