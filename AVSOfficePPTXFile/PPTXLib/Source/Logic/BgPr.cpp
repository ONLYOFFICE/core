#include "BgPr.h"

namespace PPTX
{
	namespace Logic
	{

		BgPr::BgPr()
		{
		}


		BgPr::~BgPr()
		{
		}


		BgPr::BgPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BgPr& BgPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BgPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			shadeToTitle = element.attribute("shadeToTitle").value();
			Fill.GetFillFrom(element);
			EffectList.GetEffectListFrom(element);

			FillParentPointersForChilds();
		}


		const XML::XNode BgPr::toXML() const
		{
			return XML::XElement(ns.p + "bgPr",
					XML::XAttribute("shadeToTitle", shadeToTitle) +
					XML::Write(Fill) +
					XML::Write(EffectList)
				);
		}

		void BgPr::FillParentPointersForChilds()
		{
			Fill.SetParentPointer(*this);
			EffectList.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX