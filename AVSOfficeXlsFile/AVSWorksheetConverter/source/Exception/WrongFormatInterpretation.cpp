#include "stdafx.h"
#include "WrongFormatInterpretation.h"


namespace EXCEPT
{;
namespace LE
{;

WrongFormatInterpretation::WrongFormatInterpretation(const std::string& function_name)
	: WrongAPIUsage("Format of a Biff record or union was wrongly interpreted.", function_name)
{
}

} // namespace LE
} // namespace EXCEPT
