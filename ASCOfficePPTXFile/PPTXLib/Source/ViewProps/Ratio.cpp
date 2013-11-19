#include "Ratio.h"

namespace PPTX
{
	namespace nsViewProps
	{
		Ratio::Ratio()
		{
		}

		Ratio::~Ratio()
		{
		}

		Ratio::Ratio(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Ratio& Ratio::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Ratio::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			name = element.XName.get().Name;
			n = element.attribute("n").value();
			d = element.attribute("d").value();
		}

		const XML::XNode Ratio::toXML() const
		{
			return	XML::XElement(ns.a + name.get(),
				XML::XAttribute("n", n) +
				XML::XAttribute("d", d)
			);
		}
	} // namespace nsViewProps
} // namespace PPTX