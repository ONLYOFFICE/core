
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "OriginFileError.h"
#include "Log.h"


namespace OOX2Odt
{
	OriginFileError::OriginFileError(const std::string& message)
		: std::runtime_error(message)
	{
		Log::error(message);
	}
} // namespace Odt2OOX