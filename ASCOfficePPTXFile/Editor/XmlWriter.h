/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#pragma once

#include "./BinReaderWriterDefines.h"
#include "./Drawing/Shapes/BaseShape/Common.h"

#include "../../DesktopEditor/graphics/IRenderer.h"

#include "../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

#if !defined(_WIN32) && !defined(_WIN64)
    #include "../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

namespace NSBinPptxRW
{
    static std::wstring	g_bstr_nodeopen			= L"<";
    static std::wstring	g_bstr_nodeclose		= L">";
    static std::wstring	g_bstr_nodeopen_slash		= L"</";
    static std::wstring	g_bstr_nodeclose_slash		= L"/>";
    static std::wstring	g_bstr_node_space		= L" ";
    static std::wstring	g_bstr_node_equal		= L"=";
    static std::wstring	g_bstr_node_quote		= L"\"";
    static std::wstring	g_bstr_boolean_true		= L"true";
    static std::wstring	g_bstr_boolean_false		= L"false";
    static std::wstring	g_bstr_boolean_true2		= L"1";
    static std::wstring	g_bstr_boolean_false2		= L"0";
	
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
                int size_alloc = m_lSize * sizeof(wchar_t);
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
        AVSINLINE void WriteString(std::wstring& wString)
        {
            size_t nLen = wString.length();
            WriteString(wString.c_str(), nLen);
        }
#ifdef _WIN32
        AVSINLINE void WriteString(_bstr_t& bsString)
        {
            size_t nLen = bsString.length();
            WriteString(bsString.GetBSTR(), nLen);
        }
#endif // #ifdef _WIN32
        AVSINLINE void WriteString(const CString& sString)
        {
            size_t nLen = (size_t)sString.GetLength();

#ifdef _UNICODE
            CString* pString = const_cast<CString*>(&sString);
            WriteString(pString->GetBuffer(), nLen);
            pString->ReleaseBuffer();
#else
            CStringW str = (CStringW)sString;
            WriteString(str.GetBuffer(), nLen);
            str.ReleaseBuffer();
#endif
        }
        AVSINLINE void WriteStringXML(const CString& strValue)
        {
            // можно ускорить (см. как сделано в шейпах)
            CString s = strValue;
            s.Replace(_T("&"),	_T("&amp;"));
            s.Replace(_T("'"),	_T("&apos;"));
            s.Replace(_T("<"),	_T("&lt;"));
            s.Replace(_T(">"),	_T("&gt;"));
            s.Replace(_T("\""),	_T("&quot;"));
            WriteString(s);
        }
        AVSINLINE void WriteStringXML(const std::wstring & wString)
        {
            std::wstring buffer;
            buffer.reserve(wString.size());
            for(size_t pos = 0; pos != wString.size(); ++pos)
            {
                switch(wString[pos])
                {
                case '&':  buffer.append(_T("&amp;"));          break;
                case '\"': buffer.append(_T("&quot;"));         break;
                case '\'': buffer.append(_T("&apos;"));         break;
                case '<':  buffer.append(_T("&lt;"));           break;
                case '>':  buffer.append(_T("&gt;"));           break;
                default:   buffer.append(&wString[pos], 1);	break;
                }
            }
            WriteString(buffer);
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

        CString GetData()
        {
            CString str(m_pData, (int)m_lSizeCur);
            return str;
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
        LONG m_lObjectId;
        LONG m_lObjectIdVML;
        LONG m_lObjectIdOle;

        CString m_strStyleMain;
        CString m_strAttributesMain;
        CString m_strNodes;
        CString m_strOleXlsx;

        IRenderer* m_pOOXToVMLRenderer;

        bool m_bIsTop;
        bool m_bIsUseOffice2007;

        CXmlWriter() : m_oWriter()
        {
            m_lDocType = XMLWRITER_DOC_TYPE_PPTX;

            m_lFlag                     = 0;
            m_lGroupIndex               = 0;
            m_lObjectId                 = 0;
            m_lObjectIdVML		= 0;
            m_lObjectIdOle              = 0;

            m_bIsUseOffice2007          = false;
            m_strStyleMain		= _T("");
            m_strAttributesMain         = _T("");
            m_strNodes			= _T("");
            m_strOleXlsx		= _T("");

            m_pOOXToVMLRenderer         = NULL;
            m_bIsTop                    = false;
         }
        ~CXmlWriter()
        {
        }

        AVSINLINE CString GetXmlString()
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
        AVSINLINE void WriteString(const CString& strValue)
        {
            m_oWriter.WriteString(strValue);
        }
        AVSINLINE void WriteStringXML(CString strValue)
        {
            // можно ускорить (см. как сделано в шейпах)
            CString s = strValue;
            s.Replace(_T("&"),	_T("&amp;"));
            s.Replace(_T("'"),	_T("&apos;"));
            s.Replace(_T("<"),	_T("&lt;"));
            s.Replace(_T(">"),	_T("&gt;"));
            s.Replace(_T("\""),	_T("&quot;"));
            m_oWriter.WriteString(s);
        }
        AVSINLINE void WriteDouble(const double& val)
        {
            CString str = _T("");
            str.Format(_T("%lf"), val);
            m_oWriter.WriteString(str);
        }
        AVSINLINE void WriteLONG(const long& val)
        {
            CString str = _T("");
            str.Format(_T("%d"), val);
            m_oWriter.WriteString(str);
        }
        AVSINLINE void WriteINT(const int& val)
        {
            CString str = _T("");
            str.Format(_T("%d"), val);
            m_oWriter.WriteString(str);
        }
        AVSINLINE void WriteDWORD(const DWORD& val)
        {
            CString str = _T("");
            str.Format(_T("%u"), val);
            m_oWriter.WriteString(str);
        }
        AVSINLINE void WriteDWORD_hex(const DWORD& val)
        {
            CString str = _T("");
            str.Format(_T("%x"), val);
            m_oWriter.WriteString(str);
        }
        AVSINLINE void WriteBool(const bool& val)
        {
            if (val)
                m_oWriter.WriteString(g_bstr_boolean_true2);
            else
                m_oWriter.WriteString(g_bstr_boolean_false2);
        }
        // write attribute
        AVSINLINE void WriteAttributeCSS(const CString& strAttributeName, const CString& val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(15);
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.WriteString(val);
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
        AVSINLINE void WriteAttributeCSS_int(const CString& strAttributeName, const int& val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(15);
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.AddIntNoCheck(val);
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
        AVSINLINE void WriteAttributeCSS_double1(const CString& strAttributeName, const double& val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(15);
            CString s = _T("");
            s.Format(_T("%.1lf"), val);
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.WriteString(s);
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
        AVSINLINE void WriteAttributeCSS_int_pt(const CString& strAttributeName, const int& val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(15);
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.AddIntNoCheck(val);
            m_oWriter.AddCharNoCheck(WCHAR('p'));
            m_oWriter.AddCharNoCheck(WCHAR('t'));
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
        AVSINLINE void WriteAttributeCSS_double1_pt(const CString& strAttributeName, const double& val)
        {
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.AddSize(20);
            CString s = _T("");
            s.Format(_T("%.1lf"), val);
            m_oWriter.AddCharNoCheck(WCHAR(':'));
            m_oWriter.WriteString(s);
            m_oWriter.AddCharNoCheck(WCHAR('p'));
            m_oWriter.AddCharNoCheck(WCHAR('t'));
            m_oWriter.AddCharNoCheck(WCHAR(';'));
        }
        //
        AVSINLINE void WriteAttribute(const CString& strAttributeName, const CString& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            m_oWriter.WriteString(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute2(const CString& strAttributeName, const CString& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            m_oWriter.WriteStringXML(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const CString& strAttributeName, const double& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteDouble(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const CString& strAttributeName, const int& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteINT(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const CString& strAttributeName, const bool& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteBool(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const CString& strAttributeName, const LONG& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteLONG(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttribute(const CString& strAttributeName, const DWORD& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteDWORD(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        AVSINLINE void WriteAttributeDWORD_hex(const CString& strAttributeName, const DWORD& val)
        {
            m_oWriter.WriteString(g_bstr_node_space);
            m_oWriter.WriteString(strAttributeName);
            m_oWriter.WriteString(g_bstr_node_equal);
            m_oWriter.WriteString(g_bstr_node_quote);
            WriteDWORD_hex(val);
            m_oWriter.WriteString(g_bstr_node_quote);
        }
        // document methods
        AVSINLINE void WriteNodeBegin(CString strNodeName, bool bAttributed = false)
        {
            m_oWriter.WriteString(g_bstr_nodeopen);
            m_oWriter.WriteString(strNodeName);

            if (!bAttributed)
                m_oWriter.WriteString(g_bstr_nodeclose);
        }
        AVSINLINE void WriteNodeEnd(CString strNodeName, bool bEmptyNode = false, bool bEndNode = true)
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
        AVSINLINE void WriteNodeValue(const CString& strNodeName, const CString& val)
        {
            WriteNodeBegin(strNodeName);
            WriteString(val);
            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValue(const CString& strNodeName, const bool& val)
        {
            WriteNodeBegin(strNodeName);

            if (val)
                WriteString(_T("1"));
            else
                WriteString(_T("0"));

            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValue(const CString& strNodeName, const double& val)
        {
            WriteNodeBegin(strNodeName);
            WriteDouble(val);
            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValue(const CString& strNodeName, const LONG& val)
        {
            WriteNodeBegin(strNodeName);
            WriteLONG(val);
            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValue(const CString& strNodeName, const int& val)
        {
            WriteNodeBegin(strNodeName);
            WriteINT(val);
            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValue(const CString& strNodeName, const DWORD& val)
        {
            WriteNodeBegin(strNodeName);
            WriteDWORD(val);
            WriteNodeEnd(strNodeName);
        }
        AVSINLINE void WriteNodeValueDWORD_hex(const CString& strNodeName, const DWORD& val)
        {
            WriteNodeBegin(strNodeName);
            WriteDWORD_hex(val);
            WriteNodeEnd(strNodeName);
        }

        bool SaveToFile(CString strFilePath, bool bEncodingToUTF8 = true, bool bIsClearNoAttack = true)
        {
            CString strData = m_oWriter.GetData();
            if (!bEncodingToUTF8)
            {
                CFile oFile;
                oFile.CreateFile(strFilePath);
                oFile.WriteFile((void*)strData.GetBuffer(), strData.GetLength());
                oFile.CloseFile();
            }
            else
            {
                CDirectory::SaveToFile(strFilePath, strData);

                CFile oFile;
                oFile.CreateFile(strFilePath);
                CString strHead = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
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
        AVSINLINE void WriteAttribute(const CString& strName, const nullable_int& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, *value);
        }
        AVSINLINE void WriteAttribute(const CString& strName, const nullable_double& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, *value);
        }
        AVSINLINE void WriteAttribute(const CString& strName, const nullable_string& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, *value);
        }
        AVSINLINE void WriteAttribute2(const CString& strName, const nullable_string& value)
        {
            if (value.IsInit())
                WriteAttribute2(strName, *value);
        }
        AVSINLINE void WriteAttribute(const CString& strName, const nullable_bool& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, *value);
        }
        template <typename T>
        AVSINLINE void WriteAttribute(const CString& strName, const nullable_limit<T>& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, (*value).get());
        }
        // -------------------------------------------------------------------------------------
        // NODES -------------------------------------------------------------------------------
        AVSINLINE void WriteNodeValue(const CString& strName, const nullable_int& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, *value);
        }
        AVSINLINE void WriteNodeValue(const CString& strName, const nullable_double& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, *value);
        }
        AVSINLINE void WriteNodeValue(const CString& strName, const nullable_string& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, *value);
        }
        AVSINLINE void WriteNodeValue(const CString& strName, const nullable_bool& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, *value);
        }
        template <typename T>
        AVSINLINE void WriteNodeValue(const CString& strName, const nullable_limit<T>& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, (*value).get);
        }
        // -------------------------------------------------------------------------------------
        // DOCUMENT ----------------------------------------------------------------------------
        AVSINLINE void StartNode(const CString& name)
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
        AVSINLINE void EndNode(const CString& name)
        {
            m_oWriter.WriteString(g_bstr_nodeopen_slash);
            m_oWriter.WriteString(name);
            m_oWriter.WriteString(g_bstr_nodeclose);
        }

#ifdef _WIN32
        template<typename T>
        AVSINLINE void WriteArray(const CString& strName, const CAtlArray<T>& arr)
        {
            size_t nCount = arr.GetCount();
            if (0 != nCount)
            {
                StartNode(strName);
                m_oWriter.WriteString(g_bstr_nodeclose);
                for (size_t i = 0; i < nCount; ++i)
                    arr[i].toXmlWriter(this);
                EndNode(strName);
            }
        }
#endif //#ifdef _WIN32
        template<typename T>
        AVSINLINE void WriteArray(const CString& strName, const std::vector<T>& arr)
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

#ifdef _WIN32
        template<typename T>
        AVSINLINE void WriteArray2(const CAtlArray<T>& arr)
        {
            size_t nCount = arr.GetCount();
            if (0 != nCount)
            {
                for (size_t i = 0; i < nCount; ++i)
                    arr[i].toXmlWriter(this);
            }
        }
#endif //#ifdef _WIN32
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

        void ReplaceString(CString str1, CString str2)
        {
            // ужасная функция. вызывать ее не надо. не для этого класс писался.
            CString sCur = m_oWriter.GetData();
            sCur.Replace(str1, str2);
            ClearNoAttack();
            WriteString(sCur);
        }
    };
}
