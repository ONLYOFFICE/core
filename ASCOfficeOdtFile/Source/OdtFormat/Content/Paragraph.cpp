
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Paragraph.h"
#include "Note.h"


namespace Odt
{
	namespace Content
	{

		Paragraph::Paragraph()
		{
			m_type = "p";
			m_hasNote = false;
		}
		

		Paragraph::~Paragraph()
		{
		}


		Paragraph::Paragraph(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Paragraph& Paragraph::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Paragraph::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_type	= element.XName->Name;
			Style		= element.attribute("style-name").value();
			m_outlineLevel = element.attribute("outline-level").value();

			m_hasNote = false;
			for (XML::const_node_iterator i = element.Nodes.begin(); i != element.Nodes.end(); ++i)
			{
				ParagraphItem run(*i);
				m_hasNote |= run.is<Note>();
				Items->push_back(run);
			}
		}


		const XML::XNode Paragraph::toXML() const
		{
			return 
				 XML::XElement(ns.text + m_type, 
					XML::XAttribute(ns.text + "style-name", Style) +
					XML::XAttribute(ns.text + "outline-level", m_outlineLevel) + 
					XML::Write(Items)
				);
		}


		void Paragraph::AddText(const std::string& text)
		{
			ParagraphItem item(text);
			Items->push_back(item);
		}


		const bool Paragraph::hasList() const
		{
			return false;
		}


		const bool Paragraph::hasNote() const
		{
			return m_hasNote;
		}

	} // namespace Content
} // namespace Odt