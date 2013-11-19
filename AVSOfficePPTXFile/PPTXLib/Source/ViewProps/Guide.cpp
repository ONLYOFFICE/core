#include "Guide.h"

namespace PPTX
{
	namespace nsViewProps
	{
		Guide::Guide()
		{
		}

		Guide::~Guide()
		{
		}

		Guide::Guide(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Guide& Guide::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Guide::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			orient = element.attribute("orient").value();
			pos = element.attribute("pos").value();
		}

		const XML::XNode Guide::toXML() const
		{
			return	XML::XElement(ns.p + "guide",
				XML::XAttribute("orient", orient) +
				XML::XAttribute("pos", pos)
			);
		}
	} // namespace nsViewProps
} // namespace PPTX