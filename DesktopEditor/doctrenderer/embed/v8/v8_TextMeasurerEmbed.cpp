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

// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../TextMeasurerEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSTextMeasurerEmbed
{
#define CURRENTWRAPPER CTextMeasurerEmbed

#ifdef SUPPORT_HARFBUZZ_SHAPER
	FUNCTION_WRAPPER_V8_1(_HB_LanguageFromString, HB_LanguageFromString)
	FUNCTION_WRAPPER_V8_7(_HB_ShapeText, HB_ShapeText)
	FUNCTION_WRAPPER_V8_0(_HB_FontMalloc, HB_FontMalloc)
	FUNCTION_WRAPPER_V8_1(_HB_FontFree, HB_FontFree)
#endif
	FUNCTION_WRAPPER_V8_1(_FT_Malloc, FT_Malloc)
	FUNCTION_WRAPPER_V8_1(_FT_Free, FT_Free)
	FUNCTION_WRAPPER_V8_0(_FT_Init, FT_Init)
	FUNCTION_WRAPPER_V8_2(_FT_Set_TrueType_HintProp, FT_Set_TrueType_HintProp)
	FUNCTION_WRAPPER_V8_4(_FT_Open_Face, FT_Open_Face)
	FUNCTION_WRAPPER_V8_3(_FT_Open_Face2, FT_Open_Face2)
	FUNCTION_WRAPPER_V8_1(_FT_GetFaceInfo, FT_GetFaceInfo)
	FUNCTION_WRAPPER_V8_3(_FT_Load_Glyph, FT_Load_Glyph)
	FUNCTION_WRAPPER_V8_2(_FT_Get_Glyph_Measure_Params, FT_Get_Glyph_Measure_Params)
	FUNCTION_WRAPPER_V8_2(_FT_Get_Glyph_Render_Params, FT_Get_Glyph_Render_Params)
	FUNCTION_WRAPPER_V8_2(_FT_Get_Glyph_Render_Buffer, FT_Get_Glyph_Render_Buffer)
	FUNCTION_WRAPPER_V8_5(_FT_Set_Transform, FT_Set_Transform)
	FUNCTION_WRAPPER_V8_5(_FT_Set_Char_Size, FT_Set_Char_Size)
	FUNCTION_WRAPPER_V8_2(_FT_SetCMapForCharCode, FT_SetCMapForCharCode)
	FUNCTION_WRAPPER_V8_3(_FT_GetKerningX, FT_GetKerningX)
	FUNCTION_WRAPPER_V8_1(_FT_GetFaceMaxAdvanceX, FT_GetFaceMaxAdvanceX)
	FUNCTION_WRAPPER_V8_1(_Hyphen_SetCacheSize, Hyphen_SetCacheSize)
	FUNCTION_WRAPPER_V8_2(_Hyphen_Word, Hyphen_Word)
	FUNCTION_WRAPPER_V8_1(_Hyphen_IsDictionaryExist, Hyphen_IsDictionaryExist)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

#ifdef SUPPORT_HARFBUZZ_SHAPER
		NSV8Objects::Template_Set(result, "HB_LanguageFromString",	_HB_LanguageFromString);
		NSV8Objects::Template_Set(result, "HB_ShapeText",	_HB_ShapeText);
		NSV8Objects::Template_Set(result, "HB_FontMalloc",	_HB_FontMalloc);
		NSV8Objects::Template_Set(result, "HB_FontFree",	_HB_FontFree);
#endif
		NSV8Objects::Template_Set(result, "FT_Malloc",	_FT_Malloc);
		NSV8Objects::Template_Set(result, "FT_Free",	_FT_Free);
		NSV8Objects::Template_Set(result, "FT_Init",	_FT_Init);
		NSV8Objects::Template_Set(result, "FT_Set_TrueType_HintProp",	_FT_Set_TrueType_HintProp);
		NSV8Objects::Template_Set(result, "FT_Open_Face",	_FT_Open_Face);
		NSV8Objects::Template_Set(result, "FT_Open_Face2",	_FT_Open_Face2);
		NSV8Objects::Template_Set(result, "FT_GetFaceInfo",	_FT_GetFaceInfo);
		NSV8Objects::Template_Set(result, "FT_Load_Glyph",	_FT_Load_Glyph);
		NSV8Objects::Template_Set(result, "FT_Get_Glyph_Measure_Params",	_FT_Get_Glyph_Measure_Params);
		NSV8Objects::Template_Set(result, "FT_Get_Glyph_Render_Params",	_FT_Get_Glyph_Render_Params);
		NSV8Objects::Template_Set(result, "FT_Get_Glyph_Render_Buffer",	_FT_Get_Glyph_Render_Buffer);
		NSV8Objects::Template_Set(result, "FT_Set_Transform",	_FT_Set_Transform);
		NSV8Objects::Template_Set(result, "FT_Set_Char_Size",	_FT_Set_Char_Size);
		NSV8Objects::Template_Set(result, "FT_SetCMapForCharCode",	_FT_SetCMapForCharCode);
		NSV8Objects::Template_Set(result, "FT_GetKerningX",	_FT_GetKerningX);
		NSV8Objects::Template_Set(result, "FT_GetFaceMaxAdvanceX",	_FT_GetFaceMaxAdvanceX);
		NSV8Objects::Template_Set(result, "Hyphen_SetCacheSize",	_Hyphen_SetCacheSize);
		NSV8Objects::Template_Set(result, "Hyphen_Word",	_Hyphen_Word);
		NSV8Objects::Template_Set(result, "Hyphen_IsDictionaryExist",	_Hyphen_IsDictionaryExist);

		return handle_scope.Escape(result);
	}
}

class CTextMeasurerEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSTextMeasurerEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
	}
};

CJSEmbedObjectAdapterBase* CTextMeasurerEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CTextMeasurerEmbedAdapter();
	return m_pAdapter;
}

std::string CTextMeasurerEmbed::getName() { return "CTextMeasurerEmbed"; }

CJSEmbedObject* CTextMeasurerEmbed::getCreator()
{
	return new CTextMeasurerEmbed();
}
