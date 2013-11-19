#include "stdafx.h"
#include "StructureSizeNotSet.h"


namespace EXCEPT
{;
namespace LE
{;

StructureSizeNotSet::StructureSizeNotSet(const std::string& struct_name, const std::string& function_name)
	: StructureParameterNotSet(struct_name, "size", function_name)
{
}

} // namespace LE
} // namespace EXCEPT
