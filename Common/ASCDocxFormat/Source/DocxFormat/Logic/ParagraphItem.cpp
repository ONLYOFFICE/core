
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ParagraphItem.h"
#include "./../FormatError.h"


namespace OOX
{
	namespace Logic
	{

		ParagraphItem::ParagraphItem()
		{
		}
		

		ParagraphItem::ParagraphItem(const Run& run)
			: IItemable<ParagraphItemBase>(new Run(run))
		{
		}


		ParagraphItem::ParagraphItem(const Hyperlink& hyperlink)
			: IItemable<ParagraphItemBase>(new Hyperlink(hyperlink))
		{
		}


		ParagraphItem::ParagraphItem(const BookmarkStart& start)
			: IItemable<ParagraphItemBase>(new BookmarkStart(start))
		{
		}
		
		
		ParagraphItem::ParagraphItem(const BookmarkEnd& end)
			: IItemable<ParagraphItemBase>(new BookmarkEnd(end))
		{
		}


		ParagraphItem::~ParagraphItem()
		{
		}


		ParagraphItem::ParagraphItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ParagraphItem& ParagraphItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ParagraphItem::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element.XName == "r")
				m_item.reset(new Run(element));
			else if (element.XName == ("hyperlink"))
				m_item.reset(new Hyperlink(element));
			else if (element.XName == ("bookmarkStart"))
				m_item.reset(new BookmarkStart(element));
			else if (element.XName == ("bookmarkEnd"))
				m_item.reset(new BookmarkEnd(element));
			else if (element.XName == ("fldSimple"))
				m_item.reset(new FldSimple(element));
			else if (element.XName == ("ins"))
				m_item.reset(new Insert(element));
			else if (element.XName == ("del"))
				m_item.reset(new Delete(element));
			else
				throw FormatError("bad ParagraphItem type");		
		}
		
		
		const XML::XNode ParagraphItem::toXML() const
		{
			return XML::Write(m_item);
		}


		const std::string ParagraphItem::toTxt() const
		{
			return m_item->toTxt();
		}

	} // namespace Logic
} // namespace OOX