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

#include "./officedrawingfile.h"
#include "./Graphics.h"

CBgraFrame* GetFrame(IOfficeDrawingFile* pFile, int nPageIndex, int nRasterW, int nRasterH, bool bIsFlip, bool bIsSwapRGB,
                     NSFonts::IFontManager* pFonts = NULL, int nBackgroundColor = 0xFFFFFF, bool bIsDarkMode = false)
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

    BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
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
        nColor = 0xFF000000 | nColor;

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
                                                   bool bIsFlip, NSFonts::IFontManager* pFonts, int nBackgroundColor, bool bIsDarkMode)
{
    CBgraFrame* pFrame = GetFrame(this, nPageIndex, nRasterW, nRasterH, bIsFlip, true, pFonts, nBackgroundColor, bIsDarkMode);
    if (!pFrame)
        return NULL;

    unsigned char* pData = pFrame->get_Data();
    pFrame->ClearNoAttack();
    RELEASEOBJECT(pFrame);

    return pData;
}

void IOfficeDrawingFile::ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType, const int nRasterW, const int nRasterH,
                                         bool bIsFlip, NSFonts::IFontManager* pFonts, int nBackgroundColor, bool bIsDarkMode)
{
    CBgraFrame* pFrame = GetFrame(this, nPageIndex, nRasterW, nRasterH, bIsFlip, false, pFonts, nBackgroundColor, bIsDarkMode);
    if (!pFrame)
        return;

    pFrame->SaveFile(path, nImageType);
    RELEASEOBJECT(pFrame);
}
