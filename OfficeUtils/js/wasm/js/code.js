window.loadedZip = null;
window.onload = function()
{
	var holder = document.body;
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
			window.loadedZip = window.nativeZlibEngine.openZip(e.target.result);
			if (!window.loadedZip)
				return;
			window.writeFile();
		};
		reader.readAsArrayBuffer(file);
	
		return false; 
	};
};

window.writeFile = function()
{
	var dst = document.getElementById("main");
	if (!window.loadedZip)
		return;
	dst.innerHTML = window.loadedZip;
};
