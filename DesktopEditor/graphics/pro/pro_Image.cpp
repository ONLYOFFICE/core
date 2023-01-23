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

#include "../ImageFilesCache.h"

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

