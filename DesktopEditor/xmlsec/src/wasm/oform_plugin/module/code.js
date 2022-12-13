window.onload = function()
{
	let url = "./deploy/engine";
	let useWasm = false;
	let webAsmObj = window["WebAssembly"];
	if (typeof webAsmObj === "object" && typeof webAsmObj["Memory"] === "function")
	{
		if ((typeof webAsmObj["instantiateStreaming"] === "function") || (typeof webAsmObj["instantiate"] === "function"))
			useWasm = true;
	}

	url += (useWasm ? ".js" : "_ie.js");
	
	let script = document.createElement('script');
    script.type = 'text/javascript';
    script.src = url;
    document.head.appendChild(script);
};
