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

(function(window, undefined) {

	function getMemoryPathIE(name)
	{
		if (self["AscViewer"] && self["AscViewer"]["baseUrl"])
			return self["AscViewer"]["baseUrl"] + name;
		return name;
	}

	var baseFontsPath = "../../../../fonts/";

	var FS = undefined;

	//desktop_fetch

	//polyfill

	//string_utf8

	//module

	self.drawingFile = null;

	function CBinaryReader(data, start, size)
	{
		this.data = data;
		this.pos = start;
		this.limit = start + size;
	}
	CBinaryReader.prototype.readByte = function()
	{
		let val = this.data[this.pos];
		this.pos += 1;
		return val;
	};
	CBinaryReader.prototype.readInt = function()
	{
		let val = this.data[this.pos] | this.data[this.pos + 1] << 8 | this.data[this.pos + 2] << 16 | this.data[this.pos + 3] << 24;
		this.pos += 4;
		return val;
	};
	CBinaryReader.prototype.readDouble = function()
	{
		return this.readInt() / 100;
	};
	CBinaryReader.prototype.readDouble2 = function()
	{
		return this.readInt() / 10000;
	};
	CBinaryReader.prototype.readString = function()
	{
		let len = this.readInt();
		let val = String.prototype.fromUtf8(this.data, this.pos, len);
		this.pos += len;
		return val;
	};
	CBinaryReader.prototype.readData = function()
	{
		let len = this.readInt();
		let val = this.data.slice(this.pos, this.pos + len);
		this.pos += len;
		return val;
	};
	CBinaryReader.prototype.isValid = function()
	{
		return (this.pos < this.limit) ? true : false;
	};
	CBinaryReader.prototype.Skip = function(nPos)
	{
		this.pos += nPos;
	};

	function CBinaryWriter()
	{
		this.size = 100000;
		this.dataSize = 0;
		this.buffer = new Uint8Array(this.size);
	}
	CBinaryWriter.prototype.checkAlloc = function(addition)
	{
		if ((this.dataSize + addition) <= this.size)
			return;

		let newSize = Math.max(this.size * 2, this.size + addition);
		let newBuffer = new Uint8Array(newSize);
		newBuffer.set(this.buffer, 0);

		this.size = newSize;
		this.buffer = newBuffer;
	};
	CBinaryWriter.prototype.writeUint = function(value)
	{
		this.checkAlloc(4);
		let val = (value>2147483647)?value-4294967296:value;
		this.buffer[this.dataSize++] = (val) & 0xFF;
		this.buffer[this.dataSize++] = (val >>> 8) & 0xFF;
		this.buffer[this.dataSize++] = (val >>> 16) & 0xFF;
		this.buffer[this.dataSize++] = (val >>> 24) & 0xFF;
	};
	CBinaryWriter.prototype.writeString = function(value)
	{
		let valueUtf8 = value.toUtf8();
		this.checkAlloc(valueUtf8.length);
		this.buffer.set(valueUtf8, this.dataSize);
		this.dataSize += valueUtf8.length;
	};

	var UpdateFontsSource = {
		Undefined   : 0,
		Page        : 1,
		Annotation  : 2,
		Forms       : 4
	};

	function CFile()
	{
		this.nativeFile = 0;
		this.stream = -1;
		this.stream_size = 0;
		this.type = -1;
		this.pages = [];
		this.info = null;
		this._isNeedPassword = false;

		// for async fonts loader
		this.fontPageIndex = -1;
		this.fontPageUpdateType = UpdateFontsSource.Undefined;
		this.fontStreams = {};
	}

	CFile.prototype["loadFromData"] = function(arrayBuffer)
	{
		let data = new Uint8Array(arrayBuffer);
		let _stream = Module["_malloc"](data.length);
		Module["HEAP8"].set(data, _stream);
		this.nativeFile = Module["_Open"](_stream, data.length, 0);
		let error = Module["_GetErrorCode"](this.nativeFile);
		this.stream = _stream;
		this.stream_size = data.length;
		this.type = Module["_GetType"](_stream, data.length);
		self.drawingFile = this;
		if (!error)
			this.getInfo();
		this._isNeedPassword = (4 === error) ? true : false;

		// 0 - ok
		// 4 - password
		// else - error
		return error;
	};
	CFile.prototype["loadFromDataWithPassword"] = function(password)
	{
		if (0 != this.nativeFile)
			Module["_Close"](this.nativeFile);

		let passBuffer = password.toUtf8();
		let passPointer = Module["_malloc"](passBuffer.length);
		Module["HEAP8"].set(passBuffer, passPointer);
		this.nativeFile = Module["_Open"](this.stream, this.stream_size, passPointer);
		Module["_free"](passPointer);
		let error = Module["_GetErrorCode"](this.nativeFile);
		this.type = Module["_GetType"](this.stream, this.stream_size);
		self.drawingFile = this;
		if (!error)
			this.getInfo();
		this._isNeedPassword = (4 === error) ? true : false;

		// 0 - ok
		// 4 - password
		// else - error
		return error;
	};
	CFile.prototype["getFileAsBase64"] = function()
	{
		if (0 >= this.stream)
			return "";

		return new Uint8Array(Module["HEAP8"].buffer, this.stream, this.stream_size);
	};
	CFile.prototype["isNeedPassword"] = function()
	{
		return this._isNeedPassword;
	};
	CFile.prototype["isNeedCMap"] = function()
	{
		if (!this.nativeFile)
			return false;

		let isNeed = Module["_IsNeedCMap"](this.nativeFile);
		return (isNeed === 1) ? true : false;
	};
	CFile.prototype["setCMap"] = function(memoryBuffer)
	{
		if (!this.nativeFile)
			return;

		let pointer = Module["_malloc"](memoryBuffer.length);
		Module.HEAP8.set(memoryBuffer, pointer);
		Module["_SetCMapData"](this.nativeFile, pointer, memoryBuffer.length);
	};
	CFile.prototype["getInfo"] = function()
	{
		if (!this.nativeFile)
			return false;

		let _info = Module["_GetInfo"](this.nativeFile);
		if (_info == 0)
			return false;

		let lenArray = new Int32Array(Module["HEAP8"].buffer, _info, 4);
		if (lenArray == null)
			return false;

		let len = lenArray[0];
		len -= 4;
		if (len <= 0)
			return false;

		let buffer = new Uint8Array(Module["HEAP8"].buffer, _info + 4, len);
		let reader = new CBinaryReader(buffer, 0, len);

		this.StartID = reader.readInt();

		let _pages = reader.readInt();
		for (let i = 0; i < _pages; i++)
		{
			let rec = {};
			rec["W"] = reader.readInt();
			rec["H"] = reader.readInt();
			rec["Dpi"] = reader.readInt();
			rec["Rotate"] = reader.readInt();
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

		Module["_free"](_info);
		return this.pages.length > 0;
	};
	CFile.prototype["close"] = function()
	{
		Module["_Close"](this.nativeFile);
		this.nativeFile = 0;
		this.pages = [];
		this.info = null;
		this.StartID = null;
		if (this.stream > 0)
			Module["_free"](this.stream);
		this.stream = -1;
		self.drawingFile = null;
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

	CFile.prototype["getPagePixmap"] = function(pageIndex, width, height, backgroundColor)
	{
		if (this.pages[pageIndex].fonts.length > 0)
		{
			// waiting fonts
			return null;
		}

		this.lockPageNumForFontsLoader(pageIndex, UpdateFontsSource.Page);
		let retValue = Module["_GetPixmap"](this.nativeFile, pageIndex, width, height, backgroundColor === undefined ? 0xFFFFFF : backgroundColor);
		this.unlockPageNumForFontsLoader();

		if (this.pages[pageIndex].fonts.length > 0)
		{
			// waiting fonts
			Module["_free"](retValue);
			retValue = null;
		}
		return retValue;
	};
	CFile.prototype["getGlyphs"] = function(pageIndex)
	{
		if (this.pages[pageIndex].fonts.length > 0)
		{
			// waiting fonts
			return null;
		}

		this.lockPageNumForFontsLoader(pageIndex, UpdateFontsSource.Page);
		let retValue = Module["_GetGlyphs"](this.nativeFile, pageIndex);
		// there is no need to delete the result; this buffer is used as a text buffer 
		// for text commands on other pages. After receiving ALL text pages, 
		// you need to call destroyTextInfo()
		this.unlockPageNumForFontsLoader();

		if (this.pages[pageIndex].fonts.length > 0)
		{
			// waiting fonts
			retValue = null;
		}

		if (null == retValue)
			return null;

		let lenArray = new Int32Array(Module["HEAP8"].buffer, retValue, 5);
		let len = lenArray[0];
		len -= 20;

		if (this.onUpdateStatistics)
			this.onUpdateStatistics(lenArray[1], lenArray[2], lenArray[3], lenArray[4]);

		if (len <= 0)
		{
			return [];
		}

		let textCommandsSrc = new Uint8Array(Module["HEAP8"].buffer, retValue + 20, len);
		let textCommands = new Uint8Array(len);
		textCommands.set(textCommandsSrc);

		textCommandsSrc = null;
		return textCommands;
	};
	CFile.prototype["destroyTextInfo"] = function()
	{
		Module["_DestroyTextInfo"]();
	};
	CFile.prototype["getLinks"] = function(pageIndex)
	{
		let res = [];
		let ext = Module["_GetLinks"](this.nativeFile, pageIndex);
		if (ext == 0)
			return res;

		let lenArray = new Int32Array(Module["HEAP8"].buffer, ext, 4);
		if (lenArray == null)
			return res;

		let len = lenArray[0];
		len -= 4;
		if (len <= 0)
			return res;

		let buffer = new Uint8Array(Module["HEAP8"].buffer, ext + 4, len);
		let reader = new CBinaryReader(buffer, 0, len);

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

		Module["_free"](ext);
		return res;
	};

	function readAction(reader, rec)
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
			rec["JS"] = reader.readString();
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
						rec["left"] = reader.readDouble();
					if (nFlag & (1 << 1))
						rec["top"]  = reader.readDouble();
					if (nFlag & (1 << 2))
						rec["zoom"] = reader.readDouble();
					break;
				}
				case 4:
				{
					rec["left"]   = reader.readDouble();
					rec["bottom"] = reader.readDouble();
					rec["right"]  = reader.readDouble();
					rec["top"]    = reader.readDouble();
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
			rec["N"] = reader.readString();
		}
		else if (SType == 6)
		{
			rec["URI"] = reader.readString();
		}
		else if (SType == 9)
		{
			rec["H"] = reader.readByte();
			let m = reader.readInt();
		    rec["T"] = [];
			// array of annotation names - rec["name"]
		    for (let j = 0; j < m; ++j)
				rec["T"].push(reader.readString());
		}
		else if (SType == 12)
		{
			rec["Flags"] = reader.readInt();
			let m = reader.readInt();
		    rec["Fields"] = [];
			// array of annotation names - rec["name"]
		    for (let j = 0; j < m; ++j)
				rec["Fields"].push(reader.readString());
		}
		let NextAction = reader.readByte();
		if (NextAction)
		{
			rec["Next"] = {};
			readAction(reader, rec["Next"]);
		}
	}
	function readAnnot(reader, rec)
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
		rec["rect"]["x1"] = reader.readDouble2();
		rec["rect"]["y1"] = reader.readDouble2();
		rec["rect"]["x2"] = reader.readDouble2();
		rec["rect"]["y2"] = reader.readDouble2();
		let flags = reader.readInt();
		// Unique name - NM
		if (flags & (1 << 0))
			rec["UniqueName"] = reader.readString();
		// Alternate annotation text - Contents
		if (flags & (1 << 1))
			rec["Contents"] = reader.readString();
		// Border effect - BE
		if (flags & (1 << 2))
		{
			rec["BE"] = {};
			rec["BE"]["S"] = reader.readByte();
			rec["BE"]["I"] = reader.readDouble();
		}
		// Special annotation color - С
		if (flags & (1 << 3))
		{
			let n = reader.readInt();
			rec["C"] = [];
			for (let i = 0; i < n; ++i)
				rec["C"].push(reader.readDouble2());
		}
		// Border/BS
		if (flags & (1 << 4))
		{
			// 0 - solid, 1 - beveled, 2 - dashed, 3 - inset, 4 - underline
			rec["border"] = reader.readByte();
			rec["borderWidth"] = reader.readDouble();
			// Border Dash Pattern
			if (rec["border"] == 2)
			{
				let n = reader.readInt();
				rec["dashed"] = [];
				for (let i = 0; i < n; ++i)
					rec["dashed"].push(reader.readDouble());
			}
		}
		// Date of last change - M
		if (flags & (1 << 5))
			rec["LastModified"] = reader.readString();
		rec["AP"]["have"] = (flags >> 6) & 1;
	}
	function readAnnotAP(reader, AP)
	{
		// number for relations with AP
		AP["i"] = reader.readInt();
		AP["x"] = reader.readInt();
		AP["y"] = reader.readInt();
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
	function getWidgetFonts(nativeFile, type)
	{
		let res = [];
		let ext = Module["_GetInteractiveFormsFonts"](nativeFile, type);
		if (ext == 0)
			return res;

		let lenArray = new Int32Array(Module["HEAP8"].buffer, ext, 4);
		if (lenArray == null)
		{
			Module["_free"](ext);
			return res;
		}

		let len = lenArray[0];
		len -= 4;
		if (len <= 0)
		{
			Module["_free"](ext);
			return res;
		}

		let buffer = new Uint8Array(Module["HEAP8"].buffer, ext + 4, len);
		let reader = new CBinaryReader(buffer, 0, len);
		
		while (reader.isValid())
		{
			let n = reader.readInt();
			for (let i = 0; i < n; ++i)
				res.push(reader.readString());
		}
		
		Module["_free"](ext);
		return res;
	}

	CFile.prototype["getInteractiveFormsInfo"] = function()
	{
		let res = {};
		let ext = Module["_GetInteractiveFormsInfo"](this.nativeFile);
		if (ext == 0)
			return res;

		let lenArray = new Int32Array(Module["HEAP8"].buffer, ext, 4);
		if (lenArray == null)
		{
			Module["_free"](ext);
			return res;
		}

		let len = lenArray[0];
		len -= 4;
		if (len <= 0)
		{
			Module["_free"](ext);
			return res;
		}

		let buffer = new Uint8Array(Module["HEAP8"].buffer, ext + 4, len);
		let reader = new CBinaryReader(buffer, 0, len);

		if (!reader.isValid())
		{
			Module["_free"](ext);
			return res;
		}

		let k = reader.readInt();
		if (k > 0)
			res["CO"] = [];
		for (let i = 0; i < k; ++i)
			res["CO"].push(reader.readInt());
		
		k = reader.readInt();
		if (k > 0)
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
				rec["Opt"] = [];
				for (let i = 0; i < n; ++i)
					rec["Opt"].push(reader.readString());
			}
			res["Parents"].push(rec);
		}

		res["Fields"] = [];
		k = reader.readInt();
		for (let q = 0; reader.isValid() && q < k; ++q)
		{
			let rec = {};
			// Widget type - FT
			// 26 - Unknown, 27 - button, 28 - radiobutton, 29 - checkbox, 30 - text, 31 - combobox, 32 - listbox, 33 - signature
			rec["type"] = reader.readByte();
			// Annot
			readAnnot(reader, rec);
			// Widget
			rec["font"] = {};
			rec["font"]["name"] = reader.readString();
			rec["font"]["size"] = reader.readDouble();
			rec["font"]["style"] = reader.readInt();
			let tc = reader.readInt();
			if (tc)
			{
				rec["font"]["color"] = [];
				for (let i = 0; i < tc; ++i)
					rec["font"]["color"].push(reader.readDouble2());
			}
			// 0 - left-justified, 1 - centered, 2 - right-justified
			rec["alignment"] = reader.readByte();
			rec["flag"] = reader.readInt();
			// 12.7.3.1
			rec["readOnly"] = (rec["flag"] >> 0) & 1; // ReadOnly
			rec["required"] = (rec["flag"] >> 1) & 1; // Required
			rec["noexport"] = (rec["flag"] >> 2) & 1; // NoExport
			let flags = reader.readInt();
			// Alternative field name, used in tooltip and error messages - TU
			if (flags & (1 << 0))
				rec["userName"] = reader.readString();
			// Default style string (CSS2 format) - DS
			if (flags & (1 << 1))
				rec["defaultStyle"] = reader.readString();
			// Actual font
			if (flags & (1 << 2))
				rec["font"]["actual"] = reader.readString();
			// Selection mode - H
			// 0 - none, 1 - invert, 2 - push, 3 - outline
			if (flags & (1 << 3))
				rec["highlight"] = reader.readByte();
			// Font key
			if (flags & (1 << 4))
				rec["font"]["key"] = reader.readString();
			// Border color - BC. Even if the border is not specified by BS/Border, 
			// then if BC is present, a default border is provided (solid, thickness 1). 
			// If the text annotation has MaxLen, borders appear for each character
			if (flags & (1 << 5))
			{
				let n = reader.readInt();
				rec["BC"] = [];
				for (let i = 0; i < n; ++i)
					rec["BC"].push(reader.readDouble2());
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
					rec["BG"].push(reader.readDouble2());
			}
			// Default value - DV
			if (flags & (1 << 8))
				rec["defaultValue"] = reader.readString();
			if (flags & (1 << 17))
				rec["Parent"] = reader.readInt();
			if (flags & (1 << 18))
				rec["name"] = reader.readString();
			if (flags & (1 << 19))
				rec["font"]["AP"] = reader.readString();
			// Action
			let nAction = reader.readInt();
			if (nAction > 0)
				rec["AA"] = {};
			for (let i = 0; i < nAction; ++i)
			{
				let AAType = reader.readString();
				rec["AA"][AAType] = {};
				readAction(reader, rec["AA"][AAType]);
			}
			// Widget types
			if (rec["type"] == 27)
			{
				if (flags & (1 << 9))
					rec["value"] = reader.readString();
				let IFflags = reader.readInt();
				// Header - СA
				if (flags & (1 << 10))
					rec["caption"] = reader.readString();
				// Rollover header - RC
				if (flags & (1 << 11))
					rec["rolloverCaption"] = reader.readString();
				// Alternate header - AC
				if (flags & (1 << 12))
					rec["alternateCaption"] = reader.readString();
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
						rec["IF"]["A"].push(reader.readDouble());
						rec["IF"]["A"].push(reader.readDouble());
					}
					rec["IF"]["FB"] = (IFflags >> 4) & 1;
				}
			}
			else if (rec["type"] == 29 || rec["type"] == 28)
			{
				if (flags & (1 << 9))
					rec["value"] = reader.readString();
				// 0 - check, 1 - cross, 2 - diamond, 3 - circle, 4 - star, 5 - square
				rec["style"] = reader.readByte();
			    if (flags & (1 << 14))
					rec["ExportValue"] = reader.readString();
				// 12.7.4.2.1
				rec["NoToggleToOff"]  = (rec["flag"] >> 14) & 1; // NoToggleToOff
				rec["radiosInUnison"] = (rec["flag"] >> 25) & 1; // RadiosInUnison
			}
			else if (rec["type"] == 30)
			{
				if (flags & (1 << 9))
					rec["value"] = reader.readString();
				if (flags & (1 << 10))
					rec["maxLen"] = reader.readInt();
				if (rec["flag"] & (1 << 25))
					rec["richValue"] = reader.readString();
				// 12.7.4.3
				rec["multiline"]       = (rec["flag"] >> 12) & 1; // Multiline
				rec["password"]        = (rec["flag"] >> 13) & 1; // Password
				rec["fileSelect"]      = (rec["flag"] >> 20) & 1; // FileSelect
				rec["doNotSpellCheck"] = (rec["flag"] >> 22) & 1; // DoNotSpellCheck
				rec["doNotScroll"]     = (rec["flag"] >> 23) & 1; // DoNotScroll
				rec["comb"]            = (rec["flag"] >> 24) & 1; // Comb
				rec["richText"]        = (rec["flag"] >> 25) & 1; // RichText
			}
			else if (rec["type"] == 31 || rec["type"] == 32)
			{
				if (flags & (1 << 9))
					rec["value"] = reader.readString();
				if (flags & (1 << 10))
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
				if (flags & (1 << 11))
					rec["TI"] = reader.readInt();
				if (flags & (1 << 12))
				{
					let n = reader.readInt();
					rec["curIdxs"] = [];
					for (let i = 0; i < n; ++i)
						rec["curIdxs"].push(reader.readInt());
				}
				if (flags & (1 << 13))
				{
					let n = reader.readInt();
					rec["value"] = [];
					for (let i = 0; i < n; ++i)
						rec["value"].push(reader.readString());
				}
				// 12.7.4.4
				rec["editable"]          = (rec["flag"] >> 18) & 1; // Edit
				rec["multipleSelection"] = (rec["flag"] >> 21) & 1; // MultiSelect
				rec["doNotSpellCheck"]   = (rec["flag"] >> 22) & 1; // DoNotSpellCheck
				rec["commitOnSelChange"] = (rec["flag"] >> 26) & 1; // CommitOnSelChange
			}
			else if (rec["type"] == 33)
			{
				rec["Sig"] = (flags >> 9) & 1;
			}
			
			res["Fields"].push(rec);
		}

		Module["_free"](ext);
		return res;
	};
	CFile.prototype["getInteractiveFormsEmbeddedFonts"] = function()
	{
		return getWidgetFonts(this.nativeFile, 1);
	};
	CFile.prototype["getInteractiveFormsStandardFonts"] = function()
	{
		return getWidgetFonts(this.nativeFile, 2);
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

		let res = [];
		this.lockPageNumForFontsLoader(pageIndex, UpdateFontsSource.Forms);
		let ext = Module["_GetInteractiveFormsAP"](this.nativeFile, width, height, backgroundColor === undefined ? 0xFFFFFF : backgroundColor, pageIndex, nWidget === undefined ? -1 : nWidget, nView, nButtonView);
		this.unlockPageNumForFontsLoader();
		if (ext == 0)
			return res;

		let lenArray = new Int32Array(Module["HEAP8"].buffer, ext, 4);
		if (lenArray == null)
			return res;

		let len = lenArray[0];
		len -= 4;
		if (len <= 0)
			return res;

		let buffer = new Uint8Array(Module["HEAP8"].buffer, ext + 4, len);
		let reader = new CBinaryReader(buffer, 0, len);

		while (reader.isValid())
		{
			// Annotation view
			let AP = {};
			readAnnotAP(reader, AP);
			res.push(AP);
		}

		Module["_free"](ext);
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

		let res = {};
		let ext = Module["_GetButtonIcons"](this.nativeFile, backgroundColor === undefined ? 0xFFFFFF : backgroundColor, pageIndex, bBase64 ? 1 : 0, nWidget === undefined ? -1 : nWidget, nView);
		if (ext == 0)
			return res;

		let lenArray = new Int32Array(Module["HEAP8"].buffer, ext, 4);
		if (lenArray == null)
			return res;

		let len = lenArray[0];
		len -= 4;
		if (len <= 0)
			return res;

		let buffer = new Uint8Array(Module["HEAP8"].buffer, ext + 4, len);
		let reader = new CBinaryReader(buffer, 0, len);
		
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

		Module["_free"](ext);
		return res;
	};
	// optional pageIndex - get annotations from specific page
	CFile.prototype["getAnnotationsInfo"] = function(pageIndex)
	{
		let res = [];
		let ext = Module["_GetAnnotationsInfo"](this.nativeFile, pageIndex === undefined ? -1 : pageIndex);
		if (ext == 0)
			return res;

		let lenArray = new Int32Array(Module["HEAP8"].buffer, ext, 4);
		if (lenArray == null)
		{
			Module["_free"](ext);
			return res;
		}

		let len = lenArray[0];
		len -= 4;
		if (len <= 0)
		{
			Module["_free"](ext);
			return res;
		}

		let buffer = new Uint8Array(Module["HEAP8"].buffer, ext + 4, len);
		let reader = new CBinaryReader(buffer, 0, len);

		if (!reader.isValid())
		{
			Module["_free"](ext);
			return res;
		}
		
		while (reader.isValid())
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
			readAnnot(reader, rec);
			// Markup
			let flags = 0;
			if ((rec["Type"] < 18 && rec["Type"] != 1 && rec["Type"] != 15) || rec["Type"] == 25)
			{
				flags = reader.readInt();
				if (flags & (1 << 0))
					rec["Popup"] = reader.readInt();
				// T
				if (flags & (1 << 1))
					rec["User"] = reader.readString();
				// CA
				if (flags & (1 << 2))
					rec["CA"] = reader.readDouble();
				// RC
				if (flags & (1 << 3))
					rec["RC"] = reader.readString();
				// CreationDate
				if (flags & (1 << 4))
					rec["CreationDate"] = reader.readString();
				// IRT
				if (flags & (1 << 5))
					rec["RefTo"] = reader.readInt();
				// RT
				// 0 - R, 1 - Group
				if (flags & (1 << 6))
					rec["RefToReason"] = reader.readByte();
				// Subj
				if (flags & (1 << 7))
					rec["Subj"] = reader.readString();
			}
			// Text
			if (rec["Type"] == 0)
			{
				// Bachground color - C->IC
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
					rec["L"].push(reader.readDouble());
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
						rec["IC"].push(reader.readDouble2());
				}
				// LL
				if (flags & (1 << 17))
					rec["LL"] = reader.readDouble();
				// LLE
				if (flags & (1 << 18))
					rec["LLE"] = reader.readDouble();
				// Cap
				rec["Cap"] = (flags >> 19) & 1;
				// IT
				// 0 - LineDimension, 1 - LineArrow
				if (flags & (1 << 20))
					rec["IT"] = reader.readByte();
				// LLO
				if (flags & (1 << 21))
					rec["LLO"] = reader.readDouble();
				// CP
				// 0 - Inline, 1 - Top
				if (flags & (1 << 22))
					rec["CP"] = reader.readByte();
				// CO
				if (flags & (1 << 23))
				{
					rec["CO"] = [];
					rec["CO"].push(reader.readDouble());
					rec["CO"].push(reader.readDouble());
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
						rec["InkList"][i].push(reader.readDouble());
				}
			}
			// Highlight, Underline, Squiggly, Strikeout
			else if (rec["Type"] > 7 && rec["Type"] < 12)
			{
				// QuadPoints
				let n = reader.readInt();
				rec["QuadPoints"] = [];
				for (let i = 0; i < n; ++i)
					rec["QuadPoints"].push(reader.readDouble());
			}
			// Square, Circle
			else if (rec["Type"] == 4 || rec["Type"] == 5)
			{
				// Rect and RD differences
				if (flags & (1 << 15))
				{
					rec["RD"] = [];
					for (let i = 0; i < 4; ++i)
						rec["RD"].push(reader.readDouble());
				}
				// IC
				if (flags & (1 << 16))
				{
					let n = reader.readInt();
					rec["IC"] = [];
					for (let i = 0; i < n; ++i)
						rec["IC"].push(reader.readDouble2());
				}
			}
			// Polygon, PolyLine
			else if (rec["Type"] == 6 || rec["Type"] == 7)
			{
				let nVertices = reader.readInt();
				rec["Vertices"] = [];
				for (let i = 0; i < nVertices; ++i)
					rec["Vertices"].push(reader.readDouble());
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
						rec["IC"].push(reader.readDouble2());
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
				// Bachground color - C->IC
				if (rec["C"])
				{
					rec["IC"] = rec["C"];
					delete rec["C"];
				}
				// 0 - left-justified, 1 - centered, 2 - right-justified
				rec["alignment"] = reader.readByte();
				// Rect and RD differences
				if (flags & (1 << 15))
				{
					rec["RD"] = [];
					for (let i = 0; i < 4; ++i)
						rec["RD"].push(reader.readDouble());
				}
				// CL
				if (flags & (1 << 16))
				{
					let n = reader.readInt();
					rec["CL"] = [];
					for (let i = 0; i < n; ++i)
						rec["CL"].push(reader.readDouble());
				}
				// Default style (CSS2 format) - DS
				if (flags & (1 << 17))
					rec["defaultStyle"] = reader.readString();
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
					rec["C"] = [];
					for (let i = 0; i < n; ++i)
						rec["C"].push(reader.readDouble2());
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
						rec["RD"].push(reader.readDouble());
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
					rec["Icon"] = reader.readString();
				if (flags & (1 << 16))
					rec["FS"] = reader.readString();
				if (flags & (1 << 17))
				{
					rec["F"] = {};
					rec["F"]["FileName"] = reader.readString();
				}
				if (flags & (1 << 18))
				{
					rec["UF"] = {};
					rec["UF"]["FileName"] = reader.readString();
				}
				if (flags & (1 << 19))
				{
					rec["DOS"] = {};
					rec["DOS"]["FileName"] = reader.readString();
				}
				if (flags & (1 << 20))
				{
					rec["Mac"] = {};
					rec["Mac"]["FileName"] = reader.readString();
				}
				if (flags & (1 << 21))
				{
					rec["Unix"] = {};
					rec["Unix"]["FileName"] = reader.readString();
				}
				if (flags & (1 << 22))
				{
					rec["ID"] = [];
					rec["ID"].push(reader.readString());
					rec["ID"].push(reader.readString());
				}
				rec["V"] = flags & (1 << 23);
				if (flags & (1 << 24))
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
				if (flags & (1 << 26))
					rec["Desc"] = reader.readString();
			}
			res.push(rec);
		}
		
		Module["_free"](ext);
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

		let res = [];
		this.lockPageNumForFontsLoader(pageIndex, UpdateFontsSource.Annotation);
		let ext = Module["_GetAnnotationsAP"](this.nativeFile, width, height, backgroundColor === undefined ? 0xFFFFFF : backgroundColor, pageIndex, nAnnot === undefined ? -1 : nAnnot, nView);
		this.unlockPageNumForFontsLoader();
		if (ext == 0)
			return res;

		let lenArray = new Int32Array(Module["HEAP8"].buffer, ext, 4);
		if (lenArray == null)
			return res;

		let len = lenArray[0];
		len -= 4;
		if (len <= 0)
			return res;

		let buffer = new Uint8Array(Module["HEAP8"].buffer, ext + 4, len);
		let reader = new CBinaryReader(buffer, 0, len);
		
		while (reader.isValid())
		{
			// Annotation view
			let AP = {};
			readAnnotAP(reader, AP);
			res.push(AP);
		}

		Module["_free"](ext);
		return res;
	};
	CFile.prototype["getStructure"] = function()
	{
		let res = [];
		let str = Module["_GetStructure"](this.nativeFile);
		if (str == 0)
			return res;
		let lenArray = new Int32Array(Module["HEAP8"].buffer, str, 4);
		if (lenArray == null)
			return res;
		let len = lenArray[0];
		len -= 4;
		if (len <= 0)
			return res;

		let buffer = new Uint8Array(Module["HEAP8"].buffer, str + 4, len);
		let reader = new CBinaryReader(buffer, 0, len);

		while (reader.isValid())
		{
			let rec = {};
			rec["page"]  = reader.readInt();
			rec["level"] = reader.readInt();
			rec["y"]  = reader.readDouble();
			rec["description"] = reader.readString();
			res.push(rec);
		}

		Module["_free"](str);
		return res;
	};
	CFile.prototype["getFontByID"] = function(ID)
	{
		let res = null;
		if (ID === undefined)
			return res;
		
		let idBuffer = ID.toUtf8();
		let idPointer = Module["_malloc"](idBuffer.length);
		Module["HEAP8"].set(idBuffer, idPointer);
		let ext = Module["_GetFontBinary"](this.nativeFile, idPointer);
		Module["_free"](idPointer);
		if (ext == 0)
			return res;
		
		let lenArray = new Int32Array(Module["HEAP8"].buffer, ext, 4);
		if (lenArray == null)
		{
			Module["_free"](ext);
			return res;
		}

		let len = lenArray[0];
		len -= 4;
		if (len <= 0)
		{
			Module["_free"](ext);
			return res;
		}

		let buffer = new Uint8Array(Module["HEAP8"].buffer, ext + 4, len);
		let reader = new CBinaryReader(buffer, 0, len);
		
		while (reader.isValid())
		{
			let nFontLength = reader.readInt();
			let np1 = reader.readInt();
			let np2 = reader.readInt();
			let pFontPoint = np2 << 32 | np1;
			
			res = new Uint8Array(Module["HEAP8"].buffer, pFontPoint, nFontLength);
		}

		Module["_free"](ext);
		
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
	
	self["AscViewer"]["CDrawingFile"] = CFile;
	self["AscViewer"]["InitializeFonts"] = function(basePath) {
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
	self["AscViewer"]["Free"] = function(pointer) {
		Module["_free"](pointer);
	};
	
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

	self["AscViewer"]["CheckStreamId"] = function(data, status) {
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
})(window, undefined);
