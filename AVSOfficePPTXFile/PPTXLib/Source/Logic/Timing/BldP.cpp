#include "BldP.h"

namespace PPTX
{
	namespace Logic
	{

		BldP::BldP()
		{
		}

		BldP::~BldP()
		{
		}

		BldP::BldP(const XML::XNode& node)
		{
			fromXML(node);
		}

		const BldP& BldP::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BldP::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			tmplLst = element.element("tmplLst");

			spid = element.attribute("spid").value();
			grpId = element.attribute("grpId").value();
			uiExpand = element.attribute("uiExpand").value();
			build = element.attribute("build").value();
			bldLvl = element.attribute("bldLvl").value();
			animBg = element.attribute("animBg").value();
			autoUpdateAnimBg = element.attribute("autoUpdateAnimBg").value();
			rev = element.attribute("rev").value();
			advAuto = element.attribute("advAuto").value();

			FillParentPointersForChilds();
		}

		const XML::XNode BldP::toXML() const
		{
			return XML::XElement(ns.p + "bldP",
				XML::XAttribute("spid", spid) +
				XML::XAttribute("grpId", grpId) +
				XML::XAttribute("uiExpand", uiExpand) +
				XML::XAttribute("build", build) +
				XML::XAttribute("bldLvl", bldLvl) +
				XML::XAttribute("animBg", animBg) +
				XML::XAttribute("autoUpdateAnimBg", autoUpdateAnimBg) +
				XML::XAttribute("rev", rev) +
				XML::XAttribute("advAuto", advAuto) +
				XML::Write(tmplLst)
				);
		}

		void BldP::FillParentPointersForChilds()
		{
			if(tmplLst.is_init())
				tmplLst->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX