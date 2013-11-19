#pragma once
#ifndef ODT_LOGIC_PROPERTY_TEXT_INCLUDE_H_
#define ODT_LOGIC_PROPERTY_TEXT_INCLUDE_H_

#include "./../../WritingElement.h"
#include "nullable_property.h"
#include "./../../Unit.h"
#include <string>
#include "./../Hyphenate.h"
#include "./../Index.h"
#include "./../Color.h"
#include "./../BackgroundColor.h"
#include "./../../Limit/FontFamilyGeneric.h"
#include "./../../Limit/FontPitch.h"
#include "./../../Limit/LineType.h"
#include "./../../Limit/LineStyle.h"
#include "./../../Limit/FontVariant.h"
#include "./../../Limit/TextTransform.h"
#include "./../../Limit/Display.h"
#include "./../../Limit/Relief.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{
			class Text : public WritingElement
			{
			public:
				Text();
				virtual ~Text();
				explicit Text(const XML::XNode& node);
				const Text& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;

			public:
				static const Text merge(const Text& lhs, const Text& rhs);

			public:
				const bool isSimple() const;

			private:
				const UnitPt sizeFromXML(const std::string& strSize);

			public:
				nullable_property<std::string>									FontName;
				nullable_property<std::string>									FontNameAsian;
				nullable_property<std::string>									FontNameComplex;
				nullable_property<std::string>									Language;
				nullable_property<std::string>									Country;
				nullable_property<Color>										FontColor;
				nullable_property<BackgroundColor>								Background;

				nullable_property<bool>											Bold;
				nullable_property<bool>											Italic;
				//nullable_property<bool>											Under;
				nullable_property<UnitPt, setter::between<UnitPt, 2, 999> >		FontSize;
				nullable_property<bool>											UseWindowFontColor;

				nullable_property<Hyphenate>									Hyphenate;
				nullable_property<Index>										Index;
				nullable_property<UnitCm>										LetterSpacing;
				nullable_property<std::string>									FontFamily;
				nullable_property<std::string, Odt::Limit::FontFamilyGeneric>	FontFamilyGeneric;
				nullable_property<std::string, Odt::Limit::FontPitch>			FontPitch;

				nullable_property<std::string, Limit::LineStyle>				LineThroughStyle;
				nullable_property<std::string, Limit::LineType>					LineThroughType;
				nullable_property<std::string>									LineThroughWidth;
				nullable_property<std::string>									LineThroughColor;

				nullable_property<std::string, Limit::LineStyle>				UnderlineStyle;
				nullable_property<std::string, Limit::LineType>					UnderlineType;
				nullable_property<std::string>									UnderlineWidth;
				nullable_property<std::string>									UnderlineColor;

				nullable_property<std::string, Limit::FontVariant>				FontVariant;
				nullable_property<std::string, Limit::TextTransform>			TextTransform;
				nullable_property<UnitPercent>									TextScale;
				nullable_property<bool>											Outline;
				nullable_property<std::string>									Shadow;
				nullable_property<std::string, Limit::Display>					Display;
				nullable_property<std::string, Limit::Relief>					Relief;
			};
		} // namespace Property
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTT_TEXT_INCLUDE_H_