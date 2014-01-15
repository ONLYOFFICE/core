
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "NullElement.h"


namespace XML
{
	namespace Private
	{

		const bool NullElement::exist() const
		{
			return false;
		}

		
		const XString NullElement::text() const
		{
			return XString();
		}

	} // namespace Private
} // namespace XML