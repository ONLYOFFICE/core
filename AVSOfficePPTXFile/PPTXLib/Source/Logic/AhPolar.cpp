#include "AhPolar.h"

namespace PPTX
{
	namespace Logic
	{
		AhPolar::AhPolar()
		{
		}

		AhPolar::~AhPolar()
		{
		}

		AhPolar::AhPolar(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AhPolar& AhPolar::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AhPolar::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			x = element.element("pos").attribute("x").value();
			y = element.element("pos").attribute("y").value();

			gdRefAng = element.attribute("gdRefAng").value();
			gdRefR = element.attribute("gdRefR").value();
			maxAng = element.attribute("maxAng").value();
			maxR = element.attribute("maxR").value();
			minAng = element.attribute("minAng").value();
			minR = element.attribute("minR").value();
		}

		const XML::XNode AhPolar::toXML() const
		{
			return XML::XElement(ns.a + "ahPolar",
					XML::XElement(ns.a + "pos",
						XML::XAttribute("x", x) +
						XML::XAttribute("y", y)
					) +
					XML::XAttribute("gdRefR", gdRefR) +
					XML::XAttribute("minR", minR) +
					XML::XAttribute("maxR", maxR) +
					XML::XAttribute("gdRefAng", gdRefAng) +
					XML::XAttribute("minAng", minAng) +
					XML::XAttribute("maxAng", maxAng)
				);
		}

		//std::string AhPolar::GetODString()const
		void AhPolar::GetODString(std::list<std::string>& strList)const
		{
			XML::XElement element("ahPolar", XML::XElement("pos", XML::XAttribute("x", x) + XML::XAttribute("y", y)) +
					XML::XAttribute("gdRefR", gdRefR) + XML::XAttribute("minR", minR) +	XML::XAttribute("maxR", maxR) +	XML::XAttribute("gdRefAng", gdRefAng) +
					XML::XAttribute("minAng", minAng) + XML::XAttribute("maxAng", maxAng));
			//return element.ToString();
			element.SaveToStringList(strList);
		}
	} // namespace Logic
} // namespace PPTX