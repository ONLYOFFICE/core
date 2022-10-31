#include <fstream>
#include <iostream>

#include "./../js/src/ExportedFunctions.h"

int main(int argc, char *argv[])
{
	HyphenDict *dict;

	std::string dict_filename = PRO_DIR;
	std::string words_filename = PRO_DIR;
	std::string result_filename = PRO_DIR;
	std::string dict_name = "en_US";

	// set your filenames here
	dict_filename += ("../../../../../dictionaries/" + dict_name + "/hyph_" + dict_name + ".dic");
	words_filename += "words.txt";
	result_filename += "result.txt";

	// load the hyphenation dictionary
	dict = hnj_hyphen_load(dict_filename.c_str());

	std::ifstream fin(words_filename);
	if(!fin.is_open())
	{
		std::cerr << "could not open " << words_filename << "!" << std::endl;
		return -1;
	}

	std::ofstream fout(result_filename);
	if(!fout.is_open())
	{
		std::cerr << "could not open " << result_filename << "!" << std::endl;
		return -1;
	}

	while(!fin.eof())
	{
		char **rep = NULL;
		int *pos = NULL;
		int *cut = NULL;

		std::string word;

		fin >> word;
		int n = word.size();
		char *hword = new char[n * 2];
		char *hyphens = new char[n + 5];

		/**
		 * @brief
		 * input data:
		 *
		 * word: input word
		 * word_size: byte length of the input word
		 * hyphens: allocated character buffer (size = word_size + 5)
		 * hyphenated_word: allocated character buffer (size ~ word_size * 2) or NULL
		 * rep, pos, cut: pointers (point to the allocated and _zeroed_ buffers
		 *	(size=word_size) or with NULL value) or NULL
		 *
		 * output data:
		 *
		 * hyphens: hyphenation vector (hyphenation points signed with odd numbers).
		 * hyphenated_word: hyphenated input word (hyphens signed with `=').
		 *	optional (NULL input).
		 * rep: NULL (only standard hyph.), or replacements (hyphenation points
		 *	signed with `=' in replacements).
		 * pos: NULL, or difference of the actual position and the beginning
		 *	positions of the change in input words.
		 * cut:	NULL, or counts of the removed characters of the original words
		 *	at hyphenation.
		 *
		 * Note: rep, pos, cut are complementary arrays to the hyphens, indexed with the
		 *	character positions of the input word.
		 */
		hnj_hyphen_hyphenate2(dict, word.c_str(), n, hyphens, hword, &rep, &pos, &cut);

		fout << hword << ' ';

		delete[] hword;
		delete[] hyphens;
	}
	fin.close();
	fout.close();

#if 1

	CHyphenApplication* pApplication = hyphenCreateApplication();

	FILE* fDictionary = fopen(dict_filename.c_str(), "rb");
	fseek(fDictionary, 0, SEEK_END);
	long lDictSize = ftell(fDictionary);
	fseek(fDictionary, 0, SEEK_SET);  /* same as rewind(f); */

	char* pDictData = (char*)malloc(lDictSize);
	fread(pDictData, (size_t)lDictSize, 1, fDictionary);
	fclose(fDictionary);

	int nResult = hyphenLoadDictionary(pApplication, pDictData, (unsigned int)lDictSize, dict_name.c_str());

	free(pDictData);

	char* pHyphenVector = hyphenWord(pApplication, "expedition", dict_name.c_str());

	hyphenDestroyApplication(pApplication);

#endif
}







