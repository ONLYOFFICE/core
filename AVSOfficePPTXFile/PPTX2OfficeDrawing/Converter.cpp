#include "stdafx.h"
#include "Converter.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX2OfficeDrawing
{
	Converter::Converter()
	{
	}

	Converter::~Converter()
	{
	}

	DWORD Converter::Convert(const PPTX::Folder& pFolder, NSPresentationEditor::CDocument& pDocument)
	{
		//TODO : не забыть сделать конвертацию бэкграундов`
		pDocument.ClearThemes();
		pDocument.ClearSlides();

		boost::shared_ptr<PPTX::Presentation> presentation = boost::shared_dynamic_cast<PPTX::Presentation, OOX::File>(pFolder.get(OOX::FileTypes::Presentation));
		int cx = presentation->sldSz->cx;
		int cy = presentation->sldSz->cy;
		for(std::list<PPTX::Logic::XmlId>::const_iterator iter = presentation->sldMasterIdLst->begin(); iter != presentation->sldMasterIdLst->end(); iter++)
		{
			MasterConvert(boost::shared_dynamic_cast<PPTX::SlideMaster, OOX::File>((*presentation)[iter->rid.get()]), pDocument, cx, cy);
		}

		for(std::list<PPTX::Logic::XmlId>::const_iterator iter = presentation->sldIdLst->begin(); iter != presentation->sldIdLst->end(); iter++)
		{
			CSlide lSlide;
			lSlide.m_lOriginalWidth = cx;
			lSlide.m_lOriginalHeight = cy;

			lSlide.m_lWidth = cx / 36000;
			lSlide.m_lHeight = cy / 36000;

			SlideConvert(boost::shared_dynamic_cast<PPTX::Slide, OOX::File>((*presentation)[iter->rid.get()]), lSlide);
			pDocument.AddSlide(lSlide);
		}

		return 0;
	}

} // namespace PPTX2OfficeDrawing