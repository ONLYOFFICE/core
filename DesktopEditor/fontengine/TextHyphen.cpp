/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "./TextHyphen.h"

#include <iostream>
#include <map>
#include <sstream>

#include "../../Common/3dParty/hyphen/hyphen/hnjalloc.h"
#include "../../Common/3dParty/hyphen/hyphen/hnjalloc.c"
#include "../../Common/3dParty/hyphen/hyphen/hyphen.c"

#ifndef HYPHEN_ENGINE_DISABLE_FILESYSTEM
#include "../common/Directory.h"
#endif

// function from hyphen.c using std::ifstream
HyphenDict* hnj_hyphen_load_stream(std::istream &in)
{
	HyphenDict *dict[2];
	HashTab *hashtab;
	char buf[MAX_CHARS];
	int nextlevel = 0;
	int i, j, k;
	HashEntry *e;
	int state_num = 0;

	/* loading one or two dictionaries (separated by NEXTLEVEL keyword) */
	for (k = 0; k < 2; k++)
	{
		hashtab = hnj_hash_new();
#ifdef VERBOSE
		global[k] = hashtab;
#endif
		hnj_hash_insert (hashtab, "", 0);
		dict[k] = (HyphenDict *) hnj_malloc (sizeof(HyphenDict));
		dict[k]->num_states = 1;
		dict[k]->states = (HyphenState *) hnj_malloc (sizeof(HyphenState));
		dict[k]->states[0].match = NULL;
		dict[k]->states[0].repl = NULL;
		dict[k]->states[0].fallback_state = -1;
		dict[k]->states[0].num_trans = 0;
		dict[k]->states[0].trans = NULL;
		dict[k]->nextlevel = NULL;
		dict[k]->lhmin = 0;
		dict[k]->rhmin = 0;
		dict[k]->clhmin = 0;
		dict[k]->crhmin = 0;
		dict[k]->nohyphen = NULL;
		dict[k]->nohyphenl = 0;

	/* read in character set info */
	if (k == 0)
	{
		for (i = 0; i < MAX_NAME; i++)
			dict[k]->cset[i]= 0;

		if (in >> dict[k]->cset)
		{
			for (i = 0; i < MAX_NAME; i++)
				if ((dict[k]->cset[i] == '\r') || (dict[k]->cset[i] == '\n'))
					dict[k]->cset[i] = 0;
		}
		else
		{
			dict[k]->cset[0] = 0;
		}
		dict[k]->utf8 = (strcmp(dict[k]->cset, "UTF-8") == 0);
	}
	else
	{
		strncpy(dict[k]->cset, dict[0]->cset, sizeof(dict[k]->cset)-1);
		dict[k]->cset[sizeof(dict[k]->cset)-1] = '\0';
		dict[k]->utf8 = dict[0]->utf8;
	}

	if (k == 0 || nextlevel)
	{
		while (in.getline(buf, sizeof(buf), '\n'))
		{
			if(strlen(buf) < sizeof(buf))
				strcat(buf, "\n");

			/* discard lines that don't fit in buffer */
			if (!in.eof() && strchr(buf, '\n') == NULL)
			{
				int c;
				while ((c = in.get()) != '\n' && c != EOF);

				/* issue warning if not a comment */
				if (buf[0] != '%')
				{
					fprintf(stderr, "Warning: skipping too long pattern (more than %lu chars)\n", sizeof(buf));
				}
				continue;
			}

			if (strncmp(buf, "NEXTLEVEL", 9) == 0)
			{
				nextlevel = 1;
				break;
			}
			else if (buf[0] != '%')
			{
				hnj_hyphen_load_line(buf, dict[k], hashtab);
			}
		}
	}
	else if (k == 1)
	{
		/* default first level: hyphen and ASCII apostrophe */
		if (!dict[0]->utf8)
			hnj_hyphen_load_line((char*)"NOHYPHEN ',-\n", dict[k], hashtab);

		else
			hnj_hyphen_load_line((char*)"NOHYPHEN ',\xe2\x80\x93,\xe2\x80\x99,-\n", dict[k], hashtab);

		strncpy(buf, (char*)"1-1\n", MAX_CHARS - 1); /* buf rewritten by hnj_hyphen_load here */
		buf[MAX_CHARS-1] = '\0';
		hnj_hyphen_load_line(buf, dict[k], hashtab); /* remove hyphen */
		hnj_hyphen_load_line((char*)"1'1\n", dict[k], hashtab); /* ASCII apostrophe */

		if (dict[0]->utf8)
		{
			hnj_hyphen_load_line((char*)"1\xe2\x80\x93" "1\n", dict[k], hashtab); /* endash */
			hnj_hyphen_load_line((char*)"1\xe2\x80\x99" "1\n", dict[k], hashtab); /* apostrophe */
		}
	}

  /* Could do unioning of matches here (instead of the preprocessor script).
	 If we did, the pseudocode would look something like this:

	 foreach state in the hash table
		foreach i = [1..length(state) - 1]
		   state to check is substr (state, i)
		   look it up
		   if found, and if there is a match, union the match in.

	 It's also possible to avoid the quadratic blowup by doing the
	 search in order of increasing state string sizes - then you
	 can break the loop after finding the first match.

	 This step should be optional in any case - if there is a
	 preprocessed rule table, it's always faster to use that.

*/

  /* put in the fallback states */
	for (i = 0; i < HASH_SIZE; i++)
		for (e = hashtab->entries[i]; e; e = e->next)
		{
			if (*(e->key)) for (j = 1; 1; j++)
			{
				state_num = hnj_hash_lookup (hashtab, e->key + j);
				if (state_num >= 0)
				break;
			}
			/* KBH: FIXME state 0 fallback_state should always be -1? */
			if (e->val)
			dict[k]->states[e->val].fallback_state = state_num;
		}

#ifdef VERBOSE
  for (i = 0; i < HASH_SIZE; i++)
	for (e = hashtab->entries[i]; e; e = e->next)
	  {
	printf ("%d string %s state %d, fallback=%d\n", i, e->key, e->val,
		dict[k]->states[e->val].fallback_state);
	for (j = 0; j < dict[k]->states[e->val].num_trans; j++)
	  printf (" %c->%d\n", dict[k]->states[e->val].trans[j].ch,
		  dict[k]->states[e->val].trans[j].new_state);
	  }
#endif

#ifndef VERBOSE
		hnj_hash_free (hashtab);
#endif
		state_num = 0;
	}
	if (nextlevel) dict[0]->nextlevel = dict[1];
	else
	{
		dict[1] -> nextlevel = dict[0];
		dict[1]->lhmin = dict[0]->lhmin;
		dict[1]->rhmin = dict[0]->rhmin;
		dict[1]->clhmin = (dict[0]->clhmin) ? dict[0]->clhmin : ((dict[0]->lhmin) ? dict[0]->lhmin : 3);
		dict[1]->crhmin = (dict[0]->crhmin) ? dict[0]->crhmin : ((dict[0]->rhmin) ? dict[0]->rhmin : 3);
#ifdef VERBOSE
		HashTab *r = global[0];
		global[0] = global[1];
		global[1] = r;
#endif
		return dict[1];
	}
	return dict[0];
}


namespace NSHyphen
{
	class CEngine_private
	{
	public:
		static std::wstring m_sDirectory;

		int m_nCacheSize;
		std::map<int, HyphenDict*> m_mapDicts;

		int         m_nLastLang;
		HyphenDict* m_pLastDict;

		// работаем всегда в пределах одной памяти
		char* m_pHyphenVector;
		size_t m_nHyphenVectorSize;

		CEngine_private()
		{
			m_nHyphenVectorSize = 100;

			m_pHyphenVector = new char[m_nHyphenVectorSize];
			memset(m_pHyphenVector, 0, m_nHyphenVectorSize);

			m_nLastLang = 0;
			m_pLastDict = NULL;

#ifndef HYPHEN_ENGINE_DISABLE_FILESYSTEM
			m_nCacheSize = 5;
#else
			m_nCacheSize = -1;
#endif
		}

		~CEngine_private()
		{
			delete [] m_pHyphenVector;
		}

		void AddLast()
		{
			if (m_pLastDict)
			{
				if (m_nCacheSize > 0 && m_mapDicts.size() == m_nCacheSize)
				{
					std::map<int, HyphenDict*>::iterator first = m_mapDicts.begin();
					delete first->second;
					m_mapDicts.erase(first);
				}
				m_mapDicts.insert(std::pair<int, HyphenDict*>{m_nLastLang, m_pLastDict});
			}
		}

		int LoadDictionary(const int& lang)
		{
			m_nLastLang = lang;
			m_pLastDict = NULL;

#ifndef HYPHEN_ENGINE_DISABLE_FILESYSTEM
			std::wstring sFilePath = GetDictionaryPath(m_nLastLang);
			if (m_sDirectory.empty())
				m_sDirectory = NSFile::GetProcessDirectory() + L"/dictionaries";

			if (NSFile::CFileBinary::Exists(sFilePath))
			{
				FILE* f = NSFile::CFileBinary::OpenFileNative(sFilePath, L"r");
				if (f == NULL)
					return 1;

				m_pLastDict = hnj_hyphen_load_file(f);
				fclose(f);
			}
#endif

			AddLast();

			return (NULL == m_pLastDict) ? 1 : 0;
		}
		int LoadDictionary(const int& lang, const unsigned char* data, const unsigned int& data_len)
		{
			m_nLastLang = lang;
			m_pLastDict = NULL;

			std::stringstream ss;
			ss.write((const char*)data, data_len);

			m_pLastDict = hnj_hyphen_load_stream(ss);
			AddLast();

			return (NULL == m_pLastDict) ? 1 : 0;
		}
#ifndef HYPHEN_ENGINE_DISABLE_FILESYSTEM
		std::wstring GetDictionaryPath(const int& lang)
		{
			if (m_sDirectory.empty())
				m_sDirectory = NSFile::GetProcessDirectory() + L"/dictionaries";

			for (int i = 0; i < NSTextLanguages::DictionaryRec_count; ++i)
			{
				if (m_nLastLang == NSTextLanguages::Dictionaries[i].m_lang)
				{
					const char* sNameStr = NSTextLanguages::Dictionaries[i].m_name;
					std::wstring sNameU = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sNameStr, (LONG)(strlen(sNameStr)));
					std::wstring sFilePath = m_sDirectory + L"/" + sNameU + L"/hyph_" + sNameU + L".dic";
					return sFilePath;
				}
			}

			return L"";
		}
#endif
		char* Process(const int& lang, const char* word, const int& len)
		{
			// resize 2x
			if (len + 5 > m_nHyphenVectorSize)
			{
				delete[] m_pHyphenVector;
				m_nHyphenVectorSize = 2 * (len + 5);
				m_pHyphenVector = new char[m_nHyphenVectorSize];

				memset(m_pHyphenVector, 0, m_nHyphenVectorSize);
			}
			else
			{
				// обнуляем после последнего использования
				char* mem = m_pHyphenVector;
				while (*mem)
					*mem++ = 0;
			}

			if (m_nLastLang != lang)
			{
				std::map<int, HyphenDict*>::iterator find = m_mapDicts.find(lang);
				if (find != m_mapDicts.end())
				{
					m_nLastLang = lang;
					m_pLastDict = find->second;
				}
				else
				{
#ifndef HYPHEN_ENGINE_DISABLE_FILESYSTEM
					LoadDictionary(lang);
#else
					m_nLastLang = lang;
					m_pLastDict = NULL;
#endif
				}
			}

			if (m_pLastDict)
			{
				char **rep = NULL;
				int *pos = NULL;
				int *cut = NULL;

				hnj_hyphen_hyphenate2(m_pLastDict, word, len, m_pHyphenVector, NULL, &rep, &pos, &cut);
				return m_pHyphenVector;
			}

			return NULL;
		}
	};

	std::wstring CEngine_private::m_sDirectory = L"";

	CEngine::CEngine()
	{
		m_internal = new CEngine_private();
	}
	CEngine::~CEngine()
	{
		delete m_internal;
	}

	void CEngine::Init(const std::wstring& directory)
	{
		CEngine_private::m_sDirectory = directory;
	}

	void CEngine::SetCacheSize(const int& size)
	{
#ifndef HYPHEN_ENGINE_DISABLE_FILESYSTEM
		if (0 == m_internal->m_mapDicts.size())
			m_internal->m_nCacheSize = size;
#endif
	}

	int CEngine::LoadDictionary(const int& lang)
	{
		return m_internal->LoadDictionary(lang);
	}
	int CEngine::LoadDictionary(const int& lang, const unsigned char* data, const unsigned int& data_len)
	{
		return m_internal->LoadDictionary(lang, data, data_len);
	}
	char* CEngine::Process(const int& lang, const char* word, const int& len)
	{
		return m_internal->Process(lang, word, (len == -1) ? strlen(word) : len);
	}
	bool CEngine::IsDictionaryExist(const int& lang)
	{
		for (int i = 0; i < NSTextLanguages::DictionaryRec_count; ++i)
		{
			if (lang == NSTextLanguages::Dictionaries[i].m_lang)
				return true;
		}
		return false;
	}
}
