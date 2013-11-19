#pragma once
#ifndef ODP_STYLES_PAGELAYOUTPROPERTIES_INCLUDE_H_
#define ODP_STYLES_PAGELAYOUTPROPERTIES_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "nullable_property.h"
#include "Common.h"
#include "OdtFormat/Unit.h"
#include <string>
#include "OdtFormat/Limit/Orientation.h"

namespace Odp
{
	namespace Styles
	{
		class PageLayoutProperties : public Odt::WritingElement
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
			const bool isSimple() const;

		public:
			property<UniversalUnit> margin_top;
			property<UniversalUnit> margin_left;
			property<UniversalUnit> margin_right;
			property<UniversalUnit> margin_bottom;
			property<UniversalUnit> page_width;
			property<UniversalUnit> page_height;
			property<std::string, Odt::Limit::Orientation> print;
		};
	} // namespace Styles
} // namespace Odp

#endif // ODP_STYLES_PAGELAYOUTPROPERTIES_INCLUDE_H_