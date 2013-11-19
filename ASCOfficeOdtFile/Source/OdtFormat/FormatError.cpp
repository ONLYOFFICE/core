
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "FormatError.h"
#include "Log.h"


namespace Odt
{
	FormatError::FormatError(const std::string& message)
		: runtime_error(message)
	{
		Log::error(message);
	}
} // namespace Odt