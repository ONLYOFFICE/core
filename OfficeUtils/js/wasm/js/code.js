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
			var jsZlib = new AscCommon.CZLibEngineJS();
			if (!jsZlib.open(e.target.result)) return;
			var files = jsZlib.files;
			var _files = {};
			for (var _path in files)
			{
				_files[_path] = jsZlib.getFile(_path);
				window.writeFile(_path, _files[_path]);
			}
			jsZlib.close();

			if (!jsZlib.create()) return;
			for (var _path in files)
			{
				jsZlib.addFile(_path, _files[_path]);
				jsZlib.addFile(_path + "new", _files[_path]);
				jsZlib.removeFile(_path);
			}
			var archive = jsZlib.save();
			jsZlib.close();
			
			jsZlib.open(archive);
			var files2 = jsZlib.files;
			for (var _path in files2)
			{
				jsZlib.getFile(_path);
				window.writeFile(_path, files2[_path]);
			}
			jsZlib.close();
		};
		reader.readAsArrayBuffer(file);
	
		return false; 
	};
};

window.writeFile = function(path, file)
{
	if (!file) return;
	var dst = document.getElementById("main");
	dst.innerHTML += ("<p>" + path + ' ' + file.length + "</p>");
};
