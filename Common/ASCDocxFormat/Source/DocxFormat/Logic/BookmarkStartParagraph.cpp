
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
			m_Id	=	std::wstring(L"");
			m_Name	=	std::wstring(L"");
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
			
		BookmarkStartParagraph::BookmarkStartParagraph(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const BookmarkStartParagraph& BookmarkStartParagraph::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BookmarkStartParagraph::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			m_Id	= element.attribute("id").value().ToWString();
			m_Name	= element.attribute("name").value().ToWString();
		}
		
		void BookmarkStartParagraph::fromXML(XmlUtils::CXmlNode& node)
		{
			m_Id	=	std::wstring(static_cast<const wchar_t*>(node.GetAttributeBase( _T("w:id"))));
			m_Name	=	std::wstring(static_cast<const wchar_t*>(node.GetAttributeBase( _T("w:id"))));
			
			//const XML::XElement element(node);
			//m_Id = element.attribute("id").value();
		}

		const XML::XNode BookmarkStartParagraph::toXML() const
		{
		return XML::XElement();
		}

		const std::string BookmarkStartParagraph::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX