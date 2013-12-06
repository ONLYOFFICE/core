
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "RunItem.h"
#include "Text.h"
#include "Symbol.h"
#include "Drawing.h"
#include "Pict.h"
#include "ContinuationSeparator.h"
#include "FootnoteRef.h"
#include "EndnoteRef.h"
#include "Separator.h"
#include "Tab.h"
#include "Break.h"
#include "FootnoteReference.h"
#include "EndnoteReference.h"

#include "Pict.h"
#include "FldChar.h"
#include "InstrText.h"
#include "DelText.h"
#include "NullRun.h"

namespace OOX
{
	namespace Logic
	{
		RunItem::RunItem()
		{

		}

		RunItem::~RunItem()
		{

		}

		RunItem::RunItem(RunItemBase* item)
		{
			fromItem(item);
		}

		RunItem::RunItem(const XML::XNode& node)
		{
			fromXML(node);
		}

		RunItem::RunItem(const std::string& text)
		{
			fromTxt(text);
		}

		const RunItem& RunItem::operator =(RunItemBase* item)
		{
			fromItem(item);
			return *this;
		}

		const RunItem& RunItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		const RunItem& RunItem::operator =(const std::string& text)
		{
			fromTxt(text);
			return *this;
		}

		void RunItem::fromItem(RunItemBase* item)
		{
			m_item.reset(item);
		}

		void RunItem::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element.XName == "t")
				fromItem(new Text(node));
			else if (element.XName == "tab")
				fromItem(new Tab(node));
			else if (element.XName == "br")
				fromItem(new Break(node));
			else if (element.XName == "drawing")
				fromItem(new Drawing(node));
			else if (element.XName == "pict" || element.XName == "object")
				fromItem(new Pict(node));
			else if (element.XName == "footnoteRef")
				fromItem(new FootnoteRef(node));
			else if (element.XName == "endnoteRef")
				fromItem(new EndnoteRef(node));
			else if (element.XName == "continuationSeparator")
				fromItem(new ContinuationSeparator(node));
			else if (element.XName == "separator")
				fromItem(new Separator(node));
			else if (element.XName == "footnoteReference")
				fromItem(new FootnoteReference(node));
			else if (element.XName == "endnoteReference")
				fromItem(new EndnoteReference(node));
			/*else if (element.XName == "object")
			fromItem(new Object(node));*/
			else if (element.XName == "fldChar")
				fromItem(new FldChar(node));
			else if (element.XName == "instrText")
				fromItem(new InstrText(node));
			else if (element.XName == "sym")
				fromItem(new Symbol(node));
			else if (element.XName == "delText")
				fromItem(new DelText(node));
			else
				fromItem(new NullRun(node));
		}

		const XML::XNode RunItem::toXML() const
		{
			return XML::XNode(); //	return XML::Write(m_item);
		}

		void RunItem::fromTxt(const std::string& text)
		{
			m_item.reset(new Text(text));
		}

		const std::string RunItem::toTxt() const
		{
			return m_item->toTxt();
		}

	} // namespace Logic
} // namespace OOX