//desktop_fetch

//polyfill

//module

/**
 * @param {Module} module
 * @param {Number} length
 * @return {Number}
 * Allocate memory for wasm, returns pointer
 */
function allocateMemory(module, length) {
    const ptr = module._malloc(length);
    return ptr;
}

/**
 * @param {Module} module
 * @param {Number} ptr
 * @return {void}
 * Free memory
 */
function freeMemory(module, ptr) {
    module._free(ptr);
}

/**
 * @param {Module} module
 * @param {Uint8Array} data
 * @param {Number} ptr
 * Fill memory
 */
function setMemory(module, data, ptr) {
    module.HEAP8.set(data, ptr);
}
/**
 * 
 * @param {Module} module 
 * @return {Number}
 * Returns pointer
 */
function createHyphenApplication(module){
    return module._createHyphenApplication();
}
/**
 * 
 * @param {Module} module 
 * @param {Number} app 
 */
function destroyHyphenApplication(module, app){
    module._destroyHyphenApplication(app);
}
/**
 * 
 * @param {Module} module 
 * @param {Number} app 
 * @param {String} src 
 * @param {String} lang 
 */
function loadDictionary(module, app, src, lang){

    var src_size = 4 * src.length + 1;
    var lang_size = 4 * lang.length + 1;
    

    var _src = allocateMemory(module, src_size);
    var _lang = allocateMemory(module, lang_size);

    module.stringToUTF8(src, _src, src_size);
    module.stringToUTF8(lang, _lang, lang_size);

    module._loadDictionary(app, _src, _lang);

    freeMemory(module, _src);
    freeMemory(module, _lang);
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

    var _word = allocateMemory(module, word_size);
    var _lang = allocateMemory(module, lang_size);

    module.stringToUTF8(word, _word, word_size);
    module.stringToUTF8(lang, _lang, lang_size);

    var hyphens = module._hyphenWord(app, _word, _lang);

    freeMemory(module, _word);
    freeMemory(module, _lang);

    return hyphens;
}

(function (window, undefined){
    var textarea = document.getElementById("textarea");
    var form = document.querySelector("form");

    var control = document.getElementById("dictionary-file");

    var application = undefined;
    var lang = undefined;

    control.addEventListener("change", function(event) {
        var file = control.files[0];
        lang = file.name;
        var reader = new FileReader();
        reader.readAsText(file);
    
        if(application == undefined) {
            application = createHyphenApplication(Module);
        }
    
        reader.onload = function() {
            loadDictionary(Module, application, reader.result, lang);
        };
     
    }, false);
    
    form.onsubmit = function(event) {
        event.preventDefault();
        var text = textarea.value.split("\n").join(" ").split(" ");
    
        if(application == undefined) {
            application = createHyphenApplication(Module);
        }
    
        for(var i = 0; i < text.length; i++) {
            const ptr = hyphenWord(Module, application, text[i].toLowerCase(), lang);
            var vector = new Uint8ClampedArray(Module.HEAP8.buffer, ptr, 4 * text[i].length + 6);
    
            // calc actual size of vector
            var size = 0;
            for(var j = 0; j < vector.length && vector[j] != 0; j++){
                size++;
            }
    
            // size of one symbol
            var symbol = size / text[i].length;
    
            for(var j = 0; j < vector.length; j++) {
                if(vector[j] % 2 == 1) {
                    console.log((j + 1) / symbol);
                }
            }
        }
    }
})(self, undefined);

