
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "NotesConfiguration.h"


namespace Odt
{
	namespace Styles
	{

		NotesConfiguration::NotesConfiguration()
		{
		}


		NotesConfiguration::~NotesConfiguration()
		{
		}


		NotesConfiguration::NotesConfiguration(const XML::XNode& node)
		{
			fromXML(node);
		}


		const NotesConfiguration& NotesConfiguration::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void NotesConfiguration::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			NoteClass					=	element.attribute("note-class").value();
			NumFormat					=	element.attribute("num-format").value();
			CitationStyleName = element.attribute("citation-style-name").value();
			CitationBodyStyleName = element.attribute("citation-body-style-name").value();
			MasterPageName		= element.attribute("master-page-name").value();			
			StartValue				=	element.attribute("start-value").value();
			FootnotesPosition =	element.attribute("footnotes-position").value();
			StartNumberingAt	=	element.attribute("start-numbering-at").value();
		}


		const XML::XNode NotesConfiguration::toXML() const
		{
			return
				XML::XElement(ns.text + "notes-configuration",
					XML::XAttribute(ns.text + "note-class", NoteClass) +
					XML::XAttribute(ns.text + "citation-style-name", CitationStyleName) +
					XML::XAttribute(ns.text + "citation-body-style-name", CitationBodyStyleName) +
					XML::XAttribute(ns.text + "master-page-name", MasterPageName) +
					XML::XAttribute(ns.style + "num-format", NumFormat) +
					XML::XAttribute(ns.text + "start-value", StartValue) +
					XML::XAttribute(ns.text + "footnotes-position", FootnotesPosition) +
					XML::XAttribute(ns.text + "start-numbering-at", StartNumberingAt)
				);
		}

	} // namespace Styles
} // namespace Odt