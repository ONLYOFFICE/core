
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Base.h"


namespace XML
{
	namespace Exception
	{
		Base::Base(const std::string& message)
			: log_invalid_argument(message)
		{
		}
	} // namespace Exception
} // namespace XML