#include "CubicBezTo.h"

namespace PPTX
{
	namespace Logic
	{

		CubicBezTo::CubicBezTo()
		{
		}

		CubicBezTo::~CubicBezTo()
		{
		}

		CubicBezTo::CubicBezTo(const XML::XNode& node)
		{
			fromXML(node);
		}

		const CubicBezTo& CubicBezTo::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void CubicBezTo::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			XML::const_element_iterator i = element.Elements.begin();
			XML::XElement child_element(*i);
			x1 = child_element.attribute("x").value();
			y1 = child_element.attribute("y").value();
			i++;
			child_element = *i;
			x2 = child_element.attribute("x").value();
			y2 = child_element.attribute("y").value();
			i++;
			child_element = *i;
			x3 = child_element.attribute("x").value();
			y3 = child_element.attribute("y").value();
		}

		const XML::XNode CubicBezTo::toXML() const
		{
			 return XML::XElement(ns.a + "cubicBezTo",
					XML::XElement(ns.a + "pt",
						XML::XAttribute("x", x1) +
						XML::XAttribute("y", y1)
					) +
					XML::XElement(ns.a + "pt",
						XML::XAttribute("x", x2) +
						XML::XAttribute("y", y2)
					) +
					XML::XElement(ns.a + "pt",
						XML::XAttribute("x", x3) +
						XML::XAttribute("y", y3)
					)
				);
		}

		//std::string CubicBezTo::GetODString()const
		void CubicBezTo::GetODString(std::list<std::string>& strList)const
		{
			XML::XElement element("cubicBezTo",	XML::XElement("pt",	XML::XAttribute("x", x1) + XML::XAttribute("y", y1)) +
					XML::XElement("pt",	XML::XAttribute("x", x2) + XML::XAttribute("y", y2)) +
					XML::XElement("pt", XML::XAttribute("x", x3) + XML::XAttribute("y", y3)));
			//return element.ToString();
			element.SaveToStringList(strList);
		}
	} // namespace Logic
} // namespace PPTX