#include "./base.h"
#include <iostream>

int main()
{
    CSpellchecker* spell = Spellchecker_Create(100);

    FILE* fAff = fopen("en_GB.aff", "rb");
    fseek(fAff, 0, SEEK_END);
    unsigned int nAffSize = (unsigned int)ftell(fAff);
    fseek(fAff, 0, SEEK_SET);
    unsigned char* pAffData = (unsigned char*)Spellchecker_Malloc(nAffSize);
    size_t nAffSizeRead = fread((void*)pAffData, 1, (size_t)nAffSize, fAff);
    fclose(fAff);

    FILE* fDic = fopen("en_GB.dic", "rb");
    fseek(fDic, 0, SEEK_END);
    unsigned int nDicSize = (unsigned int)ftell(fDic);
    fseek(fDic, 0, SEEK_SET);
    unsigned char* pDicData = (unsigned char*)Spellchecker_Malloc(nDicSize);
    size_t nDicSizeRead = fread((void*)pDicData, 1, (size_t)nDicSize, fDic);
    fclose(fDic);

    int bIsAdd1 = Spellchecker_AddDictionary(spell, "en_US.aff", pAffData, nAffSize);
    int bIsAdd2 = Spellchecker_AddDictionary(spell, "en_US.dic", pDicData, nDicSize);
    while(1) {
        Spellchecker_Load(spell, "en_US.aff", "en_US.dic");
        Spellchecker_RemoveEngine(spell, "en_US.affen_US.dic");
        std::cout << Spellchecker_Debug() << std::endl;
    }

    // int nCheck1 = Spellchecker_Spell(spell, "hello");
    // int nCheck2 = Spellchecker_Spell(spell, "hellop");
    // unsigned char* pSuggestData = Spellchecker_Suggest(spell, "hellop");
    Spellchecker_Destroy(spell);
    return 0;
}
