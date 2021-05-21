(function(window, undefined){

	//desktop_fetch
	//string_utf8

	//module
	
	function XmlSignature()
	{
		this.engine = 0;
		this.cert = 0;
		this.key = 0;
	}
	
	XmlSignature.prototype.isModuleInit = false;
	
	XmlSignature.prototype.create = function()
	{
		if (!this.isModuleInit)
			return false;

		if (this.engine)
			this.close();

		this.engine = Module["_XmlSignature_CreateCertificate"]();
		return !!this.engine;
	};
	
	XmlSignature.prototype.loadCert = function(data)
	{
		if (!this.isModuleInit || !this.engine || !data)
			return false;
		
		var arrayBuffer = (undefined !== data.byteLength) ? new Uint8Array(data) : data;
		
		var FileRawDataSize = arrayBuffer.length;
		this.cert = Module["_XmlSirnature_Malloc"](FileRawDataSize);
		if (0 == this.cert)
			return false;
		Module["HEAP8"].set(arrayBuffer, this.cert);

		Module["_XmlSignature_LoadCert"](this.engine, this.cert, FileRawDataSize, null);
		return true;
	};
	
	XmlSignature.prototype.loadKey = function(data)
	{
		if (!this.isModuleInit || !this.engine || !data)
			return false;
		
		var arrayBuffer = (undefined !== data.byteLength) ? new Uint8Array(data) : data;
		
		var FileRawDataSize = arrayBuffer.length;
		this.key = Module["_XmlSirnature_Malloc"](FileRawDataSize);
		if (0 == this.key)
			return false;
		Module["HEAP8"].set(arrayBuffer, this.key);

		Module["_XmlSignature_LoadKey"](this.engine, this.key, FileRawDataSize, null);
		return true;
	};
	
	XmlSignature.prototype.sign = function(data)
	{
		if (!this.isModuleInit || !this.engine || !data)
			return null;
		
		var arrayBuffer = (undefined !== data.byteLength) ? new Uint8Array(data) : data;
		
		var FileRawDataSize = arrayBuffer.length;
		var FileRawData = Module["_XmlSirnature_Malloc"](FileRawDataSize);
		if (0 == FileRawData)
			return null;
		Module["HEAP8"].set(arrayBuffer, FileRawData);

		var file = Module["_XmlSignature_CreateFile"](FileRawData, FileRawDataSize);
		if (0 == file) 
		{
			Module["_XmlSirnature_Free"](FileRawData);
			return null;
		}
		var res  = Module["_XmlSignature_Sign"](file, this.engine);
		if (0 == res) 
		{
			Module["_XmlSirnature_Free"](FileRawData);
			Module["_XmlSignature_DestroyFile"](file);
			return null;
		}
		
		var _lenFile = new Int32Array(Module["HEAP8"].buffer, res, 4);
		var len = _lenFile[0];
		var fileData = new Uint8Array(Module["HEAP8"].buffer, res + 4, len);
		
		Module["_XmlSirnature_Free"](FileRawData);
		Module["_XmlSignature_DestroyFile"](file);
		Module["_XmlSirnature_Free"](res);
		return fileData;
	};
	
	XmlSignature.prototype.close = function()
	{
		if (!this.isModuleInit || !this.engine)
			return;

		if (this.engine)
			Module["_XmlSignature_DestroyCertificate"](this.engine);
		if (this.cert)
			Module["_XmlSirnature_Free"](this.cert);
		if (this.key)
			Module["_XmlSirnature_Free"](this.key);
	};

window.nativeXmlSignatureEngine = new XmlSignature();
window.onEngineInit = function()
{
	window.nativeXmlSignatureEngine.isModuleInit = true;
};

})(window, undefined);

