#include "Bevel.h"

namespace PPTX
{
	namespace Logic
	{

		Bevel::Bevel()
		{
		}


		Bevel::~Bevel()
		{
		}


		Bevel::Bevel(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Bevel& Bevel::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Bevel::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			
			w = element.attribute("w").value();
			h = element.attribute("h").value();
			prst = element.attribute("prst").value();
		}


		const XML::XNode Bevel::toXML() const
		{
			return XML::XElement(ns.a + m_name.get(),
					XML::XAttribute("w", w) +
					XML::XAttribute("h", h) +
					XML::XAttribute("prst", prst)
				);
		}

	} // namespace Logic
} // namespace PPTX