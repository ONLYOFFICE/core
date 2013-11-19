#pragma once

#include "exceptions.h"

namespace EXCEPT
{;
namespace RT
{;

class CompoundFileFormatError : public RuntimeException
{
public:
	explicit CompoundFileFormatError(const std::string& message, HRESULT hres = S_OK);
};

} // namespace RT
} // namespace EXCEPT

