#include "stdafx.h"
#include "Converter.h"

namespace Odp2DrawingXML
{
	Converter::Converter()
	{
	}

	Converter::~Converter()
	{
	}

	DWORD Converter::Convert(const Odp::Folder& pFolder, CStringW& xml)
	{
		CDrawingDocument lDocument;
		m_Folder = &pFolder;

		double dDuration = 0.0;
		size_t i;

		for(i = 0; i < pFolder.Content->body->Slides->size(); i++)
		{
			SlideConvert(i, &dDuration, lDocument);
			TransitionConvert(i, lDocument);
		}

		lDocument.ToXmlSource3();
		xml = lDocument.m_strXmlVideoSource;
		DWORD Result = lDocument.m_strXmlVideoSource.GetLength();

		for(i = 0; i < lDocument.m_arPages.GetCount(); i++)
		{
			for(int j = 0; j < lDocument.m_arPages[i]->m_arElements.GetSize(); j++)
				delete lDocument.m_arPages[i]->m_arElements[j];
			lDocument.m_arPages[i]->m_arElements.RemoveAll();
			delete lDocument.m_arPages[i];
		}
		lDocument.m_arPages.RemoveAll();

		return Result;
	}
} // namespace Odp2DrawingXML