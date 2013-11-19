#pragma once
#ifndef ODT_STYLES_HEADER_FOOTER_PROPERTIES_INCLUDE_H_
#define ODT_STYLES_HEADER_FOOTER_PROPERTIES_INCLUDE_H_

#include "./../WritingElement.h"
#include "./../Unit.h"
#include "nullable_property.h"


namespace Odt
{
	namespace Styles
	{
		class HeaderFooterProperties : public WritingElement
		{
		public:
			HeaderFooterProperties();
			virtual ~HeaderFooterProperties();
			explicit HeaderFooterProperties(const XML::XNode& node);
			const HeaderFooterProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public://TODO Заменить на UniversalUnit
			nullable_property<UnitCm>	Height;
			nullable_property<UnitCm>	MinHeight;
			nullable_property<UnitCm>	MarginTop;
			nullable_property<UnitCm>	MarginLeft;
			nullable_property<UnitCm>	MarginRight;
			nullable_property<UnitCm>	MarginBottom;
			nullable_property<bool>		DynamicSpacing;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_HEADER_FOOTER_PROPERTIES_INCLUDE_H_