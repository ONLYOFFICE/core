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

		this.engine = Module["_Zlib_Create"]();

		var arrayBuffer = (undefined !== buf.byteLength) ? new Uint8Array(buf) : buf;

		// TODO: открыли архив, и заполнили this.files
		// объектами { path : null }
	};

	/**
	 * Create new archive
	 * @returns {boolean} success or not
	 */
	ZLib.prototype.create = function(buf)
	{
		if (!this.isModuleInit)
			return false;

		if (this.engine)
			this.close();

		this.engine = Module["_Zlib_Create"]();
	};

	/**
	 * Save archive from current files
	 * @returns {Uint8Array | null} zip-archive bytes, or null if error
	 */
	ZLib.prototype.save = function()
	{
		if (!this.isModuleInit || !this.engine)
			return false;

		// из текущего дерева this.files - делаем архив
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
		var fileData = new Uint8Array();

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

		// добавляем файл в такой же набор файлов в c++ коде...
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

		// удаляем файл в таком же наборе файлов в c++ коде...
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

})(window, undefined);

