#include "ExportedFunctions.h"

CHyphenApplication* hyphenCreateApplication()
{
	return new CHyphenApplication();
}
void hyphenDestroyApplication(CHyphenApplication *app)
{
	delete app;
}

int hyphenLoadDictionary(CHyphenApplication *app, const char *dict, const unsigned int dict_size, const char* lang)
{
	return app->loadDictionary(dict, dict_size, lang);
}
char* hyphenWord(CHyphenApplication *app, const char *word, const char* lang)
{
	return app->hyphenWord(word, lang);
}
