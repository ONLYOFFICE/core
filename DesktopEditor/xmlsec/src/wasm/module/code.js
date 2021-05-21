window.onload = function()
{
	var holder = document.body;
	var i = 0;
	holder.ondragover = function(e) 
	{
		var isFile = false;
		if (e.dataTransfer.types)
		{
			for (var i = 0, length = e.dataTransfer.types.length; i < length; ++i)
			{
				var type = e.dataTransfer.types[i].toLowerCase();
				if (type == "files" && e.dataTransfer.items && e.dataTransfer.items.length == 1)
				{
					var item = e.dataTransfer.items[0];
					if (item.kind && "file" == item.kind.toLowerCase())
					{
						isFile = true;
						break;
					}
				}
			}
		}
		e.dataTransfer.dropEffect = isFile ? "copy" : "none";
		e.preventDefault();
		return false; 
	};
	holder.ondrop = function(e) 
	{ 
		var file = e.dataTransfer.files ? e.dataTransfer.files[0] : null;
		if (!file)
		{
			e.preventDefault();
			return false;
		}
		
		var reader = new FileReader();
		reader.onload = function(e) {
			if (i == 0)
			{
				if (!window.nativeXmlSignatureEngine.create()) return;
				window.nativeXmlSignatureEngine.loadCert(e.target.result);
				i++;
			}
			else if (i == 1)
			{
				window.nativeXmlSignatureEngine.loadKey(e.target.result);
				i++;
			}
			else if (i == 2)
			{
				var res = window.nativeXmlSignatureEngine.sign(e.target.result);
				window.nativeXmlSignatureEngine.close();
				window.writeFile(res);
				i++;
			}
		};
		reader.readAsArrayBuffer(file);
	
		return false; 
	};
};

window.writeFile = function(file)
{
	if (!file) return;
	var dst = document.getElementById("main");
	dst.innerHTML += file.length + '\n';
};
