#include "SlideMaster.h"
#include "DocxFormat/FileTypes.h"
#include "Logic/UniColor.h"
#include "DocxFormat/Media/Image.h"

namespace PPTX
{
	SlideMaster::SlideMaster()
	{
	}

	SlideMaster::SlideMaster(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	SlideMaster::~SlideMaster()
	{
	}

	void SlideMaster::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);
		const XML::XDocument document(filename, true);
		
		preserve = document.Root.attribute("preserve").value();

		cSld = document.Root.element("cSld");
		cSld->SetParentFilePointer(*this);

		clrMap = document.Root.element("clrMap");
		clrMap->SetParentFilePointer(*this);

		sldLayoutIdLst->clear();
		XML::Fill(sldLayoutIdLst, document.Root.element("sldLayoutIdLst"),"sldLayoutId");
		for(std::list<Logic::XmlId>::iterator i = sldLayoutIdLst->begin(); i != sldLayoutIdLst->end(); i++)
			i->SetParentFilePointer(*this);

		transition = document.Root.element("transition");
		if(transition.is_init())
			transition->SetParentFilePointer(*this);

		timing = document.Root.element("timing");
		if(timing.is_init())
			timing->SetParentFilePointer(*this);

		hf = document.Root.element("hf");
		if(hf.is_init())
			hf->SetParentFilePointer(*this);

		txStyles = document.Root.element("txStyles");
		if(txStyles.is_init())
			txStyles->SetParentFilePointer(*this);
	}

	void SlideMaster::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.p + "sldMaster",
			XML::XNamespace(ns.a) +
			XML::XNamespace(ns.r) +
			XML::XAttribute("preserve", preserve) +
			XML::Write(cSld) +
			XML::Write(clrMap) +
			XML::XElement(ns.p + "sldLayoutIdLst",
				XML::Write(sldLayoutIdLst)
			) +
			XML::Write(transition) +
			XML::Write(timing) +
			XML::Write(hf) +
			XML::Write(txStyles)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
		FileContainer::write(filename, directory, content);
	}

	const OOX::FileType SlideMaster::type() const
	{
		return OOX::FileTypes::SlideMaster;
	}

	const boost::filesystem::wpath SlideMaster::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath SlideMaster::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	void SlideMaster::GetLevelUp(const Logic::Shape& pShape)const
	{
		if(pShape.nvSpPr->nvPr->ph.is_init())
		{
			std::string idx = /*pShape.nvSpPr->nvPr->ph.is_init()?*/pShape.nvSpPr->nvPr->ph->idx.get_value_or("0");//:"";
			std::string type = /*pShape.nvSpPr->nvPr->ph.is_init()?*/pShape.nvSpPr->nvPr->ph->type.get_value_or("body");//:"";
			if(type == "ctrTitle")
				type = "title";

			for(std::list<Logic::SpTreeElem>::const_iterator i = cSld->spTree->SpTreeElems->begin(); i != cSld->spTree->SpTreeElems->end(); i++)
			{
				if(i->is<Logic::Shape>())
				{
					const Logic::Shape& MasterShape = i->as<Logic::Shape>();
					if(MasterShape.nvSpPr->nvPr->ph.is_init())
					{
						std::string lIdx = /*MasterShape->nvSpPr->nvPr->ph.is_init()?*/MasterShape.nvSpPr->nvPr->ph->idx.get_value_or("0");//:"";
						std::string lType = /*MasterShape->nvSpPr->nvPr->ph.is_init()?*/MasterShape.nvSpPr->nvPr->ph->type.get_value_or("body");//:"";
						if(lType == "ctrTitle")
							lType = "title";
						if(type == lType)
						{
							pShape.SetLevelUpElement(MasterShape);
							return;
						}

						//if(idx == MasterShape->nvSpPr->nvPr->idx.get_value_or("0"))
						//{
						//	if((type == MasterShape->nvSpPr->nvPr->type.get_value_or("")) || ((type == "") && (MasterShape->nvSpPr->nvPr->type.get_value_or("") != "")))
						//		pShape->SetLevelUpElement(MasterShape);
						//	return;
						//}
						//if((type == lType) && (type != ""))
						//{
						//	//if(idx == lIdx)
						//	//{
						//		pShape.SetLevelUpElement(MasterShape);
						//		return;
						//	//}
						//	//continue;
						//}
						//if((type == lType) && (type == ""))
						//{
						//	if((idx == lType) && (idx != ""))
						//	{
						//		pShape.SetLevelUpElement(MasterShape);
						//		return;
						//	}
						//}
						//if(type != lType)
						//{
						//	if((idx == lIdx) && (idx != ""))
						//	{
						//		pShape.SetLevelUpElement(MasterShape);
						//		return;
						//	}
						//}
					}
				}
			}
		}
	}

	void SlideMaster::FillShapeProperties(Logic::ShapeProperties& props, const std::string& type)const
	{
		if((Theme != NULL) && (Theme->Presentation != NULL))
		{
			Theme->Presentation->SetClrMap(clrMap);
			Theme->Presentation->SetClrScheme(Theme->themeElements->clrScheme);
		}
		if((Theme != NULL) && (type != ""))
			Theme->FillShapeProperties(props, type);

		if(txStyles.is_init())
		{
//			props.FillFromTextListStyle(txStyles->otherStyle, true);

			//if(type == "")
			//{
			//	if(Theme->spDef.is_init())
			//	{
			//		props.FillFromTextListStyle(Theme->spDef->lstStyle, true);
			//		if(Theme->spDef->style.is_init())
			//			props.FillFontRef(Theme->spDef->style->fontRef.get());
			//	}
			//	else
			//		props.FillFromTextListStyle(txStyles->otherStyle, true);
			//	return;
			//}

			if((type == "title") || (type == "ctrTitle"))
				props.FillFromTextListStyle(txStyles->titleStyle);
			else if((type == "body") || (type == "subTitle") || (type == "obj"))
				props.FillFromTextListStyle(txStyles->bodyStyle);
			else if(type != "")
				props.FillFromTextListStyle(txStyles->otherStyle);
			else
			{
				props.FillFromTextListStyle(txStyles->otherStyle);
				if(Theme != NULL)
					Theme->FillShapeProperties(props, type);
			}
		}
	}

	void SlideMaster::GetBackground(Logic::BgPr& bg, DWORD& ARGB)const
	{
		if(cSld->bg.is_init())
		{
			if(cSld->bg->bgPr.is_init())
				bg = cSld->bg->bgPr.get();
			else if(cSld->bg->bgRef.is_init())
			{
				ARGB = cSld->bg->bgRef->Color.GetARGB();
				Theme->themeElements->fmtScheme->GetFillStyle(cSld->bg->bgRef->idx.get_value_or(0), bg.Fill);
//					bg.SetParentFilePointer(this);
			}
		}
	}

	//void SlideMaster::ApplyColors()
	//{
	//	if(Theme != NULL)
	//	{
	//		Theme->themeElements->clrScheme->Scheme->insert(std::pair<std::string, Logic::UniColor>("bg1",
	//			Theme->themeElements->clrScheme->Scheme->find(clrMap->ColorMap->find("bg1")->second.get())->second));
	//		Theme->themeElements->clrScheme->Scheme->insert(std::pair<std::string, Logic::UniColor>("bg2",
	//			Theme->themeElements->clrScheme->Scheme->find(clrMap->ColorMap->find("bg2")->second.get())->second));
	//		Theme->themeElements->clrScheme->Scheme->insert(std::pair<std::string, Logic::UniColor>("tx1",
	//			Theme->themeElements->clrScheme->Scheme->find(clrMap->ColorMap->find("tx1")->second.get())->second));
	//		Theme->themeElements->clrScheme->Scheme->insert(std::pair<std::string, Logic::UniColor>("tx2",
	//			Theme->themeElements->clrScheme->Scheme->find(clrMap->ColorMap->find("tx2")->second.get())->second));
	//	}
	//}

	std::wstring SlideMaster::GetMediaFullPathNameFromRId(const OOX::RId& rid) const
	{
//			return relsTable.Media.GetFullFilePathById(rid);//Media.GetFileName(rid);
		return FileContainer::image(rid).filename().string();
	}

	void SlideMaster::ApplyRels()
	{
		Theme = boost::shared_dynamic_cast<PPTX::Theme, OOX::File>(FileContainer::get(OOX::FileTypes::ThemePPTX));
		Theme->SetColorMap(*clrMap);
		//Theme->Master.reset((PPTX::WrapperFile*)this);
		TableStyles = boost::shared_dynamic_cast<PPTX::TableStyles, OOX::File>(Theme->Presentation->get(OOX::FileTypes::TableStyles));
	}

	const boost::filesystem::wpath SlideMaster::GetPathBySpid(const std::string& spid)const
	{
		boost::filesystem::wpath filename = L"";
		if((Vml != 0) && (spid != ""))
		{
			if(Vml->SpIds.find(spid) != Vml->SpIds.end())
				filename = Vml->SpIds.find(spid)->second;
		}
		return filename;
	}

	//---------------------Colors from map---------------------------------------

	DWORD SlideMaster::GetRGBAFromMap(const std::string& str)const
	{
		return Theme->GetRGBAFromScheme(clrMap->GetColorSchemeIndex(str));
	}

	DWORD SlideMaster::GetARGBFromMap(const std::string& str)const
	{
		return Theme->GetARGBFromScheme(clrMap->GetColorSchemeIndex(str));
	}

	DWORD SlideMaster::GetBGRAFromMap(const std::string& str)const
	{
		return Theme->GetBGRAFromScheme(clrMap->GetColorSchemeIndex(str));
	}

	DWORD SlideMaster::GetABGRFromMap(const std::string& str)const
	{
		return Theme->GetABGRFromScheme(clrMap->GetColorSchemeIndex(str));
	}

	//---------------------------Colors from scheme------------------------------

	DWORD SlideMaster::GetRGBAFromScheme(const std::string& str)const
	{
		return Theme->GetRGBAFromScheme(str);
	}

	DWORD SlideMaster::GetARGBFromScheme(const std::string& str)const
	{
		return Theme->GetARGBFromScheme(str);
	}

	DWORD SlideMaster::GetBGRAFromScheme(const std::string& str)const
	{
		return Theme->GetBGRAFromScheme(str);
	}

	DWORD SlideMaster::GetABGRFromScheme(const std::string& str)const
	{
		return Theme->GetABGRFromScheme(str);
	}
} // namespace PPTX