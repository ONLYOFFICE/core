#include "ArcTo.h"

namespace PPTX
{
	namespace Logic
	{
		ArcTo::ArcTo()
		{
		}

		ArcTo::~ArcTo()
		{
		}

		ArcTo::ArcTo(const XML::XNode& node)
		{
			fromXML(node);
		}

		const ArcTo& ArcTo::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void ArcTo::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			wR = element.attribute("wR").value();
			hR = element.attribute("hR").value();
			stAng = element.attribute("stAng").value();
			swAng = element.attribute("swAng").value();
		}

		const XML::XNode ArcTo::toXML() const
		{
			 return XML::XElement(ns.a + "arcTo",
				 XML::XAttribute("wR", wR) +
				 XML::XAttribute("hR", hR) +
				 XML::XAttribute("stAng", stAng) +
				 XML::XAttribute("swAng", swAng)
				 );
		}

		//std::string ArcTo::GetODString()const
		void ArcTo::GetODString(std::list<std::string>& strList)const
		{
			XML::XElement element("arcTo", XML::XAttribute("wR", wR) + XML::XAttribute("hR", hR) + XML::XAttribute("stAng", stAng) + XML::XAttribute("swAng", swAng));
			//return element.ToString();
			element.SaveToStringList(strList);
		}
	} // namespace Logic
} // namespace PPTX