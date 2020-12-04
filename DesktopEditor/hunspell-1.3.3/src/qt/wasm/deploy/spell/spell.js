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

var Module=typeof Module!=="undefined"?Module:{};var moduleOverrides={};var key;for(key in Module){if(Module.hasOwnProperty(key)){moduleOverrides[key]=Module[key]}}var arguments_=[];var thisProgram="./this.program";var quit_=function(status,toThrow){throw toThrow};var ENVIRONMENT_IS_WEB=false;var ENVIRONMENT_IS_WORKER=false;var ENVIRONMENT_IS_NODE=false;var ENVIRONMENT_IS_SHELL=false;ENVIRONMENT_IS_WEB=typeof window==="object";ENVIRONMENT_IS_WORKER=typeof importScripts==="function";ENVIRONMENT_IS_NODE=typeof process==="object"&&typeof process.versions==="object"&&typeof process.versions.node==="string";ENVIRONMENT_IS_SHELL=!ENVIRONMENT_IS_WEB&&!ENVIRONMENT_IS_NODE&&!ENVIRONMENT_IS_WORKER;var scriptDirectory="";function locateFile(path){if(Module["locateFile"]){return Module["locateFile"](path,scriptDirectory)}return scriptDirectory+path}var read_,readAsync,readBinary,setWindowTitle;if(ENVIRONMENT_IS_WEB||ENVIRONMENT_IS_WORKER){if(ENVIRONMENT_IS_WORKER){scriptDirectory=self.location.href}else if(typeof document!=="undefined"&&document.currentScript){scriptDirectory=document.currentScript.src}if(scriptDirectory.indexOf("blob:")!==0){scriptDirectory=scriptDirectory.substr(0,scriptDirectory.lastIndexOf("/")+1)}else{scriptDirectory=""}{read_=function shell_read(url){var xhr=new XMLHttpRequest;xhr.open("GET",url,false);xhr.send(null);return xhr.responseText};if(ENVIRONMENT_IS_WORKER){readBinary=function readBinary(url){var xhr=new XMLHttpRequest;xhr.open("GET",url,false);xhr.responseType="arraybuffer";xhr.send(null);return new Uint8Array(xhr.response)}}readAsync=function readAsync(url,onload,onerror){var xhr=new XMLHttpRequest;xhr.open("GET",url,true);xhr.responseType="arraybuffer";xhr.onload=function xhr_onload(){if(xhr.status==200||xhr.status==0&&xhr.response){onload(xhr.response);return}onerror()};xhr.onerror=onerror;xhr.send(null)}}setWindowTitle=function(title){document.title=title}}else{}var out=Module["print"]||console.log.bind(console);var err=Module["printErr"]||console.warn.bind(console);for(key in moduleOverrides){if(moduleOverrides.hasOwnProperty(key)){Module[key]=moduleOverrides[key]}}moduleOverrides=null;if(Module["arguments"])arguments_=Module["arguments"];if(Module["thisProgram"])thisProgram=Module["thisProgram"];if(Module["quit"])quit_=Module["quit"];var wasmBinary;if(Module["wasmBinary"])wasmBinary=Module["wasmBinary"];var noExitRuntime;if(Module["noExitRuntime"])noExitRuntime=Module["noExitRuntime"];if(typeof WebAssembly!=="object"){abort("no native wasm support detected")}var wasmMemory;var ABORT=false;var EXITSTATUS=0;var UTF8Decoder=typeof TextDecoder!=="undefined"?new TextDecoder("utf8"):undefined;function UTF8ArrayToString(heap,idx,maxBytesToRead){var endIdx=idx+maxBytesToRead;var endPtr=idx;while(heap[endPtr]&&!(endPtr>=endIdx))++endPtr;if(endPtr-idx>16&&heap.subarray&&UTF8Decoder){return UTF8Decoder.decode(heap.subarray(idx,endPtr))}else{var str="";while(idx<endPtr){var u0=heap[idx++];if(!(u0&128)){str+=String.fromCharCode(u0);continue}var u1=heap[idx++]&63;if((u0&224)==192){str+=String.fromCharCode((u0&31)<<6|u1);continue}var u2=heap[idx++]&63;if((u0&240)==224){u0=(u0&15)<<12|u1<<6|u2}else{u0=(u0&7)<<18|u1<<12|u2<<6|heap[idx++]&63}if(u0<65536){str+=String.fromCharCode(u0)}else{var ch=u0-65536;str+=String.fromCharCode(55296|ch>>10,56320|ch&1023)}}}return str}function UTF8ToString(ptr,maxBytesToRead){return ptr?UTF8ArrayToString(HEAPU8,ptr,maxBytesToRead):""}function alignUp(x,multiple){if(x%multiple>0){x+=multiple-x%multiple}return x}var buffer,HEAP8,HEAPU8,HEAP16,HEAPU16,HEAP32,HEAPU32,HEAPF32,HEAPF64;function updateGlobalBufferAndViews(buf){buffer=buf;Module["HEAP8"]=HEAP8=new Int8Array(buf);Module["HEAP16"]=HEAP16=new Int16Array(buf);Module["HEAP32"]=HEAP32=new Int32Array(buf);Module["HEAPU8"]=HEAPU8=new Uint8Array(buf);Module["HEAPU16"]=HEAPU16=new Uint16Array(buf);Module["HEAPU32"]=HEAPU32=new Uint32Array(buf);Module["HEAPF32"]=HEAPF32=new Float32Array(buf);Module["HEAPF64"]=HEAPF64=new Float64Array(buf)}var INITIAL_MEMORY=Module["INITIAL_MEMORY"]||16777216;if(Module["wasmMemory"]){wasmMemory=Module["wasmMemory"]}else{wasmMemory=new WebAssembly.Memory({"initial":INITIAL_MEMORY/65536,"maximum":2147483648/65536})}if(wasmMemory){buffer=wasmMemory.buffer}INITIAL_MEMORY=buffer.byteLength;updateGlobalBufferAndViews(buffer);var wasmTable;var __ATPRERUN__=[];var __ATINIT__=[];var __ATMAIN__=[];var __ATPOSTRUN__=[function(){self.onEngineInit();}];var runtimeInitialized=false;function preRun(){if(Module["preRun"]){if(typeof Module["preRun"]=="function")Module["preRun"]=[Module["preRun"]];while(Module["preRun"].length){addOnPreRun(Module["preRun"].shift())}}callRuntimeCallbacks(__ATPRERUN__)}function initRuntime(){runtimeInitialized=true;callRuntimeCallbacks(__ATINIT__)}function preMain(){callRuntimeCallbacks(__ATMAIN__)}function postRun(){if(Module["postRun"]){if(typeof Module["postRun"]=="function")Module["postRun"]=[Module["postRun"]];while(Module["postRun"].length){addOnPostRun(Module["postRun"].shift())}}callRuntimeCallbacks(__ATPOSTRUN__)}function addOnPreRun(cb){__ATPRERUN__.unshift(cb)}function addOnPostRun(cb){__ATPOSTRUN__.unshift(cb)}var runDependencies=0;var runDependencyWatcher=null;var dependenciesFulfilled=null;function addRunDependency(id){runDependencies++;if(Module["monitorRunDependencies"]){Module["monitorRunDependencies"](runDependencies)}}function removeRunDependency(id){runDependencies--;if(Module["monitorRunDependencies"]){Module["monitorRunDependencies"](runDependencies)}if(runDependencies==0){if(runDependencyWatcher!==null){clearInterval(runDependencyWatcher);runDependencyWatcher=null}if(dependenciesFulfilled){var callback=dependenciesFulfilled;dependenciesFulfilled=null;callback()}}}Module["preloadedImages"]={};Module["preloadedAudios"]={};function abort(what){if(Module["onAbort"]){Module["onAbort"](what)}what+="";err(what);ABORT=true;EXITSTATUS=1;what="abort("+what+"). Build with -s ASSERTIONS=1 for more info.";var e=new WebAssembly.RuntimeError(what);throw e}function hasPrefix(str,prefix){return String.prototype.startsWith?str.startsWith(prefix):str.indexOf(prefix)===0}var dataURIPrefix="data:application/octet-stream;base64,";function isDataURI(filename){return hasPrefix(filename,dataURIPrefix)}var wasmBinaryFile="spell.wasm";if(!isDataURI(wasmBinaryFile)){wasmBinaryFile=locateFile(wasmBinaryFile)}function getBinary(){try{if(wasmBinary){return new Uint8Array(wasmBinary)}if(readBinary){return readBinary(wasmBinaryFile)}else{throw"both async and sync fetching of the wasm failed"}}catch(err){abort(err)}}function getBinaryPromise2(){if(!wasmBinary&&(ENVIRONMENT_IS_WEB||ENVIRONMENT_IS_WORKER)&&typeof fetch==="function"){return fetch(wasmBinaryFile,{credentials:"same-origin"}).then(function(response){if(!response["ok"]){throw"failed to load wasm binary file at '"+wasmBinaryFile+"'"}return response["arrayBuffer"]()}).catch(function(){return getBinary()})}return Promise.resolve().then(getBinary)}function createWasm(){var info={"a":asmLibraryArg};function receiveInstance(instance,module){var exports=instance.exports;Module["asm"]=exports;wasmTable=Module["asm"]["i"];removeRunDependency("wasm-instantiate")}addRunDependency("wasm-instantiate");function receiveInstantiatedSource(output){receiveInstance(output["instance"])}function instantiateArrayBuffer(receiver){return getBinaryPromise().then(function(binary){return WebAssembly.instantiate(binary,info)}).then(receiver,function(reason){err("failed to asynchronously prepare wasm: "+reason);abort(reason)})}function instantiateAsync(){if(!wasmBinary&&typeof WebAssembly.instantiateStreaming==="function"&&!isDataURI(wasmBinaryFile)&&typeof fetch==="function"){return fetch(wasmBinaryFile,{credentials:"same-origin"}).then(function(response){var result=WebAssembly.instantiateStreaming(response,info);return result.then(receiveInstantiatedSource,function(reason){err("wasm streaming compile failed: "+reason);err("falling back to ArrayBuffer instantiation");return instantiateArrayBuffer(receiveInstantiatedSource)})})}else{return instantiateArrayBuffer(receiveInstantiatedSource)}}if(Module["instantiateWasm"]){try{var exports=Module["instantiateWasm"](info,receiveInstance);return exports}catch(e){err("Module.instantiateWasm callback failed with error: "+e);return false}}instantiateAsync();return{}}function callRuntimeCallbacks(callbacks){while(callbacks.length>0){var callback=callbacks.shift();if(typeof callback=="function"){callback(Module);continue}var func=callback.func;if(typeof func==="number"){if(callback.arg===undefined){wasmTable.get(func)()}else{wasmTable.get(func)(callback.arg)}}else{func(callback.arg===undefined?null:callback.arg)}}}function _abort(){abort()}function _clock(){if(_clock.start===undefined)_clock.start=Date.now();return(Date.now()-_clock.start)*(1e6/1e3)|0}function _emscripten_memcpy_big(dest,src,num){HEAPU8.copyWithin(dest,src,src+num)}function _emscripten_get_heap_size(){return HEAPU8.length}function emscripten_realloc_buffer(size){try{wasmMemory.grow(size-buffer.byteLength+65535>>>16);updateGlobalBufferAndViews(wasmMemory.buffer);return 1}catch(e){}}function _emscripten_resize_heap(requestedSize){requestedSize=requestedSize>>>0;var oldSize=_emscripten_get_heap_size();var maxHeapSize=2147483648;if(requestedSize>maxHeapSize){return false}var minHeapSize=16777216;for(var cutDown=1;cutDown<=4;cutDown*=2){var overGrownHeapSize=oldSize*(1+.2/cutDown);overGrownHeapSize=Math.min(overGrownHeapSize,requestedSize+100663296);var newSize=Math.min(maxHeapSize,alignUp(Math.max(minHeapSize,requestedSize,overGrownHeapSize),65536));var replacement=emscripten_realloc_buffer(newSize);if(replacement){return true}}return false}var SYSCALLS={mappings:{},buffers:[null,[],[]],printChar:function(stream,curr){var buffer=SYSCALLS.buffers[stream];if(curr===0||curr===10){(stream===1?out:err)(UTF8ArrayToString(buffer,0));buffer.length=0}else{buffer.push(curr)}},varargs:undefined,get:function(){SYSCALLS.varargs+=4;var ret=HEAP32[SYSCALLS.varargs-4>>2];return ret},getStr:function(ptr){var ret=UTF8ToString(ptr);return ret},get64:function(low,high){return low}};function _fd_close(fd){return 0}function _fd_seek(fd,offset_low,offset_high,whence,newOffset){}function _fd_write(fd,iov,iovcnt,pnum){var num=0;for(var i=0;i<iovcnt;i++){var ptr=HEAP32[iov+i*8>>2];var len=HEAP32[iov+(i*8+4)>>2];for(var j=0;j<len;j++){SYSCALLS.printChar(fd,HEAPU8[ptr+j])}num+=len}HEAP32[pnum>>2]=num;return 0}__ATINIT__.push({func:function(){___wasm_call_ctors()}});var asmLibraryArg={"c":_abort,"b":_clock,"f":_emscripten_memcpy_big,"g":_emscripten_resize_heap,"h":_fd_close,"e":_fd_seek,"d":_fd_write,"a":wasmMemory};var asm=createWasm();var ___wasm_call_ctors=Module["___wasm_call_ctors"]=function(){return(___wasm_call_ctors=Module["___wasm_call_ctors"]=Module["asm"]["j"]).apply(null,arguments)};var _free=Module["_free"]=function(){return(_free=Module["_free"]=Module["asm"]["k"]).apply(null,arguments)};var _malloc=Module["_malloc"]=function(){return(_malloc=Module["_malloc"]=Module["asm"]["l"]).apply(null,arguments)};var _Spellchecker_Malloc=Module["_Spellchecker_Malloc"]=function(){return(_Spellchecker_Malloc=Module["_Spellchecker_Malloc"]=Module["asm"]["m"]).apply(null,arguments)};var _Spellchecker_Free=Module["_Spellchecker_Free"]=function(){return(_Spellchecker_Free=Module["_Spellchecker_Free"]=Module["asm"]["n"]).apply(null,arguments)};var _Spellchecker_Create=Module["_Spellchecker_Create"]=function(){return(_Spellchecker_Create=Module["_Spellchecker_Create"]=Module["asm"]["o"]).apply(null,arguments)};var _Spellchecker_Destroy=Module["_Spellchecker_Destroy"]=function(){return(_Spellchecker_Destroy=Module["_Spellchecker_Destroy"]=Module["asm"]["p"]).apply(null,arguments)};var _Spellchecker_AddDictionary=Module["_Spellchecker_AddDictionary"]=function(){return(_Spellchecker_AddDictionary=Module["_Spellchecker_AddDictionary"]=Module["asm"]["q"]).apply(null,arguments)};var _Spellchecker_RemoveDicrionary=Module["_Spellchecker_RemoveDicrionary"]=function(){return(_Spellchecker_RemoveDicrionary=Module["_Spellchecker_RemoveDicrionary"]=Module["asm"]["r"]).apply(null,arguments)};var _Spellchecker_Load=Module["_Spellchecker_Load"]=function(){return(_Spellchecker_Load=Module["_Spellchecker_Load"]=Module["asm"]["s"]).apply(null,arguments)};var _Spellchecker_Spell=Module["_Spellchecker_Spell"]=function(){return(_Spellchecker_Spell=Module["_Spellchecker_Spell"]=Module["asm"]["t"]).apply(null,arguments)};var _Spellchecker_Suggest=Module["_Spellchecker_Suggest"]=function(){return(_Spellchecker_Suggest=Module["_Spellchecker_Suggest"]=Module["asm"]["u"]).apply(null,arguments)};var calledRun;dependenciesFulfilled=function runCaller(){if(!calledRun)run();if(!calledRun)dependenciesFulfilled=runCaller};function run(args){args=args||arguments_;if(runDependencies>0){return}preRun();if(runDependencies>0)return;function doRun(){if(calledRun)return;calledRun=true;Module["calledRun"]=true;if(ABORT)return;initRuntime();preMain();if(Module["onRuntimeInitialized"])Module["onRuntimeInitialized"]();postRun()}if(Module["setStatus"]){Module["setStatus"]("Running...");setTimeout(function(){setTimeout(function(){Module["setStatus"]("")},1);doRun()},1)}else{doRun()}}Module["run"]=run;if(Module["preInit"]){if(typeof Module["preInit"]=="function")Module["preInit"]=[Module["preInit"]];while(Module["preInit"].length>0){Module["preInit"].pop()()}}noExitRuntime=true;run();


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
        }

        xhr.send(null);
	}

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

		this.messages.shift();
	};

	this.allocString = function(string)
	{
		var inputLen = string.length;
		var testLen = 6 * inputLen + 1;
		if (testLen > this.tmpStrings.length)
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
		return Module._Spellchecker_Create();
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

