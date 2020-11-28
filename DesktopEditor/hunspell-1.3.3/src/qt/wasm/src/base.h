#include <memory>

class CSpellchecker;

extern "C"
{
    void* Spellchecker_Malloc(unsigned int size);
    void Spellchecker_Free(void* p);

    CSpellchecker* Spellchecker_Create();
    void Spellchecker_Destroy(CSpellchecker*);

    int Spellchecker_AddDictionary(CSpellchecker* p, const char* sid, unsigned char* data, unsigned int size);
    void Spellchecker_RemoveDicrionary(CSpellchecker* p, const char* sid);

    int Spellchecker_Load(CSpellchecker* p, const char* aff_id, const char* dic_id);
    int Spellchecker_Spell(CSpellchecker* p, const char* word_utf8);
    unsigned char* Spellchecker_Suggest(CSpellchecker* p, const char* word_utf8);
}
