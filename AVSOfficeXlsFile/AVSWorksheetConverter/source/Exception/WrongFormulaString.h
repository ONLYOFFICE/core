#pragma once

#include "exceptions.h"

namespace EXCEPT
{;
namespace RT
{;

class WrongFormulaString : public RuntimeException
{
public:
	explicit WrongFormulaString(const std::string& message, const std::wstring& formula);
};

} // namespace RT
} // namespace EXCEPT

