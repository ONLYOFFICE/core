
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XText.h"
#include "XNode.h"

namespace XML
{
	XText::XText(const XNode& xnode)
		: base(xnode.get_ptr().smart_dynamic_cast<Private::Text>()),
		Value(m_ptr->Value)
	{
	}

} // namespace XML