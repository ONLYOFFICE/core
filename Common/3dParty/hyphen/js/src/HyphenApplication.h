#ifndef HYPHEN_APPLICATION_H
#define HYPHEN_APPLICATION_H

#include <map>
#include <vector>
#include <string>

#include "./../../hyphen/hyphen.h"

class CHyphenApplication
{
public:
	CHyphenApplication();
	~CHyphenApplication();

	char* hyphenWord(const char *word, size_t size, const char *lang);
	void loadDictionary(const char *dict, size_t size, const char* lang);

private:
	std::map<std::string, HyphenDict*> m_mapDicts;
	char* m_pHyphenVector;

};

#endif // HYPHEN_APPLICATION_H