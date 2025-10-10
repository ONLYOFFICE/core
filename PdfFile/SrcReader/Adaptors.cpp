/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "Adaptors.h"
#include "../lib/xpdf/NameToCharCode.h"
#include "../lib/xpdf/TextString.h"
#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"

void GlobalParamsAdaptor::SetCMapFolder(const std::wstring &wsFolder)
{
    m_wsCMapFolder = wsFolder;

	GString* sFolder = NSStrings::CreateString(wsFolder);
	if (!sFolder)
		return;

	unicodeMaps->add(new GString("ISO-8859-6"),   sFolder->copy()->append("/ISO-8859-6.unicodeMap"));
	unicodeMaps->add(new GString("ISO-2022-CN"),  sFolder->copy()->append("/ISO-2022-CN.unicodeMap"));
	unicodeMaps->add(new GString("EUC-CN"),       sFolder->copy()->append("/EUC-CN.unicodeMap"));
	unicodeMaps->add(new GString("GBK"),          sFolder->copy()->append("/GBK.unicodeMap"));
	unicodeMaps->add(new GString("KOI8-R"),       sFolder->copy()->append("/KOI8-R.unicodeMap"));
	unicodeMaps->add(new GString("ISO-8859-7"),   sFolder->copy()->append("/ISO-8859-7.unicodeMap"));
	unicodeMaps->add(new GString("ISO-8859-8"),   sFolder->copy()->append("/ISO-8859-8.unicodeMap"));
	unicodeMaps->add(new GString("Windows-1255"), sFolder->copy()->append("/Windows-1255.unicodeMap"));
	unicodeMaps->add(new GString("ISO-2022-JP"),  sFolder->copy()->append("/ISO-2022-JP.unicodeMap"));
	unicodeMaps->add(new GString("EUC-JP"),       sFolder->copy()->append("/EUC-JP.unicodeMap"));
	unicodeMaps->add(new GString("Shift-JIS"),    sFolder->copy()->append("/Shift-JIS.unicodeMap"));
	unicodeMaps->add(new GString("ISO-2022-KR"),  sFolder->copy()->append("/ISO-2022-KR.unicodeMap"));
	unicodeMaps->add(new GString("TIS-620"),      sFolder->copy()->append("/TIS-620.unicodeMap"));
	unicodeMaps->add(new GString("ISO-8859-9"),   sFolder->copy()->append("/ISO-8859-9.unicodeMap"));
	unicodeMaps->add(new GString("Latin2"),       sFolder->copy()->append("/Latin2.unicodeMap"));

	AddNameToUnicode(GString(sFolder->getCString()).append("/Bulgarian.nameToUnicode")->getCString());
	AddNameToUnicode(GString(sFolder->getCString()).append("/Greek.nameToUnicode")->getCString());
	AddNameToUnicode(GString(sFolder->getCString()).append("/Thai.nameToUnicode")->getCString());

    AddAllCMap(sFolder);

    toUnicodeDirs->append(sFolder->copy());

    delete sFolder;
}
void GlobalParamsAdaptor::AddNameToUnicode(const char* sFile)
{
	char *tok1, *tok2;
	FILE *f;
	char buf[256];
	Unicode u;

	if (!(f = openFile(sFile, "r")))
	  return;

	while (getLine(buf, sizeof(buf), f))
	{
		tok1 = strtok(buf, " \t\r\n");
		tok2 = strtok(NULL, " \t\r\n");
		if (tok1 && tok2)
		{
			sscanf(tok1, "%x", &u);
			nameToUnicode->add(tok2, u);
		}
	}

	fclose(f);
}
void GlobalParamsAdaptor::AddAllCMap(GString* sFolder)
{
    if (cidToUnicodes->lookup("Adobe-GB1"))
        return;

    cidToUnicodes->add(new GString("Adobe-GB1"), sFolder ? sFolder->copy()->append("/Adobe-GB1.cidToUnicode") : new GString());
    cidToUnicodes->add(new GString("Adobe-CNS1"), sFolder ? sFolder->copy()->append("/Adobe-CNS1.cidToUnicode") : new GString());
    cidToUnicodes->add(new GString("Adobe-Korea1"), sFolder ? sFolder->copy()->append("/Adobe-Korea1.cidToUnicode") : new GString());
    cidToUnicodes->add(new GString("Adobe-KR"), sFolder ? sFolder->copy()->append("/Adobe-KR.cidToUnicode") : new GString());
    cidToUnicodes->add(new GString("Adobe-Japan1"), sFolder ? sFolder->copy()->append("/Adobe-Japan1.cidToUnicode") : new GString());

    if (sFolder)
        sFolder->append("/CMap");

    AddCMapFolder("Adobe-GB1", sFolder);
    AddCMapFolder("Adobe-CNS1", sFolder);
    AddCMapFolder("Adobe-Japan1", sFolder);
    AddCMapFolder("Adobe-Korea1", sFolder);
    AddCMapFolder("Adobe-KR", sFolder);
}
void GlobalParamsAdaptor::AddCMapFolder(const char* sCollection, GString* sFolder)
{
	GList *pList = new GList();
	if (!pList)
		return;

    if (sFolder)
    {
        pList->append(sFolder->copy());
    }

	cMapDirs->add(new GString(sCollection), pList);
}
void GlobalParamsAdaptor::SetCMapFile(const std::wstring &wsFile)
{
    AddAllCMap(NULL);

#ifndef BUILDING_WASM_MODULE
	NSFile::CFileBinary::ReadAllBytes(wsFile, &m_bCMapData, m_nCMapDataLength);
#endif
}
void GlobalParamsAdaptor::SetCMapMemory(BYTE* pData, DWORD nSizeData)
{
    AddAllCMap(NULL);

    if (pData)
    {
        m_bCMapData = pData;
        m_nCMapDataLength = nSizeData;
    }
}

DWORD GetLength(BYTE* x)
{
    return x ? (x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24) : 4;
}
bool GlobalParamsAdaptor::GetCMap(const char* sName, char*& pData, unsigned int& nSize)
{
    if (!m_bCMapData)
    {
        if (m_wsCMapFolder.empty())
            SetCMapFile(NSFile::GetProcessDirectory() + L"/cmap.bin");
        if (!m_bCMapData)
            return false;
    }

    DWORD i = 0;
    while (i < m_nCMapDataLength)
    {
        DWORD nPathLength = GetLength(m_bCMapData + i);
        i += 4;
        std::string sName1 = std::string((char*)(m_bCMapData + i), nPathLength);
        i += nPathLength;

        nPathLength = GetLength(m_bCMapData + i);
        i += 4;
        if (sName1 == std::string(sName))
        {
            pData = (char*)(m_bCMapData + i);
            nSize = nPathLength;
            return true;
        }
        else
        {
            i += nPathLength;
        }
    }

    return false;
}

void GlobalParamsAdaptor::AddRedact(const std::vector<double>& arrRedactBox)
{
	m_arrRedactBox.insert(m_arrRedactBox.end(), arrRedactBox.begin(), arrRedactBox.end());
}
double crossProduct(double x1, double y1, double x2, double y2, double x3, double y3)
{
	return (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
}
bool GlobalParamsAdaptor::InRedact(double dX, double dY)
{
	for (int i = 0; i < m_arrRedactBox.size(); i += 8)
	{
		double x1 = m_arrRedactBox[i + 0];
		double y1 = m_arrRedactBox[i + 1];
		double x2 = m_arrRedactBox[i + 2];
		double y2 = m_arrRedactBox[i + 3];
		double x3 = m_arrRedactBox[i + 6];
		double y3 = m_arrRedactBox[i + 7];
		double x4 = m_arrRedactBox[i + 4];
		double y4 = m_arrRedactBox[i + 5];

		// Проверяем знаки векторных произведений для всех сторон
		double cross1 = crossProduct(x1, y1, x2, y2, dX, dY);
		double cross2 = crossProduct(x2, y2, x3, y3, dX, dY);
		double cross3 = crossProduct(x3, y3, x4, y4, dX, dY);
		double cross4 = crossProduct(x4, y4, x1, y1, dX, dY);

		// Точка внутри, если все векторные произведения имеют одинаковый знак
		if ((cross1 >= 0 && cross2 >= 0 && cross3 >= 0 && cross4 >= 0) ||
			(cross1 <= 0 && cross2 <= 0 && cross3 <= 0 && cross4 <= 0))
			return true;
	}
	return false;
}
void GlobalParamsAdaptor::ClearRedact()
{
	m_arrRedactBox.clear();
}

bool operator==(const Ref &a, const Ref &b)
{
	return a.gen == b.gen && a.num == b.num;
}

bool operator<(const Ref &a, const Ref &b)
{
    if (a.num < b.num)
        return true;
    else if (a.num == b.num)
        return a.gen < b.gen;
    else
        return false;
}

bool operator<=(const Ref &a, const Ref &b)
{
    return (a < b) || (a == b);
}

bool operator>=(const Ref &a, const Ref &b)
{
    return !(a < b);
}

bool operator>(const Ref &a, const Ref &b)
{
    return !(a <= b);
}

namespace NSStrings
{
    GString *CreateString(const std::wstring &str)
    {
        std::string sUtf8 = U_TO_UTF8(str);
        return new GString(sUtf8.c_str(), sUtf8.length());
    }

    GString *CreateString(const std::string &str)
    {
        return new GString(str.c_str(), str.length());
    }

    std::wstring GetString(GString *str)
    {
        if (!str)
            return L"";
        return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE *) str->getCString(), (LONG) str->getLength());
    }

    std::string GetStringA(GString *str)
    {
        return std::string(str->getCString(), str->getLength());
    }

    std::wstring GetStringFromUTF32(GString* str)
    {
        if (!str)
            return L"";
        TextString* s = new TextString(str);
        std::wstring sValue = NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength());
        delete s;
        return sValue;
    }

    std::string GetStringAFromUTF32(GString* str)
    {
        if (!str)
            return "";
        TextString* s = new TextString(str);
        std::string sValue = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
        delete s;
        return sValue;
    }
}

void AppendStringToXml(std::wstring& wsXml, const std::string& sString)
{
	std::wstring wsTmp(sString.begin(), sString.end());
	wsXml += wsTmp;
}
void DictToXmlR(Object* obj, std::wstring& wsXml, bool bBinary)
{
    Object oTemp;

    wsXml += L" type=\"";
    switch (obj->getType())
    {
    case objBool:
        wsXml += L"Bool\" num=\"";
        wsXml += obj->getBool() ? L"true" : L"false";
        wsXml += L"\">";
        break;
    case objInt:
        wsXml += L"Int\" num=\"";
        wsXml += std::to_wstring(obj->getInt());
        wsXml += L"\">";
        break;
    case objReal:
        wsXml += L"Real\" num=\"";
        wsXml += std::to_wstring(obj->getReal());
        wsXml += L"\">";
        break;
    case objString:
        if (bBinary)
        {
            GString* str = obj->getString();
            wsXml += L"Binary\" num=\"";
            wsXml += std::to_wstring(str->getLength());
            wsXml += L"\">";
            for (int nIndex = 0; nIndex < str->getLength(); ++nIndex)
            {
                wsXml += L"<i>";
                wsXml += std::to_wstring((int)str->getChar(nIndex));
                wsXml += L"</i>";
            }
        }
        else
        {
            TextString* s = new TextString(obj->getString());
            std::wstring sValue = NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength());
            wsXml += L"String\" num=\"";
            wsXml += sValue;
            wsXml += L"\">";
            delete s;
        }
        break;
    case objName:
        wsXml += L"Name\" num=\"";
		AppendStringToXml(wsXml, obj->getName());
        wsXml += L"\">";
        break;
    case objNull:
        wsXml += L"Null\">";
        break;
    case objArray:
        wsXml += L"Array\">";
        for (int nIndex = 0; nIndex < obj->arrayGetLength(); ++nIndex)
        {
            wsXml += L"<item";
            obj->arrayGetNF(nIndex, &oTemp);
            DictToXmlR(&oTemp, wsXml, bBinary);
            oTemp.free();
            wsXml += L"</item>";
        }
        break;
    case objDict:
        wsXml += L"Dict\">";
        for (int nIndex = 0; nIndex < obj->dictGetLength(); ++nIndex)
        {
            char *sKey = obj->dictGetKey(nIndex);
            wsXml += L"<";
			AppendStringToXml(wsXml, sKey);
            if (strcmp("Resources", sKey) == 0 || strcmp("AcroForm", sKey) == 0)
                obj->dictGetVal(nIndex, &oTemp);
            else
                obj->dictGetValNF(nIndex, &oTemp);
            DictToXmlR(&oTemp, wsXml, strcmp("ID", sKey) == 0 ? true : bBinary);
            oTemp.free();
            wsXml += L"</";
			AppendStringToXml(wsXml, sKey);
            wsXml += L">";
        }
        break;
    case objStream:
        wsXml += L"Stream\">";
        break;
    case objRef:
        wsXml += L"Ref\" gen=\"";
        wsXml += std::to_wstring(obj->getRefGen());
        wsXml += L"\" num=\"";
        wsXml += std::to_wstring(obj->getRefNum());
        wsXml += L"\">";
        break;
    case objCmd:
        wsXml += L"Cmd\" num=\"";
		AppendStringToXml(wsXml, obj->getCmd());
        wsXml += L"\">";
        break;
    case objError:
        wsXml += L"Error\">";
        break;
    case objEOF:
        wsXml += L"EOF\">";
        break;
    case objNone:
        wsXml += L"None\">";
        break;
    }
}
std::wstring XMLConverter::DictToXml(const std::wstring& wsName, Object* obj, int nNum, int nGen, bool bBinary)
{
    std::wstring sRes = L"<" + wsName;
    if (nNum > 0)
    {
        sRes += (L" num=\"" + std::to_wstring(nNum) + L"\"");
        sRes += (L" gen=\"" + std::to_wstring(nGen) + L"\"");
    }
    DictToXmlR(obj, sRes, bBinary);
    sRes += (L"</" + wsName + L">");
    return sRes;
}

XMLConverter::XMLConverter(XRef* pXRef, bool isParseStreams)
{
	m_pXRef = pXRef;
	m_bParseStreams = isParseStreams;
	m_nNumMax = 0;

	ParseDicts();
	PdfToXml();
}
void XMLConverter::ParseDicts()
{
	m_nNumMax = m_pXRef->getSize() + 1000;

	for (int nNum = 0, nCount = m_pXRef->getSize(); nNum < nCount; ++nNum)
	{
		XRefEntry *pEntry = m_pXRef->getEntry(nNum);
		if (xrefEntryFree != pEntry->type)
		{
			Object oTemp;
			m_pXRef->fetch(nNum, 0, &oTemp);

			if (oTemp.isDict() || oTemp.isStream())
			{
				Dict* pDict = NULL;
				if (oTemp.isDict())
					pDict = oTemp.getDict();
				else if (oTemp.isStream())
					pDict = oTemp.streamGetDict();

				if (pDict)
					m_mDict.insert({pDict, nNum});
			}

			oTemp.free();
		}
	}
}
void XMLConverter::ObjectToXml(Object *pObject, bool isSkipCheck)
{
	Object oTemp;

	switch (pObject->getType())
	{
		case objBool:
			m_wsXml += pObject->getBool() ? L"true" : L"false";
			break;
		case objInt:
			m_wsXml += std::to_wstring(pObject->getInt());
			break;
		case objReal:
			m_wsXml += std::to_wstring(pObject->getReal());
			break;
		case objString:
			m_wsXml += L"(";
			Append(pObject->getString());
			m_wsXml += L")";
			break;
		case objName:
			m_wsXml += L"/";
			Append(pObject->getName());
			break;
		case objNull:
			m_wsXml += L"null";
			break;
		case objArray:
			m_wsXml += L"[";
			for (int nIndex = 0; nIndex < pObject->arrayGetLength(); ++nIndex)
			{
				if (nIndex > 0)
					m_wsXml += L" ";

				pObject->arrayGet(nIndex, &oTemp);
				ObjectToXml(&oTemp);
			}
			m_wsXml += L"]";
			break;
		case objDict:
		{
			Dict* pDict = pObject->getDict();
			if (!isSkipCheck && CheckDict(pDict))
				return;

			DictToXml(pDict);
			break;
		}
		case objStream:
			m_wsXml += L"<stream/>";
			// TODO: Запись стрима
			break;
		case objRef:
			m_wsXml += std::to_wstring(pObject->getRefNum());
			m_wsXml += L" ";
			m_wsXml += std::to_wstring(pObject->getRefGen());
			m_wsXml += L" R";
			break;
		case objCmd:
			AppendStringToXml(m_wsXml, pObject->getCmd());
			break;
		case objError:
			m_wsXml += L"error";
			break;
		case objEOF:
			m_wsXml += L"EOF";
			break;
		case objNone:
			m_wsXml += L"none";
			break;
	}
}
void XMLConverter::PdfToXml()
{
	std::vector<Dict*> vPassed;
	for (int nNum = 0, nCount = m_pXRef->getSize(); nNum < nCount; ++nNum)
	{
		XRefEntry *pEntry = m_pXRef->getEntry(nNum);
		if (xrefEntryFree != pEntry->type)
		{
			Object oTemp;
			m_pXRef->fetch(nNum, 0, &oTemp);

			if (oTemp.isDict() || oTemp.isStream())
			{
				if (xrefEntryCompressed == pEntry->type)
					m_wsXml +=
							L"<Obj num=\"" + std::to_wstring(nNum) + L"\" gen=\"" + std::to_wstring(0/*pEntry->nGen*/) +
							L"\" compressed=\"true\">";
				else
					m_wsXml +=
							L"<Obj num=\"" + std::to_wstring(nNum) + L"\" gen=\"" + std::to_wstring(0/*pEntry->nGen*/) +
							L"\">";

				if (oTemp.isDict())
				{
					ObjectToXml(&oTemp, true);
				}
				else if (oTemp.isStream())
				{
					Dict *pStreamDict = oTemp.streamGetDict();
					StreamDictToXml(pStreamDict, true);
					m_wsXml += L"<Stream>";

					if (m_bParseStreams)
					{
						std::wstring wsTemp;
						Stream *pStream = oTemp.getStream();
						pStream->reset();

						Object oFilter;
						pStreamDict->lookup("Filter", &oFilter);
						if (oFilter.isNull() || oFilter.isName("FlateDecode"))
						{
							int nChar;
							while (EOF != (nChar = pStream->getChar()))
							{
								char sTemp[1] = {(char) nChar};

								switch (sTemp[0])
								{
									case '\"':
										wsTemp += L"&quot;";
										break;
									case '&':
										wsTemp += L"&amp;";
										break;
									case '<':
										wsTemp += L"&lt;";
										break;
									case '>':
										wsTemp += L"&gt;";
										break;
									default:
										AppendStringToXml(wsTemp, sTemp);
								}
							}
						}
						else
						{
							m_wsXml += L"BinaryData";
						}

						m_wsXml += wsTemp;
					}

					m_wsXml += L"</Stream>";
				}

				m_wsXml += L"</Obj>";
			}

			oTemp.free();
		}
	}
}
void XMLConverter::StreamDictToXml(Dict *pStreamDict, bool isSkipCheck)
{
	if (!isSkipCheck && CheckDict(pStreamDict))
		return;

    Object oTemp;
	for (int nIndex = 0, nCount = pStreamDict->getLength(); nIndex < nCount; ++nIndex)
    {
		char *sKey = pStreamDict->getKey(nIndex);
		m_wsXml += L"<";
		Append(sKey);
		m_wsXml += L">";
		pStreamDict->getVal(nIndex, &oTemp);
		ObjectToXml(&oTemp);
        oTemp.free();
		m_wsXml += L"</";
		Append(sKey);
		m_wsXml += L">";
    }
}
void XMLConverter::DictToXml(Dict* pDict)
{
	Object oTemp;
	for (int nIndex = 0, nCount = pDict->getLength(); nIndex < nCount; ++nIndex)
	{
		char *sKey = pDict->getKey(nIndex);
		m_wsXml += L"<";
		Append(sKey);
		m_wsXml += L">";
		pDict->getVal(nIndex, &oTemp);
		ObjectToXml(&oTemp);
		m_wsXml += L"</";
		Append(sKey);
		m_wsXml += L">";
	}
}
bool XMLConverter::CheckDict(Dict *pDict)
{
	std::map<Dict*, int>::iterator iter = m_mDict.find(pDict);
	if (iter != m_mDict.end())
	{
		m_wsXml += std::to_wstring(iter->second) + L" 0 R";
		return true;
	}

	m_mDict.insert({pDict, ++m_nNumMax});
	return false;
}
