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

function CNativePointer()
{
	this.ptr = null;
}
CNativePointer.prototype.free = function()
{
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
	// TODO:
};

CFile.prototype._closeFile = function()
{
	// TODO:
};

CFile.prototype._getType = function()
{
	// TODO:
};

CFile.prototype._getError = function()
{
	// TODO:
};

// FONTS
CFile.prototype._isNeedCMap = function()
{
	// TODO:
};

CFile.prototype._setCMap = function(memoryBuffer)
{
	// TODO:
};

CFile.prototype._getFontByID = function(ID)
{
	// TODO:
};

CFile.prototype._getInteractiveFormsFonts = function(type)
{
	// TODO:
};

// INFO DOCUMENT
CFile.prototype._getInfo = function()
{
	// TODO:
};

CFile.prototype._getStructure = function()
{
	// TODO:
};

CFile.prototype._getLinks = function(pageIndex)
{
	// TODO:
};

// WIDGETS & ANNOTATIONS
CFile.prototype._getInteractiveFormsInfo = function()
{
	// TODO:
};

CFile.prototype._getAnnotationsInfo = function(pageIndex)
{
	// TODO:
};

CFile.prototype._getButtonIcons = function(backgroundColor, pageIndex, isBase64, nWidget, nView)
{
	// TODO:
};

CFile.prototype._getAnnotationsAP = function(width, height, backgroundColor, pageIndex, nAnnot, nView)
{
	// TODO:
};

CFile.prototype._getInteractiveFormsAP = function(width, height, backgroundColor, pageIndex, nWidget, nView, nButtonView)
{
	// TODO:
};

// SCAN PAGES
CFile.prototype._scanPage = function(page, mode)
{
	// TODO:
};

CFile.prototype._getImageBase64 = function(rId)
{
	// TODO:
};

// TEXT
CFile.prototype._getGlyphs = function(pageIndex)
{
	// TODO:
};

CFile.prototype._destroyTextInfo = function()
{
	// TODO:
};

// RASTER
CFile.prototype._getPixmap = function(pageIndex, width, height, backgroundColor)
{
	// TODO:
};

// STATIC FUNCTIONS
CFile.prototype._InitializeFonts = function(basePath) 
{
};

CFile.prototype._CheckStreamId = function(data, status) 
{
};
