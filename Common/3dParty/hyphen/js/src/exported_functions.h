#ifndef EXPORTED_FUNCTIONS_H
#define EXPORTED_FUNCTIONS_H

#include "HyphenApplication.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

CHyphenApplication* createHyphenApplication();
void destroyHyphenApplication(CHyphenApplication *app);

void loadDictionary(CHyphenApplication *app, const char *src, const char* lang);
char* hyphenWord(CHyphenApplication *app, const char *word, const char* lang);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // EXPORTED_FUNCTIONS_H