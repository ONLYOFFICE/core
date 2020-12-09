#include "./base.h"
#include <iostream>

int main()
{
    CSpellchecker* spell = Spellchecker_Create();

    FILE* fAff = fopen("D:/GIT/dictionaries/en_US/en_US.aff", "rb");
    fseek(fAff, 0, SEEK_END);
    unsigned int nAffSize = (unsigned int)ftell(fAff);
    fseek(fAff, 0, SEEK_SET);
    unsigned char* pAffData = (unsigned char*)Spellchecker_Malloc(nAffSize);
    size_t nAffSizeRead = fread((void*)pAffData, 1, (size_t)nAffSize, fAff);
    fclose(fAff);

    FILE* fDic = fopen("D:/GIT/dictionaries/en_US/en_US.dic", "rb");
    fseek(fDic, 0, SEEK_END);
    unsigned int nDicSize = (unsigned int)ftell(fDic);
    fseek(fDic, 0, SEEK_SET);
    unsigned char* pDicData = (unsigned char*)Spellchecker_Malloc(nDicSize);
    size_t nDicSizeRead = fread((void*)pDicData, 1, (size_t)nDicSize, fDic);
    fclose(fDic);

    int bIsAdd1 = Spellchecker_AddDictionary(spell, "en_US.aff", pAffData, nAffSize);
    int bIsAdd2 = Spellchecker_AddDictionary(spell, "en_US.dic", pDicData, nDicSize);
    int nCheck0 = Spellchecker_Load(spell, "en_US.aff", "en_US.dic");
    int nCheck1 = Spellchecker_Spell(spell, "hello");
    int nCheck2 = Spellchecker_Spell(spell, "hellop");
    unsigned char* pSuggestData = Spellchecker_Suggest(spell, "hellop");

    Spellchecker_Destroy(spell);
    return 0;
}
