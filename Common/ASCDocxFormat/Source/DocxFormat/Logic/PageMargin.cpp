
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "PageMargin.h"


namespace OOX
{
	namespace Logic
	{

		PageMargin::PageMargin()
		{
		}


		PageMargin::~PageMargin()
		{
		}


		PageMargin::PageMargin(const XML::XNode& node)
		{
			fromXML(node);
		}


		const PageMargin& PageMargin::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void PageMargin::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Top =			element.attribute("top").value();//.ToString();
			Right =		element.attribute("right").value();//.ToString();
			Bottom =	element.attribute("bottom").value();//.ToString();
			Left =		element.attribute("left").value();//.ToString();
			Header =	element.attribute("header").value();
			Footer =	element.attribute("footer").value();
			Gutter =	element.attribute("gutter").value();
		}


		const XML::XNode PageMargin::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX