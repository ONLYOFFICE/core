
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "Write.h"
#include "./../XObject.h"
#include "./../XElement.h"


namespace XML
{
	const XNode Write(const XObject& object)
	{
		return object.toXML();
	}
}