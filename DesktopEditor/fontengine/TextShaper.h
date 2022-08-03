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
#ifndef _BUILD_TEXT_MANAGER_FONTMANAGER_H_
#define _BUILD_TEXT_MANAGER_FONTMANAGER_H_

#include "../graphics/pro/Fonts.h"

namespace NSShaper
{
    class GRAPHICS_DECL CExternalPointer
    {
    public:
        unsigned char* Data;
        unsigned int Len;

    public:
        CExternalPointer();
        virtual ~CExternalPointer();

    public:
        virtual void WriteInt(unsigned char* data, const int& value);
        virtual void WriteUInt(unsigned char* data, const unsigned int& value);

        virtual void Alloc(const unsigned int& len);
        virtual void Free();
    };

    GRAPHICS_DECL void* FT_Library_Init();
    GRAPHICS_DECL void FT_Library_Destroy(void* library);
    GRAPHICS_DECL void FT_Library_UnReference(void* library);
    GRAPHICS_DECL void FT_Library_Reference(void* library);

    GRAPHICS_DECL int FT_Set_TrueType_HintProp(void* library, unsigned int interpreter_version);

    GRAPHICS_DECL void* FT_Open_Face(void* library, unsigned char* memory, unsigned int size, int face_index);
    GRAPHICS_DECL void FT_Done_Face(void* face);
    GRAPHICS_DECL void FT_Done_Face_With_Library(void* face);

    GRAPHICS_DECL bool FT_GetFaceInfo(void* face, CExternalPointer* result);
    GRAPHICS_DECL bool FT_Get_Glyph_Measure_Params(void* face, bool is_vector, CExternalPointer* result);

    GRAPHICS_DECL bool FT_Get_Glyph_Render_Params(void* face, int render_mode, CExternalPointer* result);
    GRAPHICS_DECL unsigned char* FT_Get_Glyph_Render_Buffer(void* face);

    GRAPHICS_DECL void FT_Glyph_Get_CBox(void* glyph, unsigned int bbox_mode, CExternalPointer* result);

    GRAPHICS_DECL int FT_Load_Glyph(void* face, unsigned int glyph_index, int load_flags);
    GRAPHICS_DECL void FT_Set_Transform(void* face, int xx, int yx, int xy, int yy);
    GRAPHICS_DECL int FT_Set_Char_Size(void* face, int char_width, int char_height, unsigned int hres, unsigned int vres);

    GRAPHICS_DECL unsigned int FT_SetCMapForCharCode(void* face, unsigned int unicode);
    GRAPHICS_DECL int FT_GetKerningX(void* face, unsigned int prev_gid, unsigned int gid);
    GRAPHICS_DECL int FT_GetFaceMaxAdvanceX(void* face);

#ifdef SUPPORT_HARFBUZZ_SHAPER
    GRAPHICS_DECL void* HB_LanguageFromString(const std::string language_bcp_47);

    GRAPHICS_DECL void HB_ShapeText(void* face, void*& font, char* text,
                                    unsigned int nFeatures, unsigned int nScript, unsigned int nDirection, void* nLanguage,
                                    CExternalPointer* result, bool bIsJSVersion = false);

    GRAPHICS_DECL void HB_ShapeText(void* face, void*& font, const std::string& text,
                                    unsigned int nFeatures, unsigned int nScript, unsigned int nDirection, void* nLanguage,
                                    CExternalPointer* result, bool bIsJSVersion = false);

    GRAPHICS_DECL void HB_FontFree(void* font);
#endif
}

#endif // _BUILD_TEXT_MANAGER_FONTMANAGER_H_
