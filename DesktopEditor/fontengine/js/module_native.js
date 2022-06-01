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

let g_native_engine = CreateNativeTextMeasurer();

Module.CreateLibrary = g_native_engine["FT_Init"];
Module.FT_Set_TrueType_HintProp = g_native_engine["FT_Set_TrueType_HintProp"];

Module.FT_Load_Glyph = g_native_engine["FT_Load_Glyph"];
Module.FT_Set_Transform = g_native_engine["FT_Set_Transform"];
Module.FT_Set_Char_Size = g_native_engine["FT_Set_Char_Size"];

Module.FT_SetCMapForCharCode = g_native_engine["FT_SetCMapForCharCode"];
Module.FT_GetKerningX = g_native_engine["FT_GetKerningX"];
Module.FT_GetFaceMaxAdvanceX = g_native_engine["FT_GetFaceMaxAdvanceX"];

Module.FT_Done_Face = function()
{
	// GC
};
Module.HP_FontFree = function()
{
	// GC
}

Module.CreateNativeStream = function(stream)
{
	return { asc_marker: true, data: stream.data, len: stream.size};
};

function CReturnObject()
{
	this.error = 0;
}
CReturnObject.prototype.free = function()
{
};

let g_return_obj = new CReturnObject();
let g_return_obj_count = new CReturnObject();
g_return_obj_count.count = 0;

Module.GetFaceInfo = function(face, reader)
{
	var data = g_native_engine["FT_GetFaceInfo"](face);
	if (!data)
	{
		g_return_obj.error = 1;
		return g_return_obj;
	}

	g_return_obj.error = 0;
	reader.init(data, 0, data.length);
	return g_return_obj;
};

Module.FT_Open_Face = function(library, memory, len, face_index)
{
	return g_native_engine["FT_Open_Face2"](library, memory, face_index);
};

Module.FT_Get_Glyph_Measure_Params = function(face, vector_worker, reader)
{
	var data = g_native_engine["FT_Get_Glyph_Measure_Params"](face, vector_worker ? true : false);
	if (!data)
	{
		g_return_obj_count.error = 1;
		return g_return_obj_count;
	}

	reader.init(new Uint8Array(data, 0, data.length));
	g_return_obj_count.count = data.length;
	g_return_obj_count.error = 0;
	return g_return_obj_count;
};

Module.FT_Get_Glyph_Render_Params = function(face, render_mode, reader)
{
	var data = g_native_engine["FT_Get_Glyph_Render_Params"](face, render_mode);
	if (!data)
	{
		g_return_obj.error = 1;
		return g_return_obj;
	}

	g_return_obj.error = 0;
	reader.init(data, 0, data.length);
	return g_return_obj;
};

Module.FT_Get_Glyph_Render_Buffer = function(face, rasterInfo)
{
	return g_native_engine["FT_Get_Glyph_Render_Buffer"](face, rasterInfo.pitch * rasterInfo.rows);
};

Module.HP_ShapeText = function(fontFile, text, features, script, direction, language, reader)
{
	if (!Module.hb_cache_languages[language])
	{
		Module.hb_cache_languages[language] = g_native_engine["HB_LanguageFromString"];
	}

	if (!fontFile.m_pHBFont)
		fontFile.m_pHBFont = g_native_engine["FT_Malloc"](0);

	let data = Module["HB_ShapeText"](fontFile.m_pFace, fontFile.m_pHBFont, text, features, script, direction, AscFonts.hb_cache_languages[language]);
	if (!data)
	{
		g_return_obj_count.error = 1;
		return g_return_obj_count;
	}

	reader.init(new Uint8Array(data, 0, data.length));
	let len = reader.readUInt();
	let fontPointer = reader.readPointer64(); // just skip

	g_return_obj_count.count = (len - 12) / 26;
	g_return_obj_count.error = 0;
	return g_return_obj_count;
};

// this memory is freed by GC
Module.hb_cache_languages = {};
