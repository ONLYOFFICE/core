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
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "MemoryUtils.h"
#include "File.h"
#include "Constants.h"
#include "Object.h"
#include "Lexer.h"
#include "GState.h"
#include "Stream.h"
#include "CCITT-Tables.h"
#include "JPXStream.h"
#include "JBIG2Stream.h"

//#define USE_ZLIB // Для декодирования потоков в Flate используем Zlib или внутренний декодер

namespace PdfReader
{
	//---------------------------------------------------------------------------------------------------------------
	// Stream (основной класс)
	//---------------------------------------------------------------------------------------------------------------

	Stream::Stream()
	{
		m_nRef = 1;
	}

	Stream::~Stream()
	{
	}

	void Stream::Close()
	{
	}

	int  Stream::GetRawChar()
	{
		// TO DO : Error "Internal: called GetRawChar() on non-predictor stream"
		return EOF;
	}

	char *Stream::GetLine(char *sBuffer, int nSize)
	{
		int nIndex;

		if (LookChar() == EOF)
			return NULL;
		for (nIndex = 0; nIndex < nSize - 1; ++nIndex)
		{
			int nChar = GetChar();
			if (nChar == EOF || nChar == '\n')
				break;
			if (nChar == '\r')
			{
				if ((nChar = LookChar()) == '\n')
					GetChar();
				break;
			}
			sBuffer[nIndex] = nChar;
		}
		sBuffer[nIndex] = '\0';
		return sBuffer;
	}

	StringExt *Stream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		return new StringExt();
	}

	Stream *Stream::AddFilters(Object *pDict)
	{
		Object oTemp;
		Object oParms, params2;

		Stream *pStream = this;
		pDict->DictLookup("Filter", &oTemp);
		if (oTemp.IsNull())
		{
			oTemp.Free();
			pDict->DictLookup("F", &oTemp);
		}
		pDict->DictLookup("DecodeParms", &oParms);
		if (oParms.IsNull())
		{
			oParms.Free();
			pDict->DictLookup("DP", &oParms);
		}
		if (oTemp.IsName())
		{
			pStream = ApplyFilter(oTemp.GetName(), pStream, &oParms);
		}
		else if (oTemp.IsArray())
		{
			for (int nIndex = 0; nIndex < oTemp.ArrayGetLength(); ++nIndex)
			{
				Object oItem, oItemParams;
				oTemp.ArrayGet(nIndex, &oItem);
				if (oParms.IsArray())
					oParms.ArrayGet(nIndex, &oItemParams);
				else
					oItemParams.InitNull();
				if (oItem.IsName())
				{
					pStream = ApplyFilter(oItem.GetName(), pStream, &oItemParams);
				}
				else
				{
					// TO DO: Error "Bad filter name"
					pStream = new EOFStream(pStream);
				}
				oItem.Free();
				oItemParams.Free();
			}
		}
		else if (!oTemp.IsNull())
		{
			// TO DO: Error "Bad 'Filter' attribute in stream"
		}
		oTemp.Free();
		oParms.Free();

		return pStream;
	}

	Stream *Stream::ApplyFilter(char *sName, Stream *pStream, Object *pParams)
	{
		if (!strcmp(sName, "ASCIIHexDecode") || !strcmp(sName, "AHx"))
		{
			pStream = new ASCIIHexStream(pStream);
		}
		else if (!strcmp(sName, "ASCII85Decode") || !strcmp(sName, "A85"))
		{
			pStream = new ASCII85Stream(pStream);
		}
		else if (!strcmp(sName, "LZWDecode") || !strcmp(sName, "LZW"))
		{
			int nPredictor = 1;
			int nColumns = 1;
			int nColors = 1;
			int nBitsPerComponent = 8;
			int nEarlyChange = 1;

			if (pParams->IsDict())
			{
				Object oTemp;

				pParams->DictLookup("Predictor", &oTemp);
				if (oTemp.IsInt())
					nPredictor = oTemp.GetInt();
				oTemp.Free();

				pParams->DictLookup("Columns", &oTemp);
				if (oTemp.IsInt())
					nColumns = oTemp.GetInt();
				oTemp.Free();

				pParams->DictLookup("Colors", &oTemp);
				if (oTemp.IsInt())
					nColors = oTemp.GetInt();
				oTemp.Free();

				pParams->DictLookup("BitsPerComponent", &oTemp);
				if (oTemp.IsInt())
					nBitsPerComponent = oTemp.GetInt();
				oTemp.Free();

				pParams->DictLookup("EarlyChange", &oTemp);
				if (oTemp.IsInt())
					nEarlyChange = oTemp.GetInt();
				oTemp.Free();
			}
			pStream = new LZWStream(pStream, nPredictor, nColumns, nColors, nBitsPerComponent, nEarlyChange);
		}
		else if (!strcmp(sName, "RunLengthDecode") || !strcmp(sName, "RL"))
		{
			pStream = new RunLengthStream(pStream);
		}
		else if (!strcmp(sName, "CCITTFaxDecode") || !strcmp(sName, "CCF"))
		{
			int nK = 0;

			bool bEndOfLine = false;
			bool bByteAlign = false;
			int nColumns = 1728;
			int nRows = 0;
			bool bEndOfBlock = true;
			bool bBlackIs1 = false;

			if (pParams->IsDict())
			{
				Object oTemp;

				pParams->DictLookup("K", &oTemp);
				if (oTemp.IsInt())
					nK = oTemp.GetInt();
				oTemp.Free();

				pParams->DictLookup("EndOfLine", &oTemp);
				if (oTemp.IsBool())
					bEndOfLine = oTemp.GetBool();
				oTemp.Free();

				pParams->DictLookup("EncodedByteAlign", &oTemp);
				if (oTemp.IsBool())
					bByteAlign = oTemp.GetBool();
				oTemp.Free();

				pParams->DictLookup("Columns", &oTemp);
				if (oTemp.IsInt())
					nColumns = oTemp.GetInt();
				oTemp.Free();

				pParams->DictLookup("Rows", &oTemp);
				if (oTemp.IsInt())
					nRows = oTemp.GetInt();
				oTemp.Free();

				pParams->DictLookup("EndOfBlock", &oTemp);
				if (oTemp.IsBool())
					bEndOfBlock = oTemp.GetBool();
				oTemp.Free();

				pParams->DictLookup("BlackIs1", &oTemp);
				if (oTemp.IsBool())
					bBlackIs1 = oTemp.GetBool();
				oTemp.Free();

				// TO DO: Добавить чтение поля "DamagedRowsBeforeError"
			}
			pStream = new CCITTFaxStream(pStream, nK, bEndOfLine, bByteAlign, nColumns, nRows, bEndOfBlock, bBlackIs1);
		}
		else if (!strcmp(sName, "DCTDecode") || !strcmp(sName, "DCT"))
		{
			int nColorTransform = -1;

			if (pParams->IsDict())
			{
				Object oTemp;
				if (pParams->DictLookup("ColorTransform", &oTemp)->IsInt())
					nColorTransform = oTemp.GetInt();
				oTemp.Free();
			}
			pStream = new DCTStream(pStream, nColorTransform);
		}
		else if (!strcmp(sName, "FlateDecode") || !strcmp(sName, "Fl"))
		{
			int nPredictor = 1;
			int nColors = 1;
			int nBitsPerComponent = 8;
			int nColumns = 1;

			if (pParams->IsDict())
			{
				Object oTemp;

				pParams->DictLookup("Predictor", &oTemp);
				if (oTemp.IsInt())
					nPredictor = oTemp.GetInt();
				oTemp.Free();

				pParams->DictLookup("Colors", &oTemp);
				if (oTemp.IsInt())
					nColors = oTemp.GetInt();
				oTemp.Free();

				pParams->DictLookup("BitsPerComponent", &oTemp);
				if (oTemp.IsInt())
					nBitsPerComponent = oTemp.GetInt();
				oTemp.Free();

				pParams->DictLookup("Columns", &oTemp);
				if (oTemp.IsInt())
					nColumns = oTemp.GetInt();
				oTemp.Free();
			}
#ifdef USE_ZLIB
			pStream = new FlateZlibStream( pStream, nPredictor, nColumns, nColors, nBitsPerComponent );
#else
			pStream = new FlateStream(pStream, nPredictor, nColumns, nColors, nBitsPerComponent);
#endif
		}
		else if (!strcmp(sName, "JBIG2Decode"))
		{
			Object oJBIG2Globals;
			if (pParams->IsDict())
			{
				pParams->DictLookup("JBIG2Globals", &oJBIG2Globals);
			}
			// TO DO: Сделать данный фильтр
			pStream = new JBIG2Stream(pStream, &oJBIG2Globals);
			oJBIG2Globals.Free();
		}
		else if (!strcmp(sName, "JPXDecode"))
		{
			pStream = new JPXStream(pStream);
		}
		else
		{
			// TO DO: Error "Unknown filter"
			pStream = new EOFStream(pStream);
		}
		return pStream;
	}

	//---------------------------------------------------------------------------------------------------------------
	// BaseStream
	//---------------------------------------------------------------------------------------------------------------

	BaseStream::BaseStream(Object *pDict)
	{
		m_pDict = *pDict;
	}

	BaseStream::~BaseStream()
	{
		m_pDict.Free();
	}

	//---------------------------------------------------------------------------------------------------------------
	// FilterStream
	//---------------------------------------------------------------------------------------------------------------

	FilterStream::FilterStream(Stream *pStream)
	{
		m_pStream = pStream;
	}

	FilterStream::~FilterStream()
	{
	}

	void FilterStream::Close()
	{
		m_pStream->Close();
	}

	void FilterStream::SetPos(unsigned int unPos, int nDirection)
	{
		// TO DO: Error "Internal: called SetPos() on FilterStream"
	}

	//---------------------------------------------------------------------------------------------------------------
	// ImageStream
	//---------------------------------------------------------------------------------------------------------------

	ImageStream::ImageStream(Stream *pStream, int nWidth, int nComponents, int nBitsPerComponent)
	{
		int nLineSize = 0;

		m_pStream = pStream;
		m_nWidth  = nWidth;
		m_nComponentsPerPixel = nComponents;
		m_nBitsPerComponent   = nBitsPerComponent;

		m_nComponentsPerLine  = m_nWidth * m_nComponentsPerPixel;

		if (1 == m_nBitsPerComponent)
		{
			nLineSize = (m_nComponentsPerLine + 7) & ~7;
		}
		else
		{
			nLineSize = m_nComponentsPerLine;
		}
		m_pLineBuffer = (unsigned char *)MemUtilsMallocArray(nLineSize, sizeof(unsigned char));
		m_nLinePos = m_nComponentsPerLine;
	}

	ImageStream::~ImageStream()
	{
		MemUtilsFree(m_pLineBuffer);
	}

	void ImageStream::Reset()
	{
		m_pStream->Reset();
	}

	bool ImageStream::GetPixel(unsigned char *pPixel)
	{
		if (m_nLinePos >= m_nComponentsPerLine)
		{
			GetNextLine();
			m_nLinePos = 0;
		}
		for (int nIndex = 0; nIndex < m_nComponentsPerPixel; ++nIndex)
		{
			pPixel[nIndex] = m_pLineBuffer[m_nLinePos++];
		}
		return true;
	}

	unsigned char *ImageStream::GetNextLine()
	{
		if (m_nBitsPerComponent == 1)
		{
			for (int nIndex = 0; nIndex < m_nComponentsPerLine; nIndex += 8)
			{
				int nChar = m_pStream->GetChar();
				m_pLineBuffer[nIndex + 0] = (unsigned char)((nChar >> 7) & 1);
				m_pLineBuffer[nIndex + 1] = (unsigned char)((nChar >> 6) & 1);
				m_pLineBuffer[nIndex + 2] = (unsigned char)((nChar >> 5) & 1);
				m_pLineBuffer[nIndex + 3] = (unsigned char)((nChar >> 4) & 1);
				m_pLineBuffer[nIndex + 4] = (unsigned char)((nChar >> 3) & 1);
				m_pLineBuffer[nIndex + 5] = (unsigned char)((nChar >> 2) & 1);
				m_pLineBuffer[nIndex + 6] = (unsigned char)((nChar >> 1) & 1);
				m_pLineBuffer[nIndex + 7] = (unsigned char)(nChar & 1);
			}
		}
		else if (m_nBitsPerComponent == 8)
		{
			for (int nIndex = 0; nIndex < m_nComponentsPerLine; ++nIndex)
			{
				m_pLineBuffer[nIndex] = m_pStream->GetChar();
			}
		}
		else
		{
			unsigned long nBitMask = (1 << m_nBitsPerComponent) - 1;
			unsigned long nTemp = 0;
			int nBits = 0;
			for (int nIndex = 0; nIndex < m_nComponentsPerLine; ++nIndex)
			{
				if (nBits < m_nBitsPerComponent)
				{
					nTemp = (nTemp << 8) | (m_pStream->GetChar() & 0xff);
					nBits += 8;
				}
				m_pLineBuffer[nIndex] = (unsigned char)((nTemp >> (nBits - m_nBitsPerComponent)) & nBitMask);
				nBits -= m_nBitsPerComponent;
			}
		}
		return m_pLineBuffer;
	}

	void ImageStream::SkipLine()
	{
		int nCount = (m_nComponentsPerLine * m_nBitsPerComponent + 7) >> 3;
		for (int nIndex = 0; nIndex < nCount; ++nIndex)
		{
			m_pStream->GetChar();
		}
	}

	//---------------------------------------------------------------------------------------------------------------
	// StreamPredictor
	//---------------------------------------------------------------------------------------------------------------

	StreamPredictor::StreamPredictor(Stream *pStream, int nPredictor, int nWidth, int nComponents, int nBitsPerComponent)
	{
		m_pStream      = pStream;
		m_nPredictor   = nPredictor;
		m_nWidth       = nWidth;
		m_nComponentsPerPixel = nComponents;
		m_nBitsPerComponent   = nBitsPerComponent;
		m_pLineBuffer         = NULL;
		m_bSuccess            = false;

		m_nComponentsPerLine = m_nWidth * m_nComponentsPerPixel;

		// Patch1
		m_nBytesPerPixel = (m_nComponentsPerPixel * m_nBitsPerComponent + 7) >> 3;
		m_nBytesPerLine  = ((m_nComponentsPerLine * m_nBitsPerComponent + 7) >> 3) + m_nBytesPerPixel;

		if (m_nWidth <= 0 || m_nComponentsPerPixel <= 0 || m_nBitsPerComponent <= 0 || m_nComponentsPerPixel >= GrColorMaxComps || m_nBitsPerComponent > 16 || m_nWidth >= INT_MAX / m_nComponentsPerPixel || m_nComponentsPerLine >= (INT_MAX - 7) / m_nBitsPerComponent)
			return; // m_bSuccess = false;

		// End Patch1

		if (m_nBytesPerLine <= 0)
			return; // m_bSuccess = false;

		m_pLineBuffer = (unsigned char *)MemUtilsMalloc(m_nBytesPerLine);

		memset(m_pLineBuffer, 0, m_nBytesPerLine);
		m_nLinePos = m_nBytesPerLine;

		m_bSuccess = true;
	}

	StreamPredictor::~StreamPredictor()
	{
		MemUtilsFree(m_pLineBuffer);
	}

	int StreamPredictor::LookChar()
	{
		if (m_nLinePos >= m_nBytesPerLine)
		{
			if (!GetNextLine())
			{
				return EOF;
			}
		}
		return m_pLineBuffer[m_nLinePos];
	}

	int StreamPredictor::GetChar()
	{
		if (m_nLinePos >= m_nBytesPerLine)
		{
			if (!GetNextLine())
			{
				return EOF;
			}
		}
		return m_pLineBuffer[m_nLinePos++];
	}

	bool StreamPredictor::GetNextLine()
	{
		int nCurPredictor = 1;
		int nLeft, nTop, nTopLeft, nCur, nLeftDiff, nTopDiff, nTopLeftDiff;
		int nChar;

		// PNG prediction
		if (m_nPredictor >= 10)
		{
			if ((nCurPredictor = m_pStream->GetRawChar()) == EOF)
			{
				return false;
			}
			nCurPredictor += 10;
		}
		else
		{
			nCurPredictor = m_nPredictor;
		}

		// Читаем по строке, применяя PNG (byte) фильтрацию(prediction)
		unsigned char arrTopLeftBuf[GrColorMaxComps * 2 + 1];
		memset(arrTopLeftBuf, 0, m_nBytesPerPixel + 1);

		for (int nIndex = m_nBytesPerPixel; nIndex < m_nBytesPerLine; ++nIndex)
		{
			for (int nJ = m_nBytesPerPixel; nJ > 0; --nJ)
			{
				arrTopLeftBuf[nJ] = arrTopLeftBuf[nJ - 1];
			}
			arrTopLeftBuf[0] = m_pLineBuffer[nIndex];
			if ((nChar = m_pStream->GetRawChar()) == EOF)
			{
				if (nIndex > m_nBytesPerPixel)
				{
					// this ought to return false, but some (broken) PDF files
					// contain truncated image data, and Adobe apparently reads the
					// last partial line
					break;
				}
				return false;
			}
			switch (nCurPredictor)
			{
				case 11:			// PNG Sub
				m_pLineBuffer[nIndex] = m_pLineBuffer[nIndex - m_nBytesPerPixel] + (unsigned char)nChar;
				break;
				case 12:			// PNG Up
				m_pLineBuffer[nIndex] = m_pLineBuffer[nIndex] + (unsigned char)nChar;
				break;
				case 13:			// PNG Average
				m_pLineBuffer[nIndex] = ((m_pLineBuffer[nIndex - m_nBytesPerPixel] + m_pLineBuffer[nIndex]) >> 1) + (unsigned char)nChar;
				break;
				case 14:			// PNG Paeth
				nLeft = m_pLineBuffer[nIndex - m_nBytesPerPixel];
				nTop  = m_pLineBuffer[nIndex];
				nTopLeft = arrTopLeftBuf[m_nBytesPerPixel];
				nCur = nLeft + nTop - nTopLeft;
				if ((nLeftDiff = nCur - nLeft) < 0)
					nLeftDiff = -nLeftDiff;
				if ((nTopDiff = nCur - nTop) < 0)
					nTopDiff = -nTopDiff;
				if ((nTopLeftDiff = nCur - nTopLeft) < 0)
					nTopLeftDiff = -nTopLeftDiff;
				if (nLeftDiff <= nTopDiff && nLeftDiff <= nTopLeftDiff)
					m_pLineBuffer[nIndex] = nLeft + (unsigned char)nChar;
				else if (nTopDiff <= nTopLeftDiff)
					m_pLineBuffer[nIndex] = nTop + (unsigned char)nChar;
				else
					m_pLineBuffer[nIndex] = nTopLeft + (unsigned char)nChar;
				break;
				case 10:			// PNG none
				default:			// No predictor или TIFF predictor
				m_pLineBuffer[nIndex] = (unsigned char)nChar;
				break;
			}
		}

		// Применяем TIFF фильтрацию (predictor)
		unsigned long nInTemp, nOutTemp;
		if (m_nPredictor == 2)
		{
			if (m_nBitsPerComponent == 1)
			{
				nInTemp = m_pLineBuffer[m_nBytesPerPixel - 1];
				for (int nIndex = m_nBytesPerPixel; nIndex < m_nBytesPerLine; nIndex += 8)
				{
					// 1-bit add is just xor
					nInTemp = (nInTemp << 8) | m_pLineBuffer[nIndex];
					m_pLineBuffer[nIndex] ^= nInTemp >> m_nComponentsPerPixel;
				}
			}
			else if (m_nBitsPerComponent == 8)
			{
				for (int nIndex = m_nBytesPerPixel; nIndex < m_nBytesPerLine; ++nIndex)
				{
					m_pLineBuffer[nIndex] += m_pLineBuffer[nIndex - m_nComponentsPerPixel];
				}
			}
			else
			{
				memset(arrTopLeftBuf, 0, m_nComponentsPerPixel + 1);
				int nBitMask = (1 << m_nBitsPerComponent) - 1;
				nInTemp = 0, nOutTemp = 0;
				int nInBits = 0, nOutBits = 0;
				int nJ = m_nBytesPerPixel, nK = m_nBytesPerPixel;
				for (int nIndex = 0; nIndex < m_nWidth; ++nIndex)
				{
					for (int nKK = 0; nKK < m_nComponentsPerPixel; ++nKK)
					{
						if (nInBits < m_nBitsPerComponent)
						{
							nInTemp = (nInTemp << 8) | (m_pLineBuffer[nJ++] & 0xff);
							nInBits += 8;
						}
						arrTopLeftBuf[nKK] = (unsigned char)((arrTopLeftBuf[nKK] + (nInTemp >> (nInBits - m_nBitsPerComponent))) & nBitMask);
						nInBits -= m_nBitsPerComponent;
						nOutTemp = (nOutTemp << m_nBitsPerComponent) | arrTopLeftBuf[nKK];
						nOutBits += m_nBitsPerComponent;
						if (nOutBits >= 8)
						{
							m_pLineBuffer[nK++] = (unsigned char)(nOutTemp >> (nOutBits - 8));
							nOutBits -= 8;
						}
					}
				}
				if (nOutBits > 0)
				{
					m_pLineBuffer[nK++] = (unsigned char)((nOutTemp << (8 - nOutBits)) + (nInTemp & ((1 << (8 - nOutBits)) - 1)));
				}
			}
		}

		m_nLinePos = m_nBytesPerPixel;

		return true;
	}

	//---------------------------------------------------------------------------------------------------------------
	// FileStream
	//---------------------------------------------------------------------------------------------------------------

	FileStream::FileStream(FILE *pFile, unsigned int unStart, bool bLimited, unsigned int nLength, Object *pDict) :
		BaseStream(pDict)
	{
		m_pFile    = pFile;

		m_unStart  = unStart;
		m_bLimited = bLimited;
		m_unLength = nLength;
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;
		m_unBufferPos = m_unStart;
		m_nSavePos = 0;
		m_bSaved = false;
	}

	FileStream::~FileStream()
	{
		Close();
	}

	Stream *FileStream::MakeSubStream(unsigned int unStart, bool bLimited, unsigned int unLength, Object *pDict)
	{
		return new FileStream(m_pFile, unStart, bLimited, unLength, pDict);
	}

	void FileStream::Reset()
	{
		m_nSavePos = (unsigned int)ftell(m_pFile);
		fseek(m_pFile, m_unStart, SEEK_SET);

		m_bSaved = true;
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;
		m_unBufferPos = m_unStart;
	}

	void FileStream::Close()
	{
		if (m_bSaved)
		{
			fseek(m_pFile, m_nSavePos, SEEK_SET);
			m_bSaved = false;
		}
	}

	bool FileStream::FillBuffer()
	{
		int nCurBufLen = 0;

		m_unBufferPos += m_pBufferEnd - m_sBuffer;
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;
		if (m_bLimited && m_unBufferPos >= m_unStart + m_unLength)
			return false;

		if (m_bLimited && m_unBufferPos + FileStreamBufferSize > m_unStart + m_unLength)
			nCurBufLen = m_unStart + m_unLength - m_unBufferPos;
		else
			nCurBufLen = FileStreamBufferSize;

		nCurBufLen = fread(m_sBuffer, 1, nCurBufLen, m_pFile);
		m_pBufferEnd = m_sBuffer + nCurBufLen;

		if (m_pBufferPointer >= m_pBufferEnd)
			return false;

		return true;
	}

	void FileStream::SetPos(unsigned int unPos, int nDirection)
	{
		if (nDirection >= 0)
		{
			fseek(m_pFile, unPos, SEEK_SET);
			m_unBufferPos = unPos;
		}
		else
		{
			fseek(m_pFile, 0, SEEK_END);
			unsigned int unSize = (unsigned int)ftell(m_pFile);
			if (unPos > unSize)
				unPos = (unsigned int)unSize;
			fseek(m_pFile, -(int)unPos, SEEK_END);
			m_unBufferPos = (unsigned int)ftell(m_pFile);
		}
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;
	}

	void FileStream::SetStartPos(int nDelta)
	{
		m_unStart += nDelta;
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;
		m_unBufferPos = m_unStart;
	}

	//---------------------------------------------------------------------------------------------------------------
	// MemoryStream
	//---------------------------------------------------------------------------------------------------------------

	MemoryStream::MemoryStream(char *sBuffer, unsigned int unStart, unsigned int unLength, Object *pDict) :
		BaseStream(pDict)
	{
		m_sBuffer    = sBuffer;
		m_unStart    = unStart;
		m_unLength   = unLength;
		m_pBufferEnd = m_sBuffer + m_unStart + m_unLength;
		m_pBufferPointer = m_sBuffer + m_unStart;
		m_bNeedFree = false;
	}

	MemoryStream::~MemoryStream()
	{
		if (m_bNeedFree)
			MemUtilsFree(m_sBuffer);
	}
	char* MemoryStream::getCurrent()
	{
		return m_pBufferPointer;
	}
	unsigned int MemoryStream::getCurrentLength()
	{
		return (m_pBufferEnd - m_pBufferPointer);
	}

	Stream *MemoryStream::MakeSubStream(unsigned int unStart, bool bLimited, unsigned int unLength, Object *pDict)
	{
		unsigned int unNewLength = 0;

		if (!bLimited || unStart + unLength > m_unStart + m_unLength)
		{
			unNewLength = m_unStart + m_unLength - unStart;
			if (unStart >= m_unStart + m_unLength)
                unNewLength = 0;
		}
		else
		{
			unNewLength = unLength;
		}
		MemoryStream *pSubStream = new MemoryStream(m_sBuffer, unStart, unNewLength, pDict);
		return pSubStream;
	}

	void MemoryStream::Reset()
	{
		m_pBufferPointer = m_sBuffer + m_unStart;
	}

	void MemoryStream::Close()
	{
	}

	void MemoryStream::SetPos(unsigned int unPos, int nDirection)
	{
		unsigned int unCurPos = 0;

		if (nDirection >= 0)
		{
			unCurPos = unPos;
		}
		else
		{
			unCurPos = m_unStart + m_unLength - unPos;
		}

		if (unCurPos < m_unStart)
		{
			unCurPos = m_unStart;
		}
		else if (unCurPos > m_unStart + m_unLength)
		{
			unCurPos = m_unStart + m_unLength;
		}
		m_pBufferPointer = m_sBuffer + unCurPos;
	}

	void MemoryStream::SetStartPos(int nDelta)
	{
		m_unStart += nDelta;
		m_unLength -= nDelta;
		m_pBufferPointer = m_sBuffer + m_unStart;
	}

	//---------------------------------------------------------------------------------------------------------------
	// EmbedStream
	//---------------------------------------------------------------------------------------------------------------

	EmbedStream::EmbedStream(Stream *pStream, Object *pDict, bool bLimited, unsigned int unLength) :
		BaseStream(pDict)
	{
		m_pStream  = pStream;
		m_bLimited = bLimited;
		m_unLength = unLength;
	}

	EmbedStream::~EmbedStream()
	{
	}

	Stream *EmbedStream::MakeSubStream(unsigned int unStart, bool bLimited, unsigned int unLength, Object *pDict)
	{
		// TO DO: Error "Internal: called MakeSubStream() on EmbedStream"
		return NULL;
	}

	int EmbedStream::GetChar()
	{
		if (m_bLimited && !m_unLength)
		{
			return EOF;
		}
		--m_unLength;
		return m_pStream->GetChar();
	}

	int EmbedStream::LookChar()
	{
		if (m_bLimited && !m_unLength)
		{
			return EOF;
		}
		return m_pStream->LookChar();
	}

	void EmbedStream::SetPos(unsigned int unPos, int nDirection)
	{
		// TO DO: Error "Internal: called SetPos() on EmbedStream"
	}

	unsigned int EmbedStream::GetStartPos()
	{
		// TO DO: Error "Internal: called GetStartPos() on EmbedStream"
		return 0;
	}

	void EmbedStream::SetStartPos(int nDelta)
	{
		// TO DO: Error "Internal: called SetStartPos() on EmbedStream"
	}

	//---------------------------------------------------------------------------------------------------------------
	// ASCIIHexStream
	//---------------------------------------------------------------------------------------------------------------

	ASCIIHexStream::ASCIIHexStream(Stream *pStream) :
		FilterStream(pStream)
	{
		m_nBuffer = EOF;
		m_bEOF    = false;
	}

	ASCIIHexStream::~ASCIIHexStream()
	{
		delete m_pStream;
	}

	void ASCIIHexStream::Reset()
	{
		m_pStream->Reset();
		m_nBuffer = EOF;
		m_bEOF    = false;
	}

	int ASCIIHexStream::LookChar()
	{
		int nFirstChar = 0, nSecondChar = 0, nHexValue = 0;

		if (m_nBuffer != EOF)
			return m_nBuffer;
		if (m_bEOF)
		{
			m_nBuffer = EOF;
			return EOF;
		}
		do {
			nFirstChar = m_pStream->GetChar();
		} while (isspace(nFirstChar));

		if (nFirstChar == '>')
		{
			m_bEOF = true;
			m_nBuffer = EOF;
			return m_nBuffer;
		}
		do {
			nSecondChar = m_pStream->GetChar();
		} while (isspace(nSecondChar));

		if (nSecondChar == '>')
		{
			m_bEOF = true;
			nSecondChar = '0';
		}

		if (nFirstChar >= '0' && nFirstChar <= '9')
		{
			nHexValue = (nFirstChar - '0') << 4;
		}
		else if (nFirstChar >= 'A' && nFirstChar <= 'F')
		{
			nHexValue = (nFirstChar - 'A' + 10) << 4;
		}
		else if (nFirstChar >= 'a' && nFirstChar <= 'f')
		{
			nHexValue = (nFirstChar - 'a' + 10) << 4;
		}
		else if (nFirstChar == EOF)
		{
			m_bEOF = true;
			nHexValue = 0;
		}
		else
		{
			// TO DO: Error "Illegal character in ASCIIHex stream"
			nHexValue = 0;
		}

		if (nSecondChar >= '0' && nSecondChar <= '9')
		{
			nHexValue += nSecondChar - '0';
		}
		else if (nSecondChar >= 'A' && nSecondChar <= 'F')
		{
			nHexValue += nSecondChar - 'A' + 10;
		}
		else if (nSecondChar >= 'a' && nSecondChar <= 'f')
		{
			nHexValue += nSecondChar - 'a' + 10;
		}
		else if (nSecondChar == EOF)
		{
			m_bEOF = true;
			nHexValue = 0;
		}
		else
		{
			// TO DO: Error "Illegal character in ASCIIHex stream"
		}
		m_nBuffer = nHexValue & 0xff;
		return m_nBuffer;
	}

	StringExt *ASCIIHexStream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		StringExt *seResult;

		if (nPSLevel < 2)
		{
			return NULL;
		}
		if (!(seResult = m_pStream->GetPSFilter(nPSLevel, sIndent)))
		{
			return NULL;
		}
		seResult->Append(sIndent)->Append("/ASCIIHexDecode filter\n");
		return seResult;
	}

	bool ASCIIHexStream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(false);
	}

	//---------------------------------------------------------------------------------------------------------------
	// ASCII85Stream
	//---------------------------------------------------------------------------------------------------------------

	ASCII85Stream::ASCII85Stream(Stream *pStream) :
		FilterStream(pStream)
	{
		m_nIndex = m_nCount = 0;
		m_bEOF = false;
	}

	ASCII85Stream::~ASCII85Stream()
	{
		delete m_pStream;
	}

	void ASCII85Stream::Reset()
	{
		m_pStream->Reset();
		m_nIndex = m_nCount = 0;
		m_bEOF = false;
	}

	int ASCII85Stream::LookChar()
	{
		int nK = 0;

		if (m_nIndex >= m_nCount)
		{
			if (m_bEOF)
				return EOF;
			m_nIndex = 0;
			do {
				m_arrC[0] = m_pStream->GetChar();
			} while (Lexer::IsSpace(m_arrC[0]));

			if (m_arrC[0] == '~' || m_arrC[0] == EOF)
			{
				m_bEOF = true;
				m_nCount = 0;
				return EOF;
			}
			else if (m_arrC[0] == 'z')
			{
				m_arrB[0] = m_arrB[1] = m_arrB[2] = m_arrB[3] = 0;
				m_nCount = 4;
			}
			else
			{
				for (nK = 1; nK < 5; ++nK)
				{
					do {
						m_arrC[nK] = m_pStream->GetChar();
					} while (Lexer::IsSpace(m_arrC[nK]));

					if (m_arrC[nK] == '~' || m_arrC[nK] == EOF)
						break;
				}
				m_nCount = nK - 1;
				if (nK < 5 && (m_arrC[nK] == '~' || m_arrC[nK] == EOF))
				{
					for (++nK; nK < 5; ++nK)
						m_arrC[nK] = 0x21 + 84;
					m_bEOF = true;
				}
				unsigned long unT = 0;
				for (nK = 0; nK < 5; ++nK)
					unT = unT * 85 + (m_arrC[nK] - 0x21);
				for (nK = 3; nK >= 0; --nK)
				{
					m_arrB[nK] = (int)(unT & 0xff);
					unT >>= 8;
				}
			}
		}
		return m_arrB[m_nIndex];
	}

	StringExt *ASCII85Stream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		StringExt *seResult;

		if (nPSLevel < 2)
		{
			return NULL;
		}
		if (!(seResult = m_pStream->GetPSFilter(nPSLevel, sIndent)))
		{
			return NULL;
		}
		seResult->Append(sIndent)->Append("/ASCII85Decode filter\n");
		return seResult;
	}

	bool ASCII85Stream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(false);
	}

	//---------------------------------------------------------------------------------------------------------------
	// LZWStream
	//---------------------------------------------------------------------------------------------------------------

	LZWStream::LZWStream(Stream *pStream, int nPredictor, int nColumns, int nColors, int nBitsPerPixel, int nEarlyChange) :
		FilterStream(pStream)
	{
		if (1 != nPredictor)
		{
			m_pPredictor = new StreamPredictor(this, nPredictor, nColumns, nColors, nBitsPerPixel);
			if (!m_pPredictor->CheckValidate())
			{
				delete m_pPredictor;
				m_pPredictor = NULL;
			}
		}
		else
		{
			m_pPredictor = NULL;
		}

		m_nEarlyChange = nEarlyChange;
		m_bEOF = false;
		m_nInputBits = 0;
		ClearTable();
	}

	LZWStream::~LZWStream()
	{
		if (m_pPredictor)
		{
			delete m_pPredictor;
		}
		delete m_pStream;
	}

	int LZWStream::GetChar()
	{
		if (m_pPredictor)
		{
			return m_pPredictor->GetChar();
		}
		if (m_bEOF)
		{
			return EOF;
		}
		if (m_nCurPos >= m_nCurLength)
		{
			if (!ProcessNextCode())
			{
				return EOF;
			}
		}
		return m_arrCurBuffer[m_nCurPos++];
	}

	int LZWStream::LookChar()
	{
		if (m_pPredictor)
		{
			return m_pPredictor->LookChar();
		}
		if (m_bEOF)
		{
			return EOF;
		}
		if (m_nCurPos >= m_nCurLength)
		{
			if (!ProcessNextCode())
			{
				return EOF;
			}
		}
		return m_arrCurBuffer[m_nCurPos];
	}

	int LZWStream::GetRawChar()
	{
		// Отличие от GetChar в том, что тут мы неиспользуем фильтр, указанный в m_pPredictor
		if (m_bEOF)
		{
			return EOF;
		}
		if (m_nCurPos >= m_nCurLength)
		{
			if (!ProcessNextCode())
			{
				return EOF;
			}
		}
		return m_arrCurBuffer[m_nCurPos++];
	}

	void LZWStream::Reset()
	{
		m_pStream->Reset();
		m_bEOF = false;
		m_nInputBits = 0;
		ClearTable();
	}

	bool LZWStream::ProcessNextCode()
	{
		if (m_bEOF)
		{
			return false;
		}

		// Проверяем конец кода(eod) и очищаем таблицу кодов
	start:
		int nCode = GetCode();

		if (nCode == EOF || nCode == 257)
		{
			m_bEOF = true;
			return false;
		}
		if (nCode == 256)
		{
			ClearTable();
			goto start;
		}
		if (m_nNextCode >= 4097)
		{
			// TO DO: Error "Bad LZW stream - expected clear-table code"
			ClearTable();
		}

		// Process the next code

		int nNextLength = m_nCurLength + 1;
		if (nCode < 256)
		{
			m_arrCurBuffer[0] = nCode;
			m_nCurLength = 1;
		}
		else if (nCode < m_nNextCode)
		{
			m_nCurLength = m_pTable[nCode].nLength;
			int nI = 0, nJ = 0;
			for (nI = m_nCurLength - 1, nJ = nCode; nI > 0; --nI)
			{
				m_arrCurBuffer[nI] = m_pTable[nJ].unTail;
				nJ = m_pTable[nJ].nHead;
			}
			m_arrCurBuffer[0] = nJ;
		}
		else if (nCode == m_nNextCode)
		{
			m_arrCurBuffer[m_nCurLength] = m_nNewChar;
			++m_nCurLength;
		}
		else
		{
			// TO DO : Error "Bad LZW stream - unexpected code"
			m_bEOF = true;
			return false;
		}

		m_nNewChar = m_arrCurBuffer[0];
		if (m_bFirst)
		{
			m_bFirst = false;
		}
		else
		{
			m_pTable[m_nNextCode].nLength = nNextLength;
			m_pTable[m_nNextCode].nHead   = m_nPrevCode;
			m_pTable[m_nNextCode].unTail  = m_nNewChar;
			++m_nNextCode;
			if (m_nNextCode + m_nEarlyChange == 512)
				m_nNextBits = 10;
			else if (m_nNextCode + m_nEarlyChange == 1024)
				m_nNextBits = 11;
			else if (m_nNextCode + m_nEarlyChange == 2048)
				m_nNextBits = 12;
		}
		m_nPrevCode = nCode;

		m_nCurPos = 0;

		return true;
	}

	void LZWStream::ClearTable()
	{
		m_nNextCode = 258;
		m_nNextBits = 9;
		m_nCurPos = m_nCurLength = 0;
		m_bFirst = true;
	}

	int LZWStream::GetCode()
	{
		int nChar = 0;

		while (m_nInputBits < m_nNextBits)
		{
			if ((nChar = m_pStream->GetChar()) == EOF)
				return EOF;
			m_nInputBuffer = (m_nInputBuffer << 8) | (nChar & 0xff);
			m_nInputBits += 8;
		}
		int nCode = (m_nInputBuffer >> (m_nInputBits - m_nNextBits)) & ((1 << m_nNextBits) - 1);
		m_nInputBits -= m_nNextBits;
		return nCode;
	}

	StringExt *LZWStream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		StringExt *seResult;

		if (nPSLevel < 2 || m_pPredictor)
		{
			return NULL;
		}
		if (!(seResult = m_pStream->GetPSFilter(nPSLevel, sIndent)))
		{
			return NULL;
		}
		seResult->Append(sIndent)->Append("<< ");
		if (!m_nEarlyChange)
		{
			seResult->Append("/EarlyChange 0 ");
		}
		seResult->Append(">> /LZWDecode filter\n");
		return seResult;
	}

	bool LZWStream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(true);
	}

	//---------------------------------------------------------------------------------------------------------------
	// RunLengthStream
	//---------------------------------------------------------------------------------------------------------------

	RunLengthStream::RunLengthStream(Stream *pStream) :
		FilterStream(pStream)
	{
		m_pBufferPointer = m_pEndOfBuffer = m_sBuffer;
		m_bEOF = false;
	}

	RunLengthStream::~RunLengthStream()
	{
		delete m_pStream;
	}

	void RunLengthStream::Reset()
	{
		m_pStream->Reset();
		m_pBufferPointer = m_pEndOfBuffer = m_sBuffer;
		m_bEOF = false;
	}

	StringExt *RunLengthStream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		StringExt *seResult;

		if (nPSLevel < 2)
		{
			return NULL;
		}
		if (!(seResult = m_pStream->GetPSFilter(nPSLevel, sIndent)))
		{
			return NULL;
		}
		seResult->Append(sIndent)->Append("/RunLengthDecode filter\n");
		return seResult;
	}

	bool RunLengthStream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(true);
	}

	bool RunLengthStream::FillBuffer()
	{
		int nLen = 0;

		if (m_bEOF)
			return false;

		int nChar = m_pStream->GetChar();
		if (nChar == 0x80 || nChar == EOF)
		{
			m_bEOF = true;
			return false;
		}
		if (nChar < 0x80)
		{
			nLen = nChar + 1;
			for (int nIndex = 0; nIndex < nLen; ++nIndex)
				m_sBuffer[nIndex] = (char)m_pStream->GetChar();
		}
		else
		{
			nLen = 0x101 - nChar;
			nChar = m_pStream->GetChar();
			for (int nIndex = 0; nIndex < nLen; ++nIndex)
				m_sBuffer[nIndex] = (char)nChar;
		}
		m_pBufferPointer = m_sBuffer;
		m_pEndOfBuffer = m_sBuffer + nLen;
		return true;
	}

	//---------------------------------------------------------------------------------------------------------------
	// CCITTFaxStream
	//---------------------------------------------------------------------------------------------------------------

	CCITTFaxStream::CCITTFaxStream(Stream *pStream, int nK, bool bEndOfLine, bool bByteAlign, int nColumns, int nRows, bool bEndOfBlock, bool bBlackIs1) :
		FilterStream(pStream)
	{
		m_nK = nK;
		m_bEndOfLine = bEndOfLine;
		m_bByteAlign = bByteAlign;

		m_nColumns = nColumns;
		if (m_nColumns < 1)
			m_nColumns = 1;
		if (m_nColumns + 4 <= 0)
			m_nColumns = INT_MAX - 4;

		m_nRows = nRows;
		m_bEndOfBlock = bEndOfBlock;
		m_bBlackIs1 = bBlackIs1;

		m_pRefLine    = (short *)MemUtilsMallocArray(m_nColumns + 3, sizeof(short));
		m_pCodingLine = (short *)MemUtilsMallocArray(m_nColumns + 2, sizeof(short));

		m_bEOF = false;
		m_mCurRow = 0;
		m_bNextLine2D = (m_nK < 0);
		m_nInputBits = 0;
		m_pCodingLine[0] = 0;
		m_pCodingLine[1] = m_pRefLine[2] = m_nColumns;
		m_nCurPosCL = 1;

		m_nCharBuffer = EOF;
	}

	CCITTFaxStream::~CCITTFaxStream()
	{
		delete m_pStream;
		MemUtilsFree(m_pRefLine);
		MemUtilsFree(m_pCodingLine);
	}

	void CCITTFaxStream::Reset()
	{
		short nCode1;

		m_pStream->Reset();
		m_bEOF = false;
		m_mCurRow = 0;
		m_bNextLine2D = (m_nK < 0);
		m_nInputBits = 0;
		m_pCodingLine[0] = 0;
		m_pCodingLine[1] = m_nColumns;
		m_nCurPosCL = 1;
		m_nCharBuffer = EOF;

		// Пропускам начальные нулевые биты и символы окончания строки, и считываем тэг 2D кодировки
		while ((nCode1 = LookBits(12)) == 0)
		{
			SkipBits(1);
		}
		if (nCode1 == 0x001)
		{
			SkipBits(12);
		}
		if (m_nK > 0)
		{
			m_bNextLine2D = !LookBits(1);
			SkipBits(1);
		}
	}

	int CCITTFaxStream::LookChar()
	{
		short nCode1, nCode2, nCode3;
		int nIndex = 0;

		if (m_bEOF && m_pCodingLine[m_nCurPosCL] >= m_nColumns)
		{
			return EOF;
		}

		// Считываем строку
		bool bError = false;
		if (m_pCodingLine[m_nCurPosCL] >= m_nColumns)
		{

			// 2D кодировка
			if (m_bNextLine2D)
			{
				// Начальное состояние:
				//   nNewCLPos = Текущая позиция в кодированной строке (0 <= nNewCLPos <= m_nColumns)
				//   m_pCodingLine[m_nCurPosCL] = Последнее изменение в кодированной строке
				//                    (black-to-white, если m_nCurPosCL четно,
				//                     white-to-black, если m_nCurPosCL нечетно)
				//   m_pRefLine[m_nCurPosRL] = Следующее изменение в ссылочной строке, противоположного
				//                    цвета, цвету m_nCurPosCL
				// В любой момент должно быть верно:
				//   0 <= m_pCodingLine[m_nCurPosCL] <= nNewCLPos <= m_pRefLine[m_nCurPosRL] <= m_pRefLine[m_nCurPosRL + 1] <= m_nColumns
				//   0 <= m_nCurPosCL <= m_nColumns + 1
				//   m_pRefLine[0] = 0
				//   m_pRefLine[n] = m_pRefLine[n + 1] = columns -- для некоторого 1 <= n <= m_nColumns + 1
				// Условие окончания алгоритма:
				//   0 = m_pCodingLine[0] <= m_pCodingLine[1] < m_pCodingLine[2] < ... < m_pCodingLine[n-1] < m_pCodingLine[n] = m_nColumns,
				//   где <= n <= m_nColumns + 1

				int nNewCLPos = 0;
				for (nIndex = 0; m_pCodingLine[nIndex] < m_nColumns; ++nIndex)
				{
					m_pRefLine[nIndex] = m_pCodingLine[nIndex];
				}
				m_pRefLine[nIndex] = m_pRefLine[nIndex + 1] = m_nColumns;
				m_nCurPosRL = 1;
				nNewCLPos = m_pCodingLine[m_nCurPosCL = 0] = 0;
				do {
					nCode1 = Get2DCode();
					switch (nCode1)
					{
						case Pass_2D:
						if (m_pRefLine[m_nCurPosRL] < m_nColumns)
						{
							nNewCLPos = m_pRefLine[m_nCurPosRL + 1];
							m_nCurPosRL += 2;
						}
						break;
						case Horiz_2D:
						if ((m_nCurPosCL & 1) == 0)
						{
							nCode1 = nCode2 = 0;
							do {
								nCode1 += nCode3 = GetWhiteCode();
							} while (nCode3 >= 64);
							do {
								nCode2 += nCode3 = GetBlackCode();
							} while (nCode3 >= 64);
						}
						else
						{
							nCode1 = nCode2 = 0;
							do {
								nCode1 += nCode3 = GetBlackCode();
							} while (nCode3 >= 64);
							do {
								nCode2 += nCode3 = GetWhiteCode();
							} while (nCode3 >= 64);
						}
						if (nCode1 > 0 || nCode2 > 0)
						{
							if (nNewCLPos + nCode1 <= m_nColumns)
							{
								m_pCodingLine[m_nCurPosCL + 1] = nNewCLPos + nCode1;
							}
							else
							{
								m_pCodingLine[m_nCurPosCL + 1] = m_nColumns;
							}
							++m_nCurPosCL;
							if (m_pCodingLine[m_nCurPosCL] + nCode2 <= m_nColumns)
							{
								m_pCodingLine[m_nCurPosCL + 1] = m_pCodingLine[m_nCurPosCL] + nCode2;
							}
							else
							{
								m_pCodingLine[m_nCurPosCL + 1] = m_nColumns;
							}
							++m_nCurPosCL;
							nNewCLPos = m_pCodingLine[m_nCurPosCL];
							while (m_pRefLine[m_nCurPosRL] <= nNewCLPos && m_pRefLine[m_nCurPosRL] < m_nColumns)
							{
								m_nCurPosRL += 2;
							}
						}
						break;
						case Vert0_2D:
						if (m_pRefLine[m_nCurPosRL] < m_nColumns)
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_pRefLine[m_nCurPosRL];
							++m_nCurPosRL;
							while (m_pRefLine[m_nCurPosRL] <= nNewCLPos && m_pRefLine[m_nCurPosRL] < m_nColumns)
							{
								m_nCurPosRL += 2;
							}
						}
						else
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_nColumns;
						}
						break;
						case VertR1_2D:
						if (m_pRefLine[m_nCurPosRL] + 1 < m_nColumns)
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_pRefLine[m_nCurPosRL] + 1;
							++m_nCurPosRL;
							while (m_pRefLine[m_nCurPosRL] <= nNewCLPos && m_pRefLine[m_nCurPosRL] < m_nColumns)
							{
								m_nCurPosRL += 2;
							}
						}
						else
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_nColumns;
						}
						break;
						case VertL1_2D:
						if (m_pRefLine[m_nCurPosRL] - 1 > nNewCLPos || (m_nCurPosCL == 0 && m_pRefLine[m_nCurPosRL] == 1))
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_pRefLine[m_nCurPosRL] - 1;
							--m_nCurPosRL;
							while (m_pRefLine[m_nCurPosRL] <= nNewCLPos && m_pRefLine[m_nCurPosRL] < m_nColumns)
							{
								m_nCurPosRL += 2;
							}
						}
						break;
						case VertR2_2D:
						if (m_pRefLine[m_nCurPosRL] + 2 < m_nColumns)
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_pRefLine[m_nCurPosRL] + 2;
							++m_nCurPosRL;
							while (m_pRefLine[m_nCurPosRL] <= nNewCLPos && m_pRefLine[m_nCurPosRL] < m_nColumns)
							{
								m_nCurPosRL += 2;
							}
						}
						else
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_nColumns;
						}
						break;
						case VertL2_2D:
						if (m_pRefLine[m_nCurPosRL] - 2 > nNewCLPos || (m_nCurPosCL == 0 && m_pRefLine[m_nCurPosRL] == 2))
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_pRefLine[m_nCurPosRL] - 2;
							--m_nCurPosRL;
							while (m_pRefLine[m_nCurPosRL] <= nNewCLPos && m_pRefLine[m_nCurPosRL] < m_nColumns)
							{
								m_nCurPosRL += 2;
							}
						}
						break;
						case VertR3_2D:
						if (m_pRefLine[m_nCurPosRL] + 3 < m_nColumns)
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_pRefLine[m_nCurPosRL] + 3;
							++m_nCurPosRL;
							while (m_pRefLine[m_nCurPosRL] <= nNewCLPos && m_pRefLine[m_nCurPosRL] < m_nColumns)
							{
								m_nCurPosRL += 2;
							}
						}
						else
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_nColumns;
						}
						break;
						case VertL3_2D:
						if (m_pRefLine[m_nCurPosRL] - 3 > nNewCLPos || (m_nCurPosCL == 0 && m_pRefLine[m_nCurPosRL] == 3))
						{
							nNewCLPos = m_pCodingLine[++m_nCurPosCL] = m_pRefLine[m_nCurPosRL] - 3;
							--m_nCurPosRL;
							while (m_pRefLine[m_nCurPosRL] <= nNewCLPos && m_pRefLine[m_nCurPosRL] < m_nColumns)
							{
								m_nCurPosRL += 2;
							}
						}
						break;
						case EOF:
						m_bEOF = true;
						m_pCodingLine[m_nCurPosCL = 0] = m_nColumns;
						return EOF;
						default:
						// TO DO: Error "Bad 2D code in CCITTFax stream"
						bError = true;
						break;
					}
				} while (m_pCodingLine[m_nCurPosCL] < m_nColumns);
			}
			else // 1-D encoding
			{
				m_pCodingLine[m_nCurPosCL = 0] = 0;
				while (1)
				{
					nCode1 = 0;
					do {
						nCode1 += nCode3 = GetWhiteCode();
					} while (nCode3 >= 64);
					m_pCodingLine[m_nCurPosCL + 1] = m_pCodingLine[m_nCurPosCL] + nCode1;
					++m_nCurPosCL;
					if (m_pCodingLine[m_nCurPosCL] >= m_nColumns)
					{
						break;
					}
					nCode2 = 0;
					do {
						nCode2 += nCode3 = GetBlackCode();
					} while (nCode3 >= 64);
					m_pCodingLine[m_nCurPosCL + 1] = m_pCodingLine[m_nCurPosCL] + nCode2;
					++m_nCurPosCL;
					if (m_pCodingLine[m_nCurPosCL] >= m_nColumns)
					{
						break;
					}
				}
			}

			if (m_pCodingLine[m_nCurPosCL] != m_nColumns)
			{
				// TO DO: Error "CCITTFax row is wrong length "

				// Выставляем корректную длину
				while (m_pCodingLine[m_nCurPosCL] > m_nColumns)
				{
					--m_nCurPosCL;
				}
				m_pCodingLine[++m_nCurPosCL] = m_nColumns;
				bError = true;
			}

			if (m_bByteAlign)
			{
				m_nInputBits &= ~7;
			}

			// Проверяем символ конца строки, пропуска нулевые биты
			bool bEOL = false;
			if (!m_bEndOfBlock && m_mCurRow == m_nRows - 1)
			{
				m_bEOF = true;
			}
			else
			{
				nCode1 = LookBits(12);
				while (nCode1 == 0)
				{
					SkipBits(1);
					nCode1 = LookBits(12);
				}
				if (nCode1 == 0x001)
				{
					SkipBits(12);
					bEOL = true;
				}
				else if (nCode1 == EOF)
				{
					m_bEOF = true;
				}
			}

			// Считываем тэг 2D кодировки
			if (!m_bEOF && m_nK > 0)
			{
				m_bNextLine2D = !LookBits(1);
				SkipBits(1);
			}

			// Проверяем символ конца блока(end-of-block marker)
			if (m_bEndOfBlock && bEOL)
			{
				nCode1 = LookBits(12);
				if (nCode1 == 0x001)
				{
					SkipBits(12);
					if (m_nK > 0)
					{
						LookBits(1);
						SkipBits(1);
					}
					if (m_nK >= 0)
					{
						for (nIndex = 0; nIndex < 4; ++nIndex)
						{
							nCode1 = LookBits(12);
							if (nCode1 != 0x001)
							{
								// TO DO: Error "Bad RTC code in CCITTFax stream"
							}
							SkipBits(12);
							if (m_nK > 0)
							{
								LookBits(1);
								SkipBits(1);
							}
						}
					}
					m_bEOF = true;
				}
			}
			else if (bError && m_bEndOfLine)
			{
				do {
					if (nCode1 == EOF)
					{
						m_bEOF = true;
						return EOF;
					}
					SkipBits(1);
					nCode1 = LookBits(13);
				} while ((nCode1 >> 1) != 0x001);
				SkipBits(12);
				if (m_nK > 0)
				{
					SkipBits(1);
					m_bNextLine2D = !(nCode1 & 1);
				}
			}

			m_nCurPosCL = 0;
			m_nOutputBits = m_pCodingLine[1] - m_pCodingLine[0];
			if (m_nOutputBits == 0)
			{
				m_nCurPosCL = 1;
				m_nOutputBits = m_pCodingLine[2] - m_pCodingLine[1];
			}
			++m_mCurRow;
		}

		// Считываем один байт
		int nRet = 0;
		if (m_nOutputBits >= 8)
		{
			nRet = ((m_nCurPosCL & 1) == 0) ? 0xff : 0x00;
			if ((m_nOutputBits -= 8) == 0)
			{
				++m_nCurPosCL;
				if (m_pCodingLine[m_nCurPosCL] < m_nColumns)
				{
					m_nOutputBits = m_pCodingLine[m_nCurPosCL + 1] - m_pCodingLine[m_nCurPosCL];
				}
			}
		}
		else
		{
			int nBits = 8;
			nRet = 0;
			do {
				if (m_nOutputBits > nBits)
				{
					nIndex = nBits;
					nBits = 0;
					if ((m_nCurPosCL & 1) == 0)
					{
						nRet |= 0xff >> (8 - nIndex);
					}
					m_nOutputBits -= nIndex;
				}
				else
				{
					nIndex = m_nOutputBits;
					nBits -= m_nOutputBits;
					if ((m_nCurPosCL & 1) == 0)
					{
						nRet |= (0xff >> (8 - nIndex)) << nBits;
					}
					m_nOutputBits = 0;
					++m_nCurPosCL;
					if (m_pCodingLine[m_nCurPosCL] < m_nColumns)
					{
						m_nOutputBits = m_pCodingLine[m_nCurPosCL + 1] - m_pCodingLine[m_nCurPosCL];
					}
				}
			} while (nBits > 0 && m_pCodingLine[m_nCurPosCL] < m_nColumns);
		}
		m_nCharBuffer = m_bBlackIs1 ? (nRet ^ 0xff) : nRet;
		return m_nCharBuffer;
	}

	short CCITTFaxStream::Get2DCode()
	{
		CCITTCode *pCCITTCode;

		short nCode = 0;
		if (m_bEndOfBlock)
		{
			nCode = LookBits(7);
			pCCITTCode = &c_arrTable2D[nCode];
			if (pCCITTCode->nBitsCount > 0)
			{
				SkipBits(pCCITTCode->nBitsCount);
				return pCCITTCode->nCode;
			}
		}
		else
		{
			for (int nCount = 1; nCount <= 7; ++nCount)
			{
				nCode = LookBits(nCount);
				if (nCount < 7)
				{
					nCode <<= 7 - nCount;
				}
				pCCITTCode = &c_arrTable2D[nCode];
				if (pCCITTCode->nBitsCount == nCount)
				{
					SkipBits(nCount);
					return pCCITTCode->nCode;
				}
			}
		}
		// TO DO: Error "Bad 2D code in CCITTFax stream"
		return EOF;
	}

	short CCITTFaxStream::GetWhiteCode()
	{
		CCITTCode *pCCITTCode;

		short nCode = 0;
		if (m_bEndOfBlock)
		{
			nCode = LookBits(12);
			if ((nCode >> 5) == 0)
			{
				pCCITTCode = &c_arrWhiteTable1[nCode];
			}
			else
			{
				pCCITTCode = &c_arrWhiteTable2[nCode >> 3];
			}
			if (pCCITTCode->nBitsCount > 0)
			{
				SkipBits(pCCITTCode->nBitsCount);
				return pCCITTCode->nCode;
			}
		}
		else
		{
			for (int nCount = 1; nCount <= 9; ++nCount)
			{
				nCode = LookBits(nCount);
				if (nCount < 9)
				{
					nCode <<= 9 - nCount;
				}
				pCCITTCode = &c_arrWhiteTable2[nCode];
				if (pCCITTCode->nBitsCount == nCount)
				{
					SkipBits(nCount);
					return pCCITTCode->nCode;
				}
			}
			for (int nCount = 11; nCount <= 12; ++nCount)
			{
				nCode = LookBits(nCount);
				if (nCount < 12)
				{
					nCode <<= 12 - nCount;
				}
				pCCITTCode = &c_arrWhiteTable1[nCode];
				if (pCCITTCode->nBitsCount == nCount)
				{
					SkipBits(nCount);
					return pCCITTCode->nCode;
				}
			}
		}
		// TO DO: Error "Bad white code in CCITTFax stream"

		SkipBits(1);
		return 1;
	}

	short CCITTFaxStream::GetBlackCode()
	{
		CCITTCode *pCCITTCode;

		short nCode = 0;
		if (m_bEndOfBlock)
		{
			nCode = LookBits(13);
			if ((nCode >> 7) == 0)
			{
				pCCITTCode = &c_arrBlackTable1[nCode];
			}
			else if ((nCode >> 9) == 0)
			{
				pCCITTCode = &c_arrBlackTable2[(nCode >> 1) - 64];
			}
			else
			{
				pCCITTCode = &c_arrBlackTable3[nCode >> 7];
			}
			if (pCCITTCode->nBitsCount > 0)
			{
				SkipBits(pCCITTCode->nBitsCount);
				return pCCITTCode->nCode;
			}
		}
		else
		{
			for (int nCount = 2; nCount <= 6; ++nCount)
			{
				nCode = LookBits(nCount);
				if (nCount < 6)
				{
					nCode <<= 6 - nCount;
				}
				pCCITTCode = &c_arrBlackTable3[nCode];
				if (pCCITTCode->nBitsCount == nCount)
				{
					SkipBits(nCount);
					return pCCITTCode->nCode;
				}
			}
			for (int nCount = 7; nCount <= 12; ++nCount)
			{
				nCode = LookBits(nCount);
				if (nCount < 12)
				{
					nCode <<= 12 - nCount;
				}
				if (nCode >= 64)
				{
					pCCITTCode = &c_arrBlackTable2[nCode - 64];
					if (pCCITTCode->nBitsCount == nCount)
					{
						SkipBits(nCount);
						return pCCITTCode->nCode;
					}
				}
			}
			for (int nCount = 10; nCount <= 13; ++nCount)
			{
				nCode = LookBits(nCount);
				if (nCount < 13)
				{
					nCode <<= 13 - nCount;
				}
				pCCITTCode = &c_arrBlackTable1[nCode];
				if (pCCITTCode->nBitsCount == nCount)
				{
					SkipBits(nCount);
					return pCCITTCode->nCode;
				}
			}
		}
		// TO DO: Error "Bad black code in CCITTFax stream"

		SkipBits(1);
		return 1;
	}

	short CCITTFaxStream::LookBits(int nCount)
	{
		while (m_nInputBits < nCount)
		{
			int nChar = 0;
			if ((nChar = m_pStream->GetChar()) == EOF)
			{
				if (m_nInputBits == 0)
				{
					return EOF;
				}
				// Вблизи конца потока может оказаться, что запрашивается больше бит, чем 
				// их осталось в потоке. Нужно возратить корректное значение в данном случае.
				return (m_nInputBuffer << (nCount - m_nInputBits)) & (0xffff >> (16 - nCount));
			}
			m_nInputBuffer = (m_nInputBuffer << 8) + nChar;
			m_nInputBits += 8;
		}
		return (m_nInputBuffer >> (m_nInputBits - nCount)) & (0xffff >> (16 - nCount));
	}

	StringExt *CCITTFaxStream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		StringExt *seResult;
		char sTemp[50];

		if (nPSLevel < 2)
		{
			return NULL;
		}
		if (!(seResult = m_pStream->GetPSFilter(nPSLevel, sIndent)))
		{
			return NULL;
		}

		seResult->Append(sIndent)->Append("<< ");
		if (m_nK != 0)
		{
			sprintf(sTemp, "/K %d ", m_nK);
			seResult->Append(sTemp);
		}
		if (m_bEndOfLine)
		{
			seResult->Append("/EndOfLine true ");
		}
		if (m_bByteAlign)
		{
			seResult->Append("/EncodedByteAlign true ");
		}
		sprintf(sTemp, "/Columns %d ", m_nColumns);
		seResult->Append(sTemp);
		if (m_nRows != 0)
		{
			sprintf(sTemp, "/Rows %d ", m_nRows);
			seResult->Append(sTemp);
		}
		if (!m_bEndOfBlock)
		{
			seResult->Append("/EndOfBlock false ");
		}
		if (m_bBlackIs1)
		{
			seResult->Append("/BlackIs1 true ");
		}
		seResult->Append(">> /CCITTFaxDecode filter\n");
		return seResult;
	}

	bool CCITTFaxStream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(true);
	}

	//---------------------------------------------------------------------------------------------------------------
	// DCTStream
	//---------------------------------------------------------------------------------------------------------------

	// Константы для косинусного преобразования (20.12 fixed point format)
#define DCT_Cos_1_16    4017 // cos(pi/16)
#define DCT_Sin_1_16     799 // sin(pi/16)
#define DCT_Cos_3_16    3406 // cos(3*pi/16)
#define DCT_Sin_3_16    2276 // sin(3*pi/16)
#define DCT_Cos_6_16    1567 // cos(6*pi/16)
#define DCT_Sin_6_16    3784 // sin(6*pi/16)

#define DCT_Sqrt_2      5793 // sqrt(2)
#define DCT_Sqrt_2__2   2896 // sqrt(2) / 2

	// Константы для преобразования цветов (16.16 fixed point format)
#define DCT_CrToR   91881	//  1.4020
#define DCT_CbToG  -22553	// -0.3441363
#define DCT_CrToG  -46802	// -0.71413636
#define DCT_CbToB  116130	//  1.772

	// clip [-256,511] --> [0,255]
#define DCTClipOffset 256
	static unsigned char arrDCTClip[768];
	static int nDCTClipInit = 0;

	// zig zag 
	static int arrDCTZigZag[64] =
	{
		0,
		1, 8,
		16, 9, 2,
		3, 10, 17, 24,
		32, 25, 18, 11, 4,
		5, 12, 19, 26, 33, 40,
		48, 41, 34, 27, 20, 13, 6,
		7, 14, 21, 28, 35, 42, 49, 56,
		57, 50, 43, 36, 29, 22, 15,
		23, 30, 37, 44, 51, 58,
		59, 52, 45, 38, 31,
		39, 46, 53, 60,
		61, 54, 47,
		55, 62,
		63
	};

	DCTStream::DCTStream(Stream *pStream, int nColorTransform) :
		FilterStream(pStream)
	{
		m_nColorTransform = nColorTransform;
		m_bProgressive = m_bInterleaved = false;
		m_nWidth = m_nHeight = 0;
		m_nMCUWidth = m_nMCUHeight = 0;
		m_nComponentsCount = 0;
		m_nCurComponent = 0;
		m_nX = m_nY = m_nDY = 0;

		for (int nComp = 0; nComp < 4; ++nComp)
		{
			for (int nIndex = 0; nIndex < 32; ++nIndex)
			{
				m_pppRowBuffer[nComp][nIndex] = NULL;
			}
			m_ppFrameBuffer[nComp] = NULL;
		}

		if (!nDCTClipInit)
		{
			for (int nIndex = -256; nIndex < 0; ++nIndex)
				arrDCTClip[DCTClipOffset + nIndex] = 0;
			for (int nIndex = 0; nIndex < 256; ++nIndex)
				arrDCTClip[DCTClipOffset + nIndex] = nIndex;
			for (int nIndex = 256; nIndex < 512; ++nIndex)
				arrDCTClip[DCTClipOffset + nIndex] = 255;
			nDCTClipInit = 1;
		}
	}

	DCTStream::~DCTStream()
	{
		Close();
		delete m_pStream;
	}

	void DCTStream::Reset()
	{
		m_pStream->Reset();

		m_bProgressive = m_bInterleaved = false;
		m_nWidth = m_nHeight = 0;
		m_nComponentsCount = 0;
		m_nQuantTablesCount = 0;
		m_nDCHuffTablesCount = 0;
		m_nACHuffTablesCount = 0;
		m_bJFIFMarker = false;
		m_bAdobeMarker = false;
		m_nRestartInterval = 0;

		if (!ReadHeader())
		{
			m_nY = m_nHeight;
			return;
		}

		// Вычислим размеры MCU
		if (m_nComponentsCount == 1)
		{
			m_arrCompInfo[0].nXResolution = m_arrCompInfo[0].nYResolution = 1;
		}

		m_nMCUWidth = m_arrCompInfo[0].nXResolution;
		m_nMCUHeight = m_arrCompInfo[0].nYResolution;

		for (int nIndex = 1; nIndex < m_nComponentsCount; ++nIndex)
		{
			if (m_arrCompInfo[nIndex].nXResolution > m_nMCUWidth)
			{
				m_nMCUWidth = m_arrCompInfo[nIndex].nXResolution;
			}
			if (m_arrCompInfo[nIndex].nYResolution > m_nMCUHeight)
			{
				m_nMCUHeight = m_arrCompInfo[nIndex].nYResolution;
			}
		}
		m_nMCUWidth *= 8;
		m_nMCUHeight *= 8;

		if (m_nColorTransform == -1)
		{
			if (m_nComponentsCount == 3)
			{
				if (m_bJFIFMarker)
				{
					m_nColorTransform = 1;
				}
				else if (m_arrCompInfo[0].nID == 82 && m_arrCompInfo[1].nID == 71 && m_arrCompInfo[2].nID == 66) // ASCII "RGB"
				{
					m_nColorTransform = 0;
				}
				else
				{
					m_nColorTransform = 1;
				}
			}
			else
			{
				m_nColorTransform = 0;
			}
		}

		if (m_bProgressive || !m_bInterleaved)
		{

			// Выделяем память для всей картинки
			m_nBufferWidth = ((m_nWidth + m_nMCUWidth - 1) / m_nMCUWidth) * m_nMCUWidth;
			m_nBufferHeight = ((m_nHeight + m_nMCUHeight - 1) / m_nMCUHeight) * m_nMCUHeight;

			for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
			{
				m_ppFrameBuffer[nIndex] = (int *)MemUtilsMallocArray(m_nBufferWidth * m_nBufferHeight, sizeof(int));
				memset(m_ppFrameBuffer[nIndex], 0, m_nBufferWidth * m_nBufferHeight * sizeof(int));
			}

			// Считываем данные картинки
			do {
				m_nRestartMarker = 0xd0;
				Restart();
				ReadScan();
			} while (ReadHeader());

			// Декодируем
			DecodeImage();

			// Обнуляем счетчики
			m_nCurComponent = 0;
			m_nX = 0;
			m_nY = 0;

		}
		else
		{
			// Выделяем память под одну строку для MCU
			m_nBufferWidth = ((m_nWidth + m_nMCUWidth - 1) / m_nMCUWidth) * m_nMCUWidth;
			for (int nComp = 0; nComp < m_nComponentsCount; ++nComp)
			{
				for (int nY = 0; nY < m_nMCUHeight; ++nY)
				{
					m_pppRowBuffer[nComp][nY] = (unsigned char *)MemUtilsMallocArray(m_nBufferWidth, sizeof(unsigned char));
				}
			}

			// Обнуляем счетчики
			m_nCurComponent = 0;
			m_nX = 0;
			m_nY = 0;
			m_nDY = m_nMCUHeight;

			m_nRestartMarker = 0xd0;
			Restart();
		}
	}

	void DCTStream::Close()
	{
		for (int nComp = 0; nComp < 4; ++nComp)
		{
			for (int nY = 0; nY < 32; ++nY)
			{
				MemUtilsFree(m_pppRowBuffer[nComp][nY]);
				m_pppRowBuffer[nComp][nY] = NULL;
			}
			MemUtilsFree(m_ppFrameBuffer[nComp]);
			m_ppFrameBuffer[nComp] = NULL;
		}
		FilterStream::Close();
	}

	int DCTStream::GetChar()
	{
		int nChar = 0;
		if (m_nY >= m_nHeight)
		{
			return EOF;
		}
		if (m_bProgressive || !m_bInterleaved)
		{
			nChar = m_ppFrameBuffer[m_nCurComponent][m_nY * m_nBufferWidth + m_nX];
			if (++m_nCurComponent == m_nComponentsCount)
			{
				m_nCurComponent = 0;
				if (++m_nX == m_nWidth)
				{
					m_nX = 0;
					++m_nY;
				}
			}
		}
		else
		{
			if (m_nDY >= m_nMCUHeight)
			{
				if (!ReadMCURow())
				{
					m_nY = m_nHeight;
					return EOF;
				}
				m_nCurComponent = 0;
				m_nX = 0;
				m_nDY = 0;
			}
			nChar = m_pppRowBuffer[m_nCurComponent][m_nDY][m_nX];
			if (++m_nCurComponent == m_nComponentsCount)
			{
				m_nCurComponent = 0;
				if (++m_nX == m_nWidth)
				{
					m_nX = 0;
					++m_nY;
					++m_nDY;
					if (m_nY == m_nHeight)
					{
						ReadTrailer();
					}
				}
			}
		}
		return nChar;
	}

	int DCTStream::LookChar()
	{
		if (m_nY >= m_nHeight)
		{
			return EOF;
		}
		if (m_bProgressive || !m_bInterleaved)
		{
			return m_ppFrameBuffer[m_nCurComponent][m_nY * m_nBufferWidth + m_nX];
		}
		else
		{
			if (m_nDY >= m_nMCUHeight)
			{
				if (!ReadMCURow())
				{
					m_nY = m_nHeight;
					return EOF;
				}
				m_nCurComponent = 0;
				m_nX = 0;
				m_nDY = 0;
			}
			return m_pppRowBuffer[m_nCurComponent][m_nDY][m_nX];
		}
	}

	void DCTStream::Restart()
	{
		m_nInputBits = 0;
		m_nRestartCtr = m_nRestartInterval;

		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
			m_arrCompInfo[nIndex].nPrevDC = 0;

		m_nEOBRun = 0;
	}

	// Считываем одну строку из MCUs из соответствующего потока JPEG.
	bool DCTStream::ReadMCURow()
	{
		int arrRawData[64];
		unsigned char arrTransformData[64];
		unsigned char *p1, *p2;
		int x1, x2, y2, x3, y3, x4, y4, x5, y5, i;

		for (x1 = 0; x1 < m_nWidth; x1 += m_nMCUWidth)
		{
			// Обрабатываем restart marker
			if (m_nRestartInterval > 0 && m_nRestartCtr == 0)
			{
				int nChar = ReadMarker();
				if (nChar != m_nRestartMarker)
				{
					// TO DO: Error "Bad DCT data: incorrect restart marker"
					return false;
				}
				if (++m_nRestartMarker == 0xd8)
					m_nRestartMarker = 0xd0;
				Restart();
			}

			// Считываем один MCU
			for (int nComp = 0; nComp < m_nComponentsCount; ++nComp)
			{
				int nXRes = m_arrCompInfo[nComp].nXResolution;
				int nYRes = m_arrCompInfo[nComp].nYResolution;
				int nHoriz = m_nMCUWidth / nXRes;
				int nVert = m_nMCUHeight / nYRes;
				int nHorizSub = nHoriz / 8;
				int nVertSub = nVert / 8;
				for (y2 = 0; y2 < m_nMCUHeight; y2 += nVert)
				{
					for (x2 = 0; x2 < m_nMCUWidth; x2 += nHoriz)
					{
						if (!ReadDataUnit(&m_arrDCHuffTables[m_oCurScanInfo.arrDCHuffTable[nComp]], &m_arrACHuffTables[m_oCurScanInfo.arrACHuffTable[nComp]], &m_arrCompInfo[nComp].nPrevDC, arrRawData))
							return false;

						TransformDataUnit(m_arrQuantTables[m_arrCompInfo[nComp].nQuantTableNum], arrRawData, arrTransformData);
						if (nHorizSub == 1 && nVertSub == 1)
						{
							for (y3 = 0, i = 0; y3 < 8; ++y3, i += 8)
							{
								p1 = &m_pppRowBuffer[nComp][y2 + y3][x1 + x2];
								p1[0] = arrTransformData[i + 0];
								p1[1] = arrTransformData[i + 1];
								p1[2] = arrTransformData[i + 2];
								p1[3] = arrTransformData[i + 3];
								p1[4] = arrTransformData[i + 4];
								p1[5] = arrTransformData[i + 5];
								p1[6] = arrTransformData[i + 6];
								p1[7] = arrTransformData[i + 7];
							}
						}
						else if (nHorizSub == 2 && nVertSub == 2)
						{
							for (y3 = 0, i = 0; y3 < 16; y3 += 2, i += 8)
							{
								p1 = &m_pppRowBuffer[nComp][y2 + y3][x1 + x2];
								p2 = &m_pppRowBuffer[nComp][y2 + y3 + 1][x1 + x2];
								p1[0]  = p1[1]  = p2[0]  = p2[1]  = arrTransformData[i + 0];
								p1[2]  = p1[3]  = p2[2]  = p2[3]  = arrTransformData[i + 1];
								p1[4]  = p1[5]  = p2[4]  = p2[5]  = arrTransformData[i + 2];
								p1[6]  = p1[7]  = p2[6]  = p2[7]  = arrTransformData[i + 3];
								p1[8]  = p1[9]  = p2[8]  = p2[9]  = arrTransformData[i + 4];
								p1[10] = p1[11] = p2[10] = p2[11] = arrTransformData[i + 5];
								p1[12] = p1[13] = p2[12] = p2[13] = arrTransformData[i + 6];
								p1[14] = p1[15] = p2[14] = p2[15] = arrTransformData[i + 7];
							}
						}
						else
						{
							i = 0;
							for (y3 = 0, y4 = 0; y3 < 8; ++y3, y4 += nVertSub)
							{
								for (x3 = 0, x4 = 0; x3 < 8; ++x3, x4 += nHorizSub)
								{
									for (y5 = 0; y5 < nVertSub; ++y5)
										for (x5 = 0; x5 < nHorizSub; ++x5)
											m_pppRowBuffer[nComp][y2 + y4 + y5][x1 + x2 + x4 + x5] = arrTransformData[i];
									++i;
								}
							}
						}
					}
				}
			}
			--m_nRestartCtr;

			// Преобазуем пространство цветов 
			if (m_nColorTransform)
			{
				// YCbCr -> RGB
				if (m_nComponentsCount == 3)
				{
					for (y2 = 0; y2 < m_nMCUHeight; ++y2)
					{
						for (x2 = 0; x2 < m_nMCUWidth; ++x2)
						{
							int nY = m_pppRowBuffer[0][y2][x1 + x2];
							int nCb = m_pppRowBuffer[1][y2][x1 + x2] - 128;
							int nCr = m_pppRowBuffer[2][y2][x1 + x2] - 128;
							int nR = ((nY << 16) + DCT_CrToR * nCr + 32768) >> 16;
							m_pppRowBuffer[0][y2][x1 + x2] = arrDCTClip[DCTClipOffset + nR];
							int nG = ((nY << 16) + DCT_CbToG * nCb + DCT_CrToG * nCr + 32768) >> 16;
							m_pppRowBuffer[1][y2][x1 + x2] = arrDCTClip[DCTClipOffset + nG];
							int nB = ((nY << 16) + DCT_CbToB * nCb + 32768) >> 16;
							m_pppRowBuffer[2][y2][x1 + x2] = arrDCTClip[DCTClipOffset + nB];
						}
					}
				}
				else if (m_nComponentsCount == 4) // YCbCrK -> CMYK (K оставляем неизменненным)
				{
					for (y2 = 0; y2 < m_nMCUHeight; ++y2)
					{
						for (x2 = 0; x2 < m_nMCUWidth; ++x2)
						{
							int nY = m_pppRowBuffer[0][y2][x1 + x2];
							int nCb = m_pppRowBuffer[1][y2][x1 + x2] - 128;
							int nCr = m_pppRowBuffer[2][y2][x1 + x2] - 128;
							int nR = ((nY << 16) + DCT_CrToR * nCr + 32768) >> 16;
							m_pppRowBuffer[0][y2][x1 + x2] = 255 - arrDCTClip[DCTClipOffset + nR];
							int nG = ((nY << 16) + DCT_CbToG * nCb + DCT_CrToG * nCr + 32768) >> 16;
							m_pppRowBuffer[1][y2][x1 + x2] = 255 - arrDCTClip[DCTClipOffset + nG];
							int nB = ((nY << 16) + DCT_CbToB * nCb + 32768) >> 16;
							m_pppRowBuffer[2][y2][x1 + x2] = 255 - arrDCTClip[DCTClipOffset + nB];
						}
					}
				}
			}
		}
		return true;
	}

	// Только для progressive или не interleaved JPEG.
	void DCTStream::ReadScan()
	{
		int arrData[64];
		int x1, y1, dx1, dy1, x2, y2, y3, nComponent = 0, i;
		int *pBuffer;

		if (m_oCurScanInfo.nComponentsCount == 1)
		{
			for (nComponent = 0; nComponent < m_nComponentsCount; ++nComponent)
			{
				if (m_oCurScanInfo.arrbComponent[nComponent])
				{
					break;
				}
			}
			dx1 = m_nMCUWidth / m_arrCompInfo[nComponent].nXResolution;
			dy1 = m_nMCUHeight / m_arrCompInfo[nComponent].nYResolution;
		}
		else
		{
			dx1 = m_nMCUWidth;
			dy1 = m_nMCUHeight;
		}

		for (y1 = 0; y1 < m_nHeight; y1 += dy1)
		{
			for (x1 = 0; x1 < m_nWidth; x1 += dx1)
			{

				// Обрабатываем restart marker
				if (m_nRestartInterval > 0 && m_nRestartCtr == 0)
				{
					int nChar = ReadMarker();
					if (nChar != m_nRestartMarker)
					{
						// TO DO: Error "Bad DCT data: incorrect restart marker"
						return;
					}
					if (++m_nRestartMarker == 0xd8)
					{
						m_nRestartMarker = 0xd0;
					}
					Restart();
				}

				// Считываем один MCU
				for (nComponent = 0; nComponent < m_nComponentsCount; ++nComponent)
				{
					if (!m_oCurScanInfo.arrbComponent[nComponent])
					{
						continue;
					}

					int nXRes = m_arrCompInfo[nComponent].nXResolution;
					int nYRes = m_arrCompInfo[nComponent].nYResolution;
					int nHoriz = m_nMCUWidth / nXRes;
					int nVert  = m_nMCUHeight / nYRes;
					int nVertSub = nVert / 8;
					for (y2 = 0; y2 < dy1; y2 += nVert)
					{
						for (x2 = 0; x2 < dx1; x2 += nHoriz)
						{
							pBuffer = &m_ppFrameBuffer[nComponent][(y1 + y2) * m_nBufferWidth + (x1 + x2)];
							for (y3 = 0, i = 0; y3 < 8; ++y3, i += 8)
							{
								arrData[i + 0] = pBuffer[0];
								arrData[i + 1] = pBuffer[1];
								arrData[i + 2] = pBuffer[2];
								arrData[i + 3] = pBuffer[3];
								arrData[i + 4] = pBuffer[4];
								arrData[i + 5] = pBuffer[5];
								arrData[i + 6] = pBuffer[6];
								arrData[i + 7] = pBuffer[7];
								pBuffer += m_nBufferWidth * nVertSub;
							}

							if (m_bProgressive)
							{
								if (!ReadProgressiveDataUnit(&m_arrDCHuffTables[m_oCurScanInfo.arrDCHuffTable[nComponent]], &m_arrACHuffTables[m_oCurScanInfo.arrACHuffTable[nComponent]], &m_arrCompInfo[nComponent].nPrevDC, arrData))
								{
									return;
								}
							}
							else
							{
								if (!ReadDataUnit(&m_arrDCHuffTables[m_oCurScanInfo.arrDCHuffTable[nComponent]], &m_arrACHuffTables[m_oCurScanInfo.arrACHuffTable[nComponent]], &m_arrCompInfo[nComponent].nPrevDC, arrData))
								{
									return;
								}
							}

							pBuffer = &m_ppFrameBuffer[nComponent][(y1 + y2) * m_nBufferWidth + (x1 + x2)];
							for (y3 = 0, i = 0; y3 < 8; ++y3, i += 8)
							{
								pBuffer[0] = arrData[i + 0];
								pBuffer[1] = arrData[i + 1];
								pBuffer[2] = arrData[i + 2];
								pBuffer[3] = arrData[i + 3];
								pBuffer[4] = arrData[i + 4];
								pBuffer[5] = arrData[i + 5];
								pBuffer[6] = arrData[i + 6];
								pBuffer[7] = arrData[i + 7];
								pBuffer += m_nBufferWidth * nVertSub;
							}
						}
					}
				}
				--m_nRestartCtr;
			}
		}
	}

	bool DCTStream::ReadDataUnit(DCTHuffTable *pDCHuffTable, DCTHuffTable *pACHuffTable, int *pnPrevDC, int arrData[64])
	{
		int nSize = 0, nAmp = 0;

		if ((nSize = ReadHuffSymbol(pDCHuffTable)) == 9999)
		{
			return false;
		}
		if (nSize > 0)
		{
			if ((nAmp = ReadAmp(nSize)) == 9999)
			{
				return false;
			}
		}
		else
		{
			nAmp = 0;
		}
		arrData[0] = *pnPrevDC += nAmp;
		for (int nI = 1; nI < 64; ++nI)
		{
			arrData[nI] = 0;
		}
		int nIndex = 1;
		while (nIndex < 64)
		{
			int nRun = 0;
			int nSymbol = 0;
			while ((nSymbol = ReadHuffSymbol(pACHuffTable)) == 0xf0 && nRun < 0x30)
			{
				nRun += 0x10;
			}
			if (nSymbol == 9999)
			{
				return false;
			}
			if (nSymbol == 0x00)
			{
				break;
			}
			else
			{
				nRun += (nSymbol >> 4) & 0x0f;
				nSize = nSymbol & 0x0f;
				nAmp = ReadAmp(nSize);
				if (nAmp == 9999)
				{
					return false;
				}
				nIndex += nRun;
				if (nIndex < 64)
				{
					int nNewIndex = arrDCTZigZag[nIndex++];
					arrData[nNewIndex] = nAmp;
				}
			}
		}
		return true;
	}

	bool DCTStream::ReadProgressiveDataUnit(DCTHuffTable *pDCHuffTable, DCTHuffTable *pACHuffTable, int *pnPrevDC, int arrData[64])
	{
		int nRun = 0, nSize = 0, nAmp = 0, nBit = 0;

		// DC кoэффициенты
		int nKoef = m_oCurScanInfo.nFirstKoef;
		if (nKoef == 0)
		{
			if (m_oCurScanInfo.nApproxH == 0)
			{
				if ((nSize = ReadHuffSymbol(pDCHuffTable)) == 9999)
				{
					return false;
				}
				if (nSize > 0)
				{
					if ((nAmp = ReadAmp(nSize)) == 9999)
					{
						return false;
					}
				}
				else
				{
					nAmp = 0;
				}
				arrData[0] += (*pnPrevDC += nAmp) << m_oCurScanInfo.nApproxL;
			}
			else
			{
				if ((nBit = ReadBit()) == 9999)
				{
					return false;
				}
				arrData[0] += nBit << m_oCurScanInfo.nApproxL;
			}
			++nKoef;
		}
		if (m_oCurScanInfo.nLastKoef == 0)
		{
			return true;
		}

		// Проверяем группу EOB
		if (m_nEOBRun > 0)
		{
			while (nKoef <= m_oCurScanInfo.nLastKoef)
			{
				int nNewIndex = arrDCTZigZag[nKoef++];
				if (arrData[nNewIndex] != 0)
				{
					if ((nBit = ReadBit()) == EOF)
					{
						return false;
					}
					if (nBit)
					{
						arrData[nNewIndex] += 1 << m_oCurScanInfo.nApproxL;
					}
				}
			}
			--m_nEOBRun;
			return true;
		}

		int nK = 0;
		// считываем коэффициенты AC
		while (nKoef <= m_oCurScanInfo.nLastKoef)
		{
			int nSymbol = 0;
			if ((nSymbol = ReadHuffSymbol(pACHuffTable)) == 9999)
			{
				return false;
			}

			// ZRL
			if (nSymbol == 0xf0)
			{
				nK = 0;
				while (nK < 16)
				{
					int nNewIndex = arrDCTZigZag[nKoef++];
					if (arrData[nNewIndex] == 0)
					{
						++nK;
					}
					else
					{
						if ((nBit = ReadBit()) == EOF)
						{
							return false;
						}
						if (nBit)
						{
							arrData[nNewIndex] += 1 << m_oCurScanInfo.nApproxL;
						}
					}
				}
			}
			else if ((nSymbol & 0x0f) == 0x00) // EOB run
			{
				int nTemp = nSymbol >> 4;
				m_nEOBRun = 0;
				for (nK = 0; nK < nTemp; ++nK)
				{
					if ((nBit = ReadBit()) == EOF)
					{
						return false;
					}
					m_nEOBRun = (m_nEOBRun << 1) | nBit;
				}
				m_nEOBRun += 1 << nTemp;
				while (nKoef <= m_oCurScanInfo.nLastKoef)
				{
					int nNewIndex = arrDCTZigZag[nKoef++];
					if (arrData[nNewIndex] != 0)
					{
						if ((nBit = ReadBit()) == EOF)
						{
							return false;
						}
						if (nBit)
						{
							arrData[nNewIndex] += 1 << m_oCurScanInfo.nApproxL;
						}
					}
				}
				--m_nEOBRun;
				break;
			}
			else
			{
				nRun = (nSymbol >> 4) & 0x0f;
				nSize = nSymbol & 0x0f;
				if ((nAmp = ReadAmp(nSize)) == 9999)
				{
					return false;
				}
				nK = 0;
				int nNewIndex = 0;
				do {
					nNewIndex = arrDCTZigZag[nKoef++];
					while (arrData[nNewIndex] != 0)
					{
						if ((nBit = ReadBit()) == EOF)
						{
							return false;
						}
						if (nBit)
						{
							arrData[nNewIndex] += 1 << m_oCurScanInfo.nApproxL;
						}
						nNewIndex = arrDCTZigZag[nKoef++];
					}
					++nK;
				} while (nK <= nRun);
				arrData[nNewIndex] = nAmp << m_oCurScanInfo.nApproxL;
			}
		}

		return true;
	}

	void DCTStream::DecodeImage()
	{
		int arrDataIn[64];
		unsigned char arrDataOut[64];
		unsigned short *pQuantTable;
		int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, i;

		int *p0, *p1, *p2;

		for (y1 = 0; y1 < m_nBufferHeight; y1 += m_nMCUHeight)
		{
			for (x1 = 0; x1 < m_nBufferWidth; x1 += m_nMCUWidth)
			{
				for (int nComp = 0; nComp < m_nComponentsCount; ++nComp)
				{
					pQuantTable = m_arrQuantTables[m_arrCompInfo[nComp].nQuantTableNum];
					int nXRes = m_arrCompInfo[nComp].nXResolution;
					int nYRes = m_arrCompInfo[nComp].nYResolution;
					int nHoriz = m_nMCUWidth / nXRes;
					int nVert = m_nMCUHeight / nYRes;
					int nHorizSub = nHoriz / 8;
					int nVertSub = nVert / 8;
					for (y2 = 0; y2 < m_nMCUHeight; y2 += nVert)
					{
						for (x2 = 0; x2 < m_nMCUWidth; x2 += nHoriz)
						{
							p1 = &m_ppFrameBuffer[nComp][(y1 + y2) * m_nBufferWidth + (x1 + x2)];
							for (y3 = 0, i = 0; y3 < 8; ++y3, i += 8)
							{
								arrDataIn[i + 0] = p1[0];
								arrDataIn[i + 1] = p1[1];
								arrDataIn[i + 2] = p1[2];
								arrDataIn[i + 3] = p1[3];
								arrDataIn[i + 4] = p1[4];
								arrDataIn[i + 5] = p1[5];
								arrDataIn[i + 6] = p1[6];
								arrDataIn[i + 7] = p1[7];
								p1 += m_nBufferWidth * nVertSub;
							}

							TransformDataUnit(pQuantTable, arrDataIn, arrDataOut);

							p1 = &m_ppFrameBuffer[nComp][(y1 + y2) * m_nBufferWidth + (x1 + x2)];
							if (nHorizSub == 1 && nVertSub == 1)
							{
								for (y3 = 0, i = 0; y3 < 8; ++y3, i += 8)
								{
									p1[0] = arrDataOut[i + 0] & 0xff;
									p1[1] = arrDataOut[i + 1] & 0xff;
									p1[2] = arrDataOut[i + 2] & 0xff;
									p1[3] = arrDataOut[i + 3] & 0xff;
									p1[4] = arrDataOut[i + 4] & 0xff;
									p1[5] = arrDataOut[i + 5] & 0xff;
									p1[6] = arrDataOut[i + 6] & 0xff;
									p1[7] = arrDataOut[i + 7] & 0xff;
									p1 += m_nBufferWidth;
								}
							}
							else if (nHorizSub == 2 && nVertSub == 2)
							{
								p2 = p1 + m_nBufferWidth;
								for (y3 = 0, i = 0; y3 < 16; y3 += 2, i += 8)
								{
									p1[0]  = p1[1]  = p2[0]  = p2[1]  = arrDataOut[i + 0] & 0xff;
									p1[2]  = p1[3]  = p2[2]  = p2[3]  = arrDataOut[i + 1] & 0xff;
									p1[4]  = p1[5]  = p2[4]  = p2[5]  = arrDataOut[i + 2] & 0xff;
									p1[6]  = p1[7]  = p2[6]  = p2[7]  = arrDataOut[i + 3] & 0xff;
									p1[8]  = p1[9]  = p2[8]  = p2[9]  = arrDataOut[i + 4] & 0xff;
									p1[10] = p1[11] = p2[10] = p2[11] = arrDataOut[i + 5] & 0xff;
									p1[12] = p1[13] = p2[12] = p2[13] = arrDataOut[i + 6] & 0xff;
									p1[14] = p1[15] = p2[14] = p2[15] = arrDataOut[i + 7] & 0xff;
									p1 += m_nBufferWidth * 2;
									p2 += m_nBufferWidth * 2;
								}
							}
							else
							{
								i = 0;
								for (y3 = 0, y4 = 0; y3 < 8; ++y3, y4 += nVertSub)
								{
									for (x3 = 0, x4 = 0; x3 < 8; ++x3, x4 += nHorizSub)
									{
										p2 = p1 + x4;
										for (y5 = 0; y5 < nVertSub; ++y5)
										{
											for (x5 = 0; x5 < nHorizSub; ++x5)
											{
												p2[x5] = arrDataOut[i] & 0xff;
											}
											p2 += m_nBufferWidth;
										}
										++i;
									}
									p1 += m_nBufferWidth * nVertSub;
								}
							}
						}
					}
				}

				// Преобразование пространства цветов
				if (m_nColorTransform)
				{
					// YCbCr -> RGB
					if (m_nComponentsCount == 3)
					{
						for (y2 = 0; y2 < m_nMCUHeight; ++y2)
						{
							p0 = &m_ppFrameBuffer[0][(y1 + y2) * m_nBufferWidth + x1];
							p1 = &m_ppFrameBuffer[1][(y1 + y2) * m_nBufferWidth + x1];
							p2 = &m_ppFrameBuffer[2][(y1 + y2) * m_nBufferWidth + x1];
							for (x2 = 0; x2 < m_nMCUWidth; ++x2)
							{
								int nY = *p0;
								int nCb = *p1 - 128;
								int nCr = *p2 - 128;
								int nR = ((nY << 16) + DCT_CrToR * nCr + 32768) >> 16;
								*p0++ = arrDCTClip[DCTClipOffset + nR];
								int nG = ((nY << 16) + DCT_CbToG * nCb + DCT_CrToG * nCr + 32768) >> 16;
								*p1++ = arrDCTClip[DCTClipOffset + nG];
								int nB = ((nY << 16) + DCT_CbToB * nCb + 32768) >> 16;
								*p2++ = arrDCTClip[DCTClipOffset + nB];
							}
						}
					}
					else if (m_nComponentsCount == 4) // YCbCrK -> CMYK (K оставляем неизмененным)
					{
						for (y2 = 0; y2 < m_nMCUHeight; ++y2)
						{
							p0 = &m_ppFrameBuffer[0][(y1 + y2) * m_nBufferWidth + x1];
							p1 = &m_ppFrameBuffer[1][(y1 + y2) * m_nBufferWidth + x1];
							p2 = &m_ppFrameBuffer[2][(y1 + y2) * m_nBufferWidth + x1];
							for (x2 = 0; x2 < m_nMCUWidth; ++x2)
							{
								int nY = *p0;
								int nCb = *p1 - 128;
								int nCr = *p2 - 128;
								int nR = ((nY << 16) + DCT_CrToR * nCr + 32768) >> 16;
								*p0++ = 255 - arrDCTClip[DCTClipOffset + nR];
								int nG = ((nY << 16) + DCT_CbToG * nCb + DCT_CrToG * nCr + 32768) >> 16;
								*p1++ = 255 - arrDCTClip[DCTClipOffset + nG];
								int nB = ((nY << 16) + DCT_CbToB * nCb + 32768) >> 16;
								*p2++ = 255 - arrDCTClip[DCTClipOffset + nB];
							}
						}
					}
				}
			}
		}
	}

	void DCTStream::TransformDataUnit(unsigned short *pQuantTable, int arrDataIn[64], unsigned char arrDataOut[64])
	{
		int arrBuffer[8] ={ 0, 0, 0, 0, 0, 0, 0, 0 };
		int nTemp = 0;
		int *pCurLine = NULL;

		// Деквантизация
		for (int nIndex = 0; nIndex < 64; ++nIndex)
		{
			arrDataIn[nIndex] *= pQuantTable[nIndex];
		}

		// Обратное синусно-косинусное преобразования (по строкам)
		for (int nIndex = 0; nIndex < 64; nIndex += 8)
		{
			pCurLine = arrDataIn + nIndex;

			// Проверяем случай, когда все коэффициенты равны 0
			if (pCurLine[1] == 0 && pCurLine[2] == 0 && pCurLine[3] == 0 && pCurLine[4] == 0 && pCurLine[5] == 0 && pCurLine[6] == 0 && pCurLine[7] == 0)
			{
				nTemp = (DCT_Sqrt_2 * pCurLine[0] + 512) >> 10;
				pCurLine[0] = nTemp;
				pCurLine[1] = nTemp;
				pCurLine[2] = nTemp;
				pCurLine[3] = nTemp;
				pCurLine[4] = nTemp;
				pCurLine[5] = nTemp;
				pCurLine[6] = nTemp;
				pCurLine[7] = nTemp;
				continue;
			}

			// Шаг 4 (Сдвиг на 128)
			arrBuffer[0] = (DCT_Sqrt_2 * pCurLine[0] + 128) >> 8;
			arrBuffer[1] = (DCT_Sqrt_2 * pCurLine[4] + 128) >> 8;
			arrBuffer[2] = pCurLine[2];
			arrBuffer[3] = pCurLine[6];
			arrBuffer[4] = (DCT_Sqrt_2__2 * (pCurLine[1] - pCurLine[7]) + 128) >> 8;
			arrBuffer[7] = (DCT_Sqrt_2__2 * (pCurLine[1] + pCurLine[7]) + 128) >> 8;
			arrBuffer[5] = pCurLine[3] << 4;
			arrBuffer[6] = pCurLine[5] << 4;

			// Шаг 3
			nTemp = (arrBuffer[0] - arrBuffer[1] + 1) >> 1;
			arrBuffer[0] = (arrBuffer[0] + arrBuffer[1] + 1) >> 1;
			arrBuffer[1] = nTemp;
			nTemp = (arrBuffer[2] * DCT_Sin_6_16 + arrBuffer[3] * DCT_Cos_6_16 + 128) >> 8;
			arrBuffer[2] = (arrBuffer[2] * DCT_Cos_6_16 - arrBuffer[3] * DCT_Sin_6_16 + 128) >> 8;
			arrBuffer[3] = nTemp;
			nTemp = (arrBuffer[4] - arrBuffer[6] + 1) >> 1;
			arrBuffer[4] = (arrBuffer[4] + arrBuffer[6] + 1) >> 1;
			arrBuffer[6] = nTemp;
			nTemp = (arrBuffer[7] + arrBuffer[5] + 1) >> 1;
			arrBuffer[5] = (arrBuffer[7] - arrBuffer[5] + 1) >> 1;
			arrBuffer[7] = nTemp;

			// Шаг 2
			nTemp = (arrBuffer[0] - arrBuffer[3] + 1) >> 1;
			arrBuffer[0] = (arrBuffer[0] + arrBuffer[3] + 1) >> 1;
			arrBuffer[3] = nTemp;
			nTemp = (arrBuffer[1] - arrBuffer[2] + 1) >> 1;
			arrBuffer[1] = (arrBuffer[1] + arrBuffer[2] + 1) >> 1;
			arrBuffer[2] = nTemp;
			nTemp = (arrBuffer[4] * DCT_Sin_3_16 + arrBuffer[7] * DCT_Cos_3_16 + 2048) >> 12;
			arrBuffer[4] = (arrBuffer[4] * DCT_Cos_3_16 - arrBuffer[7] * DCT_Sin_3_16 + 2048) >> 12;
			arrBuffer[7] = nTemp;
			nTemp = (arrBuffer[5] * DCT_Sin_1_16 + arrBuffer[6] * DCT_Cos_1_16 + 2048) >> 12;
			arrBuffer[5] = (arrBuffer[5] * DCT_Cos_1_16 - arrBuffer[6] * DCT_Sin_1_16 + 2048) >> 12;
			arrBuffer[6] = nTemp;

			// Шаг 1
			pCurLine[0] = arrBuffer[0] + arrBuffer[7];
			pCurLine[7] = arrBuffer[0] - arrBuffer[7];
			pCurLine[1] = arrBuffer[1] + arrBuffer[6];
			pCurLine[6] = arrBuffer[1] - arrBuffer[6];
			pCurLine[2] = arrBuffer[2] + arrBuffer[5];
			pCurLine[5] = arrBuffer[2] - arrBuffer[5];
			pCurLine[3] = arrBuffer[3] + arrBuffer[4];
			pCurLine[4] = arrBuffer[3] - arrBuffer[4];
		}

		// Обратное синусно-косинусное преобразования (по столбцам)
		for (int nIndex = 0; nIndex < 8; ++nIndex)
		{
			pCurLine = arrDataIn + nIndex;

			// проверяем все ли коэффициенты нулевые
			if (pCurLine[1 * 8] == 0 && pCurLine[2 * 8] == 0 && pCurLine[3 * 8] == 0 && pCurLine[4 * 8] == 0 && pCurLine[5 * 8] == 0 && pCurLine[6 * 8] == 0 && pCurLine[7 * 8] == 0)
			{
				nTemp = (DCT_Sqrt_2 * arrDataIn[nIndex + 0] + 8192) >> 14;
				pCurLine[0 * 8] = nTemp;
				pCurLine[1 * 8] = nTemp;
				pCurLine[2 * 8] = nTemp;
				pCurLine[3 * 8] = nTemp;
				pCurLine[4 * 8] = nTemp;
				pCurLine[5 * 8] = nTemp;
				pCurLine[6 * 8] = nTemp;
				pCurLine[7 * 8] = nTemp;
				continue;
			}

			// Шаг 4
			arrBuffer[0] = (DCT_Sqrt_2 * pCurLine[0 * 8] + 2048) >> 12;
			arrBuffer[1] = (DCT_Sqrt_2 * pCurLine[4 * 8] + 2048) >> 12;
			arrBuffer[2] = pCurLine[2 * 8];
			arrBuffer[3] = pCurLine[6 * 8];
			arrBuffer[4] = (DCT_Sqrt_2__2 * (pCurLine[1 * 8] - pCurLine[7 * 8]) + 2048) >> 12;
			arrBuffer[7] = (DCT_Sqrt_2__2 * (pCurLine[1 * 8] + pCurLine[7 * 8]) + 2048) >> 12;
			arrBuffer[5] = pCurLine[3 * 8];
			arrBuffer[6] = pCurLine[5 * 8];

			// Шаг 3
			nTemp = (arrBuffer[0] - arrBuffer[1] + 1) >> 1;
			arrBuffer[0] = (arrBuffer[0] + arrBuffer[1] + 1) >> 1;
			arrBuffer[1] = nTemp;
			nTemp = (arrBuffer[2] * DCT_Sin_6_16 + arrBuffer[3] * DCT_Cos_6_16 + 2048) >> 12;
			arrBuffer[2] = (arrBuffer[2] * DCT_Cos_6_16 - arrBuffer[3] * DCT_Sin_6_16 + 2048) >> 12;
			arrBuffer[3] = nTemp;
			nTemp = (arrBuffer[4] - arrBuffer[6] + 1) >> 1;
			arrBuffer[4] = (arrBuffer[4] + arrBuffer[6] + 1) >> 1;
			arrBuffer[6] = nTemp;
			nTemp = (arrBuffer[7] + arrBuffer[5] + 1) >> 1;
			arrBuffer[5] = (arrBuffer[7] - arrBuffer[5] + 1) >> 1;
			arrBuffer[7] = nTemp;

			// Шаг 2
			nTemp = (arrBuffer[0] - arrBuffer[3] + 1) >> 1;
			arrBuffer[0] = (arrBuffer[0] + arrBuffer[3] + 1) >> 1;
			arrBuffer[3] = nTemp;
			nTemp = (arrBuffer[1] - arrBuffer[2] + 1) >> 1;
			arrBuffer[1] = (arrBuffer[1] + arrBuffer[2] + 1) >> 1;
			arrBuffer[2] = nTemp;
			nTemp = (arrBuffer[4] * DCT_Sin_3_16 + arrBuffer[7] * DCT_Cos_3_16 + 2048) >> 12;
			arrBuffer[4] = (arrBuffer[4] * DCT_Cos_3_16 - arrBuffer[7] * DCT_Sin_3_16 + 2048) >> 12;
			arrBuffer[7] = nTemp;
			nTemp = (arrBuffer[5] * DCT_Sin_1_16 + arrBuffer[6] * DCT_Cos_1_16 + 2048) >> 12;
			arrBuffer[5] = (arrBuffer[5] * DCT_Cos_1_16 - arrBuffer[6] * DCT_Sin_1_16 + 2048) >> 12;
			arrBuffer[6] = nTemp;

			// Шаг 1
			pCurLine[0 * 8] = arrBuffer[0] + arrBuffer[7];
			pCurLine[7 * 8] = arrBuffer[0] - arrBuffer[7];
			pCurLine[1 * 8] = arrBuffer[1] + arrBuffer[6];
			pCurLine[6 * 8] = arrBuffer[1] - arrBuffer[6];
			pCurLine[2 * 8] = arrBuffer[2] + arrBuffer[5];
			pCurLine[5 * 8] = arrBuffer[2] - arrBuffer[5];
			pCurLine[3 * 8] = arrBuffer[3] + arrBuffer[4];
			pCurLine[4 * 8] = arrBuffer[3] - arrBuffer[4];
		}

		// convert to 8-bit integers
		for (int nIndex = 0; nIndex < 64; ++nIndex)
		{
			arrDataOut[nIndex] = arrDCTClip[DCTClipOffset + 128 + ((arrDataIn[nIndex] + 8) >> 4)];
		}
	}

	int DCTStream::ReadHuffSymbol(DCTHuffTable *pTable)
	{
		unsigned short nCode = 0;
		int nCodeBitsCount = 0;
		do {
			int nBit = 0;
			if ((nBit = ReadBit()) == EOF)
				return 9999;
			nCode = (nCode << 1) + nBit;
			++nCodeBitsCount;

			if (nCode - pTable->arrunFirstCode[nCodeBitsCount] < pTable->arrunCodesCount[nCodeBitsCount])
			{
				nCode -= pTable->arrunFirstCode[nCodeBitsCount];
				return pTable->arrunSymbols[pTable->arrunFirstSymbol[nCodeBitsCount] + nCode];
			}
		} while (nCodeBitsCount < 16);

		// TO DO: Error "Bad Huffman code in DCT stream"
		return 9999;
	}

	int DCTStream::ReadAmp(int nSize)
	{
		int nAmp = 0;
		for (int nBitsCount = 0; nBitsCount < nSize; ++nBitsCount)
		{
			int nBit = 0;
			if ((nBit = ReadBit()) == EOF)
				return 9999;
			nAmp = (nAmp << 1) + nBit;
		}
		if (nAmp < (1 << (nSize - 1)))
			nAmp -= (1 << nSize) - 1;
		return nAmp;
	}

	int DCTStream::ReadBit()
	{
		if (m_nInputBits == 0)
		{
			int nFirstChar = 0;
			if ((nFirstChar = m_pStream->GetChar()) == EOF)
				return EOF;
			if (nFirstChar == 0xff)
			{
				int nSecondChar = 0;
				do {
					nSecondChar = m_pStream->GetChar();
				} while (nSecondChar == 0xff);
				if (nSecondChar != 0x00)
				{
					// TO DO: Error "Bad DCT data: missing 00 after ff"
					return EOF;
				}
			}
			m_nInputBuffer = nFirstChar;
			m_nInputBits = 8;
		}
		int nBit = (m_nInputBuffer >> (m_nInputBits - 1)) & 1;
		--m_nInputBits;
		return nBit;
	}

	bool DCTStream::ReadHeader()
	{
		// Читаем заголовок
		bool bDoScan = false;
		while (!bDoScan)
		{
			int nChar = ReadMarker();
			switch (nChar)
			{
				case 0xc0:			// SOF0 (sequential)
				case 0xc1:			// SOF1 (extended sequential)
				if (!ReadBaselineSOF())
				{
					return false;
				}
				break;
				case 0xc2:			// SOF2 (progressive)
				if (!ReadProgressiveSOF())
				{
					return false;
				}
				break;
				case 0xc4:			// DHT
				if (!ReadHuffmanTables())
				{
					return false;
				}
				break;
				case 0xd8:			// SOI
				break;
				case 0xd9:			// EOI
				return false;
				case 0xda:			// SOS
				if (!ReadScanInfo())
				{
					return false;
				}
				bDoScan = true;
				break;
				case 0xdb:			// DQT
				if (!ReadQuantTables())
				{
					return false;
				}
				break;
				case 0xdd:			// DRI
				if (!ReadRestartInterval())
				{
					return false;
				}
				break;
				case 0xe0:			// APP0
				if (!ReadJFIFMarker())
				{
					return false;
				}
				break;
				case 0xee:			// APP14
				if (!ReadAdobeMarker())
				{
					return false;
				}
				break;
				case EOF:
				// TO DO : Error "Bad DCT header"
				return false;
				default:
				// Пропускам маркеры типа: APPn, COM и т.д.
				if (nChar >= 0xe0)
				{
					int nCount = Read16() - 2;
					for (int nIndex = 0; nIndex < nCount; ++nIndex)
					{
						m_pStream->GetChar();
					}
				}
				else
				{
					// TO DO: Error "Unknown DCT marker"
					return false;
				}
				break;
			}
		}

		return true;
	}

	bool DCTStream::ReadBaselineSOF()
	{
		int nLength = Read16();
		int nPrecision = m_pStream->GetChar();
		m_nHeight = Read16();
		m_nWidth = Read16();
		m_nComponentsCount = m_pStream->GetChar();
		if (m_nComponentsCount <= 0 || m_nComponentsCount > 4)
		{
			// TO DO: Error "Bad number of components in DCT stream"
			m_nComponentsCount = 0;
			return false;
		}
		if (nPrecision != 8)
		{
			// TO DO: Error "Bad DCT precision"
			return false;
		}
		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			m_arrCompInfo[nIndex].nID = m_pStream->GetChar();
			int nChar = m_pStream->GetChar();
			m_arrCompInfo[nIndex].nXResolution = (nChar >> 4) & 0x0f;
			m_arrCompInfo[nIndex].nYResolution = nChar & 0x0f;
			m_arrCompInfo[nIndex].nQuantTableNum = m_pStream->GetChar();
		}
		m_bProgressive = false;
		return true;
	}

	bool DCTStream::ReadProgressiveSOF()
	{
		int nLength = Read16();
		int ReadProgressiveSOF = m_pStream->GetChar();
		m_nHeight = Read16();
		m_nWidth = Read16();
		m_nComponentsCount = m_pStream->GetChar();

		if (m_nComponentsCount <= 0 || m_nComponentsCount > 4)
		{
			// TO DO: Error "Bad number of components in DCT stream"
			m_nComponentsCount = 0;
			return false;
		}
		if (ReadProgressiveSOF != 8)
		{
			// TO DO: Error "Bad DCT precision"
			return false;
		}
		for (int nIndex = 0; nIndex < m_nComponentsCount; ++nIndex)
		{
			m_arrCompInfo[nIndex].nID = m_pStream->GetChar();
			int nChar = m_pStream->GetChar();
			m_arrCompInfo[nIndex].nXResolution = (nChar >> 4) & 0x0f;
			m_arrCompInfo[nIndex].nYResolution = nChar & 0x0f;
			m_arrCompInfo[nIndex].nQuantTableNum = m_pStream->GetChar();
		}
		m_bProgressive = true;
		return true;
	}

	bool DCTStream::ReadScanInfo()
	{
		int nComp = 0;

		int nLength = Read16() - 2;
		m_oCurScanInfo.nComponentsCount = m_pStream->GetChar();
		if (m_oCurScanInfo.nComponentsCount <= 0 || m_oCurScanInfo.nComponentsCount > 4)
		{
			// TO DO: Error "Bad number of components in DCT stream"
			m_oCurScanInfo.nComponentsCount = 0;
			return false;
		}
		--nLength;
		if (nLength != 2 * m_oCurScanInfo.nComponentsCount + 3)
		{
			// TO DO: Error "Bad DCT scan info block"
			return false;
		}
		m_bInterleaved = m_oCurScanInfo.nComponentsCount == m_nComponentsCount;

		for (nComp = 0; nComp < m_nComponentsCount; ++nComp)
		{
			m_oCurScanInfo.arrbComponent[nComp] = false;
		}
		for (int nIndex = 0; nIndex < m_oCurScanInfo.nComponentsCount; ++nIndex)
		{
			int nID = m_pStream->GetChar();
			// некоторые(некорректные) DCT потоки используют свои ID числа много раз, но как минимум 
			// в потоке компоненты идут по порядку, поэтому мы сначала проверяем m_arrCompInfo[i], чтобы
			// решить данную пробему
			if (nID == m_arrCompInfo[nIndex].nID)
			{
				nComp = nIndex;
			}
			else
			{
				for (nComp = 0; nComp < m_nComponentsCount; ++nComp)
				{
					if (nID == m_arrCompInfo[nComp].nID)
					{
						break;
					}
				}
				if (nComp == m_nComponentsCount)
				{
					// TO DO: Error "Bad DCT component ID in scan info block"
					return false;
				}
			}
			m_oCurScanInfo.arrbComponent[nComp] = true;
			int nChar = m_pStream->GetChar();
			m_oCurScanInfo.arrDCHuffTable[nComp] = (nChar >> 4) & 0x0f;
			m_oCurScanInfo.arrACHuffTable[nComp] = nChar & 0x0f;
		}
		m_oCurScanInfo.nFirstKoef = m_pStream->GetChar();
		m_oCurScanInfo.nLastKoef = m_pStream->GetChar();
		int nChar = m_pStream->GetChar();
		m_oCurScanInfo.nApproxH = (nChar >> 4) & 0x0f;
		m_oCurScanInfo.nApproxL = nChar & 0x0f;
		return true;
	}

	bool DCTStream::ReadQuantTables()
	{
		int nLength = Read16() - 2;
		while (nLength > 0)
		{
			int nTableIndex = m_pStream->GetChar();
			int nPrecision = (nTableIndex >> 4) & 0x0f;
			nTableIndex &= 0x0f;
			if (nPrecision > 1 || nTableIndex >= 4)
			{
				// TO DO: Error "Bad DCT quantization table"
				return false;
			}
			if (nTableIndex == m_nQuantTablesCount)
			{
				m_nQuantTablesCount = nTableIndex + 1;
			}
			for (int nIndex = 0; nIndex < 64; ++nIndex)
			{
				if (nPrecision)
				{
					m_arrQuantTables[nTableIndex][arrDCTZigZag[nIndex]] = Read16();
				}
				else
				{
					m_arrQuantTables[nTableIndex][arrDCTZigZag[nIndex]] = m_pStream->GetChar();
				}
			}
			if (nPrecision)
			{
				nLength -= 129;
			}
			else
			{
				nLength -= 65;
			}
		}
		return true;
	}

	bool DCTStream::ReadHuffmanTables()
	{
		int nLength = Read16() - 2;

		while (nLength > 0)
		{
			int nTableIndex = m_pStream->GetChar();
			--nLength;
			if ((nTableIndex & 0x0f) >= 4)
			{
				// TO DO: Error "Bad DCT Huffman table"
				return false;
			}
			DCTHuffTable *pTable = NULL;
			if (nTableIndex & 0x10)
			{
				nTableIndex &= 0x0f;
				if (nTableIndex >= m_nACHuffTablesCount)
					m_nACHuffTablesCount = nTableIndex + 1;
				pTable = &m_arrACHuffTables[nTableIndex];
			}
			else
			{
				nTableIndex &= 0x0f;
				if (nTableIndex >= m_nDCHuffTablesCount)
					m_nDCHuffTablesCount = nTableIndex + 1;
				pTable = &m_arrDCHuffTables[nTableIndex];
			}
			unsigned char nSymbol = 0;
			unsigned short nCode = 0;
			for (int nIndex = 1; nIndex <= 16; ++nIndex)
			{
				int nChar = m_pStream->GetChar();
				pTable->arrunFirstSymbol[nIndex] = nSymbol;
				pTable->arrunFirstCode[nIndex]   = nCode;
				pTable->arrunCodesCount[nIndex]  = nChar;
				nSymbol += nChar;
				nCode = (nCode + nChar) << 1;
			}
			nLength -= 16;
			for (int nIndex = 0; nIndex < nSymbol; ++nIndex)
				pTable->arrunSymbols[nIndex] = m_pStream->GetChar();
			nLength -= nSymbol;
		}
		return true;
	}

	bool DCTStream::ReadRestartInterval()
	{
		int nLength = Read16();
		if (nLength != 4)
		{
			// TO DO: Error "Bad DCT restart interval"
			return false;
		}
		m_nRestartInterval = Read16();
		return true;
	}

	bool DCTStream::ReadJFIFMarker()
	{
		char sBuffer[5];

		int nLength = Read16();
		nLength -= 2;
		if (nLength >= 5)
		{
			for (int nIndex = 0; nIndex < 5; ++nIndex)
			{
				int nChar = 0;
				if ((nChar = m_pStream->GetChar()) == EOF)
				{
					// TO DO: Error "Bad DCT APP0 marker"
					return false;
				}
				sBuffer[nIndex] = nChar;
			}
			nLength -= 5;
			if (!memcmp(sBuffer, "JFIF\0", 5))
			{
				m_bJFIFMarker = true;
			}
		}
		while (nLength > 0)
		{
			if (m_pStream->GetChar() == EOF)
			{
				// TO DO: Error "Bad DCT APP0 marker"
				return false;
			}
			--nLength;
		}
		return true;
	}

	bool DCTStream::ReadAdobeMarker()
	{
		char sBuffer[12];

		int nLength = Read16();
		if (nLength < 14)
		{
			// TO DO: Error "Bad DCT Adobe APP14 marker"
			return false;
		}
		for (int nIndex = 0; nIndex < 12; ++nIndex)
		{
			int nChar = 0;
			if ((nChar = m_pStream->GetChar()) == EOF)
			{
				// TO DO: Error "Bad DCT Adobe APP14 marker"
				return false;
			}
			sBuffer[nIndex] = nChar;
		}
		if (strncmp(sBuffer, "Adobe", 5))
		{
			// TO DO: Error "Bad DCT Adobe APP14 marker"
			return false;
		}
		m_nColorTransform = sBuffer[11];
		m_bAdobeMarker = true;
		for (int nIndex = 14; nIndex < nLength; ++nIndex)
		{
			if (m_pStream->GetChar() == EOF)
			{
				// TO DO: Error "Bad DCT Adobe APP14 marker"
				return false;
			}
		}
		return true;
	}

	bool DCTStream::ReadTrailer()
	{
		int nChar = ReadMarker();
		if (nChar != 0xd9) // EOI
		{
			// TO DO: Error "Bad DCT trailer"
			return false;
		}
		return true;
	}

	int DCTStream::ReadMarker()
	{
		int nChar = 0;

		do {
			do {
				nChar = m_pStream->GetChar();
			} while (nChar != 0xff && nChar != EOF);

			do {
				nChar = m_pStream->GetChar();
			} while (nChar == 0xff);
		} while (nChar == 0x00);
		return nChar;
	}

	int DCTStream::Read16()
	{
		int nFirstChar = 0, nSecondChar = 0;

		if ((nFirstChar = m_pStream->GetChar()) == EOF)
			return EOF;
		if ((nSecondChar = m_pStream->GetChar()) == EOF)
			return EOF;
		return (nFirstChar << 8) + nSecondChar;
	}

	StringExt *DCTStream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		StringExt *seResult;

		if (nPSLevel < 2)
		{
			return NULL;
		}
		if (!(seResult = m_pStream->GetPSFilter(nPSLevel, sIndent)))
		{
			return NULL;
		}
		seResult->Append(sIndent)->Append("<< >> /DCTDecode filter\n");
		return seResult;
	}

	bool DCTStream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(true);
	}

	//---------------------------------------------------------------------------------------------------------------
	// FlateZlibStream
	//---------------------------------------------------------------------------------------------------------------

	FlateZlibStream::FlateZlibStream(Stream *pStream, int nPredictor, int nWidth, int nComponents, int nBitsPerComponent) :
		FilterStream(pStream)
	{
		if (1 != nPredictor)
		{
			m_pPredictor = new StreamPredictor(this, nPredictor, nWidth, nComponents, nBitsPerComponent);
			if (!m_pPredictor->CheckValidate())
			{
				delete m_pPredictor;
				m_pPredictor = NULL;
			}
		}
		else
		{
			m_pPredictor = NULL;
		}

		memset(m_arrInBuffer, 0, flateZlibWindow);
		memset(m_arrBuffer, 0, flateZlibWindow);

        m_oZStream.ClearFuncs();

        m_oZStream.Init();
	}

	FlateZlibStream::~FlateZlibStream()
	{
		if (m_pPredictor)
		{
			delete m_pPredictor;
		}
		delete m_pStream;

        m_oZStream.End();
	}

	void FlateZlibStream::Reset()
	{
		m_nBufferCurPos = 0;
		m_nRemain       = 0;

		m_bEndOfBlock = true;
		m_bEOF        = true;

        m_oZStream.End();
        m_oZStream.ClearFuncs();
        m_oZStream.Init();

		m_pStream->Reset();

		m_bEOF = false;
	}

	int FlateZlibStream::GetChar()
	{
		if (m_pPredictor)
		{
			return m_pPredictor->GetChar();
		}
		while (m_nRemain == 0)
		{
			if (m_bEndOfBlock && m_bEOF)
				return EOF;
			ReadSome();
		}

		int nChar = m_arrBuffer[m_nBufferCurPos];
		m_nBufferCurPos = (m_nBufferCurPos + 1) & flateZlibMask;
		--m_nRemain;
		return nChar;
	}

	int FlateZlibStream::LookChar()
	{
		if (m_pPredictor)
		{
			return m_pPredictor->LookChar();
		}

		while (m_nRemain == 0)
		{
			if (m_bEndOfBlock && m_bEOF)
				return EOF;
			ReadSome();
		}
		int nChar = m_arrBuffer[m_nBufferCurPos];
		return nChar;
	}

	int FlateZlibStream::GetRawChar()
	{
		while (m_nRemain == 0)
		{
			if (m_bEndOfBlock && m_bEOF)
				return EOF;
			ReadSome();
		}
		int nChar = m_arrBuffer[m_nBufferCurPos];
		m_nBufferCurPos = (m_nBufferCurPos + 1) & flateZlibMask;
		--m_nRemain;
		return nChar;
	}

	StringExt *FlateZlibStream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		StringExt *seResult;

		if (nPSLevel < 3 || m_pPredictor)
		{
			return NULL;
		}
		if (!(seResult = m_pStream->GetPSFilter(nPSLevel, sIndent)))
		{
			return NULL;
		}
		seResult->Append(sIndent)->Append("<< >> /FlateDecode filter\n");
		return seResult;
	}

	bool FlateZlibStream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(true);
	}

	void FlateZlibStream::ReadSome()
	{
		unsigned int unInSize = flateZlibWindow;
		if (m_bEndOfBlock)
		{
			memset(m_arrInBuffer, 0, flateZlibWindow);
			int nChar = 0;
			for (unsigned int unIndex = 0; unIndex < flateZlibWindow; unIndex++)
			{
				nChar = m_pStream->GetChar();

				if (EOF == nChar)
				{
					unInSize = unIndex;
					if (0 == unInSize)
					{
						m_nRemain = 0;
						m_bEndOfBlock = m_bEOF = true;
						return;
					}
					break;
				}

				m_arrInBuffer[unIndex] = nChar;
			}

            m_oZStream.SetIn(m_arrInBuffer, unInSize);

			m_bEndOfBlock = false;
		}

        m_oZStream.SetOut(m_arrBuffer, flateZlibWindow);

        int nRet = m_oZStream.Process(DEFLATE_NO_FLUSH);

        if (nRet == DEFLATE_DATA_ERROR || nRet == DEFLATE_MEM_ERROR)
		{
			m_nRemain = 0;
			m_bEOF = m_bEndOfBlock = true;
			return;
		}

        m_nRemain = flateZlibWindow - m_oZStream.GetAvailOut();

        if (m_oZStream.GetAvailOut() != 0)
			m_bEndOfBlock = true;

		return;
	}


	//---------------------------------------------------------------------------------------------------------------
	// FlateStream
	//---------------------------------------------------------------------------------------------------------------

	int FlateStream::m_arrCodeLenCodeMap[flateMaxCodeLenCodes] =
	{
		16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
	};

	FlateDecode FlateStream::m_arrLengthDecode[flateMaxLitCodes - 257] =
	{
		{ 0, 3 },
		{ 0, 4 },
		{ 0, 5 },
		{ 0, 6 },
		{ 0, 7 },
		{ 0, 8 },
		{ 0, 9 },
		{ 0, 10 },
		{ 1, 11 },
		{ 1, 13 },
		{ 1, 15 },
		{ 1, 17 },
		{ 2, 19 },
		{ 2, 23 },
		{ 2, 27 },
		{ 2, 31 },
		{ 3, 35 },
		{ 3, 43 },
		{ 3, 51 },
		{ 3, 59 },
		{ 4, 67 },
		{ 4, 83 },
		{ 4, 99 },
		{ 4, 115 },
		{ 5, 131 },
		{ 5, 163 },
		{ 5, 195 },
		{ 5, 227 },
		{ 0, 258 },
		{ 0, 258 },
		{ 0, 258 }
	};

	FlateDecode FlateStream::m_arrDistanceDecode[flateMaxDistCodes] =
	{
		{ 0, 1 },
		{ 0, 2 },
		{ 0, 3 },
		{ 0, 4 },
		{ 1, 5 },
		{ 1, 7 },
		{ 2, 9 },
		{ 2, 13 },
		{ 3, 17 },
		{ 3, 25 },
		{ 4, 33 },
		{ 4, 49 },
		{ 5, 65 },
		{ 5, 97 },
		{ 6, 129 },
		{ 6, 193 },
		{ 7, 257 },
		{ 7, 385 },
		{ 8, 513 },
		{ 8, 769 },
		{ 9, 1025 },
		{ 9, 1537 },
		{ 10, 2049 },
		{ 10, 3073 },
		{ 11, 4097 },
		{ 11, 6145 },
		{ 12, 8193 },
		{ 12, 12289 },
		{ 13, 16385 },
		{ 13, 24577 }
	};

	static FlateHuffmanCode c_arrFlateFixedLiteralCodeTableCodes[512] =
	{
		{ 7, 0x0100 },
		{ 8, 0x0050 },
		{ 8, 0x0010 },
		{ 8, 0x0118 },
		{ 7, 0x0110 },
		{ 8, 0x0070 },
		{ 8, 0x0030 },
		{ 9, 0x00c0 },
		{ 7, 0x0108 },
		{ 8, 0x0060 },
		{ 8, 0x0020 },
		{ 9, 0x00a0 },
		{ 8, 0x0000 },
		{ 8, 0x0080 },
		{ 8, 0x0040 },
		{ 9, 0x00e0 },
		{ 7, 0x0104 },
		{ 8, 0x0058 },
		{ 8, 0x0018 },
		{ 9, 0x0090 },
		{ 7, 0x0114 },
		{ 8, 0x0078 },
		{ 8, 0x0038 },
		{ 9, 0x00d0 },
		{ 7, 0x010c },
		{ 8, 0x0068 },
		{ 8, 0x0028 },
		{ 9, 0x00b0 },
		{ 8, 0x0008 },
		{ 8, 0x0088 },
		{ 8, 0x0048 },
		{ 9, 0x00f0 },
		{ 7, 0x0102 },
		{ 8, 0x0054 },
		{ 8, 0x0014 },
		{ 8, 0x011c },
		{ 7, 0x0112 },
		{ 8, 0x0074 },
		{ 8, 0x0034 },
		{ 9, 0x00c8 },
		{ 7, 0x010a },
		{ 8, 0x0064 },
		{ 8, 0x0024 },
		{ 9, 0x00a8 },
		{ 8, 0x0004 },
		{ 8, 0x0084 },
		{ 8, 0x0044 },
		{ 9, 0x00e8 },
		{ 7, 0x0106 },
		{ 8, 0x005c },
		{ 8, 0x001c },
		{ 9, 0x0098 },
		{ 7, 0x0116 },
		{ 8, 0x007c },
		{ 8, 0x003c },
		{ 9, 0x00d8 },
		{ 7, 0x010e },
		{ 8, 0x006c },
		{ 8, 0x002c },
		{ 9, 0x00b8 },
		{ 8, 0x000c },
		{ 8, 0x008c },
		{ 8, 0x004c },
		{ 9, 0x00f8 },
		{ 7, 0x0101 },
		{ 8, 0x0052 },
		{ 8, 0x0012 },
		{ 8, 0x011a },
		{ 7, 0x0111 },
		{ 8, 0x0072 },
		{ 8, 0x0032 },
		{ 9, 0x00c4 },
		{ 7, 0x0109 },
		{ 8, 0x0062 },
		{ 8, 0x0022 },
		{ 9, 0x00a4 },
		{ 8, 0x0002 },
		{ 8, 0x0082 },
		{ 8, 0x0042 },
		{ 9, 0x00e4 },
		{ 7, 0x0105 },
		{ 8, 0x005a },
		{ 8, 0x001a },
		{ 9, 0x0094 },
		{ 7, 0x0115 },
		{ 8, 0x007a },
		{ 8, 0x003a },
		{ 9, 0x00d4 },
		{ 7, 0x010d },
		{ 8, 0x006a },
		{ 8, 0x002a },
		{ 9, 0x00b4 },
		{ 8, 0x000a },
		{ 8, 0x008a },
		{ 8, 0x004a },
		{ 9, 0x00f4 },
		{ 7, 0x0103 },
		{ 8, 0x0056 },
		{ 8, 0x0016 },
		{ 8, 0x011e },
		{ 7, 0x0113 },
		{ 8, 0x0076 },
		{ 8, 0x0036 },
		{ 9, 0x00cc },
		{ 7, 0x010b },
		{ 8, 0x0066 },
		{ 8, 0x0026 },
		{ 9, 0x00ac },
		{ 8, 0x0006 },
		{ 8, 0x0086 },
		{ 8, 0x0046 },
		{ 9, 0x00ec },
		{ 7, 0x0107 },
		{ 8, 0x005e },
		{ 8, 0x001e },
		{ 9, 0x009c },
		{ 7, 0x0117 },
		{ 8, 0x007e },
		{ 8, 0x003e },
		{ 9, 0x00dc },
		{ 7, 0x010f },
		{ 8, 0x006e },
		{ 8, 0x002e },
		{ 9, 0x00bc },
		{ 8, 0x000e },
		{ 8, 0x008e },
		{ 8, 0x004e },
		{ 9, 0x00fc },
		{ 7, 0x0100 },
		{ 8, 0x0051 },
		{ 8, 0x0011 },
		{ 8, 0x0119 },
		{ 7, 0x0110 },
		{ 8, 0x0071 },
		{ 8, 0x0031 },
		{ 9, 0x00c2 },
		{ 7, 0x0108 },
		{ 8, 0x0061 },
		{ 8, 0x0021 },
		{ 9, 0x00a2 },
		{ 8, 0x0001 },
		{ 8, 0x0081 },
		{ 8, 0x0041 },
		{ 9, 0x00e2 },
		{ 7, 0x0104 },
		{ 8, 0x0059 },
		{ 8, 0x0019 },
		{ 9, 0x0092 },
		{ 7, 0x0114 },
		{ 8, 0x0079 },
		{ 8, 0x0039 },
		{ 9, 0x00d2 },
		{ 7, 0x010c },
		{ 8, 0x0069 },
		{ 8, 0x0029 },
		{ 9, 0x00b2 },
		{ 8, 0x0009 },
		{ 8, 0x0089 },
		{ 8, 0x0049 },
		{ 9, 0x00f2 },
		{ 7, 0x0102 },
		{ 8, 0x0055 },
		{ 8, 0x0015 },
		{ 8, 0x011d },
		{ 7, 0x0112 },
		{ 8, 0x0075 },
		{ 8, 0x0035 },
		{ 9, 0x00ca },
		{ 7, 0x010a },
		{ 8, 0x0065 },
		{ 8, 0x0025 },
		{ 9, 0x00aa },
		{ 8, 0x0005 },
		{ 8, 0x0085 },
		{ 8, 0x0045 },
		{ 9, 0x00ea },
		{ 7, 0x0106 },
		{ 8, 0x005d },
		{ 8, 0x001d },
		{ 9, 0x009a },
		{ 7, 0x0116 },
		{ 8, 0x007d },
		{ 8, 0x003d },
		{ 9, 0x00da },
		{ 7, 0x010e },
		{ 8, 0x006d },
		{ 8, 0x002d },
		{ 9, 0x00ba },
		{ 8, 0x000d },
		{ 8, 0x008d },
		{ 8, 0x004d },
		{ 9, 0x00fa },
		{ 7, 0x0101 },
		{ 8, 0x0053 },
		{ 8, 0x0013 },
		{ 8, 0x011b },
		{ 7, 0x0111 },
		{ 8, 0x0073 },
		{ 8, 0x0033 },
		{ 9, 0x00c6 },
		{ 7, 0x0109 },
		{ 8, 0x0063 },
		{ 8, 0x0023 },
		{ 9, 0x00a6 },
		{ 8, 0x0003 },
		{ 8, 0x0083 },
		{ 8, 0x0043 },
		{ 9, 0x00e6 },
		{ 7, 0x0105 },
		{ 8, 0x005b },
		{ 8, 0x001b },
		{ 9, 0x0096 },
		{ 7, 0x0115 },
		{ 8, 0x007b },
		{ 8, 0x003b },
		{ 9, 0x00d6 },
		{ 7, 0x010d },
		{ 8, 0x006b },
		{ 8, 0x002b },
		{ 9, 0x00b6 },
		{ 8, 0x000b },
		{ 8, 0x008b },
		{ 8, 0x004b },
		{ 9, 0x00f6 },
		{ 7, 0x0103 },
		{ 8, 0x0057 },
		{ 8, 0x0017 },
		{ 8, 0x011f },
		{ 7, 0x0113 },
		{ 8, 0x0077 },
		{ 8, 0x0037 },
		{ 9, 0x00ce },
		{ 7, 0x010b },
		{ 8, 0x0067 },
		{ 8, 0x0027 },
		{ 9, 0x00ae },
		{ 8, 0x0007 },
		{ 8, 0x0087 },
		{ 8, 0x0047 },
		{ 9, 0x00ee },
		{ 7, 0x0107 },
		{ 8, 0x005f },
		{ 8, 0x001f },
		{ 9, 0x009e },
		{ 7, 0x0117 },
		{ 8, 0x007f },
		{ 8, 0x003f },
		{ 9, 0x00de },
		{ 7, 0x010f },
		{ 8, 0x006f },
		{ 8, 0x002f },
		{ 9, 0x00be },
		{ 8, 0x000f },
		{ 8, 0x008f },
		{ 8, 0x004f },
		{ 9, 0x00fe },
		{ 7, 0x0100 },
		{ 8, 0x0050 },
		{ 8, 0x0010 },
		{ 8, 0x0118 },
		{ 7, 0x0110 },
		{ 8, 0x0070 },
		{ 8, 0x0030 },
		{ 9, 0x00c1 },
		{ 7, 0x0108 },
		{ 8, 0x0060 },
		{ 8, 0x0020 },
		{ 9, 0x00a1 },
		{ 8, 0x0000 },
		{ 8, 0x0080 },
		{ 8, 0x0040 },
		{ 9, 0x00e1 },
		{ 7, 0x0104 },
		{ 8, 0x0058 },
		{ 8, 0x0018 },
		{ 9, 0x0091 },
		{ 7, 0x0114 },
		{ 8, 0x0078 },
		{ 8, 0x0038 },
		{ 9, 0x00d1 },
		{ 7, 0x010c },
		{ 8, 0x0068 },
		{ 8, 0x0028 },
		{ 9, 0x00b1 },
		{ 8, 0x0008 },
		{ 8, 0x0088 },
		{ 8, 0x0048 },
		{ 9, 0x00f1 },
		{ 7, 0x0102 },
		{ 8, 0x0054 },
		{ 8, 0x0014 },
		{ 8, 0x011c },
		{ 7, 0x0112 },
		{ 8, 0x0074 },
		{ 8, 0x0034 },
		{ 9, 0x00c9 },
		{ 7, 0x010a },
		{ 8, 0x0064 },
		{ 8, 0x0024 },
		{ 9, 0x00a9 },
		{ 8, 0x0004 },
		{ 8, 0x0084 },
		{ 8, 0x0044 },
		{ 9, 0x00e9 },
		{ 7, 0x0106 },
		{ 8, 0x005c },
		{ 8, 0x001c },
		{ 9, 0x0099 },
		{ 7, 0x0116 },
		{ 8, 0x007c },
		{ 8, 0x003c },
		{ 9, 0x00d9 },
		{ 7, 0x010e },
		{ 8, 0x006c },
		{ 8, 0x002c },
		{ 9, 0x00b9 },
		{ 8, 0x000c },
		{ 8, 0x008c },
		{ 8, 0x004c },
		{ 9, 0x00f9 },
		{ 7, 0x0101 },
		{ 8, 0x0052 },
		{ 8, 0x0012 },
		{ 8, 0x011a },
		{ 7, 0x0111 },
		{ 8, 0x0072 },
		{ 8, 0x0032 },
		{ 9, 0x00c5 },
		{ 7, 0x0109 },
		{ 8, 0x0062 },
		{ 8, 0x0022 },
		{ 9, 0x00a5 },
		{ 8, 0x0002 },
		{ 8, 0x0082 },
		{ 8, 0x0042 },
		{ 9, 0x00e5 },
		{ 7, 0x0105 },
		{ 8, 0x005a },
		{ 8, 0x001a },
		{ 9, 0x0095 },
		{ 7, 0x0115 },
		{ 8, 0x007a },
		{ 8, 0x003a },
		{ 9, 0x00d5 },
		{ 7, 0x010d },
		{ 8, 0x006a },
		{ 8, 0x002a },
		{ 9, 0x00b5 },
		{ 8, 0x000a },
		{ 8, 0x008a },
		{ 8, 0x004a },
		{ 9, 0x00f5 },
		{ 7, 0x0103 },
		{ 8, 0x0056 },
		{ 8, 0x0016 },
		{ 8, 0x011e },
		{ 7, 0x0113 },
		{ 8, 0x0076 },
		{ 8, 0x0036 },
		{ 9, 0x00cd },
		{ 7, 0x010b },
		{ 8, 0x0066 },
		{ 8, 0x0026 },
		{ 9, 0x00ad },
		{ 8, 0x0006 },
		{ 8, 0x0086 },
		{ 8, 0x0046 },
		{ 9, 0x00ed },
		{ 7, 0x0107 },
		{ 8, 0x005e },
		{ 8, 0x001e },
		{ 9, 0x009d },
		{ 7, 0x0117 },
		{ 8, 0x007e },
		{ 8, 0x003e },
		{ 9, 0x00dd },
		{ 7, 0x010f },
		{ 8, 0x006e },
		{ 8, 0x002e },
		{ 9, 0x00bd },
		{ 8, 0x000e },
		{ 8, 0x008e },
		{ 8, 0x004e },
		{ 9, 0x00fd },
		{ 7, 0x0100 },
		{ 8, 0x0051 },
		{ 8, 0x0011 },
		{ 8, 0x0119 },
		{ 7, 0x0110 },
		{ 8, 0x0071 },
		{ 8, 0x0031 },
		{ 9, 0x00c3 },
		{ 7, 0x0108 },
		{ 8, 0x0061 },
		{ 8, 0x0021 },
		{ 9, 0x00a3 },
		{ 8, 0x0001 },
		{ 8, 0x0081 },
		{ 8, 0x0041 },
		{ 9, 0x00e3 },
		{ 7, 0x0104 },
		{ 8, 0x0059 },
		{ 8, 0x0019 },
		{ 9, 0x0093 },
		{ 7, 0x0114 },
		{ 8, 0x0079 },
		{ 8, 0x0039 },
		{ 9, 0x00d3 },
		{ 7, 0x010c },
		{ 8, 0x0069 },
		{ 8, 0x0029 },
		{ 9, 0x00b3 },
		{ 8, 0x0009 },
		{ 8, 0x0089 },
		{ 8, 0x0049 },
		{ 9, 0x00f3 },
		{ 7, 0x0102 },
		{ 8, 0x0055 },
		{ 8, 0x0015 },
		{ 8, 0x011d },
		{ 7, 0x0112 },
		{ 8, 0x0075 },
		{ 8, 0x0035 },
		{ 9, 0x00cb },
		{ 7, 0x010a },
		{ 8, 0x0065 },
		{ 8, 0x0025 },
		{ 9, 0x00ab },
		{ 8, 0x0005 },
		{ 8, 0x0085 },
		{ 8, 0x0045 },
		{ 9, 0x00eb },
		{ 7, 0x0106 },
		{ 8, 0x005d },
		{ 8, 0x001d },
		{ 9, 0x009b },
		{ 7, 0x0116 },
		{ 8, 0x007d },
		{ 8, 0x003d },
		{ 9, 0x00db },
		{ 7, 0x010e },
		{ 8, 0x006d },
		{ 8, 0x002d },
		{ 9, 0x00bb },
		{ 8, 0x000d },
		{ 8, 0x008d },
		{ 8, 0x004d },
		{ 9, 0x00fb },
		{ 7, 0x0101 },
		{ 8, 0x0053 },
		{ 8, 0x0013 },
		{ 8, 0x011b },
		{ 7, 0x0111 },
		{ 8, 0x0073 },
		{ 8, 0x0033 },
		{ 9, 0x00c7 },
		{ 7, 0x0109 },
		{ 8, 0x0063 },
		{ 8, 0x0023 },
		{ 9, 0x00a7 },
		{ 8, 0x0003 },
		{ 8, 0x0083 },
		{ 8, 0x0043 },
		{ 9, 0x00e7 },
		{ 7, 0x0105 },
		{ 8, 0x005b },
		{ 8, 0x001b },
		{ 9, 0x0097 },
		{ 7, 0x0115 },
		{ 8, 0x007b },
		{ 8, 0x003b },
		{ 9, 0x00d7 },
		{ 7, 0x010d },
		{ 8, 0x006b },
		{ 8, 0x002b },
		{ 9, 0x00b7 },
		{ 8, 0x000b },
		{ 8, 0x008b },
		{ 8, 0x004b },
		{ 9, 0x00f7 },
		{ 7, 0x0103 },
		{ 8, 0x0057 },
		{ 8, 0x0017 },
		{ 8, 0x011f },
		{ 7, 0x0113 },
		{ 8, 0x0077 },
		{ 8, 0x0037 },
		{ 9, 0x00cf },
		{ 7, 0x010b },
		{ 8, 0x0067 },
		{ 8, 0x0027 },
		{ 9, 0x00af },
		{ 8, 0x0007 },
		{ 8, 0x0087 },
		{ 8, 0x0047 },
		{ 9, 0x00ef },
		{ 7, 0x0107 },
		{ 8, 0x005f },
		{ 8, 0x001f },
		{ 9, 0x009f },
		{ 7, 0x0117 },
		{ 8, 0x007f },
		{ 8, 0x003f },
		{ 9, 0x00df },
		{ 7, 0x010f },
		{ 8, 0x006f },
		{ 8, 0x002f },
		{ 9, 0x00bf },
		{ 8, 0x000f },
		{ 8, 0x008f },
		{ 8, 0x004f },
		{ 9, 0x00ff }
	};

	FlateHuffmanTable FlateStream::m_oFixedLiteralCodeTable =
	{
		c_arrFlateFixedLiteralCodeTableCodes, 9
	};

	static FlateHuffmanCode c_arrFlateFixedDistanceCodeTableCodes[32] =
	{
		{ 5, 0x0000 },
		{ 5, 0x0010 },
		{ 5, 0x0008 },
		{ 5, 0x0018 },
		{ 5, 0x0004 },
		{ 5, 0x0014 },
		{ 5, 0x000c },
		{ 5, 0x001c },
		{ 5, 0x0002 },
		{ 5, 0x0012 },
		{ 5, 0x000a },
		{ 5, 0x001a },
		{ 5, 0x0006 },
		{ 5, 0x0016 },
		{ 5, 0x000e },
		{ 0, 0x0000 },
		{ 5, 0x0001 },
		{ 5, 0x0011 },
		{ 5, 0x0009 },
		{ 5, 0x0019 },
		{ 5, 0x0005 },
		{ 5, 0x0015 },
		{ 5, 0x000d },
		{ 5, 0x001d },
		{ 5, 0x0003 },
		{ 5, 0x0013 },
		{ 5, 0x000b },
		{ 5, 0x001b },
		{ 5, 0x0007 },
		{ 5, 0x0017 },
		{ 5, 0x000f },
		{ 0, 0x0000 }
	};

	FlateHuffmanTable FlateStream::m_oFixedDistanceCodeTable =
	{
		c_arrFlateFixedDistanceCodeTableCodes, 5
	};

	FlateStream::FlateStream(Stream *pStream, int nPredictor, int nWidth, int nComponents, int nBitsPerComponent) :
		FilterStream(pStream)
	{
		if (1 != nPredictor)
		{
			m_pPredictor = new StreamPredictor(this, nPredictor, nWidth, nComponents, nBitsPerComponent);
			if (!m_pPredictor->CheckValidate())
			{
				delete m_pPredictor;
				m_pPredictor = NULL;
			}
		}
		else
		{
			m_pPredictor = NULL;
		}
		m_oLiteratCodeTable.pCodes = NULL;
		m_oDistanceCodeTable.pCodes = NULL;

		memset(m_arrBuffer, 0, flateWindow);
	}

	FlateStream::~FlateStream()
	{
		if (m_oLiteratCodeTable.pCodes != m_oFixedLiteralCodeTable.pCodes)
		{
			MemUtilsFree(m_oLiteratCodeTable.pCodes);
		}
		if (m_oDistanceCodeTable.pCodes != m_oFixedDistanceCodeTable.pCodes)
		{
			MemUtilsFree(m_oDistanceCodeTable.pCodes);
		}
		if (m_pPredictor)
		{
			delete m_pPredictor;
		}
		delete m_pStream;
	}

	void FlateStream::Reset()
	{
		m_nBufferCurPos = 0;
		m_nRemain = 0;
		m_nCodeBuffer = 0;
		m_nCodeSize = 0;
		m_bCompressedBlock = false;
		m_bEndOfBlock = true;
		m_bEOF = true;

		m_pStream->Reset();

		m_bEndOfBlock = m_bEOF = true;

		//читаем заголовок
		int nCmf = m_pStream->GetChar();
		int nFlag = m_pStream->GetChar();
		if (nCmf == EOF || nFlag == EOF)
			return;
		if ((nCmf & 0x0f) != 0x08)
		{
			//TO DO: Error "Unknown compression method in flate stream"
			return;
		}
		if ((((nCmf << 8) + nFlag) % 31) != 0)
		{
			// TO DO: Error "Bad FCHECK in flate stream"
			return;
		}
		if (nFlag & 0x20)
		{
			//TO DO: Error "FDICT bit set in flate stream"
			return;
		}

		m_bEOF = false;
	}

	int FlateStream::GetChar()
	{
		if (m_pPredictor)
		{
			return m_pPredictor->GetChar();
		}
		while (m_nRemain == 0)
		{
			if (m_bEndOfBlock && m_bEOF)
				return EOF;
			ReadSome();
		}

		int nChar = m_arrBuffer[m_nBufferCurPos];
		m_nBufferCurPos = (m_nBufferCurPos + 1) & flateMask;
		--m_nRemain;
		return nChar;
	}

	int FlateStream::LookChar()
	{
		if (m_pPredictor)
		{
			return m_pPredictor->LookChar();
		}

		while (m_nRemain == 0)
		{
			if (m_bEndOfBlock && m_bEOF)
				return EOF;
			ReadSome();
		}
		int nChar = m_arrBuffer[m_nBufferCurPos];
		return nChar;
	}

	int FlateStream::GetRawChar()
	{
		while (m_nRemain == 0)
		{
			if (m_bEndOfBlock && m_bEOF)
				return EOF;
			ReadSome();
		}
		int nChar = m_arrBuffer[m_nBufferCurPos];
		m_nBufferCurPos = (m_nBufferCurPos + 1) & flateMask;
		--m_nRemain;
		return nChar;
	}

	StringExt *FlateStream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		StringExt *seResult;

		if (nPSLevel < 3 || m_pPredictor)
		{
			return NULL;
		}
		if (!(seResult = m_pStream->GetPSFilter(nPSLevel, sIndent)))
		{
			return NULL;
		}
		seResult->Append(sIndent)->Append("<< >> /FlateDecode filter\n");
		return seResult;
	}

	bool FlateStream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(true);
	}

	void FlateStream::ReadSome()
	{
		int nFirst = 0, nSecond = 0;

		if (m_bEndOfBlock)
		{
			if (!StartBlock())
				return;
		}

		if (m_bCompressedBlock)
		{
			int nHuffCode = 0;
			if ((nHuffCode = GetHuffmanCodeWord(&m_oLiteratCodeTable)) == EOF)
			{
				// TO DO: Error "Unexpected end of file in flate stream"
				m_bEndOfBlock = m_bEOF = true;
				m_nRemain = 0;
			}
			if (nHuffCode < 256)
			{
				m_arrBuffer[m_nBufferCurPos] = nHuffCode;
				m_nRemain = 1;
			}
			else if (nHuffCode == 256)
			{
				m_bEndOfBlock = true;
				m_nRemain = 0;
			}
			else
			{
				nHuffCode -= 257;
				int nExtraCode = m_arrLengthDecode[nHuffCode].nExtraBitsCount;
				if (nExtraCode > 0 && (nExtraCode = GetCodeWord(nExtraCode)) == EOF)
				{
					// TO DO: Error "Unexpected end of file in flate stream"
					m_bEndOfBlock = m_bEOF = true;
					m_nRemain = 0;
				}
				int nLen = m_arrLengthDecode[nHuffCode].nFirst + nExtraCode;
				if ((nHuffCode = GetHuffmanCodeWord(&m_oDistanceCodeTable)) == EOF)
				{
					// TO DO: Error "Unexpected end of file in flate stream"
					m_bEndOfBlock = m_bEOF = true;
					m_nRemain = 0;
				}
				nExtraCode = m_arrDistanceDecode[nHuffCode].nExtraBitsCount;
				if (nExtraCode > 0 && (nExtraCode = GetCodeWord(nExtraCode)) == EOF)
				{
					// TO DO: Error "Unexpected end of file in flate stream"
					m_bEndOfBlock = m_bEOF = true;
					m_nRemain = 0;
				}
				int nDistance = m_arrDistanceDecode[nHuffCode].nFirst + nExtraCode;
				nFirst = m_nBufferCurPos;
				nSecond = (m_nBufferCurPos - nDistance) & flateMask;
				for (int nK = 0; nK < nLen; ++nK)
				{
					m_arrBuffer[nFirst] = m_arrBuffer[nSecond];
					nFirst  = (nFirst + 1) & flateMask;
					nSecond = (nSecond + 1) & flateMask;
				}
				m_nRemain = nLen;
			}
		}
		else
		{
			int nLen = (m_nUncompBlockLen < flateWindow) ? m_nUncompBlockLen : flateWindow;
			for (nFirst = 0, nSecond = m_nBufferCurPos; nFirst < nLen; ++nFirst, nSecond = (nSecond + 1) & flateMask)
			{
				int nChar = 0;
				if ((nChar = m_pStream->GetChar()) == EOF)
				{
					m_bEndOfBlock = m_bEOF = true;
					break;
				}
				m_arrBuffer[nSecond] = nChar & 0xff;
			}
			m_nRemain = nFirst;
			m_nUncompBlockLen -= nLen;
			if (m_nUncompBlockLen == 0)
				m_bEndOfBlock = true;
		}
		return;
	}

	bool FlateStream::StartBlock()
	{
		// Освобождаем таблицы кодов из предыдущего блока
		if (m_oLiteratCodeTable.pCodes != m_oFixedLiteralCodeTable.pCodes)
		{
			MemUtilsFree(m_oLiteratCodeTable.pCodes);
		}
		m_oLiteratCodeTable.pCodes = NULL;
		if (m_oDistanceCodeTable.pCodes != m_oFixedDistanceCodeTable.pCodes)
		{
			MemUtilsFree(m_oDistanceCodeTable.pCodes);
		}
		m_oDistanceCodeTable.pCodes = NULL;

		// Считываем заголовок блока
		int nBlockHeader = GetCodeWord(3);
		if (nBlockHeader & 1)
			m_bEOF = true;

		nBlockHeader >>= 1;

		if (nBlockHeader == 0) // uncompressed block
		{
			m_bCompressedBlock = false;
			int nChar = 0;
			if ((nChar = m_pStream->GetChar()) == EOF)
			{
				// TO DO: Error "Bad block header in flate stream"
				m_bEndOfBlock = m_bEOF = true;
				return false;
			}
			m_nUncompBlockLen = nChar & 0xff;
			if ((nChar = m_pStream->GetChar()) == EOF)
			{
				// TO DO: Error "Bad block header in flate stream"
				m_bEndOfBlock = m_bEOF = true;
				return false;
			}
			m_nUncompBlockLen |= (nChar & 0xff) << 8;
			if ((nChar = m_pStream->GetChar()) == EOF)
			{
				// TO DO: Error "Bad block header in flate stream"
				m_bEndOfBlock = m_bEOF = true;
				return false;
			}
			int nCheck = nChar & 0xff;
			if ((nChar = m_pStream->GetChar()) == EOF)
			{
				// TO DO: Error "Bad block header in flate stream"
				m_bEndOfBlock = m_bEOF = true;
				return false;
			}
			nCheck |= (nChar & 0xff) << 8;
			if (nCheck != (~m_nUncompBlockLen & 0xffff))
			{
				// TO DO: Error "Bad uncompressed block length in flate stream"
			}
			m_nCodeBuffer = 0;
			m_nCodeSize = 0;
		}
		else if (nBlockHeader == 1) // compressed block with fixed codes
		{
			m_bCompressedBlock = true;
			LoadFixedCodes();
		}
		else if (nBlockHeader == 2) // compressed block with dynamic codes
		{
			m_bCompressedBlock = true;
			if (!ReadDynamicCodes())
			{
				// TO DO: Error "Bad block header in flate stream"
				m_bEndOfBlock = m_bEOF = true;
				return false;
			}
		}
		else  // unknown block type
		{
			// TO DO: Error "Bad block header in flate stream"
			m_bEndOfBlock = m_bEOF = true;
			return false;
		}

		m_bEndOfBlock = false;
		return true;
	}

	void FlateStream::LoadFixedCodes()
	{
		m_oLiteratCodeTable.pCodes = m_oFixedLiteralCodeTable.pCodes;
		m_oLiteratCodeTable.nMaxLen = m_oFixedLiteralCodeTable.nMaxLen;
		m_oDistanceCodeTable.pCodes = m_oFixedDistanceCodeTable.pCodes;
		m_oDistanceCodeTable.nMaxLen = m_oFixedDistanceCodeTable.nMaxLen;
	}

	bool FlateStream::ReadDynamicCodes()
	{
		int arrCodeLenCodeLengths[flateMaxCodeLenCodes];
		FlateHuffmanTable oCodeLenCodeTable;

		oCodeLenCodeTable.pCodes = NULL;

		// Lengths
		int nNumLiteralCodes = 0;
		if ((nNumLiteralCodes = GetCodeWord(5)) == EOF)
		{
			// TO DO: Error "Bad dynamic code table in flate stream"
			MemUtilsFree(oCodeLenCodeTable.pCodes);
			return false;
		}
		nNumLiteralCodes += 257;

		int nNumDistanceCodes = 0;
		if ((nNumDistanceCodes = GetCodeWord(5)) == EOF)
		{
			// TO DO: Error "Bad dynamic code table in flate stream"
			MemUtilsFree(oCodeLenCodeTable.pCodes);
			return false;
		}
		nNumDistanceCodes += 1;

		int nNumCodeLenCodes = 0;
		if ((nNumCodeLenCodes = GetCodeWord(4)) == EOF)
		{
			// TO DO: Error "Bad dynamic code table in flate stream"
			MemUtilsFree(oCodeLenCodeTable.pCodes);
			return false;
		}
		nNumCodeLenCodes += 4;

		if (nNumLiteralCodes > flateMaxLitCodes || nNumDistanceCodes > flateMaxDistCodes || nNumCodeLenCodes > flateMaxCodeLenCodes)
		{
			// TO DO: Error "Bad dynamic code table in flate stream"
			MemUtilsFree(oCodeLenCodeTable.pCodes);
			return false;
		}

		// code length code table
		for (int nI = 0; nI < flateMaxCodeLenCodes; ++nI)
		{
			arrCodeLenCodeLengths[nI] = 0;
		}
		for (int nI = 0; nI < nNumCodeLenCodes; ++nI)
		{
			if ((arrCodeLenCodeLengths[m_arrCodeLenCodeMap[nI]] = GetCodeWord(3)) == -1)
			{
				// TO DO: Error "Bad dynamic code table in flate stream"
				MemUtilsFree(oCodeLenCodeTable.pCodes);
				return false;
			}
		}
		ConvertHuffmanCodes(arrCodeLenCodeLengths, flateMaxCodeLenCodes, &oCodeLenCodeTable);

		// Literal, distance code tables
		int nLen = 0;
		int nRepeat = 0;
		int nIndex = 0;
		int nCurCode = 0;
		while (nIndex < nNumLiteralCodes + nNumDistanceCodes)
		{
			if ((nCurCode = GetHuffmanCodeWord(&oCodeLenCodeTable)) == EOF)
			{
				// TO DO: Error "Bad dynamic code table in flate stream"
				MemUtilsFree(oCodeLenCodeTable.pCodes);
				return false;
			}
			if (nCurCode == 16)
			{
				if ((nRepeat = GetCodeWord(2)) == EOF)
				{
					// TO DO: Error "Bad dynamic code table in flate stream"
					MemUtilsFree(oCodeLenCodeTable.pCodes);
					return false;
				}
				nRepeat += 3;
				if (nIndex + nRepeat > nNumLiteralCodes + nNumDistanceCodes)
				{
					// TO DO: Error "Bad dynamic code table in flate stream"
					MemUtilsFree(oCodeLenCodeTable.pCodes);
					return false;
				}
				for (; nRepeat > 0; --nRepeat)
				{
					m_arrCodeLengths[nIndex++] = nLen;
				}
			}
			else if (nCurCode == 17)
			{
				if ((nRepeat = GetCodeWord(3)) == EOF)
				{
					// TO DO: Error "Bad dynamic code table in flate stream"
					MemUtilsFree(oCodeLenCodeTable.pCodes);
					return false;
				}
				nRepeat += 3;
				if (nIndex + nRepeat > nNumLiteralCodes + nNumDistanceCodes)
				{
					// TO DO: Error "Bad dynamic code table in flate stream"
					MemUtilsFree(oCodeLenCodeTable.pCodes);
					return false;
				}
				nLen = 0;
				for (; nRepeat > 0; --nRepeat)
				{
					m_arrCodeLengths[nIndex++] = 0;
				}
			}
			else if (nCurCode == 18)
			{
				if ((nRepeat = GetCodeWord(7)) == EOF)
				{
					// TO DO: Error "Bad dynamic code table in flate stream"
					MemUtilsFree(oCodeLenCodeTable.pCodes);
					return false;
				}
				nRepeat += 11;
				if (nIndex + nRepeat > nNumLiteralCodes + nNumDistanceCodes)
				{
					// TO DO: Error "Bad dynamic code table in flate stream"
					MemUtilsFree(oCodeLenCodeTable.pCodes);
					return false;
				}
				nLen = 0;
				for (; nRepeat > 0; --nRepeat)
				{
					m_arrCodeLengths[nIndex++] = 0;
				}
			}
			else
			{
				m_arrCodeLengths[nIndex++] = nLen = nCurCode;
			}
		}
		ConvertHuffmanCodes(m_arrCodeLengths, nNumLiteralCodes, &m_oLiteratCodeTable);
		ConvertHuffmanCodes(m_arrCodeLengths + nNumLiteralCodes, nNumDistanceCodes, &m_oDistanceCodeTable);

		MemUtilsFree(oCodeLenCodeTable.pCodes);
		return true;
	}

	// Конвертируем массив <pLengths>, в таблицу Хаффмана(сортируя по значениям)
	void FlateStream::ConvertHuffmanCodes(int *pLengths, int nCount, FlateHuffmanTable *pTable)
	{
		// Ищем макимальный элемент
		pTable->nMaxLen = 0;
		for (int nValue = 0; nValue < nCount; ++nValue)
		{
			if (pLengths[nValue] > pTable->nMaxLen)
			{
				pTable->nMaxLen = pLengths[nValue];
			}
		}

		// Выделяем место для таблицы
		int nTableSize = 1 << pTable->nMaxLen;
		pTable->pCodes = (FlateHuffmanCode *)MemUtilsMallocArray(nTableSize, sizeof(FlateHuffmanCode));

		// Очищаем таблицу
		for (int nIndex = 0; nIndex < nTableSize; ++nIndex)
		{
			pTable->pCodes[nIndex].nLength = 0;
			pTable->pCodes[nIndex].nValue = 0;
		}

		// Заполняем таблицу
		int nLen = 0, nSkip = 0, nCode = 0;
		for (nLen = 1, nCode = 0, nSkip = 2; nLen <= pTable->nMaxLen; ++nLen, nCode <<= 1, nSkip <<= 1)
		{
			for (int nValue = 0; nValue < nCount; ++nValue)
			{
				if (pLengths[nValue] == nLen)
				{
					// bit-reverse code
					int nReverseCode = 0;
					int nTemp = nCode;
					for (int nIndex = 0; nIndex < nLen; ++nIndex)
					{
						nReverseCode = (nReverseCode << 1) | (nTemp & 1);
						nTemp >>= 1;
					}

					for (int nIndex = nReverseCode; nIndex < nTableSize; nIndex += nSkip)
					{
						pTable->pCodes[nIndex].nLength = (unsigned short)nLen;
						pTable->pCodes[nIndex].nValue  = (unsigned short)nValue;
					}
					++nCode;
				}
			}
		}
	}

	int FlateStream::GetHuffmanCodeWord(FlateHuffmanTable *pTable)
	{
		while (m_nCodeSize < pTable->nMaxLen)
		{
			int nChar = 0;
			if ((nChar = m_pStream->GetChar()) == EOF)
			{
				break;
			}
			m_nCodeBuffer |= (nChar & 0xff) << m_nCodeSize;
			m_nCodeSize += 8;
		}
		FlateHuffmanCode *pHuffCode = &pTable->pCodes[m_nCodeBuffer & ((1 << pTable->nMaxLen) - 1)];
		if (m_nCodeSize == 0 || m_nCodeSize < pHuffCode->nLength || pHuffCode->nLength == 0)
		{
			return EOF;
		}
		m_nCodeBuffer >>= pHuffCode->nLength;
		m_nCodeSize -= pHuffCode->nLength;
		return (int)pHuffCode->nValue;
	}

	int FlateStream::GetCodeWord(int nBits)
	{
		int nChar = 0;
		while (m_nCodeSize < nBits)
		{
			if ((nChar = m_pStream->GetChar()) == EOF)
				return EOF;
			m_nCodeBuffer |= (nChar & 0xff) << m_nCodeSize;
			m_nCodeSize += 8;
		}
		nChar = m_nCodeBuffer & ((1 << nBits) - 1);
		m_nCodeBuffer >>= nBits;
		m_nCodeSize -= nBits;
		return nChar;
	}

	//---------------------------------------------------------------------------------------------------------------
	// EOFStream
	//---------------------------------------------------------------------------------------------------------------

	EOFStream::EOFStream(Stream *pStream) :
		FilterStream(pStream)
	{
	}

	EOFStream::~EOFStream()
	{
		delete m_pStream;
	}

	//---------------------------------------------------------------------------------------------------------------
	// FixedLengthEncoder
	//---------------------------------------------------------------------------------------------------------------

	FixedLengthEncoder::FixedLengthEncoder(Stream *pStream, int nLength) :
		FilterStream(pStream)
	{
		m_nLength = nLength;
		m_nCount = 0;
	}

	FixedLengthEncoder::~FixedLengthEncoder()
	{
		if (m_pStream->IsEncoder())
			delete m_pStream;
	}

	void FixedLengthEncoder::Reset()
	{
		m_pStream->Reset();
		m_nCount = 0;
	}

	int FixedLengthEncoder::GetChar()
	{
		if (m_nLength >= 0 && m_nCount >= m_nLength)
			return EOF;
		++m_nCount;
		return m_pStream->GetChar();
	}

	int FixedLengthEncoder::LookChar()
	{
		if (m_nLength >= 0 && m_nCount >= m_nLength)
			return EOF;
		return m_pStream->GetChar();
	}

	bool FixedLengthEncoder::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(true);
	}

	//---------------------------------------------------------------------------------------------------------------
	// ASCIIHexEncoder
	//---------------------------------------------------------------------------------------------------------------

	ASCIIHexEncoder::ASCIIHexEncoder(Stream *pStream) :
		FilterStream(pStream)
	{
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;
		m_nLineLength = 0;
		m_bEOF = false;
	}

	ASCIIHexEncoder::~ASCIIHexEncoder()
	{
		if (m_pStream->IsEncoder())
			delete m_pStream;
	}

	void ASCIIHexEncoder::Reset()
	{
		m_pStream->Reset();
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;
		m_nLineLength = 0;
		m_bEOF = false;
	}

	bool ASCIIHexEncoder::FillBuffer()
	{
		static char *c_sHex = "0123456789abcdef";

		if (m_bEOF)
		{
			return false;
		}
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;

		int nChar = 0;
		if ((nChar = m_pStream->GetChar()) == EOF)
		{
			*m_pBufferEnd++ = '>';
			m_bEOF = true;
		}
		else
		{
			if (m_nLineLength >= 64)
			{
				*m_pBufferEnd++ = '\n';
				m_nLineLength = 0;
			}
			*m_pBufferEnd++ = c_sHex[(nChar >> 4) & 0x0f];
			*m_pBufferEnd++ = c_sHex[nChar & 0x0f];
			m_nLineLength += 2;
		}
		return true;
	}

	//---------------------------------------------------------------------------------------------------------------
	// ASCII85Encoder
	//---------------------------------------------------------------------------------------------------------------

	ASCII85Encoder::ASCII85Encoder(Stream *pStream) :
		FilterStream(pStream)
	{
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;
		m_nLineLength = 0;
		m_bEOF = false;
	}

	ASCII85Encoder::~ASCII85Encoder()
	{
		if (m_pStream->IsEncoder())
			delete m_pStream;
	}

	void ASCII85Encoder::Reset()
	{
		m_pStream->Reset();
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;
		m_nLineLength = 0;
		m_bEOF = false;
	}

	bool ASCII85Encoder::FillBuffer()
	{
		unsigned long unTemp = 0;
		char arrB[5];
		int arrC[4] ={ 0, 0, 0, 0 };
		int nLen = 0;

		if (m_bEOF)
		{
			return false;
		}
		arrC[0] = m_pStream->GetChar();
		arrC[1] = m_pStream->GetChar();
		arrC[2] = m_pStream->GetChar();
		arrC[3] = m_pStream->GetChar();
		m_pBufferPointer = m_pBufferEnd = m_sBuffer;
		if (arrC[3] == EOF)
		{
			if (arrC[0] == EOF)
			{
				nLen = 0;
				unTemp = 0;
			}
			else
			{
				if (arrC[1] == EOF)
				{
					nLen = 1;
					unTemp = arrC[0] << 24;
				}
				else if (arrC[2] == EOF)
				{
					nLen = 2;
					unTemp = (arrC[0] << 24) | (arrC[1] << 16);
				}
				else
				{
					nLen = 3;
					unTemp = (arrC[0] << 24) | (arrC[1] << 16) | (arrC[2] << 8);
				}
				for (int nIndex = 4; nIndex >= 0; --nIndex)
				{
					arrB[nIndex] = (char)(unTemp % 85 + 0x21);
					unTemp /= 85;
				}
				for (int nIndex = 0; nIndex <= nLen; ++nIndex)
				{
					*m_pBufferEnd++ = arrB[nIndex];
					if (++m_nLineLength == 65)
					{
						*m_pBufferEnd++ = '\n';
						m_nLineLength = 0;
					}
				}
			}
			*m_pBufferEnd++ = '~';
			*m_pBufferEnd++ = '>';
			m_bEOF = true;
		}
		else
		{
			unTemp = (arrC[0] << 24) | (arrC[1] << 16) | (arrC[2] << 8) | arrC[3];
			if (unTemp == 0)
			{
				*m_pBufferEnd++ = 'z';
				if (++m_nLineLength == 65)
				{
					*m_pBufferEnd++ = '\n';
					m_nLineLength = 0;
				}
			}
			else
			{
				for (int nIndex = 4; nIndex >= 0; --nIndex)
				{
					arrB[nIndex] = (char)(unTemp % 85 + 0x21);
					unTemp /= 85;
				}
				for (int nIndex = 0; nIndex <= 4; ++nIndex)
				{
					*m_pBufferEnd++ = arrB[nIndex];
					if (++m_nLineLength == 65)
					{
						*m_pBufferEnd++ = '\n';
						m_nLineLength = 0;
					}
				}
			}
		}
		return true;
	}

	//---------------------------------------------------------------------------------------------------------------
	// RunLengthEncoder
	//---------------------------------------------------------------------------------------------------------------

	RunLengthEncoder::RunLengthEncoder(Stream *pStream) :
		FilterStream(pStream)
	{
		m_pBufferPointer = m_pBufferEnd = m_pNextEnd = m_sBuffer;
		m_bEOF = false;
	}

	RunLengthEncoder::~RunLengthEncoder()
	{
		if (m_pStream->IsEncoder())
			delete m_pStream;
	}

	void RunLengthEncoder::Reset()
	{
		m_pStream->Reset();
		m_pBufferPointer = m_pBufferEnd = m_pNextEnd = m_sBuffer;
		m_bEOF = false;
	}

	//
	// После выполнения функции FillBuffer, m_sBuffer[] выглядит следующим образом:
	//   +-----+--------------+-----------------+--
	//   + tag | ... data ... | next 0, 1, or 2 |
	//   +-----+--------------+-----------------+--
	//    ^                    ^                 ^
	//    m_pBufferPointer     m_pBufferEnd      m_pNextEnd
	//
	bool RunLengthEncoder::FillBuffer()
	{
		if (m_bEOF)
			return false;

		// считываем два байта
		int nChar1 = 0, nChar2 = 0;
		if (m_pNextEnd < m_pBufferEnd + 1)
		{
			if ((nChar1 = m_pStream->GetChar()) == EOF)
			{
				m_bEOF = true;
				return false;
			}
		}
		else
		{
			nChar1 = m_pBufferEnd[0] & 0xff;
		}
		if (m_pNextEnd < m_pBufferEnd + 2)
		{
			if ((nChar2 = m_pStream->GetChar()) == EOF)
			{
				m_bEOF = true;
				m_sBuffer[0] = 0;
				m_sBuffer[1] = nChar1;
				m_pBufferPointer = m_sBuffer;
				m_pBufferEnd = &m_sBuffer[2];
				return true;
			}
		}
		else
		{
			nChar2 = m_pBufferEnd[1] & 0xff;
		}

		int nChar = 0;
		if (nChar1 == nChar2)
		{
			int nLen = 2;
			while (nLen < 128 && (nChar = m_pStream->GetChar()) == nChar1)
				++nLen;
			m_sBuffer[0] = (char)(257 - nLen);
			m_sBuffer[1] = nChar1;
			m_pBufferEnd = &m_sBuffer[2];
			if (nChar == EOF)
			{
				m_bEOF = true;
			}
			else if (nLen < 128)
			{
				m_sBuffer[2] = nChar;
				m_pNextEnd = &m_sBuffer[3];
			}
			else
			{
				m_pNextEnd = m_pBufferEnd;
			}


		}
		else
		{
			m_sBuffer[1] = nChar1;
			m_sBuffer[2] = nChar2;
			int nLen = 2;
			while (nLen < 128)
			{
				if ((nChar = m_pStream->GetChar()) == EOF)
				{
					m_bEOF = true;
					break;
				}
				++nLen;
				m_sBuffer[nLen] = nChar;
				if (m_sBuffer[nLen] == m_sBuffer[nLen - 1])
					break;
			}
			if (m_sBuffer[nLen] == m_sBuffer[nLen - 1])
			{
				m_sBuffer[0] = (char)(nLen - 2 - 1);
				m_pBufferEnd = &m_sBuffer[nLen - 1];
				m_pNextEnd = &m_sBuffer[nLen + 1];
			}
			else
			{
				m_sBuffer[0] = (char)(nLen - 1);
				m_pBufferEnd = m_pNextEnd = &m_sBuffer[nLen + 1];
			}
		}
		m_pBufferPointer = m_sBuffer;
		return true;
	}
}
