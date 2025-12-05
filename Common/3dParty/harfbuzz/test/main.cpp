// https://android.googlesource.com/platform/external/harfbuzz/+/ics-mr0/contrib/tables/script-properties.h

/*
 * https://unicode.org/reports/tr29/

As far as a user is concerned, the underlying representation of text is not important,
but it is important that an editing interface present a uniform implementation of what
the user thinks of as characters. Grapheme clusters can be treated as units, by default,
for processes such as the formatting of drop caps, as well as the implementation of text
selection, arrow key movement or backspacing through text, and so forth. For example,
when a grapheme cluster is represented internally by a character sequence consisting of
base character + accents, then using the right arrow key would skip from the start of the
base character to the end of the last accent.

This document defines a default specification for grapheme clusters. It may be customized
 for particular languages, operations, or other situations. For example, arrow key movement
could be tailored by language, or could use knowledge specific to particular fonts to move
in a more granular manner, in circumstances where it would be useful to edit individual
components. This could apply, for example, to the complex editorial requirements for the
Northern Thai script Tai Tham (Lanna). Similarly, editing a grapheme cluster element by
element may be preferable in some circumstances. For example, on a given system the backspace
key might delete by code point, while the delete key may delete an entire cluster.
 * */

#include "../../../core/DesktopEditor/common/File.h"
#include "../../../core/DesktopEditor/raster/BgraFrame.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

#include <hb-ft.h>
#include <hb-ot.h>
#include <hb.h>

class CDrawer
{
public:
	CBgraFrame m_oFrame;
	BYTE *pixels;
	int width;
	int height;
	int pitch;
	BYTE Rshift;
	BYTE Gshift;
	BYTE Bshift;
	BYTE Ashift;

public:
	CDrawer(int w, int h)
	{
		width = w;
		height = h;
		pitch = 4 * width;
		m_oFrame.put_Width(width);
		m_oFrame.put_Height(height);
		m_oFrame.put_Stride(pitch);

		int size = 4 * width * height;
		BYTE *pPixels = new BYTE[size];
		for (int i = 0; i < size; i += 4)
		{
			pPixels[i] = 0xFF;
			pPixels[i + 1] = 0xFF;
			pPixels[i + 2] = 0xFF;
			pPixels[i + 3] = 0xFF;
		}
		pixels = pPixels;
		m_oFrame.put_Data(pPixels);

		Bshift = 24;
		Gshift = 16;
		Rshift = 8;
		Ashift = 0;
	}
	void Save()
	{
		m_oFrame.SaveFile(NSFile::GetProcessDirectory() + L"/output.png", 4);
	}
};

#define NUM_EXAMPLES 3

/* fonts */
const char *fonts_paths[NUM_EXAMPLES] = {
	"C:/Windows/Fonts/calibri.ttf",
	//"C:/Windows/Fonts/arial.ttf",
	"C:/Users/korol/AppData/Local/Microsoft/Windows/Fonts/ArabicTest.ttf",
	"C:/Windows/Fonts/simsun.ttc"
};

#define NUM_GLYPH_TYPES 5
const char *num_glyph_types[NUM_GLYPH_TYPES] = {"UNCLASSIFIED", "BASE_GLYPH", "LIGATURE", "MARK", "COMPONENT"};

/* tranlations courtesy of google */
const char *texts[NUM_EXAMPLES] = {
	"fi",
	"لا لآ لأ لا",
	"懶惰的姜貓"
};

const hb_direction_t text_directions[NUM_EXAMPLES] = {
	HB_DIRECTION_LTR,
	HB_DIRECTION_RTL,
	HB_DIRECTION_TTB,
};

const int text_skip[NUM_EXAMPLES] = {
	0,
	0,
	1,
};

/* XXX: These are not correct, though it doesn't seem to break anything
 *      regardless of their value. */
const char *languages[NUM_EXAMPLES] = {
	"en",
	"ar",
	"ch",
};

const hb_script_t scripts[NUM_EXAMPLES] = {
	HB_SCRIPT_LATIN,
	HB_SCRIPT_ARABIC,
	HB_SCRIPT_HAN,
};

enum
{
	ENGLISH = 0,
	ARABIC,
	CHINESE
};

typedef struct _spanner_baton_t
{
	/* rendering part - assumes 32bpp surface */
	uint32_t *pixels;					// set to the glyph's origin.
	uint32_t *first_pixel, *last_pixel; // bounds check
	uint32_t pitch;
	uint32_t rshift;
	uint32_t gshift;
	uint32_t bshift;
	uint32_t ashift;

	/* sizing part */
	int min_span_x;
	int max_span_x;
	int min_y;
	int max_y;
} spanner_baton_t;

/* This spanner is write only, suitable for write-only mapped buffers,
   but can cause dark streaks where glyphs overlap, like in arabic scripts.

   Note how spanners don't clip against surface width - resize the window
   and see what it leads to. */
void spanner_wo(int y, int count, const FT_Span *spans, void *user)
{
	spanner_baton_t *baton = (spanner_baton_t *)user;
	uint32_t *scanline = baton->pixels - y * ((int)baton->pitch / 4);
	if (scanline < baton->first_pixel)
		return;
	for (int i = 0; i < count; i++)
	{
		uint32_t color = ((spans[i].coverage / 2) << baton->rshift) | ((spans[i].coverage / 2) << baton->gshift) | ((spans[i].coverage / 2) << baton->bshift);

		uint32_t *start = scanline + spans[i].x;
		if (start + spans[i].len > baton->last_pixel)
			return;

		for (int x = 0; x < spans[i].len; x++)
			*start++ = color;
	}
}

/* This spanner does read/modify/write, trading performance for accuracy.
   The color here is simply half coverage value in all channels,
   effectively mid-gray.
   Suitable for when artifacts mostly do come up and annoy.
   This might be optimized if one does rmw only for some values of x.
   But since the whole buffer has to be rw anyway, and the previous value
   is probably still in the cache, there's little point to. */
void spanner_rw(int y, int count, const FT_Span *spans, void *user)
{
	spanner_baton_t *baton = (spanner_baton_t *)user;
	uint32_t *scanline = baton->pixels - y * ((int)baton->pitch / 4);
	if (scanline < baton->first_pixel)
		return;

	for (int i = 0; i < count; i++)
	{
		uint32_t color = ((spans[i].coverage / 2) << baton->rshift) | ((spans[i].coverage / 2) << baton->gshift) | ((spans[i].coverage / 2) << baton->bshift);
		uint32_t *start = scanline + spans[i].x;
		if (start + spans[i].len > baton->last_pixel)
			return;

		for (int x = 0; x < spans[i].len; x++)
			*start++ |= color;
	}
}

/*  This spanner is for obtaining exact bounding box for the string.
	Unfortunately this can't be done without rendering it (or pretending to).
	After this runs, we get min and max values of coordinates used.
*/
void spanner_sizer(int y, int count, const FT_Span *spans, void *user)
{
	spanner_baton_t *baton = (spanner_baton_t *)user;

	if (y < baton->min_y)
		baton->min_y = y;
	if (y > baton->max_y)
		baton->max_y = y;
	for (int i = 0; i < count; i++)
	{
		if (spans[i].x + spans[i].len > baton->max_span_x)
			baton->max_span_x = spans[i].x + spans[i].len;
		if (spans[i].x < baton->min_span_x)
			baton->min_span_x = spans[i].x;
	}
}

FT_SpanFunc spanner = spanner_wo;

void ftfdump(FT_Face ftf)
{
	for (int i = 0; i < ftf->num_charmaps; i++)
	{
		printf(
			"%d: %s %s %c%c%c%c plat=%hu id=%hu\n", i, ftf->family_name, ftf->style_name, ftf->charmaps[i]->encoding >> 24, (ftf->charmaps[i]->encoding >> 16) & 0xff,
			(ftf->charmaps[i]->encoding >> 8) & 0xff, (ftf->charmaps[i]->encoding) & 0xff, ftf->charmaps[i]->platform_id, ftf->charmaps[i]->encoding_id);
	}
}

/*  See http://www.microsoft.com/typography/otspec/name.htm
	for a list of some possible platform-encoding pairs.
	We're interested in 0-3 aka 3-1 - UCS-2.
	Otherwise, fail. If a font has some unicode map, but lacks
	UCS-2 - it is a broken or irrelevant font. What exactly
	Freetype will select on face load (it promises most wide
	unicode, and if that will be slower that UCS-2 - left as
	an excercise to check. */
int force_ucs2_charmap(FT_Face ftf)
{
	for (int i = 0; i < ftf->num_charmaps; i++)
		if (((ftf->charmaps[i]->platform_id == 0) && (ftf->charmaps[i]->encoding_id == 3)) || ((ftf->charmaps[i]->platform_id == 3) && (ftf->charmaps[i]->encoding_id == 1)))
			return FT_Set_Charmap(ftf, ftf->charmaps[i]);
	return -1;
}

void hline(CDrawer *s, int min_x, int max_x, int y, uint32_t color)
{
	if (y < 0)
		y = 0;
	uint32_t *pix = (uint32_t *)s->pixels + (y * s->pitch) / 4 + min_x;
	uint32_t *end = (uint32_t *)s->pixels + (y * s->pitch) / 4 + max_x;

	while (pix - 1 != end)
		*pix++ = color;
}

void vline(CDrawer *s, int min_y, int max_y, int x, uint32_t color)
{
	if (min_y < 0)
		min_y = 0;

	uint32_t *pix = (uint32_t *)s->pixels + (min_y * s->pitch) / 4 + x;
	uint32_t *end = (uint32_t *)s->pixels + (max_y * s->pitch) / 4 + x;

	while (pix - s->pitch / 4 != end)
	{
		*pix = color;
		pix += s->pitch / 4;
	}
}

void assert(const bool &valid)
{
	// TODO:
}

#define MAIN_CC_NO_PRIVATE_API
#ifndef MAIN_CC_NO_PRIVATE_API
/* Only this part of this mini app uses private API */
#include "hb-open-file.hh"
#include "hb-ot-layout-gdef-table.hh"
#include "hb-ot-layout-gsubgpos.hh"
#include "hb-static.cc"

using namespace OT;

static void print_layout_info_using_private_api(hb_blob_t *blob)
{
	const char *font_data = hb_blob_get_data(blob, nullptr);
	hb_blob_t *font_blob = hb_sanitize_context_t().sanitize_blob<OpenTypeFontFile>(blob);
	const OpenTypeFontFile *sanitized = font_blob->as<OpenTypeFontFile>();
	if (!font_blob->data)
	{
		printf("Sanitization of the file wasn't successful. Exit");
		exit(1);
	}
	const OpenTypeFontFile &ot = *sanitized;

	switch (ot.get_tag())
	{
	case OpenTypeFontFile::TrueTypeTag:
		printf("OpenType font with TrueType outlines\n");
		break;
	case OpenTypeFontFile::CFFTag:
		printf("OpenType font with CFF (Type1) outlines\n");
		break;
	case OpenTypeFontFile::TTCTag:
		printf("TrueType Collection of OpenType fonts\n");
		break;
	case OpenTypeFontFile::TrueTag:
		printf("Obsolete Apple TrueType font\n");
		break;
	case OpenTypeFontFile::Typ1Tag:
		printf("Obsolete Apple Type1 font in SFNT container\n");
		break;
	case OpenTypeFontFile::DFontTag:
		printf("DFont Mac Resource Fork\n");
		break;
	default:
		printf("Unknown font format\n");
		break;
	}

	unsigned num_faces = hb_face_count(blob);
	printf("%d font(s) found in file\n", num_faces);
	for (unsigned n_font = 0; n_font < num_faces; ++n_font)
	{
		const OpenTypeFontFace &font = ot.get_face(n_font);
		printf("Font %d of %d:\n", n_font, num_faces);

		unsigned num_tables = font.get_table_count();
		printf("  %d table(s) found in font\n", num_tables);
		for (unsigned n_table = 0; n_table < num_tables; ++n_table)
		{
			const OpenTypeTable &table = font.get_table(n_table);
			printf("  Table %2d of %2d: %.4s (0x%08x+0x%08x)\n", n_table, num_tables, (const char *)table.tag, (unsigned)table.offset, (unsigned)table.length);

			switch (table.tag)
			{

			case HB_OT_TAG_GSUB:
			case HB_OT_TAG_GPOS:
			{

				const GSUBGPOS &g = *reinterpret_cast<const GSUBGPOS *>(font_data + table.offset);

				unsigned num_scripts = g.get_script_count();
				printf("    %d script(s) found in table\n", num_scripts);
				for (unsigned n_script = 0; n_script < num_scripts; ++n_script)
				{
					const Script &script = g.get_script(n_script);
					printf("    Script %2d of %2d: %.4s\n", n_script, num_scripts, (const char *)g.get_script_tag(n_script));

					if (!script.has_default_lang_sys())
						printf("      No default language system\n");
					int num_langsys = script.get_lang_sys_count();
					printf("      %d language system(s) found in script\n", num_langsys);
					for (int n_langsys = script.has_default_lang_sys() ? -1 : 0; n_langsys < num_langsys; ++n_langsys)
					{
						const LangSys &langsys = n_langsys == -1 ? script.get_default_lang_sys() : script.get_lang_sys(n_langsys);
						if (n_langsys == -1)
							printf("      Default Language System\n");
						else
							printf("      Language System %2d of %2d: %.4s\n", n_langsys, num_langsys, (const char *)script.get_lang_sys_tag(n_langsys));
						if (!langsys.has_required_feature())
							printf("        No required feature\n");
						else
							printf("        Required feature index: %d\n", langsys.get_required_feature_index());

						unsigned num_features = langsys.get_feature_count();
						printf("        %d feature(s) found in language system\n", num_features);
						for (unsigned n_feature = 0; n_feature < num_features; ++n_feature)
						{
							printf("        Feature index %2d of %2d: %d\n", n_feature, num_features, langsys.get_feature_index(n_feature));
						}
					}
				}

				unsigned num_features = g.get_feature_count();
				printf("    %d feature(s) found in table\n", num_features);
				for (unsigned n_feature = 0; n_feature < num_features; ++n_feature)
				{
					const Feature &feature = g.get_feature(n_feature);
					unsigned num_lookups = feature.get_lookup_count();
					printf("    Feature %2d of %2d: %c%c%c%c\n", n_feature, num_features, HB_UNTAG(g.get_feature_tag(n_feature)));

					printf("        %d lookup(s) found in feature\n", num_lookups);
					for (unsigned n_lookup = 0; n_lookup < num_lookups; ++n_lookup)
					{
						printf("        Lookup index %2d of %2d: %d\n", n_lookup, num_lookups, feature.get_lookup_index(n_lookup));
					}
				}

				unsigned num_lookups = g.get_lookup_count();
				printf("    %d lookup(s) found in table\n", num_lookups);
				for (unsigned n_lookup = 0; n_lookup < num_lookups; ++n_lookup)
				{
					const Lookup &lookup = g.get_lookup(n_lookup);
					printf("    Lookup %2d of %2d: type %d, props 0x%04X\n", n_lookup, num_lookups, lookup.get_type(), lookup.get_props());
				}
			}
			break;

			case GDEF::tableTag:
			{

				const GDEF &gdef = *reinterpret_cast<const GDEF *>(font_data + table.offset);

				printf("    Has %sglyph classes\n", gdef.has_glyph_classes() ? "" : "no ");
				printf("    Has %smark attachment types\n", gdef.has_mark_attachment_types() ? "" : "no ");
				printf("    Has %sattach points\n", gdef.has_attach_points() ? "" : "no ");
				printf("    Has %slig carets\n", gdef.has_lig_carets() ? "" : "no ");
				printf("    Has %smark sets\n", gdef.has_mark_sets() ? "" : "no ");

				hb_position_t caret_array[16];
				unsigned int caret_count = 16;

				unsigned int num_carets = gdef.get_lig_carets(nullptr, HB_DIRECTION_LTR, 302, 0, &caret_count, caret_array);
				int y = 0;
				++y;

				break;
			}
			}
		}
	}
}
/* end of private API use */
#endif

struct hb_feature_test {
	hb_tag_t tag;
	uint32_t value;
};

int main(int argc, char *argv[])
{
	// hb_blob_t* blobFileTest = hb_blob_create_from_file("C:/Windows/Fonts/calibri.ttf");
	// print_layout_info_using_private_api(blobFileTest);

	int ptSize = 40 * 64;
	int device_hdpi = 72;
	int device_vdpi = 72;

	/* Init freetype */
	FT_Library ft_library;
	assert(!FT_Init_FreeType(&ft_library));

	/* Load our fonts */
	FT_Face ft_face[NUM_EXAMPLES];
	assert(!FT_New_Face(ft_library, fonts_paths[0], 0, &ft_face[ENGLISH]));
	assert(!FT_Set_Char_Size(ft_face[ENGLISH], 0, ptSize, device_hdpi, device_vdpi));
	ftfdump(ft_face[ENGLISH]); // wonderful world of encodings ...
	// force_ucs2_charmap(ft_face[ENGLISH]); // which we ignore.

	assert(!FT_New_Face(ft_library, fonts_paths[1], 0, &ft_face[ARABIC]));
	assert(!FT_Set_Char_Size(ft_face[ARABIC], 0, ptSize, device_hdpi, device_vdpi));
	ftfdump(ft_face[ARABIC]);
	// force_ucs2_charmap(ft_face[ARABIC]);

	assert(!FT_New_Face(ft_library, fonts_paths[2], 0, &ft_face[CHINESE]));
	assert(!FT_Set_Char_Size(ft_face[CHINESE], 0, ptSize, device_hdpi, device_vdpi));
	ftfdump(ft_face[CHINESE]);
	// force_ucs2_charmap(ft_face[CHINESE]);

	/* Get our harfbuzz font structs */
	hb_font_t *hb_ft_font[NUM_EXAMPLES];
	hb_ft_font[ENGLISH] = hb_ft_font_create(ft_face[ENGLISH], NULL);

	// hb_blob_t* blobFile = hb_blob_create_from_file(sFont1.c_str());
	// hb_face_t* faceFile = hb_face_create(blobFile, 0);
	// hb_ft_font[ENGLISH] = hb_font_create(faceFile);

	hb_ft_font[ARABIC] = hb_ft_font_create(ft_face[ARABIC], NULL);
	hb_ft_font[CHINESE] = hb_ft_font_create(ft_face[CHINESE], NULL);

	hb_ft_font_set_funcs(hb_ft_font[ENGLISH]);
	hb_ft_font_set_funcs(hb_ft_font[ARABIC]);
	hb_ft_font_set_funcs(hb_ft_font[CHINESE]);

	/** Setup our SDL window **/
	int width = 800;
	int height = 600;
	int bpp = 32;

	CDrawer oDrawer(width, height);

	/* Create a buffer for harfbuzz to use */
	hb_buffer_t *buf = hb_buffer_create();
	for (int i = 0; i < NUM_EXAMPLES; ++i)
	{
		if (text_skip[i])
			continue;

		hb_buffer_set_direction(buf, text_directions[i]); /* or LTR */
		hb_buffer_set_script(buf, scripts[i]);			  /* see hb-unicode.h */
		hb_buffer_set_language(buf, hb_language_from_string(languages[i], strlen(languages[i])));
		// hb_buffer_set_cluster_level (buf, HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES);
		// hb_buffer_set_cluster_level (buf, HB_BUFFER_CLUSTER_LEVEL_MONOTONE_CHARACTERS);
		hb_buffer_set_cluster_level(buf, HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES);

		hb_feature_test features[] {
			{HB_TAG('r','l','i','g'), 1},
			{HB_TAG('l','i','g','a'), 0},
			{HB_TAG('c','l','i','g'), 1},
			{HB_TAG('h','l','i','g'), 1},
			{HB_TAG('d','l','i','g'), 1},
			{HB_TAG('k','e','r','n'), 2},
			{0, 0}
		};

		int userfeatures_count = 0;
		hb_feature_t userfeatures[100];

		hb_feature_test* current_feature = features;
		while (current_feature->tag != 0)
		{
			if (current_feature->value != 2)
			{
				userfeatures[userfeatures_count].tag   = current_feature->tag;
				userfeatures[userfeatures_count].value = current_feature->value;
				userfeatures[userfeatures_count].start = HB_FEATURE_GLOBAL_START;
				userfeatures[userfeatures_count].end   = HB_FEATURE_GLOBAL_END;
				userfeatures_count++;
			}
			current_feature++;
		}

		/* Layout the text */
		hb_buffer_add_utf8(buf, texts[i], strlen(texts[i]), 0, strlen(texts[i]));

		// detect script by codes
		hb_buffer_guess_segment_properties(buf);

		// const char*const pHbShapers[] = { "graphite2", "coretext_aat", "ot", "fallback", nullptr };
		// bool ok = hb_shape_full(hb_ft_font[i], buf, userfeatures, userfeatures_count, pHbShapers);

		hb_shape(hb_ft_font[i], buf, (userfeatures_count != 0) ? userfeatures : NULL, userfeatures_count);

		unsigned int glyph_count;
		hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
		hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);

#if 1
		hb_position_t caret_array[16];
		unsigned int caret_count = 16;

		unsigned int num_carets = hb_ot_layout_get_ligature_carets(hb_ft_font[i], text_directions[i], glyph_info[0].codepoint, -1, &caret_count, caret_array);
#endif

		/* set up rendering via spanners */
		spanner_baton_t stuffbaton;

		FT_Raster_Params ftr_params;
		ftr_params.target = 0;
		ftr_params.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
		ftr_params.user = &stuffbaton;
		ftr_params.black_spans = 0;
		ftr_params.bit_set = 0;
		ftr_params.bit_test = 0;

		/* Calculate string bounding box in pixels */
		ftr_params.gray_spans = spanner_sizer;

		/* See http://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html */

		int max_x = INT_MIN; // largest coordinate a pixel has been set at, or the pen was advanced to.
		int min_x = INT_MAX; // smallest coordinate a pixel has been set at, or the pen was advanced to.
		int max_y = INT_MIN; // this is max topside bearing along the string.
		int min_y = INT_MAX; // this is max value of (height - topbearing) along the string.
		/*  Naturally, the above comments swap their meaning between horizontal and vertical scripts,
			since the pen changes the axis it is advanced along.
			However, their differences still make up the bounding box for the string.
			Also note that all this is in FT coordinate system where y axis points upwards.
		 */

		int sizer_x = 0;
		int sizer_y = 0; /* in FT coordinate system. */

		printf("----------------------------------------------------\n");
		for (unsigned j = 0; j < glyph_count; ++j)
		{
			hb_ot_layout_glyph_class_t glyph_type = hb_ot_layout_get_glyph_class(hb_font_get_face(hb_ft_font[i]), glyph_info[j].codepoint);
			hb_glyph_flags_t glyph_type_flags = hb_glyph_info_get_glyph_flags(&glyph_info[j]);
			printf(
				"glyph(%s, flags: %d): gid:%d, cluster:%d, [%d, %d, %d, %d, %d]\n", num_glyph_types[glyph_type], glyph_type_flags, (int)glyph_info[j].codepoint, (int)glyph_info[j].cluster,
				glyph_pos[j].x_advance, glyph_pos[j].y_advance, glyph_pos[j].x_offset, glyph_pos[j].y_offset, glyph_pos[j].var);
		}

		FT_Error fterr;
		for (unsigned j = 0; j < glyph_count; ++j)
		{
			if ((fterr = FT_Load_Glyph(ft_face[i], glyph_info[j].codepoint, 0)))
			{
				printf("load %08x failed fterr=%d.\n", glyph_info[j].codepoint, fterr);
			}
			else
			{
				if (ft_face[i]->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
				{
					printf("glyph->format = %4s\n", (char *)&ft_face[i]->glyph->format);
				}
				else
				{
					int gx = sizer_x + (glyph_pos[j].x_offset / 64);
					int gy = sizer_y + (glyph_pos[j].y_offset / 64); // note how the sign differs from the rendering pass

					stuffbaton.min_span_x = INT_MAX;
					stuffbaton.max_span_x = INT_MIN;
					stuffbaton.min_y = INT_MAX;
					stuffbaton.max_y = INT_MIN;

					if ((fterr = FT_Outline_Render(ft_library, &ft_face[i]->glyph->outline, &ftr_params)))
						printf("FT_Outline_Render() failed err=%d\n", fterr);

					if (stuffbaton.min_span_x != INT_MAX)
					{
						/* Update values if the spanner was actually called. */
						if (min_x > stuffbaton.min_span_x + gx)
							min_x = stuffbaton.min_span_x + gx;

						if (max_x < stuffbaton.max_span_x + gx)
							max_x = stuffbaton.max_span_x + gx;

						if (min_y > stuffbaton.min_y + gy)
							min_y = stuffbaton.min_y + gy;

						if (max_y < stuffbaton.max_y + gy)
							max_y = stuffbaton.max_y + gy;
					}
					else
					{
						/* The spanner wasn't called at all - an empty glyph, like space. */
						if (min_x > gx)
							min_x = gx;
						if (max_x < gx)
							max_x = gx;
						if (min_y > gy)
							min_y = gy;
						if (max_y < gy)
							max_y = gy;
					}
				}
			}

			sizer_x += glyph_pos[j].x_advance / 64;
			sizer_y += glyph_pos[j].y_advance / 64; // note how the sign differs from the rendering pass
		}
		/* Still have to take into account last glyph's advance. Or not? */
		if (min_x > sizer_x)
			min_x = sizer_x;
		if (max_x < sizer_x)
			max_x = sizer_x;
		if (min_y > sizer_y)
			min_y = sizer_y;
		if (max_y < sizer_y)
			max_y = sizer_y;

		/* The bounding box */
		int bbox_w = max_x - min_x;
		int bbox_h = max_y - min_y;

		/* Two offsets below position the bounding box with respect to the 'origin',
			which is sort of origin of string's first glyph.

			baseline_offset - offset perpendecular to the baseline to the topmost (horizontal),
							  or leftmost (vertical) pixel drawn.

			baseline_shift  - offset along the baseline, from the first drawn glyph's origin
							  to the leftmost (horizontal), or topmost (vertical) pixel drawn.

			Thus those offsets allow positioning the bounding box to fit the rendered string,
			as they are in fact offsets from the point given to the renderer, to the top left
			corner of the bounding box.

			NB: baseline is defined as y==0 for horizontal and x==0 for vertical scripts.
			(0,0) here is where the first glyph's origin ended up after shaping, not taking
			into account glyph_pos[0].xy_offset (yeah, my head hurts too).
		*/

		int baseline_offset;
		int baseline_shift;

		if (HB_DIRECTION_IS_HORIZONTAL(hb_buffer_get_direction(buf)))
		{
			baseline_offset = max_y;
			baseline_shift = min_x;
		}
		if (HB_DIRECTION_IS_VERTICAL(hb_buffer_get_direction(buf)))
		{
			baseline_offset = min_x;
			baseline_shift = max_y;
		}

		/* The pen/baseline start coordinates in window coordinate system
			- with those text placement in the window is controlled.
			- note that for RTL scripts pen still goes LTR */
		int x = 0, y = 50 + i * 75;
		if (i == ENGLISH)
		{
			x = 20;
		} /* left justify */
		if (i == ARABIC)
		{
			x = width - bbox_w - 20;
		} /* right justify */
		if (i == CHINESE)
		{
			x = width / 2 - bbox_w / 2;
		} /* center, and for TTB script h_advance is half-width. */

		/* Draw baseline and the bounding box */
		/* The below is complicated since we simultaneously
		   convert to the window coordinate system. */
		int left, right, top, bottom;

		if (HB_DIRECTION_IS_HORIZONTAL(hb_buffer_get_direction(buf)))
		{
			/* bounding box in window coordinates without offsets */
			left = x;
			right = x + bbox_w;
			top = y - bbox_h;
			bottom = y;

			/* apply offsets */
			left += baseline_shift;
			right += baseline_shift;
			top -= baseline_offset - bbox_h;
			bottom -= baseline_offset - bbox_h;

			/* draw the baseline */
			hline(&oDrawer, x, x + bbox_w, y, 0x0000ff00);
		}

		if (HB_DIRECTION_IS_VERTICAL(hb_buffer_get_direction(buf)))
		{
			left = x;
			right = x + bbox_w;
			top = y;
			bottom = y + bbox_h;

			left += baseline_offset;
			right += baseline_offset;
			top -= baseline_shift;
			bottom -= baseline_shift;

			vline(&oDrawer, y, y + bbox_h, x, 0x0000ff00);
		}

		/* +1/-1 are for the bbox borders be the next pixel outside the bbox itself */
		hline(&oDrawer, left - 1, right + 1, top - 1, 0xffff0000);
		hline(&oDrawer, left - 1, right + 1, bottom + 1, 0xffff0000);
		vline(&oDrawer, top - 1, bottom + 1, left - 1, 0xffff0000);
		vline(&oDrawer, top - 1, bottom + 1, right + 1, 0xffff0000);

		/* set rendering spanner */
		ftr_params.gray_spans = spanner;

		/* initialize rendering part of the baton */
		stuffbaton.pixels = NULL;
		stuffbaton.first_pixel = (uint32_t *)oDrawer.pixels;
		stuffbaton.last_pixel = (uint32_t *)(((uint8_t *)oDrawer.pixels) + oDrawer.pitch * oDrawer.height);
		stuffbaton.pitch = oDrawer.pitch;
		stuffbaton.rshift = oDrawer.Rshift;
		stuffbaton.gshift = oDrawer.Gshift;
		stuffbaton.bshift = oDrawer.Bshift;

		/* render */
		for (unsigned j = 0; j < glyph_count; ++j)
		{
			if ((fterr = FT_Load_Glyph(ft_face[i], glyph_info[j].codepoint, 0)))
			{
				printf("load %08x failed fterr=%d.\n", glyph_info[j].codepoint, fterr);
			}
			else
			{
				if (ft_face[i]->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
				{
					printf("glyph->format = %4s\n", (char *)&ft_face[i]->glyph->format);
				}
				else
				{
					int gx = x + (glyph_pos[j].x_offset / 64);
					int gy = y - (glyph_pos[j].y_offset / 64);

					stuffbaton.pixels = (uint32_t *)(((uint8_t *)oDrawer.pixels) + gy * oDrawer.pitch) + gx;

					if ((fterr = FT_Outline_Render(ft_library, &ft_face[i]->glyph->outline, &ftr_params)))
						printf("FT_Outline_Render() failed err=%d\n", fterr);
				}
			}

			x += glyph_pos[j].x_advance / 64;
			y -= glyph_pos[j].y_advance / 64;
		}

		/* clean up the buffer, but don't kill it just yet */
		hb_buffer_clear_contents(buf);
	}

	/* Cleanup */
	hb_buffer_destroy(buf);
	for (int i = 0; i < NUM_EXAMPLES; ++i)
		hb_font_destroy(hb_ft_font[i]);

	FT_Done_FreeType(ft_library);

	oDrawer.Save();

	return 0;
}
