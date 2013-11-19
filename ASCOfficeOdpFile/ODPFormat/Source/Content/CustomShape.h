#pragma once
#ifndef ODP_CONTENT_CUSTOMSHAPE_INCLUDE_H_
#define ODP_CONTENT_CUSTOMSHAPE_INCLUDE_H_

#include "OdtFormat/WritingElement.h"
#include "OdtFormat/Content/Text.h"
#include "EnhancedGeometry.h"
#include "Utility.h"
#include <string>
#include <windows.h>

namespace Odp
{
	namespace Content
	{
		class CustomShape : public Odt::WritingElement
		{
		public:
			CustomShape();
			virtual ~CustomShape();
			explicit CustomShape(const XML::XNode& node);
			const CustomShape& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
		public:
			nullable_property<std::string> name;
			nullable_property<std::string> style_name;
			nullable_property<std::string> text_style_name;
			property<std::string> layer;
			property<UniversalUnit> height;
			property<UniversalUnit> width;
			nullable_property<UniversalUnit> x;
			nullable_property<UniversalUnit> y;
			nullable_property<std::string> transform;
			nullable_property<std::string> id;

			property<EnhancedGeometry> geometry;
			property<std::vector<Odt::Content::Text> > text;
		public:
			void GetMmRect(RECT& rect)const;
		};
	} // namespace Content
} // namespace Odp

#endif // ODP_CONTENT_CustomShape_INCLUDE_H_