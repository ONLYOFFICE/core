#pragma once

#include "WrongAPIUsage.h"

namespace EXCEPT
{;
namespace LE
{;

class OpenCloseSequenceBroken : public WrongAPIUsage
{
public:
	explicit OpenCloseSequenceBroken(const std::string& function_name);
};

} // namespace LE
} // namespace EXCEPT

