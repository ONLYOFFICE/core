#include "CTn.h"

namespace PPTX
{
	namespace Logic
	{

		CTn::CTn()
		{
		}

		CTn::~CTn()
		{
		}

		CTn::CTn(const XML::XNode& node)
		{
			fromXML(node);
		}

		const CTn& CTn::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void CTn::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			accel = element.attribute("accel").value();
			afterEffect = element.attribute("afterEffect").value();
			autoRev = element.attribute("autoRev").value();
			bldLvl = element.attribute("bldLvl").value();
			decel = element.attribute("decel").value();
			display = element.attribute("display").value();
			dur = element.attribute("dur").value();
			evtFilter = element.attribute("evtFilter").value();
			fill = element.attribute("fill").value();
			grpId = element.attribute("grpId").value();
			id = element.attribute("id").value();
			masterRel = element.attribute("masterRel").value();
			nodePh = element.attribute("nodePh").value();
			nodeType = element.attribute("nodeType").value();
			presetClass = element.attribute("presetClass").value();
			presetID = element.attribute("presetID").value();
			presetSubtype = element.attribute("presetSubtype").value();
			repeatCount = element.attribute("repeatCount").value();
			repeatDur = element.attribute("repeatDur").value();
			restart = element.attribute("restart").value();
			spd = element.attribute("spd").value();
			syncBehavior = element.attribute("syncBehavior").value();
			tmFilter = element.attribute("tmFilter").value();

			stCondLst = element.element("stCondLst");
			endCondLst = element.element("endCondLst");
			endSync = element.element("endSync");
			iterate = element.element("iterate");
			childTnLst = element.element("childTnLst");
			subTnLst = element.element("subTnLst");

			FillParentPointersForChilds();
		}

		const XML::XNode CTn::toXML() const
		{
			return XML::XElement(ns.p + "cTn",
				XML::XAttribute("id", id) +
				XML::XAttribute("presetID", presetID) +
				XML::XAttribute("presetClass", presetClass) +
				XML::XAttribute("presetSubtype", presetSubtype) +
				XML::XAttribute("dur", dur) +
				XML::XAttribute("repeatCount", repeatCount) +
				XML::XAttribute("repeatDur", repeatDur) +
				XML::XAttribute("spd", spd) +
				XML::XAttribute("accel", accel) +
				XML::XAttribute("decel", decel) +
				XML::XAttribute("autoRev", autoRev) +
				XML::XAttribute("restart", restart) +
				XML::XAttribute("fill", fill) +
				XML::XAttribute("syncBehavior", syncBehavior) +
				XML::XAttribute("tmFilter", tmFilter) +
				XML::XAttribute("evtFilter", evtFilter) +
				XML::XAttribute("display", display) +
				XML::XAttribute("masterRel", masterRel) +
				XML::XAttribute("bldLvl", bldLvl) +
				XML::XAttribute("grpId", grpId) +
				XML::XAttribute("afterEffect", afterEffect) +
				XML::XAttribute("nodeType", nodeType) +
				XML::XAttribute("nodePh", nodePh) +
				XML::Write(stCondLst) +
				XML::Write(endCondLst) +
				XML::Write(endSync) +
				XML::Write(iterate) +
				XML::Write(childTnLst) +
				XML::Write(subTnLst)
				);
		}

		void CTn::FillParentPointersForChilds()
		{
			if(stCondLst.is_init())
				stCondLst->SetParentPointer(*this);
			if(endCondLst.is_init())
				endCondLst->SetParentPointer(*this);
			if(endSync.is_init())
				endSync->SetParentPointer(*this);
			if(iterate.is_init())
				iterate->SetParentPointer(*this);
			if(childTnLst.is_init())
				childTnLst->SetParentPointer(*this);
			if(subTnLst.is_init())
				subTnLst->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX