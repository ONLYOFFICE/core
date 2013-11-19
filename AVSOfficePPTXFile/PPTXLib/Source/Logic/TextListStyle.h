#pragma once
#ifndef PPTX_LOGIC_TEXTSTYLE_INCLUDE_H_
#define PPTX_LOGIC_TEXTSTYLE_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "TextParagraphPr.h"

namespace PPTX
{
	namespace Logic
	{

		class TextListStyle : public WrapperWritingElement
		{
		public:
			TextListStyle();
			virtual ~TextListStyle();
			explicit TextListStyle(const XML::XNode& node);
			const TextListStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(nullable_property<TextListStyle>& lstStyle)const;
		public:
			//nullable_property<TextParagraphPr> defPPr;
			//nullable_property<TextParagraphPr> lvl1pPr;
			//nullable_property<TextParagraphPr> lvl2pPr;
			//nullable_property<TextParagraphPr> lvl3pPr;
			//nullable_property<TextParagraphPr> lvl4pPr;
			//nullable_property<TextParagraphPr> lvl5pPr;
			//nullable_property<TextParagraphPr> lvl6pPr;
			//nullable_property<TextParagraphPr> lvl7pPr;
			//nullable_property<TextParagraphPr> lvl8pPr;
			//nullable_property<TextParagraphPr> lvl9pPr;
			nullable_property<TextParagraphPr> levels[10];
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TEXTSTYLE_INCLUDE_H_