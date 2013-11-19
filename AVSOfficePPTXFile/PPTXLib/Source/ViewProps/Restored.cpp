#include "Restored.h"

namespace PPTX
{
	namespace nsViewProps
	{
		Restored::Restored()
		{
		}

		Restored::~Restored()
		{
		}

		Restored::Restored(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Restored& Restored::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Restored::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			name = element.XName.get().Name;
			sz = element.attribute("sz").value();
			
			autoAdjust = element.attribute("autoAdjust").value();
		}

		const XML::XNode Restored::toXML() const
		{
			return	XML::XElement(ns.p + name.get(),
				XML::XAttribute("sz", sz) +
				XML::XAttribute("autoAdjust", autoAdjust)
			);
		}
	} // namespace nsViewProps
} // namespace PPTX