#include "CNvCxnSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		CNvCxnSpPr::CNvCxnSpPr()
		{
		}


		CNvCxnSpPr::~CNvCxnSpPr()
		{
		}
	

		CNvCxnSpPr::CNvCxnSpPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const CNvCxnSpPr& CNvCxnSpPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void CNvCxnSpPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if(element.element("cxnSpLocks").exist()){
				XML::XElement l_Locks = element.element("cxnSpLocks");

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
			}

			if(element.element("stCxn").exist()){
				stCxn_id = element.element("stCxn").attribute("id").value();
				stCxn_idx = element.element("stCxn").attribute("idx").value();
			}
			if(element.element("endCxn").exist()){
				endCxn_id = element.element("endCxn").attribute("id").value();
				endCxn_idx = element.element("endCxn").attribute("idx").value();
			}
		}


		const XML::XNode CNvCxnSpPr::toXML() const
		{
			XML::XElement element(ns.p + "cNvCxnSpPr");
			XML::XElement locks(ns.a + "cxnSpLocks",
				XML::XAttribute("noAdjustHandles", noAdjustHandles) +
				XML::XAttribute("noChangeArrowheads", noChangeArrowheads) +
				XML::XAttribute("noChangeAspect", noChangeAspect) +
				XML::XAttribute("noChangeShapeType", noChangeShapeType) +
				XML::XAttribute("noEditPoints", noEditPoints) +
				XML::XAttribute("noGrp", noGrp) +
				XML::XAttribute("noMove", noMove) +
				XML::XAttribute("noResize", noResize) +
				XML::XAttribute("noRot", noRot) +
				XML::XAttribute("noSelect", noSelect)
			);

			if(!locks.Attributes.empty())
				element.Add(locks);

			if((stCxn_id.is_init()) || (stCxn_idx.is_init()))
			{
				element.Add(XML::XElement(ns.a + "stCxn",
							XML::XAttribute("id", stCxn_id) +
							XML::XAttribute("idx", stCxn_idx))
							);
			}

			if((endCxn_id.is_init()) || (endCxn_idx.is_init()))
			{
				element.Add(XML::XElement(ns.a + "endCxn",
							XML::XAttribute("id", endCxn_id) +
							XML::XAttribute("idx", endCxn_idx))
							);
			}

			return element;
		}

	} // namespace Logic
} // namespace PPTX