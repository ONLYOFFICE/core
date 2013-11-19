
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ListLevelProperties.h"


namespace Odt
{
	namespace Styles
	{

		ListLevelProperties::ListLevelProperties()
		{
		}


		ListLevelProperties::~ListLevelProperties()
		{
		}


		ListLevelProperties::ListLevelProperties(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ListLevelProperties& ListLevelProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ListLevelProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_listLevelPositionSpaceMode = element.attribute("list-level-position-and-space-mode").value();
			m_aligments = element.element("list-level-label-alignment");
			m_minLabelDistance = element.attribute("min-label-distance").value();
		}


		const XML::XNode ListLevelProperties::toXML() const
		{
			return 
				XML::XElement(ns.style + "list-level-properties",
					XML::XAttribute(ns.text + "list-level-position-and-space-mode", m_listLevelPositionSpaceMode) +
					XML::XAttribute(ns.text + "min-label-distance", m_minLabelDistance) +
					XML::Write(m_aligments)
				);
		}

	} // namespace Styles
} // namespace Odt