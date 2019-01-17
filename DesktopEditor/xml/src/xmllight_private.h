/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef _BUILD_XMLLIGHT_PRIVATE_CROSSPLATFORM_H_
#define _BUILD_XMLLIGHT_PRIVATE_CROSSPLATFORM_H_

#include "../../../DesktopEditor/common/File.h"

#include "../libxml2/libxml.h"
#include "../libxml2/include/libxml/xmlreader.h"
#include "../libxml2/include/libxml/c14n.h"
#include "../include/xmlutils.h"

namespace XmlUtils
{
    class CXmlLiteReader_Private
    {
    protected:
        xmlTextReaderPtr	reader;

        BYTE*				m_pStream;
        LONG				m_lStreamLen;
    public:

        CXmlLiteReader_Private()
        {
            reader = NULL;
            m_pStream = NULL;
        }
        virtual ~CXmlLiteReader_Private()
        {
            Clear();
        }
        xmlTextReaderPtr getNativeReader() { return reader; }

    public:

        inline void Clear()
        {
            xmlFreeTextReader(reader);
            reader = NULL;
            if (NULL != m_pStream)
                delete[]m_pStream;
            m_pStream = NULL;
            m_lStreamLen = 0;
        }
        inline bool IsValid()
        {
            return (NULL != reader);
        }

        inline bool FromFile(const wchar_t* sFilePath)
        {
            return FromFile(std::wstring(sFilePath));
        }
        inline bool FromFile(const std::wstring& sFilePath)
        {
            Clear();

            NSFile::CFileBinary oFile;
            if (oFile.OpenFile(sFilePath) == false) return false;

            m_lStreamLen = (LONG)oFile.GetFileSize();
            m_pStream = new BYTE[m_lStreamLen];
            DWORD dwRead = 0;
            oFile.ReadFile(m_pStream, (DWORD)m_lStreamLen, dwRead);
            oFile.CloseFile();

            reader = xmlReaderForMemory((char*)m_pStream, m_lStreamLen, NULL, NULL, 0);

            return true;
        }
        inline bool FromString(const wchar_t* sXml)
        {
            return FromString(std::wstring(sXml));
        }
        inline bool FromString(const std::wstring& sXml)
        {
            Clear();

            NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sXml.c_str(), sXml.length(), m_pStream, m_lStreamLen, false);
            reader = xmlReaderForMemory((char*)m_pStream, m_lStreamLen, NULL, NULL, 0);

            return true;
        }
        inline bool FromStringA(const std::string& sXml)
        {
            Clear();

            reader = xmlReaderForMemory((char*)sXml.c_str(), sXml.length(), NULL, NULL, 0);

            return true;
        }

        inline bool Read(XmlNodeType &oNodeType)
        {
            if (!IsValid())
                return false;

            if (1 != xmlTextReaderRead(reader))
                return false;

            int nTempType = xmlTextReaderNodeType(reader);
            if (-1 == nTempType)
                return false;

            oNodeType = (XmlNodeType)nTempType;

            return true;
        }
        inline bool ReadNextNode()
        {
            if (!IsValid())
                return false;

            XmlNodeType oNodeType = XmlNodeType_None;

            while (XmlNodeType_Element != oNodeType)
            {
                if (1 != xmlTextReaderRead(reader))
                    break;

                int nTempType = xmlTextReaderNodeType(reader);
                if (-1 == nTempType)
                    break;

                oNodeType = (XmlNodeType)nTempType;
            }

            if (XmlNodeType_Element == oNodeType)
                return true;

            return false;
        }
        inline bool ReadNextSiblingNode(int nDepth)
        {
            if (!IsValid())
                return false;

            XmlNodeType eNodeType = XmlNodeType_None;
            int nCurDepth = -1;

            while (1 == xmlTextReaderRead(reader))
            {
                int nTempType = xmlTextReaderNodeType(reader);
                int nTempDepth = xmlTextReaderDepth(reader);
                if (-1 == nTempType || -1 == nTempDepth)
                    return false;
                eNodeType = (XmlNodeType)nTempType;
                nCurDepth = nTempDepth;

                if (nCurDepth < nDepth)
                    break;

                if (XmlNodeType_Element == eNodeType && nCurDepth == nDepth + 1)
                    return true;
                else if (XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth)
                    return false;
            }

            return false;
        }
        inline bool ReadNextSiblingNode2(int nDepth)
        {
            if (!IsValid())
                return false;

            XmlNodeType eNodeType = XmlNodeType_None;
            int nCurDepth = -1;

            while (1 == xmlTextReaderRead(reader))
            {
                int nTempType = xmlTextReaderNodeType(reader);
                int nTempDepth = xmlTextReaderDepth(reader);

                if (-1 == nTempType || -1 == nTempDepth)
                    return false;

                eNodeType = (XmlNodeType)nTempType;
                nCurDepth = nTempDepth;

                if (nCurDepth < nDepth)
                    break;

                if ((XmlNodeType_Element == eNodeType  && nCurDepth == nDepth + 1)
                        || ((XmlNodeType_Text == eNodeType ||
                             XmlNodeType_Whitespace == eNodeType ||
                                                         XmlNodeType_SIGNIFICANT_WHITESPACE == eNodeType ) && nCurDepth == nDepth + 1))
                    return true;
                else if (XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth)
                    return false;
            }

            return false;
        }
        inline bool ReadTillEnd(int nDepth = -2)
        {
            if (!IsValid())
                return false;

            if (-2 == nDepth)
                nDepth = GetDepth();
            else if (nDepth == GetDepth() && 0 != xmlTextReaderIsEmptyElement(reader))
                return true;

            XmlNodeType eNodeType = XmlNodeType_None;

            int nCurDepth = -1;
            while (true)
            {
                if (1 != xmlTextReaderRead(reader))
                    break;

                int nTempType = xmlTextReaderNodeType(reader);
                if (-1 == nTempType)
                    return false;
                eNodeType = (XmlNodeType)nTempType;
                nCurDepth = GetDepth();

                if (nCurDepth < nDepth)
                    break;

                if (XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth)
                    break;
            }

            return true;
        }

        std::wstring GetName()
        {
            if (!IsValid())
                return L"";

            xmlChar* pName = xmlTextReaderName(reader);
            if (NULL == pName)
                return L"";

            std::wstring sRet = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pName, (LONG)strlen((const char*)pName));
            free(pName);
            return sRet;
        }
        std::string GetNameA()
        {
            if (!IsValid())
                return "";

            xmlChar* pName = xmlTextReaderName(reader);
            if (NULL == pName)
                return "";

            std::string sRet((char*)pName);
            free(pName);
            return sRet;
        }
        inline int GetDepth()
        {
            if (!IsValid())
                return -1;

            int nTempDepth = xmlTextReaderDepth(reader);
            if (-1 == nTempDepth)
                return -1;
            return nTempDepth;
        }
        inline bool IsEmptyNode()
        {
            if (!IsValid())
                return false;

            return 0 != xmlTextReaderIsEmptyElement(reader) ? true : false;
        }

        inline std::wstring GetText()
        {
            if (!IsValid())
                return NULL;

            xmlChar* pValue = xmlTextReaderValue(reader);
            if (NULL == pValue)
                return L"";

            std::wstring sTemp = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pValue, (LONG)strlen((const char*)pValue));
            free(pValue);
            return sTemp;
        }
        inline std::string GetTextA()
        {
            if (!IsValid())
                return NULL;

            xmlChar* pValue = xmlTextReaderValue(reader);
            if (NULL == pValue)
                return "";

            std::string sTemp((const char*)pValue);
            free(pValue);
            return sTemp;
        }

        std::wstring GetText2()
        {
            if (!IsValid())
                return L"";

            std::wstring sResult;

            if (0 != xmlTextReaderIsEmptyElement(reader))
                return sResult;

            int nDepth = GetDepth();
            XmlNodeType eNodeType = XmlNodeType_EndElement;
            while (Read(eNodeType) && GetDepth() >= nDepth && XmlNodeType_EndElement != eNodeType)
            {
                if (eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace || eNodeType == XmlNodeType_SIGNIFICANT_WHITESPACE)
                    sResult += GetText();
            }

            return sResult;
        }
        std::string GetText2A()
        {
            if (!IsValid())
                return "";

            std::string sResult;

            if (0 != xmlTextReaderIsEmptyElement(reader))
                return sResult;

            int nDepth = GetDepth();
            XmlNodeType eNodeType = XmlNodeType_EndElement;
            while (Read(eNodeType) && GetDepth() >= nDepth && XmlNodeType_EndElement != eNodeType)
            {
                if (eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace || eNodeType == XmlNodeType_SIGNIFICANT_WHITESPACE)
                    sResult += GetTextA();
            }

            return sResult;
        }
        std::wstring GetText3()
        {
            if ( !IsValid() )
                return L"";

            std::wstring sResult;

            if ( 0 != xmlTextReaderIsEmptyElement(reader) )
                return sResult;

            int nDepth = GetDepth();
            XmlNodeType eNodeType = XmlNodeType_EndElement;
            while ( Read( eNodeType ) && GetDepth() >= nDepth && XmlNodeType_EndElement != eNodeType )
            {
                if ( eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace || eNodeType == XmlNodeType_SIGNIFICANT_WHITESPACE )
                    sResult += GetText();
            }

            return sResult;
        }
        inline std::wstring GetOuterXml()
        {
            return GetXml(false);
        }
        inline std::wstring GetInnerXml()
        {
            return GetXml(true);
        }

        inline int GetAttributesCount()
        {
            if (!IsValid())
                return -1;

            return xmlTextReaderAttributeCount(reader);
        }
        inline bool MoveToFirstAttribute()
        {
            if (!IsValid())
                return false;

            return 1 == xmlTextReaderMoveToFirstAttribute(reader) ? true : false;
        }
        inline bool MoveToNextAttribute()
        {
            if (!IsValid())
                return false;

            return 1 == xmlTextReaderMoveToNextAttribute(reader) ? true : false;
        }

        inline bool MoveToElement()
        {
            if (!IsValid())
                return false;

            return 1 == xmlTextReaderMoveToElement(reader) ? true : false;
        }

        inline bool IsEmptyElement()
        {
            if (!IsValid())
                return false;
            return 1 == xmlTextReaderIsEmptyElement(reader) ? true : false;
        }

        std::wstring GetNamespacePrefix()
        {
            xmlChar* pName = xmlTextReaderPrefix(reader);
            if (NULL == pName)
                return L"";

            std::wstring sTemp = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pName, (LONG)strlen((const char*)pName));
            free(pName);
            return sTemp;
        }
        XmlNodeType GetNodeType()
        {
            return (XmlUtils::XmlNodeType)xmlTextReaderNodeType(reader);
        }
        bool IsDefaultAttribute()
        {
            return (0 != xmlTextReaderIsDefault(reader)) ? true : false;
        }

    private:
        inline std::wstring GetXml(bool bInner)
        {
            if (!IsValid())
                return L"";

            NSStringUtils::CStringBuilder oResult;
            if (false == bInner)
                WriteElement(oResult);

            int nDepth = GetDepth();
            if (0 == xmlTextReaderIsEmptyElement(reader))
            {
                XmlNodeType eNodeType = XmlNodeType_None;

                int nCurDepth = -1;
                while (true)
                {
                    if (1 != xmlTextReaderRead(reader))
                        break;

                    int nTempType = xmlTextReaderNodeType(reader);
                    if (-1 == nTempType)
                        break;
                    eNodeType = (XmlNodeType)nTempType;

                    nCurDepth = GetDepth();
                    if (eNodeType == XmlNodeType_Text || eNodeType == XmlNodeType_Whitespace || eNodeType == XmlNodeType_SIGNIFICANT_WHITESPACE)
                        oResult.WriteEncodeXmlString(GetText().c_str());
                    else if (eNodeType == XmlNodeType_Element)
                        WriteElement(oResult);
                    else if (eNodeType == XmlNodeType_EndElement)
                    {
                        if (false == bInner || nCurDepth != nDepth)
                        {
                            oResult.AddChar2Safe(wchar_t('<'), wchar_t('/'));
                            oResult.WriteEncodeXmlString(GetName().c_str());
                            oResult.AddCharSafe(wchar_t('>'));
                        }
                    }

                    nCurDepth = GetDepth();
                    if (nCurDepth < nDepth)
                        break;

                    if (XmlNodeType_EndElement == eNodeType && nCurDepth == nDepth)
                        break;
                }
            }

            return oResult.GetData();
        }
        void WriteElement(NSStringUtils::CStringBuilder& oResult)
        {
            oResult.AddCharSafe((wchar_t)'<');
            oResult.WriteEncodeXmlString(GetName().c_str());
            if (GetAttributesCount() > 0)
            {
                MoveToFirstAttribute();
                std::wstring sName = GetName();
                while (!sName.empty())
                {
                    oResult.AddCharSafe(wchar_t(' '));
                    oResult.WriteEncodeXmlString(GetName().c_str());
                    oResult.AddChar2Safe(wchar_t('='), wchar_t('\"'));
                    oResult.WriteEncodeXmlString(GetText().c_str());
                    oResult.AddCharSafe(wchar_t('\"'));

                    if (!MoveToNextAttribute())
                        break;
                    sName = GetName();
                }
                MoveToElement();
            }
            if (IsEmptyNode())
                oResult.AddChar2Safe(wchar_t('/'), wchar_t('>'));
            else
                oResult.AddCharSafe(wchar_t('>'));
        }
    };
}

#endif // _BUILD_XMLLIGHT_PRIVATE_CROSSPLATFORM_H_
