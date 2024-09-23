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
#include "PivotHierarchies.h"

#include "../../XlsbFormat/Biff12_unions/PCDHIERARCHIES.h"
#include "../../XlsbFormat/Biff12_unions/PCDHIERARCHY.h"
#include "../../XlsbFormat/Biff12_unions/PCDHFIELDSUSAGE.h"
#include "../../XlsbFormat/Biff12_unions/PCDHGLGMEMBER.h"
#include "../../XlsbFormat/Biff12_unions/PCDHGLGMEMBERS.h"
#include "../../XlsbFormat/Biff12_unions/PCDHGLGROUP.h"
#include "../../XlsbFormat/Biff12_unions/PCDHGLGROUPS.h"
#include "../../XlsbFormat/Biff12_unions/PCDHGLEVEL.h"
#include "../../XlsbFormat/Biff12_unions/PCDHGLEVELS.h"

#include "../../XlsbFormat/Biff12_records/BeginPCDHierarchy.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDHFieldsUsage.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDHGLGMember.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDHGLGroup.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDHGLevel.h"

namespace OOX
{
namespace Spreadsheet
{
//pivot cache hierarchies

    void CpivotCacheHierarchies::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
        WritingElement_ReadAttributes_Start( oReader )
            WritingElement_ReadAttributes_Read_if		( oReader, L"count", m_oCount )
        WritingElement_ReadAttributes_End( oReader )

        auto nCurDepth = oReader.GetDepth();
        while( oReader.ReadNextSiblingNode( nCurDepth ) )
        {
            std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

            if ( L"cacheHierarchy" == sName )
            {
                CpivotCacheHierarchy* pPivotCacheHierarchy = new CpivotCacheHierarchy();
                *pPivotCacheHierarchy = oReader;
                m_arrItems.push_back(pPivotCacheHierarchy);
            }
        }
    }
    void CpivotCacheHierarchies::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<cacheHierarchies");
        WritingStringAttrInt(L"count", (int)m_arrItems.size());
        writer.WriteString(L">");

        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
        writer.WriteString(L"</cacheHierarchies>");
    }
    XLS::BaseObjectPtr CpivotCacheHierarchies::toBin()
    {
        auto ptr(new XLSB::PCDHIERARCHIES);
        XLS::BaseObjectPtr objectPtr(ptr);
        for(auto i : m_arrItems)
            ptr->m_arPCDHIERARCHY.push_back(i->toBin());
        return objectPtr;
    }
    void CpivotCacheHierarchy::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
        ReadAttributes( oReader );
        if(oReader.IsEmptyNode())
            return;
        auto nCurDepth = oReader.GetDepth();
        while( oReader.ReadNextSiblingNode( nCurDepth ) )
        {
            std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

            if ( L"fieldsUsage" == sName )
                m_oFieldsUsage = oReader;
        }
    }
    void CpivotCacheHierarchy::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
        WritingElement_ReadAttributes_Start( oReader )
            WritingElement_ReadAttributes_Read_if		( oReader, L"uniqueName", m_oUniqueName )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"measureGroup", m_oMeasureGroup )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"caption", m_oCaption )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"defaultMemberUniqueName", m_oDefaultMemberUniqueName )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"allCaption", m_oAllCaption )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"allUniqueName", m_oAllUniqueName )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"dimensionUniqueName", m_oDimensionUniqueName )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"displayFolder", m_oDisplayFolder )

            WritingElement_ReadAttributes_Read_else_if	( oReader, L"set", m_oSet )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"time", m_oTime )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"oneField", m_oOneField )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"hidden", m_oHidden )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"measures", m_oMeasure )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"measure", m_oMeasure )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"attribute", m_oAttribute )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"unbalanced", m_oUnbalanced )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"unbalancedGroup", m_oUnbalancedGroup )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"keyAttribute", m_oKeyAttribute )

            WritingElement_ReadAttributes_Read_else_if	( oReader, L"memberValueDatatype", m_oMemberValueDatatype )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"iconSet", m_oIconSet )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"parentSet", m_oParentSet )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"count", m_oCount )
        WritingElement_ReadAttributes_End( oReader )
    }
    void CpivotCacheHierarchy::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<cacheHierarchy");
        WritingStringNullableAttrEncodeXmlString2(L"uniqueName", m_oUniqueName);
        WritingStringNullableAttrEncodeXmlString2(L"caption", m_oCaption);
        WritingStringNullableAttrBool2(L"measure", m_oMeasure);
        WritingStringNullableAttrBool2(L"set", m_oSet);
        WritingStringNullableAttrUInt(L"parentSet", m_oParentSet, m_oParentSet.get());
        WritingStringNullableAttrInt(L"iconSet", m_oIconSet, m_oIconSet.get());
        WritingStringNullableAttrBool2(L"attribute", m_oAttribute);
        WritingStringNullableAttrBool2(L"time", m_oTime);
        WritingStringNullableAttrBool2(L"keyAttribute", m_oKeyAttribute);
        WritingStringNullableAttrEncodeXmlString2(L"defaultMemberUniqueName", m_oDefaultMemberUniqueName);
        WritingStringNullableAttrEncodeXmlString2(L"allUniqueName", m_oAllUniqueName);
        WritingStringNullableAttrEncodeXmlString2(L"allCaption", m_oAllCaption);
        WritingStringNullableAttrEncodeXmlString2(L"dimensionUniqueName", m_oDimensionUniqueName);
        WritingStringNullableAttrEncodeXmlString2(L"displayFolder", m_oDisplayFolder);
        WritingStringNullableAttrEncodeXmlString2(L"measureGroup", m_oMeasureGroup);
        WritingStringNullableAttrEncodeXmlString2(L"measureGroup", m_oMeasureGroup);
        WritingStringNullableAttrBool2(L"measures", m_oMeasures);
        WritingStringNullableAttrUInt(L"count", m_oCount, m_oCount.get());
        WritingStringNullableAttrBool2(L"oneField", m_oOneField);
        WritingStringNullableAttrUInt(L"memberValueDatatype", m_oMemberValueDatatype, m_oMemberValueDatatype.get());
        WritingStringNullableAttrBool2(L"unbalanced", m_oUnbalanced);
        WritingStringNullableAttrBool2(L"unbalancedGroup", m_oUnbalancedGroup);
        WritingStringNullableAttrBool2(L"hidden", m_oHidden);
        writer.WriteString(L">");

        if(m_oFieldsUsage.IsInit())
            m_oFieldsUsage->toXML(writer);
        if(m_oGroupLevels.IsInit())
            m_oGroupLevels->toXML(writer);
        writer.WriteString(L"</cacheHierarchy>");
    }
    XLS::BaseObjectPtr CpivotCacheHierarchy::toBin()
    {
        auto ptr1(new XLSB::PCDHIERARCHY);
        XLS::BaseObjectPtr objectPtr(ptr1);
        auto ptr(new XLSB::BeginPCDHierarchy);
        ptr1->m_BrtBeginPCDHierarchy = XLS::BaseObjectPtr(ptr);

        if(m_oMeasure.IsInit())
            ptr->fMeasure = m_oMeasure.get();
        if(m_oHidden.IsInit())
            ptr->fHidden = m_oHidden.get();
        if(m_oDimensionUniqueName.IsInit())
        {
            ptr->stDimUnq = m_oDimensionUniqueName.get();
            ptr->fLoadDimUnq = true;
        }
        if(m_oDefaultMemberUniqueName.IsInit())
        {
            ptr->stDefaultUnq = m_oDefaultMemberUniqueName.get();
            ptr->fLoadDefaultUnq = true;
        }
        if(m_oAllUniqueName.IsInit())
        {
            ptr->stAllUnq = m_oAllUniqueName.get();
            ptr->fLoadAllUnq = true;
        }
        if(m_oDisplayFolder.IsInit())
        {
            ptr->stDispFld = m_oDisplayFolder.get();
            ptr->fLoadDispFld = false;
        }
        if(m_oOneField.IsInit())
            ptr->fOnlyOneField = m_oOneField.get();
        if(m_oMemberValueDatatype.IsInit())
            ptr->wAttributeMemberValueType = m_oMemberValueDatatype.get();
        if(m_oCaption.IsInit())
            ptr->stCaption = m_oCaption.get();
         else
            ptr->stCaption = L"";
         if(m_oUniqueName.IsInit())
            ptr->stUnique = m_oUniqueName.get();
         else
            ptr->stUnique = L"";
         if(m_oIconSet.IsInit())
             ptr->iconSet.set = m_oIconSet.get();
         else
             ptr->iconSet.set = 0;

        if(m_oFieldsUsage.IsInit())
            ptr1->m_PCDHFIELDSUSAGE = m_oFieldsUsage->toBin();
        if(m_oGroupLevels.IsInit())
            ptr1->m_PCDHGLEVELS = m_oGroupLevels->toBin();
        return objectPtr;
    }

    void CfieldsUsage::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
        ReadAttributes( oReader );
        auto nCurDepth = oReader.GetDepth();
        while( oReader.ReadNextSiblingNode( nCurDepth ) )
        {
            std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

            if ( L"fieldUsage" == sName )
            {
                std::wstring val;
                WritingElement_ReadAttributes_Start( oReader )
                    WritingElement_ReadAttributes_Read_if	( oReader, L"x", val )
                WritingElement_ReadAttributes_End( oReader )
                m_oFieldUsage.push_back(XmlUtils::GetInteger(val));
            }
        }
    }
    void CfieldsUsage::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
        WritingElement_ReadAttributes_Start( oReader )
            WritingElement_ReadAttributes_Read_if	( oReader, L"count", m_oCount )
        WritingElement_ReadAttributes_End( oReader )
    }
    void CfieldsUsage::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<fieldsUsage");
        WritingStringAttrInt(L"count", (int)m_oFieldUsage.size());
        writer.WriteString(L">");

        for (auto i:m_oFieldUsage)
        {
            writer.WriteString(L"<fieldUsage");
            WritingStringAttrInt(L"x", i);
            writer.WriteString(L"/>");
        }
        writer.WriteString(L"</fieldsUsage>");
    }
    XLS::BaseObjectPtr CfieldsUsage::toBin()
    {
        auto ptr1(new XLSB::PCDHFIELDSUSAGE);
        XLS::BaseObjectPtr objectPtr(ptr1);
        auto ptr(new XLSB::BeginPCDHFieldsUsage);
        ptr1->m_BrtBeginPCDHFieldsUsage = XLS::BaseObjectPtr {ptr};
        ptr->cItems = m_oCount.get();
        ptr->rgifdb = m_oFieldUsage;
        return objectPtr;
     }
    void CGroupLevels::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
       ReadAttributes( oReader );
       auto nCurDepth = oReader.GetDepth();
       while( oReader.ReadNextSiblingNode( nCurDepth ) )
       {
           std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
           if ( L"groupLevel" == sName )
           {
               CGroupLevel* pLevel = new CGroupLevel();
               *pLevel = oReader;
               m_arrItems.push_back(pLevel);
           }
       }
    }
    void CGroupLevels::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"count", m_oCount )
       WritingElement_ReadAttributes_End( oReader )
    }
    void CGroupLevels::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<groupLevels");
        WritingStringAttrInt(L"count", (int)m_arrItems.size());
        writer.WriteString(L">");

        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
        writer.WriteString(L"</groupLevels>");
    }
    XLS::BaseObjectPtr CGroupLevels::toBin()
    {
       auto ptr(new XLSB::PCDHGLEVELS);
       XLS::BaseObjectPtr objectPtr(ptr);
       for(auto i : m_arrItems)
           ptr->m_arPCDHGLEVEL.push_back(i->toBin());
       return objectPtr;
    }

    void CGroupLevel::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
      ReadAttributes( oReader );
      auto nCurDepth = oReader.GetDepth();
      while( oReader.ReadNextSiblingNode( nCurDepth ) )
      {
          std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
          if ( L"groups" == sName )
          {
              m_oGroups = oReader;
          }

      }
    }
    void CGroupLevel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
      WritingElement_ReadAttributes_Start( oReader )
          WritingElement_ReadAttributes_Read_if	( oReader, L"uniqueName", m_oUniqueName )
          WritingElement_ReadAttributes_Read_else_if( oReader, L"caption", m_oCaption )
          WritingElement_ReadAttributes_Read_if	( oReader, L"user", m_oUser )
          WritingElement_ReadAttributes_Read_else_if( oReader, L"customRollUp", m_oCustomRollUp )
      WritingElement_ReadAttributes_End( oReader )
    }
    void CGroupLevel::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<groupLevel");
        WritingStringNullableAttrEncodeXmlString2(L"uniqueName", m_oUniqueName);
        WritingStringNullableAttrEncodeXmlString2(L"caption", m_oCaption);
        WritingStringNullableAttrBool2(L"user", m_oUser);
        WritingStringNullableAttrBool2(L"customRollUp", m_oCustomRollUp);
        if(m_oGroups.IsInit())
        {
             writer.WriteString(L">");
             m_oGroups->toXML();
             writer.WriteString(L"</groupLevel>");
        }
        else
            writer.WriteString(L"/>");
    }
    XLS::BaseObjectPtr CGroupLevel::toBin()
    {
      auto ptr1(new XLSB::PCDHGLEVEL);
      XLS::BaseObjectPtr objectPtr(ptr1);
      auto ptr(new XLSB::BeginPCDHGLevel);
      ptr1->m_BrtBeginPCDHGLevel = XLS::BaseObjectPtr{ptr};

      if(m_oUniqueName.IsInit())
          ptr->stUnique = m_oUniqueName.get();
      if(m_oUser.IsInit())
          ptr->fGroupLevel = m_oUser.get();
      if(m_oCaption.IsInit())
          ptr->stLevelName = m_oCaption.get();

      if(m_oGroups.IsInit())
          ptr1->m_PCDHGLGROUPS = m_oGroups->toBin();

      return objectPtr;
    }

    void CGroups::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
     ReadAttributes( oReader );
     auto nCurDepth = oReader.GetDepth();
     while( oReader.ReadNextSiblingNode( nCurDepth ) )
     {
         std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
         if ( L"group" == sName )
         {
             CGroup* pGroup = new CGroup();
             *pGroup = oReader;
             m_arrItems.push_back(pGroup);
         }
     }
    }
    void CGroups::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
     WritingElement_ReadAttributes_Start( oReader )
         WritingElement_ReadAttributes_Read_if	( oReader, L"count", m_oCount )
     WritingElement_ReadAttributes_End( oReader )
    }
    void CGroups::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<groups");
        WritingStringAttrInt(L"count", (int)m_arrItems.size());
        writer.WriteString(L">");

        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
        writer.WriteString(L"</groups>");
    }
    XLS::BaseObjectPtr CGroups::toBin()
    {
     auto ptr(new XLSB::PCDHGLGROUPS);
     XLS::BaseObjectPtr objectPtr(ptr);
     for(auto i:m_arrItems)
         ptr->m_arPCDHGLGROUP.push_back(i->toBin());

     return objectPtr;
    }

    void CGroup::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
        ReadAttributes( oReader );
        auto nCurDepth = oReader.GetDepth();
        while( oReader.ReadNextSiblingNode( nCurDepth ) )
        {
            std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
            if ( L"groupMembers" == sName )
            {
                m_oGroupMembers = oReader;
            }
        }
    }
    void CGroup::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
        WritingElement_ReadAttributes_Start( oReader )
            WritingElement_ReadAttributes_Read_if	( oReader, L"id", m_oId )
            WritingElement_ReadAttributes_Read_else_if( oReader, L"name", m_oName )
            WritingElement_ReadAttributes_Read_if	( oReader, L"uniqueName", m_oUniqueName )
            WritingElement_ReadAttributes_Read_else_if( oReader, L"caption", m_oCaption )
            WritingElement_ReadAttributes_Read_else_if( oReader, L"uniqueParent", m_oUniqueParent )
        WritingElement_ReadAttributes_End( oReader )
    }
    void CGroup::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<group");
        WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
        WritingStringNullableAttrEncodeXmlString2(L"uniqueName", m_oUniqueName);
        WritingStringNullableAttrEncodeXmlString2(L"caption", m_oCaption);
        WritingStringNullableAttrEncodeXmlString2(L"uniqueParent", m_oUniqueParent);
        WritingStringNullableAttrInt(L"id", m_oId, m_oId.get());
        if(m_oGroupMembers.IsInit())
        {
             writer.WriteString(L">");
             m_oGroupMembers->toXML();
             writer.WriteString(L"</group>");
        }
        else
            writer.WriteString(L"/>");
    }
    XLS::BaseObjectPtr CGroup::toBin()
    {
        auto ptr1(new XLSB::PCDHGLGROUP);
        XLS::BaseObjectPtr objectPtr(ptr1);
        auto ptr(new XLSB::BeginPCDHGLGroup);
        ptr1->m_BrtBeginPCDHGLGroup  = XLS::BaseObjectPtr{ptr};

        if(m_oId.IsInit())
            ptr->iGrpNum = m_oId.get();
        if(m_oUniqueName.IsInit())
            ptr->stUniqueName = m_oUniqueName.get();
        if(m_oName.IsInit())
            ptr->stName = m_oName.get();
        if(m_oCaption.IsInit())
            ptr->stCaption = m_oCaption.get();
        if(m_oUniqueParent.IsInit())
            ptr->stParentUniqueName = m_oUniqueParent.get();

        if(m_oGroupMembers.IsInit())
            ptr1->m_PCDHGLGMEMBERS = m_oGroupMembers->toBin();

        return objectPtr;
    }

    void CGroupMembers::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
       ReadAttributes( oReader );
       auto nCurDepth = oReader.GetDepth();
       while( oReader.ReadNextSiblingNode( nCurDepth ) )
       {
           std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
           if ( L"groupMember" == sName )
           {
               CGroupMember* pMember = new CGroupMember();
               *pMember = oReader;
               m_arrItems.push_back(pMember);
           }
       }
    }
    void CGroupMembers::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"count", m_oCount )
       WritingElement_ReadAttributes_End( oReader )
    }
    void CGroupMembers::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<groupMembers");
        WritingStringAttrInt(L"count", (int)m_arrItems.size());
        writer.WriteString(L">");

        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
        writer.WriteString(L"</groupMembers>");
    }
    XLS::BaseObjectPtr CGroupMembers::toBin()
    {
       auto ptr(new XLSB::PCDHGLGMEMBERS);
       XLS::BaseObjectPtr objectPtr(ptr);
       for(auto i : m_arrItems)
       {
           ptr->m_arPCDHGLGMEMBER.push_back(i->toBin());
       }
       return objectPtr;
    }

    void CGroupMember::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
       ReadAttributes( oReader );
    }
    void CGroupMember::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"group", m_oGroup )
           WritingElement_ReadAttributes_Read_else_if( oReader, L"uniqueName", m_oUniqueName )
       WritingElement_ReadAttributes_End( oReader )
    }
    void CGroupMember::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<groupMember");
        WritingStringNullableAttrEncodeXmlString2(L"uniqueName", m_oUniqueName);
        WritingStringNullableAttrBool2(L"group", m_oGroup);
        writer.WriteString(L"/>");
    }
    XLS::BaseObjectPtr CGroupMember::toBin()
    {
       auto ptr1(new XLSB::PCDHGLGMEMBER);
       XLS::BaseObjectPtr objectPtr(ptr1);
       auto ptr(new XLSB::BeginPCDHGLGMember);
       ptr1->m_BrtBeginPCDHGLGMember = XLS::BaseObjectPtr{ptr};
       
       if(m_oGroup.IsInit())
           ptr->fGroup = m_oGroup.get();
       if(m_oUniqueName.IsInit())
           ptr->stUnique = m_oUniqueName.get();
       else
           ptr->stUnique = L"";

       return objectPtr;
    }
//pivot table hierarchies

    void CpivotTableHierarchies::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
        WritingElement_ReadAttributes_Start( oReader )
            WritingElement_ReadAttributes_Read_if		( oReader, L"count", m_oCount )
        WritingElement_ReadAttributes_End( oReader )

        auto nCurDepth = oReader.GetDepth();
        while( oReader.ReadNextSiblingNode( nCurDepth ) )
        {
            std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

            if ( L"pivotHierarchy" == sName )
            {
                CpivotTableHierarchy* pPivotCacheHierarchy = new CpivotTableHierarchy();
                *pPivotCacheHierarchy = oReader;
                m_arrItems.push_back(pPivotCacheHierarchy);
            }
        }
    }
    void CpivotTableHierarchies::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<pivotHierarchies");
        WritingStringAttrInt(L"count", (int)m_arrItems.size());
        writer.WriteString(L">");

        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
        writer.WriteString(L"</pivotHierarchies>");
    }
    XLS::BaseObjectPtr CpivotTableHierarchies::toBin()
    {
        auto ptr(new XLSB::PCDHIERARCHIES);
        XLS::BaseObjectPtr objectPtr(ptr);
        for(auto i : m_arrItems)
            ptr->m_arPCDHIERARCHY.push_back(i->toBin());
        return objectPtr;
    }

    void CpivotTableHierarchy::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
       ReadAttributes( oReader );
    }
    void CpivotTableHierarchy::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"outline", m_oOutline )
           WritingElement_ReadAttributes_Read_else_if( oReader, L"multipleItemSelectionAllowed", m_oMultipleItemSelectionAllowed )
       WritingElement_ReadAttributes_End( oReader )
    }
    void CpivotTableHierarchy::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<groupMember");

        writer.WriteString(L"/>");
    }
    XLS::BaseObjectPtr CpivotTableHierarchy::toBin()
    {
       auto ptr1(new XLSB::PCDHGLGMEMBER);
       XLS::BaseObjectPtr objectPtr(ptr1);
       auto ptr(new XLSB::BeginPCDHGLGMember);
       ptr1->m_BrtBeginPCDHGLGMember = XLS::BaseObjectPtr{ptr};


       return objectPtr;
    }
}
}
