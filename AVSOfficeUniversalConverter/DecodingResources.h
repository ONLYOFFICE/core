#include "./../Common/RSA/XMLDecoder.h"
#include "./../Common/OfficeDrawing/file.h"
#include <string>


template< class InIt, class OutIt > inline
void utf8_decode( InIt in, const InIt end, OutIt out )
{
    int cnt;
    for(; !(in == end); ++in) {
        unsigned long wc = static_cast<unsigned char>(*in);
    over:
        if( wc & 0x80 ) {
            if( 0xC0 == (0xE0 & wc) ) { cnt = 1; wc &= ~0xE0; } else
            if( 0xE0 == (0xF0 & wc) ) { cnt = 2; wc &= ~0xF0; } else
            if( 0xF0 == (0xF8 & wc) ) { cnt = 3; wc &= ~0xF8; } else
            if( 0xF8 == (0xFC & wc) ) { cnt = 4; wc &= ~0xFC; } else
            if( 0xFC == (0xFE & wc) ) { cnt = 5; wc &= ~0xFE; } else
                { *out = wchar_t('?'); ++out; continue; };//invalid start code
            if( 0 == wc ) wc = ~0UL;//codepoint encoded with overlong sequence
            do {
                if( ++in == end ) return;
                unsigned char c = static_cast<unsigned char>(*in);
                if( 0x80 != (0xC0 & c) )
                    { *out = static_cast<wchar_t>(wc); ++out; wc = c; goto over; }
                wc <<= 6; wc |= c & ~0xC0;
            } while( --cnt );
            if( 0x80000000 & wc ) wc = '?';//codepoint exceeds unicode range
            if( sizeof(wchar_t) == 2 && wc > 0xFFFF )
            {//handle surrogates for UTF-16
                wc -= 0x10000;
                *out = static_cast<wchar_t>(0xD800 | ((wc >> 10) & 0x3FF)); ++out;
                *out = static_cast<wchar_t>(0xDC00 | (wc & 0x3FF)); ++out;
                continue;
            }
        }
        *out = static_cast<wchar_t>(wc); ++out;
    }
}


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
		BYTE* textCrypt = new BYTE[lenText + 1];

		memcpy(keyCrypt, file + 4, l1);
		memcpy(textCrypt, file + l1 + 4, lenText);
		memcpy(keyCrypt + l1, file + sizeOfFile - l2, l2);
		textCrypt[lenText] = 0;

		IUnknown* key = setBlowfishKey(keyCrypt, l);		

		CXMLDecoder decoder;
		decoder.SetBlowfishKey(key);
		decoder.DecryptXML((BYTE**)&textCrypt, (DWORD*)&lenText);

		

		
		std::wstring text(lenText, 0);
		utf8_decode( textCrypt, textCrypt + lenText, text.begin() );
		CStringW textW;
		textW = text.c_str();
		textFile = textW.AllocSysString();
		

		key->Release();

		delete []file;
		delete []textCrypt;	
	}
	
	return textFile;
}
