var printErr = undefined;
var FS = undefined;
var print = undefined;

var fetch = self.fetch;
var getBinaryPromise = null;
if (self.AscDesktopEditor && document.currentScript && 0 == document.currentScript.src.indexOf("file:///"))
{
    fetch = undefined; // fetch not support file:/// scheme
    getBinaryPromise = function() {
        var wasmPath = "ascdesktop://fonts/" + wasmBinaryFile.substr(8);
        return new Promise(function (resolve, reject) {

            var xhr = new XMLHttpRequest();
            xhr.open('GET', wasmPath, true);
            xhr.responseType = 'arraybuffer';

            if (xhr.overrideMimeType)
                xhr.overrideMimeType('text/plain; charset=x-user-defined');
            else
                xhr.setRequestHeader('Accept-Charset', 'x-user-defined');

            xhr.onload = function () {
                if (this.status == 200) {
                    resolve(new Uint8Array(this.response));
                }
            };
            xhr.send(null);
        });
    }
}
else
{
    getBinaryPromise = function() {
        return getBinaryPromise2();
    }
}

//polyfill

//module

self.spellchecker = null;
function onMessageEvent(data, port)
{
    if (data.type == "init")
    {
        if (self.spellchecker)
            return;
        self.spellchecker = new Spellchecker();
        self.spellchecker.languagesPath = data.dictionaries_path;
        var languages = data.languages;
        for (var i in languages)
        	self.spellchecker.addDefaultLanguage(i, languages[i]);
        self.spellchecker.init();
        return;
    }

    if (!self.spellchecker)
        return;

    self.spellchecker.messages.push(data);
    if (port)
        self.spellchecker.ports.push(port);

    if (1 < self.spellchecker.messages.length)
    {
        // значит еще грузим что-то
        return;
    }

    self.spellchecker.checkMessage();
}

self.onconnect = function(e)
{
    var port = e.ports[0];
    port.onmessage = function(e) {
        onMessageEvent(e.data, port);
    }    
};
self.onmessage = function(e)
{
    onMessageEvent(e.data);
};
self.engineInit = false;
self.onEngineInit = function()
{
	self.engineInit = true;
	if (self.spellchecker)
	{
		self.spellchecker.init();
		self.spellchecker.checkMessage();
	}
};

function Dictionary()
{
	this.dataAff = null;
	this.dataDic = null;
	this.status = 0;
	this.id = 0;
	this.language = null;

	this.load_file = function(src, params)
	{
		var xhr = new XMLHttpRequest();
		xhr.sender = this;
		xhr.params = params;
        xhr.open('GET', src, true);
        xhr.responseType = 'arraybuffer';
        if (xhr.overrideMimeType)
            xhr.overrideMimeType('text/plain; charset=x-user-defined');
        else
            xhr.setRequestHeader('Accept-Charset', 'x-user-defined');

        xhr.onload = function()
        {
        	if (xhr.sender.status >= 2)
        		return;

        	if (this.response && this.status == 200)
            {
                var uintData = new Uint8Array(this.response);
                switch (this.params)
                {
                	case "aff":
            		{
            			this.sender.dataAff = uintData;
            			break;
            		}
            		case "dic":
            		{
            			this.sender.dataDic = uintData;
            			break;
            		}
            		default:
            			break;
                }
                xhr.sender.status++;
            }

            if (xhr.sender.status == 2)
            {
            	self.spellchecker.onLoadDictionary(this.sender);
            }
        };
        xhr.onerror = function()
        {
        	xhr.sender.status = 2;
        	self.spellchecker.onLoadDictionary(this.sender);
        };

        xhr.send(null);
	};

	this.load = function()
	{
		if (!this.language)
		{
			this.status = 2;
			self.spellchecker.onLoadDictionary(this);
			return;
		}
		this.load_file(self.spellchecker.languagesPath + "/" + this.language.aff, "aff");
		this.load_file(self.spellchecker.languagesPath + "/" + this.language.dic, "dic");
	};

	this.freeUnusedData = function()
	{
		this.dataAff = null;
		this.dataDic = null;
	};
}


function Spellchecker()
{
	this.languagesPath = "";
	this.languages = {};
	this.readyLanguages = {};
	this.messages = [];
	this.ports = [];
	this.tmpStrings = new ArrayBuffer(1000);
	this.engine = 0;

	this.maxEngines = 3; 
	this.maxDictionaries = 5;
	this.languageQueue = [];

	this.maxDictionariesHandler = function() 
	{
		if (this.languageQueue.length > this.maxDictionaries) 
		{
			var toDelete = this.languageQueue.length - this.maxDictionaries;
			for (let i = 0; i < toDelete; i++) 
			{
				var lk = this.languageQueue[0];
				this.deleteDictionaty(lk);
				delete this.readyLanguages[lk];
				this.languageQueue.shift();
			}
		}
	};

	this.deleteDictionaty = function(lk) 
	{
		if (!lk) 
			return;

		var affID = lk + ".aff";
		var dicID = lk + ".dic"
		var engineID = affID + dicID;
		var engineIDptr = this.allocString(engineID);
		var langAffptr = this.allocString(affID);
		var langDicptr = this.allocString(dicID);
		Module._Spellchecker_RemoveDicrionary(this.engine, langAffptr);
		Module._Spellchecker_RemoveDicrionary(this.engine, langDicptr);
		Module._Spellchecker_RemoveEngine(this.engine, engineIDptr);
		this.freeString(engineIDptr);
		this.freeString(langAffptr);
		this.freeString(langDicptr);
	};

	this.init = function()
	{
		if (0 == this.engine && self.engineInit)
			this.engine = this.createEngine();
	};

	this.addDefaultLanguage = function(id, path)
	{
		this.languages["" + id] = { 
			aff : path + "/" + path + ".aff", 
			dic : path + "/" + path + ".dic" 
		};
	};

	this.onLoadDictionary = function(dictionary)
	{
		if (!dictionary.dataAff || !dictionary.dataDic)
		{
			this.checkMessage();
			return;
		}

		var aff_path = this.allocString(dictionary.id + ".aff");
		var dic_path = this.allocString(dictionary.id + ".dic");

		var pointerAff = Module._Spellchecker_Malloc(dictionary.dataAff.length);
    	Module.HEAP8.set(dictionary.dataAff, pointerAff);
    	var pointerDic = Module._Spellchecker_Malloc(dictionary.dataDic.length);
    	Module.HEAP8.set(dictionary.dataDic, pointerDic);

    	Module._Spellchecker_AddDictionary(this.engine, aff_path, pointerAff, dictionary.dataAff.length);
    	Module._Spellchecker_AddDictionary(this.engine, dic_path, pointerDic, dictionary.dataDic.length);

		this.freeString(aff_path);
		this.freeString(dic_path);

		dictionary.freeUnusedData();

		this.checkMessage();
	};

	this.checkMessage = function()
	{
		if (0 == this.messages.length || !self.engineInit)
			return;

		var m = this.messages[0];
		var isReady = true;
		for (var indexLang = 0, lenLangs = m.usrLang.length; indexLang < lenLangs; indexLang++)
		{
			var lang_key = "" + m.usrLang[indexLang];
			var readyLang = this.readyLanguages[lang_key];
			if (!readyLang)
			{
				// начнем грузить
				var langToReady = new Dictionary();
				langToReady.id = lang_key;
				langToReady.language = this.languages[lang_key];
				this.readyLanguages[lang_key] = langToReady;
				this.languageQueue.push(lang_key); // push lang info into the queue
				langToReady.load();

				isReady = false;
				break;
			}
			else if (readyLang.status != 2)
			{
				// ждем
				isReady = false;
				break;
			}
			else
			{				
				// все готово.
				continue;	
			}
		}

		if (!isReady)
		{
			// ждем
			return;
		}

		switch (m.type)
		{
			case "spell":
			{
				this.Spell(m);
				break;
			}
			case "suggest":
			{
				this.Suggest(m);
				break;
			}
			default:
				break;
		}
		this.maxDictionariesHandler();
		this.messages.shift();
	};

	this.allocString = function(string)
	{
		var inputLen = string.length;
		var testLen = 6 * inputLen + 1;
		if (testLen > this.tmpStrings.byteLength)
			this.tmpStrings = new ArrayBuffer(testLen);

		var code = 0;
		var index = 0;

		var outputIndex = 0;
		var outputDataTmp = new Uint8Array(this.tmpStrings);
		var outputData = outputDataTmp;

        while (index < inputLen)
        {
        	code = string.charCodeAt(index++);
        	if (code >= 0xD800 && code <= 0xDFFF && index < inputLen)
            {
                code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & string.charCodeAt(index++)));
            }

            if (code < 0x80)
            {
                outputData[outputIndex++] = code;
            }
            else if (code < 0x0800)
            {
                outputData[outputIndex++] = 0xC0 | (code >> 6);
                outputData[outputIndex++] = 0x80 | (code & 0x3F);
            }
            else if (code < 0x10000)
            {
                outputData[outputIndex++] = 0xE0 | (code >> 12);
                outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
                outputData[outputIndex++] = 0x80 | (code & 0x3F);
            }
            else if (code < 0x1FFFFF)
            {
                outputData[outputIndex++] = 0xF0 | (code >> 18);
                outputData[outputIndex++] = 0x80 | ((code >> 12) & 0x3F);
                outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
                outputData[outputIndex++] = 0x80 | (code & 0x3F);
            }
            else if (code < 0x3FFFFFF)
            {
                outputData[outputIndex++] = 0xF8 | (code >> 24);
                outputData[outputIndex++] = 0x80 | ((code >> 18) & 0x3F);
                outputData[outputIndex++] = 0x80 | ((code >> 12) & 0x3F);
                outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
                outputData[outputIndex++] = 0x80 | (code & 0x3F);
            }
            else if (code < 0x7FFFFFFF)
            {
                outputData[outputIndex++] = 0xFC | (code >> 30);
                outputData[outputIndex++] = 0x80 | ((code >> 24) & 0x3F);
                outputData[outputIndex++] = 0x80 | ((code >> 18) & 0x3F);
                outputData[outputIndex++] = 0x80 | ((code >> 12) & 0x3F);
                outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
                outputData[outputIndex++] = 0x80 | (code & 0x3F);
            }
        }

        outputData[outputIndex++] = 0;

        var tmpBuffer = new Uint8Array(this.tmpStrings, 0, outputIndex);
        var pointer = Module._Spellchecker_Malloc(outputIndex);
    	Module.HEAP8.set(tmpBuffer, pointer);
    	return pointer;
	};
	this.freeString = function(stringPointer)
	{
		Module._Spellchecker_Free(stringPointer);
	};

	this.readFromUtf8 = function(buffer, start, len)
	{
		var result = "";
		var index = start;
		var end = start + len;
		while (index < end) 
		{
	        var u0 = buffer[index++];
	        if (!(u0 & 128)) 
	        {
	          	result += String.fromCharCode(u0);
	          	continue;
	        }
	        var u1 = buffer[index++] & 63;
	        if ((u0 & 224) == 192) 
	        {
	          	result += String.fromCharCode((u0 & 31) << 6 | u1);
	          	continue;
	        }
	        var u2 = buffer[index++] & 63;
	        if ((u0 & 240) == 224) 
	        {
	          	u0 = (u0 & 15) << 12 | u1 << 6 | u2;
	        } 
	        else 
	        {
	          	u0 = (u0 & 7) << 18 | u1 << 12 | u2 << 6 | buffer[index++] & 63;
	        }
	        if (u0 < 65536) 
	        {
	          	result += String.fromCharCode(u0);
	        } 
	        else 
	        {
	          	var ch = u0 - 65536;
	          	result += String.fromCharCode(55296 | ch >> 10, 56320 | ch & 1023);
	        }
      	}
      	return result;
	};

	this.readSuggests = function(pointer)
	{
		if (0 == pointer)
			return [];

		var lenArray = new Int32Array(Module["HEAP8"].buffer, pointer, 4);
		var len = lenArray[0];
		len -= 4;

		var buffer = new Uint8Array(Module["HEAP8"].buffer, pointer + 4, len);
		var index = 0;
		var ret = [];
		while (index < len)
		{
			var lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
			index += 4;
			ret.push(this.readFromUtf8(buffer, index, lenRec));
			index += lenRec;
		}
		return ret;
	};

	this.createEngine = function()
	{
		return Module._Spellchecker_Create(this.maxEngines);
	};
	this.destroyEngine = function()
	{
		Module._Spellchecker_Destroy();
	};

	this.Spell = function(data)
	{
		var len = Math.min(data.usrLang.length, data.usrWords.length);
		if (0 == len)
			return;
		data.usrCorrect = new Array(len);
		var curLang = "";		
		for (var i = 0; i < len; i++)
		{
			if (curLang != ("" + data.usrLang[i]))
			{
				curLang = "" + data.usrLang[i];
				var aff = this.allocString(curLang + ".aff");
				var dic = this.allocString(curLang + ".dic");
				ret = Module._Spellchecker_Load(this.engine, aff, dic);
				this.freeString(aff);
				this.freeString(dic);
			}
			var word = this.allocString(data.usrWords[i]);
			data.usrCorrect[i] = (1 == Module._Spellchecker_Spell(this.engine, word)) ? true : false;
			this.freeString(word);
		}

		this.sendAnswer(data);
	};

	this.Suggest = function(data)
	{
		var len = Math.min(data.usrLang.length, data.usrWords.length);
		if (0 == len)
			return;
		data.usrSuggest = new Array(len);
		var curLang = "";		
		for (var i = 0; i < len; i++)
		{
			if (curLang != ("" + data.usrLang[i]))
			{
				curLang = "" + data.usrLang[i];
				var aff = this.allocString(curLang + ".aff");
				var dic = this.allocString(curLang + ".dic");

				ret = Module._Spellchecker_Load(this.engine, aff, dic);

				this.freeString(aff);
				this.freeString(dic);
			}
			var word = this.allocString(data.usrWords[i]);
			var pointerSuggests = Module._Spellchecker_Suggest(this.engine, word);
			data.usrSuggest[i] = this.readSuggests(pointerSuggests);
			this.freeString(word);
		}

		this.sendAnswer(data);
	};

	this.sendAnswer = function(data)
	{
		if (self.spellchecker.ports.length == 0)
		{
            self.postMessage(data);
        }
        else
        {
            var port = self.spellchecker.ports.shift();
            port.postMessage(data);
        }

		setTimeout(function(){
			self.spellchecker.checkMessage();
		}, 1);
	};
}
