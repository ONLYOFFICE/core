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
#include "../../../MsBinaryFile/Common/Vml/Common.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"

#include "../../Base/Nullable.h"
#include "../../SystemUtility/File.h"
#include "../../../DesktopEditor/common/StringBuilder.h"

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
	
	static double FABS(double dVal)
    {
        return (dVal >= 0) ? dVal : -dVal;
    }
	static int round(double dVal)
    {
        return (int)(dVal + 0.5);
    }

    class CXmlWriter
    {
    public:
		NSStringUtils::CStringBuilder m_oWriter;
//------------------------------
        BYTE m_lDocType;

        LONG m_lFlag;
        LONG m_lGroupIndex;

        unsigned int m_lObjectId;
        unsigned int m_lObjectIdVML;
        unsigned int m_lObjectIdOle;
//------------------------------------------------- for vml
        std::wstring m_strStyleMain;
		std::wstring m_strStyleWrap;
		std::wstring m_strAttributesMain;
        std::wstring m_strNodes;
		nullable_int64 m_zIndex;
		std::wstring m_strId;

		double m_dX = 0;
		double m_dY = 0;
		double m_dWidth = 0;
		double m_dHeight = 0;

        IRenderer* m_pOOXToVMLRenderer;
//--------------------------------------------------
        bool m_bIsTop;
        bool m_bIsUseOffice2007;

		CXmlWriter (BYTE ooxType =  XMLWRITER_DOC_TYPE_PPTX);
		~CXmlWriter();

		std::wstring GetXmlString();
		void ClearNoAttack();
		int GetSize();

        // write value
		void WriteString(const std::wstring& strValue);
		void WriteStringXML(std::wstring strValue);
		void WriteDouble(const double& val);
		void WriteLONG(const long& val);
		void WriteINT(const int& val);
		void WriteUINT(const unsigned int& val);
		void WriteDWORD(const DWORD& val);
		void WriteSIZET(const size_t& val);
		void WriteDWORD_hex(const DWORD& val);
		void WriteBool(const bool& val);

        // write attribute
		void WriteAttributeCSS(const std::wstring& strAttributeName, const std::wstring& val);
		void WriteAttributeCSS(const std::wstring& strAttributeName, const wchar_t* val);
		void WriteAttributeCSS_int(const std::wstring& strAttributeName, const int& val);
		void WriteAttributeCSS_double1(const std::wstring& strAttributeName, const double& val);
		void WriteAttributeCSS_int_pt(const std::wstring& strAttributeName, const int& val);
		void WriteAttributeCSS_double1_pt(const std::wstring& strAttributeName, const double& val);

        //
		void WriteAttribute(const std::wstring& strAttributeName, const std::wstring& val);
		void WriteAttribute(const std::wstring& strAttributeName, const wchar_t* val);
		void WriteAttribute2(const std::wstring& strAttributeName, const std::wstring& val);
		void WriteAttribute(const std::wstring& strAttributeName, const double& val);
		void WriteAttribute(const std::wstring& strAttributeName, const int& val);
		void WriteAttribute(const std::wstring& strAttributeName, const bool& val);
		void WriteAttribute(const std::wstring& strAttributeName, const LONG& val);
		void WriteAttribute(const std::wstring& strAttributeName, const DWORD& val);

#if defined (_WIN32) || defined(_WIN64)
		void WriteAttribute(const std::wstring& strAttributeName, const size_t& val);
#endif
		void WriteAttributeDWORD_hex(const std::wstring& strAttributeName, const DWORD& val);

        // document methods
		void WriteNodeBegin(std::wstring strNodeName, bool bAttributed = false);
		void WriteNodeEnd(std::wstring strNodeName, bool bEmptyNode = false, bool bEndNode = true);

        // write node values
		void WriteNodeValue(const std::wstring& strNodeName, const std::wstring& val);
		void WriteNodeValue2(const std::wstring& strNodeName, const std::wstring& val);
		void WriteNodeValue(const std::wstring& strNodeName, const bool& val);
		void WriteNodeValue(const std::wstring& strNodeName, const double& val);
		void WriteNodeValue(const std::wstring& strNodeName, const LONG& val);
		void WriteNodeValue(const std::wstring& strNodeName, const int& val);
		void WriteNodeValue(const std::wstring& strNodeName, const unsigned int& val);
		void WriteNodeValue(const std::wstring& strNodeName, const DWORD& val);
		void WriteNodeValueDWORD_hex(const std::wstring& strNodeName, const DWORD& val);
		bool SaveToFile(std::wstring strFilePath, bool bEncodingToUTF8 = true, bool bIsClearNoAttack = true);

    public:
        // ATTRIBUTES --------------------------------------------------------------------------
		void WriteAttribute(const std::wstring& strName, const nullable_int& value);
		void WriteAttribute2(const std::wstring& strName, const unsigned int& value);
		void WriteAttribute2(const std::wstring& strName, const nullable_uint& value);
		void WriteAttribute(const std::wstring& strName, const nullable_sizet& value);
		void WriteAttribute(const std::wstring& strName, const nullable_double& value);
		void WriteAttribute(const std::wstring& strName, const nullable_string& value);
		void WriteAttribute2(const std::wstring& strName, const nullable_string& value);
		void WriteAttribute(const std::wstring& strName, const nullable_bool& value);

        template <typename T>
		void WriteAttribute(const std::wstring& strName, const nullable_limit<T>& value)
        {
            if (value.IsInit())
                WriteAttribute(strName, (*value).get());
        }
        // -------------------------------------------------------------------------------------
        // NODES -------------------------------------------------------------------------------
		void WriteNodeValue(const std::wstring& strName, const nullable_int& value);
		void WriteNodeValue(const std::wstring& strName, const nullable_uint& value);
		void WriteNodeValue(const std::wstring& strName, const nullable_double& value);
		void WriteNodeValue(const std::wstring& strName, const nullable_string& value);
		void WriteNodeValue2(const std::wstring& strName, const nullable_string& value);
		void WriteNodeValue(const std::wstring& strName, const nullable_bool& value);

        template <typename T>
		void WriteNodeValue(const std::wstring& strName, const nullable_limit<T>& value)
        {
            if (value.IsInit())
                WriteNodeValue(strName, (*value).get);
        }
        // -------------------------------------------------------------------------------------
        // DOCUMENT ----------------------------------------------------------------------------
		void StartNode(const std::wstring& name);
		void StartAttributes();
		void EndAttributes();
		void EndNode(const std::wstring& name);

        template<typename T>
		void WriteArray(const std::wstring& strName, const std::vector<T>& arr)
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
		void WriteArray2(const std::vector<T>& arr)
        {
            size_t nCount = arr.size();
            if (0 != nCount)
            {
                for (size_t i = 0; i < nCount; ++i)
                    arr[i].toXmlWriter(this);
            }
        }

        template<typename T>
		void Write(const nullable<T>& val)
        {
            if (val.is_init())
                val->toXmlWriter(this);
        }
        // -------------------------------------------------------------------------------------

		void ReplaceString(std::wstring str1, std::wstring str2);
    };
}
