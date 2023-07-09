﻿/*
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
#include "../include/xmlwriter.h"
#include "../../common/File.h"

namespace XmlUtils
{
    class CXmlWriter::Impl
    {
    public:
        std::wstring m_str;
    };

    CXmlWriter::CXmlWriter() : impl_(NULL)
    {
        impl_ = new CXmlWriter::Impl();
    }
    CXmlWriter::~CXmlWriter()
    {
        if (impl_)
            delete impl_;
        impl_ = NULL;
    }

    std::wstring CXmlWriter::GetXmlString()
    {
        if (impl_) return impl_->m_str;
        else return L"";
    }
    void CXmlWriter::SetXmlString(const std::wstring& strValue)
    {
        if (impl_) impl_->m_str = strValue;
    }

    bool CXmlWriter::SaveToFile(const std::wstring& strFilePath/*, bool bEncodingToUTF8 = false*/)
    {
        if (impl_)
            return NSFile::CFileBinary::SaveToFile(strFilePath, impl_->m_str);
        else
            return false;
    }
    void CXmlWriter::WriteString(const std::wstring& strValue)
    {
        if (impl_) impl_->m_str += strValue;
    }
    void CXmlWriter::WriteInteger(int Value)
    {
        if (impl_) impl_->m_str += std::to_wstring(Value);
    }
    void CXmlWriter::WriteDouble(double Value)
    {
        if (impl_) impl_->m_str += std::to_wstring(Value);
    }
    void CXmlWriter::WriteBoolean(bool Value)
    {
        if (!impl_) return;

        if (Value)
            impl_->m_str += (L"true");
        else
            impl_->m_str += (L"false");
    }
    void CXmlWriter::WriteNodeBegin(const std::wstring& strNodeName, bool bAttributed)
    {
        if (!impl_) return;

        impl_->m_str += (L"<") + strNodeName;

        if (!bAttributed)
            impl_->m_str += (L">");
    }
    void CXmlWriter::WriteNodeEnd(const std::wstring& strNodeName, bool bEmptyNode, bool bEndNode)
    {
        if (!impl_) return;

        if (bEmptyNode)
        {
            if (bEndNode)
                impl_->m_str += (L" />");
            else
                impl_->m_str += (L">");
        }
        else
            impl_->m_str += (L"</") + strNodeName + (L">");
    }
    void CXmlWriter::WriteNode(const std::wstring& strNodeName, const std::wstring& strNodeValue)
    {
        if (!impl_) return;

        if (strNodeValue.empty())
            impl_->m_str += L"<" + strNodeName + L"/>";
        else
            impl_->m_str += L"<" + strNodeName + L">" + strNodeValue + L"</" + strNodeName + L">";
    }
    void CXmlWriter::WriteNode(const std::wstring& strNodeName, int nValue, const std::wstring& strTextBeforeValue, const std::wstring& strTextAfterValue)
    {
        WriteNodeBegin(strNodeName);
        WriteString(strTextBeforeValue);
        WriteInteger(nValue);
        WriteString(strTextAfterValue);
        WriteNodeEnd(strNodeName);
    }
    void CXmlWriter::WriteNode(const std::wstring& strNodeName, double dValue)
    {
        WriteNodeBegin(strNodeName);
        WriteDouble(dValue);
        WriteNodeEnd(strNodeName);
    }
    void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const std::wstring& strAttributeValue)
    {
        if (impl_) impl_->m_str += L" " + strAttributeName + L"=\"" + strAttributeValue + L"\"";
    }
    void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, int nValue, const std::wstring& strTextBeforeValue, const std::wstring& strTextAfterValue)
    {
        WriteString(L" " + strAttributeName + L"=");
        WriteString(L"\"");
        WriteString(strTextBeforeValue);
        WriteInteger(nValue);
        WriteString(strTextAfterValue);
        WriteString(L"\"");
    }
    void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, double dValue)
    {
        WriteString(L" " + strAttributeName + L"=");
        WriteString(L"\"");
        WriteDouble(dValue);
        WriteString(L"\"");
    }
}
