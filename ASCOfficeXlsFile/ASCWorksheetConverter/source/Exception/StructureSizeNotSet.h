#pragma once

#include "StructureParameterNotSet.h"

namespace EXCEPT
{;
namespace LE
{;

class StructureSizeNotSet : public StructureParameterNotSet
{
public:
	explicit StructureSizeNotSet(const std::string& struct_name, const std::string& function_name);
};

} // namespace LE
} // namespace EXCEPT

