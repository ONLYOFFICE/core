#pragma once
#ifndef ODT_CONTENT_CUSTOM_SHAPE_INCLUDE_H_
#define ODT_CONTENT_CUSTOM_SHAPE_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include "./../Limit/Anchortype.h"
#include "Utility.h"
#include <string>
#include "EnhancedGeometry.h"
#include "Paragraph.h"
#include <vector>
#include "./../Unit.h"
#include "Size.h"
#include "Point.h"


namespace Odt
{
	namespace Content
	{
		class CustomShape : public ParagraphItemBase
		{
		public:
			CustomShape();
			virtual ~CustomShape();
			explicit CustomShape(const XML::XNode& node);
			const CustomShape& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			property<std::string, Limit::Anchortype>	AnchorType;
			property<std::string>											StyleName;
			nullable_property<std::string>						TextStyleName;
			property<int>															ZIndex;
			property<Common::Size<UnitCm> >						Size;
			property<Common::Point<UnitCm> >					Point;			
			property<EnhancedGeometry>								EnhancedGeometry;
			nullable_property<std::string>						Transform;
			nullable_property<std::string>						Title;
			property<std::vector<Paragraph> >					Items;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_CUSTOM_SHAPE_INCLUDE_H_