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
			window.loadedZip = window.nativeZlibEngine.OpenZipFromBuffer(e.target.result);
			if (!window.loadedZip)
				return;
			var files = window.loadedZip.GetFilesInZip();
			for (var i = 0; i < files.length; i++)
				window.writeFile(files[i]);
			window.loadedZip.CloseZip();
			
			var archive = window.loadedZip.CreateZipFromFiles(files);
			window.loadedZip.CloseZip();
			
			window.loadedZip.OpenZipFromUint8Array(archive);
			var files2 = window.loadedZip.GetFilesInZip();
			for (var i = 0; i < files2.length; i++)
			{
				files2[i].path += "new";
				window.loadedZip.AddFileInZip(files2[i]);
			}
			
			var files3 = window.loadedZip.GetFilesInZip();
			for (var i = 0; i < files3.length; i++)
				window.writeFile(files3[i]);
			window.loadedZip.CloseZip();
		};
		reader.readAsArrayBuffer(file);
	
		return false; 
	};
};

window.writeFile = function(file)
{
	if (!file) return;
	var dst = document.getElementById("main");
	dst.innerHTML += file.path + ' ' + file.length + ' ';
	dst.innerHTML += "in"; // Uint8Array file.file;
};
