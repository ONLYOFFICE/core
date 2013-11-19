#pragma once
#include "stdafx.h"

// класс для работы с реестром

struct TValueInfo
{
	CString sName;
	DWORD   nValueType;
	DWORD   nLength;
	BYTE    pData[256];


	void Set(CString sItemName, DWORD nItemType, DWORD nItemLength, BYTE *pItemData)
	{
		Clear();
		sName = sItemName;
		nValueType = nItemType;
		nLength = nItemLength;
		::memcpy(pData, pItemData, min(255, nLength));
	};

	void Clear(void)
	{
		sName = "";
		nValueType = 0;
		nLength = 0;
		for (int nIndex = 0; nIndex < 256; nIndex++ )
			pData[nIndex] = 0;
	};
};

class CRegistry
{

public:	
	
	HKEY                m_hKey;
	CString             m_sKeyRoot;
	CString             m_sSubKey;
	SECURITY_ATTRIBUTES m_oSecurity;

	
public:	

	CRegistry()
	{
		m_sKeyRoot  = "";
		m_sSubKey   = "";

		m_oSecurity.lpSecurityDescriptor = NULL;
	};

	~CRegistry()
	{
	};


	HKEY GetHKEY()
	{
		return m_hKey;
	};
	void SetHKEY(HKEY oValue)
	{
		m_hKey = oValue;
	};	
	CString GetKeyRoot()
	{
		return m_sKeyRoot;
	};
	void SetKeyRoot(CString sValue)
	{
		m_sKeyRoot = sValue;
	};		
	CString GetSubkey()
	{
		return m_sSubKey;
	};
	void SetSubkey(CString sValue)
	{
		m_sSubKey = sValue;
	};

	BOOL KeyExist()
	{
		HKEY oHandle;
		if ( !RegOpenKeyEx(m_hKey, m_sKeyRoot + "\\" + m_sSubKey, 0, KEY_READ, &oHandle) )
			return TRUE;
		else 
			return FALSE;
	};

	BOOL CreateKey(CString sKeyName)
	{
		HKEY oHandle;
		DWORD nDisp;
		int nResult = 0;
		if ( !RegCreateKeyEx(m_hKey, m_sKeyRoot + "\\" + sKeyName, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &m_oSecurity, &oHandle, &nDisp ) )
		{
			RegCloseKey(oHandle);
			return TRUE;
		}
		else 
			return FALSE;

	};
	BOOL DeleteKey(CString sKeyName)
	{
		if ( !RegDeleteKey(m_hKey, m_sKeyRoot + "\\" + sKeyName) )
			return TRUE;
		else 
			return FALSE;

	};

	BOOL DeleteValue(CString sValueName)
	{
		HKEY oHandle;

		if ( RegOpenKeyEx(m_hKey, m_sKeyRoot + "\\" + m_sSubKey, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &oHandle) )
			return FALSE;

		RegDeleteValue(oHandle, sValueName);
		RegCloseKey(oHandle);

		return TRUE;
	};

	CSimpleArray<CString> EnumRegistryKeys(HKEY oHKey, CString sKeyName)
	{

		FILETIME fFileTime;
		HKEY oHandle;
		CSimpleArray<CString> arNull;
		CSimpleArray<CString> arResult; 

		if ( sKeyName.GetLength() > 0 )
		{
			if ( RegOpenKeyEx(oHKey, sKeyName, 0, KEY_READ, &oHandle) )
				return arNull;
		}


		for ( int nIndex = 0; ;nIndex++ )
		{
			DWORD nLenght = 260;
			LPSTR pItemName = new char[nLenght];

			long lRes = RegEnumKeyEx(oHandle, nIndex, pItemName, &nLenght, 0, NULL, NULL, &fFileTime );

			if ( ERROR_NO_MORE_ITEMS == lRes )
			{
				if ( pItemName )
					delete []pItemName; 
				break;
			}
			else if (  ERROR_SUCCESS == lRes )
				arResult.Add(CString(pItemName));
			else
			{
				if ( pItemName )
					delete []pItemName; 

				if ( oHandle )
					RegCloseKey(oHandle);

				return arNull;
			}

			if ( pItemName )
				delete []pItemName; 

		}


		if ( oHandle )
			RegCloseKey(oHandle);

		return arResult;

	
	};


	CSimpleArray<TValueInfo> EnumRegistryValues(HKEY oHKey, CString sKeyName) 
	{
		HKEY oHandle;

		CSimpleArray<TValueInfo> arNull; 
		CSimpleArray<TValueInfo> arResult; 

		DWORD nValueType = 0;

		if ( sKeyName.GetLength() > 0 )
			if ( RegOpenKeyEx(oHKey, sKeyName, 0, KEY_READ, &oHandle) )
				return arNull;

		for ( int nIndex = 0; ; nIndex++)
		{
			TValueInfo oItem;
			
			DWORD nNameLenght = 260;
			LPSTR pItemName = new char[nNameLenght];

			DWORD nValueLength = 4096;

			BYTE* pResBinary = new BYTE[nValueLength - 1];

			long lRes = RegEnumValue(oHandle, nIndex, pItemName, &nNameLenght, 0, &nValueType, pResBinary, &nValueLength);

			if ( lRes )
			{
				if ( pResBinary )
					delete []pResBinary;

				if ( pItemName )
					delete []pItemName; 

				if ( ERROR_NO_MORE_ITEMS == lRes )
					break;

				if ( oHandle )
					RegCloseKey(oHandle);
				
				return arNull;
			}

			oItem.sName      = CString(pItemName);
			oItem.nValueType = nValueType;
			oItem.nLength    = nValueLength;
			::memcpy(oItem.pData, pResBinary, min(255, oItem.nLength));

			arResult.Add(oItem);

			if ( pResBinary )
				delete []pResBinary;

			if ( pItemName )
				delete []pItemName; 
		}
		if ( oHandle )
			RegCloseKey(oHandle);

		return arResult;
	};


	TValueInfo GetRegistryValue(CString sValueName)
	{
		HKEY oHandle;
		DWORD nValueType = 0;

		TValueInfo oItem;
		oItem.Clear();

		if ( RegOpenKeyEx(m_hKey, m_sKeyRoot + "\\" + m_sSubKey, REG_OPTION_NON_VOLATILE, KEY_READ, &oHandle) )
			return oItem;
	
		DWORD nLength = 1024;
		BYTE *pBinary = new BYTE[nLength - 1];

		long lRes = RegQueryValueEx(oHandle, sValueName, 0, &nValueType, pBinary, &nLength);

		if ( lRes )
		{
			if ( oHandle )
				RegCloseKey(oHandle);
			return oItem;
		}

		oItem.sName      = sValueName;
		oItem.nLength    = nLength;
		oItem.nValueType = nValueType;
		::memcpy(oItem.pData, pBinary, nLength);

		RegCloseKey(oHandle);

		return oItem;
	};

	BOOL SetRegistryValue(TValueInfo oValue)
	{
		HKEY oHandle;
		if ( RegOpenKeyEx(m_hKey, m_sKeyRoot + "\\" + m_sSubKey, REG_OPTION_NON_VOLATILE, KEY_WRITE, &oHandle) )
			return FALSE;

		long lRes = RegSetValueEx(oHandle, oValue.sName, 0, oValue.nValueType, oValue.pData, oValue.nLength);	

		RegCloseKey(oHandle);

		if ( lRes )
			return FALSE;
		else 
			return TRUE;
	};

	BOOL SetRegistryValue(CString sItemName, DWORD nItemType, DWORD nItemLength, BYTE *pItemData)
	{
		TValueInfo oItem;
		oItem.Set(sItemName, nItemType, nItemLength, pItemData);
		return SetRegistryValue(oItem);
	};


};