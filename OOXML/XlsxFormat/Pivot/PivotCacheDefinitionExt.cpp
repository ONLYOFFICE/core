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
#include "PivotCacheDefinitionExt.h"
#include "../../XlsbFormat/Biff12_unions/PCD14.h"
#include "../../XlsbFormat/Biff12_records/BeginPCD14.h"

namespace OOX
{
namespace Spreadsheet
{

void CPivotCacheDefinitionExt::toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
{
    writer.StartNode(sName);
    writer.StartAttributes();
    WritingNullable(m_oSlicerData, writer.WriteAttribute(L"slicerData", *m_oSlicerData););
    if(m_oPivotCacheId.IsInit())
        writer.WriteAttribute(L"pivotCacheId", *m_oPivotCacheId);
    else
        writer.WriteAttribute(L"pivotCacheId", 0);
    WritingNullable(m_oSrvSupportSubQueryNonVisual, writer.WriteAttribute(L"supportSubqueryNonVisual", *m_oSrvSupportSubQueryNonVisual););
    WritingNullable(m_oSrvSupportSubQueryCalcMem, writer.WriteAttribute(L"supportSubqueryCalcMem", *m_oSrvSupportSubQueryCalcMem););
    WritingNullable(m_oSrvSupportAddCalcMems, writer.WriteAttribute(L"supportAddCalcMems", *m_oSrvSupportAddCalcMems););
    writer.EndAttributesAndNode();


}
void CPivotCacheDefinitionExt::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
    ReadAttributes(oReader);
    if (oReader.IsEmptyNode())
        return;
    oReader.ReadTillEnd();
}
XLS::BaseObjectPtr CPivotCacheDefinitionExt::toBin()
{
    auto ptr(new XLSB::PCD14);
    XLS::BaseObjectPtr objectPtr(ptr);
    auto ptr1(new XLSB::BeginPCD14);
    ptr->m_BrtBeginPCD14 = XLS::BaseObjectPtr{ptr1};

    if(m_oSlicerData.IsInit())
        ptr1->fSlicerData = m_oSlicerData.get();
    if(m_oSrvSupportAddCalcMems.IsInit())
        ptr1->fSrvSupportAddCalcMems = m_oSrvSupportAddCalcMems.get();
    if(m_oSrvSupportSubQueryCalcMem.IsInit())
        ptr1->fSrvSupportSubQueryCalcMem = m_oSrvSupportSubQueryCalcMem.get();
    if(m_oSrvSupportSubQueryNonVisual.IsInit())
        ptr1->fSrvSupportSubQueryNonVisual = m_oSrvSupportSubQueryNonVisual.get();

    if(m_oPivotCacheId.IsInit())
        ptr1->icacheId = m_oPivotCacheId.get();
    else
        ptr1->icacheId = 0;
    return objectPtr;
}
void CPivotCacheDefinitionExt::fromBin(XLS::BaseObjectPtr& obj)
{
    if(obj->get_type() == XLS::typePCD14)
    {
        auto ptr = static_cast<XLSB::PCD14*>(obj.get());
        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCD14);
        }
    }
}
void CPivotCacheDefinitionExt::ReadAttributes(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::BeginPCD14*>(obj.get());
    if(ptr != nullptr)
    {
        m_oPivotCacheId = ptr->icacheId;
        if(ptr->fSlicerData)
            m_oSlicerData = true;
        if(ptr->fSrvSupportAddCalcMems)
            m_oSrvSupportAddCalcMems = true;
        if(ptr->fSrvSupportSubQueryCalcMem)
            m_oSrvSupportSubQueryCalcMem = true;
        if(ptr->fSrvSupportSubQueryNonVisual)
            m_oSrvSupportSubQueryNonVisual = true;
    }
}
void CPivotCacheDefinitionExt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
    WritingElement_ReadAttributes_Start( oReader )
        WritingElement_ReadAttributes_Read_if	( oReader, L"pivotCacheId", m_oPivotCacheId )
        WritingElement_ReadAttributes_Read_else_if	( oReader, L"slicerData", m_oSlicerData )
        WritingElement_ReadAttributes_Read_else_if	( oReader, L"supportSubqueryNonVisual", m_oSrvSupportSubQueryNonVisual )
        WritingElement_ReadAttributes_Read_else_if	( oReader, L"supportSubqueryCalcMem", m_oSrvSupportSubQueryCalcMem )
        WritingElement_ReadAttributes_Read_else_if	( oReader, L"supportAddCalcMems", m_oSrvSupportAddCalcMems )
    WritingElement_ReadAttributes_End( oReader )
}

} //Spreadsheet
} // namespace OOX
