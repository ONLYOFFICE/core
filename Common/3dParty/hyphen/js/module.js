//desktop_fetch

//polyfill

//module

/**
 * @param {Module} module
 * @param {Number} length
 * @return {Number}
 * Allocate memory for wasm, returns pointer
 */
function hyphenAllocateMemory(module, length) {
    const ptr = module._malloc(length);
    return ptr;
}

/**
 * @param {Module} module
 * @param {Number} ptr
 * @return {void}
 * Free memory
 */
function hyphenFreeMemory(module, ptr) {
    module._free(ptr);
}

/**
 * @param {Module} module
 * @param {Uint8Array} data
 * @param {Number} ptr
 * Fill memory
 */
function hyphenSetMemory(module, data, ptr) {
    module.HEAP8.set(data, ptr);
}
/**
 * 
 * @param {Module} module 
 * @return {Number}
 * Returns pointer
 */
function hyphenCreateHyphenApplication(module){
    return module._createHyphenApplication();
}
/**
 * 
 * @param {Module} module 
 * @param {Number} app 
 */
function hyphenDestroyHyphenApplication(module, app){
    module._destroyHyphenApplication(app);
}
/**
 * 
 * @param {Module} module 
 * @param {Number} app 
 * @param {String} src 
 * @param {String} lang 
 */
function hyphenLoadDictionary(module, app, src, lang){

    var src_size = 4 * src.length + 1;
    var lang_size = 4 * lang.length + 1;
    
    var _src = hyphenAllocateMemory(module, src_size);
    var _lang = hyphenAllocateMemory(module, lang_size);

    module.stringToUTF8(src, _src, src_size);
    module.stringToUTF8(lang, _lang, lang_size);

    module._loadDictionary(app, _src, _lang);

    hyphenFreeMemory(module, _src);
    hyphenFreeMemory(module, _lang);
}

/**
 * 
 * @param {Module} module 
 * @param {Number} app
 * @param {String} word 
 * @param {String} lang 
 * @returns {Uint8ClampedArray}
 * Returns hyphen vector of word
 */
function hyphenWord(module, app, word, lang){

    var word_size = 4 * word.length + 1;
    var lang_size = 4 * lang.length + 1;

    var _word = hyphenAllocateMemory(module, word_size);
    var _lang = hyphenAllocateMemory(module, lang_size);

    module.stringToUTF8(word, _word, word_size);
    module.stringToUTF8(lang, _lang, lang_size);

    const ptr = module._hyphenWord(app, _word, _lang);

    hyphenFreeMemory(module, _word);
    hyphenFreeMemory(module, _lang);

    var hyphens = [];
    var vector = new Uint8ClampedArray(module.HEAP8.buffer, ptr, 4 * word.length + 6);

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

