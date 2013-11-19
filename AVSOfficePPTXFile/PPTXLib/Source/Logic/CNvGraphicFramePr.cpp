#include "CNvGraphicFramePr.h"

namespace PPTX
{
	namespace Logic
	{

		CNvGraphicFramePr::CNvGraphicFramePr()
		{
		}


		CNvGraphicFramePr::~CNvGraphicFramePr()
		{
		}
	

		CNvGraphicFramePr::CNvGraphicFramePr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const CNvGraphicFramePr& CNvGraphicFramePr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void CNvGraphicFramePr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if(element.element("graphicFrameLocks").exist()){
				XML::XElement l_Locks = element.element("graphicFrameLocks");

				noChangeAspect = l_Locks.attribute("noChangeAspect").value();
				noDrilldown = l_Locks.attribute("noDrilldown").value();
				noGrp = l_Locks.attribute("noGrp").value();
				noMove = l_Locks.attribute("noMove").value();
				noResize = l_Locks.attribute("noResize").value();
				noSelect = l_Locks.attribute("noSelect").value();
			}
		}


		const XML::XNode CNvGraphicFramePr::toXML() const
		{
			XML::XElement element(ns.a + "graphicFrameLocks",
				XML::XAttribute("noChangeAspect", noChangeAspect) +
				XML::XAttribute("noDrilldown", noDrilldown) +
				XML::XAttribute("noGrp", noGrp) +
				XML::XAttribute("noMove", noMove) +
				XML::XAttribute("noResize", noResize) +
				XML::XAttribute("noSelect", noSelect)
			);
			if(!element.Attributes.empty())
				return XML::XElement(ns.p + "cNvGraphicFramePr", element);
			return XML::XElement(ns.p + "cNvGraphicFramePr");
		}

	} // namespace Logic
} // namespace PPTX