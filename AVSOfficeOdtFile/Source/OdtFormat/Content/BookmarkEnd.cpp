
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "BookmarkEnd.h"


namespace Odt
{
	namespace Content
	{

		BookmarkEnd::BookmarkEnd()
		{
		}


		BookmarkEnd::~BookmarkEnd()
		{
		}


		BookmarkEnd::BookmarkEnd(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BookmarkEnd& BookmarkEnd::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BookmarkEnd::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name = element.attribute("name").value();
		}


		const XML::XNode BookmarkEnd::toXML() const
		{
			return XML::XElement(ns.text + "bookmark-end", XML::XAttribute(ns.text + "name", Name));
		}


		const std::string BookmarkEnd::toTxt() const
		{
			return "";
		}

	} // namespace Content
} // namespace Odt