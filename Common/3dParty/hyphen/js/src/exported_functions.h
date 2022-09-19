#ifndef EXPORTED_FUNCTIONS_H
#define EXPORTED_FUNCTIONS_H

#include <fstream>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void load_dictionary();
void free_dictionary();
void hyphenate(const char *word, char *hyphens, char *hword);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // EXPORTED_FUNCTIONS_H