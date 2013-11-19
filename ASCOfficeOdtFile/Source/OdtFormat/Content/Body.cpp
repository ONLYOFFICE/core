
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Body.h"
#include "Paragraph.h"
#include "Table.h"
#include "List.h"
#include "IndexTitle.h"
#include "TableOfContent.h"
#include "Section.h"


namespace Odt
{
	namespace Content
	{

		Body::Body()
		{
			m_hasList = false;
			m_hasNote = false;
		}


		Body::~Body()
		{
		}


		Body::Body(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Body& Body::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Body::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			UseSoftPageBreaks	= element.attribute("use-soft-page-breaks").value();
			m_forms						= element.element("text").element("forms");
			m_variableDecls		= element.element("text").element("variable-decls");
			m_sequenceDecls		= element.element("text").element("sequence-decls");

			m_hasList = false;
			m_hasNote = false;
			XML::Fill(Frames, element.element("text"), "frame");
			for (XML::const_element_iterator i = element.element("text").Elements.begin(); i != element.element("text").Elements.end(); ++i)
			{
				const XML::XElement element(*i);
				if (element.XName == "p" || element.XName == "h" || element.XName == "list" || element.XName == "table" || element.XName == "table-of-content" || element.XName == "index-title" || element.XName == "section")
				{
					Text text(*i);
					m_hasList |= text.hasList();
					m_hasNote |= text.hasNote();
					Items->push_back(text);
				}
			}
		}


		const XML::XNode Body::toXML() const
		{
			return 
				XML::XElement(ns.office + "body", 
					XML::XElement(ns.office + "text",
					XML::XAttribute(ns.text + "use-soft-page-breaks", UseSoftPageBreaks) +
						XML::Write(m_forms) +
						XML::Write(m_variableDecls) +
						XML::Write(m_sequenceDecls) +
						XML::Write(Frames) +
						XML::Write(Items)
					)
				);
		}


		const bool Body::hasList() const
		{
			return m_hasList;
		}


		const bool Body::hasNote() const
		{
			return m_hasNote;
		}

	} // namespace Content
} // namespace Odt