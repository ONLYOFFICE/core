#include "XmlId.h"

namespace PPTX
{
	namespace Logic
	{
		XmlId::XmlId()
		{
		}

		XmlId::~XmlId()
		{
		}

		XmlId::XmlId(const XML::XNode& node)
		{
			fromXML(node);
		}

		const XmlId& XmlId::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void XmlId::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			rid = element.attribute(ns.r + "id").value().ToString();
			id = element.attribute("id").value();
		}

		const XML::XNode XmlId::toXML() const
		{
			return XML::XElement(ns.p + *m_name,
					XML::XAttribute("id", id) +
					XML::XAttribute(ns.r + "id", rid)
				);
		}
	} // namespace Logic
} // namespace PPTX