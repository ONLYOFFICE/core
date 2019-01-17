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
//#include <QCoreApplication>

#include "../../fontengine/ApplicationFonts.h"
#include "../../graphics/GraphicsRenderer.h"

int main(int argc, char *argv[])
{
    CApplicationFonts oFonts;
    oFonts.Initialize();

    CBgraFrame oFrame;
    int nRasterW = 300;
    int nRasterH = 300;

    double dWidthMM = 25.4 * nRasterW / 96;
    double dHeightMM = 25.4 * nRasterH / 96;

    if (false)
    {
        nRasterW *= 2;
        nRasterH *= 2;
    }

    oFrame.put_Width(nRasterW);
    oFrame.put_Height(nRasterH);
    oFrame.put_Stride(-4 * nRasterW);

    BYTE* pDataRaster = new BYTE[4 * nRasterW * nRasterH];
    memset(pDataRaster, 0xFF, 4 * nRasterW * nRasterH);
    oFrame.put_Data(pDataRaster);

    CFontManager* pManager = oFonts.GenerateFontManager();
    pManager->SetSubpixelRendering(true, false);
    CImageFilesCache* pCache = new CImageFilesCache();

    CGraphicsRenderer oRenderer;
    oRenderer.SetFontManager(pManager);
    oRenderer.SetImageCache(pCache);

    oRenderer.CreateFromBgraFrame(&oFrame);
    oRenderer.SetSwapRGB(false);

    oRenderer.put_Width(dWidthMM);
    oRenderer.put_Height(dHeightMM);

    oRenderer.put_FontPath(L"Lobster-Regular.ttf");
    oRenderer.put_FontFaceIndex(0);
    oRenderer.put_FontSize(18);

    oRenderer.CommandDrawText(L"Lobster", 10, dHeightMM / 2, 0, 0);

    oFrame.SaveFile(L"D:\\111.png", 4);

    return 0;
}
