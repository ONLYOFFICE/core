//
// Created by danya on 23.06.2021.
//

#include "Adaptors.h"

void GlobalParamsAdaptor::SetFontManager(NSFonts::IFontManager *pFontManager)
{
    m_pFontManager = pFontManager;
}

bool operator==(const Ref &a, const Ref &b)
{
    return a.gen == b.gen && a.num == b.gen;
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
}


void XMLConverter::XRefToXml(XRef &xref, std::wstring &wsXml, bool bParseStreams)
{
    for (int nNum = 0; nNum < xref.getSize(); ++nNum)
    {
        XRefEntry *pEntry = xref.getEntry(nNum);
        if (xrefEntryFree != pEntry->type)
        {

            Object oTemp;
            xref.fetch(nNum, 0/*pEntry->nGen*/, &oTemp);

            if (oTemp.isDict() || oTemp.isStream())
            {
                if (xrefEntryCompressed == pEntry->type)
                    wsXml +=
                            L"<Obj num=\"" + std::to_wstring(nNum) + L"\" gen=\"" + std::to_wstring(0/*pEntry->nGen*/) +
                            L"\" compressed=\"true\">";
                else
                    wsXml +=
                            L"<Obj num=\"" + std::to_wstring(nNum) + L"\" gen=\"" + std::to_wstring(0/*pEntry->nGen*/) +
                            L"\">";

                if (oTemp.isDict())
                {
                    ObjectToXml(&oTemp, wsXml);
                } else if (oTemp.isStream())
                {
                    Dict *pStreamDict = oTemp.streamGetDict();
                    StreamDictToXml(pStreamDict, wsXml);
                    wsXml += L"<Stream>";

                    if (bParseStreams)
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
                                        AppendStringToXml(wsTemp, std::string(sTemp));
                                }
                            }
                        } else
                        {
                            wsXml += L"BinaryData";
                        }

                        wsXml += wsTemp;
                    }

                    wsXml += L"</Stream>";
                }

                wsXml += L"</Obj>";
            }

            oTemp.free();
        }
    }
}

void XMLConverter::ObjectToXml(Object *obj, std::wstring &wsXml)
{
    Object oTemp;

    switch (obj->getType())
    {
        case objBool:
            wsXml += obj->getBool() ? L"true" : L"false";
            break;
        case objInt:
            wsXml += std::to_wstring(obj->getInt());
            break;
        case objReal:
            wsXml += std::to_wstring(obj->getReal());
            break;
        case objString:
            wsXml += L"(";
            AppendStringToXml(wsXml, obj->getString()->getCString());
            wsXml += L")";
            break;
        case objName:
            wsXml += L"/";
            AppendStringToXml(wsXml, obj->getName());
            break;
        case objNull:
            wsXml += L"null";
            break;
        case objArray:
            wsXml += L"[";
            for (int nIndex = 0; nIndex < obj->arrayGetLength(); ++nIndex)
            {
                if (nIndex > 0)
                    wsXml += L" ";

                obj->arrayGet(nIndex, &oTemp);
                ObjectToXml(&oTemp, wsXml);
                oTemp.free();
            }
            wsXml += L"]";
            break;
        case objDict:
            for (int nIndex = 0; nIndex < obj->dictGetLength(); ++nIndex)
            {
                char *sKey = obj->dictGetKey(nIndex);
                wsXml += L"<";
                AppendStringToXml(wsXml, sKey);
                wsXml += L">";
                obj->dictGetVal(nIndex, &oTemp);
                ObjectToXml(&oTemp, wsXml);
                oTemp.free();
                wsXml += L"</";
                AppendStringToXml(wsXml, sKey);
                wsXml += L">";
            }
            break;
        case objStream:
            wsXml += L"<stream/>";
            // TODO: Запись стрима
            break;
        case objRef:
            wsXml += std::to_wstring(obj->getRefNum());
            wsXml += L" ";
            wsXml += std::to_wstring(obj->getRefGen());
            wsXml += L" R";
            break;
        case objCmd:
            AppendStringToXml(wsXml, obj->getCmd());
            break;
        case objError:
            wsXml += L"error";
            break;
        case objEOF:
            wsXml += L"EOF";
            break;
        case objNone:
            wsXml += L"none";
            break;
    }
}

void XMLConverter::StreamDictToXml(Dict *dict, std::wstring &wsXml)
{
    Object oTemp;
    for (int nIndex = 0; nIndex < dict->getLength(); ++nIndex)
    {
        char *sKey = dict->getKey(nIndex);
        wsXml += L"<";
        AppendStringToXml(wsXml, sKey);
        wsXml += L">";
        dict->getVal(nIndex, &oTemp);
        ObjectToXml(&oTemp, wsXml);
        oTemp.free();
        wsXml += L"</";
        AppendStringToXml(wsXml, sKey);
        wsXml += L">";
    }
}
