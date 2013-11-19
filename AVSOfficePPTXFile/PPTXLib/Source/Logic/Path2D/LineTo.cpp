#include "LineTo.h"

namespace PPTX
{
	namespace Logic
	{

		LineTo::LineTo()
		{
		}

		LineTo::~LineTo()
		{
		}

		LineTo::LineTo(const XML::XNode& node)
		{
			fromXML(node);
		}

		const LineTo& LineTo::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void LineTo::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			x = element.element("pt").attribute("x").value();
			y = element.element("pt").attribute("y").value();
		}

		const XML::XNode LineTo::toXML() const
		{
			 return XML::XElement(ns.a + "lnTo",
					XML::XElement(ns.a + "pt",
						XML::XAttribute("x", x) +
						XML::XAttribute("y", y)
					)
				);
		}

		//std::string LineTo::GetODString()const
		void LineTo::GetODString(std::list<std::string>& strList)const
		{
			XML::XElement element("lnTo", XML::XElement("pt", XML::XAttribute("x", x) + XML::XAttribute("y", y)));
			//return element.ToString();
			element.SaveToStringList(strList);
		}
	} // namespace Logic
} // namespace PPTX