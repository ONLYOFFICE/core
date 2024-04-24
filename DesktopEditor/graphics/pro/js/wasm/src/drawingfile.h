/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#ifndef _WASM_GRAPHICS_
#define _WASM_GRAPHICS_

#include "../../../../../common/File.h"
#include "../../../../pro/officedrawingfile.h"
#include "../../../../../../XpsFile/XpsFile.h"
#include "../../../../../../DjVuFile/DjVu.h"
#include "../../../../../../PdfFile/PdfFile.h"
#include "../../../../../../HtmlRenderer/include/HTMLRendererText.h"
#include "serialize.h"

class CGraphicsFileDrawing
{
private:
	IOfficeDrawingFile* pReader;
	NSFonts::IApplicationFonts* pApplicationFonts;
	NSFonts::IFontManager* pFontManager;
	NSHtmlRenderer::CHTMLRendererText* pTextRenderer;
	int nType;
public:
	CGraphicsFileDrawing(NSFonts::IApplicationFonts* pFonts)
	{
		pReader = NULL;
		pTextRenderer = NULL;
		pApplicationFonts = pFonts;
		pApplicationFonts->AddRef();

		pFontManager = pApplicationFonts->GenerateFontManager();
		NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
		pFontCache->SetStreams(pApplicationFonts->GetStreams());
		pFontCache->SetCacheSize(8);
		pFontManager->SetOwnerCache(pFontCache);
		
		nType = -1;
	}
	~CGraphicsFileDrawing()
	{
		RELEASEOBJECT(pReader);
		RELEASEOBJECT(pTextRenderer);
		RELEASEOBJECT(pFontManager);
		RELEASEINTERFACE(pApplicationFonts);
		nType = -1;
	}
	bool Open(BYTE* data, DWORD length, int _nType, const char* password)
	{
		nType = _nType;
		if (nType == 0)
			pReader = new CPdfFile(pApplicationFonts);
		else if (nType == 1)
			pReader = new CDjVuFile(pApplicationFonts);
		else if (nType == 2)
			pReader = new CXpsFile(pApplicationFonts);
		if (!pReader)
			return false;
		std::wstring sPassword = L"";
		if (password)
		{
			std::string sPass(password);
			sPassword = UTF8_TO_U(sPass);
		}
		return pReader->LoadFromMemory(data, length, L"", sPassword, sPassword);
	}
	int GetErrorCode()
	{
		if (!pReader)
			return -1;
		if (nType == 0)
			// диапозон ошибки от 0 до 10
			return ((CPdfFile*)pReader)->GetError();
		return 0; // errNone
	}
	int GetPagesCount()
	{
		return pReader->GetPagesCount();
	}
	int GetMaxRefID()
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetMaxRefID();
		return 0;
	}
	void GetPageInfo(int nPageIndex, int& nWidth, int& nHeight, int& nPageDpiX, int& nRotate)
	{
		double dPageDpiX, dPageDpiY;
		double dWidth, dHeight;
		pReader->GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
		if (nType == 2)
		{
			dWidth    = dWidth    / 25.4 * 96.0;
			dHeight   = dHeight   / 25.4 * 96.0;
			dPageDpiX = dPageDpiX / 25.4 * 96.0;
		}
		if (nType == 0)
			nRotate = ((CPdfFile*)pReader)->GetRotate(nPageIndex);
		nWidth    = dWidth;
		nHeight   = dHeight;
		nPageDpiX = dPageDpiX;
	}
	std::wstring GetFont(const std::wstring& sFontName)
	{
		std::wstring sFontFile;
		if (nType == 0)
			sFontFile = ((CPdfFile*)pReader)->GetFontPath(sFontName);
		return sFontFile;
	}
	BYTE* GetPage(int nPageIndex, int nRasterW, int nRasterH, int nBackgroundColor)
	{
		return pReader->ConvertToPixels(nPageIndex, nRasterW, nRasterH, true, pFontManager, nBackgroundColor, (nBackgroundColor == 0xFFFFFF) ? false : true);
	}
	BYTE* GetGlyphs(int nPageIndex)
	{
		if (NULL == pTextRenderer)
			pTextRenderer = new NSHtmlRenderer::CHTMLRendererText();

		pTextRenderer->Init(pReader, 8);
		pReader->DrawPageOnRenderer(pTextRenderer, nPageIndex, NULL);

		return pTextRenderer->GetBuffer();
	}
	BYTE* GetLinks(int nPageIndex)
	{
		return pReader->GetLinks(nPageIndex);
	}
	BYTE* GetStructure()
	{
		return pReader->GetStructure();
	}
	BYTE* GetInteractiveFormsInfo()
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetWidgets();
		return NULL;
	}
	BYTE* GetWidgetFonts(int nTypeFonts)
	{
		if (nType == 0)
		{
			if (nTypeFonts == 1)
				return ((CPdfFile*)pReader)->GetWidgetEmbeddedFonts();
			if (nTypeFonts == 2)
				return ((CPdfFile*)pReader)->GetWidgetStandardFonts();
		}
		return NULL;
	}
	BYTE* GetAnnots(int nPageIndex = -1)
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetAnnots(nPageIndex);
		return NULL;
	}
	BYTE* GetAPWidget  (int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nWidget  = -1, const char* sView  = NULL, const char* sBView = NULL)
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetAPWidget(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nWidget, sView, sBView);
		return NULL;
	}
	BYTE* GetButtonIcon(int nBackgroundColor, int nPageIndex, bool bBase64, int nBWidget = -1, const char* sIView = NULL)
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetButtonIcon(nBackgroundColor, nPageIndex, bBase64, nBWidget, sIView);
		return NULL;
	}
	BYTE* GetAPAnnots  (int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nAnnot   = -1, const char* sView  = NULL)
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetAPAnnots(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nAnnot, sView);
		return NULL;
	}
	std::wstring GetInfo()
	{
		return pReader->GetInfo();
	}
	bool IsNeedCMap()
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->IsNeedCMap();
		return false;
	}
	void SetCMapData(BYTE* pData, DWORD nSizeData)
	{
		if (nType == 0)
			((CPdfFile*)pReader)->SetCMapMemory(pData, nSizeData);
	}
	void DestroyText()
	{
		RELEASEOBJECT(pTextRenderer);
	}
};

#endif // _WASM_GRAPHICS_
