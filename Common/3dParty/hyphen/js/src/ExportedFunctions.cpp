#include "ExportedFunctions.h"

CHyphenApplication* hyphenCreateApplication()
{
	return new CHyphenApplication();
}
void hyphenDestroyApplication(CHyphenApplication *app)
{
	delete app;
}

void hyphenLoadDictionary(CHyphenApplication *app, const char *src, const char* lang)
{
	app->loadDictionary(src, lang);
}
char* hyphenWord(CHyphenApplication *app, const char *word, const char* lang)
{
	return app->hyphenWord(word, lang);
}