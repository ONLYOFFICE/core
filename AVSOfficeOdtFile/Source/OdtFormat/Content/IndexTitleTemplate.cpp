
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "IndexTitleTemplate.h"


namespace Odt
{
	namespace Content
	{

		IndexTitleTemplate::IndexTitleTemplate()
		{
		}


		IndexTitleTemplate::~IndexTitleTemplate()
		{
		}


		IndexTitleTemplate::IndexTitleTemplate(const XML::XNode& node)
		{
			fromXML(node);
		}


		const IndexTitleTemplate& IndexTitleTemplate::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void IndexTitleTemplate::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Text	= element.text();
			Style	= element.attribute("style-name").value();
		}


		const XML::XNode IndexTitleTemplate::toXML() const
		{
			return 
				XML::XElement(ns.text + "index-title-template",
					XML::XAttribute(ns.text + "style-name", Style) +
					XML::XText(Text)
				);
		}	

	} // namespace Content
} // namespace Odt