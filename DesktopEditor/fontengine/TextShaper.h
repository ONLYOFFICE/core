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
	GRAPHICS_DECL int FT_Get_Glyph_Render_BufferSize(void* face);

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
