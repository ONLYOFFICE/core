
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FormatError.h"
#include "Log.h"


namespace OOX
{

	FormatError::FormatError(const std::string& message)
		: runtime_error(message)
	{
		Log::error(message);
	}


	FormatError::FormatError(const std::wstring& message)
		: runtime_error("docx format error")
	{
		Log::error(message);
	}

} // namespace OOX