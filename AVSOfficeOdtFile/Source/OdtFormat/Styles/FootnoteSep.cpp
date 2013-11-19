
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "FootnoteSep.h"


namespace Odt
{
	namespace Styles
	{

		FootnoteSep::FootnoteSep()
		{
		}


		FootnoteSep::~FootnoteSep()
		{
		}


		FootnoteSep::FootnoteSep(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FootnoteSep& FootnoteSep::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FootnoteSep::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_width	= element.attribute("width").value();
			m_distanceBeforeSep = element.attribute("distance-before-sep").value();
			m_distanceAfterSep	= element.attribute("distance-after-sep").value();
			m_adjustment	=	element.attribute("adjustment").value();
			m_relWidth		=	element.attribute("rel-width").value();
			m_color				=	element.attribute("color").value();

		}


		const XML::XNode FootnoteSep::toXML() const
		{
			return
				XML::XElement(ns.style + "footnote-sep",
					XML::XAttribute(ns.style + "width", m_width) +
					XML::XAttribute(ns.style + "distance-before-sep", m_distanceBeforeSep) +
					XML::XAttribute(ns.style + "distance-after-sep", m_distanceAfterSep) +
					XML::XAttribute(ns.style + "adjustment", m_adjustment) +
					XML::XAttribute(ns.style + "rel-width", m_relWidth) +
					XML::XAttribute(ns.style + "color", m_color)
				);
		}

	} // namespace Styles
} // namespace Odt