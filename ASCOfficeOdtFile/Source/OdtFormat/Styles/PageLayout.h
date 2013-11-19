#pragma once
#ifndef ODT_STYLES_PAGE_LAYOUT_INCLUDE_H_
#define ODT_STYLES_PAGE_LAYOUT_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "PageLayoutProperties.h"
#include "HeaderStyle.h"
#include "FooterStyle.h"
#include "property.h"


namespace Odt
{
	namespace Styles
	{
		class PageLayout : public WritingElement
		{
		public:
			PageLayout();
			virtual ~PageLayout();
			explicit PageLayout(const XML::XNode& node);
			const PageLayout& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>						Name;	
			property<PageLayoutProperties>	Properties;
			nullable_property<HeaderStyle>  HeaderStyle;
			nullable_property<FooterStyle>	FooterStyle;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_PAGE_LAYOUT_INCLUDE_H_