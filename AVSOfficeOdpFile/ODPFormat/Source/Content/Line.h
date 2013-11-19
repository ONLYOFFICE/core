#pragma once
#ifndef ODP_CONTENT_LINE_INCLUDE_H_
#define ODP_CONTENT_LINE_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "Utility.h"
#include <windows.h>
#include "OdtFormat/Content/Text.h"

namespace Odp
{
	namespace Content
	{
		class Line : public Odt::WritingElement
		{
		public:
			Line();
			virtual ~Line();
			explicit Line(const XML::XNode& node);
			const Line& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			property<std::string> StyleName;
			nullable_property<std::string> TextStyleName;
			property<std::string> layer;
			
			property<UniversalUnit> x1;
			property<UniversalUnit> y1;
			property<UniversalUnit> x2;
			property<UniversalUnit> y2;

			property<std::vector<Odt::Content::Text> > Text;
		public:
			void GetMmRect(RECT& rect)const;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_LINE_INCLUDE_H_