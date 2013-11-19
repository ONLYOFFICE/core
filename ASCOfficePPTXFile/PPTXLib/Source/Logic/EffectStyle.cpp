#include "EffectStyle.h"

namespace PPTX
{
	namespace Logic
	{

		EffectStyle::EffectStyle()
		{
		}


		EffectStyle::~EffectStyle()
		{
		}


		EffectStyle::EffectStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const EffectStyle& EffectStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void EffectStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			EffectList.GetEffectListFrom(element);
			scene3d = element.element("scene3d");
			sp3d = element.element("sp3d");

			FillParentPointersForChilds();
		}


		const XML::XNode EffectStyle::toXML() const
		{
			return XML::XElement(ns.a + "effectStyle",
					XML::Write(EffectList) +
					XML::Write(scene3d) +
					XML::Write(sp3d)
				);
		}

		void EffectStyle::FillParentPointersForChilds()
		{
			EffectList.SetParentPointer(*this);
			if(scene3d.is_init())
				scene3d->SetParentPointer(*this);
			if(sp3d.is_init())
				sp3d->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX