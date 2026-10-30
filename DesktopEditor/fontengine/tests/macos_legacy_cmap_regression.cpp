// SPDX-License-Identifier: AGPL-3.0-only
#include <cstdio>
#include <cstdlib>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../CharMapFilter.h"

namespace NSShaper { unsigned FT_SetCMapForCharCode(void*, unsigned); }

#define CHECK(condition) do { \
	if (!(condition)) { \
		std::fprintf(stderr, "FAIL: %s (line %d)\n", #condition, __LINE__); \
		std::exit(1); \
	} \
} while (0)

// Mirror the pre-fix and post-fix lookup paths to demonstrate the original
// collision and verify the legacy-font filter independently of font fallback.
static unsigned Lookup(FT_Face face, unsigned codepoint, bool patched)
{
	unsigned index = 0;
	for (int i = 0; i < face->num_charmaps; ++i)
	{
		FT_CharMap map = face->charmaps[i];
		if ((patched && IsLegacyMacScriptCharMap(map)) || FT_Set_Charmap(face, map))
			continue;
		if (map->encoding == FT_ENCODING_UNICODE)
		{
			index = FT_Get_Char_Index(face, codepoint);
			if (index)
				return index;
		}
		else if (map->encoding == FT_ENCODING_NONE || map->encoding == FT_ENCODING_MS_SYMBOL ||
			map->encoding == FT_ENCODING_APPLE_ROMAN)
			index = FT_Get_Char_Index(face, codepoint);
	}
	return index;
}

static bool Coverage(FT_Face face, unsigned codepoint, bool patched)
{
	for (int i = 0; i < face->num_charmaps; ++i)
	{
		FT_CharMap map = face->charmaps[i];
		if ((patched && IsLegacyMacScriptCharMap(map)) || FT_Set_Charmap(face, map))
			continue;
		if (FT_Get_Char_Index(face, codepoint))
			return true;
	}
	return false;
}

int main()
{
	FT_Library library;
	CHECK(!FT_Init_FreeType(&library));

	FT_Face heiti;
	CHECK(!FT_New_Face(library, "/System/Library/Fonts/STHeiti Light.ttc", 1, &heiti));
	CHECK(!FT_Select_Charmap(heiti, FT_ENCODING_UNICODE));
	const unsigned chinese = FT_Get_Char_Index(heiti, 0x6850); // 桐
	CHECK(chinese);
	CHECK(Lookup(heiti, 0xcda9, false) == chinese); // 충 incorrectly maps to 桐
	CHECK(Lookup(heiti, 0xcda9, true) == 0);
	CHECK(NSShaper::FT_SetCMapForCharCode(heiti, 0xcda9) == 0);
	CHECK(Coverage(heiti, 0xcda9, false));
	CHECK(!Coverage(heiti, 0xcda9, true));
	CHECK(Lookup(heiti, 0x6850, true) == chinese);
	CHECK(NSShaper::FT_SetCMapForCharCode(heiti, 0x6850) == chinese);
	CHECK(Coverage(heiti, 0x6850, true));

	FT_Face korean;
	CHECK(!FT_New_Face(library, "/System/Library/Fonts/AppleSDGothicNeo.ttc", 0, &korean));
	CHECK(Lookup(korean, 0xcda9, true));
	CHECK(NSShaper::FT_SetCMapForCharCode(korean, 0xcda9));
	CHECK(Coverage(korean, 0xcda9, true));

	FT_Face symbol;
	CHECK(!FT_New_Face(library, "/System/Library/Fonts/Supplemental/Wingdings.ttf", 0, &symbol));
	FT_ULong symbolCodepoint = 0;
	bool found = false;
	for (int i = 0; i < symbol->num_charmaps; ++i)
	{
		FT_CharMap map = symbol->charmaps[i];
		if (IsLegacyMacScriptCharMap(map) || FT_Set_Charmap(symbol, map))
			continue;
		FT_UInt gid;
		symbolCodepoint = FT_Get_First_Char(symbol, &gid);
		if (gid)
		{
			found = true;
			break;
		}
	}
	CHECK(found);
	CHECK(Coverage(symbol, symbolCodepoint, true));
	CHECK(Lookup(symbol, symbolCodepoint, false) == Lookup(symbol, symbolCodepoint, true));

	FT_CharMapRec map = {};
	map.platform_id = 3;
	map.encoding_id = 0;
	map.encoding = FT_ENCODING_MS_SYMBOL;
	CHECK(!IsLegacyMacScriptCharMap(&map));
	map.platform_id = 1;
	map.encoding_id = 0;
	map.encoding = FT_ENCODING_APPLE_ROMAN;
	CHECK(!IsLegacyMacScriptCharMap(&map));
	map.platform_id = 1;
	map.encoding_id = 25;
	map.encoding = FT_ENCODING_NONE;
	CHECK(IsLegacyMacScriptCharMap(&map));

	FT_Done_Face(symbol);
	FT_Done_Face(korean);
	FT_Done_Face(heiti);
	FT_Done_FreeType(library);
	std::puts("PASS: Hangul lookup and false coverage fixed; Chinese, Hangul and symbol coverage preserved.");
	return 0;
}
