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

    BYTE* m_bCMapData;
    DWORD m_nCMapDataLength;
public:
    NSFonts::IFontManager *m_pFontManager;
    GlobalParamsAdaptor(const char *filename) : GlobalParams(filename)
    {
        m_bCMapData = NULL;
        m_nCMapDataLength = 0;
    }
    ~GlobalParamsAdaptor()
    {
        RELEASEARRAYOBJECTS(m_bCMapData);
    }

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
    void SetCMapFile(const std::wstring &wsFile);
    void SetCMapMemory(BYTE* pData, DWORD nSizeData);
    bool GetCMap(const char* sName, char*& pData, unsigned int& nSize);
private:

	void AddNameToUnicode(const char* sFile);
    void AddAllCMap(GString* sFolder);
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

    std::wstring GetStringFromUTF32(GString* str);
    std::string GetStringAFromUTF32(GString* str);
}

namespace PdfReader
{
    static void SpitPathExt(std::wstring& wsFullPath, std::wstring* pwsFilePath, std::wstring* pwsExt)
    {
        // Ищем '.' начиная с конца пути, и разделяем путь на расширение и остальную часть
        size_t nPos = wsFullPath.find_last_of(L".");
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

class XMLConverter
{
public:
	XMLConverter(XRef* xref, bool isParseStreams = false);
	const std::wstring& GetXml() const {return m_wsXml;};

	static void XRefToXml(XRef* xref, std::wstring &wsXml, bool parse_streams);
    static std::wstring DictToXml(const std::wstring& wsName, Object* obj, int nNum = 0, int nGen = 0, bool bBinary = false);

private:

	void ParseDicts();
	void PdfToXml();
	void ObjectToXml(Object* pObject, bool isSkipCheck = false);
	void StreamDictToXml(Dict* pStreamDict, bool isSkipCheck = false);
	void DictToXml(Dict* pDict);
	bool CheckDict(Dict* pDict);
	void Append(const char* sString)
	{
		std::string s(sString);
		std::wstring wsTmp(s.begin(), s.end());
		m_wsXml += wsTmp;
	}
	void Append(GString* pString)
	{
		for (int nPos = 0, nLen = pString->getLength(); nPos < nLen; ++nPos)
		{
			m_wsXml.push_back(std::btowc(pString->getChar(nPos)));
		}
	}

private:

	XRef*                m_pXRef;
	bool                 m_bParseStreams;
	std::map<Dict*, int> m_mDict;
	std::wstring         m_wsXml;
	int                  m_nNumMax;
};
#endif //CORE_ADAPTORS_H
