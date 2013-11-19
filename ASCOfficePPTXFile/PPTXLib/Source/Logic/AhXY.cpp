#include "AhXY.h"

namespace PPTX
{
	namespace Logic
	{
		AhXY::AhXY()
		{
		}

		AhXY::~AhXY()
		{
		}

		AhXY::AhXY(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AhXY& AhXY::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AhXY::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			x = element.element("pos").attribute("x").value();
			y = element.element("pos").attribute("y").value();

			gdRefX = element.attribute("gdRefX").value();
			gdRefY = element.attribute("gdRefY").value();
			maxX = element.attribute("maxX").value();
			maxY = element.attribute("maxY").value();
			minX = element.attribute("minX").value();
			minY = element.attribute("minY").value();
		}

		const XML::XNode AhXY::toXML() const
		{
			return XML::XElement(ns.a + "ahXY",
					XML::XElement(ns.a + "pos",
						XML::XAttribute("x", x) +
						XML::XAttribute("y", y)
					) +
					XML::XAttribute("gdRefX", gdRefX) +
					XML::XAttribute("minX", minX) +
					XML::XAttribute("maxX", maxX) +
					XML::XAttribute("gdRefY", gdRefY) +
					XML::XAttribute("minY", minY) +
					XML::XAttribute("maxY", maxY)
				);
		}

		//std::string AhXY::GetODString()const
		void AhXY::GetODString(std::list<std::string>& strList)const
		{
			XML::XElement element("ahXY",XML::XElement("pos",XML::XAttribute("x", x) + XML::XAttribute("y", y)) +
					XML::XAttribute("gdRefX", gdRefX) +	XML::XAttribute("minX", minX) +	XML::XAttribute("maxX", maxX) +	XML::XAttribute("gdRefY", gdRefY) +
					XML::XAttribute("minY", minY) + XML::XAttribute("maxY", maxY));
			//return element.ToString();
			element.SaveToStringList(strList);
		}
	} // namespace Logic
} // namespace PPTX