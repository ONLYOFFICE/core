
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "DocPartObj.h"


namespace OOX
{
	namespace Logic
	{

		DocPartObj::DocPartObj()
		{
		}


		DocPartObj::~DocPartObj()
		{
		}
	

		DocPartObj::DocPartObj(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DocPartObj& DocPartObj::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DocPartObj::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Gallery	= element.element("docPartGallery").attribute("val").value();
			Unique	= element.element("docPartUnique").exist();
		}


		const XML::XNode DocPartObj::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX