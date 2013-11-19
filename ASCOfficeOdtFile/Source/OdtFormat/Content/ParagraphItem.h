#ifndef ODT_CONTENT_PARAGRAPH_ITEM_INCLUDE_H_
#define ODT_CONTENT_PARAGRAPH_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include "IItemable.h"
#include <boost/shared_ptr.hpp>
#include <string>


namespace Odt
{
	namespace Content
	{
		class ParagraphItemBase;

		class ParagraphItem : public WritingElement, public IItemable<ParagraphItemBase>
		{
		public:
			ParagraphItem();
			virtual ~ParagraphItem();
			explicit ParagraphItem(ParagraphItemBase* item);
			explicit ParagraphItem(const XML::XNode& node);
			explicit ParagraphItem(const std::string& text);
			const ParagraphItem& operator =(ParagraphItemBase* item);
			const ParagraphItem& operator =(const XML::XNode& node);
			const ParagraphItem& operator =(const std::string& text);

		public:
			void fromItem(ParagraphItemBase* item);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			void fromTxt(const std::string& text);
			const std::string toTxt() const;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_PARAGRAPH_ITEM_INCLUDE_H_