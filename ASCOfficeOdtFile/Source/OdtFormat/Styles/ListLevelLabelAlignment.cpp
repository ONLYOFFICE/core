
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ListLevelLabelAlignment.h"


namespace Odt
{
	namespace Styles
	{

		ListLevelLabelAlignment::ListLevelLabelAlignment()
		{
		}


		ListLevelLabelAlignment::~ListLevelLabelAlignment()
		{
		}


		ListLevelLabelAlignment::ListLevelLabelAlignment(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ListLevelLabelAlignment& ListLevelLabelAlignment::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ListLevelLabelAlignment::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_labelFollowedBy			= element.attribute("label-followed-by").value();
			m_listTabStopPosition = element.attribute("list-tab-stop-position").value();
			m_textIndent = element.attribute("text-indent").value();
			m_marginLeft = element.attribute("margin-left").value();
		}


		const XML::XNode ListLevelLabelAlignment::toXML() const
		{
			return 
				XML::XElement(ns.style + "list-level-label-alignment",
					XML::XAttribute(ns.text + "label-followed-by", m_labelFollowedBy) +
					XML::XAttribute(ns.text + "list-tab-stop-position", m_listTabStopPosition) +
					XML::XAttribute(ns.fo + "text-indent", m_textIndent) +
					XML::XAttribute(ns.fo + "margin-left", m_marginLeft)
				);
		}

	} // namespace Styles
} // namespace Odt