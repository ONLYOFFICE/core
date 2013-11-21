#pragma once

#include "XLS_bin\BinSmartPointers.h"
#include "Structures\PropertySetStream.h"

namespace OLEPS
{;

// This class and the beneath tree is created for future full implementation
// The current necessity is to extract code page from a SummaryInformation stream
// If the rest of functionality is never used in the future then the whole tree may
// be replaced with plain read implementation. But I doubt in any performance bonus.
class SummaryInformation
{
public:
	SummaryInformation(XLS::CFStreamPtr stream);
	
	const WORD GetCodePage();

private:
	PropertySetStream property_set_stream;
};

} // namespace OLEPS
