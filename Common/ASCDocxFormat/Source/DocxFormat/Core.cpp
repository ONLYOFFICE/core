
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Core.h"
#include "FileTypes.h"


namespace OOX
{

	Core::Core()
	{
	}


	Core::Core(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	Core::~Core()
	{
	}


	void Core::read(const boost::filesystem::wpath& filename)
	{
		const XML::XDocument document(filename);

		m_title		=	document.Root.element("title").text();
		m_subject	=	document.Root.element("subject").text();
		m_creator	=	document.Root.element("creator").text();
		m_keywords	=	document.Root.element("keywords").text();
		m_description	=	document.Root.element("description").text();
		m_lastModifiedBy =	document.Root.element("lastModifiedBy").text();
		m_revision = 1;
	}


	void Core::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.cp + "coreProperties",
			XML::XElement(ns.dc + "title",		XML::XText(m_title)) +
			XML::XElement(ns.dc + "subject",	XML::XText(m_subject)) +
			XML::XElement(ns.dc + "creator",	XML::XText(m_creator)) +
			XML::XElement(ns.cp + "keywords",	XML::XText(m_keywords)) +
			XML::XElement(ns.dc + "description",		XML::XText(m_description)) +
			XML::XElement(ns.cp + "lastModifiedBy", XML::XText(m_lastModifiedBy)) +
			XML::XElement(ns.cp + "revision",				XML::XText(m_revision)) +
			XML::WriteIf(XML::XElement(ns.dcterms + "created", 
								XML::XAttribute(ns.xsi + "type", "dcterms:W3CDTF") + 
								XML::XText(m_created)),
						 m_created.is_init()
			) +
			XML::WriteIf(XML::XElement(ns.dcterms + "modified", 
								XML::XAttribute(ns.xsi + "type", "dcterms:W3CDTF") + 
								XML::XText(m_modified)), 
						 m_modified.is_init()
			)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
	}


	const FileType Core::type() const
	{
		return FileTypes::Core;
	}


	const boost::filesystem::wpath Core::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath Core::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX