
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "List.h"


namespace Odt
{
	namespace Content
	{

		List::List()
		{
			m_hasNote = false;
		}


		List::~List()
		{
		}


		List::List(const XML::XNode& node)
		{
			fromXML(node);
		}


		const List& List::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void List::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Style = element.attribute("style-name").value();
			ContinueNumbering = element.attribute("continue-numbering").value();
			id = element.attribute("id").value();
			ContinueList = element.attribute("continue-list").value();

			m_hasNote = false;
			for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
			{
				ListItem item(*i);
				m_hasNote |= item.hasNote();
				Items->push_back(item);
			}
		}


		const XML::XNode List::toXML() const
		{
			return
				XML::XElement(ns.text + "list",
					XML::XAttribute(ns.text + "style-name", Style) +
					XML::XAttribute(ns.text + "continue-numbering", ContinueNumbering) +
					XML::XAttribute(ns.text + "continue-list", ContinueList) +
					XML::XAttribute("xml:id", id) +
					XML::Write(Items)
				);
		}


		const bool List::hasNote() const
		{
			return m_hasNote;
		}


		const bool List::hasList() const
		{
			return true;
		}

	} // namespace Content
} // namespace Odt