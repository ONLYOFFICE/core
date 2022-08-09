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

(function(window, undefined){

	//desktop_fetch

	//string_utf8

	//module

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
	window.onZlibEngineInit = function()
	{
		ZLib.prototype.isModuleInit = true;
		window["ZLibModule_onLoad"] && window["ZLibModule_onLoad"]();
	};

})(window, undefined);

