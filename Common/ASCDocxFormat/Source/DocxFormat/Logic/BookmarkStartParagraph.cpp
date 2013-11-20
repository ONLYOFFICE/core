
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BookmarkStartParagraph.h"


namespace OOX
{
	namespace Logic
	{

		BookmarkStartParagraph::BookmarkStartParagraph()
		{
		}


		BookmarkStartParagraph::~BookmarkStartParagraph()
		{
		}


		BookmarkStartParagraph::BookmarkStartParagraph(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BookmarkStartParagraph& BookmarkStartParagraph::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BookmarkStartParagraph::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Id		= element.attribute("id").value();
			Name	= element.attribute("name").value();
		}


		const XML::XNode BookmarkStartParagraph::toXML() const
		{
			return 
				XML::XElement(ns.w + "bookmarkStart",
					XML::XAttribute(ns.w + "id", Id) + 
					XML::XAttribute(ns.w + "name", Name)
				);
		}


		const std::string BookmarkStartParagraph::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX