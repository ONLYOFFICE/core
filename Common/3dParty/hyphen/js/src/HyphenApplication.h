#ifndef HYPHEN_APPLICATION_H
#define HYPHEN_APPLICATION_H

#include <map>
#include <vector>
#include <string>
#include <sstream>

#include "./../../hyphen/hyphen.h"

class CHyphenApplication
{
public:
	CHyphenApplication();
	~CHyphenApplication();

	char* hyphenWord(const char *word, const char *lang);
	void loadDictionary(const char *src, const char *lang);

private:
	std::map<std::string, HyphenDict*> m_mapDicts;
	size_t m_nMaxDictsCount;

	char* m_pHyphenVector;
	size_t m_nHyphenVectorSize;

};

#endif // HYPHEN_APPLICATION_H