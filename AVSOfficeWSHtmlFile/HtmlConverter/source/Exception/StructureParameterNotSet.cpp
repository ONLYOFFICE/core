#include "stdafx.h"
#include "StructureParameterNotSet.h"


namespace EXCEPT
{;
namespace LE
{;

StructureParameterNotSet::StructureParameterNotSet(const std::string& struct_name, const std::string& param_name, const std::string& function_name)
	: WrongAPIUsage("Structure " + struct_name + " was used without mandatory parameter \"" + 
					param_name + "\" setting.", function_name)
{
}

} // namespace LE
} // namespace EXCEPT
