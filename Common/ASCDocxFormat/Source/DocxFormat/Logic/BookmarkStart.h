#pragma once
#ifndef OOX_LOGIC_BOOKMARK_START_INCLUDE_H_
#define OOX_LOGIC_BOOKMARK_START_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include <string>


namespace OOX
{
	namespace Logic
	{
		class BookmarkStart : public ParagraphItemBase
		{
		public:
			BookmarkStart();
			virtual ~BookmarkStart();
			explicit BookmarkStart(const std::string& text);
			explicit BookmarkStart(const XML::XNode& node);
			const BookmarkStart& operator =(const std::string& text);
			const BookmarkStart& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			virtual void fromTxt(const std::string& text);
			virtual const std::string toTxt() const;	

		public:
			property<std::string>		Id;
			property<std::string>		Name;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BOOKMARK_START_INCLUDE_H_