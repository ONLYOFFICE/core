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
			if (!window.nativeZlibEngine.open(e.target.result)) return;
			var files = window.nativeZlibEngine.files;
			var _files = {};
			for (var _path in files)
			{
				_files[_path] = window.nativeZlibEngine.getFile(_path);
				window.writeFile(_path, _files[_path]);
			}
			window.nativeZlibEngine.close();

			if (!window.nativeZlibEngine.create()) return;
			for (var _path in files)
			{
				window.nativeZlibEngine.addFile(_path, _files[_path]);
				window.nativeZlibEngine.addFile(_path + "new", _files[_path]);
				window.nativeZlibEngine.removeFile(_path);
			}
			var archive = window.nativeZlibEngine.save();
			window.nativeZlibEngine.close();
			
			window.nativeZlibEngine.open(archive2);
			var files3 = window.nativeZlibEngine.files;
			for (var _path in files3)
			{
				window.nativeZlibEngine.getFile(_path);
				window.writeFile(_path, files3[_path]);
			}
			window.nativeZlibEngine.close();
		};
		reader.readAsArrayBuffer(file);
	
		return false; 
	};
};

window.writeFile = function(path, file)
{
	if (!file) return;
	var dst = document.getElementById("main");
	dst.innerHTML += path + ' ' + file.length + '\n';
};
