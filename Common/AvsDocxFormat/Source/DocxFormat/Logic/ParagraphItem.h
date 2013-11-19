#pragma once
#ifndef OOX_LOGIC_PARAGRAPH_ITEM_INCLUDE_H_
#define OOX_LOGIC_PARAGRAPH_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include "IItemable.h"
#include "ParagraphItemBase.h"
#include <string>
#include "Run.h"
#include "Hyperlink.h"
#include "BookmarkStart.h"
#include "BookmarkEnd.h"
#include "FldSimple.h"
#include "Insert.h"
#include "Delete.h"


namespace OOX
{
	namespace Logic
	{
		class ParagraphItem : public WritingElement, public IItemable<ParagraphItemBase>
		{
		public:
			ParagraphItem();
			ParagraphItem(const Run& run);
			ParagraphItem(const Hyperlink& hyperlink);
			ParagraphItem(const BookmarkStart& start);
			ParagraphItem(const BookmarkEnd& end);
			virtual ~ParagraphItem();
			explicit ParagraphItem(const XML::XNode& node);
			const ParagraphItem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const std::string toTxt() const;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PARAGRAPH_ITEM_INCLUDE_H_