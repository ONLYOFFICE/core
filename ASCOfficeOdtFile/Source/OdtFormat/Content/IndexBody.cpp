
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "IndexBody.h"


namespace Odt
{
	namespace Content
	{

		IndexBody::IndexBody()
		{
		}


		IndexBody::~IndexBody()
		{
		}


		IndexBody::IndexBody(const XML::XNode& node)
		{
			fromXML(node);
		}


		const IndexBody& IndexBody::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void IndexBody::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
			{
				const XML::XElement element(*i);
				if (element.XName == "p" || element.XName == "h" || element.XName == "list" || element.XName == "table" || element.XName == "table-of-content" || element.XName == "index-title")
				{
					Text text(*i);
					Items->push_back(text);
				}
			}
		}


		const XML::XNode IndexBody::toXML() const
		{
			return 
				XML::XElement(ns.text + "index-body",
					XML::Write(Items)
				);
		}	

	} // namespace Content
} // namespace Odt