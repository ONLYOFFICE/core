/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "./JPXStream2.h"
#include "./MemoryUtils.h"

#include "../../DesktopEditor/raster/BgraFrame.h"

#ifdef USE_GRAPHICS_JPEG2000
#include "../../DesktopEditor/raster/Jp2/J2kFile.h"
#else
#include "../../DesktopEditor/raster/Jp2/openjpeg/opj_bgraframe.h"
#endif

using namespace PdfReader;

JPXStream2::JPXStream2(Stream *pStream) :
    FilterStream(pStream)
{
    m_lCurPos       = 0;
    m_lBufferSize   = 0;
    m_pSourceBuffer = NULL;
}

JPXStream2::~JPXStream2()
{
    close();
    delete str;
}

void JPXStream2::reset()
{
    str->reset();

    // Initialization
    m_lCurPos       = 0;
    m_lBufferSize   = 0;

    unsigned int nAllocSize = 4096;
    unsigned int nStreamSize = 0;

    unsigned char* pBuffer = (unsigned char*)MemUtilsMalloc(nAllocSize);
    int nCurrentChar;
    while (true)
    {
        nCurrentChar = str->getChar();
        if (EOF == nCurrentChar)
            break;

        if (nStreamSize == nAllocSize)
        {
            nAllocSize *= 2;
            pBuffer = (unsigned char*)MemUtilsRealloc(pBuffer, nAllocSize, nStreamSize);
        }

        pBuffer[nStreamSize++] = nCurrentChar;
    }

	BYTE* pBufferPointer;
	int nHeight = 0;
	int nWidth  = 0;
	int nComponentsCount = 0;

	if (!openjpeg::GetData(pBuffer, nStreamSize, pBufferPointer, nComponentsCount, nWidth, nHeight, true))
    {
        MemUtilsFree(pBuffer);
        return;
    }

    MemUtilsFree(pBuffer);

	m_pSourceBuffer = pBufferPointer;
	m_lBufferSize = nComponentsCount * nWidth * nHeight;
}

Stream* JPXStream2::copy()
{
    return new JPXStream2(str->copy());
}

void JPXStream2::close()
{
	if (m_pSourceBuffer)
		openjpeg::DestroyData(m_pSourceBuffer);
}

int JPXStream2::getChar()
{
	int nChar = 0;

	if (m_lCurPos < m_lBufferSize)
		nChar = m_pSourceBuffer[m_lCurPos++];
	else
		return EOF;

	return nChar;
}

int JPXStream2::lookChar()
{
    if (m_lBufferSize > 0 && m_lCurPos < m_lBufferSize)
        return m_pSourceBuffer[m_lCurPos];
    return EOF;
}
GString* JPXStream2::getPSFilter(int psLevel, const char *indent, GBool okToReadStream)
{
    return NULL;
}
GBool JPXStream2::isBinary(GBool bLast)
{
    return str->isBinary(gTrue);
}
void JPXStream2::reduceResolution(int reductionA) {}
StreamKind JPXStream2::getKind() { return strJPX; }

void JPXStream2::getImageParams(int *pBitsPerComponent, StreamColorSpaceMode *peModeCS)
{
    *pBitsPerComponent = 8;
    *peModeCS = streamCSDeviceRGB;
}
