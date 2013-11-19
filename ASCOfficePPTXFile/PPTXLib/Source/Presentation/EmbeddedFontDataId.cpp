#include "EmbeddedFontDataId.h"

namespace PPTX
{
	namespace nsPresentation
	{
		EmbeddedFontDataId::EmbeddedFontDataId()
		{
		}

		EmbeddedFontDataId::~EmbeddedFontDataId()
		{
		}

		EmbeddedFontDataId::EmbeddedFontDataId(const XML::XNode& node)
		{
			fromXML(node);
		}

		const EmbeddedFontDataId& EmbeddedFontDataId::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void EmbeddedFontDataId::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			rid = element.attribute(ns.r + "id").value();
		}

		const XML::XNode EmbeddedFontDataId::toXML() const
		{
			return	XML::XElement(ns.p + m_name.get(),
				XML::XAttribute(ns.r + "id", rid)
			);
		}
	} // namespace nsPresentation
} // namespace PPTX