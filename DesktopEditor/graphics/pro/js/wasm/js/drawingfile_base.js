/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

(function(window, undefined) {

    var printErr = undefined;
    var FS = undefined;
    var print = undefined;

    var getBinaryPromise = null;
    if (window["AscDesktopEditor"] && document.currentScript && 0 == document.currentScript.src.indexOf("file:///"))
    {
        // fetch not support file:/// scheme
        window.fetch = undefined;

        getBinaryPromise = function() {

            var wasmPath = "ascdesktop://fonts/" + wasmBinaryFile.substr(8);
            return new Promise(function (resolve, reject) {

                var xhr = new XMLHttpRequest();
                xhr.open('GET', wasmPath, true);
                xhr.responseType = 'arraybuffer';

                if (xhr.overrideMimeType)
                    xhr.overrideMimeType('text/plain; charset=x-user-defined');
                else
                    xhr.setRequestHeader('Accept-Charset', 'x-user-defined');

                xhr.onload = function () {
                    if (this.status == 200) {
                        resolve(new Uint8Array(this.response));
                    }
                };

                xhr.send(null);

            });
        }
    }
    else
    {
        getBinaryPromise = function() {
            return getBinaryPromise2();
        }
    }
	
	function getMemoryPathIE(name)
	{
		if (self["AscViewer"] && self["AscViewer"]["baseUrl"])
			return self["AscViewer"]["baseUrl"] + name;
		return name;
	}

    //polyfill

    //string_utf8

    //module

    function CBinaryReader(data, start, size)
    {
        this.data = data;
        this.pos = start;
        this.limit = start + size;
    }
    CBinaryReader.prototype.readInt = function()
    {
        var val = this.data[this.pos] | this.data[this.pos + 1] << 8 | this.data[this.pos + 2] << 16 | this.data[this.pos + 3] << 24;
        this.pos += 4;
        return val;
    };
    CBinaryReader.prototype.readDouble = function()
    {
        return this.readInt() / 100;
    };
    CBinaryReader.prototype.readString = function()
    {
        var len = this.readInt();
        var val = String.prototype.fromUtf8(this.data, this.pos, len);
        this.pos += len;
        return val;
    };
    CBinaryReader.prototype.isValid = function()
    {
        return (this.pos < this.limit) ? true : false;
    };

    function CFile()
    {
        this.nativeFile = 0;
        this.stream = -1;
        this.type = -1;
        this.pages = [];
    }

    CFile.prototype["loadFromData"] = function(arrayBuffer)
    {
        var data = new Uint8Array(arrayBuffer);
        var _stream = Module["_malloc"](data.length);
        Module["HEAP8"].set(data, _stream);
        this.nativeFile = Module["_Open"](_stream, data.length);
        this.stream = _stream;
        this.type = Module["_GetType"](_stream, data.length);
        return this.getInfo();
    };
    CFile.prototype["getInfo"] = function()
    {
        if (!this.nativeFile)
            return false;

        var _info = Module["_GetInfo"](this.nativeFile);
        if (!_info)
            return false;

        var _pages = Module["HEAP32"][_info >> 2];
        var _buffer = new Int32Array(Module["HEAP8"].buffer, _info, 1 + 3 * _pages);

        var _cur = 1;
        for (var i = 0; i < _pages; i++)
        {
            this.pages.push({ 
                "W" : _buffer[_cur++], 
                "H" : _buffer[_cur++], 
                "Dpi" : _buffer[_cur++] 
            });
        }

        this.free(_info);
        return this.pages.length > 0;
    };
    CFile.prototype["close"] = function()
    {
        Module["_Close"](this.nativeFile);
        this.nativeFile = 0;
        this.pages = [];
        if (this.stream > 0)
            Module["_free"](this.stream);
        this.stream = -1;
    };

    CFile.prototype["getPages"] = function()
    {
        return this.pages;
    };

    CFile.prototype["getPagePixmap"] = function(pageIndex, width, height)
    {
        return Module["_GetPixmap"](this.nativeFile, pageIndex, width, height);
    };
    CFile.prototype["getGlyphs"] = function(pageIndex, width, height)
    {
        var glyphs = Module["_GetGlyphs"](this.nativeFile, pageIndex, width, height);
        if (glyphs == null)
            return;

        var lenArray = new Int32Array(Module["HEAP8"].buffer, glyphs, 4);
        var len = lenArray[0];
        len -= 4;
        if (len <= 0)
            return;

        this.pages[pageIndex].Lines = [];
        var buffer = new Uint8Array(Module["HEAP8"].buffer, glyphs + 4, len);
        var reader = new CBinaryReader(buffer, 0, len);

        var Line = -1;
        while (reader.isValid())
        {
            var rec = {};
            rec["word"] = reader.readString();
            if (this.type == 2)
            {
                rec["x"] = 1.015 * reader.readDouble();
                rec["y"] = 1.015 * reader.readDouble();
            }
            else
            {
                rec["x"] = reader.readDouble();
                rec["y"] = reader.readDouble();
            }
            rec["w"] = reader.readDouble();
            rec["h"] = reader.readDouble();

            Line++;
            this.pages[pageIndex].Lines.push({ Glyphs : [] });
            for (let i = 0; i < _Word.length; i++)
            {
                this.pages[pageIndex].Lines[Line].Glyphs.push({
                    X : _X + _W / (_Word.length - 1) * i,
                    UChar : _Word[i]
                });
            }
            this.pages[pageIndex].Lines[Line].Glyphs[0].Y = _Y + _H;
            this.pages[pageIndex].Lines[Line].Glyphs[0].fontSize = _H;
        }

        Module["_free"](glyphs);
    };
    CFile.prototype["getLinks"] = function(pageIndex, width, height)
    {
        var res = [];
        var ext = Module["_GetLinks"](this.nativeFile, pageIndex, width, height);
        var lenArray = new Int32Array(Module["HEAP8"].buffer, ext, 4);
        if (lenArray == null)
            return res;

        var len = lenArray[0];
        len -= 4;
        if (len <= 0)
            return res;

        var buffer = new Uint8Array(Module["HEAP8"].buffer, ext + 4, len);
        var reader = new CBinaryReader(buffer, 0, len);

        while (reader.isValid())
        {
            var rec = {};
            rec["link"] = reader.readString();
            if (this.type == 2)
            {
                rec["x"] = 1.015 * reader.readDouble();
                rec["y"] = 1.015 * reader.readDouble();
            }
            else
            {
                rec["x"] = reader.readDouble();
                rec["y"] = reader.readDouble();
            }
            rec["w"] = reader.readDouble();
            rec["h"] = reader.readDouble();
            res.push(rec);
        }

        Module["_free"](ext);
        return res;
    };
    CFile.prototype["getStructure"] = function()
    {
        var res = [];
        var str = Module["_GetStructure"](this.nativeFile);
        var lenArray = new Int32Array(Module["HEAP8"].buffer, str, 4);
        if (lenArray == null)
            return res;
        var len = lenArray[0];
        len -= 4;
        if (len <= 0)
            return res;

        var buffer = new Uint8Array(Module["HEAP8"].buffer, str + 4, len);
        var reader = new CBinaryReader(buffer, 0, len);

        while (reader.isValid())
        {
            var rec = {};
            rec["page"] = reader.readInt();
            rec["level"] = reader.readInt();
            if (this.type == 2)
            {
                rec["y"] = reader.readDouble();
            }
            else
            {
                rec["y"] = reader.readInt();
            }
            rec["description"] = reader.readString();
            res.push(rec);
        }

        Module["_free"](str);
        return res;
    };

    CFile.prototype.memory = function()
    {
        return Module["HEAP8"];
    };
    CFile.prototype.free = function(pointer)
    {
        Module["_free"](pointer);
    };

    window["AscViewer"]["CDrawingFile"] = CFile;

})(window, undefined);
