#include "MoveTo.h"

namespace PPTX
{
	namespace Logic
	{

		MoveTo::MoveTo()
		{
		}

		MoveTo::~MoveTo()
		{
		}

		MoveTo::MoveTo(const XML::XNode& node)
		{
			fromXML(node);
		}

		const MoveTo& MoveTo::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void MoveTo::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			x = element.element("pt").attribute("x").value();
			y = element.element("pt").attribute("y").value();
		}

		const XML::XNode MoveTo::toXML() const
		{
			 return XML::XElement(ns.a + "moveTo",
					XML::XElement(ns.a + "pt",
						XML::XAttribute("x", x) +
						XML::XAttribute("y", y)
					)
				);
		}

		//std::string MoveTo::GetODString()const
		void MoveTo::GetODString(std::list<std::string>& strList)const
		{
			XML::XElement element("moveTo", XML::XElement("pt",	XML::XAttribute("x", x) + XML::XAttribute("y", y)));
			//return element.ToString();
			element.SaveToStringList(strList);
		}
	} // namespace Logic
} // namespace PPTX