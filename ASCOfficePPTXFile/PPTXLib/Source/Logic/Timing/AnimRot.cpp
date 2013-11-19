#include "AnimRot.h"

namespace PPTX
{
	namespace Logic
	{

		AnimRot::AnimRot()
		{
		}

		AnimRot::~AnimRot()
		{
		}

		AnimRot::AnimRot(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AnimRot& AnimRot::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AnimRot::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cBhvr = element.element("cBhvr");

			by = element.attribute("by").value();
			from = element.attribute("from").value();
			to = element.attribute("to").value();

			FillParentPointersForChilds();
		}

		const XML::XNode AnimRot::toXML() const
		{
			return XML::XElement(ns.p + "animRot",
				XML::XAttribute("by", by) +
				XML::XAttribute("from", from) +
				XML::XAttribute("to", to) +
				XML::Write(cBhvr)
				);
		}

		void AnimRot::FillParentPointersForChilds()
		{
			cBhvr->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX