#include "TextSpacing.h"

namespace PPTX
{
	namespace Logic
	{

		TextSpacing::TextSpacing()
		{
		}

		TextSpacing::~TextSpacing()
		{
		}

		TextSpacing::TextSpacing(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TextSpacing& TextSpacing::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TextSpacing::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			spcPct = element.element("spcPct").attribute("val").value();
			spcPts = element.element("spcPts").attribute("val").value();
		}

		const XML::XNode TextSpacing::toXML() const
		{
			if(spcPct.is_init())
				return XML::XElement(m_namespace + m_name.get(), XML::XElement(ns.a +"spcPct", XML::XAttribute("val", spcPct)));
			return XML::XElement(m_namespace + m_name.get(), XML::XElement(ns.a + "spcPts", XML::XAttribute("val", spcPts)));
		}

		int TextSpacing::GetVal()const
		{
			if(spcPts.is_init())
				return -spcPts.get();
			else
				return spcPct.get_value_or(100000)/1000;
		}
	} // namespace Logic
} // namespace PPTX