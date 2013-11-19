
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "NoteCitation.h"


// TODO устаревший
namespace Odt
{
	namespace Content
	{

		NoteCitation::NoteCitation()
		{
		}


		NoteCitation::~NoteCitation()
		{
		}


		NoteCitation::NoteCitation(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NoteCitation& NoteCitation::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NoteCitation::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Label	= element.attribute("label").value();
			Text	= element.text();
		}


		const XML::XNode NoteCitation::toXML() const
		{
			return
				XML::XElement(ns.text + "note-citation",
						XML::XAttribute(ns.text + "label", Label) + 
						XML::XText(Text)
					);
		}

	} // namespace Content
} // namespace Odt