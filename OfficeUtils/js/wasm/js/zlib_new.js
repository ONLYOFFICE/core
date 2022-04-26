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

window.nativeZlibEngine = new ZLib();
window.onEngineInit = function()
{
	window.nativeZlibEngine.isModuleInit = true;
};

})(window, undefined);

