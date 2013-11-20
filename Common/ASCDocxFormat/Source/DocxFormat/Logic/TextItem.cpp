
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TextItem.h"
#include "Paragraph.h"
#include "Table.h"
#include "Sdt.h"
#include "BookmarkStartParagraph.h"
#include "BookmarkEndParagraph.h"


namespace OOX
{
	namespace Logic
	{

		TextItem::TextItem()
		{
		}


		TextItem::~TextItem()
		{
		}


		TextItem::TextItem(const Paragraph& paragraph)
			: m_item(new Paragraph(paragraph))
		{
		}


		TextItem::TextItem(const Table& table)
			: m_item(new Table(table))
		{
		}


		TextItem::TextItem(const Sdt& sdt)
			: m_item(new Sdt(sdt))
		{
		}


		TextItem::TextItem(const BookmarkStartParagraph& bookmark)
			: m_item(new BookmarkStartParagraph(bookmark))
		{
		}


		TextItem::TextItem(const BookmarkEndParagraph& bookmark)
			: m_item(new BookmarkEndParagraph(bookmark))
		{
		}


		TextItem::TextItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TextItem& TextItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TextItem::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element->XName == "p")
			{
				m_item.reset(new Paragraph(node));
			}
			else if (element->XName == "tbl")
			{
				m_item.reset(new Table(node));
			}
			else if (element->XName == "sdt")
			{
				m_item.reset(new Sdt(node));
			}
			else if (element->XName == "bookmarkStart")
			{
				m_item.reset(new BookmarkStartParagraph(node));
			}
			else if (element->XName == "bookmarkEnd")
			{
				m_item.reset(new BookmarkEndParagraph(node));
			}

		}


		const XML::XNode TextItem::toXML() const
		{
			return m_item->toXML();
		}		


		//void TextItem::inList()
		//{
		//	if (is<Paragraph>())
		//		as<Paragraph>().inList();
		//}

	} // namespace Logic
} // namespace OOX