(function(window) {

	var isModuleLoaded = false;

	function onLoadModule() {
		isModuleLoaded = true;
		if (window.hyphen) {
			window.hyphen.onLoadModule && window.hyphen.onLoadModule({
				hyphenCreateApplication: hyphenCreateApplication,
				hyphenDestroyApplication: hyphenDestroyApplication,
				hyphenLoadDictionary: hyphenLoadDictionary,
				hyphenWord: hyphenWord,
			});
		}
	};

	// getBinaryPromise fix? dekstop_fetch included
	/*
	var fetch = self.fetch;
	var getBinaryPromise = null;
	if (self.AscDesktopEditor && document.currentScript && 0 == document.currentScript.src.indexOf("file:///")) {
		fetch = undefined; // fetch not support file:/// scheme

		getBinaryPromise = function() {
			var wasmPath = "ascdesktop://fonts/" + wasmBinaryFile.substr(8);
			
			return new Promise(function (resolve, reject) {
				var xhr = new XMLHttpRequest();
				xhr.open('GET', wasmPath, true);
				xhr.responseType = 'arraybuffer';

				if (xhr.overrideMimeType) {
					xhr.overrideMimeType('text/plain; charset=x-user-defined');
				} else {
					xhr.setRequestHeader('Accept-Charset', 'x-user-defined');
				}
				xhr.onload = function () {
					if (this.status == 200) {
						resolve(new Uint8Array(this.response));
					}
				};
				xhr.send(null);
			});
		}
	} else {
		getBinaryPromise = function() {
			return getBinaryPromise2();
		}
	}
	*/

	//desktop_fetch

	//polyfill

	//module

	/**
	 * @param {Number} length
	 * @return {Number}
	 * Allocate memory for wasm, returns pointer
	 */
	function hyphenAllocateMemory(length) {
		const ptr = Module._malloc(length);
		return ptr;
	}

	/**
	 * @param {Number} ptr
	 * @return {void}
	 * Free memory
	 */
	function hyphenFreeMemory(ptr) {
		Module._free(ptr);
	}

	/**
	 * @param {Uint8Array} data
	 * @param {Number} ptr
	 * Fill memory
	 */
	function hyphenSetMemory(data, ptr) {
		Module.HEAP8.set(data, ptr);
	}
	/**
	 * 
	 * @return {Number}
	 * Returns pointer
	 */
	function hyphenCreateApplication() {
		if(!isModuleLoaded) {
			return;
		}
		return Module._hyphenCreateApplication();
	}
	/**
	 * 
	 * @param {Number} app 
	 */
	function hyphenDestroyApplication(app) {
		if(!isModuleLoaded) {
			return;
		}
		Module._hyphenDestroyApplication(app);
	}
	/**
	 * 
	 * @param {Number} app 
	 * @param {String} src 
	 * @param {String} lang 
	 */
	function hyphenLoadDictionary(app, src, lang) {
		if(!isModuleLoaded) {
			return;
		}

		var dict = new Uint8ClampedArray(src);
		var dict_size = dict.length;
		var lang_size = 4 * lang.length + 1;
		
		var _dict = hyphenAllocateMemory(dict_size);
		var _lang = hyphenAllocateMemory(lang_size);

		hyphenSetMemory(dict, _dict);
		Module.stringToUTF8(lang, _lang, lang_size);

		Module._hyphenLoadDictionary(app, _dict, _lang);

		hyphenFreeMemory(_dict);
		hyphenFreeMemory(_lang);
	}

	/**
	 * 
	 * @param {Number} app
	 * @param {String} word 
	 * @param {String} lang 
	 * @returns {Uint8ClampedArray}
	 * Returns hyphen vector of word
	 */
	function hyphenWord(app, word, lang){
		if(!isModuleLoaded) {
			return;
		}

		var word_size = 4 * word.length + 1;
		var lang_size = 4 * lang.length + 1;

		var _word = hyphenAllocateMemory(word_size);
		var _lang = hyphenAllocateMemory(lang_size);

		Module.stringToUTF8(word, _word, word_size);
		Module.stringToUTF8(lang, _lang, lang_size);

		const ptr = Module._hyphenWord(app, _word, _lang);

		hyphenFreeMemory( _word);
		hyphenFreeMemory(_lang);

		var hyphens = [];
		var vector = new Uint8ClampedArray(Module.HEAP8.buffer, ptr, 4 * word.length + 6);

		// calc actual size of vector
		var size = 0;
		for(var i = 0; i < vector.length && vector[i] != 0; i++) {
			size++;
		}

		// size of one symbol
		var symbol = size / word.length;

		// if word is "broken", can returns floats
		for(var i = 0; i < vector.length; i++) {
			if(vector[i] % 2 == 1) {
				hyphens.push((i + 1) / symbol);
			}
		}
		return hyphens;
	}
})(self);