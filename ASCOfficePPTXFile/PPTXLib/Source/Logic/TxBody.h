#pragma once
#ifndef PPTX_LOGIC_TXBODY_INCLUDE_H_
#define PPTX_LOGIC_TXBODY_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "BodyPr.h"
#include "TextListStyle.h"
#include "Paragraph.h"
#include <list>

namespace PPTX
{
	namespace Logic
	{
		class TxBody : public WrapperWritingElement
		{
		public:
			TxBody();
			virtual ~TxBody();			
			explicit TxBody(const XML::XNode& node);
			const TxBody& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			std::string GetText()const;
			std::wstring GetWText()const;

			void Merge(nullable_property<TxBody>& txBody)const;
		public:
			property<BodyPr> bodyPr;
			nullable_property<TextListStyle> lstStyle;
			property<std::list<Paragraph> > Paragrs;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TXBODY_INCLUDE_H