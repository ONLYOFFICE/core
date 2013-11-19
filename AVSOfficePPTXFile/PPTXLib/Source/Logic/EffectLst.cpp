#include "EffectLst.h"

namespace PPTX
{
	namespace Logic
	{
		EffectLst::EffectLst()
		{
		}

		EffectLst::~EffectLst()
		{
		}

		EffectLst::EffectLst(const XML::XNode& node)
		{
			fromXML(node);
		}

		const EffectLst& EffectLst::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void EffectLst::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			blur = element.element("blur");
			fillOverlay = element.element("fillOverlay");
			glow = element.element("glow");
			innerShdw = element.element("innerShdw");
			outerShdw = element.element("outerShdw");
			prstShdw = element.element("prstShdw");
			reflection = element.element("reflection");
			softEdge = element.element("softEdge");

			FillParentPointersForChilds();
		}

		const XML::XNode EffectLst::toXML() const
		{
			return XML::XElement(ns.a + "effectLst",
					XML::Write(blur) +
					XML::Write(fillOverlay) +
					XML::Write(glow) +
					XML::Write(innerShdw) +
					XML::Write(outerShdw) +
					XML::Write(prstShdw) +
					XML::Write(reflection) +
					XML::Write(softEdge)
				);
		}

		void EffectLst::FillParentPointersForChilds()
		{
			if(blur.is_init())
				blur->SetParentPointer(*this);
			if(fillOverlay.is_init())
				fillOverlay->SetParentPointer(*this);
			if(glow.is_init())
				glow->SetParentPointer(*this);
			if(innerShdw.is_init())
				innerShdw->SetParentPointer(*this);
			if(outerShdw.is_init())
				outerShdw->SetParentPointer(*this);
			if(prstShdw.is_init())
				prstShdw->SetParentPointer(*this);
			if(reflection.is_init())
				reflection->SetParentPointer(*this);
			if(softEdge.is_init())
				softEdge->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX