(function(window, undefined) {

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

		for(var i = 0; i < vector.length; i++) {
			if(vector[i] % 2 == 1) {
				hyphens.push((i + 1) / symbol);
			}
		}
		return hyphens;
	}
})(self);