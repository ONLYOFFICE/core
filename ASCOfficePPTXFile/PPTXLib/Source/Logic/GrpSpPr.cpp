#include "GrpSpPr.h"

namespace PPTX
{
	namespace Logic
	{

		GrpSpPr::GrpSpPr()
		{
		}


		GrpSpPr::~GrpSpPr()
		{
		}
	

		GrpSpPr::GrpSpPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const GrpSpPr& GrpSpPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void GrpSpPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			bwMode = element.attribute("bwMode").value();

			xfrm = element.element("xfrm");
			Fill.GetFillFrom(element);
			EffectList.GetEffectListFrom(element);
			scene3d = element.element("scene3d");

			FillParentPointersForChilds();
		}


		const XML::XNode GrpSpPr::toXML() const
		{
			return XML::XElement(ns.p + "grpSpPr",
					XML::XAttribute("bwMode", bwMode) +
					XML::Write(xfrm) +
					XML::Write(Fill) +
					XML::Write(EffectList) +
					XML::Write(scene3d)
				);
		}

		void GrpSpPr::FillParentPointersForChilds()
		{
			if(xfrm.is_init())
				xfrm->SetParentPointer(*this);
			Fill.SetParentPointer(*this);
			EffectList.SetParentPointer(*this);
			if(scene3d.is_init())
				scene3d->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX