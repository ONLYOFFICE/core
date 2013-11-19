#include "stdafx.h"
#include "./Converter.h"
#include "../../Common/DocxFormat/Source/DocxFormat/FileTypes.h"

namespace PPTX2EditorSimple
{
	Converter::Converter()
	{
	}

	Converter::~Converter()
	{
	}

	DWORD Converter::Convert(PPTX::Folder& oFolder, NSPresentationEditor::CDocument& lDocument)
	{
		//m_Folder = pFolder;
		double dDuration = 0.0;

		smart_ptr<PPTX::Presentation> presentation = oFolder.get(OOX::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
		int cx = presentation->sldSz->cx;
		int cy = presentation->sldSz->cy;
		DWORD sld = 0;

		size_t nCount = presentation->sldIdLst.GetCount();
		for (size_t i = 0; i < nCount; ++i)
		{
			smart_ptr<PPTX::Slide> slide = ((*presentation)[presentation->sldIdLst[i].rid.get()]).smart_dynamic_cast<PPTX::Slide>();
			SlideConvert(slide, sld, &dDuration, cx, cy, lDocument);
		}

		return 0;
	}
	DWORD Converter::ConvertToVS(PPTX::Folder& oFolder, CString& xml)
	{
		NSPresentationEditor::CDocument oDocument;
		Convert(oFolder, oDocument);

		oDocument.Calculate();
		xml = oDocument.ToXmlVideoSource();
		return (DWORD)xml.GetLength();
	}
	DWORD Converter::ConvertToEditor(PPTX::Folder& oFolder, CString& xml)
	{
		NSPresentationEditor::CDocument oDocument;
		Convert(oFolder, oDocument);

		oDocument.Calculate();
		xml = oDocument.ToXmlEditor();
		return (DWORD)xml.GetLength();
	}
} // namespace PPTX2DrawingXML