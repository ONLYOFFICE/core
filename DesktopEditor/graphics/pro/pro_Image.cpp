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

#include "../ImageFilesCache.h"
#include "../../common/StringExt.h"

NSImages::IImagesMemoryStorage* g_global_images_memory_storage = NULL;

namespace NSImages
{
    namespace NSCacheImage
    {
        ICacheImage* Create(NSFonts::IApplicationFonts* pFonts, const std::wstring& sFile)
        {
            if (sFile.empty())
				return new CCacheImage(pFonts);
			return new CCacheImage(pFonts, sFile);
        }
    }

    namespace NSFilesCache
    {
        IImageFilesCache* Create(NSFonts::IApplicationFonts* pFonts)
        {
			return new CImageFilesCache(pFonts);
        }
    }

	IImageStream::IImageStream() : NSBase::CBaseRefCounter() {}
	IImageStream::~IImageStream() {}
	namespace NSStream
	{
		IImageStream* Create()
		{
			return new CImageStream();
		}
	}

	IImagesMemoryStorage::IImagesMemoryStorage()  {}
	IImagesMemoryStorage::~IImagesMemoryStorage() {}

	namespace NSApplicationImageStream
	{
	// default global memory storage
	class CDefaultGlobalImagesMemoryStorage : public IImagesMemoryStorage
	{
	private:
		std::map<std::wstring, IImageStream*> m_mapStreams;
		int m_nInternalCounter;

	public:
		CDefaultGlobalImagesMemoryStorage()
		{
			m_nInternalCounter = 1;
		}
		virtual ~CDefaultGlobalImagesMemoryStorage()
		{
			Clear();
		}

		virtual bool Add(const std::wstring& id, BYTE* data, LONG size, bool bClear = false)
		{
			std::wstring sFile = id;
			NSStringExt::Replace(sFile, L"\\", L"/");
			std::map<std::wstring, IImageStream*>::iterator it = m_mapStreams.find(sFile);
			if (it != m_mapStreams.end())
				return false;

			IImageStream* pStream = NSImages::NSStream::Create();
			pStream->CreateFromMemory(data, size, bClear);
			m_mapStreams.insert(std::pair<std::wstring, IImageStream*>(sFile, pStream));
			return true;
		}
		virtual bool Remove(const std::wstring& id)
		{
			std::wstring sFile = id;
			NSStringExt::Replace(sFile, L"\\", L"/");
			std::map<std::wstring, IImageStream*>::iterator it = m_mapStreams.find(sFile);
			if (it == m_mapStreams.end())
				return false;

			RELEASEINTERFACE(it->second);
			m_mapStreams.erase(it);
			return true;
		}
		virtual void Clear()
		{
			for (std::map<std::wstring, IImageStream*>::iterator it = m_mapStreams.begin(); it != m_mapStreams.end(); it++)
				RELEASEINTERFACE(it->second);
			m_mapStreams.clear();
		}

		virtual IImageStream* Get(const std::wstring& id)
		{
			std::wstring sFile = id;
			NSStringExt::Replace(sFile, L"\\", L"/");
			std::map<std::wstring, IImageStream*>::iterator it = m_mapStreams.find(sFile);
			return it != m_mapStreams.end() ? it->second : NULL;
		}

		virtual std::wstring GenerateId()
		{
			std::wstring sKey;
			do
			{
				sKey = L"storage_internal_" + std::to_wstring(m_nInternalCounter++);
			} while (m_mapStreams.find(sKey) != m_mapStreams.end());
			return sKey;
		}
	};

	GRAPHICS_DECL IImagesMemoryStorage* CreateDefaultGlobalImagesMemoryStorage()
	{
		return new CDefaultGlobalImagesMemoryStorage();
	}

	GRAPHICS_DECL IImagesMemoryStorage* GetGlobalImagesMemoryStorage()
	{
		return g_global_images_memory_storage;
	}
	GRAPHICS_DECL void SetGlobalImagesMemoryStorage(IImagesMemoryStorage* pStorage)
	{
		if (g_global_images_memory_storage != pStorage)
			RELEASEINTERFACE(g_global_images_memory_storage);

		g_global_images_memory_storage = pStorage;
	}
	}
}

#include "../../raster/JBig2/source/Encoder/jbig2enc.h"
#include "../../raster/JBig2/source/LeptonLib/allheaders.h"
namespace NSImages
{
    class CPixJbig2_private
    {
    public:
        Pix* m_pix;

    public:
        CPixJbig2_private()
        {
            m_pix = NULL;
        }
    };

    CPixJbig2::CPixJbig2()
    {
        m_internal = new CPixJbig2_private();
    }
    CPixJbig2::~CPixJbig2()
    {
        Destroy();
        RELEASEOBJECT(m_internal);
    }

    bool CPixJbig2::Create(int width, int height, int depth)
    {
        m_internal->m_pix = pixCreate(width, height, depth);
        return (m_internal->m_pix) ? true : false;
    }

    void CPixJbig2::SetPixel(int x, int y, int val)
    {
        pixSetPixel(m_internal->m_pix, x, y, val);
    }

    void CPixJbig2::Destroy()
    {
        pixDestroy(&m_internal->m_pix);
        m_internal->m_pix = NULL;
    }

    void* CPixJbig2::native()
    {
        return (void*)m_internal->m_pix;
    }

    class CJbig2Context_private
    {
    public:
        jbig2ctx* m_context;

    public:
        CJbig2Context_private()
        {
            m_context = NULL;
        }
        ~CJbig2Context_private()
        {
        }
    };

    CJbig2Context::CJbig2Context()
    {
        m_internal = new CJbig2Context_private();
    }
    CJbig2Context::~CJbig2Context()
    {
        Destroy();
        RELEASEOBJECT(m_internal);
    }

    bool CJbig2Context::IsInit()
    {
        return (m_internal->m_context != NULL) ? true : false;
    }

    void CJbig2Context::Init(float thresh, float weight, int xres, int yres, bool full_headers, int refine_level)
    {
        m_internal->m_context = jbig2_init(thresh, weight, xres, yres, full_headers, refine_level);
    }

    BYTE* CJbig2Context::PagesComplete(int* const length)
    {
        return jbig2_pages_complete(m_internal->m_context, length);
    }

    BYTE* CJbig2Context::ProducePage(int page_no, int xres, int yres, int *const length)
    {
        return jbig2_produce_page(m_internal->m_context, page_no, xres, yres, length);
    }

    void CJbig2Context::AddPage(CPixJbig2* pix)
    {
        jbig2_add_page(m_internal->m_context, (Pix*)pix->native());
    }

    void CJbig2Context::Destroy()
    {
        if (!m_internal->m_context)
            return;

        jbig2_destroy(m_internal->m_context);
        m_internal->m_context = NULL;
    }
}

