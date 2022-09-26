(function(window) {

	var isModuleLoaded = false;
	var application;

	function onLoadModule() {
		isModuleLoaded = true;
		application = Module._hyphenCreateApplication();
		if (window.hyphen) {			
			window.hyphen.onLoadModule && window.hyphen.onLoadModule({
				destroyApplication: function() {
					Module._hyphenDestroyApplication(application);
				},
				loadDictionary: hyphenLoadDictionary,
				hyphenWord: hyphenWord
			});
		}
	};

	//desktop_fetch

	//polyfill

	//string_utf8

	//module

	/**
	 * 
	 * @param {Number} app 
	 * @param {arraybuffer} dict 
	 * @param {String} lang
	 * @returns {Boolean} isSuccess
	 */
	function hyphenLoadDictionary(dict, lang) {
		if(!isModuleLoaded) {
			return;
		}

		let dictSize = dict.byteLength;
		let dictPointer = Module._malloc(dictSize);
		Module.HEAP8.set(new Uint8ClampedArray(dict), dictPointer);

		let langPointer = lang.toUtf8Pointer();

		let result = Module._hyphenLoadDictionary(application, dictPointer, dictSize, langPointer.ptr);

		langPointer.free();
		Module._free(dictPointer);
		
		return (result === 1) ? true : false;
	}

	/**
	 * 
	 * @param {String} word 
	 * @param {String} lang 
	 * @returns {Uint8ClampedArray}
	 * Returns hyphen vector of word
	 */
	function hyphenWord(word, lang) {
		if (!isModuleLoaded) {
			return;
		}

		let wordPointer = word.toUtf8Pointer();
		let langPointer = lang.toUtf8Pointer();
		let hyphens = [];

		if (wordPointer && langPointer) {
			const ptr = Module._hyphenWord(application, wordPointer.ptr, langPointer.ptr);

			wordPointer.free();
			langPointer.free();

			var vector = new Uint8ClampedArray(Module.HEAP8.buffer, ptr, 4 * word.length + 6);
			for(var i = 0; i < vector.length && vector[i] != 0; i++) {
				if(vector[i] % 2 == 1) {
					hyphens.push((i + 1));
				}
			}
		}
		return hyphens;
	}
})(self);