#pragma once

#include "exceptions.h"

namespace EXCEPT
{;
namespace RT
{;

class MSXMLError : public RuntimeException
{
public:
	explicit MSXMLError(const std::string& message);
};

} // namespace RT
} // namespace EXCEPT

