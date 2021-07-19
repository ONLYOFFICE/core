#include "base.h"
#include "engine.h"

std::map<std::string, CFileMemory*> CSpellchecker::g_dictionaries;

void* Spellchecker_Malloc(unsigned int size)
{
    return ::malloc(size);
}
void Spellchecker_Free(void* p)
{
    if (p)
        ::free(p);
}

CSpellchecker* Spellchecker_Create(size_t max_engine_number)
{
    return new CSpellchecker(max_engine_number);
}
void Spellchecker_Destroy(CSpellchecker* p)
{
    delete p;
}

int Spellchecker_AddDictionary(CSpellchecker* p, const char* id, unsigned char* data, unsigned int size)
{
    if (!p)
        return 0;
    return p->AddDictionary(id, data, size);
}
void Spellchecker_RemoveDicrionary(CSpellchecker* p, const char* id)
{
    if (!p)
        return;
    p->RemoveDictionary(id);
}

int Spellchecker_Load(CSpellchecker* p, const char* aff_id, const char* dic_id)
{
    if (!p) return 0;
    return p->Load(aff_id, dic_id);
}
int Spellchecker_Spell(CSpellchecker* p, const char* word_utf8)
{
    return  p->Spell(word_utf8);
}
unsigned char* Spellchecker_Suggest(CSpellchecker* p, const char* word_utf8)
{;
    return p->Suggest(word_utf8);
}
void Spellchecker_RemoveEngine(CSpellchecker* p, const char* sid) 
{
    p->RemoveEngine(sid);
}

double Spellchecker_TotalAllocatedMemory() 
{	
    struct mallinfo a;
    a = mallinfo(); // Debug
    return (double)a.uordblks / 1000. / 1000.;
}
