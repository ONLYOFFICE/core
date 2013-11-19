// EncryptRes.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "OfficeDrawing\file.h"
#include "RSA\XMLEncoder.h"
#include "RSA\XMLDecoder.h"
#include <string>

const std::string getFileOutEncName(const std::string& filename)
{
	std::string fileOut = filename;

	std::string::size_type pos = fileOut.find_last_of(".");
	if (pos != std::string::npos)	
		fileOut = fileOut.substr(0, pos);
	fileOut += ".crpt";

	return fileOut;
}

const std::string getFileOutDecName(const std::string& filename)
{
	std::string fileOut = filename;

	std::string::size_type pos = fileOut.find_last_of(".");
	if (pos != std::string::npos)	
		fileOut = fileOut.substr(0, pos);
	fileOut += ".decrpt";

	return fileOut;
}


void encrypt(const std::string& filename)
{
	CFile file;
	if (S_OK == file.OpenFile(CString(filename.c_str())))
	{
		ULONG64 lenFile = file.GetFileSize();
		BYTE* fileText = new BYTE[lenFile + 1];
		file.ReadFile( fileText, lenFile);
		fileText[lenFile] = 0;

		CXMLEncoder encoder;

		encoder.EncryptXML((BYTE**)&fileText, (DWORD*)&lenFile );

		CFile outFile;
		CString fileOutName(CString(getFileOutEncName(filename).c_str()));
		if ( outFile.CreateFile(fileOutName) == S_OK)
		{
			IUnknown* key = encoder.GetBlowfishKey();
			MediaCore::IAVSMediaData* p = NULL;
			key->QueryInterface(MediaCore::IID_IAVSMediaData, (void**)&p);
			LONG l;
			BYTE* cryptKey;
			p->get_DataSize(&l);
			p->get_Buffer(&cryptKey);

			LONG l1 = l/2;
			LONG l2 = l - l1;
			int t = 2;

			
			outFile.WriteFile((void*)&l, 4);
			outFile.WriteFile((void*)cryptKey, l1);
			outFile.WriteFile((void*)fileText, lenFile);
			outFile.WriteFile((void*)(cryptKey + l1), l2);

			key->Release();
			outFile.CloseFile();
			int k = 5;
		}

		delete []fileText;			
		file.CloseFile();
	}
}

void decrypt(const std::string& filename)
{
	CFile file;
	if (S_OK == file.OpenFile(CString(filename.c_str())))
	{
		LONG l;
		file.ReadFile( (BYTE*)&l, 4 );
		LONG l1 = l/2;
		LONG l2 = l - l1;
		BYTE *cryptKey = new BYTE[l];
		BYTE *pBuf = new BYTE[l1];
		file.ReadFile( pBuf, l1 );
		::memcpy( cryptKey, pBuf, l1 );
		delete []pBuf;
		pBuf = new BYTE[l2];

		ULONG64 lenFile = file.GetFileSize() - 4 - l1 - l2;
		BYTE* fileText = new BYTE[lenFile + 1];
		file.ReadFile( fileText, lenFile);
		file.ReadFile( pBuf, l2 );
		::memcpy( cryptKey + l1, pBuf, l2 );
		delete []pBuf;
		fileText[lenFile] = 0;

		CXMLDecoder decoder;

		MediaCore::IAVSMediaData *pKey = NULL;
		CoCreateInstance(MediaCore::CLSID_CAVSMediaData, NULL, CLSCTX_ALL, MediaCore::IID_IAVSMediaData, (void**)&pKey);
		
		BYTE *pBuffer = NULL;
		pKey->AllocateBuffer(l);
		pKey->get_Buffer(&pBuffer);
		::memcpy( pBuffer, cryptKey, l );
		delete []cryptKey;

		IUnknown* punkRes = NULL;
		pKey->QueryInterface(IID_IUnknown, (void**)&punkRes);
		RELEASEINTERFACE( pKey );

		decoder.SetBlowfishKey(punkRes);

		decoder.DecryptXML((BYTE**)&fileText, (DWORD*)&lenFile );

		RELEASEINTERFACE( punkRes );

		CFile outFile;
		CString fileOutName(CString(getFileOutDecName(filename).c_str()));
		if ( outFile.CreateFile(fileOutName) == S_OK)
		{
			outFile.WriteFile((void*)fileText, lenFile);
			outFile.CloseFile();
		}

		delete []fileText;			
		file.CloseFile();
	}
}


int main(int argc, char *argv[])
{	
	CoInitialize(NULL);


	if (argc == 2)
	{	
			encrypt( argv[1] );
		//	decrypt( argv[1] );
	}

	CoUninitialize();
	return 0;
}