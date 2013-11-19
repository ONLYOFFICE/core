#include "BldOleChart.h"

namespace PPTX
{
	namespace Logic
	{

		BldOleChart::BldOleChart()
		{
		}

		BldOleChart::~BldOleChart()
		{
		}

		BldOleChart::BldOleChart(const XML::XNode& node)
		{
			fromXML(node);
		}

		const BldOleChart& BldOleChart::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BldOleChart::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			spid = element.attribute("spid").value();
			grpId = element.attribute("grpId").value();
			uiExpand = element.attribute("uiExpand").value();
			bld = element.attribute("bld").value();
			animBg = element.attribute("animBg").value();

			FillParentPointersForChilds();
		}

		const XML::XNode BldOleChart::toXML() const
		{
			return XML::XElement(ns.p + "bldOleChart",
				XML::XAttribute("spid", spid) +
				XML::XAttribute("grpId", grpId) +
				XML::XAttribute("uiExpand", uiExpand) +
				XML::XAttribute("bld", bld) +
				XML::XAttribute("animBg", animBg)
				);
		}
	} // namespace Logic
} // namespace PPTX