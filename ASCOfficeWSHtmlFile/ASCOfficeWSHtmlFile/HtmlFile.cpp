// HtmlFile.cpp : Implementation of CWSHtmlFile

#include "stdafx.h"
#include "HtmlFile.h"

#include <vector>
#include <map>
#include <deque>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <iosfwd>
#include <memory>
#include <functional>
#include "..\HtmlConverter\source\Logging\log.h"

#include <EWSDocument.h>

// CWSHtmlFile

STDMETHODIMP CWSHtmlFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
// 	EWSCORE::EWSDocument doc;
// 	bool res = doc.loadFrom(sSrcFileName, ef_HTML) &&
// 				doc.saveTo(sDstPath, ef_XLSX);
// 
// 	return res ? S_OK : S_FALSE;
	return S_FALSE;
}


STDMETHODIMP CWSHtmlFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	EWSCORE::EWSDocument doc;
	bool res = doc.loadFrom(sSrcPath, ef_XLSX) &&
		doc.saveTo(sDstFileName, ef_HTML, _bstr_t(sXMLOptions));

	return res ? S_OK : S_FALSE;
}



