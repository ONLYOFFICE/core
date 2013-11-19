#pragma once
#ifndef ODT_STYLES_HEADER_STYLE_INCLUDE_H_
#define ODT_STYLES_HEADER_STYLE_INCLUDE_H_

#include "./../WritingElement.h"
#include "HeaderFooterProperties.h"
#include "property.h"


namespace Odt
{
	namespace Styles
	{
		class HeaderStyle : public WritingElement
		{
		public:
			HeaderStyle();
			virtual ~HeaderStyle();
			explicit HeaderStyle(const XML::XNode& node);
			const HeaderStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<HeaderFooterProperties> HeaderFooterProperties;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_HEADER_STYLE_INCLUDE_H_