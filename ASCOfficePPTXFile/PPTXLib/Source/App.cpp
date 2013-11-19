#include "App.h"
#include <string>
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	App::App()
	{
	}

	App::App(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	App::~App()
	{
	}

	void App::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		HeadingPairs->clear();
		TitlesOfParts->clear();
		const XML::XDocument document(filename, true);

		Template = document.Root.element("Template").text();
		TotalTime = document.Root.element("TotalTime").text();
		Words = document.Root.element("Words").text();
		Application = document.Root.element("Application").text();
		PresentationFormat = document.Root.element("PresentationFormat").text();
		Paragraphs = document.Root.element("Paragraphs").text();
		Slides = document.Root.element("Slides").text();
		Notes = document.Root.element("Notes").text();
		HiddenSlides = document.Root.element("HiddenSlides").text();
		MMClips = document.Root.element("MMClips").text();
		ScaleCrop = document.Root.element("ScaleCrop").text();
		if(document.Root.element("HeadingPairs").exist()){
			const XML::XElement vector = document.Root.element("HeadingPairs").element("vector");
			m_Headings = vector.attribute("size").value();
			XML::Fill(HeadingPairs, vector, "variant");
		}
		if(document.Root.element("TitlesOfParts").exist()){
			const XML::XElement vector = document.Root.element("TitlesOfParts").element("vector");
			m_VectorSize = vector.attribute("size").value();
			XML::Fill(TitlesOfParts, vector, "lpstr");
		}
		Company = document.Root.element("Company").text();
		LinksUpToDate = document.Root.element("LinksUpToDate").text();
		SharedDoc = document.Root.element("SharedDoc").text();
		HyperlinksChanged = document.Root.element("HyperlinksChanged").text();
		AppVersion = document.Root.element("AppVersion").text();
//			Characters = document.Root.element("Characters").text();
//			CharactersWithSpaces = document.Root.element("CharactersWithSpaces").text();
//DigSig (Digital Signature)
//			DocSecurity = document.Root.element("DocSecurity").text();
//HLinks
//			HyperlinkBase = document.Root.element("HyperlinkBase").text();
//			Lines = document.Root.element("Lines").text();
//			Manager = document.Root.element("Manager").text();
//			Pages = document.Root.element("Pages").text();
	}

	void App::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement("Properties",
			XML::XNamespace(ns.xmlns) + 
			XML::XNamespace(ns.vt) +
			XML::XElement("Template", XML::XText(Template)) +
			XML::XElement("TotalTime", XML::XText(TotalTime)) +
			XML::XElement("Words", XML::XText(Words)) +
			XML::XElement("Application", XML::XText(Application)) +
			XML::XElement("PresentationFormat", XML::XText(PresentationFormat)) +
			XML::XElement("Paragraphs", XML::XText(Paragraphs)) +
			XML::XElement("Slides", XML::XText(Slides)) +
			XML::XElement("Notes", XML::XText(Notes)) +
			XML::XElement("HiddenSlides", XML::XText(HiddenSlides)) +
			XML::XElement("MMClips", XML::XText(MMClips)) +
			XML::XElement("ScaleCrop", XML::XText(ScaleCrop)) +
			XML::XElement("HeadingPairs", 
				XML::XElement(ns.vt + "vector",
					XML::XAttribute("size", m_Headings) +
					XML::XAttribute("baseType", "variant") +
					XML::Write(HeadingPairs)
				)
			) +
			XML::XElement("TitlesOfParts", 
				XML::XElement(ns.vt + "vector",
					XML::XAttribute("size", m_VectorSize) +
					XML::XAttribute("baseType", "lpstr") +
					XML::Write(TitlesOfParts)
				)
			) +
			XML::XElement("Company", XML::XText(Company)) +
			XML::XElement("LinksUpToDate", XML::XText(LinksUpToDate)) +
			XML::XElement("SharedDoc", XML::XText(SharedDoc)) +
			XML::XElement("HyperlinksChanged", XML::XText(HyperlinksChanged)) +
			XML::XElement("AppVersion", XML::XText(AppVersion))
		).Save(filename);

//			if(DocSecurity.is_init())
//				root.Add(XML::XElement("DocSecurity", XML::XText(DocSecurity)));
//			if(Manager.is_init())
//				root.Add(XML::XElement("Manager", XML::XText(Manager)));
//			if(HyperlinkBase.is_init())
//				root.Add(XML::XElement("HyperlinkBase", XML::XText(HyperlinkBase)));
//				XML::XElement("Characters", XML::XText(Characters)) +
//				XML::XElement("Lines", XML::XText(Lines)) +
//				XML::XElement("CharactersWithSpaces", XML::XText(CharactersWithSpaces))
//DigSig (Digital Signature)
//HLinks
		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
	}

	const OOX::FileType App::type() const
	{
		return OOX::FileTypes::App;
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