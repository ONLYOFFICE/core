#include "SpPr.h"
#include "PrstGeom.h"
#include "CustGeom.h"

namespace PPTX
{
	namespace Logic
	{

		SpPr::SpPr()
		{
		}


		SpPr::~SpPr()
		{
		}
	

		SpPr::SpPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SpPr& SpPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SpPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_namespace = element.XName.get().Ns;

			bwMode = element.attribute("bwMode").value();

			xfrm = element.element("xfrm");
			Geometry.GetGeometryFrom(element);
			Fill.GetFillFrom(element);
			ln = element.element("ln");
			EffectList.GetEffectListFrom(element);
			scene3d = element.element("scene3d");
			sp3d = element.element("sp3d");

			FillParentPointersForChilds();
		}


		const XML::XNode SpPr::toXML() const
		{
			return XML::XElement(m_namespace + "spPr",
					XML::XAttribute("bwMode", bwMode) +
					XML::Write(xfrm) +
					XML::Write(Geometry) +
					XML::Write(Fill) +
					XML::Write(ln) +
					XML::Write(EffectList) +
					XML::Write(scene3d) +
					XML::Write(sp3d)
				);
		}

		void SpPr::Merge(SpPr& spPr)const
		{
			if(xfrm.is_init())
				xfrm->Merge(spPr.xfrm);
			if(Geometry.is_init())
				spPr.Geometry.fromXML(Geometry.toXML());
			if(Fill.is_init())
				spPr.Fill = Fill;//.fromXML(Fill.toXML());
			if(ln.is_init())
				ln->Merge(spPr.ln);
		}

		void SpPr::FillParentPointersForChilds()
		{
			if(xfrm.is_init())
				xfrm->SetParentPointer(*this);
			Geometry.SetParentPointer(*this);
			Fill.SetParentPointer(*this);
			if(ln.is_init())
				ln->SetParentPointer(*this);
			EffectList.SetParentPointer(*this);
			if(scene3d.is_init())
				scene3d->SetParentPointer(*this);
			if(sp3d.is_init())
				sp3d->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX