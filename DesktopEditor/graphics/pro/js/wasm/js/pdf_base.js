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
        this.nativeFile = Module["_Open"](_stream, data.length);
        this.stream = _stream;
        return this.getInfo();
    };
    CFile.prototype.getInfo = function()
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
            this.pages.push({ W : _buffer[_cur++], H : _buffer[_cur++], Dpi : _buffer[_cur++] });
        }
        
        this.free(_info);
        return this.pages.length > 0;
    };
    CFile.prototype.getPagePixmap = function(pageIndex, width, height)
    {
        return Module["_GetPixmap"](this.nativeFile, pageIndex, width, height);
    };
    CFile.prototype.getGlyphs = function(pageIndex, width, height)
	{
	};
	CFile.prototype.getLinks = function(pageIndex, width, height)
	{
		return [];
	};
	CFile.prototype.structure = function()
	{
		return [];
	};
    CFile.prototype.close = function()
    {
        Module["_Close"](this.nativeFile);
        this.nativeFile = 0;
        this.pages = [];
		if (this.stream > 0)
        	Module["_free"](this.stream);
        this.stream = -1;
    };
    CFile.prototype.memory = function()
    {
        return Module["HEAP8"];
    };
    CFile.prototype.free = function(pointer)
    {
        Module["_free"](pointer);
    };

    window["AscViewer"].PdfFile = CFile;

})(window, undefined);
