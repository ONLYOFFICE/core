#include "CNvGrpSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		CNvGrpSpPr::CNvGrpSpPr()
		{
		}


		CNvGrpSpPr::~CNvGrpSpPr()
		{
		}
	

		CNvGrpSpPr::CNvGrpSpPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const CNvGrpSpPr& CNvGrpSpPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void CNvGrpSpPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if(element.element("grpSpLocks").exist()){
				XML::XElement l_Locks = element.element("grpSpLocks");

				noChangeAspect = l_Locks.attribute("noChangeAspect").value();
				noGrp = l_Locks.attribute("noGrp").value();
				noMove = l_Locks.attribute("noMove").value();
				noResize = l_Locks.attribute("noResize").value();
				noRot = l_Locks.attribute("noRot").value();
				noSelect = l_Locks.attribute("noSelect").value();
				noUngrp = l_Locks.attribute("noUngrp").value();
			}
		}


		const XML::XNode CNvGrpSpPr::toXML() const
		{
			XML::XElement element(ns.a + "grpSpLocks",
				XML::XAttribute("noChangeAspect", noChangeAspect) +
				XML::XAttribute("noGrp", noGrp) +
				XML::XAttribute("noMove", noMove) +
				XML::XAttribute("noResize", noResize) +
				XML::XAttribute("noRot", noRot) +
				XML::XAttribute("noSelect", noSelect) +
				XML::XAttribute("noUngrp", noUngrp)
			);

			if(!element.Attributes.empty())
				return XML::XElement(ns.p + "cNvGrpSpPr", element);
			else return XML::XElement(ns.p + "cNvGrpSpPr");
		}

	} // namespace Logic
} // namespace PPTX