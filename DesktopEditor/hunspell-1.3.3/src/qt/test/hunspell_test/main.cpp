#include <QCoreApplication>
#include <QString>

#include "../../../../src/hunspell/hunspell.h"
#include "../../../../../../common/File.h"
#include <string>
#include <tchar.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if 1
    std::wstring sPathAff = L"D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\nodeJSProjects\\SpellChecker\\Dictionaries\\ru_RU\\ru_RU.aff";
    std::wstring sPathDic = L"D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\nodeJSProjects\\SpellChecker\\Dictionaries\\ru_RU\\ru_RU.dic";

    std::wstring sWord = L"привет";
#else
    std::wstring sPathAff = L"D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\nodeJSProjects\\SpellChecker\\Dictionaries\\en_US\\en_US.aff";
    std::wstring sPathDic = L"D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\nodeJSProjects\\SpellChecker\\Dictionaries\\en_US\\en_US.dic";

    std::wstring sWord = L"hello";
#endif

    std::string sPathAffA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sPathAff);
    std::string sPathDicA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sPathDic);
    std::string sWordA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sWord);

    Hunhandle* pDictionary = Hunspell_create(sPathAffA.c_str(), sPathDicA.c_str());
    char* pEncoding = Hunspell_get_dic_encoding(pDictionary);

    int nSpellResult = Hunspell_spell(pDictionary, sWordA.c_str());

    char** pSuggest;
    int nSuggestCount = Hunspell_suggest(pDictionary, &pSuggest, sWordA.c_str());

    _setmode(_fileno(stdout), _O_U16TEXT);

    for (int i = 0; i < nSuggestCount; ++i)
    {
        std::wstring q = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pSuggest[i], (LONG)strlen(pSuggest[i]));
        std::wcout << q.c_str() << std::endl;
    }

    Hunspell_destroy(pDictionary);

    return a.exec();
}
