#include "stdafx.h"
#include "OpenCloseSequenceBroken.h"


namespace EXCEPT
{;
namespace LE
{;

OpenCloseSequenceBroken::OpenCloseSequenceBroken(const std::string& function_name)
	: WrongAPIUsage("Open/Close sequence broken.", function_name)
{
}

} // namespace LE
} // namespace EXCEPT
