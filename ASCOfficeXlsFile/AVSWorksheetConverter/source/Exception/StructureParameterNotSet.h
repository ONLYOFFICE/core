#pragma once

#include "WrongAPIUsage.h"

namespace EXCEPT
{;
namespace LE
{;

class StructureParameterNotSet : public WrongAPIUsage
{
public:
	explicit StructureParameterNotSet(const std::string& struct_name, const std::string& param_name, const std::string& function_name);
};

} // namespace LE
} // namespace EXCEPT

