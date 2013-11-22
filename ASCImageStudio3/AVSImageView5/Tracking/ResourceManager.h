#pragma once

namespace Resources
{
	class CResources
	{
	protected:
	
		HINSTANCE m_hInstance;
		
	public:
		
		CResources()
		{
			m_hInstance = NULL;
		}
		BOOL Valid()
		{
			if (m_hInstance != NULL)
				return TRUE;

			return FALSE;
		}
	
		void Create(HINSTANCE hInstance = NULL)
		{
			m_hInstance = hInstance;
		}
	
		CString GetResourceXml(int nResourceID, CString strDef = "")
		{
			BYTE* pResData = NULL;
			int nResSize = 0;

			if (!GetResourceBinary(nResourceID, pResData, nResSize))
				return strDef;

			BYTE* pTemp = new BYTE[nResSize + 1];

			memcpy(pTemp, pResData, nResSize);

			pTemp[nResSize] = 0;

			CString strXml;

			strXml = (char*)pTemp;

			return strXml;
		}
		BOOL GetResourceBinary(int nResourceID, BYTE*& pData, int& nSize)
		{
			if (!Valid())
				return FALSE;

			HRSRC hResource = FindResource(m_hInstance, MAKEINTRESOURCE(nResourceID), TEXT("RESOURCE"));

			if (hResource == NULL)
				return FALSE;

			nSize = (int)SizeofResource(m_hInstance, hResource);

			HGLOBAL hResourceHandle = LoadResource(m_hInstance, hResource);

			pData = (BYTE*)LockResource(hResourceHandle);

			if (!pData || nSize < 1)
				return FALSE;

			return TRUE;
		}
	};
}
