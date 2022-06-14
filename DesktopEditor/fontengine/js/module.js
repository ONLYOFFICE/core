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

function onLoadFontsModule(window, undefined)
{
	var AscFonts = window['AscFonts'];

	AscFonts.TT_INTERPRETER_VERSION_35 = 35;
	AscFonts.TT_INTERPRETER_VERSION_38 = 38;
	AscFonts.TT_INTERPRETER_VERSION_40 = 40;

	AscFonts.CopyStreamToMemory = AscFonts["CopyStreamToMemory"];

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
			this.family_name += String.fromCharCode(c);
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

	const FONTSIZE       = 72;
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

	function CCodePointsCalculator(codePointsBuffer, clusterBuffer)
	{
		this.currentCluster   = 0;
		this.currentCodePoint = 0;
		this.clusterBuffer    = clusterBuffer;
		this.codePoints       = codePointsBuffer;
		this.codePointsCount  =  0
	}
	CCodePointsCalculator.prototype.start = function()
	{
		this.currentCluster = 0;
		this.currentCodePoint = 0;
		this.codePointsCount  = 0;
	}
	CCodePointsCalculator.prototype.calculate = function(cluster)
	{
		this.currentCodePoint += this.codePointsCount;

		let nCodePointsCount = 0;

		if (cluster < this.currentCluster)
		{
			// TODO: RTL
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

		CODEPOINTS_CALCULATOR.start();
		let prevCluster = -1, type, flags, gid, cluster, x_advance, y_advance, x_offset, y_offset, codePoints;
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
				CODEPOINTS_CALCULATOR.calculate(cluster);
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
		CODEPOINTS_CALCULATOR.calculate(CLUSTER_MAX);
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
}
