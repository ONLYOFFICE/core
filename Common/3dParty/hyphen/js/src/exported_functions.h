#include <fstream>

#include "./../../hyphen/hyphen.h"

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