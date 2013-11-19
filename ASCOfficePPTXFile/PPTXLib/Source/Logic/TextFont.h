#pragma once
#ifndef PPTX_LOGIC_TEXTFONT_INCLUDE_H_
#define PPTX_LOGIC_TEXTFONT_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class TextFont : public WrapperWritingElement
		{
		public:
			TextFont();
			virtual ~TextFont();
			explicit TextFont(const XML::XNode& node);
			const TextFont& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			void Merge(nullable_property<TextFont>& font)const;
		public:
			nullable_property<std::string> charset;
			nullable_property<std::string> panose;
			nullable_property<std::string> pitchFamily;
			property<std::string> typeface;
		//private:
		public:
			property<std::string> m_name;
			nullable_property<XML::XNamespace> m_namespace;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TEXTFONT_INCLUDE_H_