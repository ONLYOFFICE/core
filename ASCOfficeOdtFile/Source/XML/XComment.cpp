
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XComment.h"
#include "XNode.h"
#include <boost/shared_ptr.hpp>


namespace XML
{

	XComment::XComment(const XNode& xnode)
		: base(boost::dynamic_pointer_cast<Private::Comment>(xnode.get_ptr())),
			Value(m_ptr->Value)
	{
	}


} // namespace XML