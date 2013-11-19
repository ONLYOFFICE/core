#pragma once
#ifndef ODT_STYLES_COLUMNS_SEPARATOR_INCLUDE_H_
#define ODT_STYLES_COLUMNS_SEPARATOR_INCLUDE_H_

#include "./../WritingElement.h"
#include "./../Unit.h"
#include "./../Logic/Color.h"
#include "property.h"


namespace Odt
{
	namespace Styles
	{
		class ColumnSeparator : public WritingElement
		{
		public:
			ColumnSeparator();
			virtual ~ColumnSeparator();
			explicit ColumnSeparator(const XML::XNode& node);
			const ColumnSeparator& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<UnitCm>				Width;
			property<Logic::Color>  Color;
			property<std::string>		Height;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_COLUMNS_SEPARATOR_INCLUDE_H_