
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XText.h"
#include "XNode.h"
#include <boost/shared_ptr.hpp>


namespace XML
{

	XText::XText(const XNode& xnode)
		: base(boost::dynamic_pointer_cast<Private::Text>(xnode.get_ptr())),
			Value(m_ptr->Value)
	{
	}


} // namespace XML