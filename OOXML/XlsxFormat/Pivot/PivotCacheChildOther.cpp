/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
#pragma once
#include "PivotCacheChildOther.h"

#include "../../XlsbFormat/Biff12_unions/DIMS.h"
#include "../../XlsbFormat/Biff12_unions/DIM.h"
#include "../../XlsbFormat/Biff12_unions/MGS.h"
#include "../../XlsbFormat/Biff12_unions/MG.h"
#include "../../XlsbFormat/Biff12_unions/MGMAPS.h"
#include "../../XlsbFormat/Biff12_unions/MAP.h"

#include "../../XlsbFormat/Biff12_records/BeginDim.h"
#include "../../XlsbFormat/Biff12_records/BeginMG.h"
#include "../../XlsbFormat/Biff12_records/BeginMap.h"

namespace OOX
{
namespace Spreadsheet
{

void CPivotDimensions::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
    WritingElement_ReadAttributes_Start( oReader )
        WritingElement_ReadAttributes_Read_if		( oReader, L"count", m_oCount )
    WritingElement_ReadAttributes_End( oReader )

    auto nCurDepth = oReader.GetDepth();
    while( oReader.ReadNextSiblingNode( nCurDepth ) )
    {
        std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

        if ( L"dimension" == sName )
        {
            CPivotDimension* pDimension = new CPivotDimension();
            *pDimension = oReader;
            m_arrItems.push_back(pDimension);
        }
    }
}

void CPivotDimensions::toXML(NSStringUtils::CStringBuilder& writer) const
{
    writer.WriteString(L"<dimensions");
    WritingStringAttrInt(L"count", (int)m_arrItems.size());
    writer.WriteString(L">");

    for ( size_t i = 0; i < m_arrItems.size(); ++i)
    {
        if (  m_arrItems[i] )
        {
            m_arrItems[i]->toXML(writer);
        }
    }
    writer.WriteString(L"</dimensions>");
}

void CPivotDimensions::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::DIMS*>(obj.get());
    if(!ptr)
        return;
    m_oCount = (_UINT32)ptr->m_arDIM .size();
    for(auto i:ptr->m_arDIM)
        m_arrItems.push_back(new CPivotDimension(i));
}

XLS::BaseObjectPtr CPivotDimensions::toBin()
{
    auto ptr(new XLSB::DIMS);
    XLS::BaseObjectPtr objectPtr(ptr);
    for(auto i : m_arrItems)
        ptr->m_arDIM.push_back(i->toBin());
    return objectPtr;
}

void CPivotDimension::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
    ReadAttributes( oReader );
}

void CPivotDimension::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
    WritingElement_ReadAttributes_Start( oReader )
        WritingElement_ReadAttributes_Read_if		( oReader, L"uniqueName", m_oUniqueName )
        WritingElement_ReadAttributes_Read_else_if	( oReader, L"measure", m_oMeasure )
        WritingElement_ReadAttributes_Read_else_if	( oReader, L"name", m_oName )
        WritingElement_ReadAttributes_Read_else_if	( oReader, L"caption", m_oCaption )
    WritingElement_ReadAttributes_End( oReader )
}

void CPivotDimension::toXML(NSStringUtils::CStringBuilder& writer) const
{
    writer.WriteString(L"<dimension");
    WritingStringNullableAttrBool2(L"measure", m_oMeasure);
    WritingStringNullableAttrEncodeXmlString2( L"name", m_oName );
    WritingStringNullableAttrEncodeXmlString2( L"uniqueName", m_oUniqueName);
    WritingStringNullableAttrEncodeXmlString2( L"caption", m_oCaption);
    writer.WriteString(L"/>");
}

void CPivotDimension::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr1 = static_cast<XLSB::DIM*>(obj.get());
    auto ptr = static_cast<XLSB::BeginDim*>(ptr1->m_BrtBeginDim.get());

    m_oMeasure = ptr->fMeasure;
    m_oName = ptr->stName;
    m_oCaption = ptr->stDisplay;
    m_oUniqueName = ptr->stUnique;
}

XLS::BaseObjectPtr CPivotDimension::toBin()
{
    auto ptr1(new XLSB::DIM);
    XLS::BaseObjectPtr objectPtr(ptr1);
    auto ptr(new XLSB::BeginDim);
    ptr1->m_BrtBeginDim = XLS::BaseObjectPtr(ptr);
    if(m_oMeasure.IsInit())
        ptr->fMeasure = m_oMeasure.get();
    if(m_oName.IsInit())
        ptr->stName = m_oName.get();
    else
        ptr->stName = L"";
    if(m_oCaption.IsInit())
        ptr->stDisplay = m_oCaption.get();
    else
        ptr->stDisplay = L"";
    if(m_oUniqueName.IsInit())
        ptr->stUnique = m_oUniqueName.get();
    else
        ptr->stUnique = L"";
    return objectPtr;
}

void CPivotMeasureGroups::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
    WritingElement_ReadAttributes_Start( oReader )
        WritingElement_ReadAttributes_Read_if		( oReader, L"count", m_oCount )
    WritingElement_ReadAttributes_End( oReader )

    auto nCurDepth = oReader.GetDepth();
    while( oReader.ReadNextSiblingNode( nCurDepth ) )
    {
        std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

        if ( L"measureGroup" == sName )
        {
            CPivotMeasureGroup* pMGroup = new CPivotMeasureGroup();
            *pMGroup = oReader;
            m_arrItems.push_back(pMGroup);
        }
    }
}

void CPivotMeasureGroups::toXML(NSStringUtils::CStringBuilder& writer) const
{
    writer.WriteString(L"<measureGroups");
    WritingStringAttrInt(L"count", (int)m_arrItems.size());
    writer.WriteString(L">");

    for ( size_t i = 0; i < m_arrItems.size(); ++i)
    {
        if (  m_arrItems[i] )
        {
            m_arrItems[i]->toXML(writer);
        }
    }
    writer.WriteString(L"</measureGroups>");
}

void CPivotMeasureGroups::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::MGS*>(obj.get());
    if(!ptr)
        return;
    m_oCount = (_UINT32)ptr->m_arMG .size();
    for(auto i:ptr->m_arMG)
        m_arrItems.push_back(new CPivotMeasureGroup(i));
}

XLS::BaseObjectPtr CPivotMeasureGroups::toBin()
{
    auto ptr(new XLSB::MGS);
    XLS::BaseObjectPtr objectPtr(ptr);
    for(auto i : m_arrItems)
        ptr->m_arMG.push_back(i->toBin());
    return objectPtr;
}

void CPivotMeasureGroup::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
    ReadAttributes( oReader );
}

void CPivotMeasureGroup::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
    WritingElement_ReadAttributes_Start( oReader )
        WritingElement_ReadAttributes_Read_if	( oReader, L"name", m_oName )
        WritingElement_ReadAttributes_Read_else_if	( oReader, L"caption", m_oCaption )
    WritingElement_ReadAttributes_End( oReader )
}

void CPivotMeasureGroup::toXML(NSStringUtils::CStringBuilder& writer) const
{
    writer.WriteString(L"<measureGroup");
    WritingStringNullableAttrEncodeXmlString2( L"name", m_oName );
    WritingStringNullableAttrEncodeXmlString2( L"caption", m_oCaption);
    writer.WriteString(L"/>");
}

void CPivotMeasureGroup::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr1 = static_cast<XLSB::MG*>(obj.get());
    auto ptr = static_cast<XLSB::BeginMG*>(ptr1->m_BrtBeginMG.get());

    m_oName = ptr->name;
    m_oCaption = ptr->caption;
}

XLS::BaseObjectPtr CPivotMeasureGroup::toBin()
{
    auto ptr1(new XLSB::MG);
    XLS::BaseObjectPtr objectPtr(ptr1);
    auto ptr(new XLSB::BeginMG);
    ptr1->m_BrtBeginMG = XLS::BaseObjectPtr(ptr);
    if(m_oName.IsInit())
        ptr->name = m_oName.get();
    else
        ptr->name = L"";
    if(m_oCaption.IsInit())
        ptr->caption = m_oCaption.get();
    else
        ptr->caption = L"";
    return objectPtr;
}

void CMeasureDimensionMaps::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
    WritingElement_ReadAttributes_Start( oReader )
        WritingElement_ReadAttributes_Read_if		( oReader, L"count", m_oCount )
    WritingElement_ReadAttributes_End( oReader )

    auto nCurDepth = oReader.GetDepth();
    while( oReader.ReadNextSiblingNode( nCurDepth ) )
    {
        std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

        if ( L"map" == sName )
        {
            CMeasureDimensionMap* pMap = new CMeasureDimensionMap();
            *pMap = oReader;
            m_arrItems.push_back(pMap);
        }
    }
}

void CMeasureDimensionMaps::toXML(NSStringUtils::CStringBuilder& writer) const
{
    writer.WriteString(L"<maps");
    WritingStringAttrInt(L"count", (int)m_arrItems.size());
    writer.WriteString(L">");

    for ( size_t i = 0; i < m_arrItems.size(); ++i)
    {
        if (  m_arrItems[i] )
        {
            m_arrItems[i]->toXML(writer);
        }
    }
    writer.WriteString(L"</maps>");
}

void CMeasureDimensionMaps::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr = static_cast<XLSB::MGMAPS*>(obj.get());
    if(!ptr)
        return;
    m_oCount = (_UINT32)ptr->m_arMAP.size();
    for(auto i:ptr->m_arMAP)
        m_arrItems.push_back(new CMeasureDimensionMap(i));
}

XLS::BaseObjectPtr CMeasureDimensionMaps::toBin()
{
    auto ptr(new XLSB::MGMAPS);
    XLS::BaseObjectPtr objectPtr(ptr);
    for(auto i : m_arrItems)
        ptr->m_arMAP.push_back(i->toBin());
    return objectPtr;
}

void CMeasureDimensionMap::fromXML(XmlUtils::CXmlLiteReader& oReader)
{
    ReadAttributes( oReader );
}

void CMeasureDimensionMap::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
{
    WritingElement_ReadAttributes_Start( oReader )
        WritingElement_ReadAttributes_Read_if	( oReader, L"measureGroup", m_oMeasureGroup )
        WritingElement_ReadAttributes_Read_else_if	( oReader, L"dimension", m_oDimension )
    WritingElement_ReadAttributes_End( oReader )
}

void CMeasureDimensionMap::toXML(NSStringUtils::CStringBuilder& writer) const
{
    writer.WriteString(L"<map");
    WritingStringNullableAttrUInt( L"measureGroup", m_oMeasureGroup,  m_oMeasureGroup.get());
    WritingStringNullableAttrUInt( L"dimension", m_oDimension, m_oDimension.get());
    writer.WriteString(L"/>");
}

void CMeasureDimensionMap::fromBin(XLS::BaseObjectPtr& obj)
{
    auto ptr1 = static_cast<XLSB::MAP*>(obj.get());
    auto ptr = static_cast<XLSB::BeginMap*>(ptr1->m_BrtBeginMap.get());

    m_oDimension = ptr->isxdh;
    m_oMeasureGroup = ptr->img;
}

XLS::BaseObjectPtr CMeasureDimensionMap::toBin()
{
    auto ptr1(new XLSB::MAP);
    XLS::BaseObjectPtr objectPtr(ptr1);
    auto ptr(new XLSB::BeginMap);
    ptr1->m_BrtBeginMap = XLS::BaseObjectPtr(ptr);

    if(m_oDimension.IsInit())
        ptr->isxdh = m_oDimension.get();
    if(m_oMeasureGroup.IsInit())
        ptr->img = m_oMeasureGroup.get();

    return objectPtr;
}

}
}
