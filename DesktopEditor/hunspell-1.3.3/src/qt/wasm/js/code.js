function CSpellchecker(settings)
{
	useWasm = false;
	var webAsmObj = window["WebAssembly"];
    if (typeof webAsmObj === "object")
	{
        if (typeof webAsmObj["Memory"] === "function")
		{
			if ((typeof webAsmObj["instantiateStreaming"] === "function") || (typeof webAsmObj["instantiate"] === "function"))
				useWasm = true;
		}
	}

	var enginePath = "./spell/";
	if (settings && settings.enginePath)
	{
		enginePath = settings.enginePath;
		if (enginePath.substring(enginePath.length - 1) != "/")
			enginePath += "/";
	}

	var dictionariesPath = "./../dictionaries";
	if (settings && settings.dictionariesPath)
	{
		dictionariesPath = settings.dictionariesPath;
		if (dictionariesPath.substring(dictionariesPath.length - 1) == "/")
			dictionariesPath = dictionariesPath.substr(0, dictionariesPath.length - 1);
	}

	var worker_src = useWasm ? "spell.js" : "spell_ie.js";
	worker_src = enginePath + worker_src;

	this.worker = new Worker(worker_src);
	var _worker = this;
	this.worker.onmessage = function(message) {
		_worker.oncommand && _worker.oncommand(message.data);
	};
	this.worker.postMessage({ "type" : "init", "dictionaries_path" : dictionariesPath });

	this.stop = function()
	{
		this.worker.terminate();
		this.worker = null;
	};

	this.command = function(message)
	{
		this.worker && this.worker.postMessage(message);
	};
	this.oncommand = function(message) { console.log(message); }
}
