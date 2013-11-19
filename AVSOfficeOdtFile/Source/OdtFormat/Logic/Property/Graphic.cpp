
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Graphic.h"
#include "./../ListStyle.h"

namespace Odt
{
	namespace Logic
	{
		namespace Property
		{

			Graphic::Graphic()
			{
			}


			Graphic::~Graphic()
			{
			}


			Graphic::Graphic(const XML::XNode& node)
			{
				fromXML(node);
			}


			const Graphic& Graphic::operator =(const XML::XNode& node)
			{
				fromXML(node);
				return *this;
			}


			void Graphic::fromXML(const XML::XNode& node)
			{
				const XML::XElement element(node);
				ShadowOffsetX	= element.attribute("shadow-offset-x").value();
				ShadowOffsetY	= element.attribute("shadow-offset-y").value();
				StartLineSpacingHorizontal	= element.attribute("start-line-spacing-horizontal").value();
				StartLineSpacingVertical	= element.attribute("start-line-spacing-vertical").value();
				EndLineSpacingHorizontal	= element.attribute("end-line-spacing-horizontal").value();
				EndLineSpacingVertical		= element.attribute("end-line-spacing-vertical").value();
				FlowWithText	= element.attribute("flow-with-text").value();
				Mirror		= element.attribute("mirror").value();
				Clip		= element.attribute("clip").value();
				Luminance	= element.attribute("luminance").value();
				Contrast	= element.attribute("contrast").value();
				Red		= element.attribute("red").value();
				Green	= element.attribute("green").value();
				Blue	= element.attribute("blue").value();
				Gamma	= element.attribute("gamma").value();
				ColorInversion	= element.attribute("color-inversion").value();
				ImageOpacity	= element.attribute("image-opacity").value();
				ColorMode		= element.attribute("color-mode").value();
				AnchorType		= element.attribute("anchor-type").value();
				SvgX	= element.attribute("x").value();
				SvgY	= element.attribute("y").value();
				VerticalPos		= element.attribute("vertical-pos").value();
				VerticalRel		= element.attribute("vertical-rel").value();
				HorizontalPos	= element.attribute("horizontal-pos").value();
				HorizontalRel	= element.attribute("horizontal-rel").value();	
				OleDrawAspect	= element.attribute("ole-draw-aspect").value();
				VisibleAreaTop	= element.attribute("visible-area-top").value();
				VisibleAreaWidth	= element.attribute("visible-area-width").value();
				VisibleAreaHeight	= element.attribute("visible-area-height").value();
				Padding			= element.attribute("padding").value();
				BorderRight		= element.attribute("border-right").value();
				BorderLeft		= element.attribute("border-left").value();
				BorderTop		= element.attribute("border-top").value();
				BorderBottom	= element.attribute("border-bottom").value();
				Border			= element.attribute("border").value();
				Shadow			= element.attribute("shadow").value();
				BackgroundColor		= element.attribute("background-color").value();
				BackgroundTransparency	= element.attribute("background-transparency").value();
				Stroke				= element.attribute("stroke").value();
				StrokeWidth			= element.attribute("stroke-width").value();
				StrokeColor			= element.attribute("stroke-color").value();
				MarkerStartWidth	= element.attribute("marker-start-width").value();
				MarkerStartCenter	= element.attribute("marker-start-center").value();
				MarkerEndWidth	= element.attribute("marker-end-width").value();
				MarkerEndCenter	= element.attribute("marker-end-center").value();
				StrokeLineJoin	= element.attribute("stroke-linejoin").value();
				Fill		= element.attribute("fill").value();
				FillColor	= element.attribute("fill-color").value();
				FillImageName	= element.attribute("fill-image-name").value();
				FillGradientName	= element.attribute("fill-gradient-name").value();
				FillImageWidth	= element.attribute("fill-image-width").value();
				FillImageHeight	= element.attribute("fill-image-height").value();
				Repeat			= element.attribute("repeat").value();
				TextareaHorizontalAlign = element.attribute("textarea-horizontal-align").value();
				TextareaVerticalAlign	= element.attribute("textarea-vertical-align").value();
				AutoGrowHeight	= element.attribute("auto-grow-height").value();
				AutoGrowWidth	= element.attribute("auto-grow-width").value();
				FitToSize		= element.attribute("fit-to-size").value();
				MinHeight		= element.attribute("min-height").value();
				MinWidth		= element.attribute("min-width").value();
				MaxHeight		= element.attribute("max-height").value();
				MaxWidth		= element.attribute("max-width").value();
				PaddingTop		= element.attribute("padding-top").value();
				PaddingBottom	= element.attribute("padding-bottom").value();
				PaddingLeft		= element.attribute("padding-left").value();
				PaddingRight	= element.attribute("padding-right").value();
				Wrap			= element.attribute("wrap").value();
				RunThrough		= element.attribute("run-through").value();
				Protect			= element.attribute("protect").value();
				Opacity			= element.attribute("opacity").value();
				ShadowOpacity	= element.attribute("shadow-opacity").value();


				if(element.element("list-style").exist())
					ListStyle.reset(new Odt::Logic::ListStyle(element.element("list-style")));

//				Logic::Wrap wrap(element);
//				Wrap = wrap;
//				Logic::Position position(element);
//				Position = position;
			}


			const XML::XNode Graphic::toXML() const
			{
				return
					XML::XElement(ns.style + "graphic-properties",			
						XML::XAttribute(ns.draw + "stroke", Stroke) +
						XML::XAttribute(ns.svg + "stroke-width", StrokeWidth) +
						XML::XAttribute(ns.draw + "stroke-color", StrokeColor) + //TODO : здесь наверно не ns.draw, а ns.svg
						XML::XAttribute(ns.draw + "marker-start-width", MarkerStartWidth) +
						XML::XAttribute(ns.draw + "marker-start-center", MarkerStartCenter) +
						XML::XAttribute(ns.draw + "marker-end-width", MarkerEndWidth) +
						XML::XAttribute(ns.draw + "marker-end-center", MarkerEndCenter) +
						XML::XAttribute(ns.draw + "stroke-linejoin", StrokeLineJoin) +
						XML::XAttribute(ns.draw + "fill", Fill) +
						XML::XAttribute(ns.draw + "fill-color", FillColor) +
						XML::XAttribute(ns.draw + "fill-image-name", FillImageName) +
						XML::XAttribute(ns.draw + "fill-gradient-name", FillGradientName) +
						XML::XAttribute(ns.draw + "fill-image-width", FillImageWidth) +
						XML::XAttribute(ns.draw + "fill-image-height", FillImageHeight) +
						XML::XAttribute(ns.draw + "repeat", Repeat) +
						XML::XAttribute(ns.draw + "textarea-horizontal-align", TextareaHorizontalAlign) +
						XML::XAttribute(ns.draw + "textarea-vertical-align", TextareaVerticalAlign) +
						XML::XAttribute(ns.draw + "auto-grow-height", AutoGrowHeight) +
						XML::XAttribute(ns.draw + "auto-grow-width", AutoGrowWidth) +
						XML::XAttribute(ns.draw + "fit-to-size", FitToSize) +
						XML::XAttribute(ns.fo + "min-height", MinHeight) +
						XML::XAttribute(ns.fo + "min-width", MinWidth) +
						XML::XAttribute(ns.fo + "max-height", MaxHeight) +
						XML::XAttribute(ns.fo + "max-width", MaxWidth) +
						XML::XAttribute(ns.style + "vertical-pos", VerticalPos) +
						XML::XAttribute(ns.style + "vertical-rel", VerticalRel) +
						XML::XAttribute(ns.style + "horizontal-pos", HorizontalPos) +
						XML::XAttribute(ns.style + "horizontal-rel", HorizontalRel) +
						XML::XAttribute(ns.fo + "background-color", BackgroundColor) +
						XML::XAttribute(ns.style + "background-transparency", BackgroundTransparency) +
						XML::XAttribute(ns.fo + "padding", Padding) +
						XML::XAttribute(ns.fo + "padding-top", PaddingTop) +
						XML::XAttribute(ns.fo + "padding-bottom", PaddingBottom) +
						XML::XAttribute(ns.fo + "padding-left", PaddingLeft) +
						XML::XAttribute(ns.fo + "padding-right", PaddingRight) +
						XML::XAttribute(ns.fo + "border-right", BorderRight) +
						XML::XAttribute(ns.fo + "border-left", BorderLeft) +
						XML::XAttribute(ns.fo + "border-top", BorderTop) +
						XML::XAttribute(ns.fo + "border-bottom", BorderBottom) +
						XML::XAttribute(ns.fo + "border", Border) +						
						XML::XAttribute(ns.draw + "shadow", Shadow)	+
						XML::XAttribute(ns.style + "mirror", Mirror) +
						XML::XAttribute(ns.draw + "shadow-offset-x", ShadowOffsetX) + 
						XML::XAttribute(ns.draw + "shadow-offset-y", ShadowOffsetY) +
						XML::XAttribute(ns.draw + "start-line-spacing-horizontal", StartLineSpacingHorizontal) +
						XML::XAttribute(ns.draw + "start-line-spacing-vertical", StartLineSpacingVertical) +
						XML::XAttribute(ns.draw + "end-line-spacing-horizontal", EndLineSpacingHorizontal) +
						XML::XAttribute(ns.draw + "end-line-spacing-vertical", EndLineSpacingVertical) +
						XML::XAttribute(ns.style + "flow-with-text", FlowWithText) +						
						XML::XAttribute(ns.fo + "clip", Clip) +
						XML::XAttribute(ns.draw + "luminance", Luminance) +
						XML::XAttribute(ns.draw + "contrast", Contrast) +
						XML::XAttribute(ns.draw + "red", Red) +
						XML::XAttribute(ns.draw + "green", Green) +
						XML::XAttribute(ns.draw + "blue", Blue) +
						XML::XAttribute(ns.draw + "gamma", Gamma) +
						XML::XAttribute(ns.draw + "color-inversion", ColorInversion) +
						XML::XAttribute(ns.draw + "image-opacity", ImageOpacity) +
						XML::XAttribute(ns.draw + "color-mode", ColorMode) +
						XML::XAttribute(ns.text + "anchor-type", AnchorType) +
						XML::XAttribute(ns.svg + "x", SvgX) + 
						XML::XAttribute(ns.svg + "y", SvgY) +		
//						Wrap->toXML(ns) +
//						Position->toXML(ns) +
						XML::XAttribute(ns.draw + "ole-draw-aspect", OleDrawAspect) +
						XML::XAttribute(ns.draw + "visible-area-top", VisibleAreaTop) +
						XML::XAttribute(ns.draw + "visible-area-width", VisibleAreaWidth) +
						XML::XAttribute(ns.draw + "visible-area-height", VisibleAreaHeight) +
						XML::XAttribute(ns.style + "wrap", Wrap) +
						XML::XAttribute(ns.style + "run-through", RunThrough) +
						XML::XAttribute(ns.style + "protect", Protect) +
						XML::XAttribute(ns.draw + "opacity", Opacity) +
						XML::XAttribute(ns.draw + "shadow-opacity", ShadowOpacity) +
						XML::Write(ListStyle)
				);
			}


			const bool Graphic::isSimple() const
			{
				return true;
			}


			const Graphic Graphic::merge(const Graphic& prev, const Graphic& current)
			{
				Graphic properties;
				properties.ShadowOffsetX = ::merge(prev.ShadowOffsetX, current.ShadowOffsetX);
				properties.ShadowOffsetY = ::merge(prev.ShadowOffsetY, current.ShadowOffsetY);
				properties.StartLineSpacingHorizontal = ::merge(prev.StartLineSpacingHorizontal, current.StartLineSpacingHorizontal);
				properties.StartLineSpacingVertical = ::merge(prev.StartLineSpacingVertical, current.StartLineSpacingVertical);
				properties.EndLineSpacingHorizontal = ::merge(prev.EndLineSpacingHorizontal, current.EndLineSpacingHorizontal);
				properties.EndLineSpacingVertical = ::merge(prev.EndLineSpacingVertical, current.EndLineSpacingVertical);
				properties.FlowWithText = ::merge(prev.FlowWithText, current.FlowWithText);
				properties.Mirror = ::merge(prev.Mirror, current.Mirror);
				properties.Clip = ::merge(prev.Clip, current.Clip);
				properties.Luminance = ::merge(prev.Luminance, current.Luminance);
				properties.Contrast = ::merge(prev.Contrast, current.Contrast);
				properties.Red = ::merge(prev.Red, current.Red);
				properties.Green = ::merge(prev.Green, current.Green);
				properties.Blue = ::merge(prev.Blue, current.Blue);
				properties.Gamma = ::merge(prev.Gamma, current.Gamma);
				properties.ColorInversion = ::merge(prev.ColorInversion, current.ColorInversion);
				properties.ImageOpacity = ::merge(prev.ImageOpacity, current.ImageOpacity);
				properties.ColorMode = ::merge(prev.ColorMode, current.ColorMode);
				properties.AnchorType = ::merge(prev.AnchorType, current.AnchorType);
				properties.SvgX = ::merge(prev.SvgX, current.SvgX);
				properties.SvgY = ::merge(prev.SvgY, current.SvgY);
				properties.Wrap = ::merge(prev.Wrap, current.Wrap);	// TODO Сделать нормальный wrap
				properties.RunThrough = ::merge(prev.RunThrough, current.RunThrough);
				properties.NumberWrappedParagraphs = ::merge(prev.NumberWrappedParagraphs, current.NumberWrappedParagraphs);
				properties.VerticalPos = ::merge(prev.VerticalPos, current.VerticalPos);
				properties.VerticalRel = ::merge(prev.VerticalRel, current.VerticalRel);
				properties.HorizontalPos = ::merge(prev.HorizontalPos, current.HorizontalPos);
				properties.HorizontalRel = ::merge(prev.HorizontalRel, current.HorizontalRel);				
				properties.OleDrawAspect = ::merge(prev.OleDrawAspect, current.OleDrawAspect);
				properties.VisibleAreaTop = ::merge(prev.VisibleAreaTop, current.VisibleAreaTop);
				properties.VisibleAreaWidth = ::merge(prev.VisibleAreaWidth, current.VisibleAreaWidth);
				properties.VisibleAreaHeight = ::merge(prev.VisibleAreaHeight, current.VisibleAreaHeight);
				properties.Padding = ::merge(prev.Padding, current.Padding);
				properties.BorderRight = ::merge(prev.BorderRight, current.BorderRight);
				properties.BorderLeft = ::merge(prev.BorderLeft, current.BorderLeft);
				properties.BorderTop = ::merge(prev.BorderTop, current.BorderTop);
				properties.BorderBottom = ::merge(prev.BorderBottom, current.BorderBottom);
				properties.Border = ::merge(prev.Border, current.Border);	
				properties.Shadow = ::merge(prev.Shadow, current.Shadow);
				properties.BackgroundColor = ::merge(prev.BackgroundColor, current.BackgroundColor);
				properties.BackgroundTransparency = ::merge(prev.BackgroundTransparency, current.BackgroundTransparency);
				properties.Stroke = ::merge(prev.Stroke, current.Stroke);
				properties.StrokeWidth = ::merge(prev.StrokeWidth, current.StrokeWidth);
				properties.StrokeColor = ::merge(prev.StrokeColor, current.StrokeColor);
				properties.MarkerStartWidth = ::merge(prev.MarkerStartWidth, current.MarkerStartWidth);
				properties.MarkerStartCenter = ::merge(prev.MarkerStartCenter, current.MarkerStartCenter);
				properties.MarkerEndWidth = ::merge(prev.MarkerEndWidth, current.MarkerEndWidth);
				properties.MarkerEndCenter = ::merge(prev.MarkerEndCenter, current.MarkerEndCenter);
				properties.StrokeLineJoin = ::merge(prev.StrokeLineJoin, current.StrokeLineJoin);
				properties.Fill = ::merge(prev.Fill, current.Fill);
				properties.FillColor = ::merge(prev.FillColor, current.FillColor);
				properties.FillImageName = ::merge(prev.FillImageName, current.FillImageName);
				properties.FillGradientName = ::merge(prev.FillGradientName, current.FillGradientName);
				properties.FillImageWidth = ::merge(prev.FillImageWidth, current.FillImageWidth);
				properties.FillImageHeight = ::merge(prev.FillImageHeight, current.FillImageHeight);
				properties.Repeat = ::merge(prev.Repeat, current.Repeat);
				properties.TextareaHorizontalAlign = ::merge(prev.TextareaHorizontalAlign, current.TextareaHorizontalAlign);
				properties.TextareaVerticalAlign = ::merge(prev.TextareaVerticalAlign, current.TextareaVerticalAlign);
				properties.AutoGrowHeight = ::merge(prev.AutoGrowHeight, current.AutoGrowHeight);
				properties.AutoGrowWidth = ::merge(prev.AutoGrowWidth, current.AutoGrowWidth);
				properties.FitToSize = ::merge(prev.FitToSize, current.FitToSize);
				properties.MinHeight = ::merge(prev.MinHeight, current.MinHeight);
				properties.MinWidth = ::merge(prev.MinWidth, current.MinWidth);
				properties.MaxHeight = ::merge(prev.MaxHeight, current.MaxHeight);
				properties.MaxWidth = ::merge(prev.MaxWidth, current.MaxWidth);
				properties.PaddingTop = ::merge(prev.PaddingTop, current.PaddingTop);
				properties.PaddingBottom = ::merge(prev.PaddingBottom, current.PaddingBottom);
				properties.PaddingLeft = ::merge(prev.PaddingLeft, current.PaddingLeft);
				properties.PaddingRight = ::merge(prev.PaddingRight, current.PaddingRight);
				properties.Protect = ::merge(prev.Protect, current.Protect);
				properties.Opacity = ::merge(prev.Opacity, current.Opacity);
				properties.ShadowOpacity = ::merge(prev.ShadowOpacity, current.ShadowOpacity);
				return properties;
			}

		} // namespace Property
	} // namespace Logic
} // namespace Odt