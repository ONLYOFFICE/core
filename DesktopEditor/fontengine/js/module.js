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

// BASE FILE FOR GENERATION

function onLoadFontsModule(window, undefined)
{
	var AscFonts = window['AscFonts'];

	AscFonts.TT_INTERPRETER_VERSION_35 = 35;
	AscFonts.TT_INTERPRETER_VERSION_38 = 38;
	AscFonts.TT_INTERPRETER_VERSION_40 = 40;

	AscFonts.CopyStreamToMemory = AscFonts["CopyStreamToMemory"];
	AscFonts.GetUint8ArrayFromPointer = AscFonts["GetUint8ArrayFromPointer"];

	AscFonts.AllocString2 = AscFonts["AllocString"];
	AscFonts.AllocString = function(size)
	{
		var pointer = AscFonts.AllocString2(size);
		pointer.pointer = pointer["pointer"];
		pointer.getBuffer = pointer["getBuffer"];
		pointer.free = pointer["free"];
		pointer.set = pointer["set"];
		return pointer;
	};

	AscFonts.FT_CreateLibrary = AscFonts["FT_CreateLibrary"];
	AscFonts.FT_Done_Library = AscFonts["FT_Done_Library"];
	AscFonts.FT_Set_TrueType_HintProp = AscFonts["FT_Set_TrueType_HintProp"];

	AscFonts.FT_Open_Face2 = AscFonts["FT_Open_Face"];
	AscFonts.FT_Done_Face = AscFonts["FT_Done_Face"];
	AscFonts.FT_SetCMapForCharCode = AscFonts["FT_SetCMapForCharCode"];
	AscFonts.FT_GetKerningX = AscFonts["FT_GetKerningX"];
	AscFonts.FT_GetFaceMaxAdvanceX = AscFonts["FT_GetFaceMaxAdvanceX"];
	AscFonts.FT_Set_Transform = AscFonts["FT_Set_Transform"];
	AscFonts.FT_Set_Char_Size = AscFonts["FT_Set_Char_Size"];
	AscFonts.FT_GetFaceInfo = AscFonts["FT_GetFaceInfo"];

	AscFonts.FT_Load_Glyph = AscFonts["FT_Load_Glyph"];
	AscFonts.FT_SetCMapForCharCode = AscFonts["FT_SetCMapForCharCode"];
	AscFonts.FT_Get_Glyph_Measure_Params = AscFonts["FT_Get_Glyph_Measure_Params"];
	AscFonts.FT_Get_Glyph_Render_Params = AscFonts["FT_Get_Glyph_Render_Params"];
	AscFonts.FT_Get_Glyph_Render_Buffer2 = AscFonts["FT_Get_Glyph_Render_Buffer"];

	AscFonts.HB_FontFree = AscFonts["HB_FontFree"];
	AscFonts.HB_ShapeText = AscFonts["HB_ShapeText"];

	AscFonts["Hyphen_Init"]();
	AscFonts.Hyphen_Destroy = AscFonts["Hyphen_Destroy"];
	AscFonts.Hyphen_LoadDictionary = AscFonts["Hyphen_LoadDictionary"];
	AscFonts.Hyphen_CheckDictionary = AscFonts["Hyphen_CheckDictionary"];
	AscFonts.Hyphen_Word = AscFonts["Hyphen_Word"];

	AscFonts.CreateNativeStreamByIndex = function(stream_index)
	{
		let stream = AscFonts.g_fonts_streams[stream_index];
		if (stream && true !== stream.asc_marker)
		{
			AscFonts.g_fonts_streams[stream_index] = { 
				asc_marker: true, 
				data: AscFonts.CopyStreamToMemory(stream.data, stream.size), 
				len: stream.size
			};
		}
	};

	function CBinaryReader(data, start, size)
	{
		this.data = data;
		this.pos = (undefined === start) ? 0 : start;
		this.limit = this.pos + ((undefined === size) ? data.length : size);
	}
	CBinaryReader.prototype.init = CBinaryReader.prototype["init"] = function(data, start, size)
	{
		this.data = data;
		this.pos = (undefined === start) ? 0 : start;
		this.limit = this.pos + ((undefined === size) ? data.length : size);
	}
	CBinaryReader.prototype.readInt = CBinaryReader.prototype["readInt"] = function()
	{
		var val = (this.data[this.pos] & 0xFF) | (this.data[this.pos + 1] & 0xFF) << 8 | (this.data[this.pos + 2] & 0xFF) << 16 | (this.data[this.pos + 3] & 0xFF) << 24;
		this.pos += 4;
		return val;
	};
	CBinaryReader.prototype.readUInt = CBinaryReader.prototype["readUInt"] = function()
	{
		var val = (this.data[this.pos] & 0xFF) | (this.data[this.pos + 1] & 0xFF) << 8 | (this.data[this.pos + 2] & 0xFF) << 16 | (this.data[this.pos + 3] & 0xFF) << 24;
		this.pos += 4;
		return (val < 0) ? val + 4294967296 : val;
	};
	CBinaryReader.prototype.readByte = CBinaryReader.prototype["readByte"] = function()
	{
		return (this.data[this.pos++] & 0xFF);
	};
	CBinaryReader.prototype.readPointer64 = CBinaryReader.prototype["readPointer64"] = function()
	{
		let i1 = this.readUInt();
		let i2 = this.readUInt();
		if (i2 === 0)
			return i1;
		return i2 * 4294967296 + i1;
	};
	CBinaryReader.prototype.isValid = CBinaryReader.prototype["isValid"] = function()
	{
		return (this.pos < this.limit) ? true : false;
	};
	const READER = new CBinaryReader(null, 0, 0);

	function CFaceInfo()
	{
		this.units_per_EM = 0;
		this.ascender = 0;
		this.descender = 0;
		this.height = 0;
		this.face_flags = 0;
		this.num_faces = 0;
		this.num_glyphs = 0;
		this.num_charmaps = 0;
		this.style_flags = 0;
		this.face_index = 0;

		this.family_name = "";

		this.style_name = "";

		this.os2_version = 0;
		this.os2_usWeightClass = 0;
		this.os2_fsSelection = 0;
		this.os2_usWinAscent = 0;
		this.os2_usWinDescent = 0;
		this.os2_usDefaultChar = 0;
		this.os2_sTypoAscender = 0;
		this.os2_sTypoDescender = 0;
		this.os2_sTypoLineGap = 0;

		this.os2_ulUnicodeRange1 = 0;
		this.os2_ulUnicodeRange2 = 0;
		this.os2_ulUnicodeRange3 = 0;
		this.os2_ulUnicodeRange4 = 0;
		this.os2_ulCodePageRange1 = 0;
		this.os2_ulCodePageRange2 = 0;

		this.os2_nSymbolic = 0;

		this.header_yMin = 0;
		this.header_yMax = 0;

		this.monochromeSizes = [];
	}

	CFaceInfo.prototype.load = function(face)
	{
		let errorObj = AscFonts.FT_GetFaceInfo(face, READER);
		if (errorObj["error"])
			return;

		this.units_per_EM 	= READER.readUInt();
		this.ascender 		= READER.readInt();
		this.descender 		= READER.readInt();
		this.height 		= READER.readInt();
		this.face_flags 	= READER.readInt();
		this.num_faces 		= READER.readInt();
		this.num_glyphs 	= READER.readInt();
		this.num_charmaps 	= READER.readInt();
		this.style_flags 	= READER.readInt();
		this.face_index 	= READER.readInt();

		var c = READER.readInt();
		while (c)
		{
			this.family_name += (c < 0x10000) ? String.fromCharCode(c) : (String.fromCharCode(0xD800 | (c >> 10)) + String.fromCharCode(0xDC00 | (c & 0x3FF)));
			c = READER.readInt();
		}

		c = READER.readInt();
		while (c)
		{
			this.style_name += String.fromCharCode(c);
			c = READER.readInt();
		}

		this.os2_version 		= READER.readInt();
		this.os2_usWeightClass 	= READER.readInt();
		this.os2_fsSelection 	= READER.readInt();
		this.os2_usWinAscent 	= READER.readInt();
		this.os2_usWinDescent 	= READER.readInt();
		this.os2_usDefaultChar 	= READER.readInt();
		this.os2_sTypoAscender 	= READER.readInt();
		this.os2_sTypoDescender = READER.readInt();
		this.os2_sTypoLineGap 	= READER.readInt();

		this.os2_ulUnicodeRange1 	= READER.readUInt();
		this.os2_ulUnicodeRange2 	= READER.readUInt();
		this.os2_ulUnicodeRange3 	= READER.readUInt();
		this.os2_ulUnicodeRange4 	= READER.readUInt();
		this.os2_ulCodePageRange1 	= READER.readUInt();
		this.os2_ulCodePageRange2 	= READER.readUInt();

		this.os2_nSymbolic 			= READER.readInt();
		this.header_yMin 			= READER.readInt();
		this.header_yMax 			= READER.readInt();

		var fixedSizesCount = READER.readInt();
		for (var i = 0; i < fixedSizesCount; i++)
			this.monochromeSizes.push(READER.readInt());

		errorObj["free"]();
	};

	function CGlyphMetrics()
	{
		this.bbox_xMin = 0;
		this.bbox_yMin = 0;
		this.bbox_xMax = 0;
		this.bbox_yMax = 0;

		this.width          = 0;
		this.height         = 0;

		this.horiAdvance    = 0;
		this.horiBearingX   = 0;
		this.horiBearingY   = 0;

		this.vertAdvance    = 0;
		this.vertBearingX   = 0;
		this.vertBearingY   = 0;

		this.linearHoriAdvance = 0;
		this.linearVertAdvance = 0;
	}

	function CGlyphBitmapImage()
	{
		this.left   = 0;
		this.top    = 0;
		this.width  = 0;
		this.rows   = 0;
		this.pitch  = 0;
		this.mode   = 0;
	}

	AscFonts.CFaceInfo = CFaceInfo;
	AscFonts.CGlyphMetrics = CGlyphMetrics;
	AscFonts.CGlyphBitmapImage = CGlyphBitmapImage;

	AscFonts.FT_Open_Face = function(library, stream, face_index)
	{
		return AscFonts.FT_Open_Face2(library, stream.data, stream.len, face_index);
	};

	AscFonts.FT_Glyph_Get_Measure = function(face, vector_worker, painter)
	{
		let errorObj = AscFonts.FT_Get_Glyph_Measure_Params(face, vector_worker ? 1 : 0, READER);
		if (errorObj["error"])
			return null;

		let len = errorObj.count;

		var info = new CGlyphMetrics();
		info.bbox_xMin     = READER.readInt();
		info.bbox_yMin     = READER.readInt();
		info.bbox_xMax     = READER.readInt();
		info.bbox_yMax     = READER.readInt();

		info.width         = READER.readInt();
		info.height        = READER.readInt();

		info.horiAdvance   = READER.readInt();
		info.horiBearingX  = READER.readInt();
		info.horiBearingY  = READER.readInt();

		info.vertAdvance   = READER.readInt();
		info.vertBearingX  = READER.readInt();
		info.vertBearingY  = READER.readInt();

		info.linearHoriAdvance     = READER.readInt();
		info.linearVertAdvance     = READER.readInt();

		if (vector_worker)
		{
			painter.start(vector_worker);

			let pos = 15;
			while (pos < len)
			{
				pos++;
				switch (READER.readInt())
				{
					case 0:
					{
						painter._move_to(READER.readInt(), READER.readInt(), vector_worker);
						pos += 2;
						break;
					}
					case 1:
					{
						painter._line_to(READER.readInt(), READER.readInt(), vector_worker);
						pos += 2;
						break;
					}
					case 2:
					{
						painter._conic_to(READER.readInt(), READER.readInt(), READER.readInt(), READER.readInt(), vector_worker);
						pos += 4;
						break;
					}
					case 3:
					{
						painter._cubic_to(READER.readInt(), READER.readInt(), READER.readInt(), READER.readInt(), READER.readInt(), READER.readInt(), vector_worker);
						pos += 6;
						break;
					}
					default:
						break;
				}
			}

			painter.end(vector_worker);
		}

		errorObj["free"]();
		return info;
	};

	AscFonts.FT_Glyph_Get_Raster = function(face, render_mode)
	{
		let errorObj = AscFonts.FT_Get_Glyph_Render_Params(face, render_mode, READER);
		if (errorObj["error"])
			return null;

		var info = new CGlyphBitmapImage();
		info.left    = READER.readInt();
		info.top     = READER.readInt();
		info.width   = READER.readInt();
		info.rows    = READER.readInt();
		info.pitch   = READER.readInt();
		info.mode    = READER.readInt();

		errorObj["free"]();
		return info;
	};

	AscFonts.FT_Get_Glyph_Render_Buffer = function(face, rasterInfo, isCopyToRasterMemory)
	{
		let buffer = AscFonts.FT_Get_Glyph_Render_Buffer2(face, rasterInfo.pitch * rasterInfo.rows);

		if (!isCopyToRasterMemory)
			return buffer;

		AscFonts.raster_memory.CheckSize(rasterInfo.width, rasterInfo.rows);

		let offsetSrc = 0;
		let offsetDst = 3;
		let dstData = AscFonts.raster_memory.m_oBuffer.data;

		if (rasterInfo.pitch >= rasterInfo.width)
		{
			for (let j = 0; j < rasterInfo.rows; ++j, offsetSrc += rasterInfo.pitch)
			{
				offsetDst = 3 + j * AscFonts.raster_memory.pitch;
				for (let i = 0; i < rasterInfo.width; i++, offsetDst += 4)
				{
					dstData[offsetDst] = buffer[offsetSrc + i];
				}
			}
		}
		else
		{
			var bitNumber = 0;
			var byteNumber = 0;
			for (let j = 0; j < rasterInfo.rows; ++j, offsetSrc += rasterInfo.pitch)
			{
				offsetDst = 3 + j * AscFonts.raster_memory.pitch;
				bitNumber = 0;
				byteNumber = 0;
				for (let i = 0; i < rasterInfo.width; i++, offsetDst += 4, bitNumber++)
				{
					if (8 === bitNumber)
					{
						bitNumber = 0;
						byteNumber++;
					}
					dstData[offsetDst] = (buffer[offsetSrc + byteNumber] & (1 << (7 - bitNumber))) ? 255 : 0;
				}
			}
		}
	};

	const FONTSIZE       = 576;
	const STRING_MAX_LEN = 1024;
	const COEF           = 25.4 / 72 / 64 / FONTSIZE;
	let   STRING_POINTER = null;
	let   STRING_LEN     = 0;
	const CLUSTER        = new Uint8Array(STRING_MAX_LEN);
	let   CLUSTER_LEN    = 0;
	let   CLUSTER_MAX    = 0;
	const LIGATURE       = 2;
	const CODEPOINTS     = new Uint32Array(STRING_MAX_LEN);

	AscFonts.MEASURE_FONTSIZE        = FONTSIZE;
	AscFonts.GRAPHEME_STRING_MAX_LEN = STRING_MAX_LEN;
	AscFonts.GRAPHEME_COEF           = COEF;
	AscFonts.HB_STRING_MAX_LEN       = STRING_MAX_LEN;

	function CCodePointsCalculator(codePointsBuffer, clusterBuffer)
	{
		this.currentCluster   = 0;
		this.currentCodePoint = 0;
		this.clusterBuffer    = clusterBuffer;
		this.codePoints       = codePointsBuffer;
		this.codePointsCount  =  0
	}
	CCodePointsCalculator.prototype.start = function(startCluster)
	{
		this.currentCluster   = startCluster;
		this.currentCodePoint = 0;
		this.codePointsCount  = 0;
	}
	CCodePointsCalculator.prototype.calculate = function(cluster)
	{
		this.currentCodePoint += this.codePointsCount;

		let nCodePointsCount = 0;
		
		if (cluster < this.currentCluster)
		{
			while (this.currentCluster > cluster)
			{
				this.currentCluster -= this.clusterBuffer[this.currentCodePoint + nCodePointsCount];
				++nCodePointsCount;
			}
		}
		else
		{
			while (this.currentCluster < cluster)
			{
				this.currentCluster += this.clusterBuffer[this.currentCodePoint + nCodePointsCount];
				nCodePointsCount++;
			}
		}

		this.codePointsCount = nCodePointsCount;
	}
	CCodePointsCalculator.prototype.getCount = function()
	{
		return this.codePointsCount;
	};
	CCodePointsCalculator.prototype.get = function(nPos)
	{
		return this.codePoints[this.currentCodePoint + nPos];
	};
	const CODEPOINTS_CALCULATOR = new CCodePointsCalculator(CODEPOINTS, CLUSTER);


	AscFonts.HB_StartString = function()
	{
		if (!STRING_POINTER)
			STRING_POINTER = AscFonts.AllocString(6 * STRING_MAX_LEN + 1);

		STRING_LEN  = 0;
		CLUSTER_LEN = 0;
		CLUSTER_MAX = 0;
	};
	AscFonts.HB_AppendToString = function(code)
	{
		let arrBuffer   = STRING_POINTER.getBuffer();
		let nClusterLen = -1;

		if (code < 0x80)
		{
			arrBuffer[STRING_LEN++] = code;
			nClusterLen = 1;
		}
		else if (code < 0x0800)
		{
			arrBuffer[STRING_LEN++] = (0xC0 | (code >> 6));
			arrBuffer[STRING_LEN++] = (0x80 | (code & 0x3F));
			nClusterLen = 2;
		}
		else if (code < 0x10000)
		{
			arrBuffer[STRING_LEN++] = (0xE0 | (code >> 12));
			arrBuffer[STRING_LEN++] = (0x80 | ((code >> 6) & 0x3F));
			arrBuffer[STRING_LEN++] = (0x80 | (code & 0x3F));
			nClusterLen = 3;
		}
		else if (code < 0x1FFFFF)
		{
			arrBuffer[STRING_LEN++] = (0xF0 | (code >> 18));
			arrBuffer[STRING_LEN++] = (0x80 | ((code >> 12) & 0x3F));
			arrBuffer[STRING_LEN++] = (0x80 | ((code >> 6) & 0x3F));
			arrBuffer[STRING_LEN++] = (0x80 | (code & 0x3F));
			nClusterLen = 4;
		}
		else if (code < 0x3FFFFFF)
		{
			arrBuffer[STRING_LEN++] = (0xF8 | (code >> 24));
			arrBuffer[STRING_LEN++] = (0x80 | ((code >> 18) & 0x3F));
			arrBuffer[STRING_LEN++] = (0x80 | ((code >> 12) & 0x3F));
			arrBuffer[STRING_LEN++] = (0x80 | ((code >> 6) & 0x3F));
			arrBuffer[STRING_LEN++] = (0x80 | (code & 0x3F));
			nClusterLen = 5;
		}
		else if (code < 0x7FFFFFFF)
		{
			arrBuffer[STRING_LEN++] = (0xFC | (code >> 30));
			arrBuffer[STRING_LEN++] = (0x80 | ((code >> 24) & 0x3F));
			arrBuffer[STRING_LEN++] = (0x80 | ((code >> 18) & 0x3F));
			arrBuffer[STRING_LEN++] = (0x80 | ((code >> 12) & 0x3F));
			arrBuffer[STRING_LEN++] = (0x80 | ((code >> 6) & 0x3F));
			arrBuffer[STRING_LEN++] = (0x80 | (code & 0x3F));
			nClusterLen = 6;
		}

		if (-1 !== nClusterLen)
		{
			CODEPOINTS[CLUSTER_LEN] = code;
			CLUSTER[CLUSTER_LEN++]  = nClusterLen;
			CLUSTER_MAX += nClusterLen;
		}
	};
	AscFonts.HB_EndString = function()
	{
		STRING_POINTER.set(STRING_LEN++, 0);
	};
	AscFonts.HB_GetStringLength = function()
	{
		return STRING_LEN;
	};
	AscFonts.HB_ShapeString = function(textShaper, fontId, fontStyle, fontFile, features, script, direction, language)
	{
		if (!STRING_POINTER)
			return;

		let retObj = AscFonts.HB_ShapeText(fontFile, STRING_POINTER, features, script, direction, language, READER);
		if (retObj["error"])
			return;
		
		let isRtl = direction === AscFonts.HB_DIRECTION.HB_DIRECTION_RTL;
		
		CODEPOINTS_CALCULATOR.start(isRtl ? CLUSTER_MAX : 0);
		let prevCluster = -1;
		let type, flags, gid, cluster, x_advance, y_advance, x_offset, y_offset;
		let isLigature = false;
		let nWidth     = 0;
		let reader = READER;
		let glyphsCount = retObj.count;
		for (let i = 0; i < glyphsCount; i++)
		{
			type      = reader.readByte();
			flags     = reader.readByte();
			gid       = reader.readInt();
			cluster   = reader.readInt();
			x_advance = reader.readInt();
			y_advance = reader.readInt();
			x_offset  = reader.readInt();
			y_offset  = reader.readInt();
			
			if (cluster !== prevCluster && -1 !== prevCluster)
			{
				CODEPOINTS_CALCULATOR.calculate(isRtl ? prevCluster : cluster);
				textShaper.FlushGrapheme(AscFonts.GetGrapheme(CODEPOINTS_CALCULATOR), nWidth, CODEPOINTS_CALCULATOR.getCount(), isLigature);
				nWidth = 0;
			}

			if (cluster !== prevCluster)
			{
				prevCluster = cluster;
				isLigature  = LIGATURE === type;
				AscFonts.InitGrapheme(fontId, fontStyle);
			}

			AscFonts.AddGlyphToGrapheme(gid, x_advance, y_advance, x_offset, y_offset);
			nWidth += x_advance * COEF;
		}
		
		CODEPOINTS_CALCULATOR.calculate(isRtl ? 0 : CLUSTER_MAX);
		textShaper.FlushGrapheme(AscFonts.GetGrapheme(CODEPOINTS_CALCULATOR), nWidth, CODEPOINTS_CALCULATOR.getCount(), isLigature);
		
		retObj["free"]();
	};

	AscFonts.HB_Shape = function(fontFile, text, features, script, direction, language)
	{
		if (text.length === 0)
			return null;

		AscFonts.HB_StartString();
		for (let iter = text.getUnicodeIterator(); iter.check(); iter.next())
		{
			AscFonts.HB_AppendToString(iter.value());
		}
		AscFonts.HB_EndString();

		let retObj = AscFonts.HB_ShapeText(fontFile, STRING_POINTER, features, script, direction, language, READER);
		if (retObj.error)
			return;

		let reader = READER;
		let glyphs = [];
		let glyphsCount = retObj.count;
		for (let i = 0; i < glyphsCount; i++)
		{
			let glyph = {};

			glyph.type = reader.readByte();
			glyph.flags = reader.readByte();

			glyph.gid = reader.readInt();
			glyph.cluster = reader.readInt();

			glyph.x_advance = reader.readInt();
			glyph.y_advance = reader.readInt();
			glyph.x_offset = reader.readInt();
			glyph.y_offset = reader.readInt();

			glyphs.push(glyph);
		}

		let utf8_start = 0;
		let utf8_end = 0;

		let arBuffer = STRING_POINTER.getBuffer();
		if (direction === AscFonts.HB_DIRECTION.HB_DIRECTION_RTL)
		{
			for (let i = glyphsCount - 1; i >= 0; i--)
			{
				if (i === 0)
					utf8_end = CLUSTER_MAX;
				else
					utf8_end = glyphs[i - 1].cluster;

				glyphs[i].text = String.prototype.fromUtf8(arBuffer, utf8_start, utf8_end - utf8_start);
				utf8_start = utf8_end;
			}
		}
		else
		{
			for (let i = 0; i < glyphsCount; i++)
			{
				if (i === (glyphsCount - 1))
					utf8_end = CLUSTER_MAX;
				else
					utf8_end = glyphs[i + 1].cluster;

				glyphs[i].text = String.prototype.fromUtf8(arBuffer, utf8_start, utf8_end - utf8_start);
				utf8_start = utf8_end;
			}
		}

		retObj["free"]();
		return glyphs;
	};

	// ZLIB
	function ZLib()
	{
		/** @suppress {checkVars} */
		this.engine = window["NATIVE_EDITOR_ENJINE"] ? CreateEmbedObject("CZipEmbed") : new AscCommon["CZLibEngineJS"]();
		this.files = [];
	}
	/**
	 * Open archive from bytes
	 * @param {Uint8Array | ArrayBuffer} buf
	 * @returns {boolean} success or not
	 */
	ZLib.prototype.open = function(buf)
	{
		if (this.engine.open(buf))
			this.files = this.engine["getPaths"]();
		return (this.files.length > 0) ? true : false;
	};
	/**
	 * Create new archive
	 * @returns {boolean} success or not
	 */
	ZLib.prototype.create = function()
	{
		return this.engine["create"]();
	};
	/**
	 * Save archive from current files
	 * @returns {Uint8Array | null} zip-archive bytes, or null if error
	 */
	ZLib.prototype.save = function()
	{
		return this.engine["save"]();
	};
	/**
	 * Get uncomressed file from archive
	 * @param {string} path
	 * @returns {Uint8Array | null} bytes of uncompressed data, or null if error
	 */
	ZLib.prototype.getFile = function(path)
	{
		return this.engine["getFile"](path);
	};
	/**
	 * Add uncomressed file to archive
	 * @param {string} path
	 * @param {Uint8Array | ArrayBuffer} new file in archive
	 * @returns {boolean} success or not
	 */
	ZLib.prototype.addFile = function(path, data)
	{
		return this.engine["addFile"](path, (undefined !== data.byteLength) ? new Uint8Array(data) : data);
	};
	/**
	 * Remove file from archive
	 * @param {string} path
	 * @returns {boolean} success or not
	 */
	ZLib.prototype.removeFile = function(path)
	{
		return this.engine["removeFile"](path);
	};
	/**
	 * Close & remove all used memory in archive
	 * @returns {undefined}
	 */
	ZLib.prototype.close = function()
	{
		return this.engine["close"]();
	};
	/**
	 * Get image blob for browser
	 * @returns {Blob}
	 */
	ZLib.prototype.getImageBlob = function(path)
	{
		return this.engine["getImageBlob"](path);
	};
	/**
	 * Get image file raw data. this memory was copied and detach from archive.
	 * @returns {Uint8Array}
	 */
	ZLib.prototype.getImageBuffer = function(path)
	{
		return this.engine["getImageBuffer"](path);
	};
	/**
	 * Get all file paths in archive
	 * @returns {Array}
	 */
	ZLib.prototype.getPaths = function()
	{
		return this.engine["getPaths"]();
	};

	AscCommon.ZLib = ZLib;

	function ZlibImageBlobs()
	{
		this.url2BlobUrl = {};
		this.blobUrl2Data = {};
		this.url2Base64 = {};

		this.nativeBlobCounter = 1;
	}
	ZlibImageBlobs.prototype.getBlobUrl = function(path, zip)
	{
		if (this.url2BlobUrl[path])
			return this.url2BlobUrl[path];

		let result = zip.getImageBuffer(path);
		if (result == null)
			return "";

		let blobUrl = "";
		let blobType = AscCommon.openXml.GetMimeType((24 !== result.type) ? AscCommon.GetFileExtension(path) : "svg");

		if (window["NATIVE_EDITOR_ENJINE"])
		{
			blobUrl = "blob:internal-image" + this.nativeBlobCounter++;
		}
		else
		{
			try
			{
				let blob = new Blob([result.data], {type: blobType});
				blobUrl = window.URL.createObjectURL(blob);
			}
			catch (e)
			{
				blobUrl = "error";
				AscCommon.consoleLog("ERROR: Image blob was not loaded");
			}
		}

		this.blobUrl2Data[blobUrl] = result;
		this.url2BlobUrl[path] = blobUrl;
		return blobUrl;
	};
	ZlibImageBlobs.prototype.getImageBase64 = function(url)
	{
		if (this.url2Base64[url])
			return this.url2Base64[url];

		let obj = this.blobUrl2Data[url];
		if (!obj)
			return url;

		let header = "";
		switch (obj.type)
		{
			case 3:
				header = "data:image/jpeg;base64,";
				break;
			case 24:
				header = "data:image/svg+xml;base64,";
				break;
			case 4:
			default:
				header = "data:image/png;base64,";
		}

		this.url2Base64[url] = header + AscCommon.Base64.encode(obj.data);
		return this.url2Base64[url];
	};

	window["AscCommon"].g_oDocumentBlobUrls = new ZlibImageBlobs();

	if (AscCommon["CZLibEngineJS"])
		AscCommon["CZLibEngineJS"].prototype["isModuleInit"] = true;

	if (window["NATIVE_EDITOR_ENJINE"])
		window["InitNativeZLib"] = function() { window.nativeZlibEngine = new ZLib(); };
	else
		window.nativeZlibEngine = new ZLib();

	function Hyphenation()
	{
		this._value = "";
		this._lang = 0;
		this._dictionaries = {};
		this._mapToNames = null;

		this.addCodePoint = function(codePoint)
		{
			this._value += String.fromCodePoint(codePoint);
		};
		this.clear = function()
		{
			this._value = "";
		};
		this.setLang = function(langCode, callback)
		{
			this._lang = langCode;

			let _langKey = "" + langCode;
			if (this._dictionaries[_langKey] !== undefined)
				return this._dictionaries[_langKey];

			if (window["NATIVE_EDITOR_ENJINE"])
			{
				this._dictionaries[_langKey] = AscFonts.Hyphen_CheckDictionary(this._lang);
				return this._dictionaries[_langKey];
			}
			else if (callback)
			{
				if (!this._mapToNames)
					this._mapToNames = AscCommon.spellcheckGetLanguages();

				if (undefined !== this._mapToNames["" + langCode])
					this.loadDictionary(langCode, callback);
			}

			return false;
		};
		this.hyphenate = function()
		{
			if ("" === this._value) 
				return [];	
			return AscFonts.Hyphen_Word(this._lang, this._value);
		};

		this.loadDictionary = function(lang, callback)
		{
			if (window["NATIVE_EDITOR_ENJINE"])
			{
				callback();
				return;
			}

			if (!this._mapToNames)
				this._mapToNames = AscCommon.spellcheckGetLanguages();

			let _langKey = "" + lang;
			let _langName = this._mapToNames[_langKey];
			if (_langName === undefined)
			{
				this._dictionaries[_langKey] = false;
				callback();
				return;
			}

			this._loadDictionaryAttemt(_langKey, _langName, callback);
		};

		this._loadDictionaryAttemt = function(langKey, langName, callback, currentAttempt)
		{
			var xhr = new XMLHttpRequest();
			let urlDictionaries = "../../../../dictionaries/";
			if (window["AscDesktopEditor"] && window["AscDesktopEditor"]["getDictionariesPath"])
			{
				let urlDesktop = window["AscDesktopEditor"]["getDictionariesPath"]();
				if ("" !== urlDesktop)
					urlDictionaries = urlDesktop;
			}
			
			let url = urlDictionaries + langName + "/hyph_" + langName + ".dic";

			xhr.open('GET', url, true);
			xhr.responseType = 'arraybuffer';
			xhr.currentAttempt = currentAttempt || 0;

			if (xhr.overrideMimeType)
				xhr.overrideMimeType('text/plain; charset=x-user-defined');
			else
				xhr.setRequestHeader('Accept-Charset', 'x-user-defined');

			var _t = this;
			xhr.onload = function()
			{
				if (this.status === 200 || location.href.indexOf("file:") === 0)
				{
					_t._dictionaries[langKey] = true;
					AscFonts.Hyphen_LoadDictionary(parseInt(langKey), this.response);
					callback();
				}
			};
			xhr.onerror = function()
			{
				let _currentAttempt = xhr.currentAttempt + 1;
				if (_currentAttempt > 3)
				{
					_t._dictionaries[langKey] = false;
					callback();
					return;
				}

				_t._loadDictionaryAttemt(langKey, langName, callback, _currentAttempt);
			};

			xhr.send(null);
		};
	}

	window["AscHyphenation"] = new Hyphenation();
}
