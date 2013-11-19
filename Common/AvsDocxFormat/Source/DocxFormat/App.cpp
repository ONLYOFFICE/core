
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "App.h"
#include "FileTypes.h"


namespace OOX
{

	App::App()
	{
	}


	App::App(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	App::~App()
	{
	}
		

	void App::read(const boost::filesystem::wpath& filename)
	{
		const XML::XDocument document(filename);

		m_template	=	document.Root.element("Template").text();
		m_totalTime = document.Root.element("TotalTime").text();
		Pages				=	document.Root.element("Pages").text();
		Words				=	document.Root.element("Words").text();
		Characters	=	document.Root.element("Characters").text();
		m_appllication=	document.Root.element("Application").text();
		m_docSecurity =	document.Root.element("DocSecurity").text();
		m_lines				=	document.Root.element("Lines").text();
		Paragraphs		=	document.Root.element("Paragraphs").text();
		m_scaleCrop		=	document.Root.element("ScaleCrop").text();
		m_company			=	document.Root.element("Company").text();
		m_linksUpToDate				=	document.Root.element("LinksUpToDate").text();
		CharactersWithSpaces	=	document.Root.element("CharactersWithSpaces").text();
		m_shareDoc						=	document.Root.element("SharedDoc").text();
		m_hyperLinksChanged		=	document.Root.element("HyperlinksChanged").text();
		m_appVersion					=	document.Root.element("AppVersion").text();
	}

		
	void App::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement element = XML::XElement("Properties",
			XML::XNamespace("xmlns", "http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"));
		if (m_template.is_init())
			element.Add(XML::XElement("Template",		XML::XText(m_template)));
		if (m_totalTime.is_init())
			element.Add(XML::XElement("TotalTime",		XML::XText(m_totalTime)));
		if (Pages.is_init())
			element.Add(XML::XElement("Pages",			XML::XText(Pages)));
		if (Words.is_init())
			element.Add(XML::XElement("Words",			XML::XText(Words)));
		if (Characters.is_init())
			element.Add(XML::XElement("Characters",		XML::XText(Characters)));
		if (m_appllication.is_init())
			element.Add(XML::XElement("Application",	XML::XText(m_appllication)));
		if (m_docSecurity.is_init())
			element.Add(XML::XElement("DocSecurity",	XML::XText(m_docSecurity)));
		if (m_template.is_init())
			element.Add(XML::XElement("Lines",			XML::XText(m_lines)));
		if (Paragraphs.is_init())
			element.Add(XML::XElement("Paragraphs",		XML::XText(Paragraphs)));
		if (m_scaleCrop.is_init())
			element.Add(XML::XElement("ScaleCrop",		XML::XText(m_scaleCrop)));
		if (m_company.is_init())
			element.Add(XML::XElement("Company",		XML::XText(m_company)));
		if (m_linksUpToDate.is_init())
			element.Add(XML::XElement("LinksUpToDate",	XML::XText(m_linksUpToDate)));
		if (CharactersWithSpaces.is_init())
			element.Add(XML::XElement("CharactersWithSpaces",	XML::XText(CharactersWithSpaces)));
		if (m_shareDoc.is_init())
			element.Add(XML::XElement("SharedDoc",		XML::XText(m_shareDoc)));
		if (m_hyperLinksChanged.is_init())
			element.Add(XML::XElement("HyperlinksChanged",		XML::XText(m_hyperLinksChanged)));
		if (m_appVersion.is_init())
			element.Add(XML::XElement("AppVersion",		XML::XText(m_appVersion)));
		element.Save(filename);
		

		content.registration(type().OverrideType(), directory, filename);
	}


	const FileType App::type() const
	{
		return FileTypes::App;
	}


	const boost::filesystem::wpath App::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath App::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX