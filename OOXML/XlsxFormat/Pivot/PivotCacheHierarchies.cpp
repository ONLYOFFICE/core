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
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"caption", m_oCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"attribute", m_oAttribute )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"defaultMemberUniqueName", m_oDefaultMemberUniqueName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"allUniqueName", m_oAllUniqueName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dimensionUniqueName", m_oDimensionUniqueName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"displayFolder", m_oDisplayFolder )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"count", m_oCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"memberValueDatatype", m_oMemberValueDatatype )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"unbalanced", m_oUnbalanced )
		WritingElement_ReadAttributes_End( oReader )
    }
    void CpivotCacheHierarchy::toXML(NSStringUtils::CStringBuilder& writer) const
    {
        
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
}
}
