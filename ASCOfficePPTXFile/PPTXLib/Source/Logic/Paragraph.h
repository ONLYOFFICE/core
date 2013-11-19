#pragma once
#ifndef PPTX_LOGIC_PARAGRAPH_INCLUDE_H_
#define PPTX_LOGIC_PARAGRAPH_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "property.h"
#include "TextParagraphPr.h"
#include "RunProperties.h"
#include "RunElem.h"
#include <list>

namespace PPTX
{
	namespace Logic
	{
		class Paragraph : public WrapperWritingElement
		{
		public:
			Paragraph();
			virtual ~Paragraph();			
			explicit Paragraph(const XML::XNode& node);
			const Paragraph& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			std::string GetText()const;
			std::wstring GetWText()const;

		public:
			nullable_property<TextParagraphPr> pPr;
			property<std::list<RunElem> > RunElems;
			nullable_property<RunProperties> endParaRPr;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PARAGRAPH_INCLUDE_H