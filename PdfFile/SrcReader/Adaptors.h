/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
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
#include <vector>

#include "../lib/goo/GList.h"
#include "../lib/goo/GHash.h"

#define GrClipEOFlag 0x01

class GlobalParamsAdaptor : public GlobalParams
{
private:
    std::wstring m_wsTempFolder;
    std::wstring m_wsCMapFolder;

    BYTE* m_bCMapData;
    DWORD m_nCMapDataLength;

    bool m_bDrawFormField;
	std::vector<double> m_arrRedactBox;

public:
    GlobalParamsAdaptor(const char *filename) : GlobalParams(filename)
    {
        m_bCMapData = NULL;
        m_nCMapDataLength = 0;
        m_bDrawFormField = false;
    }
    virtual ~GlobalParamsAdaptor()
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

    bool IsNeedCMap() { return !m_bCMapData; }
    void SetCMapFolder(const std::wstring &wsFolder);
    void SetCMapFile(const std::wstring &wsFile);
    void SetCMapMemory(BYTE* pData, DWORD nSizeData);
    bool GetCMap(const char* sName, char*& pData, unsigned int& nSize);

	void setDrawFormField(bool bDrawFormField) { m_bDrawFormField = bDrawFormField; }
    bool getDrawFormField() { return m_bDrawFormField; }

	void AddRedact(const std::vector<double>& arrRedactBox);
	bool InRedact(double dX, double dY);
	void ClearRedact();
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
        // Find '.' starting from end of path, and split path into extension and remaining part
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
