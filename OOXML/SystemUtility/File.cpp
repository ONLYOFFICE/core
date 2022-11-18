/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include <boost/lexical_cast.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

#include "../../Common/3dParty/pole/pole.h"
#include "../Base/unicode_util.h"
#include "./File.h"

namespace StreamUtils
{
	std::string md5(const BYTE* pData, const ULONG dataLen)
	{
		std::string strHash;
		boost::uuids::detail::md5 hash;
		boost::uuids::detail::md5::digest_type digest;

		hash.process_bytes(pData, dataLen);
		hash.get_digest(digest);

		const auto charDigest = reinterpret_cast<const char *>(&digest);
		boost::algorithm::hex(charDigest, charDigest + sizeof(boost::uuids::detail::md5::digest_type),
							  std::back_inserter(strHash));

		return strHash;
	}

	BYTE ReadBYTE(POLE::Stream* pStream)
	{
		if (pStream == NULL) return 0;

		BYTE lMem = 0;
		ULONG lReadByte = 0;

		lReadByte = (ULONG)pStream->read(&lMem, 1);
		if (lReadByte < 1)
		{
			lMem = 0;
		}
		return lMem;
	}
	WORD ReadWORD(POLE::Stream* pStream)
	{
		if (pStream == NULL) return 0;

		WORD lWord = 0;
		BYTE pMem[2];
		ULONG lReadByte = 0;

		lReadByte = (ULONG)pStream->read(pMem, 2);
		if (lReadByte == 2)
		{
			lWord = ((pMem[1] << 8) | pMem[0]);
		}
		return lWord;
	}
	DWORD ReadDWORD(POLE::Stream* pStream)
	{
		if (pStream == NULL) return 0;

		DWORD lDWord = 0;
		BYTE pMem[4];
		ULONG lReadByte = 0;
		lReadByte = (ULONG)pStream->read(pMem, 4);

		//#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
		//		ATLASSERT(4 == lReadByte);
		//#endif

		if (lReadByte == 4)
		{
			lDWord = ((pMem[3] << 24) | (pMem[2] << 16) | (pMem[1] << 8) | pMem[0]);
		}
		return 0xFFFFFFFF & lDWord;
	}
	SHORT ReadSHORT(POLE::Stream* pStream)
	{
		return (short)ReadWORD(pStream);
	}
	LONG ReadLONG(POLE::Stream* pStream)
	{
		return (LONG)ReadDWORD(pStream);
	}
	FLOAT ReadFLOAT( POLE::Stream* pStream)
	{
		if (pStream == NULL) return 0;

		FLOAT Value = 0.0f;
		pStream->read ((unsigned char*) &Value, sizeof (FLOAT));
		return Value;
	}

	std::string ReadStringA(POLE::Stream* pStream, LONG lLen)
	{
		if (pStream == NULL) return ("");

		char* pData = new char[lLen + 1];
		ULONG lReadByte = (ULONG)pStream->read((unsigned char*)pData, lLen);

		pData[lLen] = 0;

		std::string str(pData, lLen);

		delete[] pData;
		return str;
	}
	std::wstring ReadStringW(POLE::Stream* pStream, LONG lLen)
	{
		if (pStream == NULL) return (L"");

		unsigned char* pData = new unsigned char[2 * (lLen + 1)];
		memset (pData, 0, 2 * (lLen + 1));

		ULONG lReadByte = (ULONG)pStream->read(pData, 2 * lLen);

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
				return (L"");
			}
			std::wstring res((wchar_t*)pStrUtf32, lLen);
			if (pStrUtf32) delete [] pStrUtf32;
			return res;
		}
		else
		{
			std::wstring str((wchar_t*)pData);
			delete[] pData;
			return str;
		}

	}

	void StreamSeek(long lOffset, POLE::Stream* pStream)
	{
		if (pStream == NULL) return;

		pStream->seek(lOffset);
	}
	void StreamPosition(long& lPosition, POLE::Stream* pStream)
	{
		if (pStream == NULL) return ;

		lPosition = (LONG)pStream->tell();
	}
	void StreamSkip(long lCount, POLE::Stream* pStream)
	{
		if (pStream == NULL) return;

		pStream->seek(pStream->tell() + lCount);
	}
	void StreamSkipBack(long lCount, POLE::Stream* pStream)
	{
		if (pStream == NULL) return;

		pStream->seek(pStream->tell()-lCount);
	}
}

#ifdef CopyFile
#undef CopyFile
#endif

namespace CDirectory
{
	void SaveToFile(const std::wstring& strFileName, const std::wstring& strXml)
	{
		NSFile::CFileBinary file;
		file.CreateFileW(strFileName);
		file.WriteStringUTF8(strXml);
		file.CloseFile();
	}
	bool DeleteFile (const std::wstring& strFileName)
	{
		return NSFile::CFileBinary::Remove(strFileName);
	}
	bool CopyFile(const std::wstring& strExists, const std::wstring& strNew)
	{
		return NSFile::CFileBinary::Copy(strExists, strNew);
	}

	void WriteValueToNode(std::wstring strName, DWORD value, XmlUtils::CXmlWriter* pWriter)
	{
		pWriter->WriteNodeBegin(strName);
		pWriter->WriteString(boost::lexical_cast<std::wstring>(value));
		pWriter->WriteNodeEnd(strName);
	}

	void WriteValueToNode(std::wstring strName, LONG value, XmlUtils::CXmlWriter* pWriter)
	{
		pWriter->WriteNodeBegin(strName);
		pWriter->WriteString(boost::lexical_cast<std::wstring>(value));
		pWriter->WriteNodeEnd(strName);
	}
	void WriteValueToNode(std::wstring strName, std::wstring value, XmlUtils::CXmlWriter* pWriter)
	{
		pWriter->WriteNodeBegin(strName);
		pWriter->WriteString(value);
		pWriter->WriteNodeEnd(strName);
	}
	void WriteValueToNode(std::wstring strName, WCHAR value, XmlUtils::CXmlWriter* pWriter)
	{
		wchar_t str_arr[2]={};
		str_arr[0] = value;
		std::wstring str(str_arr);

		pWriter->WriteNodeBegin(strName);
		pWriter->WriteString(str);
		pWriter->WriteNodeEnd(strName);
	}
	void WriteValueToNode(std::wstring strName, bool value, XmlUtils::CXmlWriter* pWriter)
	{
		pWriter->WriteNodeBegin(strName);
		std::wstring str = (true == value) ? (L"1") : (L"0");
		pWriter->WriteString(str);
		pWriter->WriteNodeEnd(strName);
	}

	double FixedPointToDouble(DWORD point)
	{
		double dVal = (double)(point % 65536) / 65536;
		dVal += (point / 65536);
		return dVal;
	}
	LONG NormFixedPoint(DWORD point, LONG base)
	{
		return (LONG)(FixedPointToDouble(point) * base);
	}

	std::wstring BYTEArrayToString(BYTE* arr, size_t nCount)
	{
		std::wstring str;
		for (size_t index = 0; index < nCount; ++index)
		{
			if ('\0' != (char)(arr[index]))
				str += (char)(arr[index]);
		}
		if (str.length() == 0)
			str = (L"0");
		return str;
	}

	std::wstring BYTEArrayToStringW(BYTE* arr, size_t nCount)
	{
		std::wstring str;
		wchar_t* pArr = (wchar_t*)arr;
		size_t nCountNew = nCount / 2;
		for (size_t index = 0; index < nCountNew; ++index)
		{
			str += pArr[index];
		}
		if (str.length() == 0)
			str = (L"0");
		return str;
	}
}
