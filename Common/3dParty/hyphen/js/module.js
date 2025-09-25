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
	 * @param {Number} lang
	 * @param {arraybuffer} dict
	 * @returns {Boolean} isSuccess
	 */
	function hyphenLoadDictionary(lang, dict) 
	{
		if (!isModuleLoaded)
			return;

		let dictSize = dict.byteLength;
		let dictPointer = Module._malloc(dictSize);
		Module.HEAP8.set(new Uint8ClampedArray(dict), dictPointer);

		let result = Module._hyphenLoadDictionary(application, lang, dictPointer, dictSize);

		Module._free(dictPointer);
		
		return (result === 0) ? true : false;
	}

	/**
	 * 
	 * @param {Number} lang 
	 * @param {String} word 
	 * @returns {Array}
	 * Returns hyphen vector of word
	 */
	function hyphenWord(lang, word) 
	{
		if (!isModuleLoaded)
			return;

		let wordPointer = word.toUtf8Pointer();
		let wordLen = wordPointer.length;
		let hyphens = [];

		if (wordPointer) 
		{
			const ptr = Module._hyphenWord(application, lang, wordPointer.ptr, wordLen);
			wordPointer.free();

			let vector = new Uint8ClampedArray(Module.HEAP8.buffer, ptr, wordLen + 5);
			for (let i = 0; vector[i] != 0; i++) 
			{
				if (1 == (vector[i] & 1))
					hyphens.push((i + 1));
			}
		}
		return hyphens;
	}
})(self);