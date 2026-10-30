// SPDX-License-Identifier: AGPL-3.0-only
#include "../ApplicationFonts.h"
#include <cstdio>
#include <cstdlib>
#include <set>

#define CHECK(condition) do { \
	if (!(condition)) { \
		std::fprintf(stderr, "FAIL: %s (line %d)\n", #condition, __LINE__); \
		std::exit(1); \
	} \
} while (0)

struct Checker : NSFonts::CApplicationFontsSymbolsChecker
{
	std::set<int> codes;
	void Check(const int& codepoint, const unsigned int&) override { codes.insert(codepoint); }
};

int main()
{
	NSFonts::CApplicationFontsSymbols symbols;
	Checker heiti;
	symbols.CheckSymbols(L"/System/Library/Fonts/STHeiti Light.ttc", 1, &heiti);
	CHECK(heiti.codes.count(0x6850)); // 桐 is supported
	CHECK(!heiti.codes.count(0xcda9)); // 충 is not supported

	Checker korean;
	symbols.CheckSymbols(L"/System/Library/Fonts/AppleSDGothicNeo.ttc", 0, &korean);
	CHECK(korean.codes.count(0xcda9));

	Checker symbol;
	symbols.CheckSymbols(L"/System/Library/Fonts/Supplemental/Wingdings.ttf", 0, &symbol);
	CHECK(!symbol.codes.empty());

	std::puts("PASS: Chinese and Korean coverage, false Hangul mapping, symbol glyphs.");
	return 0;
}
