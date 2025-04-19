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

//string_utf8

function CNativePointer()
{
	this.ptr = null;
}
CNativePointer.prototype.free = function()
{
	if (this.ptr)
		g_native_drawing_file["FreeWasmData"](this.ptr);
	this.ptr = null;
};
CNativePointer.prototype.getReader = function()
{
	if (!this.ptr)
		return null;

	return new CBinaryReader(this.ptr, 0, this.ptr.length);
};

var g_module_pointer = new CNativePointer();

// js interface
CFile.prototype._free = function(ptr)
{
	// TODO:
};
CFile.prototype._getUint8Array = function(ptr, len)
{
	// TODO:
};
CFile.prototype._getUint8ClampedArray = function(ptr, len)
{
	// TODO:
};

// FILE
CFile.prototype._openFile = function(buffer, password)
{
	let res = false;
	if (!buffer)
		res = -1 !== g_native_drawing_file["GetType"]();

	if (res)
		this.nativeFile = 1;
	return res;
};

CFile.prototype._closeFile = function()
{
	g_native_drawing_file["CloseFile"]();
	this.nativeFile = 0;
};

CFile.prototype._getType = function()
{
	return g_native_drawing_file["GetType"]();
};

CFile.prototype._getError = function()
{
	return g_native_drawing_file["GetErrorCode"]();
};

CFile.prototype._addPDF = function(buffer, password)
{
	return g_native_drawing_file["AddPDF"]();
}

// FONTS
CFile.prototype._isNeedCMap = function()
{
	return g_native_drawing_file["IsNeedCMap"]();
};

CFile.prototype._setCMap = function(memoryBuffer)
{
	// none
};

CFile.prototype._getFontByID = function(ID)
{
	return g_native_drawing_file["GetFontBinary"](ID);
};

CFile.prototype._getInteractiveFormsFonts = function(type)
{
	g_module_pointer.ptr = g_native_drawing_file["GetInteractiveFormsFonts"](type);
	return g_module_pointer;
};

// INFO DOCUMENT
CFile.prototype._getInfo = function()
{
	g_module_pointer.ptr = g_native_drawing_file["GetInfo"]();
	return g_module_pointer;
};

CFile.prototype._getStructure = function()
{
	g_module_pointer.ptr = g_native_drawing_file["GetStructure"]();
	return g_module_pointer;
};

CFile.prototype._getLinks = function(pageIndex)
{
	g_module_pointer.ptr = g_native_drawing_file["GetLinks"](pageIndex);
	return g_module_pointer;
};

// WIDGETS & ANNOTATIONS
CFile.prototype._getInteractiveFormsInfo = function()
{
	g_module_pointer.ptr = g_native_drawing_file["GetInteractiveFormsInfo"]();
	return g_module_pointer;
};

CFile.prototype._getAnnotationsInfo = function(pageIndex)
{
	g_module_pointer.ptr = g_native_drawing_file["GetAnnotationsInfo"](pageIndex === undefined ? -1 : pageIndex);
	return g_module_pointer;
};

CFile.prototype._getButtonIcons = function(backgroundColor, pageIndex, isBase64, nWidget, nView)
{
	g_module_pointer.ptr = g_native_drawing_file["GetButtonIcons"](
		backgroundColor === undefined ? 0xFFFFFF : backgroundColor, 
		pageIndex, 
		isBase64 ? 1 : 0, 
		nWidget === undefined ? -1 : nWidget, 
		nView);
	return g_module_pointer;
};

CFile.prototype._getAnnotationsAP = function(width, height, backgroundColor, pageIndex, nAnnot, nView)
{
	g_module_pointer.ptr = g_native_drawing_file["GetAnnotationsAP"](
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
	g_module_pointer.ptr = g_native_drawing_file["GetInteractiveFormsAP"](
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
	g_module_pointer.ptr = g_native_drawing_file["ScanPage"](page, (mode === undefined) ? 0 : mode);
	return g_module_pointer;
};

CFile.prototype._getImageBase64 = function(rId)
{
	return g_native_drawing_file["GetImageBase64"](rId);
};

// TEXT
CFile.prototype._getGlyphs = function(pageIndex)
{
	let res = {};
	res.info = [0, 0, 0, 0];
	res.result = [];
	return res;
};

CFile.prototype._destroyTextInfo = function()
{
	g_native_drawing_file["DestroyTextInfo"](rId);
};

// RASTER
CFile.prototype._getPixmap = function(pageIndex, width, height, backgroundColor)
{
	return null;
};

// STATIC FUNCTIONS
CFile.prototype._InitializeFonts = function(basePath) 
{
	// none
};

CFile.prototype._CheckStreamId = function(data, status) 
{
	// none
};
