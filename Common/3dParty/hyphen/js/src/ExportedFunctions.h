#ifndef EXPORTED_FUNCTIONS_H
#define EXPORTED_FUNCTIONS_H

#include "HyphenApplication.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

CHyphenApplication* hyphenCreateApplication();
void hyphenDestroyApplication(CHyphenApplication *app);

int hyphenLoadDictionary(CHyphenApplication *app, const char *dict, const unsigned int dict_size, const char* lang);
char* hyphenWord(CHyphenApplication *app, const char *word, const char* lang);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // EXPORTED_FUNCTIONS_H
