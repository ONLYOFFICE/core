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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#pragma once

#include "./BinReaderWriterDefines.h"
#include "./Drawing/Shapes/BaseShape/Common.h"

#include "../../DesktopEditor/graphics/IRenderer.h"

#include "../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

namespace NSBinPptxRW
{
    static std::wstring	g_bstr_nodeopen		= L"<";
    static std::wstring	g_bstr_nodeclose	= L">";
    static std::wstring	g_bstr_nodeopen_slash	= L"</";
    static std::wstring	g_bstr_nodeclose_slash	= L"/>";
    static std::wstring	g_bstr_node_space	= L" ";
    static std::wstring	g_bstr_node_equal	= L"=";
    static std::wstring	g_bstr_node_quote	= L"\"";
    static std::wstring	g_bstr_boolean_true	= L"true";
    static std::wstring	g_bstr_boolean_false	= L"false";
    static std::wstring	g_bstr_boolean_true2	= L"1";
    static std::wstring	g_bstr_boolean_false2	= L"0";
	
    AVSINLINE static double FABS(double dVal)
    {
        return (dVal >= 0) ? dVal : -dVal;
    }
    AVSINLINE static int round(double dVal)
    {
        return (int)(dVal + 0.5);
    }

    class CStringWriter
    {
    private:
        wchar_t*	m_pData;
        size_t		m_lSize;

        wchar_t*	m_pDataCur;
        size_t		m_lSizeCur;

    public:
        CStringWriter()
        {
            m_pData = NULL;
            m_lSize = 0;

            m_pDataCur	= m_pData;
            m_lSizeCur	= m_lSize;
        }
        ~CStringWriter()
        {
            RELEASEMEM(m_pData);
        }

        AVSINLINE void AddSize(size_t nSize)
        {
            if (NULL == m_pData)
            {
                m_lSize = (std::max)(nSize, (size_t) 1024);
                m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t)+64);

                m_lSizeCur = 0;
                m_pDataCur = m_pData;
                return;
            }

            if ((m_lSizeCur + nSize) > m_lSize)
            {
                while ((m_lSizeCur + nSize) > m_lSize)
                {
                    //m_lSize *= 2; - бесконтрольно ..
                    m_lSize += (std::max)(nSize, (size_t) 1024);
                }
                size_t size_alloc = m_lSize * sizeof(wchar_t);
#if defined(_WIN32) || defined (_WIN64)
                wchar_t* pRealloc = (wchar_t*)realloc(m_pData, size_alloc );
                if (NULL != pRealloc)
                {
                    // реаллок сработал
                    m_pData		= pRealloc;
                    m_pDataCur	= m_pData + m_lSizeCur;
                }
                else
#endif
                {
                    wchar_t* pMalloc = (wchar_t*)malloc(size_alloc );
                    memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

                    free(m_pData);
                    m_pData		= pMalloc;
                    m_pDataCur	= m_pData + m_lSizeCur;
                }
            }
        }

    public:

        AVSINLINE void WriteString(const wchar_t* pString, size_t& nLen)
        {
            AddSize(nLen);

            memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));

            m_pDataCur += nLen;
            m_lSizeCur += nLen;
        }
        AVSINLINE void WriteString(const std::wstring& wString)
        {
            size_t nLen = wString.length();
            WriteString(wString.c_str(), nLen);
        }
        AVSINLINE void WriteStringXML(const std::wstring& _strValue)
        {
            // можно ускорить (см. как сделано в шейпах)
			std::wstring strValue = _strValue;

			XmlUtils::replace_all(strValue, L"&",	L"&amp;");
            XmlUtils::replace_all(strValue, L"'",	L"&apos;");
            XmlUtils::replace_all(strValue, L"<",	L"&lt;");
            XmlUtils::replace_all(strValue, L">",	L"&gt;");
            XmlUtils::replace_all(strValue, L"\"",	L"&quot;");
            
			WriteString(strValue);
        }
        AVSINLINE size_t GetCurSize()
        {
            return m_lSizeCur;
        }

        AVSINLINE void Write(CStringWriter& oWriter)
        {
            WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
        }

        AVSINLINE void WriteBefore(CStringWriter& oWriter)
        {
            size_t nNewS = oWriter.GetCurSize();
            AddSize(nNewS);
            memmove(m_pData + nNewS, m_pData, m_lSizeCur * sizeof (wchar_t));
            memcpy(m_pData, oWriter.m_pData, nNewS * sizeof (wchar_t));
            m_pDataCur += nNewS;
            m_lSizeCur += nNewS;
        }

        inline void Clear()
        {
            RELEASEMEM(m_pData);

            m_pData = NULL;
            m_lSize = 0;

            m_pDataCur	= m_pData;
            m_lSizeCur	= 0;
        }
        inline void ClearNoAttack()
        {
            m_pDataCur	= m_pData;
            m_lSizeCur	= 0;
        }

        std::wstring GetData()
        {
			return std::wstring(m_pData, (int)m_lSizeCur);
        }

        AVSINLINE void AddCharNoCheck(const WCHAR& wc)
        {
            *m_pDataCur++ = wc;
            ++m_lSizeCur;
        }
        AVSINLINE void AddIntNoCheck(int val)
        {
            if (0 == val)
            {
                *m_pDataCur++ = (WCHAR)'0';
                ++m_lSizeCur;
                return;
            }
            if (val < 0)
            {
                val = -val;
                *m_pDataCur++ = (WCHAR)'-';
                ++m_lSizeCur;
            }

            int len = 0;
            int oval = val;
            while (oval > 0)
            {
                oval /= 10;
                ++len;
            }

            oval = 1;
            while (val > 0)
            {
                m_pDataCur[len - oval] = (WCHAR)('0' + (val % 10));
                ++oval;
                val /= 10;
            }

            m_pDataCur += len;
            m_lSizeCur += len;
        }

        AVSINLINE void AddStringNoCheck(const wchar_t* pData, const int& len)
        {
            memcpy(m_pDataCur, pData, len *sizeof(wchar_t));

            m_pDataCur += len;
            m_lSizeCur += len;
        }
        AVSINLINE void AddSpaceNoCheck()
        {
            *m_pDataCur = WCHAR(' ');
            ++m_pDataCur;
            ++m_lSizeCur;
        }
    };

    class CXmlWriter
    {
    public:
        CStringWriter m_oWriter;

    public:
        BYTE m_lDocType;

        LONG m_lFlag;
        LONG m_lGroupIndex;

        unsigned int m_lObjectId;
        unsigned int m_lObjectIdVML;
        unsigned int m_lObjectIdOle;

        std::wstring m_strStyleMain;
        std::wstring m_strAttributesMain;
        std::wstring m_strNodes;

        IRenderer* m_pOOXToVMLRenderer;

        bool m_bIsTop;
        bool m_bIsUseOffice2007;

		CXmlWriter (BYTE ooxType =  XMLWRITER_DOC_TYPE_PPTX) : m_oWriter()
		{
			m_lDocType					= ooxType;
            m_lFlag                     = 0;
            m_lGroupIndex               = 0;
            m_lObjectId                 = 0;
            m_lObjectIdVML              = 0;
            m_lObjectIdOle              = 0;

            m_bIsUseOffice2007          = false;

			m_pOOXToVMLRenderer         = NULL;
            m_bIsTop                    = false;
		}
        ~CXmlWriter()
        {
        }

        AVSINLINE std::wstring GetXmlString()
        {
            return m_oWriter.GetData();
        }
        AVSINLINE void ClearNoAttack()
        {
            m_oWriter.ClearNoAttack();
        }
        AVSINLINE int GetSize()
        {
            return (int)m_oWriter.GetCurSize();
        }

        // write value
        AVSINLINE void WriteString(const std::wstring& strValue)
        {
            m_oWriter.WriteString(strValue);
        }
        AVSINLINE void WriteStringXML(std::wstring strValue)
        {
            std::wstring s = strValue;
            XmlUtils::replace_all( s, L"&",	L"&amp;");
            XmlUtils::replace_all( s, L"'",	L"&apos;");
            XmlUtils::replace_all( s, L"<",	L"&lt;");
            XmlUtils::replace_all( s, L">",	L"&gt;");
            XmlUtils::replace_all( s, L"\"",	L"&quot;");
            m_oWriter.WriteString(s);
        }
        AVSINLINE void WriteDouble(const double& val)
        {
            m_oWriter.WriteString(std::to_wstring(val));
        }
        AVSINLINE void WriteLONG(const long& val)
        {
            m_oWriter.WriteString(std::to_wstring(val));
        }
        AVSINLINE void WriteINT(const int& val)
        {
            m_oWriter.WriteString(std::to_wstring(val));
        }
        AVSINLINE void WriteDWORD(const DWORD& val)
        {
            m_oWriter.WriteString(std::to_wstring(val));
        }
        AVSINLINE void WriteSIZET(const size_t& val)
        {
            m_oWriter.WriteString(std::to_wstring((unsigned int)val));
        }
		AVSINLINE void WriteDWORD_hex(const DWORD& val)
        {
            m_oWriter.WriteString(XmlUtils::IntToString(val, L"%x"));
        }
        AVSINLINE void WriteBool(const bool& val)
        {
            if (val)
                m_oWriter.WriteString(g_bstr_boolean_true2);
            else
                m_oWriter.WriteString(g_bstr_boolean_false2);
        }
        // write attribute
        AVSINLINE void WriteAttributeCSS(const std::wstring& strAttributeName, const std::wstring& val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(15);
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.WriteString(val);
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
        AVSINLINE void WriteAttributeCSS(const std::wstring& strAttributeName, const wchar_t* val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(15);
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.WriteString(val);
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
		AVSINLINE void WriteAttributeCSS_int(const std::wstring& strAttributeName, const int& val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(15);
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.AddIntNoCheck(val);
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
        AVSINLINE void WriteAttributeCSS_double1(const std::wstring& strAttributeName, const double& val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(15);
            std::wstring s = XmlUtils::DoubleToString(val, L"%.1lf");
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.WriteString(s);
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
        AVSINLINE void WriteAttributeCSS_int_pt(const std::wstring& strAttributeName, const int& val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(15);
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.AddIntNoCheck(val);
            m_oWriter.AddCharNoCheck(WCHAR('p'));
            m_oWriter.AddCharNoCheck(WCHAR('t'));
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
        AVSINLINE void WriteAttributeCSS_double1_pt(const std::wstring& strAttributeName, const double& val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(20);
            std::wstring s = XmlUtils::DoubleToString(val, L"%.1lf");
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.WriteString(s);
            m_oWriter.AddCharNoCheck(WCHAR('p'));
            m_oWriter.AddCharNoCheck(WCHAR('t'));
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
        //
        AVSINLINE void WriteAttribute(const std::wstring& strAttributeName, const std::wstring& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            m_oWriter.WriteString(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const std::wstring& strAttributeName, const wchar_t* val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            m_oWriter.WriteString(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
		AVSINLINE void WriteAttribute2(const std::wstring& strAttributeName, const std::wstring& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            m_oWriter.WriteStringXML(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const std::wstring& strAttributeName, const double& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteDouble(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const std::wstring& strAttributeName, const int& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteINT(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const std::wstring& strAttributeName, const bool& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteBool(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const std::wstring& strAttributeName, const LONG& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteLONG(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const std::wstring& strAttributeName, const DWORD& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteDWORD(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
#if defined (_WIN32) || defined(_WIN64)
		AVSINLINE void WriteAttribute(const std::wstring& strAttributeName, const size_t& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteSIZET(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
#endif
		AVSINLINE void WriteAttributeDWORD_hex(const std::wstring& strAttributeName, const DWORD& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteDWORD_hex(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        // document methods
        AVSINLINE void WriteNodeBegin(std::wstring strNodeName, bool bAttributed = false)
        {
            m_oWriter.WriteString(g_bstr_nodeopen);
            m_oWriter.WriteString(strNodeName);

            if (!bAttributed)
                m_oWriter.WriteString(g_bstr_nodeclose);
        }
        AVSINLINE void WriteNodeEnd(std::wstring strNodeName, bool bEmptyNode = false, bool bEndNode = true)
        {
            if (bEmptyNode)
            {
                if (bEndNode)
                    m_oWriter.WriteString(g_bstr_nodeclose_slash);
                else
                    m_oWriter.WriteString(g_bstr_nodeclose);
            }
            else
            {
                m_oWriter.WriteString(g_bstr_nodeopen_slash);
                m_oWriter.WriteString(strNodeName);
                m_oWriter.WriteString(g_bstr_nodeclose);
            }
        }
        // write node values
        AVSINLINE void WriteNodeValue(const std::wstring& strNodeName, const std::wstring& val)
        {
            WriteNodeBegin(strNodeName);
            WriteString(val);
            WriteNodeEnd(strNodeName);
        }
		AVSINLINE void WriteNodeValue2(const std::wstring& strNodeName, const std::wstring& val)
		{
			WriteNodeBegin(strNodeName);
			WriteStringXML(val);
			WriteNodeEnd(strNodeName);
		}
        AVSINLINE void WriteNodeValue(const std::wstring& strNodeName, const bool& val)
        {
            WriteNodeBegin(strNodeName);

            if (val)
                WriteString(_T("1"));
            else
                WriteString(_T("0"));

            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValue(const std::wstring& strNodeName, const double& val)
        {
            WriteNodeBegin(strNodeName);
            WriteDouble(val);
            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValue(const std::wstring& strNodeName, const LONG& val)
        {
            WriteNodeBegin(strNodeName);
            WriteLONG(val);
            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValue(const std::wstring& strNodeName, const int& val)
        {
            WriteNodeBegin(strNodeName);
            WriteINT(val);
            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValue(const std::wstring& strNodeName, const DWORD& val)
        {
            WriteNodeBegin(strNodeName);
            WriteDWORD(val);
            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValueDWORD_hex(const std::wstring& strNodeName, const DWORD& val)
        {
            WriteNodeBegin(strNodeName);
            WriteDWORD_hex(val);
            WriteNodeEnd(strNodeName);
        }

        bool SaveToFile(std::wstring strFilePath, bool bEncodingToUTF8 = true, bool bIsClearNoAttack = true)
        {
            std::wstring strData = m_oWriter.GetData();
            if (!bEncodingToUTF8)
            {
                CFile oFile;
                oFile.CreateFile(strFilePath);
                oFile.WriteFile((void*)strData.c_str(), (DWORD)strData.length());
                oFile.CloseFile();
            }
            else
            {
                CDirectory::SaveToFile(strFilePath, strData);

                CFile oFile;
                oFile.CreateFile(strFilePath);
                std::wstring strHead = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
                oFile.WriteStringUTF8(strHead);
                oFile.WriteStringUTF8(strData);
                oFile.CloseFile();
            }

            if (bIsClearNoAttack)
            {
                m_oWriter.ClearNoAttack();
            }

            return true;
        }

    public:
        // ATTRIBUTES --------------------------------------------------------------------------
        AVSINLINE void WriteAttribute(const std::wstring& strName, const nullable_int& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, *value);
        }
		AVSINLINE void WriteAttribute(const std::wstring& strName, const nullable_sizet& value)
        {
            if (value.IsInit())
                #ifdef __ANDROID__
                    WriteAttribute(strName, (int)(*value));
                #else
                    WriteAttribute(strName, *value);
                #endif

        }
        AVSINLINE void WriteAttribute(const std::wstring& strName, const nullable_double& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, *value);
        }
        AVSINLINE void WriteAttribute(const std::wstring& strName, const nullable_string& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, *value);
        }
        AVSINLINE void WriteAttribute2(const std::wstring& strName, const nullable_string& value)
        {
            if (value.IsInit())
                WriteAttribute2(strName, *value);
        }
        AVSINLINE void WriteAttribute(const std::wstring& strName, const nullable_bool& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, *value);
        }
        template <typename T>
        AVSINLINE void WriteAttribute(const std::wstring& strName, const nullable_limit<T>& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, (*value).get());
        }
        // -------------------------------------------------------------------------------------
        // NODES -------------------------------------------------------------------------------
        AVSINLINE void WriteNodeValue(const std::wstring& strName, const nullable_int& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, *value);
        }
        AVSINLINE void WriteNodeValue(const std::wstring& strName, const nullable_double& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, *value);
        }
        AVSINLINE void WriteNodeValue(const std::wstring& strName, const nullable_string& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, *value);
        }
		AVSINLINE void WriteNodeValue2(const std::wstring& strName, const nullable_string& value)
		{
			if (value.IsInit())
				WriteNodeValue2(strName, *value);
		}
        AVSINLINE void WriteNodeValue(const std::wstring& strName, const nullable_bool& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, *value);
        }
        template <typename T>
        AVSINLINE void WriteNodeValue(const std::wstring& strName, const nullable_limit<T>& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, (*value).get);
        }
        // -------------------------------------------------------------------------------------
        // DOCUMENT ----------------------------------------------------------------------------
        AVSINLINE void StartNode(const std::wstring& name)
        {
            m_oWriter.WriteString(g_bstr_nodeopen);
            m_oWriter.WriteString(name);
        }
        AVSINLINE void StartAttributes()
        {
            // none
        }
        AVSINLINE void EndAttributes()
        {
            m_oWriter.WriteString(g_bstr_nodeclose);
        }
        AVSINLINE void EndNode(const std::wstring& name)
        {
            m_oWriter.WriteString(g_bstr_nodeopen_slash);
            m_oWriter.WriteString(name);
            m_oWriter.WriteString(g_bstr_nodeclose);
        }

        template<typename T>
        AVSINLINE void WriteArray(const std::wstring& strName, const std::vector<T>& arr)
        {
            size_t nCount = arr.size();
            if (0 != nCount)
            {
                StartNode(strName);
                m_oWriter.WriteString(g_bstr_nodeclose);
                for (size_t i = 0; i < nCount; ++i)
                    arr[i].toXmlWriter(this);
                EndNode(strName);
            }
        }
        template<typename T>
        AVSINLINE void WriteArray2(const std::vector<T>& arr)
        {
            size_t nCount = arr.size();
            if (0 != nCount)
            {
                for (size_t i = 0; i < nCount; ++i)
                    arr[i].toXmlWriter(this);
            }
        }

        template<typename T>
        AVSINLINE void Write(const nullable<T>& val)
        {
            if (val.is_init())
                val->toXmlWriter(this);
        }
        // -------------------------------------------------------------------------------------

        void ReplaceString(std::wstring str1, std::wstring str2)
        {
            // ужасная функция. вызывать ее не надо. не для этого класс писался. .. получше .. но все равно не надо !!!

			std::wstring sCur = m_oWriter.GetData();
			XmlUtils::replace_all(sCur, str1, str2);

			ClearNoAttack();
            WriteString(sCur);
        }
    };
}
