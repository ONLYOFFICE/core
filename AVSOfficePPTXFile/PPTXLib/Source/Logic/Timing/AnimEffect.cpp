#include "AnimEffect.h"

namespace PPTX
{
	namespace Logic
	{

		AnimEffect::AnimEffect()
		{
		}

		AnimEffect::~AnimEffect()
		{
		}

		AnimEffect::AnimEffect(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AnimEffect& AnimEffect::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AnimEffect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			filter = element.attribute("filter").value();
			prLst = element.attribute("prLst").value();
			transition = element.attribute("transition").value();

			cBhvr = element.element("cBhvr");
			progress = element.element("progress");

			FillParentPointersForChilds();
		}

		const XML::XNode AnimEffect::toXML() const
		{
			return XML::XElement(ns.p + "animEffect",
				XML::XAttribute("filter", filter) +
				XML::XAttribute("prLst", prLst) +
				XML::XAttribute("transition", transition) +
				XML::Write(cBhvr) +
				XML::Write(progress)
				);
		}

		void AnimEffect::FillParentPointersForChilds()
		{
			cBhvr->SetParentPointer(*this);
			if(progress.is_init())
				progress->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX