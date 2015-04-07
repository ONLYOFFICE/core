#include "stdafx.h"
#include "TxtFile.h"

#include "../TxtXmlFormatLib/Source/TxtXmlFile.h"

#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"


CTxtFile::CTxtFile()
{
}


HRESULT CTxtFile::FinalConstruct()
{
	return S_OK;
}


void CTxtFile::FinalRelease()
{
}


bool CTxtFile::Progress(long ID, long Percent)
{
	SHORT res = 0;
	m_lPercent = Percent;
	OnProgressEx(ID, Percent, &res);
	return (res != 0);
}


STDMETHODIMP CTxtFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	CTxtXmlFile file;
	//As Text
	try
	{
		file.txt_LoadFromFile(sSrcFileName, sDstPath, sXMLOptions);
		Progress(0, 1000000);
	}
	catch(...)
	{
		return S_FALSE;
	}

	return S_OK;
}


STDMETHODIMP CTxtFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	try
	{
		CTxtXmlFile file;

		file.txt_SaveToFile(sDstFileName, sSrcPath, sXMLOptions);
		
		Progress(0, 1000000);
	}
	catch(...)
	{
		return S_FALSE;
	}

  return S_OK;
}
