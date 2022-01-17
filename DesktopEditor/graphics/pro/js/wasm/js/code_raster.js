window.loadedImage = null;
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
			window.loadedImage = window.nativeRasterEngine.openImage(e.target.result);
			if (!window.loadedImage)
				return;

			window.onresize();
		};
		reader.readAsArrayBuffer(file);

		return false;
	};
};

window.onresize = function()
{
	var dst = document.getElementById("main");
	if (!window.loadedImage)
		return;
	dst.height = window.loadedImage.height > 1000 ? 1000 : window.loadedImage.height;
	dst.width  = dst.height == 1000 ? 1000 * window.loadedImage.width / window.loadedImage.height : window.loadedImage.width;
	dst.getContext("2d").transform(1, 0, 0, -1, 0, dst.height);
	dst.getContext("2d").drawImage(window.loadedImage, 0, 0, dst.width, dst.height);
};
