#pragma once
#ifndef OOX_LOGIC_BOOKMARK_END_PARAGRAPH_INCLUDE_H_
#define OOX_LOGIC_BOOKMARK_END_PARAGRAPH_INCLUDE_H_

#include "TextItemBase.h"

namespace OOX
{
	namespace Logic
	{
		class BookmarkEndParagraph : public TextItemBase
		{
		public:
			BookmarkEndParagraph();
			virtual ~BookmarkEndParagraph();
			BookmarkEndParagraph(const XML::XNode& node);
			const BookmarkEndParagraph& operator =(const XML::XNode& node);
			BookmarkEndParagraph(XmlUtils::CXmlNode& node);
			const BookmarkEndParagraph& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);

		public:
			virtual const std::string toTxt() const;	

		public:
			std::wstring	m_Id;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BOOKMARK_END_PARAGRAPH_INCLUDE_H_