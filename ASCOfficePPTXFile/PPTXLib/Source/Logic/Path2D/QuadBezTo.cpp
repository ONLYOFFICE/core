#include "QuadBezTo.h"

namespace PPTX
{
	namespace Logic
	{
		QuadBezTo::QuadBezTo()
		{
		}

		QuadBezTo::~QuadBezTo()
		{
		}

		QuadBezTo::QuadBezTo(const XML::XNode& node)
		{
			fromXML(node);
		}

		const QuadBezTo& QuadBezTo::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void QuadBezTo::fromXML(const XML::XNode& node)
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
		}

		const XML::XNode QuadBezTo::toXML() const
		{
			 return XML::XElement(ns.a + "quadBezTo",
					XML::XElement(ns.a + "pt",
						XML::XAttribute("x", x1) +
						XML::XAttribute("y", y1)
					) +
					XML::XElement(ns.a + "pt",
						XML::XAttribute("x", x2) +
						XML::XAttribute("y", y2)
					)
				);
		}

		//std::string QuadBezTo::GetODString()const
		void QuadBezTo::GetODString(std::list<std::string>& strList)const
		{
			XML::XElement element("quadBezTo", XML::XElement("pt", XML::XAttribute("x", x1) + XML::XAttribute("y", y1)) +
					XML::XElement("pt", XML::XAttribute("x", x2) + XML::XAttribute("y", y2)));
			//return element.ToString();
			element.SaveToStringList(strList);
		}
	} // namespace Logic
} // namespace PPTX