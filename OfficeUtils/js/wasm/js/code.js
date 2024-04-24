/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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
