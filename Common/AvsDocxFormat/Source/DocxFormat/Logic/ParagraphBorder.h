#pragma once
#ifndef OOX_LOGIC_PARAGRAPH_BORDER_INCLUDE_H_
#define OOX_LOGIC_PARAGRAPH_BORDER_INCLUDE_H_

#include "./../WritingElement.h"
#include "Border.h"


namespace OOX
{
	namespace Logic
	{
		class ParagraphBorder : public WritingElement
		{
		public:
			ParagraphBorder();
			virtual ~ParagraphBorder();
			explicit ParagraphBorder(const XML::XNode& node);
			const ParagraphBorder& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
	
		public:
			nullable_property<Border>		Top;
			nullable_property<Border>		Bottom;
			nullable_property<Border>		Left;
			nullable_property<Border>		Right;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PARAGRAPH_BORDER_INCLUDE_H_