#pragma once

#include "../XML/xmlutils.h"
#include "../../../../DesktopEditor/common/File.h"


//по аналогии с /DesktopEditor/common/File.h только для CString
class CFile 
{
private:
	HRESULT _Open(const CString& strFileName, bool bOpen = false, bool bCreate = false, bool bReadWrite = false)
	{
		HRESULT hRes = S_OK;
		CloseFile();

#if defined(WIN32) || defined(_WIN32_WCE)
		wchar_t* pModeOpen;
		wchar_t* pModeCreate;
		if(bReadWrite)
		{
			pModeOpen = L"rb+";
			pModeCreate = L"wb+";
		}
		else
		{
			pModeOpen = L"rb";
			pModeCreate = L"wb";
		}
		if(NULL == m_pFile && bOpen)
			m_pFile = _wfopen(strFileName, pModeOpen);
		if(NULL == m_pFile && bCreate)
			m_pFile = _wfopen(strFileName, pModeCreate);
#else
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
		NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strFileName.c_str(), strFileName.length(), pUtf8, lLen, false);
		char* pModeOpen;
		char* pModeCreate;
		if(bReadWrite)
		{
			pModeOpen = "rb+";
			pModeCreate = "wb+";
		}
		else
		{
			pModeOpen = "rb";
			pModeCreate = "wb";
		}
		if(NULL == m_pFile && bOpen)
			m_pFile = fopen((char*)pUtf8, pModeOpen);
		if(NULL == m_pFile && bCreate)
			m_pFile = fopen((char*)pUtf8, pModeCreate);
		RELEASEARRAYOBJECTS(pUtf8);
#endif
		if (NULL == m_pFile)
			return S_FALSE;

		fseek(m_pFile, 0, SEEK_END);
		m_lFileSize = ftell(m_pFile);
		fseek(m_pFile, 0, SEEK_SET);

		m_lFilePosition = 0;

		if (0 < strFileName.GetLength())
		{
			if (((wchar_t)'/') == strFileName[strFileName.GetLength() - 1])
				m_lFileSize = 0x7FFFFFFF;
		}

		unsigned int err = 0x7FFFFFFF;
		unsigned int cur = (unsigned int)m_lFileSize;
		if (err == cur)
		{
			CloseFile();
			return S_FALSE;
		}

		return hRes;
	}
public:
	CFile() 
	{
		m_pFile = NULL;
		m_lFilePosition = 0;
		m_lFileSize = 0;
	}

	virtual ~CFile()
	{
		CloseFile();
	}
	
	HRESULT OpenOrCreate(CString strFileName, bool bOnlyOpen = false, bool bReadWrite = false)
	{
		return _Open(strFileName, true, true, true);
	}
	virtual HRESULT OpenFile(CString FileName)
	{	
		return _Open(FileName, true, false, false);
	}

	virtual HRESULT OpenFileRW(CString FileName)
	{	
		return _Open(FileName, true, false, true);
	}

	HRESULT ReadFile(BYTE* pData, DWORD nBytesToRead)
	{
		if (!m_pFile)
			return S_FALSE;

		SetPosition(m_lFilePosition);
		DWORD dwSizeRead = (DWORD)fread((void*)pData, 1, nBytesToRead, m_pFile);
		m_lFilePosition += dwSizeRead; 
		return S_OK;
	}

	HRESULT ReadFile2(BYTE* pData, DWORD nBytesToRead)
	{
		HRESULT hRes = ReadFile(pData, nBytesToRead);
		//reverse bytes
		for (size_t index = 0; index < nBytesToRead / 2; ++index)
		{
			BYTE temp = pData[index];
			pData[index] = pData[nBytesToRead - index - 1];
			pData[nBytesToRead - index - 1] = temp;
		}
		return S_OK;
	}
	HRESULT ReadFile3(void* pData, DWORD nBytesToRead)
	{
		return ReadFile((BYTE*)pData, nBytesToRead);
	}

	HRESULT WriteFile(void* pData, DWORD nBytesToWrite)
	{
		if (!m_pFile)
			return S_FALSE;

		size_t nCountWrite = fwrite((void*)pData, 1, nBytesToWrite, m_pFile);
		m_lFilePosition += nBytesToWrite; 
		return S_OK;
	}

	HRESULT WriteFile2(void* pData, DWORD nBytesToWrite)
	{
		if (!m_pFile)
			return S_FALSE;

		BYTE* mem = new BYTE[nBytesToWrite];
		memcpy(mem, pData, nBytesToWrite);

		for (size_t index = 0; index < nBytesToWrite / 2; ++index)
		{
			BYTE temp = mem[index];
			mem[index] = mem[nBytesToWrite - index - 1];
			mem[nBytesToWrite - index - 1] = temp;
		}

		return WriteFile(mem, nBytesToWrite);
	}

	HRESULT CreateFile(CString strFileName)
	{
		return _Open(strFileName, false, true, true);
	}
	HRESULT SetPosition( ULONG64 nPos )
	{	
		if (m_pFile && nPos <= (ULONG)m_lFileSize)
		{
			m_lFilePosition = nPos;
			fseek(m_pFile, m_lFilePosition, SEEK_SET);
			return S_OK;
		}
		else 
		{
			return !m_pFile ? S_FALSE : S_OK;
		}
	}
	LONG64  GetPosition()
	{
		return m_lFilePosition;
	}
	HRESULT SkipBytes(ULONG64 nCount)
	{
		return SetPosition(m_lFilePosition + nCount);
	}

	HRESULT CloseFile()
	{
		m_lFilePosition = 0;
		m_lFileSize = 0;

		if (m_pFile != NULL)
		{
			fclose(m_pFile);
			m_pFile = NULL;		
		}
		return S_OK;
	}

	ULONG64 GetFileSize()
	{
		return m_lFileSize;
	}

	HRESULT WriteReserved(DWORD dwCount)
	{
		BYTE* buf = new BYTE[dwCount];
		memset(buf, 0, (size_t)dwCount);
		HRESULT hr = WriteFile(buf, dwCount);
		RELEASEARRAYOBJECTS(buf);
		return hr;
	}
	HRESULT WriteReserved2(DWORD dwCount)
	{
		BYTE* buf = new BYTE[dwCount];
		memset(buf, 0xFF, (size_t)dwCount);
		HRESULT hr = WriteFile(buf, dwCount);
		RELEASEARRAYOBJECTS(buf);
		return hr;
	}
	HRESULT WriteReservedTo(DWORD dwPoint)
	{
		if (m_lFilePosition >= dwPoint)
			return S_OK;

		DWORD dwCount = dwPoint - (DWORD)m_lFilePosition;
		BYTE* buf = new BYTE[dwCount];
		memset(buf, 0, (size_t)dwCount);
		HRESULT hr = WriteFile(buf, dwCount);
		RELEASEARRAYOBJECTS(buf);
		return hr;
	}
	HRESULT SkipReservedTo(DWORD dwPoint)
	{
		if (m_lFilePosition >= dwPoint)
			return S_OK;

		DWORD dwCount = dwPoint - (DWORD)m_lFilePosition;
		return SkipBytes(dwCount);
	}

	LONG GetProgress()
	{
		if (0 >= m_lFileSize)
			return -1;

		double dVal = (double)(100 * m_lFilePosition);
		LONG lProgress = (LONG)(dVal / m_lFileSize);
		return lProgress;
	}

    void WriteStringUTF8(const CString& strXml)
	{
		BYTE* pData = NULL;
		LONG lLen = 0;

		NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strXml.GetString(), strXml.GetLength(), pData, lLen, false);

		WriteFile(pData, lLen);

		RELEASEARRAYOBJECTS(pData);
	}

protected:
	FILE* m_pFile;

	long m_lFilePosition;
	long m_lFileSize;
};

#ifdef _WIN32
namespace CDirectory
{
	static CString GetFolderName(CString strFolderPath)
	{
		int n1 = strFolderPath.ReverseFind('\\');
		if (-1 == n1)
			return _T("");

		return strFolderPath.Mid(n1 + 1);
	}
	static CString GetFolderPath(CString strFolderPath)
	{
		int n1 = strFolderPath.ReverseFind('\\');
		if (-1 == n1)
			return _T("");

		return strFolderPath.Mid(0, n1);
	}
	static BOOL OpenFile(CString strFolderPath, CString strFileName, CFile* pFile)
	{
        CString strFile = strFolderPath  + FILE_SEPARATOR_CHAR + strFileName;
		return (S_OK == pFile->OpenFile(strFile));
	}
	static BOOL CreateFile(CString strFolderPath, CString strFileName, CFile* pFile)
	{
        CString strFile = strFolderPath + FILE_SEPARATOR_CHAR + strFileName;
		return (S_OK == pFile->CreateFile(strFile));
	}
	static BOOL CreateDirectory(CString strFolderPathRoot, CString strFolderName)
	{
        CString strFolder = strFolderPathRoot + FILE_SEPARATOR_CHAR + strFolderName;
		return ::CreateDirectory(strFolder, NULL);
	}
	static BOOL CreateDirectory(CString strFolderPath)
	{
		return ::CreateDirectory(strFolderPath, NULL);
	}
    static BOOL DeleteFile (CString strFileName)
    {
        ::DeleteFile (strFileName);
        return TRUE;
    }

	static BOOL MoveFile(CString strExists, CString strNew, LPPROGRESS_ROUTINE lpFunc, LPVOID lpData) 
	{
#if (_WIN32_WINNT >= 0x0500)
		return ::MoveFileWithProgress(strExists, strNew, lpFunc, lpData, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH); 
#else
		return ::MoveFileEx(strExists, strNew, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH); 
#endif
	}

	static BOOL CopyFile(CString strExists, CString strNew, LPPROGRESS_ROUTINE lpFunc, LPVOID lpData) 
	{
		DeleteFile(strNew);
		return ::CopyFileEx(strExists, strNew, lpFunc, lpData, FALSE, 0); 
	}

	static CString GetUnder(CString strFolderPathRoot, CString strFolderName)
	{
		CString strFolder = strFolderPathRoot + '\\' + strFolderName;
		return strFolder;
	}

	static CString GetFileName(CString strFullName)
	{
		int nStart = strFullName.ReverseFind('\\');
		CString strName = strFullName.Mid(nStart + 1);
		return strName;
	}

	static CString BYTEArrayToString(BYTE* arr, size_t nCount)
	{
		CString str;
		for (size_t index = 0; index < nCount; ++index)
		{
			if ('\0' != (char)(arr[index]))
				str += (char)(arr[index]);
		}
		if (str.GetLength() == 0)
			str = _T("0");
		return str;
	}

	static CStringW BYTEArrayToStringW(BYTE* arr, size_t nCount)
	{
		CStringW str;
		wchar_t* pArr = (wchar_t*)arr;
		size_t nCountNew = nCount / 2;
		for (size_t index = 0; index < nCountNew; ++index)
		{
			str += pArr[index];
		}
		if (str.GetLength() == 0)
			str = _T("0");
		return str;
	}

	static CString BYTEArrayToString2(USHORT* arr, size_t nCount)
	{
		CString str;
		for (size_t index = 0; index < nCount; ++index)
		{
			if ('\0' != (char)(arr[index]))
				str += (char)(arr[index]);
		}
		if (str.GetLength() == 0)
			str = _T("0");
		return str;
	}

	static CString ToString(DWORD val)
	{
		CString str = _T("");
		str.Format(_T("%d"), (LONG)val);
		return str;
	}

	static CString ToString(UINT64 val, bool bInit)
	{
		CString strCoarse = ToString((DWORD)(val >> 32));
		if (_T("0") != strCoarse)
		{
			return strCoarse + ToString((DWORD)val);
		}
		
		return ToString((DWORD)val);
	}

	static UINT64 GetUINT64(CString strVal)
	{
		UINT64 nRet = 0;
		int nLen = strVal.GetLength();
		while (nLen > 0)
		{
			int nDig = XmlUtils::GetDigit(strVal[0]);
			nRet *= 10;
			nRet += nDig;
			strVal.Delete(0);
			--nLen;
		}
		return nRet;
	}
	static UINT GetUINT(CString strVal)
	{
		return (UINT)GetUINT64(strVal);
	}

	static void WriteValueToNode(CString strName, DWORD value, XmlUtils::CXmlWriter* pWriter)
	{
		pWriter->WriteNodeBegin(strName);
		pWriter->WriteString(CDirectory::ToString(value));
		pWriter->WriteNodeEnd(strName);
	}
	static void WriteValueToNode(CString strName, LONG value, XmlUtils::CXmlWriter* pWriter)
	{
		pWriter->WriteNodeBegin(strName);
		CString strLONG = _T("");
		strLONG.Format(_T("%d"), value);
		pWriter->WriteString(strLONG);
		pWriter->WriteNodeEnd(strName);
	}
	static void WriteValueToNode(CString strName, CString value, XmlUtils::CXmlWriter* pWriter)
	{
		pWriter->WriteNodeBegin(strName);
		pWriter->WriteString(value);
		pWriter->WriteNodeEnd(strName);
	}
	static void WriteValueToNode(CString strName, WCHAR value, XmlUtils::CXmlWriter* pWriter)
	{
		CString str(value);

		pWriter->WriteNodeBegin(strName);
		pWriter->WriteString(str);
		pWriter->WriteNodeEnd(strName);
	}
	static void WriteValueToNode(CString strName, bool value, XmlUtils::CXmlWriter* pWriter)
	{
		pWriter->WriteNodeBegin(strName);
		CString str = (true == value) ? _T("1") : _T("0");
		pWriter->WriteString(str);
		pWriter->WriteNodeEnd(strName);
	}
	static double FixedPointToDouble(DWORD point)
	{
		double dVal = (double)(point % 65536) / 65536;
		dVal += (point / 65536);
		return dVal;
	}
	static LONG NormFixedPoint(DWORD point, LONG base)
	{
		return (LONG)(FixedPointToDouble(point) * base);
	}
	static void SaveToFile(CString strFileName, CString strXml)
	{
        int nLength = strXml.GetLength();

		CStringA saStr;
		
#ifdef UNICODE
		// Encoding Unicode to UTF-8
		WideCharToMultiByte(CP_UTF8, 0, strXml.GetBuffer(), nLength + 1, saStr.GetBuffer(nLength*3 + 1), nLength*3, NULL, NULL);
		saStr.ReleaseBuffer();    
#else
		wchar_t* pWStr = new wchar_t[nLength + 1];
		if (!pWStr)
			return;

		// set end string
		pWStr[nLength] = 0;

		// Encoding ASCII to Unicode
        MultiByteToWideChar(CP_ACP, 0, strXml, nLength, pWStr, nLength);

        int nLengthW = (int)wcslen(pWStr);

		// Encoding Unicode to UTF-8
        WideCharToMultiByte(CP_UTF8, 0, pWStr, nLengthW + 1, saStr.GetBuffer(nLengthW*3 + 1), nLengthW*3, NULL, NULL);
		saStr.ReleaseBuffer();

	    delete[] pWStr;
#endif
		
		CFile oFile;
		oFile.CreateFile(strFileName);
		oFile.WriteFile((void*)saStr.GetBuffer(), saStr.GetLength());
		oFile.CloseFile();
	}

	static void SaveToFile2(CString strFileName, CStringA strVal)
	{
		CFile oFile;
		HRESULT hr = oFile.OpenFileRW(strFileName);

		if (S_OK != hr)
			oFile.CreateFile(strFileName);

		oFile.SkipBytes(oFile.GetFileSize());
		oFile.WriteFile((void*)strVal.GetBuffer(), strVal.GetLength());
		oFile.CloseFile();
	}
}

namespace StreamUtils
{
	static BYTE ReadBYTE(IStream* pStream)
	{
		BYTE lMem = 0;
		ULONG lReadByte = 0;
		pStream->Read(&lMem, 1, &lReadByte);
		if (lReadByte < 1)
		{
			lMem = 0;
		}
		return lMem;
	}
	static WORD ReadWORD(IStream* pStream)
	{
		WORD lWord = 0;
		BYTE pMem[2];
		ULONG lReadByte = 0;
		pStream->Read(pMem, 2, &lReadByte);
		if (lReadByte == 2)
		{
			lWord = ((pMem[1] << 8) | pMem[0]);
		}
		return lWord;
	}
	static DWORD ReadDWORD(IStream* pStream)
	{
		DWORD lDWord = 0;
		BYTE pMem[4];
		ULONG lReadByte = 0;
		pStream->Read(pMem, 4, &lReadByte);
		
#ifdef _DEBUG
		ATLASSERT(4 == lReadByte);
#endif

		if (lReadByte == 4)
		{
			lDWord = ((pMem[3] << 24) | (pMem[2] << 16) | (pMem[1] << 8) | pMem[0]);
		}
		return lDWord;
	}
	static SHORT ReadSHORT(IStream* pStream)
	{
		return (SHORT)ReadWORD(pStream);
	}
	static LONG ReadLONG(IStream* pStream)
	{
		return (LONG)ReadDWORD(pStream);
	}
	
	static FLOAT ReadFLOAT ( IStream* pStream )
	{
		FLOAT Value = 0.0f;
		pStream->Read ( &Value, sizeof (FLOAT), NULL );
		return Value;
	}

	static CString ReadCString(IStream* pStream, LONG lLen)
	{
		char* pData = new char[lLen + 1];
		ULONG lReadByte = 0;
		pStream->Read(pData, lLen, &lReadByte);

		pData[lLen] = 0;

		CString str(pData);

		delete[] pData;
		return str;
	}
	static CStringW ReadCStringW(IStream* pStream, LONG lLen)
	{
		wchar_t* pData = new wchar_t[lLen + 1];
		ULONG lReadByte = 0;
		pStream->Read(pData, 2 * lLen, &lReadByte);

		pData[lLen] = 0;

		CStringW str(pData);
		delete[] pData;
		return str;
	}
	static CString ConvertCStringWToCString(CStringW& strW)
	{
		/*WCHAR* pData = strW.GetBuffer();
		CString str(pData);*/
		//strW.ReleaseBuffer();

		BSTR bstr = strW.AllocSysString();
		CString str(bstr);
		SysFreeString(bstr);

		return str;
	}
	static void StreamSeek(long lOffset, IStream* pStream)
	{
		LARGE_INTEGER li; 
		li.LowPart = lOffset; 
		li.HighPart = 0; 
		pStream->Seek(li, STREAM_SEEK_SET, NULL); 
	}
	static void StreamPosition(long& lPosition, IStream* pStream)
	{
		ULARGE_INTEGER uli;
		LARGE_INTEGER li; 
		li.LowPart = 0; 
		li.HighPart = 0; 
		pStream->Seek(li, STREAM_SEEK_CUR, &uli);
		lPosition = (LONG)uli.LowPart;
	}
	static void StreamSkip(long lCount, IStream* pStream)
	{
		LARGE_INTEGER li; 
		li.LowPart = lCount;
		li.HighPart = 0; 
		pStream->Seek(li, STREAM_SEEK_CUR, NULL); 
	}
	static void StreamSkipBack(long lCount, IStream* pStream)
	{
		ULARGE_INTEGER ulPos;
		LARGE_INTEGER li; 
		li.LowPart = 0; 
		li.HighPart = 0; 
		pStream->Seek(li, STREAM_SEEK_CUR, &ulPos); 

		StreamSeek((long)(ulPos.LowPart - lCount), pStream);
	}
}
#else

#include <iostream>
#include <fstream>  // instream
#include <unistd.h> // unlink

#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/common/File.h"
/*
class CFile: public NSFile::CFileBinary
{
public:
    CFile()
        : NSFile::CFileBinary ()
    {

    }
    HRESULT WriteFile(void* pData, DWORD nBytesToWrite)
    {
        return (WriteFile (static_cast<char *> (pData), nBytesToWrite) ? S_OK : S_FALSE);
    }
    HRESULT CreateFile(CString strFileName)
    {
        return CreateFileW(strFileName) ? S_OK : S_FALSE;
    }
    HRESULT ReadFile(BYTE* pData, DWORD nBytesToRead)
    {
        DWORD dwRead = 0;
        return NSFile::CFileBinary::ReadFile(pData, nBytesToRead, dwRead) ? S_OK : S_FALSE;
    }
    HRESULT SetPosition( ULONG64 nPos )
    {
        if ((NULL != m_pFile) && nPos <= (ULONG)m_lFileSize)
        {
            int res = fseek(m_pFile, 0, SEEK_SET);

            m_lFilePosition = nPos;
            return 0 == res ? S_OK : S_FALSE;
        }

        return S_FALSE;
    }
};
*/
namespace CDirectory
{
    static void SaveToFile(CString strFileName, CString strXml)
    {
        std::string aContentUtf8 = stringWstingToUtf8String (strXml);
        std::string aFileNameUtf8 = stringWstingToUtf8String (strFileName);

        FILE *pFile = fopen(aFileNameUtf8.c_str(), "wb");

        if (NULL == pFile)
        {
            throw std::exception();
        }
        fwrite (aContentUtf8.c_str() , sizeof(char), aContentUtf8.size(), pFile);
        fclose (pFile);
    }
    static bool DeleteFile (CString strFileName)
    {
        std::string aFileNameUtf8 = stringWstingToUtf8String (strFileName);
        return 0 == unlink (aFileNameUtf8.c_str());
    }
    static CString ToString(DWORD val)
    {
        CString str = _T("");
        str.Format(_T("%d"), (LONG)val);
        return str;
    }
#ifndef LPPROGRESS_ROUTINE
#define LPPROGRESS_ROUTINE LPVOID
#endif
    static BOOL CopyFile (CString strExists, CString strNew, LPPROGRESS_ROUTINE lpFunc = NULL, LPVOID lpData = NULL)
    {
        BOOL bRes = TRUE;
        try
        {
            std::string stdstrFrom = stringWstingToUtf8String(strExists);
            std::string stdstrTo = stringWstingToUtf8String(strNew);
            std::ifstream  src (stdstrFrom.c_str(), std::ios::binary);
            std::ofstream  dst (stdstrTo.c_str(),   std::ios::binary);

            dst << src.rdbuf();
        }
        catch (...)
        {
            bRes = FALSE;
        }
        return bRes;
    }

}
#endif
