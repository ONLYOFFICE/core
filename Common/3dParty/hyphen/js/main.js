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
 * @return {void}
 * Fill memory
 */
function setMemory(module, data, ptr) {
    module.HEAP8.set(data, ptr);
}

/**
 * @type {HTMLTextAreaElement}
 */
var textarea = document.getElementById("textarea");
var form = document.querySelector("form");

function hyphenate(word)
{
    var len = word.length;
    var word_size = 4 * len + 1;
    var hyphen_size = word_size + 5;
    var hword_size = word_size * 2;
    
    var pword = allocateMemory(Module, word_size);
    var phyphens = allocateMemory(Module, hyphen_size);
    var phword = allocateMemory(Module, hword_size);

    Module.stringToUTF8(word, pword, word_size);
    Module._hyphenate(pword, phyphens, phword)

    var hword = Module.UTF8ToString(phword);
    var hyphens = Module.UTF8ToString(phyphens);

    var positions = [];
    for(var i = 0; i < hyphens.length; i++) {
        // hyphenation vector has odd number in positions where hyphen is needed
        if(Number(hyphens[i]) % 2 == 1) {
            positions.push(i);
        }
    }
    console.log(hword);

    freeMemory(Module, pword);
    freeMemory(Module, phyphens);
    freeMemory(Module, phword);

    return positions;
}

form.onsubmit = function(event) {
    event.preventDefault();
    var text = textarea.value.split("\n").join(" ").split(" ");
    Module._load_dictionary();

    for(var i = 0; i < text.length; i++) {
        console.log(hyphenate(text[i].toLowerCase()));
    }
    Module._free_dictionary();
}

