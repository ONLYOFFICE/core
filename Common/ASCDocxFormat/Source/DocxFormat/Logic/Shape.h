#pragma once
#ifndef OOX_LOGIC_SPAPE_INCLUDE_H_
#define OOX_LOGIC_SPAPE_INCLUDE_H_

#include "Fill.h"
#include "Wrap.h"
#include "Shadow.h"
#include "TextBox.h"
#include "TextPath.h"
#include "ImageData.h"
#include "Extrusion.h"
#include "LineStyle.h"
#include "FillStyle.h"
#include "ShapeStyle.h"

namespace OOX
{
	namespace Logic
	{
		class Shape : public WritingElement
		{
		public:
			Shape();
			virtual ~Shape();			
			explicit Shape(const XML::XNode& node);
			const Shape& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>								Id;
			property<std::string>								Type;
			property<ShapeStyle>								style;

			nullable_property<TextPath>							TextPath;
			nullable_property<std::string>						adj;
			nullable_property<std::string>						Ole;
			
			nullable_property<ImageData>						imageData;
			nullable_property<std::string, Limit::TrueFalse>	AllowInCell;
			nullable_property<Wrap>								Wrap;
			nullable_property<Extrusion>						Extrusion;	
			nullable_property<std::string>						coordsize;
			
			nullable_property<NullElement>						anchorlock;

			nullable_property<FillStyle>						fillstyle;
			nullable_property<LineStyle>						linestyle;
			nullable_property<Shadow>							shadow;
		
			nullable_property<std::string>						path;
			nullable_property<TextBox>							textbox;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_SPAPE_INCLUDE_H_