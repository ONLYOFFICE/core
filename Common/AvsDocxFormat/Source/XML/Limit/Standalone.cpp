
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Standalone.h"
#include "./../Exception/Declaration.h"


namespace XML
{
	namespace Limit
	{
		Standalone::Standalone()
		{
			add("yes");
			add("no");
		}

		const std::string Standalone::no_find() const
		{
			throw Exception::Declaration("declaration standalone error");
		}
	} // namespace Limit
} // namespace XML