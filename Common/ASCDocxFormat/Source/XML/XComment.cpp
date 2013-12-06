
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XComment.h"
#include "XNode.h"

namespace XML
{
	XComment::XComment(const XNode& xnode)
		: base(xnode.get_ptr().smart_dynamic_cast<Private::Comment>()),
		Value(m_ptr->Value)
	{
	}

} // namespace XML