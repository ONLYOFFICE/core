(function(window, undefined){

	//desktop_fetch
	//string_utf8

	//module
	
	function NativePointer(buffer)
	{
		this.data = 0;
		this.size = 0;

		if (buffer)
			this.alloc(buffer);
	}
	NativePointer.prototype.alloc = function(data_or_size)
	{
		this.free();
		if (typeof data_or_size === "number")
		{
			this.data = Module["_XmlSignature_Malloc"](data_or_size);
			if (0 != this.data)
				this.size = data_or_size;
		}
		else
		{
			var arrayBuffer = (undefined !== data_or_size.byteLength) ? new Uint8Array(data_or_size) : data_or_size;
			this.data = Module["_XmlSignature_Malloc"](arrayBuffer.length);
			if (0 != this.data)
			{
				this.size = arrayBuffer.length;
				Module["HEAP8"].set(arrayBuffer, this.data);
			}
		}
	};
	NativePointer.prototype.free = function(freeFunc)
	{
		if (this.data != 0)
		{
			(freeFunc || Module["_XmlSignature_Free"])(this.data);
		}
		this.data = 0;
		this.size = 0;
	};
	NativePointer.prototype.isEmpty = function()
	{
		return (0 == this.data);
	};

	var XmlSignatureError = {
		OPEN_SSL_WARNING_OK 		: 0,
		OPEN_SSL_WARNING_ERR        : 1,
		OPEN_SSL_WARNING_ALL_OK     : 2,
		OPEN_SSL_WARNING_PASS       : 4,
		OPEN_SSL_WARNING_NOVERIFY   : 8
	};

	function XmlSignature()
	{
		this._certFilePtr = new NativePointer();
		this._keyFilePtr = new NativePointer();

		this.certificate = 0;
	}

	XmlSignature.prototype.isModuleInit = false;
	XmlSignature.prototype.loadCert = function(data, password)
	{
		if (!this.isModuleInit || !data)
			return XmlSignatureError.OPEN_SSL_WARNING_ERR;

		if (!this.create())
			return XmlSignatureError.OPEN_SSL_WARNING_ERR;

		this._certFilePtr.alloc(data);
		if (this._certFilePtr.isEmpty())
			return XmlSignatureError.OPEN_SSL_WARNING_ERR;

		var _password = new NativePointer();
		if (password)
			_password.alloc(password.toUtf8());

		var retError = Module["_XmlSignature_LoadCert"](this.certificate, this._certFilePtr.data, this._certFilePtr.size, _password.data);
		_password.free();

		return retError;
	};
	
	XmlSignature.prototype.loadKey = function(data, password)
	{
		if (!this.isModuleInit || !this.certificate || !data)
			return XmlSignatureError.OPEN_SSL_WARNING_ERR;

		this._keyFilePtr.alloc(data);
		if (this._keyFilePtr.isEmpty())
			return XmlSignatureError.OPEN_SSL_WARNING_ERR;

		var _password = new NativePointer();
		if (password)
			_password.alloc(password.toUtf8());
		
		var retError = Module["_XmlSignature_LoadKey"](this.certificate, this._keyFilePtr.data, this._keyFilePtr.size, _password.data);
		_password.free();

		return retError;
	};
	
	XmlSignature.prototype.sign = function(data)
	{
		if (!this.isModuleInit || !this.certificate || !data)
			return null;
		
		var _filePtr = new NativePointer(data);
		if (_filePtr.isEmpty())
			return null;

		var _file = Module["_XmlSignature_CreateFile"](_filePtr.data, _filePtr.size);
		// удалится дата при удалении файла
		_filePtr.data = 0;
		_filePtr.size = 0;

		if (0 == _file)
			return null;

		var result = Module["_XmlSignature_Sign"](_file, this.certificate);
		if (0 == result)
			return null;

		Module["_XmlSignature_DestroyFile"](_file);
		if (0 == result)
			return null;

		var dst= null;
		var resultSize = Module["_XmlSignature_GetFileSize"](result);
		if (0 != resultSize)
		{
			var resultRaw = Module["_XmlSignature_GetFileRaw"](result);
			var fileData = new Uint8Array(Module["HEAP8"].buffer, resultRaw, resultSize);
			dst = new Uint8Array(resultSize);
			dst.set(fileData);
		}
		Module["_XmlSignature_DestroyFile"](result);
		return dst;
	};

	XmlSignature.prototype.create = function()
	{
		if (!this.isModuleInit)
			return false;

		if (0 != this.certificate)
			this.close();

		this.certificate = Module["_XmlSignature_CreateCertificate"]();
		return (0 != this.certificate) ? true : false;
	};
	
	XmlSignature.prototype.close = function()
	{
		if (!this.isModuleInit)
			return;

		if (this.certificate)
		{
			Module["_XmlSignature_DestroyCertificate"](this.certificate);
			this.certificate = 0;
		}

		this._certFilePtr.free();
		this._keyFilePtr.free();
	};

	window.XmlSignature = XmlSignature;
	window.XmlSignatureError = XmlSignatureError;
	window.onEngineInit = function()
	{
		window.XmlSignature.prototype.isModuleInit = true;
	};

})(window, undefined);

