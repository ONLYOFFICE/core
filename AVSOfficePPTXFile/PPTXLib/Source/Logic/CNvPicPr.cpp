#include "CNvPicPr.h"

namespace PPTX
{
	namespace Logic
	{

		CNvPicPr::CNvPicPr()
		{
		}


		CNvPicPr::~CNvPicPr()
		{
		}
	

		CNvPicPr::CNvPicPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const CNvPicPr& CNvPicPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void CNvPicPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			preferRelativeResize = element.attribute("preferRelativeResize").value();
			if(element.element("picLocks").exist()){
				XML::XElement l_Locks = element.element("picLocks");

				noAdjustHandles = l_Locks.attribute("noAdjustHandles").value();
				noChangeArrowheads = l_Locks.attribute("noChangeArrowheads").value();
				noChangeAspect = l_Locks.attribute("noChangeAspect").value();
				noChangeShapeType = l_Locks.attribute("noChangeShapeType").value();
				noCrop = l_Locks.attribute("noCrop").value();
				noEditPoints = l_Locks.attribute("noEditPoints").value();
				noGrp = l_Locks.attribute("noGrp").value();
				noMove = l_Locks.attribute("noMove").value();
				noResize = l_Locks.attribute("noResize").value();
				noRot = l_Locks.attribute("noRot").value();
				noSelect = l_Locks.attribute("noSelect").value();
			}
		}


		const XML::XNode CNvPicPr::toXML() const
		{
			XML::XElement element(ns.a + "picLocks",
					XML::XAttribute("noAdjustHandles", noAdjustHandles) +
					XML::XAttribute("noChangeAspect", noChangeAspect) +
					XML::XAttribute("noChangeArrowheads", noChangeArrowheads) +
					XML::XAttribute("noChangeShapeType", noEditPoints) +
					XML::XAttribute("noEditPoints", noEditPoints) +
					XML::XAttribute("noGrp", noGrp) +
					XML::XAttribute("noMove", noMove) +
					XML::XAttribute("noResize", noResize) +
					XML::XAttribute("noRot", noRot) +
					XML::XAttribute("noSelect", noSelect) +
					XML::XAttribute("noCrop", noCrop)
				);

			if(!element.Attributes.empty())
				return XML::XElement(ns.p + "cNvPicPr", XML::XAttribute("preferRelativeResize", preferRelativeResize) + element);
			return XML::XElement(ns.p + "cNvPicPr", XML::XAttribute("preferRelativeResize", preferRelativeResize));
		}

	} // namespace Logic
} // namespace PPTX