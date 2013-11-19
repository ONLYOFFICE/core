
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Styles.h"
#include "Exception/log_runtime_error.h"


namespace Odt
{
	namespace Styles
	{

		Styles::Styles()
		{
		}


		Styles::~Styles()
		{
		}


		Styles::Styles(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Styles& Styles::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Styles::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			OutlineStyle = element.element("outline-style");
			LinenumberingConfiguration = element.element("linenumbering-configuration");
			DefaultStyles = element;
			NamedStyles		= element;
			ListStyles = element;
			XML::Fill(NotesConfigurations, element, "notes-configuration");
		}


		const XML::XNode Styles::toXML() const
		{
			return 
				XML::XElement(ns.office + "styles",
					XML::Write(DefaultStyles) +
					XML::Write(NamedStyles) +
					XML::Write(OutlineStyle) +
					XML::Write(ListStyles) +
					XML::Write(NotesConfigurations) +
					XML::Write(LinenumberingConfiguration)
				);
		}


		const NotesConfiguration Styles::getNotesConfiguration(const std::string& noteClass) const
		{
			BOOST_FOREACH(const NotesConfiguration& notesConfiguration, *NotesConfigurations)
			{
				if (notesConfiguration.NoteClass == noteClass)
					return notesConfiguration;
			}
			throw log_runtime_error("bad note class");
		}

	} // namespae Styles
} // namespace Odt