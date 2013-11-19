#pragma once
#ifndef ODT_LOGIC_PROPERTY_TABLE_CELL_INCLUDE_H_
#define ODT_LOGIC_PROPERTY_TABLE_CELL_INCLUDE_H_

#include "./../../WritingElement.h"
#include <string>
#include "./../../Unit.h"
#include "./../Border.h"
#include "./../BorderLine.h"
#include "nullable_property.h"
#include "./../../Limit/Align.h"
#include "./../Color.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{
			class TableCell : public Odt::WritingElement
			{
			public:
				TableCell();
				virtual ~TableCell();
				explicit TableCell(const XML::XNode& node);
				const TableCell& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;

			public:
				static const TableCell merge(const TableCell& prev, const TableCell& current);

			public:
				const bool isSimple() const;
				const bool onlyBorder() const;

			public:
				nullable_property<std::string, Limit::Align>	VerticalAlign;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> > Padding;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> > PaddingLeft;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> > PaddingRight;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> > PaddingTop;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> > PaddingBottom;
				nullable_property<Logic::Border>		Border;
				nullable_property<Logic::Border>		BorderLeft;
				nullable_property<Logic::Border>		BorderRight;
				nullable_property<Logic::Border>		BorderTop;
				nullable_property<Logic::Border>		BorderBottom;
				nullable_property<Logic::BorderLine>	BorderLineWidthLeft;
				nullable_property<Logic::BorderLine>	BorderLineWidthRight;
				nullable_property<Logic::BorderLine>	BorderLineWidthTop;
				nullable_property<Logic::BorderLine>	BorderLineWidthBottom;
				nullable_property</*Color*/std::string>	BackgroundColor;
			};
		} // namespace Property
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTY_TABLE_CELL_INCLUDE_H_