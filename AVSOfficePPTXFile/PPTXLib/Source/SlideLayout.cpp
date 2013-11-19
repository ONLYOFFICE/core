#include "SlideLayout.h"
#include "DocxFormat/FileTypes.h"
#include "Logic/SpTreeElem.h"
#include "DocxFormat/Media/Image.h"

namespace PPTX
{
	SlideLayout::SlideLayout()
	{
	}

	SlideLayout::SlideLayout(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	SlideLayout::~SlideLayout()
	{
	}

	void SlideLayout::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);

		const XML::XDocument document(filename, true);
		XML::XElement root = document.Root;

		cSld = root.element("cSld");
		cSld->SetParentFilePointer(*this);

		clrMapOvr = root.element("clrMapOvr");
		if(clrMapOvr.is_init())
			clrMapOvr->SetParentFilePointer(*this);

		transition = root.element("transition");
		if(transition.is_init())
			transition->SetParentFilePointer(*this);

		timing = root.element("timing");
		if(timing.is_init())
			timing->SetParentFilePointer(*this);

		hf = root.element("hf");
		if(hf.is_init())
			hf->SetParentFilePointer(*this);

		matchingName = root.attribute("matchingName").value();
		preserve = root.attribute("preserve").value();
		showMasterPhAnim = root.attribute("showMasterPhAnim").value();
		showMasterSp = root.attribute("showMasterSp").value();
		attrType = root.attribute("type").value();
		userDrawn = root.attribute("userDrawn").value();
	}

	void SlideLayout::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.p + "sldLayout",
			XML::XNamespace(ns.a) +
			XML::XNamespace(ns.r) +
			XML::XAttribute("matchingName", matchingName) +
			XML::XAttribute("preserve", preserve) +
			XML::XAttribute("showMasterPhAnim", showMasterPhAnim) +
			XML::XAttribute("showMasterSp", showMasterSp) +
			XML::XAttribute("type", attrType) +
			XML::XAttribute("userDrawn", userDrawn) +
			XML::Write(cSld) +
			XML::Write(clrMapOvr) +
			XML::Write(transition) +
			XML::Write(timing)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
		FileContainer::write(filename, directory, content);
	}

	const OOX::FileType SlideLayout::type() const
	{
		return OOX::FileTypes::SlideLayout;
	}

	const boost::filesystem::wpath SlideLayout::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath SlideLayout::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	void SlideLayout::GetLevelUp(const Logic::Shape& pShape)const
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
					const Logic::Shape& LayoutShape = i->as<Logic::Shape>();
					if(LayoutShape.nvSpPr->nvPr->ph.is_init())
					{
						std::string lIdx = /*LayoutShape->nvSpPr->nvPr->ph.is_init()?*/LayoutShape.nvSpPr->nvPr->ph->idx.get_value_or("0");//:"";
						std::string lType = /*LayoutShape->nvSpPr->nvPr->ph.is_init()?*/LayoutShape.nvSpPr->nvPr->ph->type.get_value_or("body");//:"";
						if(lType == "ctrTitle")
							lType = "title";
						if((type == lType) && (idx == lIdx))
						{
							pShape.SetLevelUpElement(LayoutShape);
							return;
						}

						//if(lType == "ctrTitle")
						//	lType = "title";
						//if(idx == LayoutShape->nvSpPr->nvPr->idx.get_value_or("0"))
						//{
						//	if((type == LayoutShape->nvSpPr->nvPr->type.get_value_or("")) || ((type == "") && (LayoutShape->nvSpPr->nvPr->type.get_value_or("") != "")))
						//		pShape->SetLevelUpElement(LayoutShape);
						//	return;
						//}
						//if((type == lType) && (type != ""))
						//{
						//	if(idx == lIdx)
						//	{
						//		pShape.SetLevelUpElement(LayoutShape);
						//		return;
						//	}
						//	continue;
						//}
						//if((type == lType) && (type == ""))
						//{
						//	if((idx == lIdx) && (idx != ""))
						//	{
						//		pShape.SetLevelUpElement(LayoutShape);
						//		return;
						//	}
						//}
						//if(type != lType)
						//{
						//	if((idx == lIdx) && (idx != ""))
						//	{
						//		pShape.SetLevelUpElement(LayoutShape);
						//		return;
						//	}
						//}
					}
				}
			}
		}
	}

	void SlideLayout::FillShapeProperties(Logic::ShapeProperties& props, const std::string& type)const
	{
		if(Master != NULL)
			Master->FillShapeProperties(props, type);
	}

	void SlideLayout::GetBackground(Logic::BgPr& bg, DWORD& ARGB)const
	{
		if(cSld->bg.is_init())
		{
			if(cSld->bg->bgPr.is_init())
				bg = cSld->bg->bgPr.get();
			else if(cSld->bg->bgRef.is_init())
			{
				ARGB = cSld->bg->bgRef->Color.GetARGB();
				Theme->themeElements->fmtScheme->GetFillStyle(cSld->bg->bgRef->idx.get_value_or(0), bg.Fill);
				//bg.SetParentFilePointer(this);
			}
		}
		else//from slideMaster
		{
			if(Master != NULL)
				Master->GetBackground(bg, ARGB);
		}
	}

	std::wstring SlideLayout::GetMediaFullPathNameFromRId(const OOX::RId& rid) const
	{
//			return relsTable.Media.GetFullFilePathById(rid);//Media.GetFileName(rid);
		return FileContainer::image(rid).filename().string();
	}

	void SlideLayout::ApplyRels()
	{
		Master = boost::shared_dynamic_cast<PPTX::SlideMaster, OOX::File>(FileContainer::get(OOX::FileTypes::SlideMaster));
		Theme = Master->Theme;
		TableStyles = boost::shared_dynamic_cast<PPTX::TableStyles, OOX::File>(Theme->Presentation->get(OOX::FileTypes::TableStyles));
	}

	const boost::filesystem::wpath SlideLayout::GetPathBySpid(const std::string& spid)const
	{
		boost::filesystem::wpath filename = L"";
		if((Vml != 0) && (spid != ""))
		{
			if(Vml->SpIds.find(spid) != Vml->SpIds.end())
				filename = Vml->SpIds.find(spid)->second;
		}
		return filename;
	}

	DWORD SlideLayout::GetRGBAFromMap(const std::string& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Master->GetRGBAFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Master->GetRGBAFromMap(str);
		return Master->GetRGBAFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}

	DWORD SlideLayout::GetARGBFromMap(const std::string& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Master->GetARGBFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Master->GetARGBFromMap(str);
		return Master->GetARGBFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}

	DWORD SlideLayout::GetBGRAFromMap(const std::string& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Master->GetBGRAFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Master->GetBGRAFromMap(str);
		return Master->GetBGRAFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}

	DWORD SlideLayout::GetABGRFromMap(const std::string& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Master->GetABGRFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Master->GetABGRFromMap(str);
		return Master->GetABGRFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}
} // namespace PPTX