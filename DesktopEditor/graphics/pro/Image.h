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
#ifndef _GRAPHICS_EXPORTS_IMAGE_H_
#define _GRAPHICS_EXPORTS_IMAGE_H_

#include "../../common/Types.h"
#include "./../IRenderer.h"
#include "./../Image.h"
#include "./Fonts.h"
#include "./MemoryStorage.h"

namespace NSImages
{
	class GRAPHICS_DECL ICacheImage : public NSBase::CBaseRefCounter
	{
	public:
		ICacheImage() : NSBase::CBaseRefCounter() {}
		virtual ~ICacheImage() {}

		virtual Aggplus::CImage* GetImage() { return NULL; }
	};

	namespace NSCacheImage
	{
		GRAPHICS_DECL ICacheImage* Create(NSFonts::IApplicationFonts* pFonts, const std::wstring& sFile = L"");
	}

	class GRAPHICS_DECL IImageFilesCache : public NSBase::CBaseRefCounter
	{
	public:
		IImageFilesCache() : NSBase::CBaseRefCounter() {}
		virtual ~IImageFilesCache() {}

		virtual void Clear() = 0;

		virtual ICacheImage* Lock(const std::wstring& strFile) = 0;
		virtual bool UnLock(const std::wstring& strFile) = 0;

		virtual void SetApplicationFonts(NSFonts::IApplicationFonts* pApplicationFonts) = 0;
	};

	namespace NSFilesCache
	{
		GRAPHICS_DECL IImageFilesCache* Create(NSFonts::IApplicationFonts* pFonts);
	}

	typedef NSMemoryStorage::IMemoryStream IImageStream;
	typedef NSMemoryStorage::IMemoryStorage<IImageStream> IImagesMemoryStorage;

	namespace NSStream
	{
		GRAPHICS_DECL IImageStream* Create();
	}

	namespace NSApplicationImageStream
	{
		// create default
		GRAPHICS_DECL IImagesMemoryStorage* CreateDefaultGlobalImagesMemoryStorage();

		// without AddRef!!!
		GRAPHICS_DECL IImagesMemoryStorage* GetGlobalImagesMemoryStorage();
		GRAPHICS_DECL void SetGlobalImagesMemoryStorage(IImagesMemoryStorage* pStorage);
	} // namespace NSApplicationImageStream

	class CPixJbig2_private;
	class GRAPHICS_DECL CPixJbig2
	{
	private:
		CPixJbig2_private* m_internal;

	public:
		CPixJbig2();
		~CPixJbig2();

		bool Create(int width, int height, int depth);
		void SetPixel(int x, int y, int val);
		void Destroy();
		void* native();
	};

	class CJbig2Context_private;
	class GRAPHICS_DECL CJbig2Context
	{
	private:
		CJbig2Context_private* m_internal;

	public:
		CJbig2Context();
		~CJbig2Context();

		bool IsInit();
		void Init(float thresh, float weight, int xres, int yres, bool full_headers, int refine_level);
		BYTE* PagesComplete(int* const length);
		BYTE* ProducePage(int page_no, int xres, int yres, int *const length);
		void AddPage(CPixJbig2* pix);
		void Destroy();
	};
}

#ifndef GRAPHICS_DISABLE_METAFILE
namespace MetaFile
{
	const int c_lMetaWmf = 0x01;
	const int c_lMetaEmf = 0x02;
	const int c_lMetaSvg = 0x04;
	const int c_lMetaSvm = 0x05;

	class GRAPHICS_DECL IMetaFile : public NSBase::CBaseRefCounter
	{
	public:
		IMetaFile(NSFonts::IApplicationFonts *pAppFonts) {}
		virtual ~IMetaFile() {}

		virtual bool LoadFromFile(const wchar_t* wsFilePath) = 0;
		virtual bool LoadFromBuffer(BYTE* pBuffer, unsigned int unSize) = 0;
		virtual bool LoadFromString(const std::wstring& data) = 0;
		virtual bool DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight) = 0;
		virtual void Close() = 0;
		virtual void GetBounds(double* pdX, double* pdY, double* pdW, double* pdH) = 0;
		virtual int GetType() = 0;
		virtual void ConvertToRaster(const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight = -1) = 0;
		virtual NSFonts::IFontManager* get_FontManager() = 0;

		virtual std::wstring ConvertToSvg(unsigned int unWidth = 0, unsigned int unHeight = 0) = 0;
		virtual void SetTempDirectory(const std::wstring& dir) = 0;

		// For tests
	#ifdef METAFILE_SUPPORT_WMF_EMF
		virtual void ConvertToXml(const wchar_t *wsFilePath) = 0;
		virtual void ConvertToXmlAndRaster(const wchar_t *wsXmlFilePath, const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight = -1) = 0;
		virtual bool LoadFromXmlFile(const wchar_t* wsFilePath) = 0;
		virtual void ConvertToEmf(const wchar_t *wsFilePath) = 0;
	#endif
	};

	GRAPHICS_DECL IMetaFile* Create(NSFonts::IApplicationFonts *pAppFonts);
}
#endif

#endif // _GRAPHICS_EXPORTS_IMAGE_H_
