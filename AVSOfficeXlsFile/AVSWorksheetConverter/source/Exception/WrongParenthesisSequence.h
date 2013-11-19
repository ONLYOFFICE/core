#pragma once

#include "WrongFormulaString.h"

namespace EXCEPT
{;
namespace RT
{;

class WrongParenthesisSequence : public WrongFormulaString
{
public:
	explicit WrongParenthesisSequence(const std::wstring& formula);
};

} // namespace RT
} // namespace EXCEPT

