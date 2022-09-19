#include <iostream>
#include <fstream>
#include <map>

#include "exported_functions.h"
#include "HyphenUtils.cpp"

// CHyphenApplication* createHyphenApplication()
// {
// 	return new CHyphenApplication();
// }
// void destoyHyphenApplication(CHyphenApplication *app)
// {
// 	delete app;
// }

HyphenDict *dict;


void load_dictionary()
{
	dict = hnj_hyphen_load("dictionaries/hyph_ru_RU.dic");
	// stringstream ss
	// dict = hnj_hyphen_load_stream(fin)
}
void free_dictionary()
{
	hnj_hyphen_free(dict);
}

void hyphenate(const char *word, char *hyphens, char *hword)
{
	int n = strlen(word);

	char **rep = NULL;
	int *pos = NULL;
	int *cut = NULL;
	
	hnj_hyphen_hyphenate2(dict, word, n, hyphens, hword, &rep, &pos, &cut);
}
// emcc src/exported_functions.cpp src/HyphenUtils.cpp -o deploy/hyphen.js -sEXPORTED_FUNCTIONS='_malloc','_free','_hyphenate','_load_dictionary','_free_dictionary' -sEXPORTED_RUNTIME_METHODS='stringToUTF8','UTF8ToString' --preload-file dictionaries -sALLOW_MEMORY_GROWTH -sERROR_ON_UNDEFINED_SYMBOLS=0
// emcc src/exported_functions.cpp src/CHYphenApplication.cpp ../hyphen/hyphen.c ../hyphen/hnjalloc.c -o deploy/hyphen.js -sEXPORTED_FUNCTIONS='_malloc','_free','_hyphenate','_load_dictionary','_free_dictionary' -sEXPORTED_RUNTIME_METHODS='stringToUTF8','UTF8ToString' --preload-file dictionaries -sALLOW_MEMORY_GROWTH