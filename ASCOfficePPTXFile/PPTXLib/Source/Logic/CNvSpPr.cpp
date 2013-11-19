#include "CNvSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		CNvSpPr::CNvSpPr()
		{
		}


		CNvSpPr::~CNvSpPr()
		{
		}
	

		CNvSpPr::CNvSpPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const CNvSpPr& CNvSpPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void CNvSpPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			txBox = element.attribute("txBox").value();
			if(element.element("spLocks").exist()){
				XML::XElement l_Locks = element.element("spLocks");

				noAdjustHandles = l_Locks.attribute("noAdjustHandles").value();
				noChangeArrowheads = l_Locks.attribute("noChangeArrowheads").value();
				noChangeAspect = l_Locks.attribute("noChangeAspect").value();
				noChangeShapeType = l_Locks.attribute("noChangeShapeType").value();
				noEditPoints = l_Locks.attribute("noEditPoints").value();
				noGrp = l_Locks.attribute("noGrp").value();
				noMove = l_Locks.attribute("noMove").value();
				noResize = l_Locks.attribute("noResize").value();
				noRot = l_Locks.attribute("noRot").value();
				noSelect = l_Locks.attribute("noSelect").value();
				noTextEdit = l_Locks.attribute("noTextEdit").value();
			}
		}


		const XML::XNode CNvSpPr::toXML() const
		{
			XML::XElement element(ns.a + "spLocks",
				XML::XAttribute("noAdjustHandles", noAdjustHandles) +
				XML::XAttribute("noChangeArrowheads", noChangeArrowheads) +
				XML::XAttribute("noChangeAspect", noChangeAspect) +
				XML::XAttribute("noChangeShapeType", noChangeShapeType) +
				XML::XAttribute("noEditPoints", noEditPoints) +
				XML::XAttribute("noGrp", noGrp) +
				XML::XAttribute("noMove", noMove) +
				XML::XAttribute("noResize", noResize) +
				XML::XAttribute("noRot", noRot) +
				XML::XAttribute("noSelect", noSelect) +
				XML::XAttribute("noTextEdit", noTextEdit)
			);

			if(!element.Attributes.empty())
				return XML::XElement(ns.p + "cNvSpPr",XML::XAttribute("txBox", txBox) + element);
			return XML::XElement(ns.p + "cNvSpPr", XML::XAttribute("txBox", txBox));
		}

	} // namespace Logic
} // namespace PPTX