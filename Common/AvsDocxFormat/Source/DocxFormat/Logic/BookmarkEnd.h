#pragma once
#ifndef OOX_LOGIC_BOOKMARK_END_INCLUDE_H_
#define OOX_LOGIC_BOOKMARK_END_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"


namespace OOX
{
	namespace Logic
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

		public:
			virtual const std::string toTxt() const;		
			
		public:
			property<std::string>					Id;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BOOKMARK_END_INCLUDE_H_