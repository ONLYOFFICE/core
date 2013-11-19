#include "Sld.h"

namespace PPTX
{
	namespace nsViewProps
	{
		Sld::Sld()
		{
		}

		Sld::~Sld()
		{
		}

		Sld::Sld(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Sld& Sld::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Sld::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			id = element.attribute("id").value();
			collapse = element.attribute("collapse").value();
		}

		const XML::XNode Sld::toXML() const
		{
			return	XML::XElement(ns.p + "sld",
				XML::XAttribute(ns.r + "id", id) +
				XML::XAttribute("collapse", collapse)
			);
		}
	} // namespace nsViewProps
} // namespace PPTX