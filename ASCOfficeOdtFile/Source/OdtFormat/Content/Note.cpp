
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Note.h"


namespace Odt
{
	namespace Content
	{

		Note::Note()
		{
		}


		Note::~Note()
		{
		}


		Note::Note(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Note& Note::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Note::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Id			= element.attribute("id").value();
			Class		= element.attribute("note-class").value();
			Citation	= element.element("note-citation");
			XML::Fill(Texts, element.element("note-body"), "h", "p", "table", "list");		
		}


		const XML::XNode Note::toXML() const
		{
			return
				XML::XElement(ns.text + "note",
					XML::XAttribute(ns.text + "id", Id) + 
					XML::XAttribute(ns.text + "note-class", Class) +
					XML::Write(Citation) +
					XML::XElement(ns.text + "note-body", XML::Write(Texts))
				);
		}


		const std::string Note::toTxt() const
		{
			return std::string("[") + Class.ToString() + std::string("]");
		}
		
	} // namespace Content
} // namespace Odt