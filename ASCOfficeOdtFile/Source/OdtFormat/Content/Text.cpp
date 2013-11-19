
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Text.h"
#include "Paragraph.h"
#include "Table.h"
#include "List.h"
#include "TableOfContent.h"
#include "IndexTitle.h"
#include "Section.h"


namespace Odt
{
	namespace Content
	{

		Text::Text()
		{
		}


		Text::~Text()
		{
		}


		Text::Text(const Paragraph& paragraph)
			: m_item(new Paragraph(paragraph))
		{
		}


		Text::Text(const Table& table)
			: m_item(new Table(table))
		{
		}


		Text::Text(const List& list)
			: m_item(new List(list))
		{
		}

		Text::Text(const TableOfContent& tableOfContent)
			: m_item(new TableOfContent(tableOfContent))
		{
		}


		Text::Text(const IndexTitle& indexTitle)
			: m_item(new IndexTitle(indexTitle))
		{
		}


		Text::Text(const Section& section)
			: m_item(new Section(section))
		{
		}


		Text::Text(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Text& Text::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Text::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			if (element->XName == "p" || element->XName == "h")
				m_item.reset(new Paragraph(node));
			else if (element->XName == "table")
				m_item.reset(new Table(node));
			else if (element->XName == "list")
				m_item.reset(new List(node));
			else if (element->XName == "table-of-content")
				m_item.reset(new TableOfContent(node));
			else if (element->XName == "index-title")
				m_item.reset(new IndexTitle(node));
			else if (element->XName == "section")
				m_item.reset(new Section(node));

		}


		const XML::XNode Text::toXML() const
		{
			return m_item->toXML();
		}


		const bool Text::hasNote() const
		{
			return m_item->hasNote();
		}


		const bool Text::hasList() const
		{
			return m_item->hasList();
		}

	} // namespace Content
} // namespace