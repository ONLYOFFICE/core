/*
 * (c) Copyright Ascensio System SIA 2010-2023
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

(function(window, undefined) {

var AscFonts = window['AscFonts'];

if (window["NATIVE_EDITOR_ENJINE"])
{
	var immediateArray = [];
	window.setImmediate = function(fn) {
		if (immediateArray)
			immediateArray.push(fn);
		else
			fn();
	};
	window.immediateRun = function() {
		for (var i = 0; i < immediateArray.length; i++)
			immediateArray[i]();
		immediateArray = undefined;
	};
}

var setImmediate = window.setImmediate;

//desktop_fetch

//polyfill

//string_utf8

//module

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

AscFonts.CopyStreamToMemory = function(data, size)
{
	var fontStreamPointer = Module["_ASC_FT_Malloc"](size);
	Module["HEAP8"].set(data, fontStreamPointer);
	return fontStreamPointer;
};

AscFonts.GetUint8ArrayFromPointer = function(pointer, size)
{
	return new Uint8Array(Module["HEAP8"].buffer, pointer, size);
};

function CShapeString(size)
{
	this.size = size;
	this.pointer = Module["_malloc"](size);
}
CShapeString.prototype.getBuffer = function()
{
	return new Uint8Array(Module["HEAPU8"].buffer, this.pointer, this.size);
};
CShapeString.prototype.free = function()
{
	Module["_free"](this.pointer);
};
CShapeString.prototype.set = function(index, value)
{
	Module["HEAPU8"][this.pointer + index] = value;
};

AscFonts.AllocString = function(size)
{
	return new CShapeString(size);
};

AscFonts.FT_CreateLibrary = Module["_ASC_FT_Init"];
AscFonts.FT_Done_Library = Module["_ASC_FT_Done_FreeType"];
AscFonts.FT_Set_TrueType_HintProp = Module["_ASC_FT_Set_TrueType_HintProp"];

AscFonts.FT_Open_Face = Module["_ASC_FT_Open_Face"];
AscFonts.FT_Done_Face = Module["_ASC_FT_Done_Face"];
AscFonts.FT_SetCMapForCharCode = Module["_ASC_FT_SetCMapForCharCode"];
AscFonts.FT_GetKerningX = Module["_ASC_FT_GetKerningX"];
AscFonts.FT_GetFaceMaxAdvanceX = Module["_ASC_FT_GetFaceMaxAdvanceX"];
AscFonts.FT_Set_Transform = Module["_ASC_FT_Set_Transform"];
AscFonts.FT_Set_Char_Size = Module["_ASC_FT_Set_Char_Size"];
AscFonts.FT_GetFaceInfo = function(face, reader)
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

AscFonts.FT_Load_Glyph = Module["_ASC_FT_Load_Glyph"];
AscFonts.FT_SetCMapForCharCode = Module["_ASC_FT_SetCMapForCharCode"];
AscFonts.FT_Get_Glyph_Measure_Params = function(face, vector_worker, reader)
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
AscFonts.FT_Get_Glyph_Render_Params = function(face, render_mode, reader)
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
AscFonts.FT_Get_Glyph_Render_Buffer = function(face, size)
{
	var pointer = Module["_ASC_FT_Get_Glyph_Render_Buffer"](face);
	return new Uint8Array(Module["HEAP8"].buffer, pointer, size);
};

let hb_cache_languages = {};
AscFonts.HB_FontFree = Module["ASC_HB_FontFree"];
AscFonts.HB_ShapeText = function(fontFile, text, features, script, direction, language, reader)
{
	if (!hb_cache_languages[language])
	{
		let langBuffer = language.toUtf8();
		var langPointer = Module["_malloc"](langBuffer.length);
		Module["HEAP8"].set(langBuffer, langBuffer);
		hb_cache_languages[language] = Module["_ASC_HB_LanguageFromString"](langPointer);
		Module["_free"](langPointer);
	}

	let pointer = Module["_ASC_HB_ShapeText"](fontFile["GetFace"](), fontFile["GetHBFont"](), text.pointer, features, script, direction, hb_cache_languages[language]);
	if (!pointer)
	{
		g_return_obj_count.error = 1;
		return g_return_obj_count;
	}

	let buffer = Module["HEAP8"];
	let len = (buffer[pointer + 3] & 0xFF) << 24 | (buffer[pointer + 2] & 0xFF) << 16 | (buffer[pointer + 1] & 0xFF) << 8 | (buffer[pointer] & 0xFF);

	reader.init(buffer, pointer + 4, len - 4);
	fontFile["SetHBFont"](reader.readPointer64());

	g_return_obj_count.freeObj = pointer;
	g_return_obj_count.count = (len - 12) / 26;
	g_return_obj_count.error = 0;
	return g_return_obj_count;
};

/**
 * Class representing a zip archive creator/reader.
 * @constructor
 */
function ZLib()
{
	this.engine = 0; // указатель на нативный класс Zlib
	this.files = {};
}

/**
 * Check loaded wasm/asmjs module
 */
ZLib.prototype.isModuleInit = false;

/**
 * Open archive from bytes
 * @param {Uint8Array | ArrayBuffer} buf
 * @returns {boolean} success or not
 */
ZLib.prototype.open = function(buf)
{
	if (!this.isModuleInit)
		return false;

	if (this.engine)
		this.close();

	if (!buf)
		return false;

	var arrayBuffer = (undefined !== buf.byteLength) ? new Uint8Array(buf) : buf;

	// TODO: открыли архив, и заполнили this.files
	// объектами { path : null }

	// копируем память в память webasm
	var FileRawDataSize = arrayBuffer.length;
	var FileRawData = Module["_Zlib_Malloc"](FileRawDataSize);
	if (0 == FileRawData)
		return false;
	Module["HEAP8"].set(arrayBuffer, FileRawData);

	// грузим данные
	this.engine = Module["_Zlib_Open"](FileRawData, FileRawDataSize);
	if (0 == this.engine)
	{
		Module["_Zlib_Free"](FileRawData);
		return false;
	}

	// получаем пути в архиве
	var pointer = Module["_Zlib_GetPaths"](this.engine);
	if (0 == pointer)
	{
		Module["_Zlib_Close"](this.engine);
		Module["_Zlib_Free"](FileRawData);
		return false;
	}
	var lenArray = new Int32Array(Module["HEAP8"].buffer, pointer, 4);
	var len = lenArray[0];
	len -= 4;

	var buffer = new Uint8Array(Module["HEAP8"].buffer, pointer + 4, len);
	var index = 0;
	while (index < len)
	{
		var lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
		index += 4;
		var _path = "".fromUtf8(buffer, index, lenRec);
		this.files[_path] = null;
		index += lenRec;
	}
	Module["_Zlib_Free"](FileRawData);
	Module["_Zlib_Free"](pointer);
	return true;
};

/**
 * Create new archive
 * @returns {boolean} success or not
 */
ZLib.prototype.create = function()
{
	if (!this.isModuleInit)
		return false;

	if (this.engine)
		this.close();

	this.engine = Module["_Zlib_Create"]();
	return !!this.engine;
};

/**
 * Save archive from current files
 * @returns {Uint8Array | null} zip-archive bytes, or null if error
 */
ZLib.prototype.save = function()
{
	if (!this.isModuleInit || !this.engine)
		return null;

	var pointerZip = Module["_Zlib_Save"](this.engine);
	if (0 == pointerZip)
		return null;

	var _lenFile = new Int32Array(Module["HEAP8"].buffer, pointerZip, 4);
	var len = _lenFile[0];
	var zip = new Uint8Array(Module["HEAP8"].buffer, pointerZip + 4, len);
	return zip;
};

/**
 * Get all file paths in archive
 * @returns {Array}
 */
ZLib.prototype.getPaths = function()
{
	var retFiles = [];
	if (!this.files)
		return retFiles;

	for (var path in this.files) 
	{
		if (this.files.hasOwnProperty(path))
			retFiles.push(path);
	}
	return retFiles;
};

/**
 * Get uncomressed file from archive
 * @param {string} path
 * @returns {Uint8Array | null} bytes of uncompressed data, or null if error
 */
ZLib.prototype.getFile = function(path)
{
	if (!this.isModuleInit || !this.engine)
		return null;

	// проверяем - есть ли файл вообще?
	if (undefined === this.files[path])
		return null;

	// проверяем - может мы уже его разжимали?
	if (null !== this.files[path])
	{
		if (this.files[path].l > 0)
		{
			return new Uint8Array(Module["HEAP8"].buffer, this.files[path].p, this.files[path].l);
		}
		else
		{
			var _lenFile = new Int32Array(Module["HEAP8"].buffer, this.files[path].p, 4);
			var len = _lenFile[0];
			return new Uint8Array(Module["HEAP8"].buffer, this.files[path].p + 4, len);
		}
	}

	var tmp = path.toUtf8();
	var pointer = Module["_Zlib_Malloc"](tmp.length);
	if (0 == pointer)
		return null;
	Module["HEAP8"].set(tmp, pointer);

	var pointerFile = Module["_Zlib_GetFile"](this.engine, pointer);
	if (0 == pointerFile) 
	{
		Module["_Zlib_Free"](pointer);
		return null;
	}

	var _lenFile = new Int32Array(Module["HEAP8"].buffer, pointerFile, 4);
	var len = _lenFile[0];

	Module["_Zlib_Free"](pointer);
	this.files[path] = { p : pointerFile, l : 0};
	return new Uint8Array(Module["HEAP8"].buffer, pointerFile + 4, len);
};

/**
 * Add uncomressed file to archive
 * @param {string} path
 * @param {Uint8Array} new file in archive
 * @returns {boolean} success or not
 */
ZLib.prototype.addFile = function(path, data)
{
	if (!this.isModuleInit || !this.engine)
		return false;

	if (!data)
		return false;

	// проверяем - может такой файл уже есть? тогда его надо сначала удалить?
	if (undefined !== this.files[path])
		this.removeFile(path);

	var tmp = path.toUtf8();
	var pointer = Module["_Zlib_Malloc"](tmp.length);
	if (0 == pointer)
		return false;
	Module["HEAP8"].set(tmp, pointer);

	var arrayBuffer = (undefined !== data.byteLength) ? new Uint8Array(data) : data;

	var FileRawDataSize = arrayBuffer.length;
	var FileRawData = Module["_Zlib_Malloc"](FileRawDataSize);
	if (0 == FileRawData)
	{
		Module["_Zlib_Free"](pointer);
		return false;
	}
	Module["HEAP8"].set(arrayBuffer, FileRawData);
	
	Module["_Zlib_AddFile"](this.engine, pointer, FileRawData, FileRawDataSize);

	this.files[path] = { p : FileRawData, l : FileRawDataSize};
	Module["_Zlib_Free"](pointer);
	return true;
};

/**
 * Remove file from archive
 * @param {string} path
 * @returns {boolean} success or not
 */
ZLib.prototype.removeFile = function(path)
{
	if (!this.isModuleInit || !this.engine)
		return false;

	// проверяем - может такого файла и нет?
	if (undefined === this.files[path])
		return false;
		
	var tmp = path.toUtf8();
	var pointer = Module["_Zlib_Malloc"](tmp.length);
	if (0 == pointer)
		return false;
	Module["HEAP8"].set(tmp, pointer);
	
	Module["_Zlib_RemoveFile"](this.engine, pointer);

	if (this.files[path] && this.files[path].p)
	{
		Module["_Zlib_Free"](this.files[path].p);
		delete this.files[path];
	}
	Module["_Zlib_Free"](pointer);
	return true;
};

/**
 * Close & remove all used memory in archive
 * @returns {undefined}
 */
ZLib.prototype.close = function()
{
	if (!this.isModuleInit || !this.engine)
		return;

	for (var i in this.files)
	{
		if (this.files[i] && this.files[i].p)
			Module["_Zlib_Free"](this.files[i].p);
	}

	this.files = {};
	if (this.engine)
		Module["_Zlib_Free"](this.engine);
	this.engine = 0;
};

/**
 * Get image type
 * @returns {Number}
 */
ZLib.prototype.getImageType = function(path)
{
	let fileData = this.getFile(path);
	return Module["_Image_GetFormat"](this.files[path].p + 4, fileData.length);
};

/**
 * Get image in needed format
 * @returns {Uint8Array}
 */
ZLib.prototype.getImageAsFormat = function(path, format)
{
	let fileData = this.getFile(path);
	let encodedData = Module["_Raster_Encode"](this.files[path].p + 4, fileData.length, format);
	let encodedSize = Module["_Raster_GetEncodedSize"](encodedData);
	let encodedBuffer = Module["_Raster_GetEncodedBuffer"](encodedData);

	let copyData = new Uint8Array(encodedSize);
	copyData.set(new Uint8Array(Module["HEAP8"].buffer, encodedBuffer, encodedSize));

	Module["_Raster_DestroyEncodedData"](encodedData);

	return copyData;
};
/**
 * Get image as svg (for simple test)
 * @returns {string}
 */
ZLib.prototype.getImageAsSvg = function(path)
{
	let fileData = this.getFile(path);
	let encodedData = Module["_Raster_Encode"](this.files[path].p + 4, fileData.length, 24);
	let encodedSize = Module["_Raster_GetEncodedSize"](encodedData);
	let encodedBuffer = Module["_Raster_GetEncodedBuffer"](encodedData);

	let string = String.prototype.fromUtf8(new Uint8Array(Module["HEAP8"].buffer, encodedBuffer, encodedSize));

	Module["_Raster_DestroyEncodedData"](encodedData);

	return string;
};
/**
 * Get image blob for browser
 * @returns {Blob}
 */
ZLib.prototype.getImageBlob = function(path)
{
	let imageType = this.getImageType(path);
	if (imageType != 10 && imageType != 21)
	{
		return new Blob([this.getFile(path)], {type:AscCommon.openXml.GetMimeType(AscCommon.GetFileExtension(path))});
	}

	let fileData = this.getFile(path);
	let encodedData = Module["_Raster_Encode"](this.files[path].p + 4, fileData.length, 24);
	let encodedSize = Module["_Raster_GetEncodedSize"](encodedData);
	let encodedBuffer = Module["_Raster_GetEncodedBuffer"](encodedData);

	let blob = new Blob([new Uint8Array(Module["HEAP8"].buffer, encodedBuffer, encodedSize)], {type : AscCommon.openXml.GetMimeType("svg")});

	Module["_Raster_DestroyEncodedData"](encodedData);

	return blob;
};

window.AscCommon = window.AscCommon || {};
window.AscCommon.CZLibEngineJS = ZLib;

var hyphenApplication = 0;
AscFonts.Hyphen_Init = function()
{
	hyphenApplication = Module["_hyphenCreateApplication"]();
};
AscFonts.Hyphen_Destroy = function()
{
	Module["_hyphenDestroyApplication"](hyphenApplication);
};
AscFonts.Hyphen_CheckDictionary = function(lang)
{
	return false;
};
AscFonts.Hyphen_LoadDictionary = function(lang, data)
{
	let dictSize = data.byteLength;
	let dictPointer = Module["_malloc"](dictSize);
	Module["HEAP8"].set(new Uint8ClampedArray(data), dictPointer);

	let result = Module["_hyphenLoadDictionary"](hyphenApplication, lang, dictPointer, dictSize);

	Module["_free"](dictPointer);
		
	return (result === 0) ? true : false;
};

AscFonts.Hyphen_Word = function(lang, word)
{
	let wordPointer = word.toUtf8Pointer(true);
	let wordLen = wordPointer.length;
	let hyphens = [];

	if (wordPointer) 
	{
		let ptr = Module._hyphenWord(hyphenApplication, lang, wordPointer.ptr, wordLen);
		let vector = new Uint8ClampedArray(Module["HEAP8"].buffer, ptr, wordLen + 5);

		let pos = 0;
		while (vector[pos] != 0)
		{
			if (1 === (vector[pos] & 1))
				hyphens.push(pos+1);			
			pos++;
		}

		wordPointer.free();
	}
	return hyphens;
};

if (window["NATIVE_EDITOR_ENJINE"])
	window.immediateRun();
AscFonts.onLoadModule();

})(window, undefined);
