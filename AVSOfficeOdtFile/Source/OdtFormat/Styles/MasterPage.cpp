
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "MasterPage.h"
#include "./../Content/Paragraph.h"
#include "./../Content/Table.h"
#include "./../Content/List.h"
#include "./../Content/IndexTitle.h"
#include "./../Content/TableOfContent.h"

namespace Odt
{
	namespace Styles
	{

		MasterPage::MasterPage()
		{
		}


		MasterPage::~MasterPage()
		{
		}


		MasterPage::MasterPage(const XML::XNode& node)
		{
			fromXML(node);
		}


		const MasterPage& MasterPage::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void MasterPage::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name				= element.attribute("name").value();
			Layout			= element.attribute("page-layout-name").value();
			NextStyleName = element.attribute("next-style-name").value();

			if (element.element("header").exist())
				for (XML::const_element_iterator i = element.element("header").Elements.begin(); i != element.element("header").Elements.end(); ++i)
				{
					const XML::XElement element(*i);
					if (element.XName == "p" || element.XName == "h" || element.XName == "list" || element.XName == "table" || element.XName == "table-of-content" || element.XName == "index-title")
					{
						Content::Text text(*i);
						Header->push_back(text);
					}
				}
			if (element.element("footer").exist())
				for (XML::const_element_iterator i = element.element("footer").Elements.begin(); i != element.element("footer").Elements.end(); ++i)
				{
					const XML::XElement element(*i);
					if (element.XName == "p" || element.XName == "h" || element.XName == "list" || element.XName == "table" || element.XName == "table-of-content" || element.XName == "index-title")
					{
						Content::Text text(*i);
						Footer->push_back(text);
					}
				}
			if (element.element("footer-left").exist())
				for (XML::const_element_iterator i = element.element("footer-left").Elements.begin(); i != element.element("footer-left").Elements.end(); ++i)
				{
					const XML::XElement element(*i);
					if (element.XName == "p" || element.XName == "h" || element.XName == "list" || element.XName == "table" || element.XName == "table-of-content" || element.XName == "index-title")
					{
						Content::Text text(*i);
						FooterLeft->push_back(text);
					}
				}
			if (element.element("header-left").exist())
				for (XML::const_element_iterator i = element.element("header-left").Elements.begin(); i != element.element("header-left").Elements.end(); ++i)
				{
					const XML::XElement element(*i);
					if (element.XName == "p" || element.XName == "h" || element.XName == "list" || element.XName == "table" || element.XName == "table-of-content" || element.XName == "index-title")
					{
						Content::Text text(*i);
						HeaderLeft->push_back(text);
					}
				}
			/*XML::Fill(Header, element.element("header"), "p", "h", "list", "table", "table-of-content");
			XML::Fill(Footer, element.element("footer"), "p", "h", "list", "table", "table-of-content");
			XML::Fill(HeaderLeft, element.element("header-left"), "p", "h", "list", "table", "table-of-content");
			XML::Fill(FooterLeft, element.element("footer-left"), "p", "h", "list", "table", "table-of-content");*/
		}


		const XML::XNode MasterPage::toXML() const
		{
			return
				XML::XElement(ns.style + "master-page",
					XML::XAttribute(ns.style + "name", Name) +
					XML::XAttribute(ns.style + "page-layout-name", Layout) +
					XML::XAttribute(ns.style + "next-style-name", NextStyleName) +
					XML::WriteIf(XML::XElement(ns.style + "header", XML::Write(Header)), !Header->empty()) +
					XML::WriteIf(XML::XElement(ns.style + "footer", XML::Write(Footer)), !Footer->empty()) +
					XML::WriteIf(XML::XElement(ns.style + "header-left", XML::Write(HeaderLeft)), !HeaderLeft->empty()) +
					XML::WriteIf(XML::XElement(ns.style + "footer-left", XML::Write(FooterLeft)), !FooterLeft->empty())
				);
		}

	} // namespace Styles;
} // namespace Odt