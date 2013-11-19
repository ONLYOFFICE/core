#include "stdafx.h"
#include "WrongFormulaString.h"


namespace EXCEPT
{;
namespace RT
{;

WrongFormulaString::WrongFormulaString(const std::string& message, const std::wstring& formula)
: RuntimeException(message + " Formula: " + static_cast<char*>(_bstr_t(formula.c_str())))
{
}

} // namespace RT
} // namespace EXCEPT
