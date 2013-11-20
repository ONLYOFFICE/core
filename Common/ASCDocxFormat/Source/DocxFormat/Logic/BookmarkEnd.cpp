
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BookmarkEnd.h"


namespace OOX
{
	namespace Logic
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
			Id = element.attribute("id").value();
		}


		const XML::XNode BookmarkEnd::toXML() const
		{
			return 
				XML::XElement(ns.w + "bookmarkEnd",
					XML::XAttribute(ns.w + "id", Id)
				);
		}


		const std::string BookmarkEnd::toTxt() const
		{
			return "";
		}	
	} // namespace Logic
} // namespace OOX