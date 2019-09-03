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
#ifndef _GRAPHICS_EXPORTS_IMAGE_H_
#define _GRAPHICS_EXPORTS_IMAGE_H_

#include "../../common/Types.h"
#include "./Fonts.h"
#include "./../IRenderer.h"
#include "./../Image.h"

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

        virtual void SetApplicationFonts(NSFonts::IApplicationFonts* pApplicationFonts) = 0;
    };

    namespace NSFilesCache
    {
        GRAPHICS_DECL IImageFilesCache* Create(NSFonts::IApplicationFonts* pFonts);
    }

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
        virtual bool DrawOnRenderer(IRenderer* pRenderer, double dX, double dY, double dWidth, double dHeight) = 0;
        virtual void Close() = 0;
        virtual void GetBounds(double* pdX, double* pdY, double* pdW, double* pdH) = 0;
        virtual int GetType() = 0;
        virtual void ConvertToRaster(const wchar_t* wsOutFilePath, unsigned int unFileType, int nWidth, int nHeight = -1) = 0;
        virtual NSFonts::IFontManager* get_FontManager() = 0;
    };

    GRAPHICS_DECL IMetaFile* Create(NSFonts::IApplicationFonts *pAppFonts);
}

#endif // _GRAPHICS_EXPORTS_IMAGE_H_
