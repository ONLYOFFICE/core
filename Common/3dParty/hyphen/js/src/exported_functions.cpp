#include "exported_functions.h"

CHyphenApplication* createHyphenApplication()
{
	return new CHyphenApplication();
}
void destroyHyphenApplication(CHyphenApplication *app)
{
	delete app;
}

void loadDictionary(CHyphenApplication *app, const char *src, const char* lang)
{
	app->loadDictionary(src, lang);
}
char* hyphenWord(CHyphenApplication *app, const char *word, const char* lang)
{
	return app->hyphenWord(word, lang);
}


// emcc src/exported_functions.cpp src/HyphenUtils.cpp -o deploy/hyphen.js -sEXPORTED_FUNCTIONS='_malloc','_free','_hyphenate','_load_dictionary','_free_dictionary' -sEXPORTED_RUNTIME_METHODS='stringToUTF8','UTF8ToString' --preload-file dictionaries -sALLOW_MEMORY_GROWTH -sERROR_ON_UNDEFINED_SYMBOLS=0
// emcc src/exported_functions.cpp src/CHYphenApplication.cpp ../hyphen/hyphen.c ../hyphen/hnjalloc.c -o deploy/hyphen.js -sEXPORTED_FUNCTIONS='_malloc','_free','_hyphenate','_load_dictionary','_free_dictionary' -sEXPORTED_RUNTIME_METHODS='stringToUTF8','UTF8ToString' --preload-file dictionaries -sALLOW_MEMORY_GROWTH