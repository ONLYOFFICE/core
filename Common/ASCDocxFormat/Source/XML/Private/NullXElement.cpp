
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "NullXElement.h"

namespace XML
{
	namespace Private
	{
		NullXElement::NullXElement() : XElement(*this) // Передача типа, а не значения
		{

		}

		const bool NullXElement::exist() const
		{
			return false;
		}

	} // namespace Private
} // namespace XML