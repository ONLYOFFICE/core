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

#ifndef _BUILD_NATIVE_TEXT_MEASURER_EMBED_H_
#define _BUILD_NATIVE_TEXT_MEASURER_EMBED_H_

#include "../js_internal/js_base.h"

#if defined(__ANDROID__) || defined(_IOS)
#define SUPPORT_HARFBUZZ_SHAPER
#endif

using namespace NSJSBase;
class JS_DECL CTextMeasurerEmbed : public CJSEmbedObject
{
private:
	void* m_hyphen_engine;

public:
	CTextMeasurerEmbed();
	~CTextMeasurerEmbed();

public:
	JSSmart<CJSValue> FT_Malloc(JSSmart<CJSValue> typed_array_or_len);
	JSSmart<CJSValue> FT_Free(JSSmart<CJSValue> pointer);

	JSSmart<CJSValue> FT_Init();
	JSSmart<CJSValue> FT_Set_TrueType_HintProp(JSSmart<CJSValue> library, JSSmart<CJSValue> tt_interpreter);

	JSSmart<CJSValue> FT_Open_Face(JSSmart<CJSValue> library, JSSmart<CJSValue> memory, JSSmart<CJSValue> size, JSSmart<CJSValue> face_index);
	JSSmart<CJSValue> FT_Open_Face2(JSSmart<CJSValue> library, JSSmart<CJSValue> array, JSSmart<CJSValue> face_index);
	JSSmart<CJSValue> FT_GetFaceInfo(JSSmart<CJSValue> face);

	JSSmart<CJSValue> FT_Load_Glyph(JSSmart<CJSValue> face, JSSmart<CJSValue> gid, JSSmart<CJSValue> mode);
	JSSmart<CJSValue> FT_Get_Glyph_Measure_Params(JSSmart<CJSValue> face, JSSmart<CJSValue> is_vector);
	JSSmart<CJSValue> FT_Get_Glyph_Render_Params(JSSmart<CJSValue> face, JSSmart<CJSValue> render_mode);
	JSSmart<CJSValue> FT_Get_Glyph_Render_Buffer(JSSmart<CJSValue> face, JSSmart<CJSValue> size);

	JSSmart<CJSValue> FT_Set_Transform(JSSmart<CJSValue> face, JSSmart<CJSValue> xx, JSSmart<CJSValue> yx, JSSmart<CJSValue> xy, JSSmart<CJSValue> yy);
	JSSmart<CJSValue> FT_Set_Char_Size(JSSmart<CJSValue> face, JSSmart<CJSValue> char_width, JSSmart<CJSValue> char_height, JSSmart<CJSValue> hres, JSSmart<CJSValue> vres);
	JSSmart<CJSValue> FT_SetCMapForCharCode(JSSmart<CJSValue> face, JSSmart<CJSValue> unicode);
	JSSmart<CJSValue> FT_GetKerningX(JSSmart<CJSValue> face, JSSmart<CJSValue> gid1, JSSmart<CJSValue> gid2);
	JSSmart<CJSValue> FT_GetFaceMaxAdvanceX(JSSmart<CJSValue> face);

#ifdef SUPPORT_HARFBUZZ_SHAPER
	JSSmart<CJSValue> HB_LanguageFromString(JSSmart<CJSValue> language_bcp_47);

	JSSmart<CJSValue> HB_ShapeText(JSSmart<CJSValue> face, JSSmart<CJSValue> font, JSSmart<CJSValue> text,
								   JSSmart<CJSValue> nFeatures, JSSmart<CJSValue> nScript, JSSmart<CJSValue> nDirection, JSSmart<CJSValue> nLanguage);

	JSSmart<CJSValue> HB_FontMalloc();
	JSSmart<CJSValue> HB_FontFree(JSSmart<CJSValue> font);
#endif

	JSSmart<CJSValue> Hyphen_SetCacheSize(JSSmart<CJSValue> size);
	JSSmart<CJSValue> Hyphen_Word(JSSmart<CJSValue> lang, JSSmart<CJSValue> word);
	JSSmart<CJSValue> Hyphen_IsDictionaryExist(JSSmart<CJSValue> lang);

	DECLARE_EMBED_METHODS
};

#endif // _BUILD_NATIVE_TEXT_MEASURER_EMBED_H_
