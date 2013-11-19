#include "Cxn.h"

namespace PPTX
{
	namespace Logic
	{
		Cxn::Cxn()
		{
		}

		Cxn::~Cxn()
		{
		}

		Cxn::Cxn(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Cxn& Cxn::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Cxn::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			x = element.element("pos").attribute("x").value();
			y = element.element("pos").attribute("y").value();
			ang = element.attribute("ang").value();
		}

		const XML::XNode Cxn::toXML() const
		{
			return XML::XElement(ns.a + "cxn",
					XML::XAttribute("ang", ang) +
					XML::XElement(ns.a + "pos",
						XML::XAttribute("x", x) +
						XML::XAttribute("y", y)
					)
				);
		}

		//std::string Cxn::GetODString()const
		void Cxn::GetODString(std::list<std::string>& strList)const
		{
			XML::XElement element("cxn", XML::XAttribute("ang", ang) + XML::XElement("pos", XML::XAttribute("x", x) + XML::XAttribute("y", y)));
			//return element.ToString();
			element.SaveToStringList(strList);
		}
	} // namespace Logic
} // namespace PPTX