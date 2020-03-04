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
#include "PivotTable.h"
#include "PivotCacheDefinition.h"
#include "PivotCacheRecords.h"

namespace OOX
{
namespace Spreadsheet
{
	void CPivotTableFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oPath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		m_oPivotTableDefinition = oReader;
	}
	void CPivotTableFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if(m_oPivotTableDefinition.IsInit())
		{
			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			m_oPivotTableDefinition->toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write( oPath, oDirectory, oContent );
		}
		else if(m_nDataLength > 0 && m_pData)
		{
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(oPath.GetPath());
			oFile.WriteFile(m_pData, m_nDataLength);
			oFile.CloseFile();

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write( oPath, oDirectory, oContent );
		}
	}
//------------------------------------
	void CPivotTableDefinition::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(false == m_oName.IsInit()) return;

		writer.WriteString(L"<pivotTableDefinition \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr16\" \
xmlns:xr16=\"http://schemas.microsoft.com/office/spreadsheetml/2017/revision16\"");

		WritingStringNullableAttrEncodeXmlString2(L"name",			m_oName);
		WritingStringNullableAttrBool2(L"applyAlignmentFormats",	m_oApplyAlignmentFormats);
		WritingStringNullableAttrBool2(L"applyBorderFormats",		m_oApplyBorderFormats);
		WritingStringNullableAttrBool2(L"applyFontFormats",			m_oApplyFontFormats);
		WritingStringNullableAttrBool2(L"applyNumberFormats",		m_oApplyNumberFormats);
		WritingStringNullableAttrBool2(L"applyPatternFormats",		m_oApplyPatternFormats);
		WritingStringNullableAttrBool2(L"applyWidthHeightFormats",	m_oApplyWidthHeightFormats);
		WritingStringNullableAttrBool2(L"asteriskTotals",			m_oAsteriskTotals);
		WritingStringNullableAttrInt(L"autoFormatId",				m_oAutoFormatId, m_oAutoFormatId->GetValue());
		WritingStringNullableAttrInt(L"cacheId",					m_oCacheId, m_oCacheId->GetValue());
		WritingStringNullableAttrInt(L"chartFormat",				m_oChartFormat, m_oChartFormat->GetValue());
		WritingStringNullableAttrBool2(L"colGrandTotals",			m_oColGrandTotals);
		WritingStringNullableAttrEncodeXmlString2(L"colHeaderCaption",m_oColHeaderCaption);
		WritingStringNullableAttrBool2(L"compact",					m_oCompact);
		WritingStringNullableAttrBool2(L"compactData",				m_oCompactData);
		WritingStringNullableAttrInt(L"createdVersion",				m_oCreatedVersion, m_oCreatedVersion->GetValue());
		WritingStringNullableAttrBool2(L"customListSort",			m_oCustomListSort);
		WritingStringNullableAttrEncodeXmlString2(L"dataCaption",	m_oDataCaption);
		WritingStringNullableAttrBool2(L"dataOnRows",				m_oDataOnRows);
		WritingStringNullableAttrInt(L"dataPosition",				m_oDataPosition, m_oDataPosition->GetValue());
		WritingStringNullableAttrBool2(L"disableFieldList",			m_oDisableFieldList);
		WritingStringNullableAttrBool2(L"editData",					m_oEditData);
		WritingStringNullableAttrBool2(L"enableDrill",				m_oEnableDrill);
		WritingStringNullableAttrBool2(L"enableFieldProperties",	m_oEnableFieldProperties);
		WritingStringNullableAttrBool2(L"enableWizard",				m_oEnableWizard);
		WritingStringNullableAttrEncodeXmlString2(L"errorCaption",	m_oErrorCaption);
		WritingStringNullableAttrBool2(L"disableEdit",				m_oFieldListSortAscending);
		WritingStringNullableAttrBool2(L"fieldPrintTitles",			m_oFieldPrintTitles);
		WritingStringNullableAttrEncodeXmlString2(L"grandTotalCaption",m_oGrandTotalCaption);
		WritingStringNullableAttrBool2(L"gridDropZones",			m_oGridDropZones);
		WritingStringNullableAttrBool2(L"immersive",				m_oImmersive);
		WritingStringNullableAttrInt(L"indent",						m_oIndent, m_oIndent->GetValue());
		WritingStringNullableAttrBool2(L"itemPrintTitles",			m_oItemPrintTitles);
		WritingStringNullableAttrBool2(L"mdxSubqueries",			m_oMdxSubqueries);
		WritingStringNullableAttrBool2(L"mergeItem",				m_oMergeItem);
		WritingStringNullableAttrInt(L"minRefreshableVersion",		m_oMinRefreshableVersion, m_oMinRefreshableVersion->GetValue());
		WritingStringNullableAttrEncodeXmlString2(L"missingCaption",m_oMissingCaption);
		WritingStringNullableAttrBool2(L"multipleFieldFilters",		m_oMultipleFieldFilters);
		WritingStringNullableAttrBool2(L"outline",					m_oOutline);
		WritingStringNullableAttrBool2(L"outlineDatae",				m_oOutlineDatae);
		WritingStringNullableAttrBool2(L"pageOverThenDown",			m_oPageOverThenDown);
		WritingStringNullableAttrEncodeXmlString2(L"pageStyle",		m_oPageStyle);
		WritingStringNullableAttrInt(L"pageWrap",					m_oPageWrap, m_oPageWrap->GetValue());
		WritingStringNullableAttrEncodeXmlString2(L"pivotTableStyle",m_oPivotTableStyle);
		WritingStringNullableAttrBool2(L"preserveFormatting",		m_oPreserveFormatting);
		WritingStringNullableAttrBool2(L"printDrill",				m_oPrintDrill);
		WritingStringNullableAttrBool2(L"published",				m_oPublished);
		WritingStringNullableAttrBool2(L"rowGrandTotals",			m_oRowGrandTotals);
		WritingStringNullableAttrEncodeXmlString2(L"rowHeaderCaption",m_oRowHeaderCaption);
		WritingStringNullableAttrBool2(L"showCalcMbrs",				m_oShowCalcMbrs);
		WritingStringNullableAttrBool2(L"showDataDropDown",			m_oShowDataDropDown);
		WritingStringNullableAttrBool2(L"showDataTips",				m_oShowDataTips);
		WritingStringNullableAttrBool2(L"showDrill",				m_oShowDrill);
		WritingStringNullableAttrBool2(L"showDropZones",			m_oShowDropZones);
		WritingStringNullableAttrBool2(L"showEmptyCol",				m_oShowEmptyCol);
		WritingStringNullableAttrBool2(L"showEmptyRow",				m_oShowEmptyRow);
		WritingStringNullableAttrBool2(L"showError",				m_oShowError);
		WritingStringNullableAttrBool2(L"showHeaders",				m_oShowHeaders);
		WritingStringNullableAttrBool2(L"showItems",				m_oShowItems);
		WritingStringNullableAttrBool2(L"showMemberPropertyTips",	m_oShowMemberPropertyTips);
		WritingStringNullableAttrBool2(L"showMissing",				m_oShowMissing);
		WritingStringNullableAttrBool2(L"showMultipleLabel",		m_oShowMultipleLabel);
		WritingStringNullableAttrBool2(L"subtotalHiddenItems",		m_oSubtotalHiddenItems);
		WritingStringNullableAttrEncodeXmlString2(L"tag",			m_oTag);
		WritingStringNullableAttrInt(L"updatedVersion",				m_oUpdatedVersion, m_oUpdatedVersion->GetValue());
		WritingStringNullableAttrBool2(L"useAutoFormatting",		m_oUseAutoFormatting);
		WritingStringNullableAttrEncodeXmlString2(L"vacatedStyle",	m_oVacatedStyle);
		WritingStringNullableAttrBool2(L"visualTotals",				m_oVisualTotals);
		
		writer.WriteString(L">");

		if(m_oLocation.IsInit())
			m_oLocation->toXML(writer);

		if(m_oPivotFields.IsInit())
			m_oPivotFields->toXML(writer);
		if(m_oRowFields.IsInit())
			m_oRowFields->toXML2(writer, L"rowFields");		
		if(m_oRowItems.IsInit())
			m_oRowItems->toXML2(writer, L"rowItems");
		if(m_oColFields.IsInit())
			m_oColFields->toXML2(writer, L"colFields");
		if(m_oColItems.IsInit())
			m_oColItems->toXML2(writer, L"colItems");
		if(m_oPageFields.IsInit())
			m_oPageFields->toXML(writer);
		if(m_oDataFields.IsInit())
			m_oDataFields->toXML(writer);
		
		if(m_oFormats.IsInit())
			m_oFormats->toXML(writer);
		if(m_oPivotTableStyleInfo.IsInit())
			m_oPivotTableStyleInfo->toXML(writer);

		//if(m_oConditionalFormats.IsInit())
		//	m_oConditionalFormats->toXML(writer);
		//if(m_oChartFormats.IsInit())
		//	m_oChartFormats->toXML(writer);
		//if(m_oFilters.IsInit())
		//	m_oFilters->toXML(writer);

		//if(m_oColHierarchiesUsage.IsInit())
		//	m_oColHierarchiesUsage->toXML(writer);
		//if(m_oPivotHierarchies.IsInit())
		//	m_oPivotHierarchies->toXML(writer);
		//if(m_oRowHierarchiesUsage.IsInit())
		//	m_oRowHierarchiesUsage->toXML(writer);
			
		if(m_oExtLst.IsInit())
		{
			writer.WriteString(m_oExtLst->toXMLWithNS(_T("")));
		}

		writer.WriteString(L"</pivotTableDefinition>");
	}
	void CPivotTableDefinition::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				 if (L"colFields" == sName)				m_oColFields = oReader;
			else if (L"colItems" == sName)				m_oColItems = oReader;
			else if (L"dataFields" == sName)			m_oDataFields = oReader;
			else if (L"formats" == sName)				m_oFormats = oReader;
			else if (L"location" == sName)				m_oLocation = oReader;
			else if (L"pageFields" == sName)			m_oPageFields = oReader;
			else if (L"pivotFields" == sName)			m_oPivotFields = oReader;
			else if (L"pivotTableStyleInfo" == sName)	m_oPivotTableStyleInfo = oReader;
			else if (L"rowFields" == sName)				m_oRowFields = oReader;
			else if (L"rowItems" == sName)				m_oRowItems = oReader;
			//else if (L"filters" == sName)				m_oFilters = oReader;
			//else if (L"chartFormats" == sName)		m_oChartFormats = oReader;
			//else if (L"colHierarchiesUsage" == sName)	m_oColHierarchiesUsage = oReader;
			//else if (L"conditionalFormats" == sName)	m_oConditionalFormats = oReader;
			//else if (L"pivotHierarchies" == sName)	m_oPivotHierarchies = oReader;
			//else if (L"rowHierarchiesUsage" == sName)	m_oRowHierarchiesUsage = oReader;
			else if (L"extLst" == sName)				m_oExtLst = oReader;
		}
	}
	void CPivotTableDefinition::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"applyAlignmentFormats",	m_oApplyAlignmentFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyBorderFormats",	m_oApplyBorderFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyFontFormats",	m_oApplyFontFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyNumberFormats",	m_oApplyNumberFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyPatternFormats",	m_oApplyPatternFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"applyWidthHeightFormats",	m_oApplyWidthHeightFormats )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"asteriskTotals",	m_oAsteriskTotals )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"autoFormatId",	m_oAutoFormatId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"cacheId",	m_oCacheId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"chartFormat",	m_oChartFormat )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"colGrandTotals",	m_oColGrandTotals )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"colHeaderCaption",	m_oColHeaderCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"compact",	m_oCompact )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"compactData",	m_oCompactData )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"createdVersion",	m_oCreatedVersion )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"customListSort",	m_oCustomListSort )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dataCaption",	m_oDataCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dataOnRows",	m_oDataOnRows )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dataPosition",	m_oDataPosition )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"disableFieldList",	m_oDisableFieldList )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"editData",	m_oEditData )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"enableDrill",	m_oEnableDrill )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"enableFieldProperties",	m_oEnableFieldProperties )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"enableWizard",	m_oEnableWizard )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"errorCaption",	m_oErrorCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"fieldListSortAscending",	m_oFieldListSortAscending )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"fieldPrintTitles",	m_oFieldPrintTitles )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"grandTotalCaption",	m_oGrandTotalCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"gridDropZones",	m_oGridDropZones )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"immersive",	m_oImmersive )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"indent",	m_oIndent )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"itemPrintTitles",	m_oItemPrintTitles )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"mdxSubqueries",	m_oMdxSubqueries )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"mergeItem",	m_oMergeItem )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"minRefreshableVersion",	m_oMinRefreshableVersion )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"missingCaption",	m_oMissingCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"multipleFieldFilters",	m_oMultipleFieldFilters )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"name",	m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"outline",	m_oOutline )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"outlineDatae",	m_oOutlineDatae )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"pageOverThenDown",	m_oPageOverThenDown )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"pageStyle",	m_oPageStyle )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"pageWrap",	m_oPageWrap )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"pivotTableStyle",	m_oPivotTableStyle )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"preserveFormatting",	m_oPreserveFormatting )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"printDrill",	m_oPrintDrill )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"published",	m_oPublished )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"rowGrandTotals",	m_oRowGrandTotals )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"rowHeaderCaption",	m_oRowHeaderCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showCalcMbrs",	m_oShowCalcMbrs )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showDataDropDown",	m_oShowDataDropDown )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showDataTips",	m_oShowDataTips )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showDrill",	m_oShowDrill )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showDropZones",	m_oShowDropZones )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showEmptyCol",	m_oShowEmptyCol )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showEmptyRow",	m_oShowEmptyRow )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showError",	m_oShowError )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showHeaders",	m_oShowHeaders )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showItems",	m_oShowItems )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showMemberPropertyTips",	m_oShowMemberPropertyTips )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showMissing",	m_oShowMissing )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showMultipleLabel",	m_oShowMultipleLabel )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"subtotalHiddenItems",	m_oSubtotalHiddenItems )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"tag",	m_oTag )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"updatedVersion",	m_oUpdatedVersion )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"useAutoFormatting",	m_oUseAutoFormatting )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"vacatedStyle",	m_oVacatedStyle )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"visualTotals",	m_oVisualTotals )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CColumnRowFields::toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<" + sName);
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</" + sName + L">");
	}
	void CColumnRowFields::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"field" == sName )
				m_arrItems.push_back(new CField(oReader));
		}
	}
	void CColumnRowFields::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CColumnRowItems::toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<" + sName);
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</" + sName + L">");
	}
	void CColumnRowItems::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"i" == sName )
				m_arrItems.push_back(new CColumnRowItem(oReader));
		}
	}
	void CColumnRowItems::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CColumnRowItem::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<i");
			WritingStringNullableAttrInt(L"i", m_oI, m_oI->GetValue());
			WritingStringNullableAttrInt(L"r", m_oR, m_oR->GetValue());
			WritingStringNullableAttrString(L"t", m_oT, m_oT->ToString());
		writer.WriteString(L">");
        
		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</i>");
	}
	void CColumnRowItem::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"x" == sName)
				m_arrItems.push_back(new CSharedItemsIndex(oReader));
		}
	}
	void CColumnRowItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"i", m_oI )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"r", m_oR )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"t", m_oT )
		WritingElement_ReadAttributes_End( oReader )
	}

//------------------------------------
	void CDataFields::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<dataFields");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</dataFields>");
	}
	void CDataFields::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"dataField" == sName )
				m_arrItems.push_back(new CDataField(oReader));
		}
	}
	void CDataFields::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CDataField::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<dataField");
			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrInt2(L"baseField", m_oBaseField);
			WritingStringNullableAttrInt(L"baseItem", m_oBaseItem, m_oBaseItem->GetValue());
			WritingStringNullableAttrInt(L"fld", m_oFld, m_oFld->GetValue());
			WritingStringNullableAttrInt(L"numFmtId", m_oNumFmtId, m_oNumFmtId->GetValue());
			WritingStringNullableAttrString(L"showDataAs", m_oShowDataAs, m_oShowDataAs->ToString());
			WritingStringNullableAttrString(L"subtotal", m_oSubtotal, m_oSubtotal->ToString());
		writer.WriteString(L"/>");
	}
	void CDataField::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"extLst" == sName)
				m_oExtLst = oReader;
		}
	}
	void CDataField::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"baseField", m_oBaseField )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"baseItem", m_oBaseItem )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"fld", m_oFld )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"name", m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"numFmtId", m_oNumFmtId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showDataAs", m_oShowDataAs )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"subtotal", m_oSubtotal )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPageFields::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<pageFields");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</pageFields>");
	}
	void CPageFields::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"pageField" == sName )
				m_arrItems.push_back(new CPageField(oReader));
		}
	}
	void CPageFields::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPageField::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<dataField");
			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrEncodeXmlString2(L"cap", m_oCap);
			WritingStringNullableAttrInt2(L"fld", m_oFld);
			WritingStringNullableAttrInt2(L"hier", m_oHier);
			WritingStringNullableAttrInt(L"item", m_oItem, m_oItem->GetValue());
		writer.WriteString(L"/>");
	}
	void CPageField::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"extLst" == sName)
				m_oExtLst = oReader;
		}
	}
	void CPageField::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"name", m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"cap", m_oCap )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"fld", m_oFld )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"item", m_oItem )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"hier", m_oHier )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CFieldItems::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<items");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</items>");
	}
	void CFieldItems::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"item" == sName )
				m_arrItems.push_back(new CFieldItem(oReader));
		}
	}
	void CFieldItems::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, _T("count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CFieldItem::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<item");
			WritingStringNullableAttrBool2(L"c",	m_oChild);
			WritingStringNullableAttrBool2(L"d",	m_oExpanded);
			WritingStringNullableAttrBool2(L"e",	m_oDrillAcross);
			WritingStringNullableAttrBool2(L"f",	m_oCalculated);
			WritingStringNullableAttrBool2(L"h",	m_oHidden);
			WritingStringNullableAttrBool2(L"m",	m_oMissing);
			WritingStringNullableAttrBool2(L"c",	m_oCharacter);
			WritingStringNullableAttrBool2(L"sd",	m_oHideDetails);
			WritingStringNullableAttrEncodeXmlString2(L"n",	m_oUserCaption);
			WritingStringNullableAttrString(L"x",	m_oItemIndex, m_oItemIndex->ToString());
			WritingStringNullableAttrString(L"t",	m_oItemType, m_oItemType->ToString())
		writer.WriteString(L"/>");
	}
	void CFieldItem::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
	}
	void CFieldItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"c", m_oChild )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"d", m_oExpanded )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"e", m_oDrillAcross )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"f", m_oCalculated )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"h", m_oHidden )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"m", m_oMissing )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"n", m_oUserCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"c", m_oCharacter )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"sd", m_oHideDetails )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"t", m_oItemType )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"x", m_oItemIndex )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotFields::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<pivotFields");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</pivotFields>");
	}
	void CPivotFields::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"pivotField" == sName )
				m_arrItems.push_back(new CPivotField(oReader));
		}
	}
	void CPivotFields::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, _T("count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotField::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<pivotField");
			WritingStringNullableAttrBool2(L"allDrilled",		m_oAllDrilled);
			WritingStringNullableAttrBool2(L"autoShow",			m_oAutoShow);
			WritingStringNullableAttrBool2(L"avgSubtotal",		m_oAvgSubtotal);
			WritingStringNullableAttrBool2(L"compact",			m_oCompact);
			WritingStringNullableAttrBool2(L"countASubtotal",	m_oCountASubtotal);
			WritingStringNullableAttrBool2(L"countSubtotal",	m_oCountSubtotal);
			WritingStringNullableAttrBool2(L"dataField",		m_oDataField);
			WritingStringNullableAttrBool2(L"dataSourceSort",	m_oDataSourceSort);
			WritingStringNullableAttrBool2(L"defaultAttributeDrillState",	m_oDefaultAttributeDrillState);
			WritingStringNullableAttrBool2(L"defaultSubtotal",	m_oDefaultSubtotal);
			WritingStringNullableAttrBool2(L"dragOff",			m_oDragOff);
			WritingStringNullableAttrBool2(L"dragToCol",		m_oDragToCol);
			WritingStringNullableAttrBool2(L"dragToData",		m_oDragToData);
			WritingStringNullableAttrBool2(L"dragToPage",		m_oDragToPage);
			WritingStringNullableAttrBool2(L"dragToRow",		m_oDragToRow);
			WritingStringNullableAttrBool2(L"hiddenLevel",		m_oHiddenLevel);
			WritingStringNullableAttrBool2(L"hideNewItems",		m_oHideNewItems);
			WritingStringNullableAttrBool2(L"includeNewItemsInFilter",	m_oIncludeNewItemsInFilter);
			WritingStringNullableAttrBool2(L"insertBlankRow",	m_oInsertBlankRow);
			WritingStringNullableAttrBool2(L"insertPageBreak",	m_oInsertPageBreak);
			WritingStringNullableAttrBool2(L"maxSubtotal",		m_oMaxSubtotal);
			WritingStringNullableAttrBool2(L"measureFilter",	m_oMeasureFilter);
			WritingStringNullableAttrBool2(L"minSubtotal",		m_oMinSubtotal);
			WritingStringNullableAttrBool2(L"multipleItemSelectionAllowed",	m_oMultipleItemSelectionAllowed);
			WritingStringNullableAttrBool2(L"nonAutoSortDefault",m_oNonAutoSortDefault);
			WritingStringNullableAttrBool2(L"outline",			m_oOutline);
			WritingStringNullableAttrBool2(L"productSubtotal",	m_oProductSubtotal);
			WritingStringNullableAttrBool2(L"serverField",		m_oServerField);
			WritingStringNullableAttrBool2(L"showAll",			m_oShowAll);
			WritingStringNullableAttrBool2(L"showDropDowns",	m_oShowDropDowns);
			WritingStringNullableAttrBool2(L"showPropAsCaption",m_oShowPropAsCaption);
			WritingStringNullableAttrBool2(L"showPropCell",		m_oShowPropCell);
			WritingStringNullableAttrBool2(L"showPropTip",		m_oShowPropTip);
			WritingStringNullableAttrBool2(L"stdDevPSubtotal",	m_oStdDevPSubtotal);
			WritingStringNullableAttrBool2(L"stdDevSubtotal",	m_oStdDevSubtotal);
			WritingStringNullableAttrBool2(L"subtotalTop",		m_oSubtotalTop);
			WritingStringNullableAttrBool2(L"sumSubtotal",		m_oSumSubtotal);
			WritingStringNullableAttrBool2(L"topAutoShow",		m_oTopAutoShow);
			WritingStringNullableAttrBool2(L"varPSubtotal",		m_oVarPSubtotal);
			WritingStringNullableAttrBool2(L"varSubtotal",		m_oVarSubtotal);
			WritingStringNullableAttrEncodeXmlString2(L"name",			m_oName);
			WritingStringNullableAttrEncodeXmlString2(L"subtotalCaption",m_oSubtotalCaption);
			WritingStringNullableAttrEncodeXmlString2(L"uniqueMemberProperty",	m_oUniqueMemberProperty);
			WritingStringNullableAttrString(L"axis",		m_oAxis, m_oAxis->ToString())
			WritingStringNullableAttrString(L"sortType",	m_oSortType, m_oSortType->ToString())
			WritingStringNullableAttrString(L"rankBy",		m_oRankBy, m_oRankBy->ToString());
			WritingStringNullableAttrString(L"itemPageCount",m_oItemPageCount, m_oItemPageCount->ToString());
			WritingStringNullableAttrString(L"numFmtId",	m_oNumFmtId, m_oNumFmtId->ToString());
		writer.WriteString(L">");
		
		if(m_oAutoSortScope.IsInit())
			m_oAutoSortScope->toXML(writer);

		if(m_oItems.IsInit())
			m_oItems->toXML(writer);

		writer.WriteString(L"</pivotField>");
	}
	void CPivotField::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"autoSortScope" == sName )
				m_oAutoSortScope = oReader;
			else if ( L"items" == sName )
				m_oItems = oReader;
			else if ( L"extLst" == sName )
				m_oExtLst = oReader;
		}
	}
	void CPivotField::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"allDrilled", m_oAllDrilled )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"autoShow", m_oAutoShow )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"avgSubtotal", m_oAvgSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"axis", m_oAxis )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"compact", m_oCompact )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"countASubtotal", m_oCountASubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"countSubtotal", m_oCountSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dataField", m_oDataField )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"defaultAttributeDrillState", m_oDefaultAttributeDrillState )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"defaultSubtotal", m_oDefaultSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dragOff", m_oDragOff )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dragToCol", m_oDragToCol )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dragToData", m_oDragToData )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dragToPage", m_oDragToPage )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dragToRow", m_oDragToRow )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"hiddenLevel", m_oHiddenLevel )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"hideNewItems", m_oHideNewItems )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"includeNewItemsInFilter", m_oIncludeNewItemsInFilter )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"insertBlankRow", m_oInsertBlankRow )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"insertPageBreak", m_oInsertPageBreak )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"itemPageCount", m_oItemPageCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"maxSubtotal", m_oMaxSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"measureFilter", m_oMeasureFilter )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"minSubtotal", m_oMinSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"multipleItemSelectionAllowed", m_oMultipleItemSelectionAllowed )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"name", m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"nonAutoSortDefault", m_oNonAutoSortDefault )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"numFmtId", m_oNumFmtId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"outline", m_oOutline )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"productSubtotal", m_oProductSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"rankBy", m_oRankBy )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"serverField", m_oServerField )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showAll", m_oShowAll )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showDropDowns", m_oShowDropDowns )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showPropAsCaption", m_oShowPropAsCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showPropCell", m_oShowPropCell )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showPropTip", m_oShowPropTip )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"sortType", m_oSortType )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"stdDevPSubtotal", m_oStdDevPSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"stdDevSubtotal", m_oStdDevSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"subtotalCaption", m_oSubtotalCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"subtotalTop", m_oSubtotalTop )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"topAutoShow", m_oTopAutoShow )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"uniqueMemberProperty", m_oUniqueMemberProperty )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"varPSubtotal", m_oVarPSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"varSubtotal", m_oVarSubtotal )
		WritingElement_ReadAttributes_End( oReader )
	}		

//------------------------------------
	void CReferences::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<references");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</references>");
	}
	void CReferences::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"reference" == sName )
				m_arrItems.push_back(new CReference(oReader));
		}
	}
	void CReferences::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, _T("count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CReference::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<reference");
			WritingStringNullableAttrString(L"field", m_oField, m_oField->ToString());
			WritingStringNullableAttrString(L"count", m_oCount, m_oCount->ToString());
			WritingStringNullableAttrBool2(L"avgSubtotal",		m_oAvgSubtotal);
			WritingStringNullableAttrBool2(L"byPosition",		m_oByPosition);
			WritingStringNullableAttrBool2(L"countASubtotal",	m_oCountASubtotal);
			WritingStringNullableAttrBool2(L"defaultSubtotal",	m_oDefaultSubtotal);
			WritingStringNullableAttrBool2(L"maxSubtotal",		m_oMaxSubtotal);
			WritingStringNullableAttrBool2(L"minSubtotal",		m_oMinSubtotal);
			WritingStringNullableAttrBool2(L"productSubtotal",	m_oProductSubtotal);
			WritingStringNullableAttrBool2(L"relative",			m_oRelative);
			WritingStringNullableAttrBool2(L"selected",			m_oSelected);
			WritingStringNullableAttrBool2(L"stdDevPSubtotal",	m_oStdDevPSubtotal);
			WritingStringNullableAttrBool2(L"stdDevSubtotal",	m_oStdDevSubtotal);
			WritingStringNullableAttrBool2(L"sumSubtotal",		m_oSumSubtotal);
			WritingStringNullableAttrBool2(L"varPSubtotal",		m_oVarPSubtotal);
			WritingStringNullableAttrBool2(L"varSubtotal",		m_oVarSubtotal);
		writer.WriteString(L">");
	
		if(m_oX.IsInit())
		{
			m_oX->toXML(writer);
		}

		writer.WriteString(L"</reference>");
	}
	void CReference::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"x" == sName )
				m_oX = oReader;
			else if ( L"extLst" == sName )
				m_oExtLst = oReader;
		}
	}
	void CReference::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"avgSubtotal", m_oAvgSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"byPosition", m_oByPosition )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"countASubtotal", m_oCountASubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"countSubtotal", m_oCountSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"defaultSubtotal", m_oDefaultSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"maxSubtotal", m_oMaxSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"minSubtotal", m_oMinSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"relative", m_oRelative )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"selected", m_oSelected )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"stdDevPSubtotal", m_oStdDevPSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"stdDevSubtotal", m_oStdDevSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"sumSubtotal", m_oSumSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"varPSubtotal", m_oVarPSubtotal )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"field", m_oField )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"count", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotTableFormats::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<formats");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</formats>");
	}
	void CPivotTableFormats::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"format" == sName )
				m_arrItems.push_back(new CPivotTableFormat(oReader));
		}
	}
	void CPivotTableFormats::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, _T("count"), m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotTableFormat::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<pivotTableFormat");
			WritingStringNullableAttrString(L"action", m_oAction, m_oAction->ToString());
			WritingStringNullableAttrString(L"dxfId", m_oDxfId, m_oDxfId->ToString());
		writer.WriteString(L">");
	
		if(m_oPivotArea.IsInit())
		{
			m_oPivotArea->toXML(writer);
		}

		writer.WriteString(L"</pivotTableFormat>");
	}
	void CPivotTableFormat::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"pivotArea" == sName )
				m_oPivotArea = oReader;
			else if ( L"extLst" == sName )
				m_oExtLst = oReader;
		}
	}			
	void CPivotTableFormat::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"action", m_oAction )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dxfId", m_oDxfId )
		WritingElement_ReadAttributes_End( oReader )
	}		
//------------------------------------
	void CAutoSortScope::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<autoSortScope>");
	
		if(m_oPivotArea.IsInit())
		{
			m_oPivotArea->toXML(writer);
		}

		writer.WriteString(L"</autoSortScope>");
	}
	void CAutoSortScope::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"pivotArea" == sName )
				m_oPivotArea = oReader;
		}
	}
//------------------------------------
	void CPivotArea::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<pivotArea");
			WritingStringNullableAttrString(L"type", m_oType, m_oType->ToString());
			WritingStringNullableAttrString(L"axis", m_oAxis, m_oAxis->ToString());
			WritingStringNullableAttrInt(L"fieldPosition", m_oFieldPosition, m_oFieldPosition->GetValue());
			WritingStringNullableAttrBool2(L"cacheIndex",		m_oCacheIndex);
			WritingStringNullableAttrBool2(L"collapsedLevelsAreSubtotals",	m_oCollapsedLevelsAreSubtotals);
			WritingStringNullableAttrBool2(L"dataOnly",	m_oDataOnly);
			WritingStringNullableAttrInt2(L"field",	m_oField);
			WritingStringNullableAttrBool2(L"grandCol",	m_oGrandCol);
			WritingStringNullableAttrBool2(L"grandRow",	m_oGrandRow);
			WritingStringNullableAttrBool2(L"labelOnly",	m_oLabelOnly);
			WritingStringNullableAttrBool2(L"outline",	m_oOutline);
			WritingStringNullableAttrString(L"offsetRef", m_oOffsetRef, *m_oOffsetRef);
		writer.WriteString(L">");
	
		if(m_oReferences.IsInit())
		{
			m_oReferences->toXML(writer);
		}
		writer.WriteString(L"</pivotArea>");
	}
	void CPivotArea::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"references" == sName )
				m_oReferences = oReader;
			else if ( L"extLst" == sName )
				m_oExtLst = oReader;
		}
	}			
	void CPivotArea::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"axis", m_oAxis )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"cacheIndex", m_oCacheIndex )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"collapsedLevelsAreSubtotals", m_oCollapsedLevelsAreSubtotals )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"dataOnly", m_oDataOnly )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"field", m_oField )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"fieldPosition", m_oFieldPosition )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"grandCol", m_oGrandCol )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"grandRow", m_oGrandRow )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"labelOnly", m_oLabelOnly )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"offsetRef", m_oOffsetRef )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"outline", m_oOutline )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"type", m_oType )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotTableLocation::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<pivotArea");
			WritingStringNullableAttrString(L"colPageCount", m_oColPageCount, m_oColPageCount->ToString());
			WritingStringNullableAttrString(L"firstDataCol", m_oFirstDataCol, m_oFirstDataCol->ToString());
			WritingStringNullableAttrString(L"firstDataRow", m_oFirstDataRow, m_oFirstDataRow->ToString());
			WritingStringNullableAttrString(L"firstHeaderRow", m_oFirstHeaderRow, m_oFirstHeaderRow->ToString());
			WritingStringNullableAttrString(L"ref", m_oRef, *m_oRef);
			WritingStringNullableAttrInt(L"rowPageCount", m_oRowPageCount, m_oRowPageCount->GetValue());
		writer.WriteString(L"/>");
	}
	
	void CPivotTableLocation::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"colPageCount", m_oColPageCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"firstDataCol", m_oFirstDataCol )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"firstDataRow", m_oFirstDataRow )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"firstHeaderRow", m_oFirstHeaderRow )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"rowPageCount", m_oRowPageCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"ref", m_oRef )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotTableStyleInfo::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<pivotArea");
			WritingStringNullableAttrEncodeXmlString2(L"mame",	m_oName);
			WritingStringNullableAttrBool2(L"showColHeaders",	m_oShowColHeaders);
			WritingStringNullableAttrBool2(L"showColStripes",	m_oShowColStripes);
			WritingStringNullableAttrBool2(L"showLastColumn",	m_oShowLastColumn);
			WritingStringNullableAttrBool2(L"showRowHeaders",	m_oShowRowHeaders);
			WritingStringNullableAttrBool2(L"showRowStripes",	m_oShowRowStripes);
	}
	
	void CPivotTableStyleInfo::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"name", m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showColHeaders", m_oShowColHeaders )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showLastColumn", m_oShowLastColumn )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showLastColumn", m_oShowLastColumn )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showRowHeaders", m_oShowRowHeaders )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"showRowStripes", m_oShowRowStripes )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotCacheDefinitionFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oPath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		m_oPivotCashDefinition = oReader;
	}
	void CPivotCacheDefinitionFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if(m_oPivotCashDefinition.IsInit())
		{
			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			m_oPivotCashDefinition->toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write( oPath, oDirectory, oContent );
		}
		else if(m_nDataLength > 0 && m_pData)
		{
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(oPath.GetPath());
			oFile.WriteFile(m_pData, m_nDataLength);
			oFile.CloseFile();

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write( oPath, oDirectory, oContent );
		}
	}
//------------------------------------
	void CPivotCacheDefinition::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<pivotCacheDefinition \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr16\" \
xmlns:xr16=\"http://schemas.microsoft.com/office/spreadsheetml/2017/revision16\"");
		WritingStringNullableAttrString(L"r:id",			m_oRid, m_oRid->ToString());
		WritingStringNullableAttrBool2(L"backgroundQuery",	m_oBackgroundQuery);
		WritingStringNullableAttrBool2(L"enableRefresh",	m_oEnableRefresh);
		WritingStringNullableAttrBool2(L"invalid",			m_oInvalid);
		WritingStringNullableAttrInt(L"createdVersion",		m_oCreatedVersion, m_oCreatedVersion->GetValue());
		WritingStringNullableAttrInt(L"minRefreshableVersion", m_oMinRefreshableVersion, m_oMinRefreshableVersion->GetValue());
		WritingStringNullableAttrInt(L"missingItemsLimit",	m_oMissingItemsLimit, m_oMissingItemsLimit->GetValue());
		WritingStringNullableAttrBool2(L"optimizeMemory",	m_oOptimizeMemory);
		WritingStringNullableAttrInt(L"refreshedVersion",	m_oRefreshedVersion, m_oRefreshedVersion->GetValue());
		WritingStringNullableAttrInt(L"recordCount",		m_oRecordCount, m_oRecordCount->GetValue());
		WritingStringNullableAttrString(L"refreshedDateIso", m_oRefreshedDateIso, m_oRefreshedDateIso->ToString());
		WritingStringNullableAttrBool2(L"refreshOnLoad",	m_oRefreshOnLoad);
		WritingStringNullableAttrBool2(L"saveData",			m_oSaveData);
		WritingStringNullableAttrBool2(L"supportAdvancedDrill",m_oSupportAdvancedDrill);
		WritingStringNullableAttrEncodeXmlString2(L"refreshedBy",m_oRefreshedBy);
		WritingStringNullableAttrBool2(L"supportAdvancedDrill",	m_oSupportAdvancedDrill);
		WritingStringNullableAttrBool2(L"supportSubquery",	m_oSupportSubquery);
		WritingStringNullableAttrBool2(L"tupleCache",		m_oTupleCache);
		WritingStringNullableAttrBool2(L"upgradeOnRefresh",	m_oUpgradeOnRefresh);
		writer.WriteString(L">");

		if(m_oCacheSource.IsInit())
			m_oCacheSource->toXML(writer);
		if(m_oCacheFields.IsInit())
			m_oCacheFields->toXML(writer);		
			
		if(m_oExtLst.IsInit())
		{
			writer.WriteString(m_oExtLst->toXMLWithNS(_T("")));
		}

		writer.WriteString(L"</pivotCacheDefinition>");
	}
	void CPivotCacheDefinition::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				 if (L"cacheFields" == sName)	m_oCacheFields = oReader;
			else if (L"cacheSource" == sName)	m_oCacheSource = oReader;
			else if (L"extLst" == sName)		m_oExtLst = oReader;
		}
	}
	void CPivotCacheDefinition::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"backgroundQuery",	m_oBackgroundQuery )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"enableRefresh",	m_oEnableRefresh )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"id",				m_oRid )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"invalid",			m_oEnableRefresh )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"createdVersion",	m_oCreatedVersion )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"refreshableVersion",m_oMinRefreshableVersion )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"missingItemsLimit",m_oMissingItemsLimit )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"optimizeMemory",	m_oOptimizeMemory )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"recordCount",		m_oRecordCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"refreshedBy",		m_oRefreshedBy )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"refreshedDateIso",	m_oRefreshedDateIso )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"refreshedVersion",	m_oRefreshedVersion )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"refreshOnLoad",	m_oRefreshOnLoad )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"saveData",			m_oSaveData )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"supportAdvancedDrill",	m_oSupportAdvancedDrill )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"supportSubquery",	m_oSupportSubquery )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"tupleCache",		m_oTupleCache )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"upgradeOnRefresh",	m_oUpgradeOnRefresh )
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
//------------------------------------
	void CPivotCacheFields::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		if(m_arrItems.empty()) return;

		writer.WriteString(L"<cacheFields");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
		
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</cacheFields>");
	}
	void CPivotCacheFields::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"cacheFields" == sName )
				m_arrItems.push_back(new CPivotCacheField(oReader));
		}
	}
	void CPivotCacheFields::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, L"count", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotCacheField::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cacheField");
			WritingStringNullableAttrEncodeXmlString2(L"name",	m_oName);
			WritingStringNullableAttrEncodeXmlString2(L"caption",m_oCaption);
			WritingStringNullableAttrBool2(L"databaseField",	m_oDatabaseField);
			WritingStringNullableAttrBool2(L"serverField",	m_oServerField);
			WritingStringNullableAttrEncodeXmlString2(L"pPropertyName",m_oPropertyName);
			WritingStringNullableAttrEncodeXmlString2(L"formula",m_oFormula);
			WritingStringNullableAttrInt2(L"hierarchy",	m_oHierarchy);
			WritingStringNullableAttrInt2(L"sqlType",	m_oSqlType);
			WritingStringNullableAttrBool2(L"memberPropertyField",	m_oMemberPropertyField);
			WritingStringNullableAttrBool2(L"uniqueList",	m_oUniqueList);
			WritingStringNullableAttrInt(L"level", m_oLevel, m_oLevel->GetValue());
			WritingStringNullableAttrInt(L"mappingCount", m_oMappingCount, m_oMappingCount->GetValue());
			WritingStringNullableAttrInt(L"numFmtId", m_oNumFmtId, m_oNumFmtId->GetValue());
		writer.WriteString(L">");
		
		if(m_oSharedItems.IsInit())
		{
			m_oSharedItems->toXML(writer);
		}
		if(m_oFieldGroup.IsInit())
		{
			m_oFieldGroup->toXML(writer);
		}
		//if(m_oMpMap.IsInit())
		//{
		//	m_oMpMap->toXML(writer);
		//}
		writer.WriteString(L"</cacheField>");
	}
	void CPivotCacheField::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"sharedItems" == sName )
				m_oSharedItems = oReader;
			else if ( L"fieldGroup" == sName )
				m_oFieldGroup = oReader;
			//else if ( L"mpMap" == sName )
			//	m_oMpMap = oReader;
			else if ( L"extLst" == sName )
				m_oExtLst = oReader;
		}
	}
	void CPivotCacheField::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"name", m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"caption", m_oCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"databaseField", m_oDatabaseField )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"serverField", m_oServerField )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"formula", m_oFormula )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"hierarchy", m_oHierarchy )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"memberPropertyField", m_oMemberPropertyField )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"propertyName", m_oPropertyName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"sqlType", m_oSqlType )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"uniqueList", m_oUniqueList )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"level", m_oLevel )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"mappingCount", m_oMappingCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"uniqueList", m_oNumFmtId )
		WritingElement_ReadAttributes_End( oReader )
	}		
//------------------------------------
	void CSharedItems::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<sharedItems");
			WritingStringAttrInt(L"count", (int)m_arrItems.size());
			WritingStringNullableAttrBool2(L"containsBlank",	m_oContainsBlank);
			WritingStringNullableAttrBool2(L"containsDate",	m_oContainsDate);
			WritingStringNullableAttrBool2(L"containsInteger",	m_oContainsInteger);
			WritingStringNullableAttrBool2(L"containsMixedTypes",	m_oContainsMixedTypes);
			WritingStringNullableAttrBool2(L"containsNonDate",	m_oContainsNonDate);
			WritingStringNullableAttrBool2(L"containsNumber",	m_oContainsNumber);
			WritingStringNullableAttrBool2(L"containsSemiMixedTypes",	m_oContainsSemiMixedTypes);
			WritingStringNullableAttrBool2(L"containsString",	m_oContainsString);
			WritingStringNullableAttrBool2(L"longText",	m_oLongText);
			WritingStringNullableAttrDouble2(L"minValue",	m_oMinValue);
			WritingStringNullableAttrDouble2(L"maxValue",	m_oMaxValue);
			WritingStringNullableAttrString(L"minDate", m_oMinDate, m_oMinDate->ToString());
			WritingStringNullableAttrString(L"maxDate", m_oMaxDate, m_oMaxDate->ToString());
		writer.WriteString(L">");

		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</sharedItems>");
	}
	void CSharedItems::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"b" == sName )
				m_arrItems.push_back(new CPivotBooleanValue(oReader));
			else if ( L"d" == sName )
				m_arrItems.push_back(new CPivotDateTimeValue(oReader));
			else if ( L"e" == sName )
				m_arrItems.push_back(new CPivotErrorValue(oReader));
			else if ( L"m" == sName )
				m_arrItems.push_back(new CPivotNoValue(oReader));
			else if ( L"n" == sName )
				m_arrItems.push_back(new CPivotNumericValue(oReader));
			else if ( L"s" == sName )
				m_arrItems.push_back(new CPivotCharacterValue(oReader));
		}
	}
	void CSharedItems::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"containsBlank", m_oContainsBlank )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"containsDate", m_oContainsDate )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"containsInteger", m_oContainsInteger )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"containsMixedTypes", m_oContainsMixedTypes )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"containsNonDate", m_oContainsNonDate )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"containsNumber", m_oContainsNumber )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"containsSemiMixedTypes", m_oContainsSemiMixedTypes )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"containsString", m_oContainsString )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"longText", m_oLongText )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"minValue", m_oMinValue )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"maxValue", m_oMaxValue )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"minDate", m_oMinDate )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"maxDate", m_oMaxDate )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"count", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CDiscreteGroupingProperties::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<discretePr");
			WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
        
		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</discretePr>");
	}
	void CDiscreteGroupingProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"x" == sName)
				m_arrItems.push_back(new CSharedItemsIndex(oReader));
		}
	}
	void CDiscreteGroupingProperties::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, L"count", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void COLAPGroupItems::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<groupItems");
			WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
        
		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</groupItems>");
	}
	void COLAPGroupItems::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"b" == sName )
				m_arrItems.push_back(new CPivotBooleanValue(oReader));
			else if ( L"d" == sName )
				m_arrItems.push_back(new CPivotDateTimeValue(oReader));
			else if ( L"e" == sName )
				m_arrItems.push_back(new CPivotErrorValue(oReader));
			else if ( L"m" == sName )
				m_arrItems.push_back(new CPivotNoValue(oReader));
			else if ( L"n" == sName )
				m_arrItems.push_back(new CPivotNumericValue(oReader));
			else if ( L"s" == sName )
				m_arrItems.push_back(new CPivotCharacterValue(oReader));
		}
	}
	void COLAPGroupItems::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, L"count", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}

//------------------------------------
	void CRangeGroupingProperties::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<reference");
			WritingStringNullableAttrString(L"groupBy",		m_oGroupBy, m_oGroupBy->ToString());
			WritingStringNullableAttrBool2(L"autoStart",	m_oAutoStart);
			WritingStringNullableAttrBool2(L"autoEnd",		m_oAutoEnd);
			WritingStringNullableAttrDouble2(L"startNum",	m_oStartNum);
			WritingStringNullableAttrDouble2(L"endNum",		m_oEndNum);
			WritingStringNullableAttrDouble2(L"groupInterval",	m_oGroupInterval);
			WritingStringNullableAttrString(L"startDate",	m_oStartDate, m_oStartDate->ToString());
			WritingStringNullableAttrString(L"endDate",		m_oEndDate, m_oEndDate->ToString());
		writer.WriteString(L"/>");

	}
	void CRangeGroupingProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
	}
	void CRangeGroupingProperties::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"m_oGroupBy", m_oGroupBy )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"m_oAutoStart", m_oAutoStart )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"m_oAutoEnd", m_oAutoEnd )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"m_oStartDate", m_oStartDate )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"m_oEndDate", m_oEndDate )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"m_oStartNum", m_oStartNum )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"m_oEndNum", m_oEndNum )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"m_oGroupInterval", m_oGroupInterval )
		WritingElement_ReadAttributes_End( oReader )
	}					
//------------------------------------
	void CPivotCharacterValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<s");
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			WritingStringNullableAttrEncodeXmlString2(L"v",	m_oValue);
			WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			WritingStringNullableAttrBool2(L"f",	m_oCalculated);
			WritingStringNullableAttrBool2(L"u",	m_oUnused);
			WritingStringNullableAttrBool2(L"b",	m_oBold);
			WritingStringNullableAttrBool2(L"i",	m_oItalic);
			WritingStringNullableAttrBool2(L"st",	m_oStrike);
			WritingStringNullableAttrBool2(L"un",	m_oUnderline);
			WritingStringNullableAttrBool2(L"i",	m_oItalic);
			WritingStringNullableAttrInt(L"bc", m_oBackColor, m_oBackColor->GetValue());
			WritingStringNullableAttrInt(L"fc", m_oForeColor, m_oForeColor->GetValue());
			WritingStringNullableAttrInt(L"in", m_oFormatIndex, m_oFormatIndex->GetValue());
		writer.WriteString(L">");

		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</s>");
	}
	void CPivotCharacterValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"x" == sName )
				m_arrItems.push_back(new CMemberPropertyIndex(oReader));
		}
	}
	void CPivotCharacterValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"v", m_oValue )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"c", m_oCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"f", m_oCalculated )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"u", m_oUnused )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"cp", m_oCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"b", m_oBold )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"i", m_oItalic )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"st", m_oStrike )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"un", m_oUnderline )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"bc", m_oBackColor )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"fc", m_oForeColor )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"in", m_oFormatIndex )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotErrorValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<e");
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			WritingStringNullableAttrEncodeXmlString2(L"v",	m_oValue);
			WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			WritingStringNullableAttrBool2(L"f",	m_oCalculated);
			WritingStringNullableAttrBool2(L"u",	m_oUnused);
			WritingStringNullableAttrBool2(L"b",	m_oBold);
			WritingStringNullableAttrBool2(L"i",	m_oItalic);
			WritingStringNullableAttrBool2(L"st",	m_oStrike);
			WritingStringNullableAttrBool2(L"un",	m_oUnderline);
			WritingStringNullableAttrBool2(L"i",	m_oItalic);
			WritingStringNullableAttrInt(L"bc", m_oBackColor, m_oBackColor->GetValue());
			WritingStringNullableAttrInt(L"fc", m_oForeColor, m_oForeColor->GetValue());
			WritingStringNullableAttrInt(L"in", m_oFormatIndex, m_oFormatIndex->GetValue());
		writer.WriteString(L">");

		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</s>");
	}
	void CPivotErrorValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"x" == sName )
				m_arrItems.push_back(new CMemberPropertyIndex(oReader));
		}
	}
	void CPivotErrorValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"v", m_oValue )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"c", m_oCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"f", m_oCalculated )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"u", m_oUnused )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"cp", m_oCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"b", m_oBold )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"i", m_oItalic )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"st", m_oStrike )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"un", m_oUnderline )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"bc", m_oBackColor )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"fc", m_oForeColor )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"in", m_oFormatIndex )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotNumericValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<n");
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			WritingStringNullableAttrDouble2(L"v",	m_oValue);
			WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			WritingStringNullableAttrBool2(L"f",	m_oCalculated);
			WritingStringNullableAttrBool2(L"u",	m_oUnused);
			WritingStringNullableAttrBool2(L"b",	m_oBold);
			WritingStringNullableAttrBool2(L"i",	m_oItalic);
			WritingStringNullableAttrBool2(L"st",	m_oStrike);
			WritingStringNullableAttrBool2(L"un",	m_oUnderline);
			WritingStringNullableAttrBool2(L"i",	m_oItalic);
			WritingStringNullableAttrInt(L"bc", m_oBackColor, m_oBackColor->GetValue());
			WritingStringNullableAttrInt(L"fc", m_oForeColor, m_oForeColor->GetValue());
			WritingStringNullableAttrInt(L"in", m_oFormatIndex, m_oFormatIndex->GetValue());
		writer.WriteString(L">");

		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</n>");
	}
	void CPivotNumericValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"x" == sName )
				m_arrItems.push_back(new CMemberPropertyIndex(oReader));
		}
	}
	void CPivotNumericValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"v", m_oValue )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"c", m_oCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"f", m_oCalculated )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"u", m_oUnused )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"cp", m_oCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"b", m_oBold )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"i", m_oItalic )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"st", m_oStrike )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"un", m_oUnderline )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"bc", m_oBackColor )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"fc", m_oForeColor )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"in", m_oFormatIndex )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotDateTimeValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<d");
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			WritingStringNullableAttrString(L"v",	m_oValue, m_oValue->ToString());
			WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			WritingStringNullableAttrBool2(L"f",	m_oCalculated);
			WritingStringNullableAttrBool2(L"u",	m_oUnused);
		writer.WriteString(L">");

		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</d>");
	}
	void CPivotDateTimeValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"x" == sName )
				m_arrItems.push_back(new CMemberPropertyIndex(oReader));
		}
	}
	void CPivotDateTimeValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"v", m_oValue )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"c", m_oCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"f", m_oCalculated )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"u", m_oUnused )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"cp", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotBooleanValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<b");
			WritingStringNullableAttrBool2(L"v",	m_oValue);
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			WritingStringNullableAttrBool2(L"f",	m_oCalculated);
			WritingStringNullableAttrBool2(L"u",	m_oUnused);
		writer.WriteString(L">");

		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</b>");
	}
	void CPivotBooleanValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"x" == sName )
				m_arrItems.push_back(new CMemberPropertyIndex(oReader));
		}
	}
	void CPivotBooleanValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"v", m_oValue )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"c", m_oCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"f", m_oCalculated )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"u", m_oUnused )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"cp", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotNoValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<m");
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			WritingStringNullableAttrBool2(L"f",	m_oCalculated);
			WritingStringNullableAttrBool2(L"u",	m_oUnused);
			WritingStringNullableAttrBool2(L"b",	m_oBold);
			WritingStringNullableAttrBool2(L"i",	m_oItalic);
			WritingStringNullableAttrBool2(L"st",	m_oStrike);
			WritingStringNullableAttrBool2(L"un",	m_oUnderline);
			WritingStringNullableAttrBool2(L"i",	m_oItalic);
			WritingStringNullableAttrInt(L"bc", m_oBackColor, m_oBackColor->GetValue());
			WritingStringNullableAttrInt(L"fc", m_oForeColor, m_oForeColor->GetValue());
			WritingStringNullableAttrInt(L"in", m_oFormatIndex, m_oFormatIndex->GetValue());
		writer.WriteString(L">");

		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</m>");
	}
	void CPivotNoValue::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"x" == sName )
				m_arrItems.push_back(new CMemberPropertyIndex(oReader));
		}
	}
	void CPivotNoValue::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if	( oReader, L"c", m_oCaption )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"f", m_oCalculated )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"u", m_oUnused )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"cp", m_oCount )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"b", m_oBold )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"i", m_oItalic )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"st", m_oStrike )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"un", m_oUnderline )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"bc", m_oBackColor )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"fc", m_oForeColor )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"in", m_oFormatIndex )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotCacheSource::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<cacheSource");
			WritingStringNullableAttrInt(L"connectionId", m_oConnectionId, m_oConnectionId->GetValue());
			WritingStringNullableAttrString(L"type", m_oType, m_oType->ToString());
		writer.WriteString(L">");
		
		if(m_oWorksheetSource.IsInit())
		{
			m_oWorksheetSource->toXML(writer);
		}
		if(m_oConsolidation.IsInit())
		{
			m_oConsolidation->toXML(writer);
		}
		writer.WriteString(L"</cacheSource>");
	}
	void CPivotCacheSource::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"consolidation" == sName )
				m_oConsolidation = oReader;
			else if ( L"worksheetSource" == sName )
				m_oWorksheetSource = oReader;
			else if ( L"extLst" == sName )
				m_oExtLst = oReader;
		}
	}
	void CPivotCacheSource::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"connectionId", m_oConnectionId )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"type", m_oType )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CWorksheetSource::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<worksheetSource");
			WritingStringNullableAttrEncodeXmlString2(L"sheet", m_oSheet);
			WritingStringNullableAttrEncodeXmlString2(L"ref", m_oRef);
			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
		writer.WriteString(L"/>");
	}
	void CWorksheetSource::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
	}
	void CWorksheetSource::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"sheet", m_oSheet )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"ref", m_oRef )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"name", m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"id", m_oRid )
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
//------------------------------------
	void CPageItemValues::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<pages");
			WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
        
		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</pages>");
	}
	void CPageItemValues::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"page" == sName)
				m_arrItems.push_back(new CPageItems(oReader));
		}
	}
	void CPageItemValues::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, L"count", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPageItems::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<page");
			WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
        
		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</page>");
	}
	void CPageItems::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"pageItem" == sName)
				m_arrItems.push_back(new CPageItem(oReader));
		}
	}
	void CPageItems::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, L"count", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPageItem::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<pageItem");
			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
		writer.WriteString(L"/>");
	}
	void CPageItem::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
	}
	void CPageItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, L"name", m_oName )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CRangeSets::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<rangeSets");
			WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
        
		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</rangeSets>");
	}
	void CRangeSets::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"rangeSet" == sName)
				m_arrItems.push_back(new CRangeSet(oReader));
		}
	}
	void CRangeSets::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, L"count", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CRangeSet::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<rangeSet");
			WritingStringNullableAttrEncodeXmlString2(L"name", m_oName);
			WritingStringNullableAttrEncodeXmlString2(L"sheet", m_oSheet);
			WritingStringNullableAttrString(L"ref", m_oRef, *m_oRef);
			WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
			WritingStringNullableAttrInt(L"i1", m_oI1, m_oI1->GetValue());
			WritingStringNullableAttrInt(L"i2", m_oI2, m_oI2->GetValue());
			WritingStringNullableAttrInt(L"i3", m_oI3, m_oI3->GetValue());
			WritingStringNullableAttrInt(L"i4", m_oI4, m_oI4->GetValue());
		writer.WriteString(L"/>");
	}
	void CRangeSet::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
	}
	void CRangeSet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"sheet", m_oSheet )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"name", m_oName )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"ref", m_oRef )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"id", m_oRid )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"i1", m_oI1 )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"i2", m_oI2 )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"i3", m_oI3 )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"i4", m_oI4 )
		WritingElement_ReadAttributes_End_No_NS( oReader )
	}
//------------------------------------
	void CConsolidationSource::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<consolidation");
			WritingStringNullableAttrBool2(L"autoPage", m_oAutoPage);
		writer.WriteString(L">");
		
		if(m_oPages.IsInit())
		{
			m_oPages->toXML(writer);
		}
		if(m_oRangeSets.IsInit())
		{
			m_oRangeSets->toXML(writer);
		}
		writer.WriteString(L"</consolidation>");
	}
	void CConsolidationSource::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"pages" == sName )
				m_oPages = oReader;
			else if ( L"rangeSets" == sName )
				m_oRangeSets = oReader;
		}
	}
	void CConsolidationSource::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"autoPage", m_oAutoPage )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CFieldGroupProperties::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<fieldGroup");
			WritingStringNullableAttrInt(L"base", m_oBase, m_oBase->GetValue());
			WritingStringNullableAttrInt(L"par", m_oPar, m_oPar->GetValue());
		writer.WriteString(L">");
		
		if(m_oDiscretePr.IsInit())
		{
			m_oDiscretePr->toXML(writer);
		}
		if(m_oRangePr.IsInit())
		{
			m_oRangePr->toXML(writer);
		}
		if(m_oGroupItems.IsInit())
		{
			m_oGroupItems->toXML(writer);
		}
		writer.WriteString(L"</fieldGroup>");
	}
	void CFieldGroupProperties::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;
		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"discretePr" == sName )
				m_oDiscretePr = oReader;
			else if ( L"rangePr" == sName )
				m_oRangePr = oReader;
			else if ( L"groupItems" == sName )
				m_oGroupItems = oReader;
		}
	}
	void CFieldGroupProperties::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"base", m_oBase )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"par", m_oPar )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotCacheRecordsFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oPath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		m_oPivotCacheRecords = oReader;
	}
	void CPivotCacheRecordsFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if(m_oPivotCacheRecords.IsInit())
		{
			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			m_oPivotCacheRecords->toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write( oPath, oDirectory, oContent );
		}
		else if(m_nDataLength > 0 && m_pData)
		{
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(oPath.GetPath());
			oFile.WriteFile(m_pData, m_nDataLength);
			oFile.CloseFile();

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write( oPath, oDirectory, oContent );
		}
	}
//------------------------------------
	void CPivotCacheRecords::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<pivotCacheRecords \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr16\" \
xmlns:xr16=\"http://schemas.microsoft.com/office/spreadsheetml/2017/revision16\"");			
			WritingStringAttrInt(L"count", (int)m_arrItems.size());
		writer.WriteString(L">");
        
		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
				m_arrItems[i]->toXML(writer);
            }
        }
		writer.WriteString(L"</pivotCacheRecords>");
	}
	void CPivotCacheRecords::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if (L"r" == sName)
				m_arrItems.push_back(new CPivotCacheRecord(oReader));
			else if (L"extLst" == sName)
				m_oExtLst = oReader;
		}
	}
	void CPivotCacheRecords::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle ( oReader, L"count", m_oCount )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CPivotCacheRecord::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<r>");

		for ( size_t i = 0; i < m_arrItems.size(); ++i)
        {
            if (  m_arrItems[i] )
            {
                m_arrItems[i]->toXML(writer);
            }
        }
		
		writer.WriteString(L"</r>");
	}
	void CPivotCacheRecord::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nCurDepth = oReader.GetDepth();
		while( oReader.ReadNextSiblingNode( nCurDepth ) )
		{
			std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

			if ( L"b" == sName )
				m_arrItems.push_back(new CPivotBooleanValue(oReader));
			else if ( L"d" == sName )
				m_arrItems.push_back(new CPivotDateTimeValue(oReader));
			else if ( L"e" == sName )
				m_arrItems.push_back(new CPivotErrorValue(oReader));
			else if ( L"m" == sName )
				m_arrItems.push_back(new CPivotNoValue(oReader));
			else if ( L"n" == sName )
				m_arrItems.push_back(new CPivotNumericValue(oReader));
			else if ( L"s" == sName )
				m_arrItems.push_back(new CPivotCharacterValue(oReader));
			else if ( L"x" == sName )
				m_arrItems.push_back(new CSharedItemsIndex(oReader));
		}
	}
} //Spreadsheet
} // namespace OOX
