#pragma once

#include "../XML/xmlutils.h"
#include "../../../../DesktopEditor/common/File.h"

#include "../../../3dParty/pole/pole.h"
#include "../Base/unicode_util.h"

//по аналогии с /DesktopEditor/common/File.h только для CString

#ifndef COINIT_MULTITHREADED
	#define COINIT_MULTITHREADED 0				//for win64 .... oO
#endif

class CFile 
{
private:
	HRESULT _Open(const CString& strFileName, bool bOpen = false, bool bCreate = false, bool bReadWrite = false)
	{
		HRESULT hRes = S_OK;
		CloseFile();

#if defined(_WIN32) || defined(_WIN32_WCE) ||defined(_WIN64)
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
namespace StreamUtils
{
	static BYTE ReadBYTE(POLE::Stream* pStream)
	{
		if (pStream == NULL) return 0;
		
		BYTE lMem = 0;
		ULONG lReadByte = 0;
		
		lReadByte = pStream->read(&lMem, 1);
		if (lReadByte < 1)
		{
			lMem = 0;
		}
		return lMem;
	}
	static WORD ReadWORD(POLE::Stream* pStream)
	{
		if (pStream == NULL) return 0;

		WORD lWord = 0;
		BYTE pMem[2];
		ULONG lReadByte = 0;
	
		lReadByte = pStream->read(pMem, 2);
		if (lReadByte == 2)
		{
			lWord = ((pMem[1] << 8) | pMem[0]);
		}
		return lWord;
	}
	static DWORD ReadDWORD(POLE::Stream* pStream)
	{
		if (pStream == NULL) return 0;

		DWORD lDWord = 0;
		BYTE pMem[4];
		ULONG lReadByte = 0;
		lReadByte = pStream->read(pMem, 4);
		
#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
		ATLASSERT(4 == lReadByte);
#endif

		if (lReadByte == 4)
		{
			lDWord = ((pMem[3] << 24) | (pMem[2] << 16) | (pMem[1] << 8) | pMem[0]);
		}
        return 0xFFFFFFFF & lDWord;
	}
	static SHORT ReadSHORT(POLE::Stream* pStream)
	{
		return (SHORT)ReadWORD(pStream);
	}
	static LONG ReadLONG(POLE::Stream* pStream)
	{
		return (LONG)ReadDWORD(pStream);
	}
	
	static FLOAT ReadFLOAT( POLE::Stream* pStream)
	{
		if (pStream == NULL) return 0;

		FLOAT Value = 0.0f;
		pStream->read ((unsigned char*) &Value, sizeof (FLOAT));
		return Value;
	}

	static CStringA ReadCStringA(POLE::Stream* pStream, LONG lLen)
	{
		if (pStream == NULL) return ("");

		char* pData = new char[lLen + 1];
		ULONG lReadByte = 0;
		
		lReadByte = pStream->read((unsigned char*)pData, lLen);

		pData[lLen] = 0;

		CStringA str(pData);

		delete[] pData;
		return str;
	}
	static CStringW ReadCStringW(POLE::Stream* pStream, LONG lLen)
	{
		if (pStream == NULL) return _T("");
	
		unsigned char* pData = new unsigned char[2 * (lLen + 1)];
		memset (pData, 0, 2 * (lLen + 1));

		ULONG lReadByte = 0;		
		lReadByte = pStream->read(pData, 2 * lLen);

		if (sizeof(wchar_t) == 4)
		{
			ConversionResult eUnicodeConversionResult;
			UTF32 *pStrUtf32 = new UTF32 [lLen + 1];
            pStrUtf32[lLen] = 0 ;

			const	UTF16 *pStrUtf16_Conv = (const UTF16 *) pData;
					UTF32 *pStrUtf32_Conv =                 pStrUtf32;

	eUnicodeConversionResult = ConvertUTF16toUTF32 ( &pStrUtf16_Conv
                                               , &pStrUtf16_Conv[lLen]
                                               , &pStrUtf32_Conv
                                               , &pStrUtf32 [lLen]
                                               , strictConversion);

			if (conversionOK != eUnicodeConversionResult)
			{
				delete [] pStrUtf32;
                return _T("");
            }
            CString res((wchar_t*)pStrUtf32, lLen);
			if (pStrUtf32) delete [] pStrUtf32;
            return res;
        }
		else
		{
			CStringW str((wchar_t*)pData);
			delete[] pData;
			return str;
		}

	}
	static CStringA ConvertCStringWToCStringA(CStringW& strW)
	{
		/*WCHAR* pData = strW.GetBuffer();
		CString str(pData);*/
		//strW.ReleaseBuffer();

		std::wstring	str_w = string2std_string(strW);
		std::string		str_a(str_w.begin(), str_w.end());

		return std_string2string(str_a);
	}
	static void StreamSeek(long lOffset, POLE::Stream* pStream)
	{
		if (pStream == NULL) return;

		pStream->seek(lOffset); 
	}
	static void StreamPosition(long& lPosition, POLE::Stream* pStream)
	{
		if (pStream == NULL) return ;

		lPosition = pStream->tell();
	}
	static void StreamSkip(long lCount, POLE::Stream* pStream)
	{
		if (pStream == NULL) return;

		pStream->seek(pStream->tell() + lCount); 
	}
	static void StreamSkipBack(long lCount, POLE::Stream* pStream)
	{
		if (pStream == NULL) return;

		pStream->seek(pStream->tell()-lCount); 
	}
}

#if defined(_WIN32) || defined(_WIN64)
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
    static bool OpenFile(CString strFolderPath, CString strFileName, CFile* pFile)
	{
        CString strFile = strFolderPath  + FILE_SEPARATOR_CHAR + strFileName;
		return (S_OK == pFile->OpenFile(strFile));
	}
    static bool CreateFile(CString strFolderPath, CString strFileName, CFile* pFile)
	{
        CString strFile = strFolderPath + FILE_SEPARATOR_CHAR + strFileName;
		return (S_OK == pFile->CreateFile(strFile));
	}
    static bool CreateDirectory(CString strFolderPathRoot, CString strFolderName)
	{
        CString strFolder = strFolderPathRoot + FILE_SEPARATOR_CHAR + strFolderName;
		return ::CreateDirectory(strFolder, NULL);
	}
    static bool CreateDirectory(CString strFolderPath)
	{
		return ::CreateDirectory(strFolderPath, NULL);
	}
    static bool DeleteFile (CString strFileName)
    {
        ::DeleteFile (strFileName);
        return true;
    }

    static bool MoveFile(CString strExists, CString strNew, LPVOID lpFunc, LPVOID lpData)
	{
#if (_WIN32_WINNT >= 0x0500) && !defined (_WIN64)
		return ::MoveFileWithProgress(strExists, strNew, (LPPROGRESS_ROUTINE)lpFunc, lpData, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH); 
#else
		return ::MoveFileEx(strExists, strNew, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH); 
#endif
	}

    static bool CopyFile(CString strExists, CString strNew, LPVOID lpFunc, LPVOID lpData)
	{
		DeleteFile(strNew);
#if defined (_WIN64)
		return ::CopyFile(strExists, strNew, true); 
#else
		return ::CopyFileEx(strExists, strNew, (LPPROGRESS_ROUTINE)lpFunc, lpData, FALSE, 0); 
#endif
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

    static bool CopyFile (CString strExists, CString strNew, LPVOID lpFunc = NULL, LPVOID lpData = NULL)
    {
        bool bRes = true;
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
            bRes = false;
        }
        return bRes;
    }
}
#endif

namespace CDirectory
{
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
        wchar_t str_arr[2]={};
        str_arr[0] = value;
        CString str(str_arr);

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
}
