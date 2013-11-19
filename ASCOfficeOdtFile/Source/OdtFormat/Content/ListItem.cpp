
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ListItem.h"


namespace Odt
{
	namespace Content
	{

		ListItem::ListItem()
		{
			m_hasNote = false;
		}


		ListItem::~ListItem()
		{
		}


		ListItem::ListItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ListItem& ListItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ListItem::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_hasNote = false;
			for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
			{
				const XML::XElement element(*i);
				if (element.XName == "p" || element.XName == "h" || element.XName == "list" || element.XName == "table")
				{
					Text text(*i);
					m_hasNote |= text.hasNote();
					Texts->push_back(text);
				}
			}
		}


		const XML::XNode ListItem::toXML() const
		{
			return
				XML::XElement(ns.text + "list-item",
					XML::Write(Texts)
				);
		}


		const bool ListItem::hasNote() const
		{
			return m_hasNote;
		}

	} // namespace Content
} // namespace Odt