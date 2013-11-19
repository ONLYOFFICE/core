#include "Gd.h"

namespace PPTX
{
	namespace Logic
	{

		Gd::Gd()
		{
		}

		Gd::~Gd()
		{
		}

		Gd::Gd(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Gd& Gd::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Gd::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			name = element.attribute("name").value();
			fmla = element.attribute("fmla").value();
		}

		const XML::XNode Gd::toXML() const
		{
			return XML::XElement(ns.a + "gd",
					XML::XAttribute("name", name) +
					XML::XAttribute("fmla", fmla)
				);
		}

		void Gd::GetODString(std::list<std::string>& strList)const
		{
			XML::XElement element("gd", XML::XAttribute("name", name) + XML::XAttribute("fmla", fmla));
			//return element.ToString();
			element.SaveToStringList(strList);
		}
	} // namespace Logic
} // namespace PPTX