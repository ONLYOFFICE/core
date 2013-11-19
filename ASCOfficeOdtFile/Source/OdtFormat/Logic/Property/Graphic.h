#pragma once
#ifndef ODT_LOGIC_PROPERTY_GRAPHIC_INCLUDE_H_
#define ODT_LOGIC_PROPERTY_GRAPHIC_INCLUDE_H_

#include "./../../WritingElement.h"
#include <string>
#include "./../Clip.h"
#include "nullable_property.h"
#include "./../../Unit.h"
#include "./../../Limit/Mirror.h"
#include "./../../Limit/ColorMode.h"
#include "./../../Limit/Rel.h"
#include "./../../Limit/Position.h"
#include "./../../Limit/AnchorType.h"
#include "./../../Limit/NumberWrappedParagraphs.h"
#include "./../../Limit/RunThrough.h"
#include "./../BackgroundColor.h"
#include "./../Border.h"
#include "./../Wrap.h"
#include "./../Position.h"
#include "./../Color.h"
#include "./../../Limit/Fill.h"
#include "./../../Limit/Stroke.h"
#include "./../../Limit/Repeat.h"
#include "./../../Limit/TextareaAlign.h"
#include "./../../Limit/StrokeLineJoin.h"
#include "./../../Limit/Protect.h"
#include <boost/shared_ptr.hpp>
#include "./../../Limit/WrapStyle.h"


namespace Odt
{
	namespace Logic
	{
		class ListStyle;

		namespace Property
		{
			class Graphic : public WritingElement
			{
			public:
				Graphic();
				virtual ~Graphic();
				explicit Graphic(const XML::XNode& node);
				const Graphic& operator =(const XML::XNode& node);

			public:
				virtual void fromXML(const XML::XNode& node);
				virtual const XML::XNode toXML() const;

			public:
				static const Graphic merge(const Graphic& prev, const Graphic& current);

			public:
				const bool isSimple() const;

			public:
				nullable_property<UnitCm, setter::between<UnitCm, 0, 10> >		ShadowOffsetX;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 10> >		ShadowOffsetY;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 10> >		StartLineSpacingHorizontal;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 10> >		StartLineSpacingVertical;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 10> >		EndLineSpacingHorizontal;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 10> >		EndLineSpacingVertical;
				nullable_property<bool>											FlowWithText;
				nullable_property<std::string, Limit::Mirror>					Mirror;
				nullable_property<Clip>											Clip;
				nullable_property<UnitPercent, setter::between<UnitPercent, 0, 100> >	Luminance;
				nullable_property<UnitPercent, setter::between<UnitPercent, 0, 100> >	Contrast;
				nullable_property<UnitPercent, setter::between<UnitPercent, 0, 100> >	Red;
				nullable_property<UnitPercent, setter::between<UnitPercent, 0, 100> >	Green;
				nullable_property<UnitPercent, setter::between<UnitPercent, 0, 100> >	Blue;
				nullable_property<UnitPercent, setter::between<UnitPercent, 0, 100> >	Gamma;
				nullable_property<bool>													ColorInversion;
				nullable_property<UnitPercent, setter::between<UnitPercent, 0, 100> >	ImageOpacity;
				nullable_property<std::string, Limit::ColorMode>			ColorMode;
				nullable_property<std::string, Limit::Anchortype>			AnchorType;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	SvgX;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	SvgY;
				nullable_property<std::string, Limit::WrapStyle>			Wrap;
				nullable_property<std::string, Limit::RunThrough>			RunThrough;
				nullable_property<std::string, Limit::NumberWrappedParagraphs>			NumberWrappedParagraphs;
				nullable_property<std::string, Limit::Position>				VerticalPos;
				nullable_property<std::string, Limit::Rel>					VerticalRel;
				nullable_property<std::string, Limit::Position>				HorizontalPos;
				nullable_property<std::string, Limit::Rel>					HorizontalRel;				
				nullable_property<int, setter::only_positive<int> >			OleDrawAspect;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	VisibleAreaTop;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	VisibleAreaWidth;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	VisibleAreaHeight;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	Padding;
				nullable_property<Border>									BorderRight;
				nullable_property<Border>									BorderLeft;
				nullable_property<Border>									BorderTop;
				nullable_property<Border>									BorderBottom;
				nullable_property<Border>									Border;	
				nullable_property<std::string>								Shadow;
				nullable_property<BackgroundColor>							BackgroundColor;
				nullable_property<UnitPercent, setter::between<UnitPercent, 0, 100> >	BackgroundTransparency;
				nullable_property<std::string, Limit::Stroke>				Stroke;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	StrokeWidth;
				nullable_property<Color>									StrokeColor;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	MarkerStartWidth;
				nullable_property<bool>										MarkerStartCenter;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	MarkerEndWidth;
				nullable_property<bool>										MarkerEndCenter;
				nullable_property<std::string, Limit::StrokeLineJoin>		StrokeLineJoin;
				nullable_property<std::string, Limit::Fill>					Fill;
				nullable_property<Color>									FillColor;
				nullable_property<std::string>								FillImageName;
				nullable_property<std::string>								FillGradientName;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	FillImageWidth;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30> >	FillImageHeight;
				nullable_property<std::string, Limit::Repeat>				Repeat;
				nullable_property<std::string, Limit::TextareaAlign>		TextareaHorizontalAlign;
				nullable_property<std::string, Limit::TextareaAlign>		TextareaVerticalAlign;
				nullable_property<bool>										AutoGrowHeight;
				nullable_property<bool>										AutoGrowWidth;
				nullable_property<bool>										FitToSize;
				nullable_property<UnitCm>									MinHeight;
				nullable_property<UnitCm>									MinWidth;
				nullable_property<UnitCm>									MaxHeight;
				nullable_property<UnitCm>									MaxWidth;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30>  >	PaddingTop;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30>  >	PaddingBottom;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30>  >	PaddingLeft;
				nullable_property<UnitCm, setter::between<UnitCm, 0, 30>  >	PaddingRight;
				nullable_property<std::string, Limit::Protect>				Protect;
				nullable_property<UnitPercent>								Opacity;
				nullable_property<UnitPercent>								ShadowOpacity;
			private:
				boost::shared_ptr<Odt::Logic::ListStyle>					ListStyle;

//				property<Wrap>																								 Wrap;
//				property<Position>																						 Position;
			};
		} // namespace Property
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_PROPERTY_GRAPHIC_INCLUDE_H_