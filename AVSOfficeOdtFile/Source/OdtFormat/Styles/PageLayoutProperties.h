#pragma once
#ifndef ODT_STYLES_PAGE_LAYOUT_PROPERTIES_INCLUDE_H_
#define ODT_STYLES_PAGE_LAYOUT_PROPERTIES_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "FootnoteSep.h"
#include "Columns.h"
#include "nullable_property.h"
#include "property.h"
#include "./../Unit.h"
#include "./../Limit/Orientation.h"
#include "./../Logic/NumFormat.h"
#include "./../Logic/Border.h"
#include "Utility.h"
#include "./../Logic/BackgroundImage.h"
#include "./../Logic/BackgroundColor.h"


namespace Odt
{
	namespace Styles
	{
		class PageLayoutProperties : public WritingElement
		{
		public:
			PageLayoutProperties();
			virtual ~PageLayoutProperties();
			explicit PageLayoutProperties(const XML::XNode& node);
			const PageLayoutProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool onlyBorder() const;
			const bool hasBorders() const;

		public:
			nullable_property<Odt::Styles::Columns>	Columns;
			nullable_property<Odt::Logic::BackgroundImage> BackgroundImage;
			property<UniversalUnit>	PageWidth;
			property<UniversalUnit>	PageHeight;
			property<std::string, Limit::Orientation> PrintOrientation;
			property<UniversalUnit>	MarginTop;
			property<UniversalUnit>	MarginBottom;
			property<UniversalUnit>	MarginLeft;
			property<UniversalUnit>	MarginRight;
			nullable_property<Logic::Border>		Border;
			nullable_property<Logic::Border>		BorderLeft;
			nullable_property<Logic::Border>		BorderRight;
			nullable_property<Logic::Border>		BorderTop;
			nullable_property<Logic::Border>		BorderBottom;
			nullable_property<Logic::BackgroundColor> BackgroundColor;


		private:
			Logic::NumFormat		m_numFormat;
			std::string				m_writingMode;
			UnitCm					m_footnoteMaxHeight;
			nullable<FootnoteSep>	m_footnoteSep;	
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_PAGE_LAYOUT_PROPERTIES_INCLUDE_H_