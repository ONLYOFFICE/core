
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BookmarkStart.h"


namespace OOX
{
	namespace Logic
	{

		BookmarkStart::BookmarkStart()
		{
		}


		BookmarkStart::~BookmarkStart()
		{
		}



		BookmarkStart::BookmarkStart(const std::string& text)
		{
			fromTxt(text);
		}


		BookmarkStart::BookmarkStart(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BookmarkStart& BookmarkStart::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		const BookmarkStart& BookmarkStart::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BookmarkStart::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Id		= element.attribute("id").value();
			Name	= element.attribute("name").value();
		}


		const XML::XNode BookmarkStart::toXML() const
		{
		return XML::XElement();
		}


		void BookmarkStart::fromTxt(const std::string& text)
		{
			Name = text;
		}


		const std::string BookmarkStart::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX