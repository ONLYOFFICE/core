#include "GridSpacing.h"

namespace PPTX
{
	namespace nsViewProps
	{
		GridSpacing::GridSpacing()
		{
		}

		GridSpacing::~GridSpacing()
		{
		}

		GridSpacing::GridSpacing(const XML::XNode& node)
		{
			fromXML(node);
		}

		const GridSpacing& GridSpacing::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void GridSpacing::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cx = element.attribute("cx").value();
			cy = element.attribute("cy").value();
		}

		const XML::XNode GridSpacing::toXML() const
		{
			return	XML::XElement(ns.p + "gridSpacing",
				XML::XAttribute("cx", cx) +
				XML::XAttribute("cy", cy)
			);
		}
	} // namespace nsViewProps
} // namespace PPTX