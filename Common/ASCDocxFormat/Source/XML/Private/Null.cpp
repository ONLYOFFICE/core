
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Null.h"
#include "NullText.h"


namespace XML
{
	namespace Private
	{

		NullText& Null::Text()
		{
			static NullText text;
			return text;
		}

	} // namespace Private
} // namespace XML