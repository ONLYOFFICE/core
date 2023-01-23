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
#include "Streams.h"
#include "Utils.h"
#include "Encrypt.h"
#include "Objects.h"
//#include "FastStringToDouble.h"

#include <sstream>

#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../UnicodeConverter/UnicodeConverter.h"
#include "../../DesktopEditor/common/StringExt.h"

#define DEFLATE_BUF_SIZ  ((int)(STREAM_BUF_SIZ * 1.1) + 13)

namespace PdfWriter
{
	static const char* c_pHexStrings[] =
	{
		"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
		"10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
		"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
		"30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
		"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
		"50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
		"60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
		"70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
		"80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
		"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
		"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
		"B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
		"C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
		"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
		"E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
		"F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"
	};
	//----------------------------------------------------------------------------------------
	// CStream
	//----------------------------------------------------------------------------------------
	bool           CStream::CheckSize(unsigned int unSize)
	{
		if (Size() - Tell() >= unSize)
			return true;

		return false;
	}
	unsigned int   CStream::ReadUInt()
	{
		if (!CheckSize(4))
			return 0;

		unsigned int unBytesRead = 1;
		int nChar0, nChar1, nChar2, nChar3;
		Read((BYTE*)&nChar0, &unBytesRead);
		Read((BYTE*)&nChar1, &unBytesRead);
		Read((BYTE*)&nChar2, &unBytesRead);
		Read((BYTE*)&nChar3, &unBytesRead);

		return (unsigned int)((nChar0 << 24) | (nChar1 << 16) | (nChar2 << 8) | nChar3);
	}
	unsigned char  CStream::ReadUChar()
	{
		if (!CheckSize(1))
			return 0;

		int nChar;
		unsigned int unBytesRead = 1;
		Read((BYTE*)&nChar, &unBytesRead);

		return nChar;	
	}
	char           CStream::ReadChar()
	{
		return (char)ReadUChar();
	}
	unsigned short CStream::ReadUShort()
	{
		if (!CheckSize(2))
			return 0;

		unsigned int unBytesRead = 1;
		int nChar0, nChar1;
		Read((BYTE*)&nChar0, &unBytesRead);
		Read((BYTE*)&nChar1, &unBytesRead);

		return (unsigned short)((nChar0 << 8) | nChar1);
	}
    void CStream::Write(const BYTE* pBuffer, unsigned int unSize, bool bCalcCheckSum)
	{
		Write(pBuffer, unSize);
		if (bCalcCheckSum)
		{
			CRC32 oCRC;
			oCRC.ProcessCRC((void*)pBuffer, unSize);
			m_unCheckSum = oCRC.m_nCRC32;
		}
	}
    void CStream::WriteChar(char nChar)
	{
		Write((BYTE*)&nChar, 1);
	}
    void CStream::WriteStr(const char* sString)
	{
		unsigned int nLen = StrLen(sString, -1);
		Write((BYTE*)sString, nLen);
	}
    void CStream::WriteUChar(unsigned char unValue)
	{
		Write(&unValue, 1);
	}
	void CStream::WriteInt(int nValue)
	{
		char pBuffer[32];
		memset(pBuffer, 0x00, 32);
		ItoA(pBuffer, nValue, pBuffer + 31);
		return WriteStr(pBuffer);
	}
    void CStream::WriteUInt(unsigned int unValue)
	{
		WriteInt((int)unValue);
	}
    void CStream::WriteHex(int nValue, int nLen)
	{
		if (2 == nLen)	
			Write((const BYTE*)c_pHexStrings[(unsigned char)nValue], 2);
		else if (4 == nLen)
		{
			Write((const BYTE*)c_pHexStrings[(unsigned char)(nValue >> 8)], 2);
			Write((const BYTE*)c_pHexStrings[(unsigned char)nValue], 2);
		}
		else if (6 == nLen)
		{
			Write((const BYTE*)c_pHexStrings[(unsigned char)(nValue >> 16)], 2);
			Write((const BYTE*)c_pHexStrings[(unsigned char)(nValue >> 8)], 2);
			Write((const BYTE*)c_pHexStrings[(unsigned char)nValue], 2);
		}
	}
    void CStream::WriteReal(float fValue)
	{
		char pBuffer[32];
		memset(pBuffer, 0x00, 32);
		FtoA(pBuffer, fValue, pBuffer + 31);
		return WriteStr(pBuffer);
	}
    void CStream::WriteReal(double dValue)
	{
		//int nIVal = (int)dValue;
		//int nFVal = (int)(fabs(dValue - nIVal) * 10000);

		//int nLen = 0;
		//const char* sString = NSFastIntToString::GetString(fabs(nIVal), nLen);
		//if (nIVal < 0)
		//	WriteChar('-');

		//Write((const BYTE*)sString, nLen);

		//if (nFVal)
		//{
		//	sString = NSFastIntToString::GetString(nFVal, nLen);

		//	WriteChar('.');
		//	int nZeros = 4 - nLen;
		//	if (nZeros > 0)
		//		Write((const BYTE*)NSFastIntToString::GetZeros(nZeros), nZeros);

		//	Write((const BYTE*)sString, nLen);
		//}

		//char pBuffer[32];
		//int nResLen = 0;
		//int nIVal = (int)dValue;
		//int nFVal = (int)(fabs(dValue - nIVal) * 10000);

		//int nLen = 0;
		//const char* sString = NSFastIntToString::GetString(fabs(nIVal), nLen);
		//if (nIVal < 0)
		//	pBuffer[nResLen++] = '-';

		//memcpy(pBuffer + nResLen, sString, nLen);
		//nResLen += nLen;

		//if (nFVal)
		//{
		//	int nRealLen = 0;
		//	sString = NSFastIntToString::GetPrecisionString(nFVal, nLen, nRealLen);

		//	pBuffer[nResLen++] = '.';
		//	int nZeros = 4 - nRealLen;
		//	if (nZeros > 0)
		//	{
		//		memcpy(pBuffer + nResLen, NSFastIntToString::GetZeros(nZeros), nZeros);
		//		nResLen += nZeros;
		//	}

		//	memcpy(pBuffer + nResLen, sString, nLen);
		//	nResLen += nLen;
		//}
		//return Write((const BYTE*)pBuffer, nResLen);

		char pBuffer[32];
		memset(pBuffer, 0x00, 32);
		FtoA(pBuffer, dValue, pBuffer + 31);
		return WriteStr(pBuffer);
	}
    void CStream::WriteEscapeName(const char* sValue)
	{
		char sTmpChar[LIMIT_MAX_NAME_LEN * 3 + 2];

		const BYTE* pPos1;
		char* pPos2;

		pPos1 = (BYTE*)sValue;
		pPos2 = sTmpChar;

		*pPos2++ = '/';

		int nLen = StrLen(sValue, LIMIT_MAX_NAME_LEN);
		for (int nIndex = 0; nIndex < nLen; nIndex++)
		{
			BYTE nChar = *pPos1++;
			if (NEEDS_ESCAPE(nChar))
			{
				*pPos2++ = '#';
				*pPos2 = (char)(nChar >> 4);

				if (*pPos2 <= 9)
					*pPos2 += 0x30;
				else
					*pPos2 += 0x41 - 10;

				pPos2++;

				*pPos2 = (char)(nChar & 0x0f);

				if (*pPos2 <= 9)
					*pPos2 += 0x30;
				else
					*pPos2 += 0x41 - 10;

				pPos2++;
			}
			else
				*pPos2++ = nChar;
		}

		*pPos2 = 0;

		Write((BYTE*)sTmpChar, StrLen(sTmpChar, -1));
	}
	void CStream::WriteEscapeText(const BYTE* sText, unsigned int unLen, bool isUTF16, bool isDictValue)
	{
		if (!unLen || !sText)
		{
			Write((BYTE*)"()", 2);
			return;
		}

		BYTE* sTxt = const_cast<BYTE*>(sText);
		unsigned short* pUtf16Data = NULL;

		WriteChar('(');

		if (isUTF16)
		{
			std::string sUtf8((char*)sText, unLen);
			std::wstring sUnicode = UTF8_TO_U(sUtf8);
			unsigned int unLenUtf16 = 0;
			pUtf16Data = NSStringExt::CConverter::GetUtf16FromUnicode(sUnicode, unLenUtf16, false);

			sTxt = (BYTE*)pUtf16Data;
			unLen = unLenUtf16 << 1;

			WriteChar(0xFE);
			WriteChar(0xFF);
		}

		char sBuf[TEXT_DEFAULT_LEN];
		unsigned int nIndex = 0;

		for (int nCounter = 0; nCounter < unLen; nCounter++)
		{
			BYTE nChar = (BYTE)*sTxt++;

			if ((isDictValue && NEEDS_ESCAPE_DICTVALUE(nChar)) || (!isDictValue && NEEDS_ESCAPE(nChar)))
			{
				sBuf[nIndex++] = '\\';
				sBuf[nIndex++] = 0x30 + (nChar >> 6);
				sBuf[nIndex++] = 0x30 + ((nChar & 0x38) >> 3);
				sBuf[nIndex++] = 0x30 + (nChar & 0x07);
			}
			else
			{
				sBuf[nIndex++] = nChar;
			}

			if (nIndex > TEXT_DEFAULT_LEN - 4)
			{
				Write((BYTE*)sBuf, nIndex);
				nIndex = 0;
			}
		}

		Write((BYTE*)sBuf, nIndex);
		WriteChar(')');

		if (pUtf16Data)
			delete []pUtf16Data;
	}
    void CStream::WriteBinary(const BYTE* pData, unsigned int unLen, CEncrypt* pEncrypt)
    {
        char sBuf[TEXT_DEFAULT_LEN];

        BYTE* pBuf = NULL;
        bool bDelete = false;
        unsigned int nIndex = 0;

        const BYTE* pBuffer = NULL;
        if (pEncrypt)
        {
            pBuf = new BYTE[unLen + 16 + 16]; // iv + padding
            bDelete = true;
            unLen = pEncrypt->CryptBuf(pData, pBuf, unLen);
            pBuffer = pBuf;
        }
        else
        {
            pBuffer = pData;
        }

        for (int nCounter = 0; nCounter < unLen; nCounter++, pBuffer++)
        {
            Write((const BYTE*)c_pHexStrings[*pBuffer], 2);
        }

        if (nIndex > 0)
        {
            Write((BYTE*)sBuf, nIndex);
        }

        if (bDelete)
            delete[] pBuf;
    }
    void CStream::WriteStreamWithDeflate(CStream* pStream, CEncrypt* pEncrypt)
    {
        unsigned long nRet = OK;

        CDeflate ZStream;
        BYTE inbuf[STREAM_BUF_SIZ];
        BYTE otbuf[DEFLATE_BUF_SIZ];

        BYTE *otbuf_all = NULL;

        // initialize input stream
        pStream->Seek(0, SeekSet);
		unsigned long size = pStream->Size();
		if (pEncrypt)
		{
			unsigned long size_out = (unsigned long)(size * 1.1 + 13 + 64);
			otbuf_all = new BYTE[size_out];
		}

        // initialize decompression stream.
        ZStream.SetOut(otbuf, DEFLATE_BUF_SIZ);
        ZStream.Init(DEFLATE_DEFAULT_COMPRESSION, -1);
        ZStream.SetIn(inbuf, 0);

        unsigned long size_crypt = 0;

		unsigned int offset = 0;
        while(true)
        {
            unsigned int unSize = STREAM_BUF_SIZ;
            pStream->Read(inbuf, &unSize);

            ZStream.SetIn(inbuf, unSize);

            if (0 == unSize)
                break;

            while (ZStream.GetAvailIn() > 0)
            {
                ZStream.Process(DEFLATE_NO_FLUSH);

                if (ZStream.GetAvailOut() == 0)
                {
                    if (pEncrypt)
                    {
						memcpy(otbuf_all + offset, otbuf, DEFLATE_BUF_SIZ);
						offset += DEFLATE_BUF_SIZ;
                    }
                    else
                        Write(otbuf, DEFLATE_BUF_SIZ);

                    ZStream.SetOut(otbuf, DEFLATE_BUF_SIZ);
                }
            }
        }

        bool bEnd = false;
        while(true)
        {
            nRet = ZStream.Process(DEFLATE_FINISH);
            if (DEFLATE_OK != nRet && DEFLATE_STREAM_END != nRet)
            {
                ZStream.End();
                return;
            }

            if (DEFLATE_STREAM_END == nRet)
                bEnd = true;

            if (ZStream.GetAvailOut() < DEFLATE_BUF_SIZ)
            {
                unsigned int osize = DEFLATE_BUF_SIZ - ZStream.GetAvailOut();
                if (pEncrypt)
                {
					memcpy(otbuf_all + offset, otbuf, osize);
					offset += osize;
                }
                else
                    Write(otbuf, osize);

                ZStream.SetOut(otbuf, DEFLATE_BUF_SIZ);
            }

            if (bEnd)
                break;
        }
        if (pEncrypt)
        {
			BYTE *etbuf_all = new BYTE[offset + 32];
			unsigned int osize = pEncrypt->CryptBuf(otbuf_all, etbuf_all, offset);
			delete []otbuf_all;

			Write(etbuf_all, osize);

			delete []etbuf_all;
        }

        ZStream.End();
    }
    void CStream::WriteStream(CStream* pStream, unsigned int unFilter, CEncrypt *pEncrypt)
    {
        if (pStream->Size() <= 0)
            return;

#ifndef FILTER_FLATE_DECODE_DISABLED

		if (unFilter & STREAM_FILTER_FLATE_DECODE)
			return WriteStreamWithDeflate(pStream, pEncrypt);

#endif
		pStream->Seek(0, SeekSet);
       
		if (pEncrypt)
        {
			unsigned int size = pStream->Size();

			BYTE *pBuf = new BYTE[size];
			BYTE *pEBuf = new BYTE[size + 32];
			
			pStream->Read(pBuf, &size);
			size = pEncrypt->CryptBuf(pBuf, pEBuf, size);
			Write(pEBuf, size);

			delete []pBuf;
			delete []pEBuf;
		}
		else
		{
			BYTE pBuf[STREAM_BUF_SIZ];

			while(true)
			{
				unsigned int unSize = STREAM_BUF_SIZ;
				pStream->Read(pBuf, &unSize);

				if (0 == unSize)
					break;
				Write(pBuf, unSize);
			}
		}
    }
    void CStream::Write(CNullObject* pNull)
    {
    }
    void CStream::Write(CBoolObject* pBool)
	{
		if (pBool->Get())
			WriteStr("true");
		else
			WriteStr("false");
	}
    void CStream::Write(CNumberObject* pNumber)
	{
		WriteInt(pNumber->Get());
	}
    void CStream::Write(CRealObject* pReal)
	{
		WriteReal(pReal->Get());
	}
    void CStream::Write(CNameObject* pName)
	{
		WriteEscapeName(pName->Get());
	}
    void CStream::Write(CStringObject* pString, CEncrypt* pEncrypt)
	{
		if (pEncrypt)
			pEncrypt->Reset();

		if (pEncrypt)
		{
			const BYTE* pBinary = pString->GetString();
			WriteChar('<');
			WriteBinary(pBinary, StrLen((const char*)pBinary, -1), pEncrypt);
			WriteChar('>');
		}
		else
		{            
            WriteEscapeText(pString->GetString(), pString->GetLength(), pString->IsUTF16(), pString->IsDictValue());
		}
	}
    void CStream::Write(CBinaryObject* pBinary, CEncrypt* pEncrypt)
	{
		unsigned int unLen  = pBinary->GetLength();
		BYTE*        pValue = pBinary->GetValue();

		if (0 == unLen || !pValue)
			return WriteStr("<>");

		if (pEncrypt)
			pEncrypt->Reset();

		WriteChar('<');
		WriteBinary(pValue, unLen, pEncrypt);
		WriteChar('>');
	}
    void CStream::Write(CArrayObject* pArray, CEncrypt* pEncrypt)
	{
		WriteStr("[ ");
		for (int nIndex = 0, nCount = pArray->GetCount(); nIndex < nCount; nIndex++)
		{
			CObjectBase* pObject = pArray->Get(nIndex, false);
			Write(pObject, pEncrypt);
			WriteChar(' ');
		}
		WriteChar(']');
	}
    void CStream::Write(CDictObject* pDict, CEncrypt* pEncrypt)
	{
		WriteStr("<<\012");

		pDict->BeforeWrite();

		// EncryptDict не надо шифровать 
		if (dict_type_ENCRYPT == pDict->GetDictType())
			pEncrypt = NULL;

		// Добавляем запись Filter
		if (pDict->GetStream())
		{
			pDict->Remove("Filter");
			unsigned int unFilter = pDict->GetFilter();
			if (STREAM_FILTER_NONE != unFilter)
			{
				CArrayObject* pFilter = new CArrayObject();
				pDict->Add("Filter", pFilter);

#ifndef FILTER_FLATE_DECODE_DISABLED
				if (unFilter & STREAM_FILTER_FLATE_DECODE)
					pFilter->Add("FlateDecode");
#endif
				if (unFilter & STREAM_FILTER_DCT_DECODE)
					pFilter->Add("DCTDecode");

				if (unFilter & STREAM_FILTER_JPX_DECODE)
					pFilter->Add("JPXDecode");

				if (unFilter & STREAM_FILTER_JBIG2_DECODE)
					pFilter->Add("JBIG2Decode");

				if (unFilter & STREAM_FILTER_LZW_DECODE)
					pFilter->Add("LZWDecode");

				if (unFilter & STREAM_FILTER_CCITT_DECODE)
					pFilter->Add("CCITTFaxDecode");

				unsigned int unPredictor = pDict->GetPredictor();
				//pDict->Remove("DecodeParams");
				if (STREAM_PREDICTOR_NONE != unPredictor)
				{
					CArrayObject* pDecode = new CArrayObject();
					pDict->Add("DecodeParams", pDecode);
#ifndef FILTER_FLATE_DECODE_DISABLED
					if (unFilter & STREAM_FILTER_FLATE_DECODE)
					{
						//if ( STREAM_PREDICTOR_FLATE_TIFF == oDict->nPredictor )
						pDecode->Add(new CNullObject());
					}
#endif
					if (unFilter & STREAM_FILTER_DCT_DECODE)
					{
						pDecode->Add(new CNullObject());
					}
					if (unFilter & STREAM_FILTER_JPX_DECODE)
					{
						pDecode->Add(new CNullObject());
					}
					if (unFilter & STREAM_FILTER_JBIG2_DECODE)
					{
						pDecode->Add(new CNullObject());
					}
					if (unFilter & STREAM_FILTER_LZW_DECODE)
					{
						pDecode->Add(new CNullObject());
					}
				}
			}
		}

		if (dict_type_SIGNATURE == pDict->GetDictType())
			pDict->WriteSignatureToStream(this, pEncrypt);
		else
			pDict->WriteToStream(this, pEncrypt);

		pDict->Write(this);
		WriteStr(">>");

		CStream* pStream = pDict->GetStream();
		if (pStream)
		{
			CNumberObject* pLength = (CNumberObject*)pDict->Get("Length");			
			// "Length" должен управляться таблицей Xref (флаг Indirect)
			if (pLength && object_type_NUMBER == pLength->GetType() && pLength->IsIndirect())
			{
				if (pEncrypt)
					pEncrypt->Reset();

				WriteStr("\012stream\015\012");
				
				unsigned int unStartSize = Tell();
				WriteStream(pStream, pDict->GetFilter(), pEncrypt);
				pLength->Set(Tell() - unStartSize);

				WriteStr("\012endstream");
			}
		}

		pDict->AfterWrite();
	}
    void CStream::Write(CObjectBase* pObject, CEncrypt* pEncrypt)
	{
		if (pObject)
		{
			pObject->Write(this, pEncrypt);
		}
	}
	//----------------------------------------------------------------------------------------
	// CMemoryStream
	//----------------------------------------------------------------------------------------
	CMemoryStream::CMemoryStream()
	{
		m_unSize      = 0;
		m_nBufferSize = 0;
		m_pBuffer     = NULL;
		m_pCur        = NULL;
	}
	CMemoryStream::CMemoryStream(unsigned int unBufferSize)
	{
		m_unSize      = 0;
		m_nBufferSize = 0;
		m_pBuffer     = NULL;
		m_pCur        = NULL;

		Shrink(unBufferSize);
	}
	CMemoryStream::~CMemoryStream()
	{
		Close();
	}
	bool         CMemoryStream::IsEof()
	{
		int nRemainBytes = Size() - Tell();
		if (nRemainBytes <= 0)
			return true;

		return false;
	}
	void         CMemoryStream::Write(const BYTE* pBuffer, unsigned int unSize)
	{
		unsigned int unRemainBytes = m_nBufferSize - Tell();
		if (unRemainBytes < unSize)
		{
            unsigned int unShrinkSize = std::max(unSize, (unsigned int)STREAM_BUF_SIZ);
			Shrink(unShrinkSize);
		}
		MemCpy(m_pCur, pBuffer, unSize);
		m_pCur += unSize;
        m_unSize = (unsigned int)std::max(m_unSize, (unsigned int)(m_pCur - m_pBuffer));
	}
	void         CMemoryStream::Read(BYTE* pBuffer, unsigned int* punSize)
	{
		unsigned int unRemainBytes = m_unSize - Tell();
        unsigned int unReadedBytes = (punSize ? std::min(*punSize, unRemainBytes) : unRemainBytes);
		MemCpy(pBuffer, m_pCur, unReadedBytes);
		m_pCur += unReadedBytes;
		if (punSize)
			*punSize = unReadedBytes;
	}
	void         CMemoryStream::Seek(int nPos, EWhenceMode eMode)
	{
		switch (eMode)
		{
		case SeekCur: m_pCur += nPos; break;
		case SeekEnd: m_pCur = m_pBuffer + m_unSize - nPos; break;
		case SeekSet: m_pCur = m_pBuffer + nPos; break;
		}
	}
	int          CMemoryStream::Tell()
	{
		return (int)(m_pCur - m_pBuffer);
	}
	void         CMemoryStream::Close()
	{
		if (m_pBuffer)
		{
			free(m_pBuffer);
			//delete[] m_pBuffer;
		}

		m_nBufferSize = 0;
		m_pBuffer     = NULL;
		m_pCur        = NULL;
		m_unSize      = 0;
	}
	unsigned int CMemoryStream::Size()
	{
		return m_unSize;
	}
	void         CMemoryStream::Shrink(unsigned int unSize)
	{
		if (m_pBuffer)
		{
			long lPos = Tell();

			//BYTE* pNewBuffer = new BYTE[m_nBufferSize + unSize];
			//MemCpy(pNewBuffer, m_pBuffer, m_unSize);
			//delete[] m_pBuffer;
			//m_pBuffer = pNewBuffer;
			//m_pCur = m_pBuffer + lPos;
			//m_nBufferSize = m_nBufferSize + unSize;

			m_pBuffer = (BYTE*)realloc(m_pBuffer, m_nBufferSize + unSize);
			m_pCur = m_pBuffer + lPos;
			m_nBufferSize = m_nBufferSize + unSize;
		}
		else
		{
			//m_pBuffer = new BYTE[unSize];
			//m_pCur = m_pBuffer;
			//m_nBufferSize = unSize;

			m_pBuffer = (BYTE*)malloc(unSize);
			m_pCur = m_pBuffer;
			m_nBufferSize = unSize;
		}
	}
	//----------------------------------------------------------------------------------------
	// CFileStream
	//----------------------------------------------------------------------------------------
	CFileStream::CFileStream()
	{
		m_wsFilePath = L"";
	}
	CFileStream::~CFileStream()
	{
		Close();
	}
	bool         CFileStream::IsEof()
	{
		if (m_oFile.TellFile() >= m_oFile.SizeFile())
			return true;

		return false;
	}
	void         CFileStream::Write(const BYTE* pBuffer, unsigned int unSize)
	{
		m_oFile.WriteFile((BYTE*)pBuffer, unSize);
	}
	void         CFileStream::Read(BYTE *pBuffer, unsigned int* punSize)
	{
		DWORD dwBytesToRead = *punSize;
		DWORD dwSizeRead = 0;
		m_oFile.ReadFile(pBuffer, dwBytesToRead, dwSizeRead);
		*punSize = dwSizeRead;
	}
	void         CFileStream::Seek(int nPos, EWhenceMode eMode)
	{
		switch (eMode)
		{
		case SeekCur: m_oFile.SeekFile(nPos, SEEK_CUR); break;
		case SeekEnd: m_oFile.SeekFile(nPos, SEEK_END); break;
		case SeekSet: m_oFile.SeekFile(nPos, SEEK_SET); break;
		}
	}
	int          CFileStream::Tell()
	{
		return m_oFile.TellFile();
	}
	void         CFileStream::Close()
	{
		m_oFile.CloseFile();
	}
	unsigned int CFileStream::Size()
	{
		return m_oFile.SizeFile();
	}
	bool         CFileStream::OpenFile(const std::wstring& wsFilePath, bool bWrite)
	{
		Close();

		m_wsFilePath = wsFilePath;

		if (!bWrite)
		{
			if (!m_oFile.OpenFile(wsFilePath, true))
				return false;	
			m_oFile.SeekFile(m_oFile.SizeFile(), SEEK_SET);
		}
		else
		{
            if (!m_oFile.CreateFileW(wsFilePath))
				return false;
		}

		return true;
	}
	//----------------------------------------------------------------------------------------
	// CImageFileStream
	//----------------------------------------------------------------------------------------
	CImageFileStream::CImageFileStream()
	{
		m_wsFilePath = L"";
		m_nFilePos  = 0;
		m_nFileSize = 0;
	}
	CImageFileStream::~CImageFileStream()
	{
		Close();
	}
	bool         CImageFileStream::IsEof()
	{
		if (m_nFilePos >= m_nFileSize)
			return true;

		return false;
	}
	void         CImageFileStream::Write(const BYTE* pBuffer, unsigned int unSize)
	{
        // not used
	}
	void         CImageFileStream::Read(BYTE *pBuffer, unsigned int* punSize)
	{
        if (!OpenFile())
        {
            *punSize = 0;
            return;
        }
		DWORD dwBytesToRead = *punSize;
		DWORD dwSizeRead = 0;
		m_oFile.ReadFile(pBuffer, dwBytesToRead, dwSizeRead);
		*punSize = dwSizeRead;
		CloseFile();
	}
	void         CImageFileStream::Seek(int nPos, EWhenceMode eMode)
	{
		switch (eMode)
		{
			case SeekCur: m_nFilePos += nPos; break;
        case SeekEnd: m_nFilePos = std::max(0, (m_nFileSize - nPos)); break;
			case SeekSet: m_nFilePos = nPos; break;
		}
	}
	int          CImageFileStream::Tell()
	{
		return m_nFilePos;
	}
	void         CImageFileStream::Close()
	{
		m_oFile.CloseFile();
	}
	unsigned int CImageFileStream::Size()
	{
		return m_nFileSize;
	}
	bool         CImageFileStream::OpenFile(const std::wstring& wsFilePath, bool bWrite)
	{
		Close();

		m_wsFilePath = wsFilePath;

		if (!bWrite)
		{
			if (!m_oFile.OpenFile(wsFilePath))
				return false;
		}
		else
		{
            if (!m_oFile.CreateFileW(wsFilePath))
				return false;
		}

		m_nFilePos = 0;
		m_nFileSize = m_oFile.SizeFile();
		CloseFile();
		return true;
	}
	bool         CImageFileStream::OpenFile()
	{
        if (!m_oFile.OpenFile(m_wsFilePath))
            return false;

		m_oFile.SeekFile(m_nFilePos);
		return true;
	}
	void         CImageFileStream::CloseFile()
	{
		m_nFilePos = m_oFile.TellFile();
		m_oFile.CloseFile();
	}
}
