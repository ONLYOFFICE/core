(function(window, undefined) {

    window.hyphen = window.hyphen || {};
    window.hyphen.isReady = false;

	var not_ready = function() {
        console.log('Module is not ready');
	}

    window.hyphen.hyphenCreateApplication = not_ready;
	window.hyphen.hyphenDestroyApplication = not_ready;
	window.hyphen.hyphenLoadDictionary = not_ready;
	window.hyphen.hyphenWord = not_ready;

    window.hyphen.onLoadModule = function(exports) {
        window.hyphen.isReady = true;

        window.hyphen.hyphenCreateApplication = exports.hyphenCreateApplication;
		window.hyphen.hyphenDestroyApplication = exports.hyphenDestroyApplication;
		window.hyphen.hyphenLoadDictionary = exports.hyphenLoadDictionary;
		window.hyphen.hyphenWord = exports.hyphenWord;
    };

	window.hyphen.loadModule = function() {
		var path = '../deploy/engine/';

		// wasm support check
		var useWasm = false;
        const webAsmObj = window['WebAssembly'];
        if (typeof webAsmObj === 'object') {
            if (typeof webAsmObj['Memory'] === 'function') {
                if ((typeof webAsmObj['instantiateStreaming'] === 'function') || (typeof webAsmObj['instantiate'] === 'function')) {
                    useWasm = true;
                }
            }
        }
		path += (useWasm ? 'hyphen.js' : 'hyphen_ie.js');

		const script = document.createElement('script');
		script.type = 'text/javascript';
		script.src = path;
		document.head.appendChild(script);

	}
})(self);