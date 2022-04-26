//
// Contains classes and functions which are necessary to xpdf to work with PDFReader
//

#ifndef CORE_ADAPTORS_H
#define CORE_ADAPTORS_H

#include "../lib/xpdf/GlobalParams.h"
#include "../lib/xpdf/Object.h"
#include "../lib/xpdf/XRef.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/common/StringExt.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include <string>

#include "../lib/goo/GList.h"
#include "../lib/goo/GHash.h"

#define GrClipEOFlag 0x01

class GlobalParamsAdaptor : public GlobalParams
{
    std::wstring m_wsTempFolder;
    std::wstring m_wsCMapFolder;
public:
    NSFonts::IFontManager *m_pFontManager;
    GlobalParamsAdaptor(const char *filename) : GlobalParams(filename) {}

    void SetFontManager(NSFonts::IFontManager* pFontManager);

    std::wstring GetTempFolder()
    {
        return m_wsTempFolder;
    }
    void SetTempFolder(const std::wstring &folder)
    {
        m_wsTempFolder = folder;
    }

	void SetCMapFolder(const std::wstring &wsFolder);
private:

	void AddNameToUnicode(const char* sFile);
	void AddCMapFolder(const char* sCollection, GString* sFolder);
};

#ifndef CORE_REF_OPERATORS
#define CORE_REF_OPERATORS

// Ref operators

bool operator==(const Ref &a, const Ref &b);

bool operator<(const Ref &a, const Ref &b);

bool operator<=(const Ref &a, const Ref &b);

bool operator>=(const Ref &a, const Ref &b);
bool operator>(const Ref &a, const Ref &b);


#endif
 //String functions


static std::wstring* AStringToPWString(const char* sString)
{
    return new std::wstring(NSStringExt::CConverter::GetUnicodeFromSingleByteString((unsigned char*)sString, (long)strlen(sString)));
}
static std::wstring AStringToWString(const char* sString)
{
    return std::wstring(NSStringExt::CConverter::GetUnicodeFromSingleByteString((unsigned char*)sString, (long)strlen(sString)));
}

namespace NSStrings
{
    GString* CreateString(const std::wstring& str);
    GString* CreateString(const std::string& str);

    std::wstring GetString(GString* str);
    std::string GetStringA(GString* str);
}

namespace PdfReader
{
    static void SpitPathExt(std::wstring& wsFullPath, std::wstring* pwsFilePath, std::wstring* pwsExt)
    {
        // Ищем '.' начиная с конца пути, и разделяем путь на расширение и остальную часть
        unsigned int nPos = wsFullPath.find_last_of(L".");
        *pwsFilePath = wsFullPath.substr(0, nPos);
        *pwsExt      = wsFullPath.substr(nPos + 1);
    }
    static bool OpenTempFile(std::wstring* pwsName, FILE **ppFile, wchar_t *wsMode, wchar_t *wsExt, wchar_t *wsFolder, wchar_t *wsName = NULL)
    {
        return NSFile::CFileBinary::OpenTempFile(pwsName, ppFile, wsMode, wsExt, wsFolder, wsName);
    }
    static char*GetLine(char *sBuffer, int nSize, FILE *pFile)
    {
        int nChar, nCurIndex = 0;

        while (nCurIndex < nSize - 1)
        {
            if ((nChar = fgetc(pFile)) == EOF)
                break;

            sBuffer[nCurIndex++] = (char)nChar;
            if ('\x0a' == nChar)
            {
                break;
            }
            if ('\x0d' == nChar)
            {
                nChar = fgetc(pFile);
                if ('\x0a' == nChar && nCurIndex < nSize - 1)
                {
                    sBuffer[nCurIndex++] = (char)nChar;
                }
                else if (EOF != nChar)
                {
                    ungetc(nChar, pFile);
                }
                break;
            }
        }
        sBuffer[nCurIndex] = '\0';
        if (0 == nCurIndex)
            return NULL;
        return sBuffer;
    }
}

class XMLConverter {
public:
    static void XRefToXml(XRef &xref, std::wstring &wsXml, bool parse_streams);
    static void StreamDictToXml(Dict *dict, std::wstring &wsXml);
    static void ObjectToXml(Object *obj, std::wstring &wsXml);

    static void AppendStringToXml(std::wstring& wsXml, const std::string& sString)
    {
        std::wstring wsTmp(sString.begin(), sString.end());
        wsXml += wsTmp;
    }
};
#endif //CORE_ADAPTORS_H
