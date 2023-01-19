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
#pragma once

#include "libdjvu/DjVuDocument.h"
#include "libdjvu/DjVuImage.h"
#include "libdjvu/GBitmap.h"
#include "libdjvu/GScaler.h"
#include "libdjvu/IFFByteStream.h"
#include "libdjvu/BSByteStream.h"
#include "libdjvu/DataPool.h"
#include "libdjvu/DjVuText.h"
#include "libdjvu/DjVmNav.h"
#include "libdjvu/IW44Image.h"

#define ZIP_NO_COMPRESSION         0
#define ZIP_BEST_SPEED             1
#define ZIP_BEST_COMPRESSION       9
#define ZIP_DEFAULT_COMPRESSION  (-1)

#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/graphics/IRenderer.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"

class CDjVuFileImplementation
{
private:
    std::wstring     m_wsTempDirectory;
	GP<DjVuDocument> m_pDoc;
    NSFonts::IApplicationFonts* m_pApplicationFonts;

public:

    CDjVuFileImplementation(NSFonts::IApplicationFonts* pFonts);
	~CDjVuFileImplementation();
    NSFonts::IApplicationFonts* GetFonts();

	bool         LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXmlOptions = L"");
	bool         LoadFromMemory(BYTE* data, DWORD length, const std::wstring& wsXmlOptions = L"");
	void         Close();
	std::wstring GetTempDirectory() const;
	void         SetTempDirectory(const std::wstring& wsDirectory);
	int          GetPagesCount() const;
	void         GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) const;
	void         DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak);
    void         ConvertToPdf(const std::wstring& wsDstPath);
    std::wstring GetInfo();

    BYTE*        GetStructure();
    BYTE*        GetPageGlyphs(int nPageIndex);
    BYTE*        GetPageLinks (int nPageIndex);

private:

	void               CreateFrame(IRenderer* pRenderer, GP<DjVuImage>& pImage, int nPage, XmlUtils::CXmlNode& oText);
    void               CreatePdfFrame(IRenderer* pRenderer, GP<DjVuImage>& pImage, int nPage, XmlUtils::CXmlNode& oText);
	XmlUtils::CXmlNode ParseText(GP<DjVuImage> pPage);
	void               TextToRenderer(IRenderer* pRenderer, XmlUtils::CXmlNode text, double koef, bool isView = true);
	void               DrawPageText(IRenderer* pRenderer, double* pdCoords, const std::wstring& wsText);
	void               ParseCoords(const std::wstring& wsCoordsStr, double* pdCoords, double dKoef);
};
