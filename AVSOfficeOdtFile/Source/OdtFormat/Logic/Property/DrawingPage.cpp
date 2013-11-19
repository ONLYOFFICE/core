
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "DrawingPage.h"

namespace Odt
{
	namespace Logic
	{
		namespace Property
		{
			DrawingPage::DrawingPage()
			{
			}

			DrawingPage::~DrawingPage()
			{
			}

			DrawingPage::DrawingPage(const XML::XNode& node)
			{
				fromXML(node);
			}

			const DrawingPage& DrawingPage::operator =(const XML::XNode& node)
			{
				fromXML(node);
				return *this;
			}

			void DrawingPage::fromXML(const XML::XNode& node)
			{
				const XML::XElement element(node);

				BackgroundVisible = element.attribute("background-visible").value();
				BackgroundObjectsVisible = element.attribute("background-objects-visible").value();
				DisplayHeader = element.attribute("display-header").value();
				DisplayFooter = element.attribute("display-footer").value();
				DisplayPageNumber = element.attribute("display-page-number").value();
				DisplayDateTime = element.attribute("display-date-time").value();
				BackgroundSize = element.attribute("background-size").value();
				Fill = element.attribute("fill").value();
				FillColor = element.attribute("fill-color").value();
				if(element.attribute("fill-image-width").exist())
					Width = element.attribute("fill-image-width").value().ToString();
				if(element.attribute("fill-image-height").exist())
					Height = element.attribute("fill-image-height").value().ToString();
				FillImageName = element.attribute("fill-image-name").value();
				FillGradientName = element.attribute("fill-gradient-name").value();
				Repeat = element.attribute("repeat").value();
				SmilType = element.attribute(ns.smil + "type").value();
				SmilSubType = element.attribute(ns.smil + "subtype").value();
			}

			const XML::XNode DrawingPage::toXML() const
			{
				return
					XML::XElement(ns.style + "drawing-page-properties",			
					XML::XAttribute(ns.presentation + "background-visible", BackgroundVisible) +
					XML::XAttribute(ns.presentation + "background-objects-visible", BackgroundObjectsVisible) +
					XML::XAttribute(ns.presentation + "display-header", DisplayHeader) +
					XML::XAttribute(ns.presentation + "display-footer", DisplayFooter) +
					XML::XAttribute(ns.presentation + "display-page-number", DisplayPageNumber) +
					XML::XAttribute(ns.presentation + "display-date-time", DisplayDateTime) +
					XML::XAttribute(ns.draw + "background-size", BackgroundSize) +
					XML::XAttribute(ns.draw + "fill", Fill) +
					XML::XAttribute(ns.draw + "fill-color", FillColor) +
					XML::XAttribute(ns.draw + "fill-image-width", Width) +
					XML::XAttribute(ns.draw + "fill-image-height", Height) +
					XML::XAttribute(ns.draw + "fill-image-name", FillImageName) +
					XML::XAttribute(ns.draw + "fill-gradient-name", FillGradientName) +
					XML::XAttribute(ns.style + "repeat", Repeat) +
					XML::XAttribute(ns.smil + "type", SmilType) +
					XML::XAttribute(ns.smil + "subtype", SmilSubType)
				);
			}

			const bool DrawingPage::isSimple() const
			{
				return true;
			}

			const DrawingPage DrawingPage::merge(const DrawingPage& prev, const DrawingPage& current)
			{
				DrawingPage properties;
				properties.BackgroundVisible		= ::merge(prev.BackgroundVisible, current.BackgroundVisible);
				properties.BackgroundObjectsVisible = ::merge(prev.BackgroundObjectsVisible, current.BackgroundObjectsVisible);
				properties.DisplayHeader			= ::merge(prev.DisplayHeader, current.DisplayHeader);
				properties.DisplayFooter			= ::merge(prev.DisplayFooter, current.DisplayFooter);
				properties.DisplayPageNumber		= ::merge(prev.DisplayPageNumber, current.DisplayPageNumber);
				properties.DisplayDateTime			= ::merge(prev.DisplayDateTime, current.DisplayDateTime);
				properties.BackgroundSize			= ::merge(prev.BackgroundSize, current.BackgroundSize);
				properties.Fill						= ::merge(prev.Fill, current.Fill);
				properties.FillColor				= ::merge(prev.FillColor, current.FillColor);
				properties.Width					= ::merge(prev.Width, current.Width);
				properties.Height					= ::merge(prev.Height, current.Height);
				properties.FillImageName			= ::merge(prev.FillImageName, current.FillImageName);
				properties.FillGradientName			= ::merge(prev.FillGradientName, current.FillGradientName);
				properties.Repeat					= ::merge(prev.Repeat, current.Repeat);
				properties.SmilType					= ::merge(prev.SmilType, current.SmilType);
				properties.SmilSubType				= ::merge(prev.SmilSubType, current.SmilSubType);
				return properties;
			}

		} // namespace Property
	} // namespace Logic
} // namespace Odt