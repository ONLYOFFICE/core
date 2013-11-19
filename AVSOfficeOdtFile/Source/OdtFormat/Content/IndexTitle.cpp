
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "IndexTitle.h"


namespace Odt
{
	namespace Content
	{

		IndexTitle::IndexTitle()
		{
		}


		IndexTitle::~IndexTitle()
		{
		}


		IndexTitle::IndexTitle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const IndexTitle& IndexTitle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void IndexTitle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name	= element.attribute("name").value();
			Style	= element.attribute("style-name").value();
			Item	= element.element("p");
		}


		const XML::XNode IndexTitle::toXML() const
		{
			return 
				XML::XElement(ns.text + "index-title",
					XML::XAttribute(ns.text + "style-name", Style) +
					XML::XAttribute(ns.text + "name", Name) +
					XML::Write(Item)
				);
		}	


		const bool IndexTitle::hasList() const
		{
			return false;
		}


		const bool IndexTitle::hasNote() const
		{
			return false;
		}

	} // namespace Content
} // namespace Odt