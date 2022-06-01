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

//desktop_fetch

//polyfill

//string_utf8

//module

Module.CreateLibrary = Module["_ASC_FT_Init"];
Module.FT_Set_TrueType_HintProp = Module["_ASC_FT_Set_TrueType_HintProp"];

Module.FT_Load_Glyph = Module["_FT_Load_Glyph"];
Module.FT_Set_Transform = Module["_ASC_FT_Set_Transform"];
Module.FT_Set_Char_Size = Module["_FT_Set_Char_Size"];

Module.FT_SetCMapForCharCode = Module["_ASC_FT_SetCMapForCharCode"];
Module.FT_GetKerningX = Module["_ASC_FT_GetKerningX"];
Module.FT_GetFaceMaxAdvanceX = Module["_ASC_FT_GetFaceMaxAdvanceX"];

Module.FT_Done_Face = Module["_FT_Done_Face"];
Module.HP_FontFree = Module["_ASC_HP_FontFree"];

Module.CreateNativeStream = function(stream)
{
	var fontStreamPointer = Module["_ASC_FT_Malloc"](stream.size);
	Module["HEAP8"].set(stream.data, fontStreamPointer);
	return { asc_marker: true, data: fontStreamPointer, len: stream.size};
};

function CReturnObject()
{
	this.error = 0;
	this.freeObj = 0;
}
CReturnObject.prototype.free = function()
{
	Module["_ASC_FT_Free"](this.freeObj);
};

let g_return_obj = new CReturnObject();
let g_return_obj_count = new CReturnObject();
g_return_obj_count.count = 0;

Module.GetFaceInfo = function(face, reader)
{
	let pointer = Module["_ASC_FT_GetFaceInfo"](face);
	if (!pointer)
	{
		g_return_obj.error = 1;
		return g_return_obj;
	}

	var len_buffer = Math.min((Module["HEAP8"].length - pointer), 1000); //max 230 symbols on name & style
	reader.init(new Uint8Array(Module["HEAP8"].buffer, pointer, len_buffer));

	g_return_obj.freeObj = pointer;
	g_return_obj.error = 0;
	return g_return_obj;
};

Module.FT_Open_Face = function(library, memory, len, face_index)
{
	return Module["_ASC_FT_Open_Face"](library, memory, len, face_index);
};

Module.FT_Get_Glyph_Measure_Params = function(face, vector_worker, reader)
{
	let pointer = Module["_ASC_FT_Get_Glyph_Measure_Params"](face, vector_worker ? 1 : 0);
	if (!pointer)
	{
		g_return_obj_count.error = 1;
		return g_return_obj_count;
	}

	let len = !vector_worker ? 15 : Module["HEAP32"][pointer >> 2];
	if (vector_worker)
		len = Module["HEAP32"][pointer >> 2];

	reader.init(new Uint8Array(Module["HEAP8"].buffer, pointer + 4, 4 * (len - 1)));
	g_return_obj_count.freeObj = pointer;
	g_return_obj_count.count = len;
	g_return_obj_count.error = 0;
	return g_return_obj_count;
};

Module.FT_Get_Glyph_Render_Params = function(face, render_mode, reader)
{
	let pointer = Module["_ASC_FT_Get_Glyph_Render_Params"](face, render_mode);
	if (!pointer)
	{
		g_return_obj_count.error = 1;
		return g_return_obj_count;
	}

	reader.init(new Uint8Array(Module["HEAP8"].buffer, pointer, 4 * 6));

	g_return_obj.freeObj = pointer;
	g_return_obj.error = 0;
	return g_return_obj;
};

Module.FT_Get_Glyph_Render_Buffer = function(face, rasterInfo)
{
	var pointer = Module["_ASC_FT_Get_Glyph_Render_Buffer"](face);
	return new Uint8Array(Module["HEAP8"].buffer, pointer, rasterInfo.pitch * rasterInfo.rows);
};

Module.HP_ShapeText = function(fontFile, text, features, script, direction, language, reader)
{
	if (!Module.hb_cache_languages[language])
	{
		let langBuffer = language.toUtf8();
		var langPointer = Module["_malloc"](langBuffer.length);
		Module["HEAP8"].set(langBuffer, langBuffer);

		Module.hb_cache_languages[language] = langPointer;
	}

	let pointer = Module["_ASC_HP_ShapeText"](fontFile.m_pFace, fontFile.m_pHBFont, text, features, script, direction, AscFonts.hb_cache_languages[language]);
	if (!pointer)
	{
		g_return_obj_count.error = 1;
		return g_return_obj_count;
	}

	let buffer = Module["HEAP8"];
	let len = (buffer[pointer + 3] & 0xFF) << 24 | (buffer[pointer + 2] & 0xFF) << 16 | (buffer[pointer + 1] & 0xFF) << 8 | (buffer[pointer] & 0xFF);

	reader.init(buffer, pointer + 4, len - 4);
	fontFile.m_pHBFont = reader.readPointer64();

	g_return_obj_count.freeObj = pointer;
	g_return_obj_count.count = (len - 12) / 26;
	g_return_obj_count.error = 0;
	return g_return_obj_count;
};

// this memory is not freed
Module.hb_cache_languages = {};
