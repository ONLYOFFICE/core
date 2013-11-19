#pragma once
#ifndef ODP_STYLES_PAGE_LAYOUT_INCLUDE_H_
#define ODP_STYLES_PAGE_LAYOUT_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include <string>
#include "property.h"
#include "PageLayoutProperties.h"

namespace Odp
{
	namespace Styles
	{
		class PageLayout : public Odt::WritingElement
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
			property<std::string> Name;
			property<PageLayoutProperties> Properties;
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_PAGE_LAYOUT_INCLUDE_H_