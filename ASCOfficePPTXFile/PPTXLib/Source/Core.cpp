#include "Core.h"
#include "XML.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	Core::Core()
	{
	}

	Core::Core(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	Core::~Core()
	{
	}

	void Core::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		const XML::XDocument document(filename, true);

		title = document.Root.element("title").text();
		creator = document.Root.element("creator").text();
		lastModifiedBy = document.Root.element("lastModifiedBy").text();
		revision = document.Root.element("revision").text();
//			created = OOX::DateTime::Parse(document.Root.element("created").text().ToString());
		created = document.Root.element("created").text();
//			modified = OOX::DateTime::Parse(document.Root.element("modified").text().ToString());
		modified = document.Root.element("modified").text();
/*
		category = document.Root.element("category").text();
		contentStatus = document.Root.element("contentStatus").text();
		description = document.Root.element("description").text();
//identifier - ???
//			keywords = document.Root.element("keywords").text();
		language = document.Root.element("language").text();
//			lastPrinted = OOX::DateTime::Parse(document.Root.element("lastPrinted").text().ToString());
		subject = document.Root.element("subject").text();
		version = document.Root.element("version").text();
*/
	}

	void Core::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.cp + "coreProperties",	
			XML::XNamespace(ns.dc) +
			XML::XNamespace(ns.dcterms) +
			XML::XNamespace(ns.dcmitype) +
			XML::XNamespace(ns.xsi) +
			XML::XElement(ns.dc + "title", XML::XText(title)) +
			XML::XElement(ns.dc + "creator", XML::XText(creator)) +
			XML::XElement(ns.cp + "lastModifiedBy", XML::XText(lastModifiedBy)) +
			XML::XElement(ns.cp + "revision", XML::XText(revision)) +
			XML::WriteIf(XML::XElement(ns.dcterms + "created", XML::XAttribute(ns.xsi + "type", "dcterms:W3CDTF") + XML::XText(created)), created.is_init()) +
			XML::XElement(ns.dcterms + "modified",
				XML::XAttribute(ns.xsi + "type", "dcterms:W3CDTF") + 
				XML::XText(modified))
		).Save(filename);
/*
		if(category.is_init())
			root.Add(XML::XElement("category", XML::XText(category)));
		if(contentStatus.is_init())
			root.Add(XML::XElement("contentStatus", XML::XText(contentStatus)));
		if(description.is_init())
			root.Add(XML::XElement(ns.dc + "description", XML::XText(description)));
		//identifier - ??? dc
			//XML::XElement(ns.cp + "keywords", XML::XText(keywords)) +
		if(language.is_init())
			root.Add(XML::XElement(ns.dc + "language", XML::XText(language)));
//			if(lastModifiedBy.is_init())
//				root.Add(XML::XElement(ns.cp + "lastModifiedBy", XML::XText(lastModifiedBy)));
			//XML::XElement("lastPrinted",
			//	XML::XAttribute(,)+
			//	XML::XText(lastPrinted)
			//) + //??? namespace ???
//			if(revision.is_init())
//				root.Add(XML::XElement(ns.cp + "revision", XML::XText(revision)));
		if(subject.is_init())
			root.Add(XML::XElement(ns.dc + "subject", XML::XText(subject)));
//			if(title.is_init())
//				root.Add(XML::XElement(ns.dc + "title", XML::XText(title)));
		if(version.is_init())
			root.Add(XML::XElement("version", XML::XText(version)));
*/
		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
	}

	const OOX::FileType Core::type() const
	{
		return OOX::FileTypes::Core;
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