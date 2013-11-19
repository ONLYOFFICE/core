#include "SupplementalFont.h"

namespace PPTX
{
	namespace Logic
	{

		SupplementalFont::SupplementalFont()
		{
		}


		SupplementalFont::~SupplementalFont()
		{
		}


		SupplementalFont::SupplementalFont(const XML::XNode& node)
		{
			fromXML(node);
		}


		const SupplementalFont& SupplementalFont::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void SupplementalFont::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			script = element.attribute("script").value();
			typeface = element.attribute("typeface").value();
		}


		const XML::XNode SupplementalFont::toXML() const
		{
			return	XML::XElement(m_namespace + m_name.get(),
				XML::XAttribute("script", script) +
				XML::XAttribute("typeface", typeface)
			);
		}

	} // namespace Logic
} // namespace PPTX