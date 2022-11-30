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

//base64

(function(window, undefined) {

	window.messageData = null;
	window.messagePort = null;
	function onMessageEvent(data, port)
	{
	    if (data.type == "hash")
	    {
	        window.messageData = data.value;
	        window.messagePort = port;
	        if (!window.engineInit)
	        	return;
	        checkMessage();
	    }
	}

	window.onconnect = function(e)
	{
	    var port = e.ports[0];
	    port.onmessage = function(e) {
	        onMessageEvent(e.data, port);
	    }    
	};
	window.onmessage = function(e)
	{
	    onMessageEvent(e.data);
	};
	window.engineInit = false;
	window.onEngineInit = function()
	{
		window.engineInit = true;
		if (window.messageData)
			checkMessage();
	};

	function checkMessage()
	{
		var data = window.messageData;
		var res = [];

		for (var i = 0, len = data.length; i < len; i++)
        {
            res.push(AscCommon.Hash.hashOffice(data[i].password, data[i].salt, data[i].spinCount, data[i].alg).base64());
        }

		var sender = window.messagePort || window;
		sender.postMessage({ hashValue : res });
	}

	//desktop_fetch
	
	//polyfill
	
	//string_utf8
    
    //module

    var HashAlgs = {
        MD2       : 0,
        MD4       : 1,
        MD5       : 2,
        RMD160    : 3,
        SHA1      : 4,
        SHA256    : 5,
        SHA384    : 6,
        SHA512    : 7,
        WHIRLPOOL : 8
    };

    var HashSizes = [
        16,
        16,
        16,
        20,
        20,
        32,
        48,
        64,
        64
    ];

	window["AscCommon"] = window.AscCommon = (window["AscCommon"] || {});
	window.AscCommon["Hash"] = window.AscCommon.Hash = {};
	window.AscCommon.Hash["HashAlgs"] = window.AscCommon.Hash.HashAlgs = HashAlgs;
	window.AscCommon.Hash["HashSizes"] = window.AscCommon.Hash.HashSizes = HashSizes;
	
	function HashObj() { this.buf; }
	HashObj.prototype["buffer"] = HashObj.prototype.buffer = function()	{ return this.buf; };
	HashObj.prototype["base64"] = HashObj.prototype.base64 = function() { return window.AscCommon.Base64.encode(this.buf); };
	HashObj.prototype["hex"] = HashObj.prototype.hex = function() { return window.AscCommon.Hex.encode(this.buf); };
	
	window.AscCommon.Hash["hash"] = window.AscCommon.Hash.hash = function(data, alg)
	{
		if (typeof alg === "string")
		{
			switch (alg)
			{
				case "md2" : alg = HashAlgs.MD2; break;
				case "md4" : alg = HashAlgs.MD4; break;
				case "md5" : alg = HashAlgs.MD5; break;
				case "rmd160" : alg = HashAlgs.RMD160; break;
				case "sha1" : alg = HashAlgs.SHA1; break;
				case "sha256" : alg = HashAlgs.SHA256; break;
				case "sha384" : alg = HashAlgs.SHA384; break;
				case "sha512" : alg = HashAlgs.SHA512; break;
				case "whirlpool" : alg = HashAlgs.WHIRLPOOL; break;
				default:
					alg = HashAlgs.SHA256;
			}
		}
		
		var arrayData = null;
		if (typeof data === "string")
			arrayData = data.toUtf8(true);
		else
			arrayData = data;
		
        var dataPointer = Module["_malloc"](arrayData.length);
        Module["HEAPU8"].set(arrayData, dataPointer);		
		var resultPointer = Module["_hash"](dataPointer, arrayData.length, alg);
		Module["_free"](dataPointer);
		
		var result = new HashObj();
		if (0 != resultPointer)
		{
			var tmp = new Uint8Array(Module["HEAPU8"].buffer, resultPointer, HashSizes[alg]);
			result.buf = new Uint8Array(tmp.length);
			result.buf.set(tmp, 0);
			Module["_free"](resultPointer);
		}
		else
		{
			result.buf = [];
		}
		
		return result;
	};
	
	window.AscCommon.Hash["hashOffice"] = window.AscCommon.Hash.hash = function(password, salt, spinCount, alg)
	{
		if (typeof alg === "string")
		{
			switch (alg)
			{
				case "md2" : alg = HashAlgs.MD2; break;
				case "md4" : alg = HashAlgs.MD4; break;
				case "md5" : alg = HashAlgs.MD5; break;
				case "rmd160" : alg = HashAlgs.RMD160; break;
				case "sha1" : alg = HashAlgs.SHA1; break;
				case "sha256" : alg = HashAlgs.SHA256; break;
				case "sha384" : alg = HashAlgs.SHA384; break;
				case "sha512" : alg = HashAlgs.SHA512; break;
				case "whirlpool" : alg = HashAlgs.WHIRLPOOL; break;
				default:
					alg = HashAlgs.SHA256;
			}
		}

		var passwordData = password.toUtf8();
		var passwordPointer = Module["_malloc"](passwordData.length);
		Module["HEAPU8"].set(passwordData, passwordPointer);

		var saltData = salt.toUtf8();
		var saltPointer = Module["_malloc"](saltData.length);
		Module["HEAPU8"].set(saltData, saltPointer);

		var resultPointer = Module["_hash2"](passwordPointer, saltPointer, spinCount, alg);

		Module["_free"](passwordPointer);
		Module["_free"](saltPointer);

		var result = new HashObj();
		if (0 != resultPointer)
		{
			var tmp = new Uint8Array(Module["HEAPU8"].buffer, resultPointer, HashSizes[alg]);
			result.buf = new Uint8Array(tmp.length);
			result.buf.set(tmp, 0);
			Module["_free"](resultPointer);
		}
		else
		{
			result.buf = [];
		}

		return result;
	};

})(self, undefined);
