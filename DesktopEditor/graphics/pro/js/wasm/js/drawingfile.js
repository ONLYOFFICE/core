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

var UpdateFontsSource = {
	Undefined   : 0,
	Page        : 1,
	Annotation  : 2,
	Forms       : 4
};

function CFile()
{
	this.nativeFile = 0;
	this.stream = [];
	this.stream_size = 0;
	this.type = -1;
	this.pages = [];
	this.info = null;
	this._isNeedPassword = false;

	// for async fonts loader
	this.fontPageIndex = -1;
	this.fontPageUpdateType = UpdateFontsSource.Undefined;
	this.fontStreams = {};

	this.scannedImages = {};
}

//file_internal

CFile.prototype.lockPageNumForFontsLoader = function(pageIndex, type)
{
	this.fontPageIndex = pageIndex;
	this.fontPageUpdateType = type;
};
CFile.prototype.unlockPageNumForFontsLoader = function()
{
	this.fontPageIndex = -1;
	drawingFile.fontPageUpdateType = UpdateFontsSource.Undefined;
};

CFile.prototype["getPages"] = function()
{
	return this.pages;
};

CFile.prototype["openForms"] = function()
{
};

CFile.prototype["getDocumentInfo"] = function()
{
	return this.info;
};

CFile.prototype["getStartID"] = function()
{
	return this.StartID;
};

// FILE
CFile.prototype["loadFromData"] = function(arrayBuffer)
{
	let isSuccess = this._openFile(arrayBuffer);
	let error = this._getError(); // 0 - ok, 4 - password, else: error
	this.type = this._getType();

	self.drawingFile = this;
	if (!error)
		this.getInfo();
	this._isNeedPassword = (4 === error) ? true : false;
	return error;
};
CFile.prototype["loadFromDataWithPassword"] = function(password)
{
	if (0 != this.nativeFile)
		this._closeFile();

	let isSuccess = this._openFile(undefined, password);
	let error = this._getError(); // 0 - ok, 4 - password, else: error
	this.type = this._getType();

	self.drawingFile = this;
	if (!error)
		this.getInfo();
	this._isNeedPassword = (4 === error) ? true : false;
	return error;
};

CFile.prototype["getType"] = function()
{
	return this.type;
};

CFile.prototype["close"] = function()
{
	this._closeFile();
	this.nativeFile = 0;
	this.pages = [];
	this.info = null;
	this.StartID = null;
	for (let i = 0; i < this.stream.length; i++)
		this._free(this.stream[i]);
	this.stream = [];
	self.drawingFile = null;
};

CFile.prototype["getFileBinary"] = function()
{
	if (this.stream.length == 0)
		return "";
	return new Uint8Array(Module["HEAP8"].buffer, this.stream[0], this.stream_size);
};

CFile.prototype["isNeedPassword"] = function()
{
	return this._isNeedPassword;
};
CFile.prototype["SplitPages"] = function(arrPageIndex)
{
	let ptr = this._SplitPages(arrPageIndex);

	if (!ptr)
		return null;

	let lenArr = new Int32Array(Module["HEAP8"].buffer, ptr, 1);
	if (!lenArr)
	{
		Module["_free"](ptr);
		return null;
	}

	let len = lenArr[0];
	if (len <= 4)
	{
		Module["_free"](ptr);
		return null;
	}
	len -= 4;

	let buffer = new Uint8Array(len);
	buffer.set(new Uint8Array(Module["HEAP8"].buffer, ptr + 4, len));
	Module["_free"](ptr);
	return buffer;
};
CFile.prototype["MergePages"] = function(arrayBuffer, maxID, prefixForm)
{
	return this._MergePages(arrayBuffer, maxID, prefixForm);
};

// INFO DOCUMENT
CFile.prototype.getInfo = function()
{
	if (!this.nativeFile)
		return false;

	let ptr = this._getInfo();
	let reader = ptr.getReader();
	if (!reader) return false;

	this.StartID = reader.readInt();

	let _pages = reader.readInt();
	for (let i = 0; i < _pages; i++)
	{
		let rec = {};
		rec["W"] = reader.readInt();
		rec["H"] = reader.readInt();
		rec["Dpi"] = reader.readInt();
		rec["Rotate"] = reader.readInt();
		rec["originIndex"] = i;
		rec.fonts = [];
		rec.fontsUpdateType = UpdateFontsSource.Undefined;
		rec.text = null;
		this.pages.push(rec);
	}
	let json_info = reader.readString();
	try
	{
		this.info = JSON.parse(json_info);
	} catch(err) {}

	ptr.free();
	return this.pages.length > 0;
};
CFile.prototype.getPagesInfo = function()
{
	if (!this.nativeFile)
		return [];

	let ptr = this._getInfo();
	let reader = ptr.getReader();
	if (!reader) return [];

	// change StartID
	this.StartID = reader.readInt();

	let _pages = [];
	let nPages = reader.readInt();
	for (let i = 0; i < nPages; i++)
	{
		let rec = {};
		rec["W"] = reader.readInt();
		rec["H"] = reader.readInt();
		rec["Dpi"] = reader.readInt();
		rec["Rotate"] = reader.readInt();
		rec["originIndex"] = i;
		rec.fonts = [];
		rec.fontsUpdateType = UpdateFontsSource.Undefined;
		rec.text = null;
		_pages.push(rec);
	}
	// skip json_info

	ptr.free();
	return _pages;
};

CFile.prototype["getStructure"] = function()
{
	let ptr = this._getStructure();
	let reader = ptr.getReader();

	if (!reader) return [];

	let res = [];
	while (reader.isValid())
	{
		let rec = {};
		rec["page"]  = reader.readInt();
		rec["level"] = reader.readInt();
		rec["y"]  = reader.readDouble();
		rec["description"] = reader.readString();
		res.push(rec);
	}

	ptr.free();
	return res;
};

CFile.prototype["getLinks"] = function(pageIndex)
{
	let ptr = this._getLinks(pageIndex);
	let reader = ptr.getReader();

	if (!reader) return [];

	let res = [];
	while (reader.isValid())
	{
		let rec = {};
		rec["link"] = reader.readString();
		rec["dest"] = reader.readDouble();
		rec["x"] = reader.readDouble();
		rec["y"] = reader.readDouble();
		rec["w"] = reader.readDouble();
		rec["h"] = reader.readDouble();
		res.push(rec);
	}

	ptr.free();
	return res;
};

// TEXT
CFile.prototype["getGlyphs"] = function(pageIndex)
{
	let page = this.pages[pageIndex];
	if (page.originIndex == undefined)
		return [];
	if (page.fonts.length > 0)
	{
		// waiting fonts
		return null;
	}

	this.lockPageNumForFontsLoader(pageIndex, UpdateFontsSource.Page);
	let res = this._getGlyphs(page.originIndex);
	// there is no need to delete the result; this buffer is used as a text buffer 
	// for text commands on other pages. After receiving ALL text pages, 
	// you need to call destroyTextInfo()
	this.unlockPageNumForFontsLoader();

	if (page.fonts.length > 0)
	{
		// waiting fonts
		res = null;
		return null;
	}

	if (res && this.onUpdateStatistics)
		this.onUpdateStatistics(res.info[0], res.info[1], res.info[2], res.info[3]);

	return res.result || null;
};
CFile.prototype["destroyTextInfo"] = function()
{
	this._destroyTextInfo();
};

// FONTS
CFile.prototype.getWidgetFonts = function(type)
{
	let ptr = this._getInteractiveFormsFonts(type);
	let reader = ptr.getReader();
	if (!reader) return [];

	let res = [];
	while (reader.isValid())
	{
		let n = reader.readInt();
		for (let i = 0; i < n; ++i)
			res.push(reader.readString());
	}

	ptr.free();
	return res;
};

CFile.prototype["getInteractiveFormsEmbeddedFonts"] = function()
{
	return this.getWidgetFonts(1);
};

CFile.prototype["getInteractiveFormsStandardFonts"] = function()
{
	return this.getWidgetFonts(2);
};

CFile.prototype["getFontByID"] = function(ID)
{
	return this._getFontByID(ID);
};

CFile.prototype["setCMap"] = function(memoryBuffer)
{
	if (!this.nativeFile)
		return;

	this._setCMap(memoryBuffer);
};

CFile.prototype["isNeedCMap"] = function()
{
	return this._isNeedCMap();
};

// WIDGETS & ANNOTATIONS
function readAction(reader, rec, readDoubleFunc, readStringFunc)
{
	let SType = reader.readByte();
	// 0 - Unknown, 1 - GoTo, 2 - GoToR, 3 - GoToE, 4 - Launch
	// 5 - Thread, 6 - URI, 7 - Sound, 8 - Movie, 9 - Hide
	// 10 - Named, 11 - SubmitForm, 12 - ResetForm, 13 - ImportData
	// 14 - JavaScript, 15 - SetOCGState, 16 - Rendition
	// 17 - Trans, 18 - GoTo3DView
	rec["S"] = SType;
	if (SType == 14)
	{
		rec["JS"] = readStringFunc.call(reader);
	}
	else if (SType == 1)
	{
		rec["page"] = reader.readInt();
		rec["kind"] = reader.readByte();
		// 0 - XYZ
		// 1 - Fit
		// 2 - FitH
		// 3 - FitV
		// 4 - FitR
		// 5 - FitB
		// 6 - FitBH
		// 7 - FitBV
		switch (rec["kind"])
		{
			case 0:
			case 2:
			case 3:
			case 6:
			case 7:
			{
				let nFlag = reader.readByte();
				if (nFlag & (1 << 0))
					rec["left"] = readDoubleFunc.call(reader);
				if (nFlag & (1 << 1))
					rec["top"]  = readDoubleFunc.call(reader);
				if (nFlag & (1 << 2))
					rec["zoom"] = readDoubleFunc.call(reader);
				break;
			}
			case 4:
			{
				rec["left"]   = readDoubleFunc.call(reader);
				rec["bottom"] = readDoubleFunc.call(reader);
				rec["right"]  = readDoubleFunc.call(reader);
				rec["top"]    = readDoubleFunc.call(reader);
				break;
			}
			case 1:
			case 5:
			default:
				break;
		}
	}
	else if (SType == 10)
	{
		rec["N"] = readStringFunc.call(reader);
	}
	else if (SType == 6)
	{
		rec["URI"] = readStringFunc.call(reader);
	}
	else if (SType == 9)
	{
		rec["H"] = reader.readByte();
		let m = reader.readInt();
	    rec["T"] = [];
		// array of annotation names - rec["name"]
	    for (let j = 0; j < m; ++j)
			rec["T"].push(readStringFunc.call(reader));
	}
	else if (SType == 12)
	{
		rec["Flags"] = reader.readInt();
		let m = reader.readInt();
	    rec["Fields"] = [];
		// array of annotation names - rec["name"]
	    for (let j = 0; j < m; ++j)
			rec["Fields"].push(readStringFunc.call(reader));
	}
	let NextAction = reader.readByte();
	if (NextAction)
	{
		rec["Next"] = {};
		readAction(reader, rec["Next"], readDoubleFunc, readStringFunc);
	}
}
function readAnnot(reader, rec, readDoubleFunc, readDouble2Func, readStringFunc, isRead = false)
{
	rec["AP"] = {};
	// Annot
	// number for relations with AP
	rec["AP"]["i"] = reader.readInt();
	rec["annotflag"] = reader.readInt();
	// 12.5.3
	let bHidden = (rec["annotflag"] >> 1) & 1; // Hidden
	let bPrint = (rec["annotflag"] >> 2) & 1; // Print
	rec["noZoom"] = (rec["annotflag"] >> 3) & 1; // NoZoom
	rec["noRotate"] = (rec["annotflag"] >> 4) & 1; // NoRotate
	let bNoView = (rec["annotflag"] >> 5) & 1; // NoView
	rec["locked"] = (rec["annotflag"] >> 7) & 1; // Locked
	rec["ToggleNoView"] = (rec["annotflag"] >> 8) & 1; // ToggleNoView
	rec["lockedC"] = (rec["annotflag"] >> 9) & 1; // LockedContents
	// 0 - visible, 1 - hidden, 2 - noPrint, 3 - noView
	rec["display"] = 0;
	if (bHidden)
		rec["display"] = 1;
	else
	{
		if (bPrint)
		{
			if (bNoView)
				rec["display"] = 3;
			else
				rec["display"] = 0;
		}
		else
		{
			if (bNoView)
				rec["display"] = 0; // ??? no hidden, but noView and no print
			else
				rec["display"] = 2;
		}
	}
	rec["page"] = reader.readInt();
	// offsets like getStructure and viewer.navigate
	rec["rect"] = {};
	rec["rect"]["x1"] = readDouble2Func.call(reader);
	rec["rect"]["y1"] = readDouble2Func.call(reader);
	rec["rect"]["x2"] = readDouble2Func.call(reader);
	rec["rect"]["y2"] = readDouble2Func.call(reader);
	let flags = reader.readInt();
	// Unique name - NM
	if (flags & (1 << 0))
		rec["UniqueName"] = readStringFunc.call(reader);
	// Alternate annotation text - Contents
	if (flags & (1 << 1))
		rec["Contents"] = readStringFunc.call(reader);
	// Border effect - BE
	if (flags & (1 << 2))
	{
		rec["BE"] = {};
		rec["BE"]["S"] = reader.readByte();
		rec["BE"]["I"] = readDoubleFunc.call(reader);
	}
	// Special annotation color - С
	if (flags & (1 << 3))
	{
		let n = reader.readInt();
		rec["C"] = [];
		for (let i = 0; i < n; ++i)
			rec["C"].push(readDouble2Func.call(reader));
	}
	// Border/BS
	if (flags & (1 << 4))
	{
		// 0 - solid, 1 - beveled, 2 - dashed, 3 - inset, 4 - underline
		rec["border"] = reader.readByte();
		rec["borderWidth"] = readDoubleFunc.call(reader);
		// Border Dash Pattern
		if (rec["border"] == 2)
		{
			let n = reader.readInt();
			rec["dashed"] = [];
			for (let i = 0; i < n; ++i)
				rec["dashed"].push(readDoubleFunc.call(reader));
		}
	}
	// Date of last change - M
	if (flags & (1 << 5))
		rec["LastModified"] = readStringFunc.call(reader);
	// AP
	if (flags & (1 << 6))
	{
		if (isRead)
			rec["AP"]["render"] = reader.readData(); // TODO use Render - Uint8Array
		else
			rec["AP"]["have"] = (flags >> 6) & 1;
	}
	// User ID
	if (flags & (1 << 7))
		rec["OUserID"] = readStringFunc.call(reader);
	// User ID
	if (flags & (1 << 8))
		rec["AP"]["Copy"] = reader.readInt();
}
function readAnnotAP(reader, AP)
{
	// number for relations with AP
	AP["i"] = reader.readInt();
	AP["x"] = reader.readDouble();
	AP["y"] = reader.readDouble();
	AP["w"] = reader.readInt();
	AP["h"] = reader.readInt();
	let n = reader.readInt();
	for (let i = 0; i < n; ++i)
	{
		let APType = reader.readString();
		if (!AP[APType])
			AP[APType] = {};
		let APi = AP[APType];
		let ASType = reader.readString();
		if (ASType)
		{
			AP[APType][ASType] = {};
			APi = AP[APType][ASType];
		}
		let np1 = reader.readInt();
		let np2 = reader.readInt();
		// this memory needs to be deleted
		APi["retValue"] = np2 << 32 | np1;
		// 0 - Normal, 1 - Multiply, 2 - Screen, 3 - Overlay, 4 - Darken, 5 - Lighten, 6 - ColorDodge, 7 - ColorBurn, 8 - HardLight,
		// 9 - SoftLight, 10 - Difference, 11 - Exclusion, 12 - Hue, 13 - Saturation, 14 - Color, 15 - Luminosity
		APi["BlendMode"] = reader.readByte();
	}
}
function readAnnotType(reader, rec, readDoubleFunc, readDouble2Func, readStringFunc, isRead = false)
{
	// Markup
	let flags = 0;
	if ((rec["Type"] < 18 && rec["Type"] != 1 && rec["Type"] != 15) || rec["Type"] == 25)
	{
		flags = reader.readInt();
		if (flags & (1 << 0))
			rec["Popup"] = reader.readInt();
		// T
		if (flags & (1 << 1))
			rec["User"] = readStringFunc.call(reader);
		// CA
		if (flags & (1 << 2))
			rec["CA"] = readDoubleFunc.call(reader);
		// RC
		if (flags & (1 << 3))
		{
			let n = reader.readInt();
			rec["RC"] = [];
			for (let i = 0; i < n; ++i)
			{
				let oFont = {};
				// 0 - left, 1 - centered, 2 - right, 3 - justify
				oFont["alignment"] = reader.readByte();
				let nFontFlag = reader.readInt();
				oFont["bold"] = (nFontFlag >> 0) & 1;
				oFont["italic"] = (nFontFlag >> 1) & 1;
				oFont["strikethrough"] = (nFontFlag >> 3) & 1;
				oFont["underlined"] = (nFontFlag >> 4) & 1;
				if (nFontFlag & (1 << 5))
					oFont["vertical"] = readDoubleFunc.call(reader);
				if (nFontFlag & (1 << 6))
					oFont["actual"] = readStringFunc.call(reader);
				oFont["size"] = readDoubleFunc.call(reader);
				oFont["color"] = [];
				oFont["color"].push(readDouble2Func.call(reader));
				oFont["color"].push(readDouble2Func.call(reader));
				oFont["color"].push(readDouble2Func.call(reader));
				oFont["name"] = readStringFunc.call(reader);
				oFont["text"] = readStringFunc.call(reader);
				rec["RC"].push(oFont);
			}
		}
		// CreationDate
		if (flags & (1 << 4))
			rec["CreationDate"] = readStringFunc.call(reader);
		// IRT
		if (flags & (1 << 5))
			rec["RefTo"] = reader.readInt();
		// RT
		// 0 - R, 1 - Group
		if (flags & (1 << 6))
			rec["RefToReason"] = reader.readByte();
		// Subj
		if (flags & (1 << 7))
			rec["Subj"] = readStringFunc.call(reader);
	}
	// Text
	if (rec["Type"] == 0)
	{
		// Background color - C->IC
		if (rec["C"])
		{
			rec["IC"] = rec["C"];
			delete rec["C"];
		}
		rec["Open"] = (flags >> 15) & 1;
		// icon - Name
		// 0 - Check, 1 - Checkmark, 2 - Circle, 3 - Comment, 4 - Cross, 5 - CrossHairs, 6 - Help, 7 - Insert, 8 - Key, 9 - NewParagraph, 10 - Note, 11 - Paragraph, 12 - RightArrow, 13 - RightPointer, 14 - Star, 15 - UpArrow, 16 - UpLeftArrow
		if (flags & (1 << 16))
			rec["Icon"] = reader.readByte();
		// StateModel
		// 0 - Marked, 1 - Review
		if (flags & (1 << 17))
			rec["StateModel"] = reader.readByte();
		// State
		// 0 - Marked, 1 - Unmarked, 2 - Accepted, 3 - Rejected, 4 - Cancelled, 5 - Completed, 6 - None
		if (flags & (1 << 18))
			rec["State"] = reader.readByte();
		
	}
	// Line
	else if (rec["Type"] == 3)
	{
		// L
		rec["L"] = [];
		for (let i = 0; i < 4; ++i)
			rec["L"].push(readDoubleFunc.call(reader));
		// LE
		// 0 - Square, 1 - Circle, 2 - Diamond, 3 - OpenArrow, 4 - ClosedArrow, 5 - None, 6 - Butt, 7 - ROpenArrow, 8 - RClosedArrow, 9 - Slash
		if (flags & (1 << 15))
		{
			rec["LE"] = [];
			rec["LE"].push(reader.readByte());
			rec["LE"].push(reader.readByte());
		}
		// IC
		if (flags & (1 << 16))
		{
			let n = reader.readInt();
			rec["IC"] = [];
			for (let i = 0; i < n; ++i)
				rec["IC"].push(readDouble2Func.call(reader));
		}
		// LL
		if (flags & (1 << 17))
			rec["LL"] = readDoubleFunc.call(reader);
		// LLE
		if (flags & (1 << 18))
			rec["LLE"] = readDoubleFunc.call(reader);
		// Cap
		rec["Cap"] = (flags >> 19) & 1;
		// IT
		// 0 - LineDimension, 1 - LineArrow
		if (flags & (1 << 20))
			rec["IT"] = reader.readByte();
		// LLO
		if (flags & (1 << 21))
			rec["LLO"] = readDoubleFunc.call(reader);
		// CP
		// 0 - Inline, 1 - Top
		if (flags & (1 << 22))
			rec["CP"] = reader.readByte();
		// CO
		if (flags & (1 << 23))
		{
			rec["CO"] = [];
			rec["CO"].push(readDoubleFunc.call(reader));
			rec["CO"].push(readDoubleFunc.call(reader));
		}
	}
	// Ink
	else if (rec["Type"] == 14)
	{
		// offsets like getStructure and viewer.navigate
		let n = reader.readInt();
		rec["InkList"] = [];
		for (let i = 0; i < n; ++i)
		{
			rec["InkList"][i] = [];
			let m = reader.readInt();
			for (let j = 0; j < m; ++j)
				rec["InkList"][i].push(readDoubleFunc.call(reader));
		}
	}
	// Highlight, Underline, Squiggly, Strikeout
	else if (rec["Type"] > 7 && rec["Type"] < 12)
	{
		// QuadPoints
		let n = reader.readInt();
		rec["QuadPoints"] = [];
		for (let i = 0; i < n; ++i)
			rec["QuadPoints"].push(readDoubleFunc.call(reader));
	}
	// Square, Circle
	else if (rec["Type"] == 4 || rec["Type"] == 5)
	{
		// Rect and RD differences
		if (flags & (1 << 15))
		{
			rec["RD"] = [];
			for (let i = 0; i < 4; ++i)
				rec["RD"].push(readDoubleFunc.call(reader));
		}
		// IC
		if (flags & (1 << 16))
		{
			let n = reader.readInt();
			rec["IC"] = [];
			for (let i = 0; i < n; ++i)
				rec["IC"].push(readDouble2Func.call(reader));
		}
	}
	// Polygon, PolyLine
	else if (rec["Type"] == 6 || rec["Type"] == 7)
	{
		let nVertices = reader.readInt();
		rec["Vertices"] = [];
		for (let i = 0; i < nVertices; ++i)
			rec["Vertices"].push(readDoubleFunc.call(reader));
		// LE
		// 0 - Square, 1 - Circle, 2 - Diamond, 3 - OpenArrow, 4 - ClosedArrow, 5 - None, 6 - Butt, 7 - ROpenArrow, 8 - RClosedArrow, 9 - Slash
		if (flags & (1 << 15))
		{
			rec["LE"] = [];
			rec["LE"].push(reader.readByte());
			rec["LE"].push(reader.readByte());
		}
		// IC
		if (flags & (1 << 16))
		{
			let n = reader.readInt();
			rec["IC"] = [];
			for (let i = 0; i < n; ++i)
				rec["IC"].push(readDouble2Func.call(reader));
		}
		// IT
		// 0 - PolygonCloud, 1 - PolyLineDimension, 2 - PolygonDimension
		if (flags & (1 << 20))
			rec["IT"] = reader.readByte();
	}
	// Popup
	/*
	else if (rec["Type"] == 15)
	{
		flags = reader.readInt();
		rec["Open"] = (flags >> 0) & 1;
		// Link to parent-annotation
		if (flags & (1 << 1))
			rec["PopupParent"] = reader.readInt();
	}
	*/
	// FreeText
	else if (rec["Type"] == 2)
	{
		// Background color - C->IC
		if (!isRead && rec["C"])
		{
			rec["IC"] = rec["C"];
			delete rec["C"];
		}
		// 0 - left-justified, 1 - centered, 2 - right-justified
		rec["alignment"] = reader.readByte();
		rec["Rotate"] = reader.readInt();
		// Rect and RD differences
		if (flags & (1 << 15))
		{
			rec["RD"] = [];
			for (let i = 0; i < 4; ++i)
				rec["RD"].push(readDoubleFunc.call(reader));
		}
		// CL
		if (flags & (1 << 16))
		{
			let n = reader.readInt();
			rec["CL"] = [];
			for (let i = 0; i < n; ++i)
				rec["CL"].push(readDoubleFunc.call(reader));
		}
		// Default style (CSS2 format) - DS
		if (flags & (1 << 17))
			rec["defaultStyle"] = readStringFunc.call(reader);
		// LE
		// 0 - Square, 1 - Circle, 2 - Diamond, 3 - OpenArrow, 4 - ClosedArrow, 5 - None, 6 - Butt, 7 - ROpenArrow, 8 - RClosedArrow, 9 - Slash
		if (flags & (1 << 18))
			rec["LE"] = reader.readByte();
		// IT
		// 0 - FreeText, 1 - FreeTextCallout, 2 - FreeTextTypeWriter
		if (flags & (1 << 20))
			rec["IT"] = reader.readByte();
		// Border color - from DA (write to C)
		if (flags & (1 << 21))
		{
			let n = reader.readInt();
			if (isRead)
			{
				rec["IC"] = [];
				for (let i = 0; i < n; ++i)
					rec["IC"].push(readDouble2Func.call(reader));
			}
			else
			{
				rec["C"] = [];
				for (let i = 0; i < n; ++i)
					rec["C"].push(readDouble2Func.call(reader));
			}
		}
	}
	// Caret
	else if (rec["Type"] == 13)
	{
		// Rect and RD differenses
		if (flags & (1 << 15))
		{
			rec["RD"] = [];
			for (let i = 0; i < 4; ++i)
				rec["RD"].push(readDoubleFunc.call(reader));
		}
		// Sy
		// 0 - None, 1 - P, 2 - S
		if (flags & (1 << 16))
			rec["Sy"] = reader.readByte();
	}
	// FileAttachment
	else if (rec["Type"] == 16)
	{
		if (flags & (1 << 15))
			rec["Icon"] = readStringFunc.call(reader);
		if (flags & (1 << 16))
			rec["FS"] = readStringFunc.call(reader);
		if (flags & (1 << 17))
		{
			rec["F"] = {};
			rec["F"]["FileName"] = readStringFunc.call(reader);
		}
		if (flags & (1 << 18))
		{
			rec["UF"] = {};
			rec["UF"]["FileName"] = readStringFunc.call(reader);
		}
		if (flags & (1 << 19))
		{
			rec["DOS"] = {};
			rec["DOS"]["FileName"] = readStringFunc.call(reader);
		}
		if (flags & (1 << 20))
		{
			rec["Mac"] = {};
			rec["Mac"]["FileName"] = readStringFunc.call(reader);
		}
		if (flags & (1 << 21))
		{
			rec["Unix"] = {};
			rec["Unix"]["FileName"] = readStringFunc.call(reader);
		}
		if (flags & (1 << 22))
		{
			rec["ID"] = [];
			rec["ID"].push(readStringFunc.call(reader));
			rec["ID"].push(readStringFunc.call(reader));
		}
		rec["V"] = flags & (1 << 23);
		if (flags & (1 << 24))
		{
			if (isRead)
			{

			}
			else
			{
				let flag = reader.readInt();
				if (flag & (1 << 0))
				{
					let n = reader.readInt();
					let np1 = reader.readInt();
					let np2 = reader.readInt();
					let pPoint = np2 << 32 | np1;
					rec["F"]["File"] = new Uint8Array(Module["HEAP8"].buffer, pPoint, n);
					Module["_free"](pPoint);
				}
				if (flag & (1 << 1))
				{
					let n = reader.readInt();
					let np1 = reader.readInt();
					let np2 = reader.readInt();
					let pPoint = np2 << 32 | np1;
					rec["UF"]["File"] = new Uint8Array(Module["HEAP8"].buffer, pPoint, n);
					Module["_free"](pPoint);
				}
				if (flag & (1 << 2))
				{
					let n = reader.readInt();
					let np1 = reader.readInt();
					let np2 = reader.readInt();
					let pPoint = np2 << 32 | np1;
					rec["DOS"]["File"] = new Uint8Array(Module["HEAP8"].buffer, pPoint, n);
					Module["_free"](pPoint);
				}
				if (flag & (1 << 3))
				{
					let n = reader.readInt();
					let np1 = reader.readInt();
					let np2 = reader.readInt();
					let pPoint = np2 << 32 | np1;
					rec["Mac"]["File"] = new Uint8Array(Module["HEAP8"].buffer, pPoint, n);
					Module["_free"](pPoint);
				}
				if (flag & (1 << 4))
				{
					let n = reader.readInt();
					let np1 = reader.readInt();
					let np2 = reader.readInt();
					let pPoint = np2 << 32 | np1;
					rec["Unix"]["File"] = new Uint8Array(Module["HEAP8"].buffer, pPoint, n);
					Module["_free"](pPoint);
				}
			}
			
		}
		if (flags & (1 << 26))
			rec["Desc"] = readStringFunc.call(reader);
	}
	// Stamp
	else if (rec["Type"] == 12)
	{
		rec["Icon"] = readStringFunc.call(reader);
		rec["Rotate"] = readDouble2Func.call(reader);
		rec["InRect"] = [];
		for (let i = 0; i < 8; ++i)
			rec["InRect"].push(readDouble2Func.call(reader));
	}
}
function readWidgetType(reader, rec, readDoubleFunc, readDouble2Func, readStringFunc, isRead = false)
{
	// Widget
	rec["font"] = {};
	rec["font"]["name"] = readStringFunc.call(reader);
	rec["font"]["size"] = readDoubleFunc.call(reader);
	if (isRead)
		rec["font"]["sizeAP"] = readDoubleFunc.call(reader);
	rec["font"]["style"] = reader.readInt();
	let tc = reader.readInt();
	if (tc)
	{
		rec["font"]["color"] = [];
		for (let i = 0; i < tc; ++i)
			rec["font"]["color"].push(readDouble2Func.call(reader));
	}
	// 0 - left-justified, 1 - centered, 2 - right-justified
	if (!isRead || (rec["Type"] != 29 && rec["Type"] != 28 && rec["Type"] != 27))
		rec["alignment"] = reader.readByte();
	rec["flag"] = reader.readInt();
	// 12.7.3.1
	if (rec["flag"] >= 0)
	{
		rec["readOnly"] = (rec["flag"] >> 0) & 1; // ReadOnly
		rec["required"] = (rec["flag"] >> 1) & 1; // Required
		rec["noexport"] = (rec["flag"] >> 2) & 1; // NoExport
	}
	let flags = reader.readInt();
	// Alternative field name, used in tooltip and error messages - TU
	if (flags & (1 << 0))
		rec["userName"] = readStringFunc.call(reader);
	// Default style string (CSS2 format) - DS
	if (flags & (1 << 1))
		rec["defaultStyle"] = readStringFunc.call(reader);
	// Actual font
	if (flags & (1 << 2))
		rec["font"]["actual"] = readStringFunc.call(reader);
	// Selection mode - H
	// 0 - none, 1 - invert, 2 - push, 3 - outline
	if (flags & (1 << 3))
		rec["highlight"] = reader.readByte();
	// Font key
	if (flags & (1 << 4))
		rec["font"]["key"] = readStringFunc.call(reader);
	// Border color - BC. Even if the border is not specified by BS/Border, 
	// then if BC is present, a default border is provided (solid, thickness 1). 
	// If the text annotation has MaxLen, borders appear for each character
	if (flags & (1 << 5))
	{
		let n = reader.readInt();
		rec["BC"] = [];
		for (let i = 0; i < n; ++i)
			rec["BC"].push(readDouble2Func.call(reader));
	}
	// Rotate an annotation relative to the page - R
	if (flags & (1 << 6))
		rec["rotate"] = reader.readInt();
	// Annotation background color - BG
	if (flags & (1 << 7))
	{
		let n = reader.readInt();
		rec["BG"] = [];
		for (let i = 0; i < n; ++i)
			rec["BG"].push(readDouble2Func.call(reader));
	}
	// Default value - DV
	if (flags & (1 << 8))
		rec["defaultValue"] = readStringFunc.call(reader);
	if (flags & (1 << 17))
		rec["Parent"] = reader.readInt();
	if (flags & (1 << 18))
		rec["name"] = readStringFunc.call(reader);
	if (flags & (1 << 19))
		rec["font"]["AP"] = readStringFunc.call(reader);
	if (flags & (1 << 20))
		rec["meta"] = readStringFunc.call(reader);
	// Action
	let nAction = reader.readInt();
	if (nAction > 0)
		rec["AA"] = {};
	for (let i = 0; i < nAction; ++i)
	{
		let AAType = readStringFunc.call(reader);
		rec["AA"][AAType] = {};
		readAction(reader, rec["AA"][AAType], readDoubleFunc, readStringFunc);
	}
	// Widget types
	if (rec["type"] == 27)
	{
		if (flags & (1 << 9))
			rec["value"] = readStringFunc.call(reader);
		let IFflags = reader.readInt();
		// Header - СA
		if (flags & (1 << 10))
			rec["caption"] = readStringFunc.call(reader);
		// Rollover header - RC
		if (flags & (1 << 11))
			rec["rolloverCaption"] = readStringFunc.call(reader);
		// Alternate header - AC
		if (flags & (1 << 12))
			rec["alternateCaption"] = readStringFunc.call(reader);
		// Header position - TP
		if (flags & (1 << 13))
			// 0 - textOnly, 1 - iconOnly, 2 - iconTextV, 3 - textIconV, 4 - iconTextH, 5 - textIconH, 6 - overlay
			rec["position"] = reader.readByte();
		// Icons - IF
		if (IFflags & (1 << 0))
		{
			rec["IF"] = {};
			// Scaling IF.SW
			// 0 - Always, 1 - Never, 2 - too big, 3 - too small
			if (IFflags & (1 << 1))
				rec["IF"]["SW"] = reader.readByte();
			// Scaling type - IF.S
			// 0 - Proportional, 1 - Anamorphic
			if (IFflags & (1 << 2))
				rec["IF"]["S"] = reader.readByte();
			if (IFflags & (1 << 3))
			{
				rec["IF"]["A"] = [];
				rec["IF"]["A"].push(readDoubleFunc.call(reader));
				rec["IF"]["A"].push(readDoubleFunc.call(reader));
			}
			rec["IF"]["FB"] = (IFflags >> 4) & 1;
		}
		if (isRead)
		{
			if (IFflags & (1 << 5))
				rec["I"] = reader.readInt();
			if (IFflags & (1 << 6))
				rec["RI"] = reader.readInt();
			if (IFflags & (1 << 7))
				rec["IX"] = reader.readInt();
		}
	}
	else if (rec["type"] == 29 || rec["type"] == 28)
	{
		if (flags & (1 << 9))
			rec["value"] = readStringFunc.call(reader);
		// 0 - check, 1 - cross, 2 - diamond, 3 - circle, 4 - star, 5 - square
		rec["style"] = reader.readByte();
		if (flags & (1 << 14))
			rec["ExportValue"] = readStringFunc.call(reader);
		// 12.7.4.2.1
		if (rec["flag"] >= 0)
		{
			rec["NoToggleToOff"]  = (rec["flag"] >> 14) & 1; // NoToggleToOff
			rec["radiosInUnison"] = (rec["flag"] >> 25) & 1; // RadiosInUnison
		}
	}
	else if (rec["type"] == 30)
	{
		if (flags & (1 << 9))
			rec["value"] = readStringFunc.call(reader);
		if (flags & (1 << 10))
			rec["maxLen"] = reader.readInt();
		if (rec["flag"] & (1 << 25))
			rec["richValue"] = readStringFunc.call(reader);
		if (isRead)
		{
			if (flags & (1 << 12))
				rec["AP"]["V"] = readStringFunc.call(reader);
			if (flags & (1 << 13))
				rec["AP"]["render"] = reader.readData(); // TODO use Render - Uint8Array
		}
		// 12.7.4.3
		if (rec["flag"] >= 0)
		{
			rec["multiline"]       = (rec["flag"] >> 12) & 1; // Multiline
			rec["password"]        = (rec["flag"] >> 13) & 1; // Password
			rec["fileSelect"]      = (rec["flag"] >> 20) & 1; // FileSelect
			rec["doNotSpellCheck"] = (rec["flag"] >> 22) & 1; // DoNotSpellCheck
			rec["doNotScroll"]     = (rec["flag"] >> 23) & 1; // DoNotScroll
			rec["comb"]            = (rec["flag"] >> 24) & 1; // Comb
			rec["richText"]        = (rec["flag"] >> 25) & 1; // RichText
		}
	}
	else if (rec["type"] == 31 || rec["type"] == 32)
	{
		if (flags & (1 << 9))
			rec["value"] = readStringFunc.call(reader);
		if (flags & (1 << 10))
		{
			let n = reader.readInt();
			rec["opt"] = [];
			for (let i = 0; i < n; ++i)
			{
				let opt1 = readStringFunc.call(reader);
				let opt2 = readStringFunc.call(reader);
				if (opt1 == "")
					rec["opt"].push(opt2);
				else
					rec["opt"].push([opt2, opt1]);
			}
		}
		if (flags & (1 << 11))
			rec["TI"] = reader.readInt();
		if (isRead)
		{
			if (flags & (1 << 12))
				rec["AP"]["V"] = readStringFunc.call(reader);
		}
		else
		{
			if (flags & (1 << 12))
			{
				let n = reader.readInt();
				rec["curIdxs"] = [];
				for (let i = 0; i < n; ++i)
					rec["curIdxs"].push(reader.readInt());
			}
		}
		if (flags & (1 << 13))
		{
			let n = reader.readInt();
			rec["value"] = [];
			for (let i = 0; i < n; ++i)
				rec["value"].push(readStringFunc.call(reader));
		}
		if (isRead)
		{
			if (flags & (1 << 14))
			{
				let n = reader.readInt();
				rec["I"] = [];
				for (let i = 0; i < n; ++i)
					rec["I"].push(reader.readInt());
			}
			if (flags & (1 << 15))
				rec["AP"]["render"] = reader.readData(); // TODO use Render - Uint8Array
		}
		// 12.7.4.4
		if (rec["flag"] >= 0)
		{
			rec["editable"]          = (rec["flag"] >> 18) & 1; // Edit
			rec["multipleSelection"] = (rec["flag"] >> 21) & 1; // MultiSelect
			rec["doNotSpellCheck"]   = (rec["flag"] >> 22) & 1; // DoNotSpellCheck
			rec["commitOnSelChange"] = (rec["flag"] >> 26) & 1; // CommitOnSelChange
		}
		
	}
	else if (rec["type"] == 33)
	{
		rec["Sig"] = (flags >> 9) & 1;
	}
	if (rec["flag"] < 0)
		delete rec["flag"];
}

CFile.prototype["getInteractiveFormsInfo"] = function()
{
	let ptr = this._getInteractiveFormsInfo();
	let reader = ptr.getReader();
	if (!reader) return {};

	let res = {};
	while (reader.isValid())
	{
		let k = reader.readInt();
		if (k > 0 && res["CO"] == undefined)
			res["CO"] = [];
		for (let i = 0; i < k; ++i)
			res["CO"].push(reader.readInt());
		
		k = reader.readInt();
		if (k > 0 && res["Parents"] == undefined)
			res["Parents"] = [];
		for (let i = 0; i < k; ++i)
		{
			let rec = {};
			rec["i"] = reader.readInt();
			let flags = reader.readInt();
			if (flags & (1 << 0))
				rec["name"] = reader.readString();
			if (flags & (1 << 1))
				rec["value"] = reader.readString();
			if (flags & (1 << 2))
				rec["defaultValue"] = reader.readString();
			if (flags & (1 << 3))
			{
				let n = reader.readInt();
				rec["curIdxs"] = [];
				for (let i = 0; i < n; ++i)
					rec["curIdxs"].push(reader.readInt());
			}
			if (flags & (1 << 4))
				rec["Parent"] = reader.readInt();
			if (flags & (1 << 5))
			{
				let n = reader.readInt();
				rec["value"] = [];
				for (let i = 0; i < n; ++i)
					rec["value"].push(reader.readString());
			}
			if (flags & (1 << 6))
			{
				let n = reader.readInt();
				rec["opt"] = [];
				for (let i = 0; i < n; ++i)
				{
					let opt1 = reader.readString();
					let opt2 = reader.readString();
					if (opt1 == "")
						rec["opt"].push(opt2);
					else
						rec["opt"].push([opt2, opt1]);
				}
			}
			if (flags & (1 << 7))
			{
				rec["flag"] = reader.readInt();

				rec["readOnly"] = (rec["flag"] >> 0) & 1; // ReadOnly
				rec["required"] = (rec["flag"] >> 1) & 1; // Required
				rec["noexport"] = (rec["flag"] >> 2) & 1; // NoExport

				rec["NoToggleToOff"]  = (rec["flag"] >> 14) & 1; // NoToggleToOff
				if ((rec["flag"] >> 15) & 1) // If radiobutton
					rec["radiosInUnison"] = (rec["flag"] >> 25) & 1; // RadiosInUnison
				else
					rec["richText"]       = (rec["flag"] >> 25) & 1; // RichText

				rec["multiline"]       = (rec["flag"] >> 12) & 1; // Multiline
				rec["password"]        = (rec["flag"] >> 13) & 1; // Password
				rec["fileSelect"]      = (rec["flag"] >> 20) & 1; // FileSelect
				rec["doNotSpellCheck"] = (rec["flag"] >> 22) & 1; // DoNotSpellCheck
				rec["doNotScroll"]     = (rec["flag"] >> 23) & 1; // DoNotScroll
				rec["comb"]            = (rec["flag"] >> 24) & 1; // Comb

				rec["editable"]          = (rec["flag"] >> 18) & 1; // Edit
				rec["multipleSelection"] = (rec["flag"] >> 21) & 1; // MultiSelect
				rec["commitOnSelChange"] = (rec["flag"] >> 26) & 1; // CommitOnSelChange
			}
			if (flags & (1 << 8))
			{
				let nAction = reader.readInt();
				if (nAction > 0)
					rec["AA"] = {};
				for (let i = 0; i < nAction; ++i)
				{
					let AAType = reader.readString();
					rec["AA"][AAType] = {};
					readAction(reader, rec["AA"][AAType]);
				}
			}
			res["Parents"].push(rec);
		}

		k = reader.readInt();
		if (k > 0 && res["Fields"] == undefined)
			res["Fields"] = [];
		for (let q = 0; reader.isValid() && q < k; ++q)
		{
			let rec = {};
			// Widget type - FT
			// 26 - Unknown, 27 - button, 28 - radiobutton, 29 - checkbox, 30 - text, 31 - combobox, 32 - listbox, 33 - signature
			rec["type"] = reader.readByte();
			// Annot
			readAnnot(reader, rec, reader.readDouble, reader.readDouble2, reader.readString);
			// Widget type
			readWidgetType(reader, rec, reader.readDouble, reader.readDouble2, reader.readString);

			res["Fields"].push(rec);
		}
	}

	ptr.free();
	return res;
};	

// optional nWidget     - rec["AP"]["i"]
// optional sView       - N/D/R
// optional sButtonView - state pushbutton-annotation - Off/Yes(or rec["ExportValue"])
CFile.prototype["getInteractiveFormsAP"] = function(pageIndex, width, height, backgroundColor, nWidget, sView, sButtonView)
{
	let nView = -1;
	if (sView)
	{
		if (sView == "N")
			nView = 0;
		else if (sView == "D")
			nView = 1;
		else if (sView == "R")
			nView = 2;
	}
	let nButtonView = -1;
	if (sButtonView)
		nButtonView = (sButtonView == "Off" ? 0 : 1);

	this.lockPageNumForFontsLoader(pageIndex, UpdateFontsSource.Forms);
	let ptr = this._getInteractiveFormsAP(width, height, backgroundColor, pageIndex, nWidget, nView, nButtonView);
	let reader = ptr.getReader();
	this.unlockPageNumForFontsLoader();
	
	if (!reader)
		return [];

	let res = [];

	while (reader.isValid())
	{
		// Annotation view
		let AP = {};
		readAnnotAP(reader, AP);
		res.push(AP);
	}

	ptr.free();
	return res;
};
// optional bBase64   - true/false base64 result
// optional nWidget ...
// optional sIconView - icon - I/RI/IX
CFile.prototype["getButtonIcons"] = function(pageIndex, width, height, backgroundColor, bBase64, nWidget, sIconView)
{
	let nView = -1;
	if (sIconView)
	{
		if (sIconView == "I")
			nView = 0;
		else if (sIconView == "RI")
			nView = 1;
		else if (sIconView == "IX")
			nView = 2;
	}

	let ptr = this._getButtonIcons(backgroundColor, pageIndex, bBase64, nWidget, nView);
	let reader = ptr.getReader();

	if (!reader) return {};

	let res = {};
	
	res["MK"] = [];
	res["View"] = [];

	while (reader.isValid())
	{
		// View pushbutton annotation
		let MK = {};
		// Relation with AP
		MK["i"] = reader.readInt();
		let n = reader.readInt();
		for (let i = 0; i < n; ++i)
		{
			let MKType = reader.readString();
			MK[MKType] = reader.readInt();
			let unique = reader.readByte();
			if (unique)
			{
				let ViewMK = {};
				ViewMK["j"] = MK[MKType];
				ViewMK["w"] = reader.readInt();
				ViewMK["h"] = reader.readInt();
				if (bBase64)
				{
					// base64 string with image
					ViewMK["retValue"] = reader.readString();
				}
				else
				{
					let np1 = reader.readInt();
					let np2 = reader.readInt();
					// this memory needs to be deleted
					ViewMK["retValue"] = np2 << 32 | np1;
				}
				res["View"].push(ViewMK);
			}
		}
		res["MK"].push(MK);
	}

	ptr.free();
	return res;
};
// optional pageIndex - get annotations from specific page
CFile.prototype["getAnnotationsInfo"] = function(pageIndex)
{
	if (!this.nativeFile)
		return [];

	let ptr = this._getAnnotationsInfo(pageIndex);
	let reader = ptr.getReader();

	if (!reader) return [];

	let res = [];
	while (reader.isValid())
	{
		let n = reader.readInt();
		for (let i = 0; i < n; ++i)
		{
			let rec = {};
			// Annotation type
			// 0 - Text, 1 - Link, 2 - FreeText, 3 - Line, 4 - Square, 5 - Circle,
			// 6 - Polygon, 7 - PolyLine, 8 - Highlight, 9 - Underline, 10 - Squiggly, 
			// 11 - Strikeout, 12 - Stamp, 13 - Caret, 14 - Ink, 15 - Popup, 16 - FileAttachment, 
			// 17 - Sound, 18 - Movie, 19 - Widget, 20 - Screen, 21 - PrinterMark,
			// 22 - TrapNet, 23 - Watermark, 24 - 3D, 25 - Redact
			rec["Type"] = reader.readByte();
			// Annot
			readAnnot(reader, rec, reader.readDouble, reader.readDouble2, reader.readString);
			// Annot type
			readAnnotType(reader, rec, reader.readDouble, reader.readDouble2, reader.readString);
			res.push(rec);
		}
	}
	
	ptr.free();
	return res;
};
// optional nAnnot ...
// optional sView ...
CFile.prototype["getAnnotationsAP"] = function(pageIndex, width, height, backgroundColor, nAnnot, sView)
{
	let nView = -1;
	if (sView)
	{
		if (sView == "N")
			nView = 0;
		else if (sView == "D")
			nView = 1;
		else if (sView == "R")
			nView = 2;
	}

	this.lockPageNumForFontsLoader(pageIndex, UpdateFontsSource.Annotation);
	let ptr = this._getAnnotationsAP(width, height, backgroundColor, pageIndex, nAnnot, nView);
	let reader = ptr.getReader();
	this.unlockPageNumForFontsLoader();

	if (!reader)
		return [];

	let res = [];
	while (reader.isValid())
	{
		// Annotation view
		let AP = {};
		readAnnotAP(reader, AP);
		res.push(AP);
	}

	ptr.free();
	return res;
};

// AnnotInfo - Uint8Array with ctAnnotField format
CFile.prototype["readAnnotationsInfoFromBinary"] = function(AnnotInfo)
{
	if (!AnnotInfo)
		return [];

	let reader = new CBinaryReader(AnnotInfo, 0, AnnotInfo.length);
	if (!reader) return [];

	let res = [];
	while (reader.isValid())
	{
		let nCommand = reader.readByte();
		let nPos = reader.pos;
		let nSize = reader.readInt();
		if (nCommand != 164) // ctAnnotField
		{
			reader.pos = nPos + nSize;
			continue;
		}
		let rec = {};
		// Annotation type
		// 0 - Text, 1 - Link, 2 - FreeText, 3 - Line, 4 - Square, 5 - Circle,
		// 6 - Polygon, 7 - PolyLine, 8 - Highlight, 9 - Underline, 10 - Squiggly, 
		// 11 - Strikeout, 12 - Stamp, 13 - Caret, 14 - Ink, 15 - Popup, 16 - FileAttachment, 
		// 17 - Sound, 18 - Movie, 19 - Widget, 20 - Screen, 21 - PrinterMark,
		// 22 - TrapNet, 23 - Watermark, 24 - 3D, 25 - Redact
		rec["Type"] = reader.readByte();
		// Annot
		readAnnot(reader, rec, reader.readDouble3, reader.readDouble3, reader.readString2, true);
		// Annot type
		readAnnotType(reader, rec, reader.readDouble3, reader.readDouble3, reader.readString2, true);
		if (rec["Type"] >= 26 && rec["Type"] <= 33)
		{
			// Widget type
			readWidgetType(reader, rec, reader.readDouble3, reader.readDouble3, reader.readString2, true);
		}
		res.push(rec);
	}

	return res;
};

// SCAN PAGES
CFile.prototype["scanPage"] = function(page, mode)
{
	let ptr = this._scanPage(page, mode);
	let reader = ptr.getReader();

	if (!reader) return [];

	let shapesCount = reader.readInt();
	let shapes = new Array(shapesCount);

	for (let i = 0; i < shapesCount; i++)
		shapes[i] = reader.readString();

	ptr.free();
	return shapes;
};

CFile.prototype["getImageBase64"] = function(rId)
{
	let strId = "" + rId;
	if (this.scannedImages[strId])
		return this.scannedImages[strId];

	this.scannedImages[strId] = this._getImageBase64(rId);
	return this.scannedImages[strId];
};

CFile.prototype["changeImageUrl"] = function(baseUrl, resultUrl)
{
	for (let i in this.scannedImages)
	{
		if (this.scannedImages[i] == baseUrl)
			this.scannedImages[i] = resultUrl;
	}
};

// MEMORY
CFile.prototype["getUint8Array"] = function(ptr, len)
{
	return this._getUint8Array(ptr, len);
};
CFile.prototype["getUint8ClampedArray"] = function(ptr, len)
{
	return this._getUint8ClampedArray(ptr, len);
};
CFile.prototype["free"] = function(pointer)
{
	this._free(pointer);
};

// PIXMAP
CFile.prototype["getPagePixmap"] = function(pageIndex, width, height, backgroundColor)
{
	let page = this.pages[pageIndex];
	if (page.originIndex == undefined)
		return null;
	if (page.fonts.length > 0)
	{
		// waiting fonts
		return null;
	}

	this.lockPageNumForFontsLoader(pageIndex, UpdateFontsSource.Page);
	let ptr = this._getPixmap(page.originIndex, width, height, backgroundColor);
	this.unlockPageNumForFontsLoader();

	if (page.fonts.length > 0)
	{
		// waiting fonts
		this._free(ptr);
		ptr = null;
	}
	return ptr;
};

// CLOUD FONTS
function addToArrayAsDictionary(arr, value)
{
	let isFound = false;
	for (let i = 0, len = arr.length; i < len; i++)
	{
		if (arr[i] == value)
		{
			isFound = true;
			break;
		}
	}
	if (!isFound)
		arr.push(value);
	return isFound;
}

function fontToMemory(file, isCheck)
{
	let idBuffer = file.GetID().toUtf8();
	let idPointer = Module["_malloc"](idBuffer.length);
	Module["HEAP8"].set(idBuffer, idPointer);

	if (isCheck)
	{
		let nExist = Module["_IsFontBinaryExist"](idPointer);
		if (nExist != 0)
		{
			Module["_free"](idPointer);
			return;
		}
	}

	let stream_index = file.GetStreamIndex();
	
	let stream = AscFonts.getFontStream(stream_index);
	let streamPointer = Module["_malloc"](stream.size);
	Module["HEAP8"].set(stream.data, streamPointer);

	Module["_SetFontBinary"](idPointer, streamPointer, stream.size);

	Module["_free"](streamPointer);
	Module["_free"](idPointer);
}

// FONTS
CFile.prototype["addPage"] = function(pageIndex, pageObj)
{
	this.pages.splice(pageIndex, 0, pageObj);
	if (this.fontStreams)
	{
		for (let i in this.fontStreams)
		{
			let pages = this.fontStreams[i].pages;
			for (let j = 0; j < pages.length; j++)
			{
				if (pages[j] >= pageIndex)
					pages[j] += 1;
			}
		}
	}
};
CFile.prototype["removePage"] = function(pageIndex)
{
	let result = this.pages.splice(pageIndex, 1);
	if (this.fontStreams)
	{
		for (let i in this.fontStreams)
		{
			let pages = this.fontStreams[i].pages;
			for (let j = 0; j < pages.length; j++)
			{
				if (pages[j] > pageIndex)
					pages[j] -= 1;
				else if (pages[j] == pageIndex)
					pages.splice(j, 1);
			}
		}
	}
	return result;
};

// ONLY WEB
self["AscViewer"]["Free"] = function(pointer)
{
	CFile.prototype._free(pointer);
};
self["AscViewer"]["InitializeFonts"] = function(basePath) 
{
	return CFile.prototype._InitializeFonts(basePath);
};

self["AscViewer"]["CheckStreamId"] = function(data, status) 
{
	return CFile.prototype._CheckStreamId(data, status);
};

// export
self["AscViewer"]["CDrawingFile"] = CFile;

self.drawingFile = null;