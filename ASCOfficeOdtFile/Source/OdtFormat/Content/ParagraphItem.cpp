
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ParagraphItem.h"
#include "ParagraphItemBase.h"
#include "SimpleText.h"
#include "Span.h"
#include "Frame.h"
#include "Space.h"
#include "Tab.h"
#include "LineBreak.h"
#include "SoftPageBreak.h"
#include "Ahref.h"
#include "Note.h"
#include "BookmarkStart.h"
#include "BookmarkEnd.h"
#include "PageNumber.h"
#include "CustomShape.h"
#include "VariableInput.h"
#include "NullRun.h"


namespace Odt
{
	namespace Content
	{

		ParagraphItem::ParagraphItem()
		{
		}
		
									
		ParagraphItem::~ParagraphItem()
		{
		}


		ParagraphItem::ParagraphItem(ParagraphItemBase* item)
		{
			fromItem(item);
		}


		ParagraphItem::ParagraphItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		ParagraphItem::ParagraphItem(const std::string& text)
		{
			fromTxt(text);
		}


		const ParagraphItem& ParagraphItem::operator =(ParagraphItemBase* item)
		{
			fromItem(item);
			return *this;
		}

		
		const ParagraphItem& ParagraphItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		const ParagraphItem& ParagraphItem::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}


		void ParagraphItem::fromItem(ParagraphItemBase* item)
		{
			m_item.reset(item);
		}


		void ParagraphItem::fromXML(const XML::XNode& node)
		{
			if (node.isText())
			{
				m_item.reset(new SimpleText(XML::XText(node)));
			}
			else if (node.isElement())
			{
				const XML::XElement element(node);
				if (element->XName == "span")
					fromItem(new Span(node));
				else if (element->XName == "frame")
					fromItem(new Frame(node));
				else if (element->XName == "s")
					fromItem(new Space(node));
				else if (element->XName == "tab")
					fromItem(new Tab(node));
				else if (element->XName == "line-break")
					fromItem(new LineBreak(node));
				else if (element->XName == "soft-page-break")
					fromItem(new SoftPageBreak(node));
				else if (element->XName == "a")
					fromItem(new Ahref(node));
				else if (element->XName == "note")
					fromItem(new Note(node));
				else if (element->XName == "bookmark-start")
					fromItem(new BookmarkStart(node));
				else if (element->XName == "bookmark-end")
					fromItem(new BookmarkEnd(node));
				else if (element->XName == "page-number")
					fromItem(new PageNumber(node));
				else if (element->XName == "custom-shape")
					fromItem(new CustomShape(node));
				else if (element->XName == "variable-input")
					fromItem(new VariableInput(node));
				else
					fromItem(new NullRun());
			}
			else
			{
				fromItem(new NullRun());
			}
		}

		
		const XML::XNode ParagraphItem::toXML() const
		{
			return XML::Write(m_item);
		}


		void ParagraphItem::fromTxt(const std::string& text)
		{
			Span span(text);
			create(span);
		}


		const std::string ParagraphItem::toTxt() const
		{
			return m_item->toTxt();
		}

	} // namespace Content
} // namespace Odt