#pragma once
#ifndef ODT_STYLES_PAGE_LAYOUTS_INCLUDE_H_
#define ODT_STYLES_PAGE_LAYOUTS_INCLUDE_H_

#include "./../WritingVector.h"
#include "PageLayout.h"
#include <string>


namespace Odt
{
	namespace Styles
	{
		class PageLayouts : public WritingVector<PageLayout>
		{
		public:
			PageLayouts();
			virtual ~PageLayouts();
			explicit PageLayouts(const XML::XNode& node);
			const PageLayouts& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);

		public:
			PageLayout& front();
			const PageLayout& front() const;

		public:
			const bool exist(const std::string& name) const;
			PageLayout& find(const std::string& name);
			const PageLayout& find(const std::string& name) const;
		};
	} // namespace Styles
} // namespace Odt

#endif // ODT_STYLES_PAGE_LAYOUTS_INCLUDE_H_