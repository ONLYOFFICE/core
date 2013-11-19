#include "Rect.h"

namespace PPTX
{
	namespace Logic
	{
		Rect::Rect()
		{
		}

		Rect::~Rect()
		{
		}

		Rect::Rect(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Rect& Rect::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Rect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			t = element.attribute("t").value();
			b = element.attribute("b").value();
			l = element.attribute("l").value();
			r = element.attribute("r").value();
		}

		const XML::XNode Rect::toXML() const
		{
			return XML::XElement(m_namespace + m_name.get(),
					XML::XAttribute("l", l) +
					XML::XAttribute("t", t) +
					XML::XAttribute("r", r) +
					XML::XAttribute("b", b)
				);
		}

		//std::string Rect::GetODString()const
		void Rect::GetODString(std::list<std::string>& strList)const
		{
			XML::XElement element("rect", XML::XAttribute("l", l) + XML::XAttribute("t", t) + XML::XAttribute("r", r) + XML::XAttribute("b", b));
			//return element.ToString();
			element.SaveToStringList(strList);
		}
	} // namespace Logic
} // namespace PPTX