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
		this.engine = Module["_Zlib_Load"](FileRawData, FileRawDataSize);
		if (0 == this.engine)
		{
			Module["_Zlib_Free"](FileRawData);
			return false;
		}

		// получаем пути в архиве
		var pointer = Module["_Zlib_GetPathsInArchive"](this.engine);
		if (0 == pointer)
		{
			Module["_Zlib_Destroy"](this.engine);
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

		// из текущего дерева this.files - делаем архив

		// вычисление длины дерева
		var _paths = [];
		var nLength = 4;
		for (var _path in this.files)
		{
			var pathUtf8 = _path.toUtf8();
			_paths.push(pathUtf8);
			nLength += 4;
			nLength += pathUtf8.length;

			nLength += 4;
			nLength += this.files[_path].length;
		}

		// создание дерева файлов
		var tmpBuffer = new Uint8Array(nLength);
		tmpBuffer.set([
			(nLength >>  0) & 0xFF,
			(nLength >>  8) & 0xFF,
			(nLength >> 16) & 0xFF,
			(nLength >> 24) & 0xFF
		], 0);
		var index = 4;
		var i = 0;
		for (var _path in this.files)
		{
			tmpBuffer.set([
				(_paths[i].length >>  0) & 0xFF,
				(_paths[i].length >>  8) & 0xFF,
				(_paths[i].length >> 16) & 0xFF,
				(_paths[i].length >> 24) & 0xFF
			], index);
			index += 4;
			tmpBuffer.set(_paths[i], index);
			index += _paths[i].length;
			i++;

			tmpBuffer.set([
				(this.files[_path].length >>  0) & 0xFF,
				(this.files[_path].length >>  8) & 0xFF,
				(this.files[_path].length >> 16) & 0xFF,
				(this.files[_path].length >> 24) & 0xFF
			], index);
			index += 4;
			tmpBuffer.set(this.files[_path], index);
			index += this.files[_path].length;
		}

		// получение zip из дерева файлов
		var pointer = Module["_Zlib_Malloc"](tmpBuffer.length);
		if (0 == pointer)
			return null;
		Module["HEAP8"].set(tmpBuffer, pointer);
		var pointerZip = Module["_Zlib_CompressFiles"](this.engine, pointer);
		if (0 == pointerZip)
		{
			Module["_Zlib_Free"](pointer);
			return null;
		}
		var _lenFile = new Int32Array(Module["HEAP8"].buffer, pointerZip, 4);
		var len = _lenFile[0];
		var zip = new Uint8Array(Module["HEAP8"].buffer, pointerZip + 4, len);
		Module["_Zlib_Free"](pointer);
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
			return this.files[path];

		// TODO: получаем данные

		var tmp = path.toUtf8();
		var pointer = Module["_Zlib_Malloc"](tmp.length);
		if (0 == pointer)
			return null;
		Module["HEAP8"].set(tmp, pointer);

		var pointerFile = Module["_Zlib_GetFileFromArchive"](this.engine, pointer);
		if (0 == pointerFile) 
		{
			Module["_Zlib_Free"](pointer);
			return null;
		}
		
		var _lenFile = new Int32Array(Module["HEAP8"].buffer, pointerFile, 4);
		var len = _lenFile[0];
		var fileData = new Uint8Array(Module["HEAP8"].buffer, pointerFile + 4, len);

		Module["_Zlib_Free"](pointer);
		this.files[path] = fileData;
		return fileData;
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

		// проверяем - может такой файл уже есть? тогда его надо сначала удалить?
		if (undefined !== this.files[path])
			return false;

		this.files[path] = data;
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

		delete this.files[path];
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

		this.files = {};
		if (this.engine)
			Module["_Zlib_Free"](this.engine);
	};

window.nativeZlibEngine = new ZLib();
window.onEngineInit = function()
{
	window.nativeZlibEngine.isModuleInit = true;
};

})(window, undefined);

