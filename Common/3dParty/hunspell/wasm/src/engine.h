#include "hunspell.h"
#include <iostream>
#include <stdlib.h>
#include "base.h"
#include <map>
#include <queue>
#include <string>
#include <memory.h>
//#define WASM_LOGGING fprintf

class CFileMemory
{
public:
    unsigned char* data;
    unsigned int len;

public:
    CFileMemory(unsigned char* _data = NULL, unsigned int _len = 0)
    {
        data = _data;
        len = _len;
    }
    ~CFileMemory()
    {
        if (data)
        {
            Spellchecker_Free(data);
            data = NULL;
        }
    }
};

class CSuggests
{
protected:
    char*	m_pData;
    size_t	m_lSize;

    char*	m_pDataCur;
    size_t	m_lSizeCur;

public:
    CSuggests()
    {
        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur	= m_pData;
        m_lSizeCur	= m_lSize;
    }
    CSuggests(const size_t& nLen)
    {
        m_lSize = nLen;
        m_pData = (char*)malloc(m_lSize * sizeof(char));

        m_lSizeCur = 0;
        m_pDataCur = m_pData;
    }
    virtual ~CSuggests()
    {
        Clear();
    }

    inline void AddSize(const size_t& nSize)
    {
        if (NULL == m_pData)
        {
            m_lSize = 1000;
            if (nSize > m_lSize)
                m_lSize = nSize;

            m_pData = (char*)malloc(m_lSize * sizeof(char));

            m_lSizeCur = 0;
            m_pDataCur = m_pData;
            return;
        }

        if ((m_lSizeCur + nSize) > m_lSize)
        {
            while ((m_lSizeCur + nSize) > m_lSize)
            {
                m_lSize *= 2;
            }

            char* pRealloc = (char*)realloc(m_pData, m_lSize * sizeof(char));
            if (NULL != pRealloc)
            {
                // реаллок сработал
                m_pData		= pRealloc;
                m_pDataCur	= m_pData + m_lSizeCur;
            }
            else
            {
                char* pMalloc = (char*)malloc(m_lSize * sizeof(char));
                memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(char));

                free(m_pData);
                m_pData		= pMalloc;
                m_pDataCur	= m_pData + m_lSizeCur;
            }
        }
    }

public:
    void AddInt(const unsigned int& value)
    {
        AddSize(4);
        memcpy(m_pDataCur, &value, sizeof(unsigned int));
        m_pDataCur += 4;
        m_lSizeCur += 4;
    }
    void WriteString(const char* value, const unsigned int& len)
    {
        AddSize(len + 4);
        memcpy(m_pDataCur, &len, sizeof(unsigned int));
        m_pDataCur += 4;
        m_lSizeCur += 4;
        memcpy(m_pDataCur, value, len);
        m_pDataCur += len;
        m_lSizeCur += len;
    }
    unsigned char* GetBuffer()
    {
        return (unsigned char*)m_pData;
    }

    void Clear()
    {
        free(m_pData);

        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur	= m_pData;
        m_lSizeCur	= 0;
    }
    void ClearNoAttack()
    {
        m_pDataCur	= m_pData;
        m_lSizeCur	= 0;
    }
    unsigned int GetSize()
    {
        return (unsigned int)m_lSizeCur;
    }

    void SkipLen()
    {
        AddInt(0);
    }
    void WriteLen()
    {
        unsigned int len = (unsigned int)m_lSizeCur;
        memcpy(m_pData, &len, sizeof(unsigned int));
    }
};

class LimitedEngineMap 
{
public:
    LimitedEngineMap(size_t _maxEngineNumer) 
    {
        m_nMaxEngineNumber = _maxEngineNumer;
    }
    ~LimitedEngineMap() 
    {
        for (std::pair<const std::string, Hunhandle*> &i : m_mapDictionariesEngines) 
        {
            Hunspell_destroy(i.second);
        }
        m_mapDictionariesEngines.clear();
    }
    std::map<std::string, Hunhandle*>::iterator find(const std::string &to_find) 
    {
        return m_mapDictionariesEngines.find(to_find);
    }
    std::map<std::string, Hunhandle*>::iterator end()
    {
        return m_mapDictionariesEngines.end();
    }
    std::pair<std::map<std::string, Hunhandle*>::iterator,bool> insert(const std::pair<std::string, Hunhandle*> &data) 
    {
        if (m_qEngineQueue.size() == m_nMaxEngineNumber)
        { 
            if (m_mapDictionariesEngines.find(m_qEngineQueue.front()) != 
                m_mapDictionariesEngines.end()) 
                {
                    Hunspell_destroy(m_mapDictionariesEngines[m_qEngineQueue.front()]); 
                    m_mapDictionariesEngines.erase(m_qEngineQueue.front());
                    m_qEngineQueue.pop();
                }
            
        }
        m_qEngineQueue.push(data.first);
        return m_mapDictionariesEngines.insert(data);
    }
    void RemoveEngine(const char* id) 
    {
        if (m_mapDictionariesEngines.find(id) == m_mapDictionariesEngines.end())
        {
            return;
        }
        std::queue<std::string> new_queue;;
        Hunspell_destroy(m_mapDictionariesEngines[id]);
        m_mapDictionariesEngines.erase(id);
        while (!m_qEngineQueue.empty())
        {
            if (m_qEngineQueue.front() != id) 
            {
                new_queue.push(m_qEngineQueue.front());
            }
            m_qEngineQueue.pop();
        }
        m_qEngineQueue = new_queue;
    }
private:
    std::map<std::string, Hunhandle*> m_mapDictionariesEngines; 
    std::queue<std::string> m_qEngineQueue;
    size_t m_nMaxEngineNumber;
};

class CSpellchecker
{
public:
    static std::map<std::string, CFileMemory*> g_dictionaries;

public:
    LimitedEngineMap m_oDictionariesEngines;
    std::string m_sCurrentDictionaryId;
    Hunhandle* m_pCurrentDictionary;
    CSuggests m_oSuggests;

public:
    CSpellchecker(size_t _MaxEngineNumber) :
    m_oDictionariesEngines(_MaxEngineNumber)
    {
        m_sCurrentDictionaryId = "";
        m_pCurrentDictionary = NULL;
    }
    ~CSpellchecker()
    {
        
        m_pCurrentDictionary = NULL;

        for (std::pair<const std::string, CFileMemory*> &i : g_dictionaries)
        {
            delete i.second;
        }
        g_dictionaries.clear();
    }

public:
    int AddDictionary(const char*& id, unsigned char* data, unsigned int size)
    {
#ifdef WASM_LOGGING
        WASM_LOGGING(stderr, "add dictionary: [%s, %d]\n", id, (int)size);
#endif

        RemoveDictionary(id);
        g_dictionaries.insert(std::pair<std::string, CFileMemory*>(std::string(id), new CFileMemory(data, size)));
        return 1;
    }
    void RemoveDictionary(const char*& id)
    {
        std::string sid(id);
        std::map<std::string, CFileMemory*>::iterator iter = g_dictionaries.find(sid);
        if (iter != g_dictionaries.end())
        {
            CFileMemory* file = iter->second;
            g_dictionaries.erase(iter);
            delete file;
        }
    }
    static CFileMemory* Get(const char*& id)
    {
        std::map<std::string, CFileMemory*>::iterator iter = g_dictionaries.find(std::string(id));
        if (iter == g_dictionaries.end())
            return NULL;
        return iter->second;
    }

    int Load(const char*& aff_id, const char*& dic_id)
    {
#ifdef WASM_LOGGING
        WASM_LOGGING(stderr, "load: [%s, %s]\n", aff_id, dic_id);
#endif
        return (NULL != SetLanguage(aff_id, dic_id)) ? 1 : 0;
    }
    int Spell(const char*& word)
    {
#ifdef WASM_LOGGING
        WASM_LOGGING(stderr, "spell: [%s]\n", word);
#endif
        if (!m_pCurrentDictionary)
            return 0;
        return Hunspell_spell(m_pCurrentDictionary, word);
    }
    unsigned char* Suggest(const char*& word)
    {
#ifdef WASM_LOGGING
        WASM_LOGGING(stderr, "suggest: [%s]\n", word);
#endif
        if (!m_pCurrentDictionary)
            return NULL;

        char** pSuggest;
        int nSuggestCount = Hunspell_suggest(m_pCurrentDictionary, &pSuggest, word);

        m_oSuggests.ClearNoAttack();
        m_oSuggests.SkipLen();

        for (int i = 0; i < nSuggestCount; ++i)
        {
            m_oSuggests.WriteString(pSuggest[i], (unsigned int)strlen(pSuggest[i]));
        }

        m_oSuggests.WriteLen();

        if (0 < nSuggestCount)
            Hunspell_free_list(m_pCurrentDictionary, &pSuggest, nSuggestCount);

        return m_oSuggests.GetBuffer();
    }
    void RemoveEngine(const char* id) 
    {
        m_oDictionariesEngines.RemoveEngine(id);
		if (m_sCurrentDictionaryId == id)
		{
			m_sCurrentDictionaryId = "";
			m_pCurrentDictionary = NULL;
		}
    }
protected:
    Hunhandle* SetLanguage(const std::string& aff_id, const std::string& dic_id)
    {
        if ((aff_id + dic_id) == m_sCurrentDictionaryId)
            return m_pCurrentDictionary;
       
        m_sCurrentDictionaryId = aff_id + dic_id;
        m_pCurrentDictionary = NULL;

        std::map<std::string, Hunhandle*>::iterator iterEngine = m_oDictionariesEngines.find(m_sCurrentDictionaryId);
        if (iterEngine != m_oDictionariesEngines.end())
        {
            m_pCurrentDictionary = iterEngine->second;
            return m_pCurrentDictionary;
        }

        Hunhandle* pDictionary = Hunspell_create(aff_id.c_str(), dic_id.c_str());
        m_oDictionariesEngines.insert(std::pair<std::string, Hunhandle*>(m_sCurrentDictionaryId, pDictionary));
        m_pCurrentDictionary = pDictionary;
        return m_pCurrentDictionary;
    }
};
