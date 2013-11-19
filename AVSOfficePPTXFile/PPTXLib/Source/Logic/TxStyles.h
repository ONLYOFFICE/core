#pragma once
#ifndef PPTX_LOGIC_TXSTYLES_INCLUDE_H_
#define PPTX_LOGIC_TXSTYLES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "BodyPr.h"
#include "TextListStyle.h"
#include "Paragraph.h"

namespace PPTX
{
	namespace Logic
	{
		class TxStyles : public WrapperWritingElement
		{
		public:
			TxStyles();
			virtual ~TxStyles();			
			explicit TxStyles(const XML::XNode& node);
			const TxStyles& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<TextListStyle> titleStyle;
			nullable_property<TextListStyle> bodyStyle;
			nullable_property<TextListStyle> otherStyle;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TXSTYLES_INCLUDE_H