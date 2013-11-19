#pragma once

#include <atlenc.h>
#include <atlcoll.h>
#include "Base64.h"

#include "RSA\XmlEncoder.h"

class CHtmlUtility
{
public:

	static CString GetTempName()
	{
		TCHAR* td = new TCHAR[4096];
		TCHAR* tp = new TCHAR[4096];

		CString res = L"";

		if (td && tp)
		{
			if (GetTempPath(4096, td) > 0)
			{
				if (GetTempFileName(td, L"frex", 0, tp) > 0)
					res = tp;
			}
		}

		if (td)
			delete[] td;
		if (tp)
			delete[] tp;

		return res;
	}


	static BOOL GetEncoderClsid(CString strFormat, CLSID* pClsid)
	{
		// variables
		UINT nEncoders = 0;
		UINT nSize = 0;
		Gdiplus::ImageCodecInfo* pImageCodecInfo = 0;

		// retrieve encoders info
		Gdiplus::GetImageEncodersSize(&nEncoders, &nSize);

		// check for valid encoders
		if (!nSize)
			return FALSE;

		// create encoders info structure of necessary size
		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(nSize));

		// check for valid encoder
		if (!pImageCodecInfo)
			return FALSE;

		// retrieve all encoders
		GetImageEncoders(nEncoders, nSize, pImageCodecInfo);

		// locate necessary encoder
		for (UINT nEncoder = 0; nEncoder < nEncoders; ++nEncoder)
		{
			CString strMime(pImageCodecInfo[nEncoder].MimeType);

			// compare MIME strings
			if (strMime == strFormat)
			{
				// save CLSID
				*pClsid = pImageCodecInfo[nEncoder].Clsid;

				// clear memory
				free(pImageCodecInfo);

				// all ok
				return TRUE;
			}    
		}

		// clear memory
		free(pImageCodecInfo);

		// codec not found
		return FALSE;
	}


	static BOOL SaveAsJpeg(Gdiplus::Bitmap* pBitmap, CString strPath, ULONG nQuality)
	{
		if (!pBitmap)
			return FALSE;

		// variables
		CLSID encoder;
		
		// locate encoder CLSID
		if (!GetEncoderClsid(L"image/jpeg", &encoder))
			return FALSE;

		// create encoder settings structure
		Gdiplus::EncoderParameters settings;

		// setup encoder settings
		settings.Count = 1;
		settings.Parameter[0].Guid = Gdiplus::EncoderQuality;
		settings.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
		settings.Parameter[0].NumberOfValues = 1;

		// setup quality setting
		settings.Parameter[0].Value = &nQuality;

		// variables
		BSTR bstr = strPath.AllocSysString();

		// save image
		pBitmap->Save(bstr, &encoder, &settings);

		// clear string memory
		SysFreeString(bstr);

		// all ok
		return (pBitmap->GetLastStatus() == Gdiplus::Ok);
	}


	static BOOL SaveAsPng(Gdiplus::Bitmap* pBitmap, CString strPath)
	{
		if (!pBitmap)
			return FALSE;

		// variables
		CLSID encoder;
		
		// locate encoder CLSID
		if (!GetEncoderClsid(L"image/png", &encoder))
			return FALSE;

		// create encoder settings structure
		Gdiplus::EncoderParameters settings;

		// setup encoder settings
		settings.Count = 0;

		// variables
		BSTR bstr = strPath.AllocSysString();

		// save image
		pBitmap->Save(bstr, &encoder, &settings);

		// clear string memory
		SysFreeString(bstr);

		// all ok
		return (pBitmap->GetLastStatus() == Gdiplus::Ok);
	}


	static BOOL BinaryToBase64(const BYTE* data, int size, CString& result)
	{
		if (!data || !size)
			return FALSE;

		// dest size
		int nSize = Base64EncodeGetRequiredLength(size);

		// dest data
		char* strData = new char[nSize + 1];

		// check for valid output array
		if (!strData)
			return FALSE;

		// encode
		BOOL success = Base64Encode(data, size, strData, &nSize);

		// check for valid encoding
		if (success)
		{
			// fill the last symbol with zero value
			strData[nSize] = '\0';

			// make output
			result = strData;
		}

		// clear memory
		delete[] strData;

		// all ok
		return success;
	}

	static BOOL Base64ToBinary(const CString& string, BYTE*& data, int& size)
	{
		int nStrLen = string.GetLength();

		if (/*data || */!nStrLen)
			return FALSE;

		// dest size
		size = Base64DecodeGetRequiredLength(nStrLen);

		// dest data
		data = new BYTE[size + 2];

		// check for valid data
		if (!data)
			return FALSE;

		// decode
		//char* strData = (char*)(LPCWSTR)(string);
		CStringA cstr;
		cstr = string;
		char* strData = (char*)(LPCSTR)(cstr);

		// decode string
		BOOL success = Base64Decode(strData, nStrLen, data, &size);

		// check for valid result
		if (!success)
			delete[] data;
		else
		{
			data[size + 0] = 0;
			data[size + 1] = 0;
		}

		// all ok
		return success;
	}


	static BOOL GetFileData(CString path, BYTE*& data, int& size)
	{
		if (path.GetLength() < 1) 
			return FALSE;

		if (data)
			delete[] data;
	
		data = 0;
		size = 0;

		FILE* file = 0;
		BOOL success = TRUE;

		try
		{
			CStringA strA;
			strA = path;
			file = fopen(strA, "rb");
			fseek(file, 0, SEEK_END);
			size = (int)ftell(file);
			
			if (size > 0)
			{
				fseek(file, 0, SEEK_SET);
				data = new BYTE[size];
				fread(data, sizeof(BYTE), size, file);
			}
		}
		catch (...)
		{
			success = FALSE;
		}

		if (file)
			fclose(file);

		return success;
	}


	static BOOL SaveFile(const BYTE* bytes, const int size, const CString path)
	{
		CStringA strA;
		strA = path;

		FILE* file = 0;
		file = fopen(strA, "wb");

		fwrite(bytes, sizeof(BYTE), size, file);

		if (file)
			fclose(file);

		return TRUE;
	}


	static CString GetFileInBase64(CString path)
	{
		BYTE* bytes = 0;
		int size = 0;

		if (!GetFileData(path, bytes, size))
			return L"";
		if (!bytes || !size)
			return L"";

		CString value = L"";

		BOOL success = BinaryToBase64(bytes, size, value);

		if (!success)
			value = L"";

		if (bytes)
			delete[] bytes;

		return value;
	}


	static BOOL GetFileInBinary(CString valueInBase64, BYTE*& data, int& size)
	{			

		BOOL success = Base64ToBinary(valueInBase64, data, size);

		return success;

		//CBase64 oBase64;

		//USES_CONVERSION;
		//oBase64.Decode( (BYTE*)W2A(valueInBase64.GetBuffer()), valueInBase64.GetLength() );
		//size = oBase64.GetSize();

		//if ( data )
		//	delete []data;
		//data = new BYTE[size];
		//::memcpy( data, oBase64.GetBuffer(), sizeof(BYTE)*size );

		//return TRUE;
	}
};
