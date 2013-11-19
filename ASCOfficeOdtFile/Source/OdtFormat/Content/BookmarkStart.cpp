
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "BookmarkStart.h"


namespace Odt
{
	namespace Content
	{

		BookmarkStart::BookmarkStart()
		{
		}


		BookmarkStart::~BookmarkStart()
		{
		}


		BookmarkStart::BookmarkStart(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BookmarkStart& BookmarkStart::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BookmarkStart::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name = element.attribute("name").value();
		}


		const XML::XNode BookmarkStart::toXML() const
		{
			return XML::XElement(ns.text + "bookmark-start", XML::XAttribute(ns.text + "name", Name));
		}


		const std::string BookmarkStart::toTxt() const
		{
			return "";
		}

	} // namespace Content
} // namespace Odt