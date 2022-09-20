#include "HyphenApplication.h"

#include "HyphenUtils.cpp"

#define HYPHEN_VECTOR_SIZE 100
#define MAX_DICTS_COUNT 10

CHyphenApplication::CHyphenApplication() : 
	m_nHyphenVectorSize(HYPHEN_VECTOR_SIZE), m_nMaxDictsCount(MAX_DICTS_COUNT)
{
	m_pHyphenVector = new char[m_nHyphenVectorSize];
}
CHyphenApplication::~CHyphenApplication()
{
	delete[] m_pHyphenVector;
}

char* CHyphenApplication::hyphenWord(const char *word, const char *lang)
{
	std::string s_lang(lang);
	HyphenDict *dict = m_mapDicts[s_lang];
	size_t n = strlen(word);

	// resize 2x
	if(n + 5 > m_nHyphenVectorSize)
	{
		delete[] m_pHyphenVector;
		m_nHyphenVectorSize *= 2;
		m_pHyphenVector = new char[m_nHyphenVectorSize];
	}

	memset(m_pHyphenVector, 0, m_nHyphenVectorSize);

	char **rep = NULL;
	int *pos = NULL;
	int *cut = NULL;
	char *hword = new char[n * 2];

	hnj_hyphen_hyphenate2(dict, word, n, m_pHyphenVector, hword, &rep, &pos, &cut);

	delete[] hword;
	return m_pHyphenVector;
}
void CHyphenApplication::loadDictionary(const char *src, const char* lang)
{
	std::string s_lang(lang);
	HyphenDict *dict;

	if(m_mapDicts[s_lang] != nullptr)
		return;

	if(m_mapDicts.size() > m_nMaxDictsCount)
		m_mapDicts.erase(m_mapDicts.begin());

	std::stringstream ss(src);
	dict = hnj_hyphen_load_stream(ss);

	m_mapDicts[s_lang] = dict;
}