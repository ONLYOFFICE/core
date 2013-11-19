#pragma once
#ifndef ODT_LOGIC_PROPERTY_PARAGRAPH_INCLUDE_H_
#define ODT_LOGIC_PROPERTY_PARAGRAPH_INCLUDE_H_

#include "./../../WritingElement.h"
#include <string>
#include "./../TabStops.h"
#include "./../../Unit.h"
#include "nullable_property.h"
#include "./../../Limit/HyphenationLadderCount.h"
#include "./../../Limit/TextAutospace.h"
#include "./../../Limit/PunctuationWrap.h"
#include "./../../Limit/LineBreak.h"
#include "./../../LImit/BreakBefore.h"
#include "./../../Limit/WritingMode.h"
#include "./../../Limit/KeepWithNext.h"
#include "./../../Limit/KeepTogether.h"
#include "./../../Limit/PageNumber.h"
#include "./../Align.h"
#include "./../Color.h"
#include "./../BackgroundColor.h"
#include "./../Border.h"
#include "./../DropCap.h"
#include "UniversalUnit.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{
			class Paragraph : public WritingElement
			{
			public:
				Paragraph();
				virtual ~Paragraph();
				explicit Paragraph(const XML::XNode& node);
				const Paragraph& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;

			public:
				static const Paragraph merge(const Paragraph& lhs, const Paragraph& rhs);

			public:
				const bool isSimple() const;
				const bool onlyBorder() const;
				const bool hasBorders() const;

			public:
				nullable_property<Align>										Align;
				nullable_property<bool>											JustifySingleWord;
				nullable_property<std::string, Limit::HyphenationLadderCount>	HyphenationLadderCount;
				nullable_property<std::string, Limit::TextAutospace>			TextAutospace;
				nullable_property<std::string, Limit::PunctuationWrap>			PunctuationWrap;
				nullable_property<std::string, Limit::LineBreak>				LineBreak;
				nullable_property<std::string, Limit::BreakBefore>				BreakBefore;
                nullable_property<std::string, Limit::BreakBefore>				BreakAfter;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >		TabStopDistance;
				nullable_property<std::string, Limit::WritingMode>				WritingMode;
				nullable_property<bool>											FontIndependentLineSpacing;
				nullable_property<UniversalUnit>								MarginTop;
				nullable_property<UniversalUnit>								MarginBottom;
				nullable_property<UniversalUnit>								MarginLeft;
				nullable_property<UniversalUnit>								MarginRight;
				nullable_property<std::string, Limit::KeepWithNext>				KeepWithNext;
				nullable_property<std::string, Limit::KeepTogether>				KeepTogether;
				nullable_property<UniversalUnit>								TextIndent;
				nullable_property<bool>											AutoTextIndent;
				nullable_property<bool>											NumberLines;
				nullable_property<int, setter::between<int, 0, 10000> >			LineNumber;
				nullable_property<BackgroundColor>								Background;
				nullable_property<TabStops>										TabStops;
				nullable_property<UniversalUnit>								LineHeight;
				nullable_property<UniversalUnit>								LineHeightAtLeast;
				nullable_property<std::string, Limit::PageNumber>				PageNumber;
				nullable_property<Logic::Border>								Border;
				nullable_property<Logic::Border>								BorderTop;
				nullable_property<Logic::Border>								BorderLeft;
				nullable_property<Logic::Border>								BorderRight;
				nullable_property<Logic::Border>								BorderBottom;
				nullable_property<bool>											SnapToLayoutGrid;
				nullable_property<DropCap>										DropCap;

			};
		} // namespace Property
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTY_PARAGRAPH_INCLUDE_H_