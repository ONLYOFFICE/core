
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "PageBorders.h"


namespace OOX
{
	namespace Logic
	{
		PageBorders::PageBorders()
		{

		}

		PageBorders::~PageBorders()
		{

		}	

		PageBorders::PageBorders(const XML::XNode& node)
		{
			fromXML(node);
		}

		const PageBorders& PageBorders::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void PageBorders::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Top			=	element.element("top");
			Bottom		=	element.element("bottom");
			Left		=	element.element("left");
			Right		=	element.element("right");
			OffsetFrom	=	element.attribute("offsetFrom").value();
		}

		const XML::XNode PageBorders::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX