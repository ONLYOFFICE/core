#pragma once
#ifndef ODT_CONTENT_BOOKMARK_START_INCLUDE_H_
#define ODT_CONTENT_BOOKMARK_START_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"


namespace Odt
{
	namespace Content
	{
		class BookmarkStart : public ParagraphItemBase
		{
		public:
			BookmarkStart();
			virtual ~BookmarkStart();
			explicit BookmarkStart(const XML::XNode& node);
			const BookmarkStart& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			property<std::string>		Name;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_BOOKMARK_START_INCLUDE_H_