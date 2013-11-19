#include "Presentation.h"
#include "XML.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	Presentation::Presentation()
	{
	}

	Presentation::Presentation(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	Presentation::~Presentation()
	{
	}

	void Presentation::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);

		const XML::XDocument document(filename, true);
		const XML::XElement Root = document.Root;

		attrAutoCompressPictures = Root.attribute("autoCompressPictures").value();
		attrBookmarkIdSeed = Root.attribute("bookmarkIdSeed").value();
		attrCompatMode = Root.attribute("compatMode").value();
		attrConformance = Root.attribute("conformance").value();
		attrEmbedTrueTypeFonts = Root.attribute("embedTrueTypeFonts").value();
		attrFirstSlideNum = Root.attribute("firstSlideNum").value();
		attrRemovePersonalInfoOnSave = Root.attribute("removePersonalInfoOnSave").value();
		attrRtl = Root.attribute("rtl").value();
		attrSaveSubsetFonts = Root.attribute("saveSubsetFonts").value();
		attrServerZoom = Root.attribute("serverZoom").value();
		attrShowSpecialPlsOnTitleSld = Root.attribute("showSpecialPlsOnTitleSld").value();
		attrStrictFirstAndLastChars = Root.attribute("strictFirstAndLastChars").value();

		//custDataLst (Customer Data List)
		//custShowLst (List of Custom Shows)
		defaultTextStyle = Root.element("defaultTextStyle");
		if(defaultTextStyle.is_init())
			defaultTextStyle->SetParentFilePointer(*this);

		embeddedFontLst->clear();
		if(Root.element("embeddedFontLst").exist())
			XML::Fill(embeddedFontLst, Root.element("embeddedFontLst"), "embeddedFont");
		for(std::list<nsPresentation::EmbeddedFont>::iterator i = embeddedFontLst->begin(); i != embeddedFontLst->end(); i++)
			i->SetParentFilePointer(*this);

		handoutMasterIdLst->clear();
		if(Root.element("handoutMasterIdLst").exist())
			XML::Fill(handoutMasterIdLst, Root.element("handoutMasterIdLst"), "handoutMasterId");
		for(std::list<Logic::XmlId>::iterator i = handoutMasterIdLst->begin(); i != handoutMasterIdLst->end(); i++)
			i->SetParentFilePointer(*this);

		kinsoku = Root.element("kinsoku");
		if(kinsoku.is_init())
			kinsoku->SetParentFilePointer(*this);
		//modifyVerifier (Modification Verifier)

		notesMasterIdLst->clear();
		if(Root.element("notesMasterIdLst").exist())
			XML::Fill(notesMasterIdLst, Root.element("notesMasterIdLst"), "notesMasterId");
		for(std::list<Logic::XmlId>::iterator i = notesMasterIdLst->begin(); i != notesMasterIdLst->end(); i++)
			i->SetParentFilePointer(*this);

		notesSz = Root.element("notesSz");
		if(notesSz.is_init())
			notesSz->SetParentFilePointer(*this);
		photoAlbum = Root.element("photoAlbum");
		if(photoAlbum.is_init())
			photoAlbum->SetParentFilePointer(*this);

		sldIdLst->clear();
		if(Root.element("sldIdLst").exist())
			XML::Fill(sldIdLst, Root.element("sldIdLst"), "sldId");
		for(std::list<Logic::XmlId>::iterator i = sldIdLst->begin(); i != sldIdLst->end(); i++)
			i->SetParentFilePointer(*this);

		sldMasterIdLst->clear();
		if(Root.element("sldMasterIdLst").exist())
			XML::Fill(sldMasterIdLst, Root.element("sldMasterIdLst"), "sldMasterId");
		for(std::list<Logic::XmlId>::iterator i = sldMasterIdLst->begin(); i != sldMasterIdLst->end(); i++)
			i->SetParentFilePointer(*this);

		sldSz = Root.element("sldSz");
		if(sldSz.is_init())
			sldSz->SetParentFilePointer(*this);
		//smartTags (Smart Tags)
	}

	void Presentation::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.p+"presentation",
				XML::XNamespace(ns.a) +
				XML::XNamespace(ns.r) +
				XML::XAttribute("autoCompressPictures", attrAutoCompressPictures) +
				XML::XAttribute("bookmarkIdSeed", attrBookmarkIdSeed) +
				XML::XAttribute("compatMode", attrCompatMode) +
				XML::XAttribute("conformance", attrConformance) +
				XML::XAttribute("embedTrueTypeFonts", attrEmbedTrueTypeFonts) +
				XML::XAttribute("firstSlideNum", attrFirstSlideNum) +
				XML::XAttribute("removePersonalInfoOnSave", attrRemovePersonalInfoOnSave) +
				XML::XAttribute("rtl", attrRtl) +
				XML::XAttribute("saveSubsetFonts", attrSaveSubsetFonts) +
				XML::XAttribute("serverZoom", attrServerZoom) +
				XML::XAttribute("showSpecialPlsOnTitleSld", attrShowSpecialPlsOnTitleSld) +
				XML::XAttribute("strictFirstAndLastChars", attrStrictFirstAndLastChars) +
				XML::XElement(ns.p + "sldMasterIdLst", XML::Write(sldMasterIdLst)) +
				XML::WriteIf(XML::XElement(ns.p + "notesMasterIdLst", Write(notesMasterIdLst)), !notesMasterIdLst->empty()) +
				XML::WriteIf(XML::XElement(ns.p + "handoutMasterIdLst", Write(handoutMasterIdLst)), !handoutMasterIdLst->empty()) +
				XML::XElement(ns.p + "sldIdLst", XML::Write(sldIdLst)) +
				XML::Write(sldSz) +
				XML::Write(notesSz) +
				XML::WriteIf(XML::XElement(ns.p + "embeddedFontLst", XML::Write(embeddedFontLst)), !embeddedFontLst->empty()) +
				XML::Write(photoAlbum) +
				XML::Write(kinsoku) +
				XML::Write(defaultTextStyle)
			).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
		FileContainer::write(filename, directory, content);
	}

	const OOX::FileType Presentation::type() const
	{
		return OOX::FileTypes::Presentation;
	}

	const boost::filesystem::wpath Presentation::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath Presentation::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	DWORD Presentation::GetRGBAFromMap(const std::string& str)const
	{
		return m_clrScheme->GetRGBAFromScheme(m_clrMap->GetColorSchemeIndex(str));
	}

	DWORD Presentation::GetARGBFromMap(const std::string& str)const
	{
		return m_clrScheme->GetARGBFromScheme(m_clrMap->GetColorSchemeIndex(str));
	}

	DWORD Presentation::GetBGRAFromMap(const std::string& str)const
	{
		return m_clrScheme->GetBGRAFromScheme(m_clrMap->GetColorSchemeIndex(str));
	}

	DWORD Presentation::GetABGRFromMap(const std::string& str)const
	{
		return m_clrScheme->GetABGRFromScheme(m_clrMap->GetColorSchemeIndex(str));
	}
} // namespace PPTX
