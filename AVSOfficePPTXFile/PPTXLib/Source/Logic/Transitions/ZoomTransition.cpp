#include "ZoomTransition.h"

namespace PPTX
{
	namespace Logic
	{

		ZoomTransition::ZoomTransition()
		{
		}

		ZoomTransition::~ZoomTransition()
		{
		}

		ZoomTransition::ZoomTransition(const XML::XNode& node)
		{
			fromXML(node);
		}

		const ZoomTransition& ZoomTransition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void ZoomTransition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			dir = element.attribute("dir").value();
		}

		const XML::XNode ZoomTransition::toXML() const
		{
			return XML::XElement(ns.p + "zoom",
					XML::XAttribute("dir", dir)
				);
		}

	} // namespace Logic
} // namespace PPTX