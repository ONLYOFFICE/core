#include "WheelTransition.h"

namespace PPTX
{
	namespace Logic
	{

		WheelTransition::WheelTransition()
		{
		}

		WheelTransition::~WheelTransition()
		{
		}

		WheelTransition::WheelTransition(const XML::XNode& node)
		{
			fromXML(node);
		}

		const WheelTransition& WheelTransition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void WheelTransition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			spokes = element.attribute("spokes").value();
		}

		const XML::XNode WheelTransition::toXML() const
		{
			return XML::XElement(ns.p + "wheel",
					XML::XAttribute("spokes", spokes)
				);
		}

	} // namespace Logic
} // namespace PPTX