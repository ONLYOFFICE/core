
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BookmarkEndParagraph.h"


namespace OOX
{
	namespace Logic
	{

		BookmarkEndParagraph::BookmarkEndParagraph()
		{
		}


		BookmarkEndParagraph::~BookmarkEndParagraph()
		{
		}


		BookmarkEndParagraph::BookmarkEndParagraph(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BookmarkEndParagraph& BookmarkEndParagraph::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BookmarkEndParagraph::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Id = element.attribute("id").value();
		}


		const XML::XNode BookmarkEndParagraph::toXML() const
		{
			return 
				XML::XElement(ns.w + "bookmarkEnd",
					XML::XAttribute(ns.w + "id", Id)
				);
		}


		const std::string BookmarkEndParagraph::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX