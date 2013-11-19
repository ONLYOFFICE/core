#pragma once
#include "../stdafx.h"
#include "../PPTXFormat/PPTX.h"

#include "SlideConvert.h"

#ifdef _DEBUG
#define __AVS_PRESENTATION_EXTRACT__	1
#else
#define __AVS_PRESENTATION_EXTRACT__	0
#endif


namespace PPTX2EditorAdvanced
{
	DWORD Convert(PPTX::Folder& oFolder, NSPresentationEditor::CDocument& lDocument, CString& strDir)
	{
		CPresentationInfo oPresentation;
		oPresentation.m_strDirectory = strDir;

		double dDuration = 0.0;

		smart_ptr<PPTX::Presentation> presentation = oFolder.get(OOX::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
		int cx = presentation->sldSz->cx;
		int cy = presentation->sldSz->cy;

		lDocument.m_oInfo.m_lUnitsHor = cx;
		lDocument.m_oInfo.m_lUnitsVer = cy;
		lDocument.m_oInfo.m_lMillimetresHor	= cx/36000;
		lDocument.m_oInfo.m_lMillimetresVer	= cy/36000;

		if (/*__AVS_PRESENTATION_EXTRACT__*/true)
		{
			// записываем все темы
			size_t nCountMasters = presentation->sldMasterIdLst.GetCount();
			for (size_t iTheme = 0; iTheme < nCountMasters; ++iTheme)
			{
				smart_ptr<PPTX::SlideMaster> slideMaster = ((*presentation)[presentation->sldMasterIdLst[iTheme].rid.get()]).smart_dynamic_cast<PPTX::SlideMaster>();
				ThemeConvert(slideMaster, oPresentation);

				size_t nCountLayouts = slideMaster->sldLayoutIdLst.GetCount();
				for (size_t iLayout = 0; iLayout < nCountLayouts; ++iLayout)
				{
					smart_ptr<PPTX::SlideLayout> slideLayout = ((*slideMaster)[slideMaster->sldLayoutIdLst[iLayout].rid.get()]).smart_dynamic_cast<PPTX::SlideLayout>();
					LayoutConvert(slideLayout, oPresentation);
				}
			}
		}


		DWORD sld = 0;

		size_t nCount = presentation->sldIdLst.GetCount();
		for (size_t i = 0; i < nCount; ++i)
		{
			smart_ptr<PPTX::Slide> slide = ((*presentation)[presentation->sldIdLst[i].rid.get()]).smart_dynamic_cast<PPTX::Slide>();
			SlideConvert(slide, sld, &dDuration, cx, cy, lDocument, oPresentation);
		}

		size_t nCountThemes = oPresentation.m_arThemes.GetCount();
		for (size_t i = 0; i < nCountThemes; ++i)
		{
			lDocument.m_arThemes.Add(oPresentation.m_arThemes[i]->m_oTheme);
		}

		lDocument.CalculateEditor(lDocument.m_oInfo);

		return 0;
	}
	DWORD ConvertToVS(PPTX::Folder& oFolder, CString& xml, CString& strDir)
	{
		NSPresentationEditor::CDocument oDocument;
		Convert(oFolder, oDocument, strDir);

		xml = oDocument.ToXmlVideoSource2();
		return (DWORD)xml.GetLength();
	}
	DWORD ConvertToEditor(PPTX::Folder& oFolder, CString& xml, CString& strDir)
	{
		NSPresentationEditor::CDocument oDocument;
		Convert(oFolder, oDocument, strDir);

		xml = oDocument.ToXmlEditor2();
		return (DWORD)xml.GetLength();
	}
}
