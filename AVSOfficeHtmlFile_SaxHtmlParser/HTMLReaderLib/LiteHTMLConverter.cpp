#include "stdafx.h"
#include "LiteHTMLConverter.h"


CLiteHTMLConverter::CTagsInfo CLiteHTMLConverter::m_mTagsInfo(200 /* block size */);

HRESULT CLiteHTMLConverter::ToXHTML (HANDLE hHtmlFile, IXHTMLWriter *pWriter, int aDefaultCodePage)
{
		// both input and output files are opened
		HRESULT hRes = S_OK;
		
		// parser
		CLiteHTMLReader oReader;
		oReader.setEventHandler (this);

		// Store writer
		pXHTMLWriter = pWriter;
		

		// Start parsing
		UINT uiOpened = oReader.ReadFile2 (hHtmlFile, aDefaultCodePage);
		return hRes;
}

HRESULT CLiteHTMLConverter::ToXHTML (CStringW& aBuffer, IXHTMLWriter *pWriter)
{
		// both input and output files are opened
		HRESULT hRes = S_OK;
		
		// parser
		CLiteHTMLReader oReader;
		oReader.setEventHandler (this);

		// Store writer
		pXHTMLWriter = pWriter;
		
		// Start parsing
		CLiteHTMLReader::DeleteTags (aBuffer, _T("SCRIPT"));
		CLiteHTMLReader::DeleteTags (aBuffer, _T("script"));
		CLiteHTMLReader::DeleteComments (aBuffer);

		UINT uiOpened = oReader.Read (aBuffer);
		return hRes;
}
