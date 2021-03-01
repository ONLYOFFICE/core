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

	var isUseSharedWorker = !!window.SharedWorker;
	if (isUseSharedWorker && (false === settings.useShared))
		isUseSharedWorker = false;

	var worker_src = useWasm ? "spell.js" : "spell_ie.js";
	worker_src = enginePath + worker_src;

	var _worker = this;
	var _port = null;
	if (isUseSharedWorker)
	{
		this.worker = new SharedWorker(worker_src);
		_port = this.worker.port;
	}
	else
	{
		this.worker = new Worker(worker_src);
		_port = this.worker;
	}

	_port.onmessage = function(message) {
		_worker.oncommand && _worker.oncommand(message.data);
	};
	_port.postMessage({ "type" : "init", "dictionaries_path" : dictionariesPath });

	this.stop = function()
	{
		this.worker.terminate();
		this.worker = null;
	};

	this.command = function(message)
	{
		_port && _port.postMessage(message);
	};
	this.oncommand = function(message) { console.log(message); }
}
