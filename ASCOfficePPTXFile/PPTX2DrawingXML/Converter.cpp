#include "stdafx.h"
#include "Converter.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX2DrawingXML
{
	Converter::Converter()
	{
	}

	Converter::~Converter()
	{
	}

	DWORD Converter::Convert(const PPTX::Folder& pFolder, CStringW& xml)
	{
		//m_Folder = pFolder;
		CDrawingDocument lDocument;
		double dDuration = 0.0;

		boost::shared_ptr<PPTX::Presentation> presentation = boost::shared_dynamic_cast<PPTX::Presentation, OOX::File>(pFolder.get(OOX::FileTypes::Presentation));
		int cx = presentation->sldSz->cx;
		int cy = presentation->sldSz->cy;
		DWORD sld = 0;
		for(std::list<PPTX::Logic::XmlId>::const_iterator iter = presentation->sldIdLst->begin();
			iter != presentation->sldIdLst->end(); iter++)
		{
			SlideConvert(boost::shared_dynamic_cast<PPTX::Slide, OOX::File>((*presentation)[iter->rid.get()]), sld, &dDuration, cx, cy, lDocument);
			sld++;
		}

		lDocument.NormalizeTransitionTime();
		lDocument.ToXmlSource3();
		xml = lDocument.m_strXmlVideoSource;
		DWORD Result = lDocument.m_strXmlVideoSource.GetLength();

		for(size_t i = 0; i < lDocument.m_arPages.GetCount(); i++)
		{
			for(int j = 0; j < lDocument.m_arPages[i]->m_arElements.GetSize(); j++)
				delete lDocument.m_arPages[i]->m_arElements[j];
			lDocument.m_arPages[i]->m_arElements.RemoveAll();
			delete lDocument.m_arPages[i];
		}
		lDocument.m_arPages.RemoveAll();

		return Result;
	}
} // namespace PPTX2DrawingXML