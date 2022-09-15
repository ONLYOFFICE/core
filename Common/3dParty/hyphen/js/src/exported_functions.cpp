#include "exported_functions.h"
#include <iostream>

HyphenDict *dict;

void load_dictionary()
{
    dict = hnj_hyphen_load("dictionaries/hyph_ru_RU.dic");
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
// emcc src/exported_functions.cpp ../hyphen/hyphen.c ../hyphen/hnjalloc.c -o deploy/effects.js -sEXPORTED_FUNCTIONS='_malloc','_free','_hyphenate','_load_dictionary','_free_dictionary' -sEXPORTED_RUNTIME_METHODS='stringToUTF8','UTF8ToString' --preload-file dictionaries