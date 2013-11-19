#include "GraphicEl.h"

namespace PPTX
{
	namespace Logic
	{

		GraphicEl::GraphicEl()
		{
		}

		GraphicEl::~GraphicEl()
		{
		}

		GraphicEl::GraphicEl(const XML::XNode& node)
		{
			fromXML(node);
		}

		const GraphicEl& GraphicEl::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void GraphicEl::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if(element.element("chart").exist())
			{
				chartBuildStep = element.element("chart").attribute("bldStep").value();
				seriesIdx = element.element("chart").attribute("seriesIdx").value();
				categoryIdx = element.element("chart").attribute("categoryIdx").value();
			}
			else if(element.element("dgm").exist())
			{
				dgmBuildStep = element.element("dgm").attribute("bldStep").value();
				dgmId = element.element("chart").attribute("id").value();
			}
		}

		const XML::XNode GraphicEl::toXML() const
		{
			if(chartBuildStep.is_init())
				return XML::XElement(ns.p + "graphicEl",
				XML::XElement(ns.p + "chart",
				XML::XAttribute("seriesIdx", seriesIdx) + XML::XAttribute("categoryIdx", categoryIdx) + XML::XAttribute("bldStep", chartBuildStep)));
			return XML::XElement(ns.p + "graphicEl",
				XML::XElement(ns.p + "dgm", XML::XAttribute("id", dgmId) + XML::XAttribute("bldStep", dgmBuildStep)));
		}
	} // namespace Logic
} // namespace PPTX