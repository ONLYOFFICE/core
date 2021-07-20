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

    //string_utf8

    //module

    /**
     *
     * @param config
     * @constructor
     * @extends {AscViewer.IFile}
     */
    function CFile()
    {
        AscViewer.IFile.call(this);
    }

    CFile.prototype = Object.create(AscViewer.IFile.prototype);
    CFile.prototype.constructor = CFile;

    CFile.prototype.loadFromData = function(arrayBuffer)
    {
        var data = new Uint8Array(arrayBuffer);
        var _stream = Module["_malloc"](data.length);
        Module["HEAP8"].set(data, _stream);
        this.nativeFile = Module["_XPS_Load"](_stream, data.length);
        Module["_free"](_stream);
        return this.getInfo();
    };
    CFile.prototype.getInfo = function()
    {
        if (!this.nativeFile)
            return false;
        
        var _info = Module["_XPS_GetInfo"](this.nativeFile);
        if (!_info)
            return false;
        
        var _pages = Module["HEAP32"][_info >> 2];
        var _buffer = new Int32Array(Module["HEAP8"].buffer, _info, 1 + 3 * _pages);
        
        var _cur = 1;
        for (var i = 0; i < _pages; i++)
        {
            this.pages.push({ W : _buffer[_cur++], H : _buffer[_cur++], Dpi : _buffer[_cur++] });
        }
        
        this.free(_info);
        return this.pages.length > 0;
    };
    CFile.prototype.getPagePixmap = function(pageIndex, width, height)
    {
        var res = Module["_XPS_GetPixmap"](this.nativeFile, pageIndex, width, height);

        var glyphs = Module["_XPS_GetGlyphs"](this.nativeFile, pageIndex);

        var lenArray = new Int32Array(Module["HEAP8"].buffer, glyphs, 4);
        if (lenArray == null)
            return res;
        var len = lenArray[0];
        len -= 4;
        if (len <= 0)
            return res;

        this.pages[pageIndex].Lines = [];
        var buffer = new Uint8Array(Module["HEAP8"].buffer, glyphs + 4, len);
        var index = 0;
        var Line = -1;
        var prevY = -1;
        while (index < len)
        {
            var lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
            var _fontName = "".fromUtf8(buffer, index, lenRec);
            index += lenRec;
            lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
            var _fontSize = parseFloat("".fromUtf8(buffer, index, lenRec));
            index += lenRec;
            lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
            var _X = parseFloat("".fromUtf8(buffer, index, lenRec));
            index += lenRec;
            lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
            var _Y = parseFloat("".fromUtf8(buffer, index, lenRec));
            // TODO: близость
            if (_Y != prevY)
            {
                if (Line >= 0)
                    this.pages[pageIndex].Lines[Line].Glyphs.sort((prev, next) => prev.X - next.X);
                Line++;
                this.pages[pageIndex].Lines.push({ Glyphs : [] });
                prevY = _Y;
            }
            index += lenRec;
            lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
            this.pages[pageIndex].Lines[Line].Glyphs.push({
                fontName : _fontName,
                fontSize : _fontSize,
                X : _X * 1.015,
                Y : _Y * 1.015,
                UChar : String.fromCharCode(lenRec)
            });
        }
        this.pages[pageIndex].Lines.sort((prev, next) => prev.Glyphs[0].Y - next.Glyphs[0].Y);
        Module["_XPS_Delete"](glyphs);
        return res;
    };
    CFile.prototype.structure = function()
    {
        var res = [];
        var str = Module["_XPS_GetStructure"](this.nativeFile);
        var lenArray = new Int32Array(Module["HEAP8"].buffer, str, 4);
        if (lenArray == null)
            return res;
        var len = lenArray[0];
        len -= 4;
        if (len <= 0)
            return res;

        var buffer = new Uint8Array(Module["HEAP8"].buffer, str + 4, len);
        var index = 0;
        var Line = -1;
        var prevY = -1;
        while (index < len)
        {
            var lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
            var _Page = lenRec;
            lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
            var _Level = lenRec;
            lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
            var _Y = parseFloat("".fromUtf8(buffer, index, lenRec));
            index += lenRec;
            lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
            var _Description = "".fromUtf8(buffer, index, lenRec);
            index += lenRec;

            res.push({ page : _Page, level : _Level, Y : _Y, description : _Description});
        }

        Module["_XPS_Delete"](str);
        return res;
    };
    CFile.prototype.close = function()
    {
        Module["_XPS_Close"](this.nativeFile);
        this.nativeFile = 0;
        this.pages = [];
    };
    CFile.prototype.memory = function()
    {
        return Module["HEAP8"];
    };
    CFile.prototype.free = function(pointer)
    {
        Module["_XPS_Delete"](pointer);
    };

    window["AscViewer"].XpsFile = CFile;

})(window, undefined);
