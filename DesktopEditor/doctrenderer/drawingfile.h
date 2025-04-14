/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
#ifndef DRAWINGFILE_H
#define DRAWINGFILE_H

#include "./common.h"
#include <stddef.h>

#include "../../PdfFile/PdfFile.h"
#include "../../XpsFile/XpsFile.h"
#include "../../DjVuFile/DjVu.h"
#include "../graphics/pro/js/wasm/src/serialize.h"
#include "../graphics/pro/js/wasm/src/HTMLRendererText.h"
#include "../../DocxRenderer/DocxRenderer.h"

#define CHECKER_FILE_BUFFER_LEN 4096

class CDrawingFile
{
private:
	NSFonts::IApplicationFonts*			m_pApplicationFonts;
	NSFonts::IFontManager*				m_pFontManager;
	NSHtmlRenderer::CHTMLRendererText*	m_pTextRenderer;
	NSDocxRenderer::IImageStorage*		m_pImageStorage;

	IOfficeDrawingFile* m_pFile;
	int m_nType = -1;

	bool m_bIsExternalFile;

public:
	CDrawingFile(NSFonts::IApplicationFonts* pFonts)
	{
		m_pApplicationFonts = pFonts;
		ADDREFINTERFACE(m_pApplicationFonts);

		m_pFontManager = m_pApplicationFonts->GenerateFontManager();
		NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
		pFontCache->SetStreams(m_pApplicationFonts->GetStreams());
		pFontCache->SetCacheSize(8);
		m_pFontManager->SetOwnerCache(pFontCache);

		m_pTextRenderer = NULL;
		m_pImageStorage = NULL;

		m_pFile = NULL;
		m_bIsExternalFile = false;
	}
	~CDrawingFile()
	{
		if (!m_bIsExternalFile)
			RELEASEOBJECT(m_pFile);
		RELEASEOBJECT(m_pTextRenderer);
		RELEASEOBJECT(m_pFontManager);
		RELEASEINTERFACE(m_pApplicationFonts);
		RELEASEOBJECT(m_pImageStorage);
	}

	void SetInternalFile(IOfficeDrawingFile* pFile)
	{
		m_pFile = pFile;
		if (!m_pFile)
			return;

		m_bIsExternalFile = true;
		switch (m_pFile->GetType())
		{
		case odftPDF:
			m_nType = 0;
			break;
		case odftDJVU:
			m_nType = 1;
			break;
		case odftXPS:
			m_nType = 2;
			break;
		default:
			break;
		}
	}

public:
	static void InitFontsGlobalStorage()
	{
		NSFonts::NSApplicationFontStream::SetGlobalMemoryStorage(NSFonts::NSApplicationFontStream::CreateDefaultGlobalMemoryStorage());
	}

	static NSFonts::IFontsMemoryStorage* GetFontsStorage()
	{
		return NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
	}

public:
	bool OpenFile(const std::wstring& sFile, const std::wstring& sPassword)
	{
		CloseFile();

		int nType = DetectFormat(sFile);

		switch (nType)
		{
		case 0:
		{
			m_pFile = new CPdfFile(m_pApplicationFonts);
			if (!m_pFile->LoadFromFile(sFile, L"", sPassword, sPassword))
			{
				if (4 != ((CPdfFile*)m_pFile)->GetError())
				{
					RELEASEOBJECT(m_pFile);
				}
				else
					m_nType = 0;
			}
			else
				m_nType = 0;
			break;
		}
		case 1:
		{
			m_pFile = new CDjVuFile(m_pApplicationFonts);
			if (!m_pFile->LoadFromFile(sFile, L"", sPassword, sPassword))
			{
				RELEASEOBJECT(m_pFile);
			}
			else
				m_nType = 1;
			break;
		}
		case 2:
		{
			m_pFile = new CXpsFile(m_pApplicationFonts);
			if (!m_pFile->LoadFromFile(sFile, L"", sPassword, sPassword))
			{
				RELEASEOBJECT(m_pFile);
			}
			else
				m_nType = 2;
		}
		default:
			break;
		}

		return m_pFile ? true : false;
	}
	bool OpenFile(BYTE* data, LONG size, const std::wstring& sPassword)
	{
		CloseFile();

		int nType = DetectFormat(data, size);
		switch (nType)
		{
			case 0:
			{
				m_pFile = new CPdfFile(m_pApplicationFonts);
				if (!m_pFile->LoadFromMemory(data, size, L"", sPassword, sPassword))
				{
					if (4 != ((CPdfFile*)m_pFile)->GetError())
					{
						RELEASEOBJECT(m_pFile);
					}
					else
						m_nType = 0;
				}
				else
					m_nType = 0;
				break;
			}
			case 1:
			{
				m_pFile = new CDjVuFile(m_pApplicationFonts);
				if (!m_pFile->LoadFromMemory(data, size, L"", sPassword, sPassword))
				{
					RELEASEOBJECT(m_pFile);
				}
				else
					m_nType = 1;
				break;
			}
			case 2:
			{
				m_pFile = new CXpsFile(m_pApplicationFonts);
				if (!m_pFile->LoadFromMemory(data, size, L"", sPassword, sPassword))
				{
					RELEASEOBJECT(m_pFile);
				}
				else
					m_nType = 2;
			}
			default:
				break;
		}

		return m_pFile ? true : false;
	}

	void CloseFile()
	{
		if (m_pFile)
			RELEASEOBJECT(m_pFile);

		m_nType = -1;
	}

	int GetType()
	{
		return m_nType;
	}

	int GetErrorCode()
	{
		if (!m_pFile)
			return -1;

		if (0 == m_nType)
			return ((CPdfFile*)m_pFile)->GetError();

		return 0;
	}

	BYTE* GetInfo()
	{
		NSWasm::CData oRes;
		oRes.SkipLen();

		oRes.AddInt(GetMaxRefID());

		int pages_count = GetPagesCount();
		oRes.AddInt(pages_count);
		for (int page = 0; page < pages_count; ++page)
		{
			int nW = 0;
			int nH = 0;
			int nDpi = 0;
			int nRotate = 0;
			GetPageInfo(page, nW, nH, nDpi, nRotate);
			oRes.AddInt(nW);
			oRes.AddInt(nH);
			oRes.AddInt(nDpi);
			oRes.AddInt(nRotate);
		}
		std::wstring wsInfo = m_pFile->GetInfo();
		std::string sInfo = U_TO_UTF8(wsInfo);
		oRes.WriteString((BYTE*)sInfo.c_str(), sInfo.length());

		oRes.WriteLen();
		BYTE* bRes = oRes.GetBuffer();
		oRes.ClearWithoutAttack();
		return bRes;
	}

	BYTE* GetPixmap(int nPageIndex, int nRasterW, int nRasterH, int nBackgroundColor)
	{
		if (!m_pFile)
			return NULL;
		return m_pFile->ConvertToPixels(nPageIndex, nRasterW, nRasterH, true, m_pFontManager, nBackgroundColor, (nBackgroundColor == 0xFFFFFF) ? false : true);
	}
	BYTE* SplitPages(int* arrPageIndex, int nLength)
	{
		if (m_nType == 0)
			return ((CPdfFile*)m_pFile)->SplitPages(arrPageIndex, nLength);
		return NULL;
	}
	bool MergePages(BYTE* data, LONG size, int nMaxID, const std::string& sPrefixForm)
	{
		if (m_nType == 0)
			return ((CPdfFile*)m_pFile)->MergePages(data, size, nMaxID, sPrefixForm);
		return false;
	}

	BYTE* GetGlyphs(int nPageIndex)
	{
		if (NULL == m_pTextRenderer)
			m_pTextRenderer = new NSHtmlRenderer::CHTMLRendererText();

		m_pTextRenderer->Init(m_pFile, 8);
		m_pFile->DrawPageOnRenderer(m_pTextRenderer, nPageIndex, NULL);

		return m_pTextRenderer->GetBuffer();
	}
	BYTE* GetLinks(int nPageIndex)
	{
		return m_pFile->GetLinks(nPageIndex);
	}
	BYTE* GetStructure()
	{
		return m_pFile->GetStructure();
	}
	BYTE* GetInteractiveFormsInfo()
	{
		if (m_nType == 0)
			return ((CPdfFile*)m_pFile)->GetWidgets();
		return NULL;
	}
	BYTE* GetInteractiveFormsFonts(int nTypeFonts)
	{
		if (m_nType == 0)
		{
			if (nTypeFonts == 1)
				return ((CPdfFile*)m_pFile)->GetAnnotEmbeddedFonts();
			if (nTypeFonts == 2)
				return ((CPdfFile*)m_pFile)->GetAnnotStandardFonts();
		}
		return NULL;
	}

	BYTE* GetInteractiveFormsAP(int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nWidget, int nView, int nButtonView)
	{
		if (0 != m_nType)
			return NULL;

		const char* sView = NULL;
		if (nView == 0)
			sView = "N";
		else if (nView == 1)
			sView = "D";
		else if (nView == 2)
			sView = "R";

		const char* sButtonView = NULL;
		if (nButtonView == 0)
			sButtonView = "Off";
		else if (nButtonView == 1)
			sButtonView = "Yes";

		return ((CPdfFile*)m_pFile)->GetAPWidget(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nWidget, sView, sButtonView);
	}
	BYTE* GetButtonIcons(int nBackgroundColor, int nPageIndex, int bBase64, int nButtonWidget, int nIconView)
	{
		if (0 != m_nType)
			return NULL;

		const char* sIconView = NULL;
		if (nIconView == 0)
			sIconView = "I";
		else if (nIconView == 1)
			sIconView = "RI";
		else if (nIconView == 2)
			sIconView = "IX";

		return ((CPdfFile*)m_pFile)->GetButtonIcon(nBackgroundColor, nPageIndex, bBase64 ? true : false, nButtonWidget, sIconView);
	}
	BYTE* GetAnnotationsInfo(int nPageIndex)
	{
		if (m_nType == 0)
			return ((CPdfFile*)m_pFile)->GetAnnots(nPageIndex);
		return NULL;
	}
	BYTE* GetAnnotationsAP(int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nAnnot, int nView)
	{
		if (m_nType != 0)
			return NULL;

		const char* sView = NULL;
		if (nView == 0)
			sView = "N";
		else if (nView == 1)
			sView = "D";
		else if (nView == 2)
			sView = "R";

		return ((CPdfFile*)m_pFile)->GetAPAnnots(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nAnnot, sView);
	}

	void DestroyTextInfo()
	{
		RELEASEOBJECT(m_pTextRenderer);
	}
	bool IsNeedCMap()
	{
		if (m_nType == 0)
			return ((CPdfFile*)m_pFile)->IsNeedCMap();
		return false;
	}
	void SetCMapData(BYTE* data, int size)
	{
		if (m_nType == 0)
			((CPdfFile*)m_pFile)->SetCMapMemory(data, size);
	}
	BYTE* ScanPage(int nPageIndex, int mode)
	{
		if (NULL == m_pImageStorage)
			m_pImageStorage = NSDocxRenderer::CreateWasmImageStorage();

		CDocxRenderer oRenderer(m_pApplicationFonts);
		oRenderer.SetExternalImageStorage(m_pImageStorage);
		oRenderer.SetTextAssociationType(NSDocxRenderer::TextAssociationType::tatParagraphToShape);

		std::vector<std::wstring> arShapes;
		if (0 == mode)
			arShapes = oRenderer.ScanPage(m_pFile, nPageIndex);
		else
			arShapes = oRenderer.ScanPagePptx(m_pFile, nPageIndex);

		int nLen = (int)arShapes.size();

		NSWasm::CData oRes;
		oRes.SkipLen();
		oRes.AddInt(nLen);

		for (int i = 0; i < nLen; ++i)
			oRes.WriteString(arShapes[i]);

		oRes.WriteLen();

		BYTE* res = oRes.GetBuffer();
		oRes.ClearWithoutAttack();
		return res;
	}

	void* GetImageBase64(int rId)
	{
		if (NULL == m_pImageStorage)
			return NULL;
		return m_pImageStorage->GetBase64(rId);
	}
	int GetImageBase64Len(std::string* p)
	{
		return (int)p->length();
	}
	char* GetImageBase64Ptr(std::string* p)
	{
		return (char*)p->c_str();
	}
	void GetImageBase64Free(std::string* p)
	{
		*p = "";
	}

	void SetFontBinary(char* path, BYTE* data, int size)
	{
		NSFonts::IFontsMemoryStorage* pStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
		if (pStorage)
		{
			std::string sPathA(path);
			pStorage->Add(UTF8_TO_U(sPathA), data, size, true);
		}
	}
	int IsFontBinaryExist(char* path)
	{
		NSFonts::IFontsMemoryStorage* pStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
		if (pStorage)
		{
			std::string sPathA(path);
			NSFonts::IFontStream* pStream = pStorage->Get(UTF8_TO_U(sPathA));
			if (pStream)
				return 1;
		}
		return 0;
	}

	BYTE* GetFontBinary(const std::string& sPathA)
	{
		std::wstring sFontName = UTF8_TO_U(sPathA);

		std::wstring sFontFile;
		if (m_nType == 0)
			sFontFile = ((CPdfFile*)m_pFile)->GetFontPath(sFontName);

		if (sFontFile.empty())
			sFontFile = sFontName;

		NSFonts::IFontsMemoryStorage* pStorage = GetFontsStorage();
		if (pStorage)
		{
			NSFonts::IFontStream* pStream = pStorage->Get(sFontFile);
			if (pStream)
			{
				BYTE* pData = NULL;
				LONG lLength = 0;
				pStream->GetMemory(pData, lLength);

				if (pData)
				{
					NSWasm::CData oRes;
					oRes.SkipLen();

					oRes.AddInt(lLength);

					unsigned long long npSubMatrix = (unsigned long long)pData;
					unsigned int npSubMatrix1 = npSubMatrix & 0xFFFFFFFF;
					oRes.AddInt(npSubMatrix1);
					oRes.AddInt(npSubMatrix >> 32);

					oRes.WriteLen();
					BYTE* bRes = oRes.GetBuffer();
					oRes.ClearWithoutAttack();
					return bRes;
				}
			}
		}
		return NULL;
	}

	std::wstring GetFontBinaryNative(const std::wstring& sName)
	{
		if (0 != m_nType)
			return L"";

		return ((CPdfFile*)m_pFile)->GetEmbeddedFontPath(sName);
	}

private:
	int GetPagesCount()
	{
		return m_pFile->GetPagesCount();
	}
	int GetMaxRefID()
	{
		if (m_nType == 0)
			return ((CPdfFile*)m_pFile)->GetMaxRefID();
		return 0;
	}

	void GetPageInfo(int nPageIndex, int& nWidth, int& nHeight, int& nPageDpiX, int& nRotate)
	{
		double dPageDpiX, dPageDpiY;
		double dWidth, dHeight;
		m_pFile->GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
		if (m_nType == 2)
		{
			dWidth    = dWidth    / 25.4 * 96.0;
			dHeight   = dHeight   / 25.4 * 96.0;
			dPageDpiX = dPageDpiX / 25.4 * 96.0;
		}
		if (m_nType == 0)
			nRotate = ((CPdfFile*)m_pFile)->GetRotate(nPageIndex);
		nWidth    = round(dWidth);
		nHeight   = round(dHeight);
		nPageDpiX = dPageDpiX;
	}

	int DetectFormat(const std::wstring& sFile)
	{
		NSFile::CFileBinary oFile;
		if (oFile.OpenFile(sFile))
		{
			LONG size = oFile.GetFileSize();
			if (size > CHECKER_FILE_BUFFER_LEN)
				size = CHECKER_FILE_BUFFER_LEN;

			BYTE* data = new BYTE[size];
			oFile.ReadFile(data, size);

			int nType = DetectFormat(data, size);

			RELEASEARRAYOBJECTS(data);
			return nType;
		}
		return -1;
	}

	int DetectFormat(BYTE* data, LONG size)
	{
		// 0 - PDF
		// 1 - DJVU
		// 2 - XPS
		LONG nSize = size < CHECKER_FILE_BUFFER_LEN ? size : CHECKER_FILE_BUFFER_LEN;
		char* pData = (char*)data;
		for (int i = 0; i < nSize - 5; ++i)
		{
			int nPDF = strncmp(&pData[i], "%PDF-", 5);
			if (!nPDF)
				return 0;
		}
		if ( (8 <= size) && (0x41 == data[0] && 0x54 == data[1] && 0x26 == data[2] && 0x54 == data[3] &&
							0x46 == data[4] && 0x4f == data[5] && 0x52 == data[6] && 0x4d == data[7]))
			return 1;
		return 2;
	}
};

#endif // DRAWINGFILE_H
