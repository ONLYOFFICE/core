/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
                return new CCacheImage((CApplicationFonts*)pFonts);
            return new CCacheImage((CApplicationFonts*)pFonts, sFile);
        }
    }

    namespace NSFilesCache
    {
        IImageFilesCache* Create(NSFonts::IApplicationFonts* pFonts)
        {
            return new CImageFilesCache((CApplicationFonts*)pFonts);
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
}

