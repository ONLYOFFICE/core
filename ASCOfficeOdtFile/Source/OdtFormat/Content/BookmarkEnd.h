#pragma once
#ifndef ODT_CONTENT_BOOKMARK_END_INCLUDE_H_
#define ODT_CONTENT_BOOKMARK_END_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"


namespace Odt
{
	namespace Content
	{
		class BookmarkEnd : public ParagraphItemBase
		{
		public:
			BookmarkEnd();
			virtual ~BookmarkEnd();
			explicit BookmarkEnd(const XML::XNode& node);
			const BookmarkEnd& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;
			
		public:
			property<std::string>		Name;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_BOOKMARK_END_INCLUDE_H_