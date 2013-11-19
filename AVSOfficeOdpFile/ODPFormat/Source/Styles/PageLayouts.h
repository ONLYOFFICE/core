#pragma once
#ifndef ODP_STYLES_PAGE_LAYOUTS_INCLUDE_H_
#define ODP_STYLES_PAGE_LAYOUTS_INCLUDE_H_

#include "OdtFormat/WritingVector.h"
#include "PageLayout.h"

namespace Odp
{
	namespace Styles
	{
		class PageLayouts : public Odt::WritingVector<PageLayout>
		{
		public:
			PageLayouts();
			virtual ~PageLayouts();
			explicit PageLayouts(const XML::XNode& node);
			const PageLayouts& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			const bool exist(const std::string& name) const;
			const PageLayout& operator[](const std::string& name) const;
			PageLayout& operator[](const std::string& name);
			const PageLayout& find(const std::string& name) const;
			PageLayout& find(const std::string& name);
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_PAGE_LAYOUTS_INCLUDE_H_