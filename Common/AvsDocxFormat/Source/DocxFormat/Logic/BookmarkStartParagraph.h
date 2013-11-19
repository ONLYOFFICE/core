#pragma once
#ifndef OOX_LOGIC_BOOKMARK_START_PARAGRAPH_INCLUDE_H_
#define OOX_LOGIC_BOOKMARK_START_PARAGRAPH_INCLUDE_H_

#include "TextItemBase.h"
#include "property.h"
#include <string>


namespace OOX
{
	namespace Logic
	{
		class BookmarkStartParagraph : public TextItemBase
		{
		public:
			BookmarkStartParagraph();
			virtual ~BookmarkStartParagraph();
			explicit BookmarkStartParagraph(const XML::XNode& node);
			const BookmarkStartParagraph& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			virtual const std::string toTxt() const;	

		public:
			property<std::string>						Id;
			property<std::string>		Name;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BOOKMARK_START_PARAGRAPH_INCLUDE_H_