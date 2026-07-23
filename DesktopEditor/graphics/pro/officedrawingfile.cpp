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

#include "./officedrawingfile.h"
#include "./Graphics.h"

CBgraFrame* GetFrame(IOfficeDrawingFile* pFile, int nPageIndex, int nRasterW, int nRasterH, bool bIsFlip, bool bIsSwapRGB,
                     NSFonts::IFontManager* pFonts = NULL, int nBackgroundColor = 0xFFFFFF, bool bIsDarkMode = false, int nBackgroundOpacity = 0xFF)
{
    NSFonts::IFontManager *pFontManager = pFonts;

    if (!pFontManager)
    {
        NSFonts::IApplicationFonts* pApplicationFonts = pFile->GetFonts();
        if (!pApplicationFonts)
            return NULL;

        pFontManager = pApplicationFonts->GenerateFontManager();
        NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
        pFontCache->SetStreams(pApplicationFonts->GetStreams());
        pFontManager->SetOwnerCache(pFontCache);
    }
    else
    {
        pFontManager->AddRef();
    }

    NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
    pRenderer->SetFontManager(pFontManager);

    double dPageDpiX, dPageDpiY;
    double dWidth, dHeight;
    pFile->GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

    int nWidth  = (nRasterW > 0) ? nRasterW : ((int)dWidth  * 96 / dPageDpiX);
    int nHeight = (nRasterH > 0) ? nRasterH : ((int)dHeight * 96 / dPageDpiY);

	BYTE* pBgraData = new(std::nothrow) BYTE[nWidth * nHeight * 4];
    if (!pBgraData)
    {
        RELEASEINTERFACE(pFontManager);
        RELEASEOBJECT(pRenderer);
        return NULL;
    }

    if (0xFFFFFF == nBackgroundColor)
    {
        memset(pBgraData, 0xff, nWidth * nHeight * 4);
    }
    else
    {
        unsigned int nColor = (unsigned int)nBackgroundColor;
        nColor = (nBackgroundOpacity << 24) | nColor;

        unsigned int nSize = (unsigned int)(nWidth * nHeight);
        unsigned int* pTemp = (unsigned int*)pBgraData;
        for (unsigned int i = 0; i < nSize; ++i)
            *pTemp++ = nColor;
    }

    CBgraFrame* pFrame = new CBgraFrame();
    pFrame->put_Data(pBgraData);
    pFrame->put_Width(nWidth);
    pFrame->put_Height(nHeight);
    pFrame->put_Stride((bIsFlip ? 4 : -4) * nWidth);

    pRenderer->CreateFromBgraFrame(pFrame);
    pRenderer->SetSwapRGB(bIsSwapRGB);

    if (bIsDarkMode)
        pRenderer->CommandLong(c_nDarkMode, 1);

    if (odftPDF == pFile->GetType())
    {
        dWidth  *= 25.4 / dPageDpiX;
        dHeight *= 25.4 / dPageDpiY;
    }

    pRenderer->put_Width(dWidth);
    pRenderer->put_Height(dHeight);

    bool bBreak = false;
    pFile->DrawPageOnRenderer(pRenderer, nPageIndex, &bBreak);

    RELEASEINTERFACE(pFontManager);
    RELEASEOBJECT(pRenderer);

    return pFrame;
}

unsigned char* IOfficeDrawingFile::ConvertToPixels(int nPageIndex, int nRasterW, int nRasterH,
                                                   bool bIsFlip, NSFonts::IFontManager* pFonts, int nBackgroundColor, bool bIsDarkMode, int nBackgroundOpacity)
{
    CBgraFrame* pFrame = GetFrame(this, nPageIndex, nRasterW, nRasterH, bIsFlip, true, pFonts, nBackgroundColor, bIsDarkMode, nBackgroundOpacity);
    if (!pFrame)
        return NULL;

    unsigned char* pData = pFrame->get_Data();
    pFrame->ClearNoAttack();
    RELEASEOBJECT(pFrame);

    return pData;
}

void IOfficeDrawingFile::ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType, const int nRasterW, const int nRasterH,
                                         bool bIsFlip, NSFonts::IFontManager* pFonts, int nBackgroundColor, bool bIsDarkMode, int nBackgroundOpacity)
{
    CBgraFrame* pFrame = GetFrame(this, nPageIndex, nRasterW, nRasterH, bIsFlip, false, pFonts, nBackgroundColor, bIsDarkMode, nBackgroundOpacity);
    if (!pFrame)
        return;

    pFrame->SaveFile(path, nImageType);
    RELEASEOBJECT(pFrame);
}

void IOfficeDrawingFile::SetPainter(IOfficeDrawingFilePainter* pPainter) {}
