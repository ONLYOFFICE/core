#pragma once
#ifndef OOX_LOGIC_BOOKMARK_START_PARAGRAPH_INCLUDE_H_
#define OOX_LOGIC_BOOKMARK_START_PARAGRAPH_INCLUDE_H_

#include <string>

#include "TextItemBase.h"
#include "property.h"

namespace OOX
{
	namespace Logic
	{
		class BookmarkStartParagraph : public TextItemBase
		{
		public:
			BookmarkStartParagraph();
			virtual ~BookmarkStartParagraph();
			BookmarkStartParagraph(const XML::XNode& node);
			const BookmarkStartParagraph& operator =(const XML::XNode& node);
			BookmarkStartParagraph(XmlUtils::CXmlNode& node);
			const BookmarkStartParagraph& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual void fromXML(XmlUtils::CXmlNode& node);

		public:
			virtual const std::string toTxt() const;	

		public:
			std::wstring	m_Id;
			std::wstring	m_Name;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BOOKMARK_START_PARAGRAPH_INCLUDE_H_