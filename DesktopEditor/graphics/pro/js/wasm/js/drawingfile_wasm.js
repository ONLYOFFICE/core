/*
 * (c) Copyright Ascensio System SIA 2010-2023
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

function CWasmPointer()
{
	this.ptr = 0;		
}
CWasmPointer.prototype.free = function()
{
	Module["_free"](this.ptr);
	this.ptr = 0;
};
CWasmPointer.prototype.getReader = function()
{
	if (!this.ptr)
		return null;
	
	let lenArr = new Int32Array(Module["HEAP8"].buffer, this.ptr, 1);
	if (!lenArr)
	{
		this.free();
		return null;
	}

	let len = lenArr[0];
	if (len <= 4)
	{
		this.free();
		return null;
	}

	len -= 4;
	let buffer = new Uint8Array(Module["HEAP8"].buffer, this.ptr + 4, len);
	return new CBinaryReader(buffer, 0, len);
};

var g_module_pointer = new CWasmPointer();

// js interface
CFile.prototype._free = function(ptr)
{
	Module["_free"](ptr);
};
CFile.prototype._getUint8Array = function(ptr, len)
{
	return new Uint8Array(Module["HEAP8"].buffer, ptr, len);
};
CFile.prototype._getUint8ClampedArray = function(ptr, len)
{
	return new Uint8ClampedArray(Module["HEAP8"].buffer, ptr, len);
};

// FILE
CFile.prototype._openFile = function(buffer, password)
{
	if (this.stream && buffer)
	{
		let data = new Uint8Array(buffer);
		this.stream_size = data.length;
		this.stream = Module["_malloc"](this.stream_size);
		Module["HEAP8"].set(data, this.stream);
	}

	let passwordPtr = 0;
	if (password)
	{
		let passwordBuf = password.toUtf8();
		let passwordPtr = Module["_malloc"](passwordBuf.length);
		Module["HEAP8"].set(passwordBuf, passwordPtr);
	}

	this.nativeFile = Module["_Open"](this.stream, this.stream_size, passwordPtr);

	if (passwordPtr)
		Module["_free"](passwordPtr);
	return this.nativeFile > 0 ? true : false;
};

CFile.prototype._closeFile = function()
{
	Module["_Close"](this.nativeFile);
};

CFile.prototype._getType = function()
{
	return Module["_GetType"](this.stream, this.stream_size);
};

CFile.prototype._getError = function()
{
	return Module["_GetErrorCode"](this.nativeFile);
};

// FONTS
CFile.prototype._isNeedCMap = function()
{
	let isNeed = Module["_IsNeedCMap"](this.nativeFile);
	return (isNeed === 1) ? true : false;
};

CFile.prototype._setCMap = function(memoryBuffer)
{
	let pointer = Module["_malloc"](memoryBuffer.length);
	Module.HEAP8.set(memoryBuffer, pointer);
	Module["_SetCMapData"](this.nativeFile, pointer, memoryBuffer.length);
};

CFile.prototype._getFontByID = function(ID)
{
	if (ID === undefined)
		return null;

	let idBuffer = ID.toUtf8();
	let idPointer = Module["_malloc"](idBuffer.length);
	Module["HEAP8"].set(idBuffer, idPointer);
	g_module_pointer.ptr = Module["_GetFontBinary"](this.nativeFile, idPointer);
	Module["_free"](idPointer);

	let reader = g_module_pointer.getReader();
	if (!reader)
		return null;

	let nFontLength = reader.readInt();
	let np1 = reader.readInt();
	let np2 = reader.readInt();
	let pFontPointer = np2 << 32 | np1;

	let res = new Uint8Array(Module["HEAP8"].buffer, pFontPointer, nFontLength);
	g_module_pointer.free();
	return res;
};

CFile.prototype._getInteractiveFormsFonts = function(type)
{
	g_module_pointer.ptr = Module["_GetInteractiveFormsFonts"](this.nativeFile, type);
	return g_module_pointer;
};

// INFO DOCUMENT
CFile.prototype._getInfo = function()
{
	g_module_pointer.ptr = Module["_GetInfo"](this.nativeFile);
	return g_module_pointer;
};

CFile.prototype._getStructure = function()
{
	g_module_pointer.ptr = Module["_GetStructure"](this.nativeFile);
	return g_module_pointer;
};

CFile.prototype._getLinks = function(pageIndex)
{
	g_module_pointer.ptr = Module["_GetLinks"](this.nativeFile, pageIndex);
	return g_module_pointer;
};

// WIDGETS & ANNOTATIONS
CFile.prototype._getInteractiveFormsInfo = function()
{
	g_module_pointer.ptr = Module["_GetInteractiveFormsInfo"](this.nativeFile);
	return g_module_pointer;
};

CFile.prototype._getAnnotationsInfo = function(pageIndex)
{
	g_module_pointer.ptr = Module["_GetAnnotationsInfo"](this.nativeFile, pageIndex === undefined ? -1 : pageIndex);
	return g_module_pointer;
};

CFile.prototype._getButtonIcons = function(backgroundColor, pageIndex, isBase64, nWidget, nView)
{
	g_module_pointer.ptr = Module["_GetButtonIcons"](this.nativeFile, 
		backgroundColor === undefined ? 0xFFFFFF : backgroundColor, 
		pageIndex, 
		isBase64 ? 1 : 0, 
		nWidget === undefined ? -1 : nWidget, 
		nView);
	return g_module_pointer;
};

CFile.prototype._getAnnotationsAP = function(width, height, backgroundColor, pageIndex, nAnnot, nView)
{
	g_module_pointer.ptr = Module["_GetAnnotationsAP"](this.nativeFile, 
		width, 
		height,
		backgroundColor === undefined ? 0xFFFFFF : backgroundColor,
		pageIndex,
		nAnnot === undefined ? -1 : nAnnot,
		nView);
	return g_module_pointer;
};

CFile.prototype._getInteractiveFormsAP = function(width, height, backgroundColor, pageIndex, nWidget, nView, nButtonView)
{
	g_module_pointer.ptr = Module["_GetInteractiveFormsAP"](this.nativeFile, 
		width, 
		height,
		backgroundColor === undefined ? 0xFFFFFF : backgroundColor,
		pageIndex,
		nWidget === undefined ? -1 : nWidget,
		nView, nButtonView);
	return g_module_pointer;
};

// SCAN PAGES
CFile.prototype._scanPage = function(page, mode)
{
	g_module_pointer.ptr = Module["_ScanPage"](this.nativeFile, page, (mode === undefined) ? 0 : mode);
	return g_module_pointer;
};

CFile.prototype._getImageBase64 = function(rId)
{
	let strPtr = Module["_GetImageBase64"](this.nativeFile, rId);
	if (0 == strPtr)
		return "error";
	let len = Module["_GetImageBase64Len"](strPtr);
	let ptr = Module["_GetImageBase64Ptr"](strPtr);

	var buffer = new Uint8Array(Module["HEAP8"].buffer, ptr, len);
	let result = String.prototype.fromUtf8(buffer, 0, len);
	Module["_GetImageBase64Free"](strPtr);
	return result;
};

// TEXT
CFile.prototype._getGlyphs = function(pageIndex)
{
	let ptr = Module["_GetGlyphs"](this.nativeFile, pageIndex);
	if (!ptr)
		return null;

	let ptrArray = new Int32Array(Module["HEAP8"].buffer, ptr, 5);
	let len = ptrArray[0];
	len -= 20;

	let res = {};
	res.info = [ptrArray[1], ptrArray[2], ptrArray[3], ptrArray[4]];

	if (len > 0)
	{
		let textCommandsSrc = new Uint8Array(Module["HEAP8"].buffer, ptr + 20, len);
		res.result = new Uint8Array(len);
		res.result.set(textCommandsSrc);
	}

	return res;
};

CFile.prototype._destroyTextInfo = function()
{
	Module["_DestroyTextInfo"]();
};

// RASTER
CFile.prototype._getPixmap = function(pageIndex, width, height, backgroundColor)
{
	return Module["_GetPixmap"](this.nativeFile, pageIndex, width, height, backgroundColor === undefined ? 0xFFFFFF : backgroundColor);
};

// STATIC FUNCTIONS
CFile.prototype._InitializeFonts = function(basePath) 
{
	if (undefined !== basePath && "" !== basePath)
		baseFontsPath = basePath;
	if (!window["g_fonts_selection_bin"])
		return;
	let memoryBuffer = window["g_fonts_selection_bin"].toUtf8();
	let pointer = Module["_malloc"](memoryBuffer.length);
	Module.HEAP8.set(memoryBuffer, pointer);
	Module["_InitializeFontsBase64"](pointer, memoryBuffer.length);
	Module["_free"](pointer);
	delete window["g_fonts_selection_bin"];

	// ranges
	let rangesBuffer = new CBinaryWriter();
	let ranges = AscFonts.getSymbolRanges();

	let rangesCount = ranges.length;
	rangesBuffer.writeUint(rangesCount);
	for (let i = 0; i < rangesCount; i++)
	{
		rangesBuffer.writeString(ranges[i].getName());
		rangesBuffer.writeUint(ranges[i].getStart());
		rangesBuffer.writeUint(ranges[i].getEnd());
	}

	let rangesFinalLen = rangesBuffer.dataSize;
	let rangesFinal = new Uint8Array(rangesBuffer.buffer.buffer, 0, rangesFinalLen);
	pointer = Module["_malloc"](rangesFinalLen);
	Module.HEAP8.set(rangesFinal, pointer);
	Module["_InitializeFontsRanges"](pointer, rangesFinalLen);
	Module["_free"](pointer);
};

CFile.prototype._CheckStreamId = function(data, status) 
{
	let drawingFile = self.drawingFile;
	if (!drawingFile)
		return;

	let lenArray = new Int32Array(Module["HEAP8"].buffer, data, 4);
	let len = lenArray[0];
	len -= 4;

	let buffer = new Uint8Array(Module["HEAP8"].buffer, data + 4, len);
	let reader = new CBinaryReader(buffer, 0, len);

	let name = reader.readString();
	let style = 0;
	if (reader.readInt() != 0)
		style |= 1;//AscFonts.FontStyle.FontStyleBold;
	if (reader.readInt() != 0)
		style |= 2;//AscFonts.FontStyle.FontStyleItalic;

	let file = AscFonts.pickFont(name, style);
	let fileId = file.GetID();
	let fileStatus = file.GetStatus();

	if (fileStatus === 0)
	{
		// font was loaded
		fontToMemory(file, true);
	}
	else
	{
		drawingFile.fontStreams[fileId] = drawingFile.fontStreams[fileId] || {};
		drawingFile.fontStreams[fileId].pages = drawingFile.fontStreams[fileId].pages || [];
		addToArrayAsDictionary(drawingFile.fontStreams[fileId].pages, drawingFile.fontPageIndex);
		addToArrayAsDictionary(drawingFile.pages[drawingFile.fontPageIndex].fonts, fileId);

		drawingFile.pages[drawingFile.fontPageIndex].fontsUpdateType |= drawingFile.fontPageUpdateType;

		// font can be loading in editor
		if (undefined === file.externalCallback)
		{
			let _t = file;
			file.externalCallback = function() {
				fontToMemory(_t, true);

				let pages = drawingFile.fontStreams[fileId].pages;
				delete drawingFile.fontStreams[fileId];

				let pagesRepaint_Page        = [];
				let pagesRepaint_Annotation  = [];
				let pagesRepaint_Forms       = [];

				for (let i = 0, len = pages.length; i < len; i++)
				{
					let pageNum = pages[i];
					let pageObj = drawingFile.pages[pageNum];
					let fonts = pageObj.fonts;
					
					for (let j = 0, len_fonts = fonts.length; j < len_fonts; j++)
					{
						if (fonts[j] == fileId)
						{
							fonts.splice(j, 1);
							break;
						}
					}

					if (0 == fonts.length)
					{
						if (pageObj.fontsUpdateType & UpdateFontsSource.Page)
							pagesRepaint_Page.push(pageNum);

						if (pageObj.fontsUpdateType & UpdateFontsSource.Annotation)
							pagesRepaint_Annotation.push(pageNum);

						if (pageObj.fontsUpdateType & UpdateFontsSource.Forms)
							pagesRepaint_Forms.push(pageNum);

						pageObj.fontsUpdateType = UpdateFontsSource.Undefined;
					}
				}

				if (pagesRepaint_Page.length > 0 && drawingFile.onRepaintPages)
					drawingFile.onRepaintPages(pagesRepaint_Page);

				if (pagesRepaint_Annotation.length > 0 && drawingFile.onRepaintAnnotations)
					drawingFile.onRepaintAnnotations(pagesRepaint_Annotation);

				if (pagesRepaint_Forms.length > 0 && drawingFile.onRepaintForms)
					drawingFile.onRepaintForms(pagesRepaint_Forms);

				delete _t.externalCallback;
			};

			if (2 !== file.LoadFontAsync)
				file.LoadFontAsync(baseFontsPath, null);
		}
	}

	let memoryBuffer = fileId.toUtf8();
	let pointer = Module["_malloc"](memoryBuffer.length);
	Module.HEAP8.set(memoryBuffer, pointer);
	Module["HEAP8"][status] = (fileStatus == 0) ? 1 : 0;
	return pointer;
};
