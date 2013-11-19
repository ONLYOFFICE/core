
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "SettingsType.h"
#include <stdexcept>
#include "Exception/log_range_error.h"


namespace Odt
{
	namespace Limit
	{
		SettingsType::SettingsType()
		{
			add("int");
			add("boolean");
			add("string");
			add("short");
			add("base64Binary");
		}

		const std::string SettingsType::no_find() const
		{
			throw log_range_error("interval error");
		}
	} // namespace Limit
} // namespace Odt