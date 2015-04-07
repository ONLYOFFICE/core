#include "stdafx.h"
#include "XmlFile.h"

#include "../TxtXmlFormatLib/Source/TxtXmlFile.h"

#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

CXmlFile::CXmlFile()
{
}


HRESULT CXmlFile::FinalConstruct()
{
	return S_OK;
}


void CXmlFile::FinalRelease()
{
}


bool CXmlFile::Progress(long ID, long Percent)
{
	SHORT res = 0;
	m_lPercent = Percent;
	OnProgressEx(ID, Percent, &res);
	return (res != 0);
}


STDMETHODIMP CXmlFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	CTxtXmlFile file;

	file.txt_LoadFromFile(sSrcFileName, sDstPath, sXMLOptions);

	return S_OK;
}


STDMETHODIMP CXmlFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
  return S_OK;
}

//const unsigned long CXmlFile::LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const
//{
//	HMODULE hMod = GetModuleHandle(L"ASCOfficeTxtFile.dll");
//	if (hMod)
//	{
//		HRSRC hRes = FindResource(hMod, lpResName, lpResType);
//		if (hRes)
//		{
//			HGLOBAL hGlob = LoadResource(hMod, hRes);
//			if (hGlob)
//			{
//				BYTE *lpbArray = (BYTE*)LockResource(hGlob);
//				if (lpbArray)
//				{
//					const DWORD dwFileSize = SizeofResource(hMod, hRes);
//					if (dwFileSize != 0)
//					{
//						int hFile = 0;
//						if (!_wsopen_s(&hFile, fileName, (O_BINARY | O_CREAT | _O_RDWR ), _SH_DENYNO, S_IWRITE))
//						{
//							_write(hFile, lpbArray, dwFileSize);
//							_close(hFile);
//						}
//					}
//				}
//			}
//		}
//	}
//	return GetLastError();
//}