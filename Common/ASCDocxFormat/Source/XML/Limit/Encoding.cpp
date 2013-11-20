
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Encoding.h"
#include "./../Exception/Declaration.h"


namespace XML
{
	namespace Limit
	{
		Encoding::Encoding()
		{
			add("UTF-8");
			add("utf-8");
		}

		const std::string Encoding::no_find() const
		{
			throw Exception::Declaration("declaration encoding error");
		}
	} // namespace Limit
} // namespace XML