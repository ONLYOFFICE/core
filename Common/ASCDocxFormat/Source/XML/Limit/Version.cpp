
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Version.h"
#include "./../Exception/Declaration.h"


namespace XML
{
	namespace Limit
	{
		Version::Version()
		{
			add("1.0");
		}

		const std::string Version::no_find() const
		{
			throw Exception::Declaration("declaration version error");
		}
	} // namespace Limit
} // namespace XML