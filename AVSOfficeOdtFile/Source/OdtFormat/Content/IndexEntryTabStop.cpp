
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "IndexEntryTabStop.h"


namespace Odt
{
	namespace Content
	{

		IndexEntryTabStop::IndexEntryTabStop()
		{
		}


		IndexEntryTabStop::~IndexEntryTabStop()
		{
		}


		IndexEntryTabStop::IndexEntryTabStop(const XML::XNode& node)
		{
			fromXML(node);
		}


		const IndexEntryTabStop& IndexEntryTabStop::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void IndexEntryTabStop::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Type			 = element.attribute("type").value();
			LeaderChar = element.attribute("leader-char").value();
		}


		const XML::XNode IndexEntryTabStop::toXML() const
		{
			return 
				XML::XElement(ns.text + "index-entry-tab-stop",
					XML::XAttribute(ns.text + "type", Type) +
					XML::XAttribute(ns.text + "leader-char", LeaderChar) 
				);
		}	

	} // namespace Content
} // namespace Odt