#ifndef STATICFUNCTIONS_H
#define STATICFUNCTIONS_H

#include "../../../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../../../DesktopEditor/common/File.h"
#include "CssCalculator_global.h"
#include <string>
#include <vector>
#include <algorithm>


namespace NSCSS
{
    namespace NS_STATIC_FUNCTIONS
    {
        inline static std::wstring stringToWstring(const std::string& sString)
        {
            return UTF8_TO_U(sString);
        }

        inline static std::string wstringToString(const std::wstring& sWstring)
        {
            return U_TO_UTF8(sWstring);
        }

        inline static std::string GetContentAsUTF8(const std::string &sString, const std::wstring &sEncoding)
        {
            const std::string& sEnc = U_TO_UTF8(sEncoding);

            NSUnicodeConverter::CUnicodeConverter oConverter;
            const std::wstring& sUnicodeContent = oConverter.toUnicode(sString, sEnc.c_str());
            return U_TO_UTF8(sUnicodeContent);
        }

        inline static std::string GetContentAsUTF8(const std::wstring& sFileName)
        {
            std::string sFileContent;

            // читаем файл как есть. utf-8 тут просто название.
            if(!NSFile::CFileBinary::ReadAllTextUtf8A(sFileName, sFileContent))
                return sFileContent;

            std::string sEncoding;
            if (true)
            {
                // определяем кодировку
                const std::string::size_type& posCharset = sFileContent.find("@charset");

                if (std::string::npos != posCharset)
                {
                    std::string::size_type pos1 = sFileContent.find_first_of("\"';", posCharset);
                    if (std::string::npos != pos1)
                    {
                        pos1 += 1;
                        std::string::size_type pos2 = sFileContent.find_first_of("\"';", pos1);
                        if (std::string::npos != pos2)
                        {
                            sEncoding = sFileContent.substr(pos1, pos2 - pos1);
                        }

                        // check valid
                        if (std::string::npos != sEncoding.find_first_of(" \n\r\t\f\v"))
                            sEncoding = "";
                    }
                }
            }

            if (sEncoding.empty())
                sEncoding = "utf-8";


            if (!sEncoding.empty() && sEncoding != "utf-8" && sEncoding != "UTF-8")
            {
                NSUnicodeConverter::CUnicodeConverter oConverter;
                sFileContent = U_TO_UTF8(oConverter.toUnicode(sFileContent, sEncoding.c_str()));
            }

            return sFileContent;
        }

        inline static bool ThereIsNumber(const std::wstring& sString)
        {
            if (sString.empty())
                return false;

            size_t posDigit = sString.find_first_of(L"0123456789");
            size_t posNoDigit = sString.find_first_not_of(L" \n\r\t\f\v123456789.");

            if (posDigit == std::wstring::npos)
                return false;

            if (posNoDigit == std::wstring::npos)
                return true;

            if ((posDigit < posNoDigit) || (posDigit == 0  && posNoDigit == 0))
                return true;

            return false;
        }

        inline static void RemoveExcessFromStyles(std::wstring& sStyle)
        {
            while (true)
            {
                size_t posLeftAngleBracket = sStyle.find(L'<');
                size_t posRightAngleBracket = sStyle.find(L'>');
                size_t posLeftComment = sStyle.find(L"<!--");
                size_t posRightComment = sStyle.find(L"-->");

                if (posLeftAngleBracket != std::wstring::npos || posLeftComment != std::wstring::npos || posRightComment != std::wstring::npos)
                {

                    if (posLeftAngleBracket != std::wstring::npos &&
                        posRightAngleBracket != std::wstring::npos)
                    {
                        sStyle.erase(posLeftAngleBracket, posRightAngleBracket - posLeftAngleBracket + 1);
                    }
                    else if (posLeftAngleBracket != std::wstring::npos)
                        sStyle.erase(posLeftAngleBracket, 1);
                    else if (posRightAngleBracket != std::wstring::npos)
                        sStyle.erase(posRightAngleBracket, 1);
                    else if (posLeftComment != std::wstring::npos)
                        sStyle.erase(posLeftComment, 4);
                    else if (posRightComment != std::wstring::npos)
                        sStyle.erase(posRightComment, 3);
                }
                else
                    break;
            }
        }

        inline bool ConvertAbsoluteValue(std::wstring& sAbsoluteValue)
        {
            if (sAbsoluteValue.empty())
                return false;

            bool bIsConvert = false;

            std::map<std::wstring, std::wstring> arAbsoluteValues = {{L"xx-small", L"9px"},  {L"x-small", L"10px"},
                                                                     {L"small",    L"13px"}, {L"medium",  L"16px"},
                                                                     {L"large",    L"18px"}, {L"x-large", L"24px"},
                                                                     {L"xx-large", L"32px"}};

            for (const auto& sAbsValue : arAbsoluteValues)
            {
                while (sAbsoluteValue.find(sAbsValue.first) != std::wstring::npos)
                {
                    size_t nPos1 = sAbsoluteValue.find(sAbsValue.first);
                    size_t nPos2 = nPos1 + sAbsValue.first.length() - 1;

                    sAbsoluteValue.replace(nPos1, nPos2, sAbsValue.second);
                    bIsConvert = true;
                }
            }
            return bIsConvert;
        }

        inline std::wstring DeleteSpace(const std::wstring& sValue)
        {
            if (sValue.empty())
                return std::wstring();

            size_t start = sValue.find_first_not_of(L" \n\r\t\f\v");
            if (std::wstring::npos == start)
                return sValue;

            size_t end = sValue.find_last_not_of(L" \n\r\t\f\v"); // точно >=0
            return sValue.substr(start, end - start + 1);
        }

        inline std::vector<std::string> GetWords(const std::wstring& sLine)
        {
            if (sLine.empty())
                return {};

            std::vector<std::string> arWords;
            arWords.reserve(16);
            std::string sTemp = wstringToString(sLine);
            size_t posFirstNotSpace = sTemp.find_first_not_of(" \n\r\t\f\v:;,");
            while (posFirstNotSpace != std::wstring::npos)
            {
                const size_t& posLastNotSpace = sTemp.find_first_of(" \n\r\t\f\v:;,", posFirstNotSpace);
                arWords.push_back(sTemp.substr(posFirstNotSpace, posLastNotSpace - posFirstNotSpace));
                posFirstNotSpace = sTemp.find_first_not_of(" \n\r\t\f\v:;,", posLastNotSpace);
            }
            return arWords;
        }

        inline std::vector<std::wstring> GetWordsW(const std::wstring& sLine)
        {
            if (sLine.empty())
                return {};

            std::vector<std::wstring> arWords;
            arWords.reserve(16);
            size_t posFirstNotSpace = sLine.find_first_not_of(L" \n\r\t\f\v:;,");
            while (posFirstNotSpace != std::wstring::npos)
            {
                const size_t& posLastNotSpace = sLine.find_first_of(L" \n\r\t\f\v:;,", posFirstNotSpace);
                arWords.push_back(sLine.substr(posFirstNotSpace, posLastNotSpace - posFirstNotSpace));
                posFirstNotSpace = sLine.find_first_not_of(L" \n\r\t\f\v:;,", posLastNotSpace);
            }
            return arWords;
        }

        inline std::vector<std::string> GetSelectorsList(const std::wstring& sSelectors)
        {
            std::wstring sNames = sSelectors;
            std::wstring sClasses;
            std::wstring sIds;

            size_t posLattice = sNames.find(L"#");
            if (posLattice != std::wstring::npos)
            {
                sNames = sSelectors.substr(0, posLattice);
                sIds   = sSelectors.substr(posLattice);
            }

            size_t posPoint = sNames.find(L'.');
            if (posPoint != std::wstring::npos)
            {
                sNames   = sSelectors.substr(0, posPoint);
                sClasses = sSelectors.substr(posPoint);
                posLattice = sClasses.find(L'#');
                if (posLattice != std::wstring::npos)
                    sClasses = sClasses.substr(0, posLattice);
            }

            std::vector<std::string> arNames   = GetWords(sNames);
            std::vector<std::string> arClasses = GetWords(sClasses);
            std::vector<std::string> arIds     = GetWords(sIds);
            std::vector<std::string> arSelectors(arNames);
            arSelectors.reserve(arNames.size() * arClasses.size() + arNames.size() * arIds.size() + arClasses.size() * arIds.size());

            for (std::string& sClass : arClasses)
            {
                if (sClass.find('.') == std::string::npos)
                    sClass = '.' + sClass;
                arSelectors.push_back(sClass);
                for (const std::string& sName : arNames)
                    arSelectors.push_back(sName + sClass);
                for (const std::string& sId : arIds)
                    arSelectors.push_back(sClass + sId);
            }
            for (std::string& sId : arIds)
            {
                if (sId.find('#') == std::string::npos)
                    sId = '#' + sId;
                arSelectors.push_back(sId);
                for (const std::string& sName : arNames)
                    arSelectors.push_back(sName + sId);
            }
            return arSelectors;
        }

        inline std::vector<std::wstring> GetWordsWithSigns(const std::wstring& sLine)
        {
            if (sLine.empty())
                return {};
            std::vector<std::wstring> arWords;
            arWords.reserve(16);
            size_t posFirstNotSpace = sLine.find_first_not_of(L" \n\r\t\f\v:;,");
            while (posFirstNotSpace != std::wstring::npos)
            {
                size_t posLastNotSpace = sLine.find_first_of(L" \n\r\t\f\v:;,", posFirstNotSpace);
                arWords.push_back(sLine.substr(posFirstNotSpace, (posLastNotSpace != std::wstring::npos) ? posLastNotSpace - posFirstNotSpace + 1 : posLastNotSpace - posFirstNotSpace ));
                posFirstNotSpace = sLine.find_first_not_of(L" \n\r\t\f\v:;,", posLastNotSpace);
            }
            return arWords;
        }

        inline bool IsDigit(const std::wstring& sValue)
        {
            return sValue.empty() ? false : std::all_of(sValue.begin(), sValue.end(), [] (const wchar_t& cChar) { return iswdigit(cChar); });
        }
    }
}

#endif // STATICFUNCTIONS_H
