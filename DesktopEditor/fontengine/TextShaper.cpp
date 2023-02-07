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

#include "./TextShaper.h"
#include "./TextShaper_p.h"

namespace NSShaper
{
#define ALLOC_WRITER(size) result->Alloc((size)); unsigned char* pCurData = result->Data
#define WRITE_INT(value) result->WriteInt(pCurData, value); pCurData += 4
#define WRITE_UINT(value) result->WriteUInt(pCurData, value); pCurData += 4
#define WRITE_UCHAR(value) *pCurData++ = value

	CExternalPointer::CExternalPointer()
	{
		Data = NULL;
		Len = 0;
	}
	CExternalPointer::~CExternalPointer()
	{
	}

	void CExternalPointer::WriteInt(unsigned char* data, const int& value)
	{
		WriteUInt(data, (unsigned int)value);
	}
	void CExternalPointer::WriteUInt(unsigned char* data, const unsigned int& value)
	{
		data[0] = (value & 0xFF);
		data[1] = ((value >> 8) & 0xFF);
		data[2] = ((value >> 16) & 0xFF);
		data[3] = ((value >> 24) & 0xFF);
	}

	void CExternalPointer::Alloc(const unsigned int& len)
	{
		Len = len;
		Data = new unsigned char[Len];
	}
	void CExternalPointer::Free()
	{
		if (Data)
			delete [] Data;
	}

	// outline
#define FT_DECOMPOSE_OUTLINE_BUFFER_INIT_SIZE 200
	typedef struct  FT_Decompose_Outline_Buffer_
	{
		FT_Pos* buffer;
		FT_Int size;
		FT_Int pos;
	} FT_Decompose_Outline_Buffer;

	void FT_Decompose_Outline_Buffer_Init(FT_Decompose_Outline_Buffer* buffer)
	{
		buffer->buffer = NULL;
		buffer->size = 0;
		buffer->pos = 0;
	}

	void FT_Decompose_Outline_Buffer_Destroy(FT_Decompose_Outline_Buffer* buffer)
	{
		ft_sfree(buffer->buffer);
	}

	void FT_Decompose_Outline_Buffer_Check(FT_Decompose_Outline_Buffer* buffer, FT_Int add)
	{
		if ((buffer->pos + add) < buffer->size)
			return;

		if (NULL == buffer->buffer)
		{
			buffer->buffer = (FT_Pos*)ft_smalloc(FT_DECOMPOSE_OUTLINE_BUFFER_INIT_SIZE * sizeof(FT_Pos));
			buffer->size = FT_DECOMPOSE_OUTLINE_BUFFER_INIT_SIZE;
			buffer->pos = 0;
			return;
		}

		FT_Int sizeNew = 2 * buffer->size;
		FT_Pos* bufferNew = (FT_Pos*)ft_smalloc((size_t)sizeNew * sizeof(FT_Pos));
		memcpy(bufferNew, buffer->buffer, (size_t)buffer->pos * sizeof(FT_Pos));
		ft_sfree(buffer->buffer);
		buffer->buffer = bufferNew;
		buffer->size = sizeNew;
		// pos
	}

	int GlyphPathMoveTo(const FT_Vector *point, void *user)
	{
		FT_Decompose_Outline_Buffer* buffer = (FT_Decompose_Outline_Buffer*)user;
		FT_Decompose_Outline_Buffer_Check(buffer, 3);
		buffer->buffer[buffer->pos++] = 0;
		buffer->buffer[buffer->pos++] = point->x;
		buffer->buffer[buffer->pos++] = point->y;
		return 0;
	}

	int GlyphPathLineTo(const FT_Vector *point, void *user)
	{
		FT_Decompose_Outline_Buffer* buffer = (FT_Decompose_Outline_Buffer*)user;
		FT_Decompose_Outline_Buffer_Check(buffer, 3);
		buffer->buffer[buffer->pos++] = 1;
		buffer->buffer[buffer->pos++] = point->x;
		buffer->buffer[buffer->pos++] = point->y;
		return 0;
	}

	int GlyphPathConicTo(const FT_Vector *pControlPoint, const FT_Vector *pEndPoint, void *user)
	{
		FT_Decompose_Outline_Buffer* buffer = (FT_Decompose_Outline_Buffer*)user;
		FT_Decompose_Outline_Buffer_Check(buffer, 5);
		buffer->buffer[buffer->pos++] = 2;
		buffer->buffer[buffer->pos++] = pControlPoint->x;
		buffer->buffer[buffer->pos++] = pControlPoint->y;
		buffer->buffer[buffer->pos++] = pEndPoint->x;
		buffer->buffer[buffer->pos++] = pEndPoint->y;
		return 0;
	}

	int GlyphPathCubicTo(const FT_Vector *pFirstControlPoint, const FT_Vector *pSecondControlPoint, const FT_Vector *pEndPoint, void *user)
	{
		FT_Decompose_Outline_Buffer* buffer = (FT_Decompose_Outline_Buffer*)user;
		FT_Decompose_Outline_Buffer_Check(buffer, 7);
		buffer->buffer[buffer->pos++] = 3;
		buffer->buffer[buffer->pos++] = pFirstControlPoint->x;
		buffer->buffer[buffer->pos++] = pFirstControlPoint->y;
		buffer->buffer[buffer->pos++] = pSecondControlPoint->x;
		buffer->buffer[buffer->pos++] = pSecondControlPoint->y;
		buffer->buffer[buffer->pos++] = pEndPoint->x;
		buffer->buffer[buffer->pos++] = pEndPoint->y;
		return 0;
	}
	/////////////////////////////////////////////////////////////////////

	void FT_Free(void* data)
	{
		ft_sfree(data);
	}

	void* FT_Library_Init()
	{
		FT_Library library = NULL;
		FT_Init_FreeType(&library);
		FT_Library_SetLcdFilter(library, FT_LCD_FILTER_DEFAULT);
		return library;
	}
	void FT_Library_Destroy(void* library)
	{
		::FT_Done_FreeType((FT_Library)library);
	}
	void FT_Library_Reference(void* library)
	{
		FT_Reference_Library((FT_Library)library);
	}
	void FT_Library_UnReference(void* library_)
	{
		FT_Library library = (FT_Library)library_;
		if (library->refcount == 1)
			::FT_Done_FreeType(library);
		else
			::FT_Done_Library(library);
	}

	int FT_Set_TrueType_HintProp(void* library, unsigned int interpreter_version)
	{
		FT_UInt ft_interpreter_version = interpreter_version;
		return FT_Property_Set((FT_Library)library, "truetype", "interpreter-version", &ft_interpreter_version);
	}

	void* FT_Open_Face(void* library, unsigned char* memory, unsigned int size, int face_index)
	{
		FT_Open_Args oOpenArgs;
		oOpenArgs.flags			= FT_OPEN_MEMORY | FT_OPEN_PARAMS;
		oOpenArgs.memory_base	= memory;
		oOpenArgs.memory_size	= (FT_Long)size;

		FT_Parameter *pParams = (FT_Parameter *)ft_smalloc( sizeof(FT_Parameter) * 4 );
		pParams[0].tag  = FT_MAKE_TAG( 'i', 'g', 'p', 'f' );
		pParams[0].data = NULL;
		pParams[1].tag  = FT_MAKE_TAG( 'i', 'g', 'p', 's' );
		pParams[1].data = NULL;
		pParams[2].tag  = FT_PARAM_TAG_IGNORE_PREFERRED_FAMILY;
		pParams[2].data = NULL;
		pParams[3].tag  = FT_PARAM_TAG_IGNORE_PREFERRED_SUBFAMILY;
		pParams[3].data = NULL;

		oOpenArgs.params = pParams;
		oOpenArgs.num_params = 4;

		FT_Face face;
		int error = ::FT_Open_Face( (FT_Library)library, &oOpenArgs, face_index, &face );

		ft_sfree(pParams);

		if (error)
			return NULL;

		return face;
	}

	void FT_Done_Face(void* face)
	{
		::FT_Done_Face((FT_Face)face);
	}

	void FT_Done_Face_With_Library(void* face)
	{
		FT_Face ftface = (FT_Face)face;
		FT_Library library = ftface->driver->root.library;

		bool bIsNeedUnreferenceLibrary = (1 == ftface->internal->refcount) ? true : false;
		::FT_Done_Face(ftface);

		if (bIsNeedUnreferenceLibrary)
			FT_Library_UnReference(library);
	}

	unsigned int FT_SetCMapForCharCode(void* face_pointer, unsigned int unicode)
	{
		if (!face_pointer)
			return 0;

		FT_Face face = (FT_Face)face_pointer;

		if ( 0 == face->num_charmaps )
			return unicode;

		unsigned int nCharIndex = 0;

		for ( int nIndex = 0; nIndex < face->num_charmaps; ++nIndex )
		{
			FT_CharMap pCharMap = face->charmaps[nIndex];

			if ( FT_Set_Charmap( face, pCharMap ) )
				continue;

			FT_Encoding pEncoding = pCharMap->encoding;

			if ( FT_ENCODING_UNICODE == pEncoding )
			{
				nCharIndex = FT_Get_Char_Index( face, unicode );
				if ( nCharIndex )
				{
					return nCharIndex;
				}
			}
			else if ( FT_ENCODING_NONE == pEncoding || FT_ENCODING_MS_SYMBOL == pEncoding || FT_ENCODING_APPLE_ROMAN == pEncoding )
			{
#if 0
				FT_ULong  charcode;
				FT_UInt   gindex;

				charcode = FT_Get_First_Char( face, &gindex );
				while ( gindex != 0 )
				{
					charcode = FT_Get_Next_Char( face, charcode, &gindex );
					if ( charcode == unicode )
					{
						nCharIndex = gindex;
						break;
					}
				}
#endif

				nCharIndex = FT_Get_Char_Index( face, unicode );
			}
		}

		return nCharIndex;
	}

	int FT_GetFaceMaxAdvanceX(void* face_pointer)
	{
		if (!face_pointer)
			return 0;

		FT_Face face = (FT_Face)face_pointer;
		if (!face->size)
			return 0;

		return (int)face->size->metrics.max_advance;
	}

	int FT_GetKerningX(void* face, unsigned int prev_gid, unsigned int gid)
	{
		FT_Vector vec;
		vec.x = 0;
		vec.y = 0;
		FT_Get_Kerning((FT_Face)face, prev_gid, gid, 0, &vec);
		return vec.x;
	}

	void FT_Set_Transform(void* face, int xx, int yx, int xy, int yy)
	{
		FT_Matrix m;
		m.xx = xx;
		m.yx = yx;
		m.xy = xy;
		m.yy = yy;
		::FT_Set_Transform((FT_Face)face, &m, NULL);
	}

	int FT_Load_Glyph(void* face, unsigned int glyph_index, int load_flags)
	{
		return ::FT_Load_Glyph((FT_Face)face, glyph_index, load_flags);
	}

	int FT_Set_Char_Size(void* face, int char_width, int char_height, unsigned int hres, unsigned int vres)
	{
		return ::FT_Set_Char_Size((FT_Face)face, char_width, char_height, hres, vres);
	}

	void FT_Glyph_Get_CBox(void* glyph, unsigned int bbox_mode, CExternalPointer* result)
	{
		FT_BBox bbox;
		FT_Glyph_Get_CBox((FT_Glyph)glyph, bbox_mode, &bbox);
		ALLOC_WRITER(4 * 4);
		WRITE_INT(bbox.xMin);
		WRITE_INT(bbox.yMin);
		WRITE_INT(bbox.xMax);
		WRITE_INT(bbox.yMax);
	}

	unsigned char* FT_Get_Glyph_Render_Buffer(void* face)
	{
		return ((FT_Face)face)->glyph->bitmap.buffer;
	}

	bool FT_Get_Glyph_Render_Params(void* face, int render_mode, CExternalPointer* result)
	{
		FT_GlyphSlot slot = ((FT_Face)face)->glyph;
		if (FT_Render_Glyph(slot, (FT_Render_Mode)render_mode))
			return false;

		ALLOC_WRITER(6 * 4);
		WRITE_INT(slot->bitmap_left);
		WRITE_INT(slot->bitmap_top);
		WRITE_UINT(slot->bitmap.width);
		WRITE_UINT(slot->bitmap.rows);
		WRITE_INT(slot->bitmap.pitch);
		WRITE_INT(slot->bitmap.pixel_mode);
		return true;
	}

	bool FT_GetFaceInfo(void* face_pointer, CExternalPointer* result)
	{
		if (!face_pointer)
			return false;

		FT_Face face = (FT_Face)face_pointer;

		//face->units_per_EM
		//face->ascender
		//face->descender
		//face->height
		//face->face_flags
		//face->num_faces
		//face->num_glyphs
		//face->num_charmaps
		//face->style_flags
		//face->face_index
		//face->family_name
		//face->style_name

		TT_OS2* os2 = (TT_OS2*)FT_Get_Sfnt_Table( face, ft_sfnt_os2 );
		//os2->version
		//os2->usWeightClass
		//os2->fsSelection
		//os2->usWinAscent
		//os2->usWinDescent
		//os2->usDefaultChar
		//os2->sTypoAscender;
		//os2->sTypoDescender;
		//os2->sTypoLineGap;

		//os2->ulUnicodeRange1
		//os2->ulUnicodeRange2
		//os2->ulUnicodeRange3
		//os2->ulUnicodeRange4
		//os2->ulCodePageRange1
		//os2->ulCodePageRange2

		int isSymbolic = -1;
		if (os2 && 0xFFFF != os2->version)
		{
			FT_ULong ulCodePageRange1 = os2->ulCodePageRange1;
			FT_ULong ulCodePageRange2 = os2->ulCodePageRange2;

			if ((ulCodePageRange1 & 0x80000000) || (ulCodePageRange1 == 0 && ulCodePageRange2 == 0))
			{
				for( int nIndex = 0; nIndex < face->num_charmaps; nIndex++ )
				{
					if (0 == face->charmaps[nIndex]->encoding_id && 3 == face->charmaps[nIndex]->platform_id)
					{
						isSymbolic = nIndex;
						break;
					}
				}
			}
		}

		int nHeader_yMin = face->descender;
		int nHeader_yMax = face->ascender;
		if (face && FT_IS_SFNT(face))
		{
			TT_Face ttface = (TT_Face)face;
			nHeader_yMin = ttface->header.yMin;
			nHeader_yMax = ttface->header.yMax;
		}

		//isSymbolic

		int* family_name = NULL;
		unsigned int family_name_len = 0;
		CheckUnicodeFaceName(face, family_name, family_name_len);

		unsigned int nLen1 = (unsigned int)family_name_len;
		unsigned int nLen2 = (unsigned int)strlen(face->style_name);

		unsigned int nLen = 28 + nLen1 + 1 + nLen2 + 1 + 1 + (int)face->num_fixed_sizes;

		ALLOC_WRITER(nLen * 4);
		WRITE_INT((int)face->units_per_EM);
		WRITE_INT((int)face->ascender);
		WRITE_INT((int)face->descender);
		WRITE_INT((int)face->height);
		WRITE_INT((int)face->face_flags);
		WRITE_INT((int)face->num_faces);
		WRITE_INT((int)face->num_glyphs);
		WRITE_INT((int)face->num_charmaps);
		WRITE_INT((int)face->style_flags);
		WRITE_INT((int)face->face_index);

		for (unsigned int i = 0; i < nLen1; ++i)
		{
			WRITE_INT(family_name[i]);
		}
		WRITE_INT(0);

		for (unsigned int i = 0; i < nLen2; ++i)
		{
			WRITE_INT(face->style_name[i]);
		}
		WRITE_INT(0);

		if (os2)
		{
			WRITE_INT((int)os2->version);
			WRITE_INT((int)os2->usWeightClass);
			WRITE_INT((int)os2->fsSelection);
			WRITE_INT((int)os2->usWinAscent);
			WRITE_INT((int)os2->usWinDescent);
			WRITE_INT((int)os2->usDefaultChar);
			WRITE_INT((int)os2->sTypoAscender);
			WRITE_INT((int)os2->sTypoDescender);
			WRITE_INT((int)os2->sTypoLineGap);

			WRITE_INT((int)os2->ulUnicodeRange1);
			WRITE_INT((int)os2->ulUnicodeRange2);
			WRITE_INT((int)os2->ulUnicodeRange3);
			WRITE_INT((int)os2->ulUnicodeRange4);
			WRITE_INT((int)os2->ulCodePageRange1);
			WRITE_INT((int)os2->ulCodePageRange2);
		}
		else
		{
			WRITE_INT((int)0xFFFF);
			WRITE_INT((int)0);
			WRITE_INT((int)0);
			WRITE_INT((int)0);
			WRITE_INT((int)0);
			WRITE_INT((int)0);
			WRITE_INT((int)0);
			WRITE_INT((int)0);
			WRITE_INT((int)0);

			WRITE_INT((int)0);
			WRITE_INT((int)0);
			WRITE_INT((int)0);
			WRITE_INT((int)0);
			WRITE_INT((int)0);
			WRITE_INT((int)0);
		}

		WRITE_INT((int)isSymbolic);
		WRITE_INT((int)nHeader_yMin);
		WRITE_INT((int)nHeader_yMax);

		WRITE_INT((int)face->num_fixed_sizes);

		for (int i = 0; i < face->num_fixed_sizes; ++i)
		{
			WRITE_INT((int)face->available_sizes[i].size);
		}

		if (family_name)
			delete [] family_name;

		return true;
	}

	bool FT_Get_Glyph_Measure_Params(void* face, bool isVector, CExternalPointer* result)
	{
		FT_GlyphSlot slot = ((FT_Face)face)->glyph;

		if (!isVector && slot->bitmap.buffer != NULL && slot->format == FT_GLYPH_FORMAT_BITMAP)
		{
			ALLOC_WRITER(15 * 4);
			WRITE_INT(15);
			WRITE_INT(0);
			WRITE_INT(0);
			WRITE_INT(slot->metrics.width);
			WRITE_INT(slot->metrics.height);

			WRITE_INT(slot->metrics.width);
			WRITE_INT(slot->metrics.height);

			WRITE_INT(slot->metrics.horiAdvance);
			WRITE_INT(slot->metrics.horiBearingX);
			WRITE_INT(slot->metrics.horiBearingY);

			WRITE_INT(slot->metrics.vertAdvance);
			WRITE_INT(slot->metrics.vertBearingX);
			WRITE_INT(slot->metrics.vertBearingY);

			WRITE_INT(slot->linearHoriAdvance);
			WRITE_INT(slot->linearVertAdvance);
			return true;
		}

		FT_Glyph glyph;
		FT_Get_Glyph(slot, &glyph);
		if (!glyph)
			return false;

		FT_BBox bbox;
		FT_Glyph_Get_CBox(glyph, 1, &bbox);

		if (isVector)
		{
			FT_Decompose_Outline_Buffer buffer;
			FT_Decompose_Outline_Buffer_Init(&buffer);

			static FT_Outline_Funcs pOutlineFuncs =
			{
				&GlyphPathMoveTo,
				&GlyphPathLineTo,
				&GlyphPathConicTo,
				&GlyphPathCubicTo,
				0, 0
			};

			FT_Outline_Decompose(&((FT_OutlineGlyph)glyph)->outline, &pOutlineFuncs, &buffer);

			int nCount = buffer.pos;

			ALLOC_WRITER((nCount + 15) * 4);
			WRITE_INT((nCount + 15));
			WRITE_INT(bbox.xMin);
			WRITE_INT(bbox.yMin);
			WRITE_INT(bbox.xMax);
			WRITE_INT(bbox.yMax);

			WRITE_INT(slot->metrics.width);
			WRITE_INT(slot->metrics.height);

			WRITE_INT(slot->metrics.horiAdvance);
			WRITE_INT(slot->metrics.horiBearingX);
			WRITE_INT(slot->metrics.horiBearingY);

			WRITE_INT(slot->metrics.vertAdvance);
			WRITE_INT(slot->metrics.vertBearingX);
			WRITE_INT(slot->metrics.vertBearingY);

			WRITE_INT(slot->linearHoriAdvance);
			WRITE_INT(slot->linearVertAdvance);

			for (int i = 0; i < nCount; i++)
			{
				WRITE_INT(buffer.buffer[i]);
			}

			FT_Decompose_Outline_Buffer_Destroy(&buffer);
		}
		else
		{
			ALLOC_WRITER(15 * 4);
			WRITE_INT(15);
			WRITE_INT(bbox.xMin);
			WRITE_INT(bbox.yMin);
			WRITE_INT(bbox.xMax);
			WRITE_INT(bbox.yMax);

			WRITE_INT(slot->metrics.width);
			WRITE_INT(slot->metrics.height);

			WRITE_INT(slot->metrics.horiAdvance);
			WRITE_INT(slot->metrics.horiBearingX);
			WRITE_INT(slot->metrics.horiBearingY);

			WRITE_INT(slot->metrics.vertAdvance);
			WRITE_INT(slot->metrics.vertBearingX);
			WRITE_INT(slot->metrics.vertBearingY);

			WRITE_INT(slot->linearHoriAdvance);
			WRITE_INT(slot->linearVertAdvance);
		}

		FT_Done_Glyph(glyph);
		return true;
	}
}

#ifdef SUPPORT_HARFBUZZ_SHAPER

#include <hb.h>
#include <hb-ft.h>
#include <hb-ot.h>

namespace NSShaper
{
#define g_userfeatures_count 5
	hb_feature_t g_userfeatures[g_userfeatures_count];
	bool g_userfeatures_init = false;

	void* HB_LanguageFromString(const std::string language_bcp_47)
	{
		return (void*)hb_language_from_string(language_bcp_47.c_str(), language_bcp_47.length());
	}

	static void _hb_ft_face_destroy_js(void *data)
	{
		FT_Done_Face_With_Library(data);
	}

	inline void HB_ShapeTextRaw(void* face, void*& font, char* text_str, const size_t& text_length,
								unsigned int nFeatures, unsigned int nScript, unsigned int nDirection, void* nLanguage,
								CExternalPointer* result, bool bIsJSVersion)
	{
		// init features
		if (!g_userfeatures_init)
		{
			hb_tag_t tags[] = {
				HB_TAG('l','i','g','a'),
				HB_TAG('c','l','i','g'),
				HB_TAG('h','l','i','g'),
				HB_TAG('d','l','i','g'),
				HB_TAG('k','e','r','n')
			};
			for (int nTag = 0; nTag < g_userfeatures_count; ++nTag)
			{
				g_userfeatures[nTag].tag = tags[nTag];
				g_userfeatures[nTag].value = 0;
				g_userfeatures[nTag].start = HB_FEATURE_GLOBAL_START;
				g_userfeatures[nTag].end = HB_FEATURE_GLOBAL_END;
			}
			g_userfeatures_init = true;
		}

		// font
		hb_font_t* pFont;
		if (NULL == font)
		{
			if (!bIsJSVersion)
			{
				pFont = hb_ft_font_create_referenced((FT_Face)face);
			}
			else
			{
				FT_Reference_Face((FT_Face)face);
				pFont = hb_ft_font_create((FT_Face)face, _hb_ft_face_destroy_js);
			}
			hb_ft_font_set_funcs(pFont);
			font = (void*)pFont;
		}
		else
			pFont = (hb_font_t*)font;

		// features
		for (int nTag = 0; nTag < g_userfeatures_count; ++nTag)
			g_userfeatures[nTag].value = (nFeatures & (1 << nTag)) ? 1 : 0;

		// buffer
		hb_buffer_t* hbBuffer = hb_buffer_create();
		hb_buffer_set_direction(hbBuffer, (hb_direction_t)nDirection);
		hb_buffer_set_script(hbBuffer, (hb_script_t)nScript);
		hb_buffer_set_language(hbBuffer, (hb_language_t)nLanguage);
		hb_buffer_set_cluster_level(hbBuffer, HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES);
		int text_len = (int)text_length;
		hb_buffer_add_utf8(hbBuffer, text_str, text_len, 0, text_len);
		hb_buffer_guess_segment_properties(hbBuffer);

		// shape
		hb_shape(pFont, hbBuffer, g_userfeatures, g_userfeatures_count);

		unsigned int glyph_count;
		hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(hbBuffer, &glyph_count);
		hb_glyph_position_t* glyph_pos = hb_buffer_get_glyph_positions(hbBuffer, &glyph_count);

		int nSize = 4 + 8 + glyph_count * (1 + 1 + 4 * 6);

		ALLOC_WRITER(nSize);

		WRITE_UINT(nSize);

		uint64_t pFontPointer = (uint64_t)pFont;
		WRITE_UINT(pFontPointer & 0xFFFFFFFF);
		WRITE_UINT((pFontPointer >> 32) & 0xFFFFFFFF);

		for (unsigned i = 0; i < glyph_count; ++i)
		{
			unsigned char nGlyphType = (unsigned char)hb_ot_layout_get_glyph_class(hb_font_get_face(pFont), glyph_info[i].codepoint);
			unsigned char nGlyphFlags = (unsigned char)hb_glyph_info_get_glyph_flags(&glyph_info[i]);

			WRITE_UCHAR(nGlyphType);
			WRITE_UCHAR(nGlyphFlags);

			WRITE_UINT(glyph_info[i].codepoint);
			WRITE_UINT(glyph_info[i].cluster);

			WRITE_INT(glyph_pos[i].x_advance);
			WRITE_INT(glyph_pos[i].y_advance);
			WRITE_INT(glyph_pos[i].x_offset);
			WRITE_INT(glyph_pos[i].y_offset);
		}

		hb_buffer_destroy(hbBuffer);
	}

	void HB_ShapeText(void* face, void*& font, char* text,
					  unsigned int nFeatures, unsigned int nScript, unsigned int nDirection, void* nLanguage,
					  CExternalPointer* result, bool bIsJSVersion)
	{
		HB_ShapeTextRaw(face, font, text, strlen(text), nFeatures, nScript, nDirection, nLanguage, result, bIsJSVersion);
	}

	void HB_ShapeText(void* face, void*& font, const std::string& text,
					  unsigned int nFeatures, unsigned int nScript, unsigned int nDirection, void* nLanguage,
					  CExternalPointer* result, bool bIsJSVersion)
	{
		HB_ShapeTextRaw(face, font, (char*)text.c_str(), text.length(), nFeatures, nScript, nDirection, nLanguage, result, bIsJSVersion);
	}

	void HB_FontFree(void* font)
	{
		if (NULL == font)
			return;
		hb_font_destroy((hb_font_t*)font);
	}
}
#endif
