#pragma once
#ifndef ODT_LOGIC_PROPERTY_SECTION_INCLUDE_H_
#define ODT_LOGIC_PROPERTY_SECTION_INCLUDE_H_

#include "./../../WritingElement.h"
#include "property.h"
//#include "./../Columns.h"
#include "./../../Styles/Columns.h"
#include "./../BackgroundImage.h"
#include "Utility.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{
			class Section : public WritingElement
			{
			public:
				Section();
				virtual ~Section();
				explicit Section(const XML::XNode& node);
				const Section& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;

			public:
				const bool isSimple() const;

			public:
				property<Odt::Styles::Columns> Columns;
				property<bool>		Editable;
				nullable_property<bool> DontBalanceTextColumns;
				nullable_property<UniversalUnit> MarginLeft;
				nullable_property<UniversalUnit> MarginRight;
				nullable_property<std::string> BackgroundColor;
				nullable_property<BackgroundImage> Image;
			};
		} // namespace Property
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTY_SECTION_INCLUDE_H_