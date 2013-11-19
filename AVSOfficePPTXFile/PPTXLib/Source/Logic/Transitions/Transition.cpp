#include "Transition.h"

namespace PPTX
{
	namespace Logic
	{

		Transition::Transition()
		{
		}

		Transition::~Transition()
		{
		}

		Transition::Transition(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Transition& Transition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Transition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			advClick = element.attribute("advClick").value();
			advTm = element.attribute("advTm").value();
			spd = element.attribute("spd").value();

			base.GetTransitionTypeFrom(element);
			sndAc = element.element("sndAc");
			FillParentPointersForChilds();
		}

		const XML::XNode Transition::toXML() const
		{
			return XML::XElement(ns.p + "transition",
				XML::XAttribute("advClick", advClick) +
				XML::XAttribute("advTm", advTm) +
				XML::XAttribute("spd", spd) +
				XML::Write(base) +
				XML::Write(sndAc)
				);
		}

		void Transition::FillParentPointersForChilds()
		{
			if(base.is_init())
				base.SetParentPointer(*this);
			if(sndAc.is_init())
				sndAc->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX