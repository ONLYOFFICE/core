#include "Slide.h"
#include "DocxFormat/FileTypes.h"
#include "DocxFormat/Media/Image.h"
#include "DocxFormat/External/External.h"
#include "DocxFormat/Media/Media.h"

namespace PPTX
{
	Slide::Slide()
	{
	}

	Slide::Slide(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	Slide::~Slide()
	{
	}

	void Slide::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);
		const XML::XDocument document(filename, true);

		show = document.Root.attribute("show").value();
		showMasterPhAnim = document.Root.attribute("showMasterPhAnim").value();
		showMasterSp = document.Root.attribute("showMasterSp").value();

		cSld = document.Root.element("cSld");
		if(cSld.is_init())
			cSld->SetParentFilePointer(*this);

		clrMapOvr = document.Root.element("clrMapOvr");
		if(clrMapOvr.is_init())
			clrMapOvr->SetParentFilePointer(*this);

		transition = document.Root.element("transition");
		if(transition.is_init())
			transition->SetParentFilePointer(*this);

		timing = document.Root.element("timing");
		if(timing.is_init())
			timing->SetParentFilePointer(*this);
	}

	void Slide::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.p + "sld",
			XML::XNamespace(ns.a) +
			XML::XNamespace(ns.r) +
			XML::XAttribute("show", show) +
			XML::XAttribute("showMasterPhAnim", showMasterPhAnim) +
			XML::XAttribute("showMasterSp", showMasterSp) +
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

	const OOX::FileType Slide::type() const
	{
		return OOX::FileTypes::Slide;
	}

	const boost::filesystem::wpath Slide::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath Slide::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	void Slide::FillShapeProperties(Logic::ShapeProperties& props, const std::string& type)const
	{
		if(Layout != NULL)
			Layout->FillShapeProperties(props, type);
	}

	void Slide::GetBackground(Logic::BgPr& bg, DWORD& ARGB)const
	{
		if(cSld->bg.is_init())
		{
			if(cSld->bg->bgPr.is_init())
				bg = cSld->bg->bgPr.get();
			else if(cSld->bg->bgRef.is_init())
			{
				ARGB = cSld->bg->bgRef->Color.GetARGB();
				Theme->themeElements->fmtScheme->GetFillStyle(cSld->bg->bgRef->idx.get_value_or(0), bg.Fill);
			}
		}
		else//from slideLayout
		{
			if(Layout != NULL)
				Layout->GetBackground(bg, ARGB);
		}
	}

	std::wstring Slide::GetMediaFullPathNameFromRId(const OOX::RId& rid) const
	{
//			return relsTable.Media.GetFullFilePathById(rid);//Media.GetFileName(rid);
		return FileContainer::image(rid).filename().string();
	}

	std::wstring Slide::GetLinkFromRId(const OOX::RId& rid)const
	{
//			return relsTable.Links.GetTargetById(rid);
		if(boost::shared_dynamic_cast<OOX::External, OOX::File>(FileContainer::operator [](rid)) != 0)
			return boost::shared_dynamic_cast<OOX::External, OOX::File>(FileContainer::operator [](rid))->Uri().string();
		else if(boost::shared_dynamic_cast<OOX::Media, OOX::File>(FileContainer::operator [](rid)) != 0)
			return boost::shared_dynamic_cast<OOX::Media, OOX::File>(FileContainer::operator [](rid))->filename().string();
		return L"";
	}

	void Slide::ApplyRels()
	{
		Layout = boost::shared_dynamic_cast<PPTX::SlideLayout, OOX::File>(FileContainer::get(OOX::FileTypes::SlideLayout));
		Master = Layout->Master;
		Theme = Layout->Theme;
		TableStyles = boost::shared_dynamic_cast<PPTX::TableStyles, OOX::File>(Theme->Presentation->get(OOX::FileTypes::TableStyles));

		if(exist(OOX::FileTypes::VmlDrawing))
		{
			Vml = boost::shared_dynamic_cast<PPTX::VmlDrawing, OOX::File>(FileContainer::get(OOX::FileTypes::VmlDrawing));
			Vml->FillRIds();
		}
	}

	const boost::filesystem::wpath Slide::GetPathBySpid(const std::string& spid)const
	{
		boost::filesystem::wpath filename = L"";
		if((Vml != 0) && (spid != ""))
		{
			if(Vml->SpIds.find(spid) != Vml->SpIds.end())
				filename = Vml->SpIds.find(spid)->second;
		}
		return filename;
	}

	DWORD Slide::GetRGBAFromMap(const std::string& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Layout->GetRGBAFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Layout->GetRGBAFromMap(str);
		return Master->GetRGBAFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}

	DWORD Slide::GetARGBFromMap(const std::string& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Layout->GetARGBFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Layout->GetARGBFromMap(str);
		return Master->GetARGBFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}

	DWORD Slide::GetBGRAFromMap(const std::string& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Layout->GetBGRAFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Layout->GetBGRAFromMap(str);
		return Master->GetBGRAFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}

	DWORD Slide::GetABGRFromMap(const std::string& str)const
	{
		if(!(clrMapOvr.is_init()))
			return Layout->GetABGRFromMap(str);
		if(!(clrMapOvr->overrideClrMapping.is_init()))
			return Layout->GetABGRFromMap(str);
		return Master->GetABGRFromScheme(clrMapOvr->overrideClrMapping->GetColorSchemeIndex(str));
	}
} // namespace PPTX