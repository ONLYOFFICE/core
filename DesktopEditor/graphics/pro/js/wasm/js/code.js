function CRaster(settings)
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
	
	var enginePath = "./raster/";
	if (settings && settings.enginePath)
	{
		enginePath = settings.enginePath;
		if (enginePath.substring(enginePath.length - 1) != "/")
			enginePath += "/";
	}
	
	var isUseSharedWorker = !!window.SharedWorker;
	if (isUseSharedWorker && (false === settings.useShared))
		isUseSharedWorker = false;
	
	var worker_src = useWasm ? "raster.js" : "raster_ie.js";
	worker_src = enginePath + worker_src;
	
	var _worker = this;
	var _port = null;
	if (isUseSharedWorker)
	{
		this.worker = new SharedWorker(worker_src, "onlyoffice-raster");
		_port = this.worker.port;
	}
	else
	{
		this.worker = new Worker(worker_src);
		_port = this.worker;
	}
	
	_port.onmessage = function(message) 
	{
		_worker.oncommand && _worker.oncommand(message.data);
	};
	_port.postMessage({ "type" : "init" });
	
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