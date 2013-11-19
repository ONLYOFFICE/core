#include "Theme.h"
#include "DocxFormat/FileTypes.h"
//#include "SlideMaster.h"
#include "DocxFormat/Media/Image.h"

namespace PPTX
{
	Theme::Theme()
	{
		m_map = NULL;
	}

	Theme::Theme(const boost::filesystem::wpath& filename, FileMap& map)
	{
		m_map = NULL;
		read(filename, map);
	}

	Theme::~Theme()
	{
	}

	void Theme::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);
		m_map = NULL;

		const XML::XDocument document(filename, true);
		name = document.Root.attribute("name").value();

		themeElements = document.Root.element("themeElements");
		themeElements->SetParentFilePointer(*this);

		if(document.Root.element("objectDefaults").exist())
		{
			const XML::XElement element = document.Root.element("objectDefaults");
			spDef = element.element("spDef");
			if(spDef.is_init())
				spDef->SetParentFilePointer(*this);
			lnDef = element.element("lnDef");
			if(lnDef.is_init())
				lnDef->SetParentFilePointer(*this);
			txDef = element.element("txDef");
			if(txDef.is_init())
				txDef->SetParentFilePointer(*this);
		}

		extraClrSchemeLst->clear();
		if(document.Root.element("extraClrSchemeLst").exist())
			XML::Fill(extraClrSchemeLst, document.Root.element("extraClrSchemeLst"), "extraClrScheme");
		for(std::list<nsTheme::ExtraClrScheme>::iterator i = extraClrSchemeLst->begin(); i != extraClrSchemeLst->end(); i++)
			i->SetParentFilePointer(*this);
	}

	void Theme::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.a + "theme",
			XML::XAttribute("name", name) +
			XML::Write(themeElements) +
			XML::XElement(ns.a + "objectDefaults",
				XML::Write(spDef) +
				XML::Write(lnDef) +
				XML::Write(txDef)
			) +
			XML::XElement(ns.a + "extraClrSchemeLst", XML::Write(extraClrSchemeLst))
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
		FileContainer::write(filename, directory, content);
	}

	const OOX::FileType Theme::type() const
	{
		return OOX::FileTypes::ThemePPTX;
	}

	const boost::filesystem::wpath Theme::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath Theme::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	DWORD Theme::GetRGBAFromScheme(const std::string& str)const
	{
		if(themeElements->clrScheme->Scheme->find(str) != themeElements->clrScheme->Scheme->end())
			return themeElements->clrScheme->GetRGBAFromScheme(str);
		//return themeElements->clrScheme->GetRGBAFromScheme(boost::shared_dynamic_cast<PPTX::SlideMaster, PPTX::WrapperFile>(Master)->clrMap->GetColorSchemeIndex(str));
		return 0;
	}

	DWORD Theme::GetARGBFromScheme(const std::string& str)const
	{
		if(themeElements->clrScheme->Scheme->find(str) != themeElements->clrScheme->Scheme->end())
			return themeElements->clrScheme->GetARGBFromScheme(str);
		//return themeElements->clrScheme->GetARGBFromScheme(boost::shared_dynamic_cast<PPTX::SlideMaster, PPTX::WrapperFile>(Master)->clrMap->GetColorSchemeIndex(str));
		return 0;
	}

	DWORD Theme::GetBGRAFromScheme(const std::string& str)const
	{
		if(themeElements->clrScheme->Scheme->find(str) != themeElements->clrScheme->Scheme->end())
			return themeElements->clrScheme->GetBGRAFromScheme(str);
		//return themeElements->clrScheme->GetBGRAFromScheme(boost::shared_dynamic_cast<PPTX::SlideMaster, PPTX::WrapperFile>(Master)->clrMap->GetColorSchemeIndex(str));
		return 0;
	}

	DWORD Theme::GetABGRFromScheme(const std::string& str)const
	{
		if(themeElements->clrScheme->Scheme->find(str) != themeElements->clrScheme->Scheme->end())
			return themeElements->clrScheme->GetABGRFromScheme(str);
		//return themeElements->clrScheme->GetABGRFromScheme(boost::shared_dynamic_cast<PPTX::SlideMaster, PPTX::WrapperFile>(Master)->clrMap->GetColorSchemeIndex(str));
		return 0;	
	}

	void Theme::FillShapeProperties(Logic::ShapeProperties& props, const std::string& type)const
	{
		if(Presentation != NULL)
			props.FillFromTextListStyle(Presentation->defaultTextStyle);
		if(type == "table-cell")
			props.FillMasterFontSize(1800);
		//if(spDef.is_init())
		//{
		//	props.FillFromTextListStyle(spDef->lstStyle, true);
		//	if(spDef->style.is_init())
		//		props.FillFontRef(spDef->style->fontRef.get());
		//}
		props.SetMajorLatin(themeElements->fontScheme->majorFont->latin);
		props.SetMinorLatin(themeElements->fontScheme->minorFont->latin);
	}

	std::wstring Theme::GetMediaFullPathNameFromRId(const OOX::RId& rid) const
	{
//			return relsTable.Media.GetFullFilePathById(rid);//Media.GetFileName(rid);
		return FileContainer::image(rid).filename().string();
	}

	void Theme::GetLineStyle(int number, Logic::Ln& lnStyle)const
	{
		themeElements->fmtScheme->GetLineStyle(number, lnStyle);
	}

	void Theme::GetFillStyle(int number, Logic::UniFill& fillStyle)const
	{
		themeElements->fmtScheme->GetFillStyle(number, fillStyle);
	}


	DWORD Theme::GetRGBAFromMap(const std::string& str)const
	{
		return GetRGBAFromScheme(m_map->GetColorSchemeIndex(str));
	}

	DWORD Theme::GetARGBFromMap(const std::string& str)const
	{
		return GetARGBFromScheme(m_map->GetColorSchemeIndex(str));
	}

	DWORD Theme::GetBGRAFromMap(const std::string& str)const
	{
		return GetBGRAFromScheme(m_map->GetColorSchemeIndex(str));
	}

	DWORD Theme::GetABGRFromMap(const std::string& str)const
	{
		return GetABGRFromScheme(m_map->GetColorSchemeIndex(str));
	}


} // namespace PPTX