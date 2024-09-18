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
#include "PivotCacheHierarchies.h"

#include "../../XlsbFormat/Biff12_unions/PCDHIERARCHIES.h"
#include "../../XlsbFormat/Biff12_unions/PCDHIERARCHY.h"
#include "../../XlsbFormat/Biff12_unions/PCDHFIELDSUSAGE.h"

#include "../../XlsbFormat/Biff12_records/BeginPCDHierarchy.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDHFieldsUsage.h"

namespace OOX
{
namespace Spreadsheet
{
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
        if(m_oFieldsUsage.IsInit())
            ptr1->m_PCDHFIELDSUSAGE = m_oFieldsUsage->toBin();
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

    }
    XLS::BaseObjectPtr CGroupLevels::toBin()
    {
       auto ptr1(new XLSB::PCDHFIELDSUSAGE);
       XLS::BaseObjectPtr objectPtr(ptr1);

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

    }
    XLS::BaseObjectPtr CGroupLevel::toBin()
    {
      auto ptr1(new XLSB::PCDHFIELDSUSAGE);
      XLS::BaseObjectPtr objectPtr(ptr1);

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

    }
    XLS::BaseObjectPtr CGroups::toBin()
    {
     auto ptr1(new XLSB::PCDHFIELDSUSAGE);
     XLS::BaseObjectPtr objectPtr(ptr1);

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

    }
    XLS::BaseObjectPtr CGroup::toBin()
    {
        auto ptr1(new XLSB::PCDHFIELDSUSAGE);
        XLS::BaseObjectPtr objectPtr(ptr1);

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

    }
    XLS::BaseObjectPtr CGroupMembers::toBin()
    {
       auto ptr1(new XLSB::PCDHFIELDSUSAGE);
       XLS::BaseObjectPtr objectPtr(ptr1);

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

    }
    XLS::BaseObjectPtr CGroupMember::toBin()
    {
       auto ptr1(new XLSB::PCDHFIELDSUSAGE);
       XLS::BaseObjectPtr objectPtr(ptr1);

       return objectPtr;
    }

}
}
