#include "./../Common/RSA/XMLDecoder.h"
#include "./../Common/OfficeDrawing/file.h"

IUnknown* setBlowfishKey(BYTE* pEncodedKey, LONG nEncodedKeySize)
{	
	MediaCore::IAVSMediaData *pData = NULL;
	CoCreateInstance(MediaCore::CLSID_CAVSMediaData, NULL, CLSCTX_ALL, MediaCore::IID_IAVSMediaData, (void**)&pData);
	if (NULL == pData)
	{
		delete []pEncodedKey;
		return NULL;
	}

	pData->AllocateBuffer(nEncodedKeySize);
	LPBYTE pBuffer = NULL;
	pData->get_Buffer(&pBuffer);
	memcpy(pBuffer, pEncodedKey, nEncodedKeySize);
	delete []pEncodedKey;

	IUnknown* punkRes = NULL;
	pData->QueryInterface(IID_IUnknown, (void**)&punkRes);

	RELEASEINTERFACE(pData);
	return punkRes;
}


BYTE* extractFile(const int id, int& size_of_file)
{
	HRSRC hRes = 0;
	HGLOBAL hData = 0;
	void* pData;	
	HMODULE hModule = _AtlBaseModule.GetModuleInstance();
		
	hRes = FindResource(hModule, MAKEINTRESOURCE(id), L"CRPT");
	
	if(hRes == NULL)
		return NULL;
	
	DWORD dwSize = SizeofResource(hModule, hRes);
	
	if(dwSize == NULL)
		return NULL;
	
	hData = LoadResource(hModule, hRes);
	
	if(hData == NULL)
		return false;
	
	pData = LockResource(hData);
	
	if(pData == NULL)
		return false;

	BYTE* file = new BYTE[size_of_file = dwSize];
	memcpy(file, (BYTE*)pData, size_of_file);

	return file;
}


BSTR readFileFromRresource(const int id)
{
	int sizeOfFile;
	BSTR textFile;
	BYTE* file = extractFile(id, sizeOfFile);
	if (file != NULL)
	{	
		LONG l = 0;
		for(int i = 0; i < 4; i++)
		{		
			LONG li = file[3 - i];
			l = l * 16 + li;
		}
		LONG l1 = l/2;
		LONG l2 = l - l1;
		LONG lenText = sizeOfFile - 4 - l;

		BYTE* keyCrypt = new BYTE[l];
		BYTE* textCrypt = new BYTE[lenText];

		memcpy(keyCrypt, file + 4, l1);
		memcpy(textCrypt, file + l1 + 4, lenText);
		memcpy(keyCrypt + l1, file + sizeOfFile - l2, l2);

		IUnknown* key = setBlowfishKey(keyCrypt, l);
		CString strA((CHAR*)textCrypt, lenText);	

		CXMLDecoder decoder;
		decoder.SetBlowfishKey(key);
		BSTR bsXml = strA.AllocSysString();
		CString text = decoder.DecryptXML(bsXml);
		SysFreeString(bsXml);

		CStringW textW;
		textW = text;
		textFile = textW.AllocSysString();
		

		key->Release();

		delete []file;
		delete []textCrypt;	
	}
	
	return textFile;
}