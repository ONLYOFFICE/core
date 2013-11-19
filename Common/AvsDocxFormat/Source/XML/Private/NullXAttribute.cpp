
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "NullXAttribute.h"
#include "NullAttribute.h"


namespace XML
{
	namespace Private
	{
		NullXAttribute::NullXAttribute()
			: XAttribute(NullAttribute())
		{
		}

		const bool NullXAttribute::exist() const
		{
			return false;
		}
	} // namespace Private
} // namespace XML