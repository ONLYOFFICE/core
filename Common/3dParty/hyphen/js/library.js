(function(window) {

	window.hyphen = window.hyphen || {};
	window.hyphen.isReady = false;

	var not_ready = function() {
		console.log('Module is not ready');
	}

	window.hyphen.destroyApplication = not_ready;
	window.hyphen.loadDictionary = not_ready;
	window.hyphen.hyphenWord = not_ready;

	window.hyphen.onLoadModule = function(exports) {
		window.hyphen.isReady = true;

		window.hyphen.destroyApplication = exports.destroyApplication;
		window.hyphen.loadDictionary = exports.loadDictionary;
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