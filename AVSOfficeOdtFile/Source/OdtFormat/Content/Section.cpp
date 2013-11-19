
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Section.h"


namespace Odt
{
	namespace Content
	{

		Section::Section()
		{
			m_hasNote = false;
			m_hasList = false;
		}
		

		Section::~Section()
		{
		}


		Section::Section(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Section& Section::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Section::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Style		= element.attribute("style-name").value();
			Name = element.attribute("name").value();

			m_hasNote = false;
			m_hasList = false;
			//for (XML::const_node_iterator i = element.Nodes.begin(); i != element.Nodes.end(); ++i)
			//{
			//	Text run(*i);
			//	m_hasNote |= run.hasNote();
			//	m_hasList |= run.hasList();
			//	Items->push_back(run);
			//}
			for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
			{
				const XML::XElement child_element(*i);
				if (child_element.XName == "p" || child_element.XName == "h" || child_element.XName == "list" || child_element.XName == "table"
					|| child_element.XName == "table-of-content" || child_element.XName == "index-title" || child_element.XName == "section")
				{
					Text text(*i);
					m_hasList |= text.hasList();
					m_hasNote |= text.hasNote();
					Items->push_back(text);
				}
			}
		}


		const XML::XNode Section::toXML() const
		{
			return 
				 XML::XElement(ns.text + "section", 
					XML::XAttribute(ns.text + "style-name", Style) +
					XML::XAttribute(ns.text + "name", Name) + 
					XML::Write(Items)
				);
		}


		const bool Section::hasList() const
		{
			return m_hasList;
		}


		const bool Section::hasNote() const
		{
			return m_hasNote;
		}

	} // namespace Content
} // namespace Odt