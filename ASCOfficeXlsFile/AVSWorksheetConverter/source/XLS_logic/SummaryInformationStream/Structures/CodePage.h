#pragma once
#include "Property.h"

#include "XLS_bin/BinSmartPointers.h"

namespace OLEPS
{;

class CodePage : public Property
{
public:
	CodePage(const WORD value_type, XLS::CFStreamPtr stream);
	
	static const DWORD Type = 0x0001;
	static const WORD DefaultCodePage = 1250;

	const WORD GetCodePage();


private:
	WORD code_page;
};

typedef boost::shared_ptr<CodePage> CodePagePropertyPtr;

} // namespace OLEPS
