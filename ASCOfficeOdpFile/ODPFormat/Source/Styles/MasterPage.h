#pragma once
#ifndef ODP_STYLES_MASTER_PAGE_INCLUDE_H_
#define ODP_STYLES_MASTER_PAGE_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include <string>
#include <vector>
#include "property.h"
#include "./../Content/DrawingElement.h"

namespace Odp
{
	namespace Styles
	{
		class MasterPage : public Odt::WritingElement
		{
		public:
			MasterPage();
			virtual ~MasterPage();
			explicit MasterPage(const XML::XNode& node);
			const MasterPage& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>		Name;
			property<std::string>		Layout;
			property<std::string>		StyleName;
			property<std::vector<Odp::Content::DrawingElement> > Elements;
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_MASTER_PAGE_INCLUDE_H_