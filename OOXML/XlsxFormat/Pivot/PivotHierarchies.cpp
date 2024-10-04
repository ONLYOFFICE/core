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
#include "../../XlsbFormat/Biff12_unions/SXTHITEM.h"
#include "../../XlsbFormat/Biff12_unions/SXTHITEMS.h"
#include "../../XlsbFormat/Biff12_unions/SXTDMPS.h"
#include "../../XlsbFormat/Biff12_unions/SXTDMP.h"
#include "../../XlsbFormat/Biff12_unions/SXTH.h"
#include "../../XlsbFormat/Biff12_unions/SXTHS.h"
#include "../../XlsbFormat/Biff12_unions/ISXTHRWS.h"
#include "../../XlsbFormat/Biff12_unions/ISXTHCOLS.h"

#include "../../XlsbFormat/Biff12_records/BeginPCDHierarchy.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDHFieldsUsage.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDHGLGMember.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDHGLGroup.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDHGLevel.h"
#include "../../XlsbFormat/Biff12_records/BeginSXTHItem.h"
#include "../../XlsbFormat/Biff12_records/BeginSXTHItems.h"
#include "../../XlsbFormat/Biff12_records/BeginSXTDMP.h"
#include "../../XlsbFormat/Biff12_records/BeginSXTH.h"
#include "../../XlsbFormat/Biff12_records/BeginISXTHCols.h"
#include "../../XlsbFormat/Biff12_records/BeginISXTHRws.h"

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
    void CpivotCacheHierarchies::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDHIERARCHIES*>(obj.get());
        if(!ptr)
            return;
        m_oCount = (_UINT32)ptr->m_arPCDHIERARCHY.size();
        for(auto i:ptr->m_arPCDHIERARCHY)
            m_arrItems.push_back(new CpivotCacheHierarchy(i));
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
        WritingStringNullableAttrBool2(L"measures", m_oMeasures);
        WritingStringNullableAttrUInt(L"count", m_oCount, m_oCount.get());
        WritingStringNullableAttrBool2(L"oneField", m_oOneField);
        WritingStringNullableAttrUInt(L"memberValueDatatype", m_oMemberValueDatatype, m_oMemberValueDatatype.get());
        WritingStringNullableAttrBool2(L"unbalanced", m_oUnbalanced);
        WritingStringNullableAttrBool2(L"unbalancedGroup", m_oUnbalancedGroup);
        WritingStringNullableAttrBool2(L"hidden", m_oHidden);
        if(!m_oFieldsUsage.IsInit() && !m_oGroupLevels.IsInit())
        {
            writer.WriteString(L"/>");
            return;
        }
        writer.WriteString(L">");

        if(m_oFieldsUsage.IsInit())
            m_oFieldsUsage->toXML(writer);
        if(m_oGroupLevels.IsInit())
            m_oGroupLevels->toXML(writer);
        writer.WriteString(L"</cacheHierarchy>");
    }
    void CpivotCacheHierarchy::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr1 = static_cast<XLSB::PCDHIERARCHY*>(obj.get());
        auto ptr = static_cast<XLSB::BeginPCDHierarchy*>(ptr1->m_BrtBeginPCDHierarchy.get());

        if(ptr->fMeasure)
            m_oMeasure = ptr->fMeasure;
        if(ptr->fMeasureHierarchy)
        m_oMeasures = ptr->fMeasureHierarchy;
        if(ptr->fHidden)
            m_oHidden = ptr->fHidden;
        if(ptr->fAttributeHierarchy)
            m_oAttribute = ptr->fAttributeHierarchy;
        if(ptr->fKeyAttributeHierarchy)
            m_oKeyAttribute = ptr->fKeyAttributeHierarchy;
        if(ptr->fOnlyOneField)
            m_oOneField = ptr->fOnlyOneField;
        if(ptr->fSet)
            m_oSet = ptr->fSet;
        if(ptr->fTimeHierarchy)
            m_oTime = ptr->fTimeHierarchy;
        m_oCount = ptr->cLevels;

        if(ptr->fUnbalancedRealKnown)
            m_oUnbalanced = ptr->fUnbalancedReal;
        if(ptr->fUnbalancedGroupKnown)
            m_oUnbalancedGroup = ptr->fUnbalancedGroup;
        if(ptr->fLoadDimUnq)
            m_oDimensionUniqueName = ptr->stDimUnq;
        if(ptr->fLoadDefaultUnq)
            m_oDefaultMemberUniqueName = ptr->stDefaultUnq;
        if(ptr->fLoadAllUnq)
            m_oAllUniqueName = ptr->stAllUnq;
        if(ptr->fLoadAllDisp)
            m_oAllCaption = ptr->stAllDisp;
        if(ptr->fLoadDispFld)
            m_oDisplayFolder = ptr->stDispFld;
        if(ptr->fLoadMeasGrp)
            m_oMeasureGroup = ptr->stMeasGrp;
        if(ptr->fAttributeMemberValueTypeKnown)
            m_oMemberValueDatatype = ptr->wAttributeMemberValueType;
        m_oCaption = ptr->stCaption;
        m_oUniqueName = ptr->stUnique;
        if(ptr->iconSet.set > 0)
            m_oIconSet = ptr->iconSet.set;

        if(ptr1->m_PCDHFIELDSUSAGE != nullptr)
            m_oFieldsUsage = ptr1->m_PCDHFIELDSUSAGE;
        if(ptr1->m_PCDHGLEVELS != nullptr)
            m_oGroupLevels = ptr1->m_PCDHGLEVELS;
    }

    XLS::BaseObjectPtr CpivotCacheHierarchy::toBin()
    {
        auto ptr1(new XLSB::PCDHIERARCHY);
        XLS::BaseObjectPtr objectPtr(ptr1);
        auto ptr(new XLSB::BeginPCDHierarchy);
        ptr1->m_BrtBeginPCDHierarchy = XLS::BaseObjectPtr(ptr);

        if(m_oMeasure.IsInit())
            ptr->fMeasure = m_oMeasure.get();
        if(m_oMeasures.IsInit())
            ptr->fMeasureHierarchy = m_oMeasures.get();
        if(m_oHidden.IsInit())
            ptr->fHidden = m_oHidden.get();
        if(m_oAttribute.IsInit())
            ptr->fAttributeHierarchy = m_oAttribute.get();
        if(m_oKeyAttribute.IsInit())
            ptr->fKeyAttributeHierarchy = m_oKeyAttribute.get();
        if(m_oOneField.IsInit())
            ptr->fOnlyOneField = m_oOneField.get();
        if(m_oSet.IsInit())
            ptr->fSet = m_oSet.get();
        if(m_oTime.IsInit())
            ptr->fTimeHierarchy = m_oTime.get();

        if(m_oUnbalanced.IsInit())
        {
            ptr->fUnbalancedRealKnown = true;
            ptr->fUnbalancedReal = m_oUnbalanced.get();
        }
        if(m_oUnbalancedGroup.IsInit())
        {
            ptr->fUnbalancedGroupKnown = true;
            ptr->fUnbalancedGroup = m_oUnbalancedGroup.get();
        }
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
        if(m_oAllCaption.IsInit())
        {
            ptr->stAllDisp = m_oAllCaption.get();
            ptr->fLoadAllDisp = true;
        }
        if(m_oDisplayFolder.IsInit())
        {
            ptr->stDispFld = m_oDisplayFolder.get();
            ptr->fLoadDispFld = true;
        }
        if(m_oMeasureGroup.IsInit())
        {
            ptr->stMeasGrp = m_oMeasureGroup.get();
            ptr->fLoadMeasGrp = true;
        }
        if(m_oMemberValueDatatype.IsInit())
        {
            ptr->fAttributeMemberValueTypeKnown = true;
            ptr->wAttributeMemberValueType = m_oMemberValueDatatype.get();
        }

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
         if(m_oCount.IsInit())
             ptr->cLevels = m_oCount.get();

        if(m_oFieldsUsage.IsInit())
            ptr1->m_PCDHFIELDSUSAGE = m_oFieldsUsage->toBin();
        if(m_oGroupLevels.IsInit())
            ptr1->m_PCDHGLEVELS = m_oGroupLevels->toBin();
        return objectPtr;
    }

    void CfieldsUsage::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
        ReadAttributes( oReader );
        if ( oReader.IsEmptyNode() )
			return;
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

    void CfieldsUsage::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr1 = static_cast<XLSB::PCDHFIELDSUSAGE*>(obj.get());
        auto ptr = static_cast<XLSB::BeginPCDHFieldsUsage*>(ptr1->m_BrtBeginPCDHFieldsUsage.get());
        m_oCount = ptr->rgifdb.size();
        m_oFieldUsage = ptr->rgifdb;
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

    void CGroupLevels::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDHGLEVELS*>(obj.get());
        if(!ptr)
            return;
        m_oCount = ptr->m_arPCDHGLEVEL.size();
        for(auto i:ptr->m_arPCDHGLEVEL)
            m_arrItems.push_back(new CGroupLevel(i));
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
      if ( oReader.IsEmptyNode() )
			return;
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

    void CGroupLevel::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr1 = static_cast<XLSB::PCDHGLEVEL*>(obj.get());
        auto ptr = static_cast<XLSB::BeginPCDHGLevel*>(ptr1->m_BrtBeginPCDHGLevel.get());
        m_oUser = ptr->fGroupLevel;
        m_oUniqueName = ptr->stUnique;
        m_oCaption = ptr->stLevelName;

        m_oGroups = ptr1->m_PCDHGLGROUPS;
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

    void CGroups::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDHGLGROUPS*>(obj.get());
        if(!ptr)
            return;
        m_oCount = ptr->m_arPCDHGLGROUP.size();
        for(auto i:ptr->m_arPCDHGLGROUP)
            m_arrItems.push_back(new CGroup(i));
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
        if ( oReader.IsEmptyNode() )
			return;
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

    void CGroup::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr1 = static_cast<XLSB::PCDHGLGROUP*>(obj.get());
        auto ptr = static_cast<XLSB::BeginPCDHGLGroup*>(ptr1->m_BrtBeginPCDHGLGroup.get());
        m_oId = ptr->iGrpNum;
        m_oUniqueName = ptr->stUniqueName;
        m_oName = ptr->stName;
        m_oCaption = ptr->stCaption;
        m_oUniqueParent = ptr->stParentUniqueName;
        m_oGroupMembers = ptr1->m_PCDHGLGMEMBERS;
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

    void CGroupMembers::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDHGLGMEMBERS*>(obj.get());
        if(!ptr)
            return;
        m_oCount = ptr->m_arPCDHGLGMEMBER.size();
        for(auto i:ptr->m_arPCDHGLGMEMBER)
            m_arrItems.push_back(new CGroupMember(i));
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

    void CGroupMember::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr1 = static_cast<XLSB::PCDHGLGMEMBER*>(obj.get());
        auto ptr = static_cast<XLSB::BeginPCDHGLGMember*>(ptr1->m_BrtBeginPCDHGLGMember.get());
        m_oGroup = (bool)ptr->fGroup;
        m_oUniqueName = ptr->stUnique;
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
                CpivotTableHierarchy* pPivotTableHierarchy = new CpivotTableHierarchy();
                *pPivotTableHierarchy = oReader;
                m_arrItems.push_back(pPivotTableHierarchy);
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
    void CpivotTableHierarchies::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXTHS*>(obj.get());
        m_oCount = (_UINT32)ptr->m_arSXTH.size();
        for(auto i:ptr->m_arSXTH)
            m_arrItems.push_back(new CpivotTableHierarchy(i));
    }
    XLS::BaseObjectPtr CpivotTableHierarchies::toBin()
    {
        auto ptr(new XLSB::SXTHS);
        XLS::BaseObjectPtr objectPtr(ptr);
        for(auto i : m_arrItems)
            ptr->m_arSXTH.push_back(i->toBin());
        return objectPtr;
    }

    void CpivotTableHierarchy::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
        ReadAttributes( oReader );
        if ( oReader.IsEmptyNode() )
                return;
        int nCurDepth = oReader.GetDepth();
        while( oReader.ReadNextSiblingNode( nCurDepth ) )
        {
            std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

            if (L"memberProperties" == sName)           m_oMemberProperties = oReader;
            else if (L"extLst" == sName)				m_oExtLst = oReader;
            else if (L"members" == sName)
            {
                CMembers* members = new CMembers();
                *members = oReader;
                m_arrItems.push_back(members) ;
            }
        }
    }
    void CpivotTableHierarchy::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"outline", m_oOutline )
           WritingElement_ReadAttributes_Read_else_if( oReader, L"multipleItemSelectionAllowed", m_oMultipleItemSelectionAllowed )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"subtotalTop", m_oSubtotalTop )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"showInFieldList", m_oShowInFieldList )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"dragToRow", m_oDragToRow  )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"dragToCol", m_oDragToCol )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"dragToPage", m_oDragToPage )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"dragToData", m_oDragToData )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"dragOff", m_oDragOff )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"includeNewItemsInFilter", m_oIncludeNewItemsInFilter )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"caption", m_oCaption )
       WritingElement_ReadAttributes_End( oReader )
    }
    void CpivotTableHierarchy::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<pivotHierarchy");
        WritingStringNullableAttrBool2(L"outline", m_oOutline);
        WritingStringNullableAttrBool2(L"multipleItemSelectionAllowed", m_oMultipleItemSelectionAllowed);
        WritingStringNullableAttrBool2(L"subtotalTop", m_oSubtotalTop);
        WritingStringNullableAttrBool2(L"showInFieldList", m_oShowInFieldList);
        WritingStringNullableAttrBool2(L"dragToRow", m_oDragToRow);
        WritingStringNullableAttrBool2(L"dragToCol", m_oDragToCol);
        WritingStringNullableAttrBool2(L"dragToPage", m_oDragToPage);
        WritingStringNullableAttrBool2(L"dragToData", m_oDragToData);
        WritingStringNullableAttrBool2(L"dragOff", m_oDragOff);
        WritingStringNullableAttrBool2(L"includeNewItemsInFilter", m_oIncludeNewItemsInFilter);
        WritingStringNullableAttrEncodeXmlString2(L"caption", m_oCaption);

        if(m_oMemberProperties.IsInit() || m_arrItems.size() > 0)
        {
             writer.WriteString(L">");
             if(m_oMemberProperties.IsInit())
                m_oMemberProperties->toXML();
             if(m_arrItems.size() > 0)
                 for(auto i:m_arrItems)
                    i->toXML();
             writer.WriteString(L"</pivotHierarchy>");
        }
        else
            writer.WriteString(L"/>");
    }
    void CpivotTableHierarchy::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr1 = static_cast<XLSB::SXTH*>(obj.get());
        auto ptr = static_cast<XLSB::BeginSXTH*>(ptr1->m_BrtBeginSXTH.get());
        if(ptr == nullptr)
            return;
        m_oOutline = ptr->fOutlineMode;
        m_oMultipleItemSelectionAllowed = ptr->fEnableMultiplePageItems;
        m_oSubtotalTop = ptr->fSubtotalAtTop;
        m_oShowInFieldList = ptr->fDontShowFList;
        m_oDragToRow = ptr->fDragToRow;
        m_oDragToCol = ptr->fDragToColumn;
        m_oDragToData = ptr->fDragToData;
        m_oDragToPage = ptr->fDragToPage;
        m_oIncludeNewItemsInFilter = ptr->fFilterInclusive;

        if(ptr->fLoadCap)
            m_oCaption = ptr->irstCaption;
        if(ptr1->m_SXTDMPS != nullptr)
            m_oMemberProperties = ptr1->m_SXTDMPS;

        for(auto i:ptr1->m_arSXTHITEMS)
            m_arrItems.push_back(new CMembers(i));

    }
    XLS::BaseObjectPtr CpivotTableHierarchy::toBin()
    {
        auto ptr1(new XLSB::SXTH);
        XLS::BaseObjectPtr objectPtr(ptr1);
        auto ptr(new XLSB::BeginSXTH);

        if(m_oOutline.IsInit())
            ptr->fOutlineMode = m_oOutline.get();
        if(m_oMultipleItemSelectionAllowed.IsInit())
            ptr->fEnableMultiplePageItems = m_oMultipleItemSelectionAllowed.get();
        if(m_oSubtotalTop.IsInit())
            ptr->fSubtotalAtTop = m_oSubtotalTop.get();
        if(m_oShowInFieldList.IsInit())
            ptr->fDontShowFList = !m_oShowInFieldList.get();
        if(m_oDragToRow.IsInit())
            ptr->fDragToRow = m_oDragToRow.get();
        if(m_oDragToCol.IsInit())
            ptr->fDragToColumn = m_oDragToCol.get();
        if(m_oDragToData.IsInit())
            ptr->fDragToData = m_oDragToData.get();
        if(m_oDragToPage.IsInit())
            ptr->fDragToPage = m_oDragToPage.get();
        if(m_oIncludeNewItemsInFilter.IsInit())
            ptr->fFilterInclusive = m_oIncludeNewItemsInFilter.get();
        if(m_oCaption.IsInit())
        {
            ptr->irstCaption = m_oCaption.get();
            ptr->fLoadCap = true;
        }

        ptr1->m_BrtBeginSXTH = XLS::BaseObjectPtr{ptr};

        for(auto i : m_arrItems)
        {
           ptr1->m_arSXTHITEMS.push_back(i->toBin());
        }

        if(m_oMemberProperties.IsInit())
            ptr1->m_SXTDMPS = m_oMemberProperties->toBin();
       return objectPtr;
    }

    void CMembers::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
       ReadAttributes( oReader );
       auto nCurDepth = oReader.GetDepth();
       while( oReader.ReadNextSiblingNode( nCurDepth ) )
       {
           std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
           if ( L"member" == sName )
           {
               CMember* pMember = new CMember();
               *pMember = oReader;
               m_arrItems.push_back(pMember);
           }
       }
    }
    void CMembers::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"count", m_oCount )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"level", m_oLevel )
       WritingElement_ReadAttributes_End( oReader )
    }
    void CMembers::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<members");
        WritingStringNullableAttrUInt(L"count", m_oCount, m_oCount.get());
        WritingStringNullableAttrUInt(L"level", m_oLevel, m_oLevel.get());
        writer.WriteString(L">");

        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
        writer.WriteString(L"</members>");
    }
    void CMembers::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXTDMPS*>(obj.get());

        auto ptr1 = static_cast<XLSB::BeginSXTHItems*>(ptr->m_BrtBeginSXTDMPs.get());
        m_oLevel = ptr1->iSXTL;
        m_oCount = (_UINT32)ptr->m_arSXTDMP.size();
        for(auto i:ptr->m_arSXTDMP)
            m_arrItems.push_back(new CMember(i));
    }
    XLS::BaseObjectPtr CMembers::toBin()
    {
       auto ptr(new XLSB::SXTHITEMS);
       XLS::BaseObjectPtr objectPtr(ptr);
       auto ptr1(new XLSB::BeginSXTHItems);
       ptr->m_BrtBeginSXTHItems = XLS::BaseObjectPtr{ptr1};
       if(m_oCount.IsInit())
           ptr1->csz = m_oCount.get();
       if(m_oLevel.IsInit())
           ptr1->iSXTL = m_oLevel.get();
       for(auto i : m_arrItems)
       {
           ptr->m_arSXTHITEM.push_back(i->toBin());
       }
       return objectPtr;
    }
    void CMember::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
       ReadAttributes( oReader );
    }
    void CMember::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"name", m_oName)
       WritingElement_ReadAttributes_End( oReader )
    }
    void CMember::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<member");
        WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
        writer.WriteString(L"/>");
    }
    void CMember::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr1 = static_cast<XLSB::SXTHITEM*>(obj.get());
        auto ptr = static_cast<XLSB::BeginSXTHItem*>(ptr1->m_BrtBeginSXTHItem.get());
        if(ptr == nullptr)
            return;
        m_oName = ptr->irstItem;
    }
    XLS::BaseObjectPtr CMember::toBin()
    {
        auto ptr1(new XLSB::SXTHITEM);
        XLS::BaseObjectPtr objectPtr(ptr1);
        auto ptr(new XLSB::BeginSXTHItem);
        ptr1->m_BrtBeginSXTHItem = XLS::BaseObjectPtr{ptr};
        if(m_oName.IsInit())
            ptr->irstItem = m_oName.get();
        return objectPtr;
    }

    void CMemberProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
       ReadAttributes( oReader );
       auto nCurDepth = oReader.GetDepth();
       while( oReader.ReadNextSiblingNode( nCurDepth ) )
       {
           std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
           if ( L"mp" == sName )
           {
               CMemberProperty* pProperty = new CMemberProperty();
               *pProperty = oReader;
               m_arrItems.push_back(pProperty);
           }
       }
    }
    void CMemberProperties::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"count", m_oCount )
       WritingElement_ReadAttributes_End( oReader )
    }
    void CMemberProperties::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<mps");
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
    void CMemberProperties::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXTDMPS*>(obj.get());
        if(!ptr)
            return;
        m_oCount = (_UINT32)ptr->m_arSXTDMP.size();
        for(auto i:ptr->m_arSXTDMP)
            m_arrItems.push_back(new CMemberProperty(i));
    }
    XLS::BaseObjectPtr CMemberProperties::toBin()
    {
       auto ptr(new XLSB::SXTDMPS);
       XLS::BaseObjectPtr objectPtr(ptr);
       for(auto i : m_arrItems)
       {
           ptr->m_arSXTDMP.push_back(i->toBin());
       }
       return objectPtr;
    }
    void CMemberProperty::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
       ReadAttributes( oReader );
    }
    void CMemberProperty::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"name", m_oName)
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"showCell", m_oShowCell )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"showTip", m_oShowTip )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"showAsCaption", m_oShowAsCaption )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"nameLen", m_oNameLen )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"pPos", m_oPPos )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"pLen", m_oPLen )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"level", m_oLevel )
           WritingElement_ReadAttributes_Read_else_if	( oReader, L"field", m_oField )
       WritingElement_ReadAttributes_End( oReader )
    }
    void CMemberProperty::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        writer.WriteString(L"<mp");
                WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
                WritingStringNullableAttrBool2(L"showCell", m_oShowCell);
                WritingStringNullableAttrBool2(L"showTip", m_oShowTip);
                WritingStringNullableAttrBool2(L"showAsCaption", m_oShowAsCaption);
                WritingStringNullableAttrUInt(L"nameLen", m_oNameLen, m_oNameLen.get());
                WritingStringNullableAttrUInt(L"pPos", m_oPPos, m_oPPos.get());
                WritingStringNullableAttrUInt(L"pLen", m_oPLen, m_oPPos.get());
                WritingStringNullableAttrUInt(L"level", m_oLevel, m_oLevel.get());
                WritingStringNullableAttrUInt(L"field", m_oField, m_oField.get());
                writer.WriteString(L"/>");
    }
    void CMemberProperty::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr1 = static_cast<XLSB::SXTDMP*>(obj.get());
        auto ptr = static_cast<XLSB::BeginSXTDMP*>(ptr1->m_BrtBeginSXTDMP.get());
        if(ptr == nullptr)
            return;
        m_oLevel = ptr->irstProperty;
        m_oNameLen = ptr->cchLevelUnq;
        m_oPLen = ptr->cchPropName;
        m_oPPos = ptr->ichPropName;
        m_oField = ptr->isxvd;
        m_oShowCell = ptr->fDisplayInReport;
    }
    XLS::BaseObjectPtr CMemberProperty::toBin()
    {
       auto ptr1(new XLSB::SXTDMP);
       XLS::BaseObjectPtr objectPtr(ptr1);
       auto ptr(new XLSB::BeginSXTDMP);
       ptr1->m_BrtBeginSXTDMP = XLS::BaseObjectPtr{ptr};

       if(m_oLevel.IsInit())
           ptr->isxtl = m_oLevel.get();
       if(m_oName.IsInit())
           ptr->irstProperty = m_oName.get();
       if(m_oNameLen.IsInit())
           ptr->cchLevelUnq = m_oNameLen.get();
       if(m_oPLen.IsInit())
           ptr->cchPropName = m_oPLen.get();
       if(m_oPPos.IsInit())
           ptr->ichPropName = m_oPPos.get();
       if(m_oField.IsInit())
           ptr->isxvd = m_oField.get();
       if(m_oShowCell.IsInit())
           ptr->fDisplayInReport = m_oShowCell.get();

       return objectPtr;
    }
///pivot hierarhies usage
    void CHierarchyUsage::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
       ReadAttributes( oReader );
    }
    void CHierarchyUsage::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"hierarchyUsage", m_oHierarchyUsage)
       WritingElement_ReadAttributes_End( oReader )
    }
    void CHierarchyUsage::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        if(m_oRowHierarchy)
            writer.WriteString(L"<rowHierarchyUsage");
        else
            writer.WriteString(L"<colHierarchyUsage");
        WritingStringAttrInt(L"hierarchyUsage", m_oHierarchyUsage.get());
        writer.WriteString(L"/>");
    }

    void CHierarchiesUsage::fromXML(XmlUtils::CXmlLiteReader& oReader)
    {
       ReadAttributes( oReader );
       auto nCurDepth = oReader.GetDepth();
       while( oReader.ReadNextSiblingNode( nCurDepth ) )
       {
           std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
           if ( L"rowHierarchyUsage" == sName )
           {
               CHierarchyUsage* pUsage = new CHierarchyUsage();
               *pUsage = oReader;
               pUsage->m_oRowHierarchy = true;
               m_arrItems.push_back(pUsage);
           }
           else if( L"colHierarchyUsage" == sName )
           {
               CHierarchyUsage* pUsage = new CHierarchyUsage();
               *pUsage = oReader;
               pUsage->m_oRowHierarchy = false;
               m_arrItems.push_back(pUsage);
           }
       }
    }
    void CHierarchiesUsage::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
    {
       WritingElement_ReadAttributes_Start( oReader )
           WritingElement_ReadAttributes_Read_if	( oReader, L"count", m_oCount )
       WritingElement_ReadAttributes_End( oReader )
    }
    void CHierarchiesUsage::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        if(m_oRowHierarchy)
            writer.WriteString(L"<rowHierarchiesUsage");
        else
            writer.WriteString(L"<colHierarchiesUsage");
        WritingStringAttrInt(L"count", (int)m_arrItems.size());
        writer.WriteString(L">");

        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
        if(m_oRowHierarchy)
            writer.WriteString(L"</rowHierarchiesUsage>");
        else
            writer.WriteString(L"</colHierarchiesUsage>");
    }
    void CHierarchiesUsage::fromBin(XLS::BaseObjectPtr& obj)
    {
        if(m_oRowHierarchy)
        {
            auto ptr1 = static_cast<XLSB::ISXTHRWS*>(obj.get());
            auto ptr = static_cast<XLSB::BeginISXTHRws*>(ptr1->m_BrtBeginISXTHRws.get());
            m_oCount = (_UINT32)ptr->rgisxth.size();
            for(auto i:ptr->rgisxth)
            {
                auto usage = new CHierarchyUsage;
                usage->m_oRowHierarchy = true;
                usage->m_oHierarchyUsage = i;
                m_arrItems.push_back(usage);
            }
        }
        else
        {
            auto ptr1 = static_cast<XLSB::ISXTHCOLS*>(obj.get());
            auto ptr = static_cast<XLSB::BeginISXTHCols*>(ptr1->m_BrtBeginISXTHCols.get());
            m_oCount = (_UINT32)ptr->rgisxth.size();
            for(auto i:ptr->rgisxth)
            {
                auto usage = new CHierarchyUsage;
                usage->m_oRowHierarchy = false;
                usage->m_oHierarchyUsage = i;
                m_arrItems.push_back(usage);
            }
        }
    }
    XLS::BaseObjectPtr CHierarchiesUsage::toBin()
    {
        XLS::BaseObjectPtr objectPtr;
        if(m_oRowHierarchy)
        {
            auto ptr1(new XLSB::ISXTHRWS);
            auto ptr(new XLSB::BeginISXTHRws);
            ptr1->m_BrtBeginISXTHRws = XLS::BaseObjectPtr{ptr};
            objectPtr = XLS::BaseObjectPtr{ptr1};
            for(auto i : m_arrItems)
            {
                if(i->m_oHierarchyUsage.IsInit())
                    ptr->rgisxth.push_back(i->m_oHierarchyUsage.get());
            }
        }
        else
        {
            auto ptr1(new XLSB::ISXTHCOLS);
            auto ptr(new XLSB::BeginISXTHCols);
            ptr1->m_BrtBeginISXTHCols = XLS::BaseObjectPtr{ptr};
            objectPtr = XLS::BaseObjectPtr{ptr1};
            for(auto i : m_arrItems)
            {
                if(i->m_oHierarchyUsage.IsInit())
                    ptr->rgisxth.push_back(i->m_oHierarchyUsage.get());
            }
        }
        return objectPtr;
    }
}
}
