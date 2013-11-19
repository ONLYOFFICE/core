#include "BldDgm.h"

namespace PPTX
{
	namespace Logic
	{

		BldDgm::BldDgm()
		{
		}

		BldDgm::~BldDgm()
		{
		}

		BldDgm::BldDgm(const XML::XNode& node)
		{
			fromXML(node);
		}

		const BldDgm& BldDgm::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BldDgm::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			spid = element.attribute("spid").value();
			grpId = element.attribute("grpId").value();
			uiExpand = element.attribute("uiExpand").value();
			bld = element.attribute("bld").value();

			FillParentPointersForChilds();
		}

		const XML::XNode BldDgm::toXML() const
		{
			return XML::XElement(ns.p + "bldDgm",
				XML::XAttribute("spid", spid) +
				XML::XAttribute("grpId", grpId) +
				XML::XAttribute("uiExpand", uiExpand) +
				XML::XAttribute("bld", bld)
				);
		}
	} // namespace Logic
} // namespace PPTX