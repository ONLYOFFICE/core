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

    var AscFonts = window['AscFonts'];
    
    var FS = undefined;
    
    //desktop_fetch

    //polyfill

    //string_utf8

    //module

    window['AscFonts'] = window['AscFonts'] || {};
    var AscFonts = window['AscFonts'];

    AscFonts.CreateLibrary = function()
    {
        return Module["_ASC_FT_Init"]();
    };

    AscFonts.TT_INTERPRETER_VERSION_35 = 35;
    AscFonts.TT_INTERPRETER_VERSION_38 = 38;
    AscFonts.TT_INTERPRETER_VERSION_40 = 40;

    AscFonts.FT_Set_TrueType_HintProp = function(library, tt_interpreter)
    {
        return Module["_ASC_FT_Set_TrueType_HintProp"](library, tt_interpreter);
    };

    AscFonts.CreateNativeStream = function(_typed_array)
    {
        var _fontStreamPointer = Module["_ASC_FT_Malloc"](_typed_array.size);
        Module["HEAP8"].set(_typed_array.data, _fontStreamPointer);
        return { asc_marker: true, data: _fontStreamPointer, len: _typed_array.size};
    };

    AscFonts.CreateNativeStreamByIndex = function(stream_index)
    {
        var _stream_pos = AscFonts.g_fonts_streams[stream_index];
        if (_stream_pos && true !== _stream_pos.asc_marker)
        {
            var _native_stream = AscFonts.CreateNativeStream(AscFonts.g_fonts_streams[stream_index]);
            AscFonts.g_fonts_streams[stream_index] = null;
            AscFonts.g_fonts_streams[stream_index] = _native_stream;
        }
    };

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
    };

    CFaceInfo.prototype.load = function(face)
    {
        var _bufferPtr = Module["_ASC_FT_GetFaceInfo"](face);
        if (!_bufferPtr)
            return;

        var _len_buffer = Math.min((Module["HEAP8"].length - _bufferPtr) >> 2, 250); //max 230 symbols on name & style
        var _buffer = new Int32Array(Module["HEAP8"].buffer, _bufferPtr, _len_buffer);
        var _index = 0;

        this.units_per_EM 	= Math.abs(_buffer[_index++]);
        this.ascender 		= _buffer[_index++];
        this.descender 		= _buffer[_index++];
        this.height 		= _buffer[_index++];
        this.face_flags 	= _buffer[_index++];
        this.num_faces 		= _buffer[_index++];
        this.num_glyphs 	= _buffer[_index++];
        this.num_charmaps 	= _buffer[_index++];
        this.style_flags 	= _buffer[_index++];
        this.face_index 	= _buffer[_index++];

        var c = _buffer[_index++];
        while (c)
        {
            this.family_name += String.fromCharCode(c);
            c = _buffer[_index++];
        }

        c = _buffer[_index++];
        while (c)
        {
            this.style_name += String.fromCharCode(c);
            c = _buffer[_index++];
        }

        this.os2_version 		= _buffer[_index++];
        this.os2_usWeightClass 	= _buffer[_index++];
        this.os2_fsSelection 	= _buffer[_index++];
        this.os2_usWinAscent 	= _buffer[_index++];
        this.os2_usWinDescent 	= _buffer[_index++];
        this.os2_usDefaultChar 	= _buffer[_index++];
        this.os2_sTypoAscender 	= _buffer[_index++];
        this.os2_sTypoDescender = _buffer[_index++];
        this.os2_sTypoLineGap 	= _buffer[_index++];

        this.os2_ulUnicodeRange1 	= AscFonts.FT_Common.IntToUInt(_buffer[_index++]);
        this.os2_ulUnicodeRange2 	= AscFonts.FT_Common.IntToUInt(_buffer[_index++]);
        this.os2_ulUnicodeRange3 	= AscFonts.FT_Common.IntToUInt(_buffer[_index++]);
        this.os2_ulUnicodeRange4 	= AscFonts.FT_Common.IntToUInt(_buffer[_index++]);
        this.os2_ulCodePageRange1 	= AscFonts.FT_Common.IntToUInt(_buffer[_index++]);
        this.os2_ulCodePageRange2 	= AscFonts.FT_Common.IntToUInt(_buffer[_index++]);

        this.os2_nSymbolic 			= _buffer[_index++];
        this.header_yMin 			= _buffer[_index++];
        this.header_yMax 			= _buffer[_index++];

        var fixedSizesCount = _buffer[_index++];
        for (var i = 0; i < fixedSizesCount; i++)
            this.monochromeSizes.push(_buffer[_index++]);

        Module["_ASC_FT_Free"](_bufferPtr);
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
        return Module["_ASC_FT_Open_Face"](library, stream.data, stream.len, face_index);
    };

    AscFonts.FT_Glyph_Get_Measure = function(face, vector_worker, painter)
    {
        var _bufferPtr = Module["_ASC_FT_Get_Glyph_Measure_Params"](face, vector_worker ? 1 : 0);
        if (!_bufferPtr)
            return null;

        var _len = 15;
        if (vector_worker)
            _len = Module["HEAP32"][_bufferPtr >> 2];

        var _buffer = new Int32Array(Module["HEAP8"].buffer, _bufferPtr, _len);

        var _info = new CGlyphMetrics();
        _info.bbox_xMin     = _buffer[1];
        _info.bbox_yMin     = _buffer[2];
        _info.bbox_xMax     = _buffer[3];
        _info.bbox_yMax     = _buffer[4];

        _info.width         = _buffer[5];
        _info.height        = _buffer[6];

        _info.horiAdvance   = _buffer[7];
        _info.horiBearingX  = _buffer[8];
        _info.horiBearingY  = _buffer[9];

        _info.vertAdvance   = _buffer[10];
        _info.vertBearingX  = _buffer[11];
        _info.vertBearingY  = _buffer[12];

        _info.linearHoriAdvance     = _buffer[13];
        _info.linearVertAdvance     = _buffer[14];

        if (vector_worker)
        {
            painter.start(vector_worker);

            var _pos = 15;
            while (_pos < _len)
            {
                switch (_buffer[_pos++])
                {
                    case 0:
                    {
                        painter._move_to(_buffer[_pos++], _buffer[_pos++], vector_worker);
                        break;
                    }
                    case 1:
                    {
                        painter._line_to(_buffer[_pos++], _buffer[_pos++], vector_worker);
                        break;
                    }
                    case 2:
                    {
                        painter._conic_to(_buffer[_pos++], _buffer[_pos++], _buffer[_pos++], _buffer[_pos++], vector_worker);
                        break;
                    }
                    case 3:
                    {
                        painter._cubic_to(_buffer[_pos++], _buffer[_pos++], _buffer[_pos++], _buffer[_pos++], _buffer[_pos++], _buffer[_pos++], vector_worker);
                        break;
                    }
                    default:
                        break;
                }
            }

            painter.end(vector_worker);
        }

        Module["_ASC_FT_Free"](_bufferPtr);
        _buffer = null;

        return _info;
    };

    AscFonts.FT_Glyph_Get_Raster = function(face, render_mode)
    {
        var _bufferPtr = Module["_ASC_FT_Get_Glyph_Render_Params"](face, render_mode);
        if (!_bufferPtr)
            return null;

        var _buffer = new Int32Array(Module["HEAP8"].buffer, _bufferPtr, 6);

        var _info = new CGlyphBitmapImage();
        _info.left    = _buffer[0];
        _info.top     = _buffer[1];
        _info.width   = _buffer[2];
        _info.rows    = _buffer[3];
        _info.pitch   = _buffer[4];
        _info.mode    = _buffer[5];

        Module["_ASC_FT_Free"](_bufferPtr);
        return _info;
    };

    AscFonts.FT_Load_Glyph = Module["_FT_Load_Glyph"];
    AscFonts.FT_Set_Transform = Module["_ASC_FT_Set_Transform"];
    AscFonts.FT_Set_Char_Size = Module["_FT_Set_Char_Size"];

    AscFonts.FT_SetCMapForCharCode = Module["_ASC_FT_SetCMapForCharCode"];
    AscFonts.FT_GetKerningX = Module["_ASC_FT_GetKerningX"];
    AscFonts.FT_GetFaceMaxAdvanceX = Module["_ASC_FT_GetFaceMaxAdvanceX"];
    AscFonts.FT_Get_Glyph_Render_Buffer = function(face, rasterInfo, isCopyToRasterMemory)
    {
        var _bufferPtr = Module["_ASC_FT_Get_Glyph_Render_Buffer"](face);
        var tmp = new Uint8Array(Module["HEAP8"].buffer, _bufferPtr, rasterInfo.pitch * rasterInfo.rows);

        if (!isCopyToRasterMemory)
            return tmp;

        AscFonts.raster_memory.CheckSize(rasterInfo.width, rasterInfo.rows);

        var offsetSrc = 0;
        var offsetDst = 3;
        var dstData = AscFonts.raster_memory.m_oBuffer.data;

        if (rasterInfo.pitch >= rasterInfo.width)
		{
			for (var j = 0; j < rasterInfo.rows; ++j, offsetSrc += rasterInfo.pitch)
			{
				offsetDst = 3 + j * AscFonts.raster_memory.pitch;
				for (var i = 0; i < rasterInfo.width; i++, offsetDst += 4)
				{
					dstData[offsetDst] = tmp[offsetSrc + i];
				}
			}
		}
		else
        {
            var bitNumber = 0;
            var byteNumber = 0;
			for (var j = 0; j < rasterInfo.rows; ++j, offsetSrc += rasterInfo.pitch)
			{
				offsetDst = 3 + j * AscFonts.raster_memory.pitch;
				bitNumber = 0;
				byteNumber = 0;
				for (var i = 0; i < rasterInfo.width; i++, offsetDst += 4, bitNumber++)
				{
				    if (8 == bitNumber)
                    {
                        bitNumber = 0;
                        byteNumber++;
                    }
					dstData[offsetDst] = (tmp[offsetSrc + byteNumber] & (1 << (7 - bitNumber))) ? 255 : 0;
				}
			}
        }

        tmp = null;
    };

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
    CBinaryReader.prototype.readUInt = function()
    {
        var val = this.data[this.pos] | this.data[this.pos + 1] << 8 | this.data[this.pos + 2] << 16 | this.data[this.pos + 3] << 24;
        this.pos += 4;
        return (val < 0) ? val + 4294967296 : val;
    };
    CBinaryReader.prototype.readByte = function()
    {
        return this.data[this.pos++];
    };
    CBinaryReader.prototype.readPointer64 = function()
    {
        let i1 = this.readUInt();
        let i2 = this.readUInt();
        if (i2 === 0)
            return i1;
        return i2 * 4294967296 + i1;
    };
    CBinaryReader.prototype.isValid = function()
    {
        return (this.pos < this.limit) ? true : false;
    };

    AscFonts.HB_Shape = function(fontFile, text, features, script, direction, language)
    {
        if (text.length === 0)
            return null;

        let textBuffer = text.toUtf8();
        let textPointer = Module["_malloc"](textBuffer.length);
        Module["HEAP8"].set(textBuffer, textPointer);

        if (!AscFonts.hb_cache_languages[language])
        {
            let langBuffer = language.toUtf8();
            var langPointer = Module["_malloc"](langBuffer.length);
            Module["HEAP8"].set(langBuffer, langBuffer);

            AscFonts.hb_cache_languages[language] = langPointer;
        }

        let shapeData = Module["_ASC_HP_ShapeText"](fontFile.m_pFace, fontFile.m_pHBFont, textPointer, features, script, direction, AscFonts.hb_cache_languages[language]);

        Module["_free"](textPointer);

        let len = new Int32Array(Module["HEAP8"].buffer, shapeData, 4)[0];

        let buffer = new Uint8Array(Module["HEAP8"].buffer, shapeData + 4, len - 4);
        let reader = new CBinaryReader(buffer, 0, len - 4);

        let glyphsCount = (len - 12) / 26;

        fontFile.m_pHBFont = reader.readPointer64();

        let glyphs = [];
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

        if (direction === AscFonts.HB_DIRECTION.HB_DIRECTION_RTL)
        {
            for (let i = glyphsCount - 1; i >= 0; i--)
            {
                if (i === 0)
                    utf8_end = textBuffer.length - 1;
                else
                    utf8_end = glyphs[i - 1].cluster;

                glyphs[i].text = String.prototype.fromUtf8(textBuffer, utf8_start, utf8_end - utf8_start);
                utf8_start = utf8_end;
            }
        }
        else
        {
            for (let i = 0; i < glyphsCount; i++)
            {
                if (i === (glyphsCount - 1))
                    utf8_end = textBuffer.length - 1;
                else
                    utf8_end = glyphs[i + 1].cluster;

                glyphs[i].text = String.prototype.fromUtf8(textBuffer, utf8_start, utf8_end - utf8_start);
                utf8_start = utf8_end;
            }
        }

        Module["_ASC_FT_Free"](shapeData);

        return glyphs;
    };

    AscFonts.onLoadModule();

    // this memory is not freed
    AscFonts.hb_cache_languages = {};

})(window, undefined);
