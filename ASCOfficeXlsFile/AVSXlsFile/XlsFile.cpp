// XlsFile.cpp : Implementation of CXlsFile

#include "stdafx.h"
#include "XlsFile.h"

#include <EWSDocument.h>

// CXlsFile

STDMETHODIMP CXlsFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	EWSCORE::EWSDocument doc;
	bool res = doc.loadFrom(sSrcFileName, ef_XLS) &&
				doc.saveTo(sDstPath, ef_XLSX);

	return res ? S_OK : S_FALSE;
}


STDMETHODIMP CXlsFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	EWSCORE::EWSDocument doc;
	bool res = doc.loadFrom(sSrcPath, ef_XLSX) &&
		doc.saveTo(sDstFileName, ef_XLS);

	return res ? S_OK : S_FALSE;
}



