#include "BldGraphic.h"

namespace PPTX
{
	namespace Logic
	{

		BldGraphic::BldGraphic()
		{
		}

		BldGraphic::~BldGraphic()
		{
		}

		BldGraphic::BldGraphic(const XML::XNode& node)
		{
			fromXML(node);
		}

		const BldGraphic& BldGraphic::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BldGraphic::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			bldSub = element.element("bldSub");
			spid = element.attribute("spid").value();
			grpId = element.attribute("grpId").value();
			uiExpand = element.attribute("uiExpand").value();

			FillParentPointersForChilds();
		}

		const XML::XNode BldGraphic::toXML() const
		{
			return XML::XElement(ns.p + "bldGraphic",
				XML::WriteIf(XML::XElement(ns.p + "bldAsOne"), !bldSub.is_init()) +
				XML::Write(bldSub) +
				XML::XAttribute("spid", spid) +
				XML::XAttribute("grpId", grpId) +
				XML::XAttribute("uiExpand", uiExpand)
				);
		}

		void BldGraphic::FillParentPointersForChilds()
		{
			if(bldSub.is_init())
				bldSub->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX