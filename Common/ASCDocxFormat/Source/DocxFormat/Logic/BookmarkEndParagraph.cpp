
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
	
		BookmarkEndParagraph::BookmarkEndParagraph(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const BookmarkEndParagraph& BookmarkEndParagraph::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BookmarkEndParagraph::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_Id = element.attribute("id").value().ToWString();
		}

		void BookmarkEndParagraph::fromXML(XmlUtils::CXmlNode& node)
		{
			m_Id = std::wstring(static_cast<const wchar_t*>(node.GetAttributeBase( _T("w:id"))));
			
			//const XML::XElement element(node);
			//m_Id = element.attribute("id").value();
		}

		const XML::XNode BookmarkEndParagraph::toXML() const
		{
		return XML::XElement();
		}

		const std::string BookmarkEndParagraph::toTxt() const
		{
			return "";
		}

	} // namespace Logic
} // namespace OOX