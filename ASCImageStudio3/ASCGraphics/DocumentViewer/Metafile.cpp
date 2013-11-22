#pragma once
#include "stdafx.h"
#include "ASCMetafile.h"
#include "..\Interfaces\ImageSerializeObjects2.h"

void CMetafile::GenerateMetafile(BSTR bsXML)
{
	XmlUtils::CXmlNode oNodeWatermark;
	if (oNodeWatermark.FromXmlString((CString)bsXML))
	{
		ImageStudio::Serialize::Paint::Common::CTextMeasurer oMeasurer;

		LONG lMode = 0;
		oMeasurer.InternalFromNonFoXml2(oNodeWatermark, lMode, m_fWidth, m_fHeight);
		
		CoCreateInstance(__uuidof(CAVSMetafile), NULL, CLSCTX_ALL, __uuidof(IAVSMetafile), (void**)&m_pAdditionalCommands);
		m_pAdditionalCommands->put_Width(m_fWidth);
		m_pAdditionalCommands->put_Height(m_fHeight);

		IAVSRenderer* pRenderer = NULL;
		m_pAdditionalCommands->QueryInterface(IID_IUnknown, (void**)&pRenderer);
		
		if (!oMeasurer.IsFormated())
		{
			oMeasurer.InitDrawText(pRenderer, 720, 576);
		}

		oMeasurer.DrawText(pRenderer, 1.0);	

		RELEASEINTERFACE(pRenderer);
	}
}