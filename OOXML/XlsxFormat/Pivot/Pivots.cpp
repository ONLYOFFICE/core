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

#include "../../XlsbFormat/Xlsb.h"
#include "../../XlsbFormat/PivotCacheRecordsStream.h"
#include "../../XlsbFormat/PivotTableStream.h"
#include "../../XlsbFormat/PivotCacheDefStream.h"
#include "../../XlsbFormat/Biff12_unions/PIVOTCACHERECORDS.h"
#include "../../XlsbFormat/Biff12_unions/PIVOTCACHERECORD.h"
#include "../../XlsbFormat/Biff12_unions/PIVOTCACHERECORDDT.h"
#include "../../XlsbFormat/Biff12_unions/PCDIDT.h"
#include "../../XlsbFormat/Biff12_records/PCDIBoolean.h"
#include "../../XlsbFormat/Biff12_records/PCDIMissing.h"
#include "../../XlsbFormat/Biff12_records/PCDINumber.h"
#include "../../XlsbFormat/Biff12_records/PCDIError.h"
#include "../../XlsbFormat/Biff12_records/PCDIString.h"
#include "../../XlsbFormat/Biff12_records/PCDIDatetime.h"
#include "../../XlsbFormat/Biff12_records/PCDIIndex.h"
#include "../../XlsbFormat/Biff12_records/PCDIABoolean.h"
#include "../../XlsbFormat/Biff12_records/PCDIAMissing.h"
#include "../../XlsbFormat/Biff12_records/PCDIANumber.h"
#include "../../XlsbFormat/Biff12_records/PCDIAError.h"
#include "../../XlsbFormat/Biff12_records/PCDIAString.h"
#include "../../XlsbFormat/Biff12_records/PCDIADatetime.h"
#include "../../XlsbFormat/Biff12_records/BeginSXView.h"
#include "../../XlsbFormat/Biff12_unions/SXLOCATION.h"
#include "../../XlsbFormat/Biff12_records/BeginSXLocation.h"
#include "../../XlsbFormat/Biff12_unions/SXVDS.h"
#include "../../XlsbFormat/Biff12_unions/SXVD.h"
#include "../../XlsbFormat/Biff12_records/BeginSXVD.h"
#include "../../XlsbFormat/Biff12_unions/SXVIS.h"
#include "../../XlsbFormat/Biff12_unions/SXVI.h"
#include "../../XlsbFormat/Biff12_records/BeginSXVI.h"
#include "../../XlsbFormat/Biff12_unions/AUTOSORTSCOPE.h"
#include "../../XlsbFormat/Biff12_unions/PIVOTRULE.h"
#include "../../XlsbFormat/Biff12_records/BeginPRule.h"
#include "../../XlsbFormat/Biff12_unions/PRFILTERS.h"
#include "../../XlsbFormat/Biff12_unions/PRFILTER.h"
#include "../../XlsbFormat/Biff12_unions/PRFITEM.h"
#include "../../XlsbFormat/Biff12_records/BeginPRFilter.h"
#include "../../XlsbFormat/Biff12_records/BeginPRFItem.h"
#include "../../XlsbFormat/Biff12_unions/ISXVDRWS.h"
#include "../../XlsbFormat/Biff12_unions/ISXVDCOLS.h"
#include "../../XlsbFormat/Biff12_records/BeginISXVDRws.h"
#include "../../XlsbFormat/Biff12_records/BeginISXVDCols.h"
#include "../../XlsbFormat/Biff12_unions/SXLIRWS.h"
#include "../../XlsbFormat/Biff12_unions/SXLICOLS.h"
#include "../../XlsbFormat/Biff12_unions/SXLI.h"
#include "../../XlsbFormat/Biff12_unions/ISXVIS.h"
#include "../../XlsbFormat/Biff12_records/BeginSXLI.h"
#include "../../XlsbFormat/Biff12_records/BeginISXVIs.h"
#include "../../XlsbFormat/Biff12_unions/SXDIS.h"
#include "../../XlsbFormat/Biff12_unions/SXDI.h"
#include "../../XlsbFormat/Biff12_records/BeginSXDI.h"
#include "../../XlsbFormat/Biff12_unions/SXFORMATS.h"
#include "../../XlsbFormat/Biff12_unions/SXFORMAT.h"
#include "../../XlsbFormat/Biff12_records/BeginSXFormat.h"
#include "../../XlsbFormat/Biff12_records/TableStyleClient.h"
#include "../../XlsbFormat/Biff12_unions/SXPIS.h"
#include "../../XlsbFormat/Biff12_unions/SXPI.h"
#include "../../XlsbFormat/Biff12_records/BeginSXPI.h"
#include "../../XlsbFormat/Biff12_records/BeginPivotCacheDef.h"
#include "../../XlsbFormat/Biff12_unions/PCDFIELDS.h"
#include "../../XlsbFormat/Biff12_unions/PCDFIELD.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDField.h"
#include "../../XlsbFormat/Biff12_unions/PCDFATBL.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDFAtbl.h"
#include "../../XlsbFormat/Biff12_unions/PCDIA.h"
#include "../../XlsbFormat/Biff12_unions/PCDI.h"
#include "../../XlsbFormat/Biff12_unions/PCDIRUN.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDIRun.h"
#include "../../XlsbFormat/Biff12_unions/PCDFGROUP.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDFGroup.h"
#include "../../XlsbFormat/Biff12_unions/PCDFGDISCRETE.h"
#include "../../XlsbFormat/Biff12_unions/PCDFGRANGE.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDFGRange.h"
#include "../../XlsbFormat/Biff12_unions/PCDFGITEMS.h"
#include "../../XlsbFormat/Biff12_unions/PCDSOURCE.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDSource.h"
#include "../../XlsbFormat/Biff12_unions/PCDSRANGE.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDSRange.h"
#include "../../XlsbFormat/Biff12_unions/PCDSCONSOL.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDSConsol.h"
#include "../../XlsbFormat/Biff12_unions/PCDSCPAGES.h"
#include "../../XlsbFormat/Biff12_unions/PCDSCPAGE.h"
#include "../../XlsbFormat/Biff12_unions/PCDSCPITEM.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDSCPItem.h"
#include "../../XlsbFormat/Biff12_unions/PCDSCSETS.h"
#include "../../XlsbFormat/Biff12_unions/PCDSCSET.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDSCSet.h"
#include "../../XlsbFormat/Biff12_records/PCRRecord.h"

#include <boost/range/adaptor/reversed.hpp>

namespace OOX
{
namespace Spreadsheet
{

    //struct NullDeleter {template<typename T> void operator()(T*) {} };
    void CPivotTableFile::readBin(const CPath& oPath)
    {
        CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
        if (xlsb)
        {
            XLS::BaseObjectPtr pivotTableStream(new XLSB::PivotTableStream());
            //XLSB::PivotTableStreamPtr pivotTableStream = std::make_shared<XLSB::PivotTableStream>();

            xlsb->ReadBin(oPath, pivotTableStream.get());

            if (pivotTableStream != nullptr)
            {                 
                //XLS::BaseObjectPtr ptr(static_cast<XLS::BaseObject*>(pivotTableStream.get()), NullDeleter());
                //XLS::BaseObjectPtr ptr = boost::make_shared<XLS::BaseObject>(static_cast<XLS::BaseObject*>(pivotTableStream.get()));
                m_oPivotTableDefinition = pivotTableStream;
            }

            //pivotTableStream.reset();
        }
    }

	void CPivotTableFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
        IFileContainer::Read( oRootPath, oPath );

        if( m_oReadPath.GetExtention() == _T(".bin"))
        {
            readBin(m_oReadPath);
            return;
        }

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
		WritingStringNullableAttrInt(L"cacheId",					m_oCacheId, m_oCacheId->GetValue());
		WritingStringNullableAttrBool2(L"dataOnRows",				m_oDataOnRows);
		WritingStringNullableAttrInt(L"dataPosition",				m_oDataPosition, m_oDataPosition->GetValue());
		WritingStringNullableAttrEncodeXmlString2(L"dataCaption",	m_oDataCaption);
		WritingStringNullableAttrEncodeXmlString2(L"grandTotalCaption",m_oGrandTotalCaption);
		WritingStringNullableAttrEncodeXmlString2(L"errorCaption",	m_oErrorCaption);
		WritingStringNullableAttrBool2(L"showError",				m_oShowError);
		WritingStringNullableAttrEncodeXmlString2(L"missingCaption",m_oMissingCaption);
		WritingStringNullableAttrBool2(L"showMissing", m_oShowMissing);
		WritingStringNullableAttrEncodeXmlString2(L"pageStyle", m_oPageStyle);
		WritingStringNullableAttrEncodeXmlString2(L"pivotTableStyle", m_oPivotTableStyle);
		WritingStringNullableAttrEncodeXmlString2(L"vacatedStyle", m_oVacatedStyle);
		WritingStringNullableAttrEncodeXmlString2(L"tag", m_oTag);
		WritingStringNullableAttrBool2(L"applyAlignmentFormats",	m_oApplyAlignmentFormats);
		WritingStringNullableAttrBool2(L"applyBorderFormats",		m_oApplyBorderFormats);
		WritingStringNullableAttrBool2(L"applyFontFormats",			m_oApplyFontFormats);
		WritingStringNullableAttrBool2(L"applyNumberFormats",		m_oApplyNumberFormats);
		WritingStringNullableAttrBool2(L"applyPatternFormats",		m_oApplyPatternFormats);
		WritingStringNullableAttrBool2(L"applyWidthHeightFormats",	m_oApplyWidthHeightFormats);
		WritingStringNullableAttrBool2(L"asteriskTotals",			m_oAsteriskTotals);
		WritingStringNullableAttrInt(L"autoFormatId",				m_oAutoFormatId, m_oAutoFormatId->GetValue());
		WritingStringNullableAttrInt(L"chartFormat",				m_oChartFormat, m_oChartFormat->GetValue());
		WritingStringNullableAttrBool2(L"colGrandTotals",			m_oColGrandTotals);
		WritingStringNullableAttrEncodeXmlString2(L"colHeaderCaption",m_oColHeaderCaption);
		WritingStringNullableAttrBool2(L"compact",					m_oCompact);
		WritingStringNullableAttrBool2(L"compactData",				m_oCompactData);
		WritingStringNullableAttrInt(L"createdVersion",				m_oCreatedVersion, m_oCreatedVersion->GetValue());
		WritingStringNullableAttrBool2(L"customListSort",			m_oCustomListSort);
		WritingStringNullableAttrBool2(L"disableFieldList",			m_oDisableFieldList);
		WritingStringNullableAttrBool2(L"editData",					m_oEditData);
		WritingStringNullableAttrBool2(L"enableDrill",				m_oEnableDrill);
		WritingStringNullableAttrBool2(L"enableFieldProperties",	m_oEnableFieldProperties);
		WritingStringNullableAttrBool2(L"enableWizard",				m_oEnableWizard);
		WritingStringNullableAttrBool2(L"disableEdit",				m_oFieldListSortAscending);
		WritingStringNullableAttrBool2(L"fieldPrintTitles",			m_oFieldPrintTitles);
		WritingStringNullableAttrBool2(L"gridDropZones",			m_oGridDropZones);
		WritingStringNullableAttrBool2(L"immersive",				m_oImmersive);
		WritingStringNullableAttrInt(L"indent",						m_oIndent, m_oIndent->GetValue());
		WritingStringNullableAttrBool2(L"itemPrintTitles",			m_oItemPrintTitles);
		WritingStringNullableAttrBool2(L"mdxSubqueries",			m_oMdxSubqueries);
		WritingStringNullableAttrBool2(L"mergeItem",				m_oMergeItem);
		WritingStringNullableAttrInt(L"minRefreshableVersion",		m_oMinRefreshableVersion, m_oMinRefreshableVersion->GetValue());
		WritingStringNullableAttrBool2(L"multipleFieldFilters",		m_oMultipleFieldFilters);
		WritingStringNullableAttrBool2(L"outline",					m_oOutline);
        WritingStringNullableAttrBool2(L"outlineData",				m_oOutlineData);
		WritingStringNullableAttrBool2(L"pageOverThenDown",			m_oPageOverThenDown);
		WritingStringNullableAttrInt(L"pageWrap",					m_oPageWrap, m_oPageWrap->GetValue());
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
		WritingStringNullableAttrBool2(L"showHeaders",				m_oShowHeaders);
		WritingStringNullableAttrBool2(L"showItems",				m_oShowItems);
		WritingStringNullableAttrBool2(L"showMemberPropertyTips",	m_oShowMemberPropertyTips);
		WritingStringNullableAttrBool2(L"showMultipleLabel",		m_oShowMultipleLabel);
		WritingStringNullableAttrBool2(L"subtotalHiddenItems",		m_oSubtotalHiddenItems);
		WritingStringNullableAttrInt(L"updatedVersion",				m_oUpdatedVersion, m_oUpdatedVersion->GetValue());
		WritingStringNullableAttrBool2(L"useAutoFormatting",		m_oUseAutoFormatting);
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
		//if(m_oConditionalFormats.IsInit())
		//	m_oConditionalFormats->toXML(writer);
		//if(m_oChartFormats.IsInit())
		//	m_oChartFormats->toXML(writer);
		//if(m_oPivotHierarchies.IsInit())
		//	m_oPivotHierarchies->toXML(writer);
		if(m_oPivotTableStyleInfo.IsInit())
			m_oPivotTableStyleInfo->toXML(writer);

		//if(m_oFilters.IsInit())
		//	m_oFilters->toXML(writer);
		//if(m_oRowHierarchiesUsage.IsInit())
		//	m_oRowHierarchiesUsage->toXML(writer);
		//if(m_oColHierarchiesUsage.IsInit())
		//	m_oColHierarchiesUsage->toXML(writer);
			
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
    void CPivotTableDefinition::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PivotTableStream*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginSXView);

            if(ptr->m_SXLOCATION != nullptr)
                m_oLocation = ptr->m_SXLOCATION;

            if(ptr->m_SXVDS != nullptr)
                m_oPivotFields = ptr->m_SXVDS;

            if(ptr->m_ISXVDRWS != nullptr)
                m_oRowFields = ptr->m_ISXVDRWS;

            if(ptr->m_ISXVDCOLS != nullptr)
                m_oColFields = ptr->m_ISXVDCOLS;

            if(ptr->m_SXLIRWS != nullptr)
                m_oRowItems = ptr->m_SXLIRWS;

            if(ptr->m_SXLICOLS != nullptr)
                m_oColItems = ptr->m_SXLICOLS;

            if(ptr->m_SXDIS != nullptr)
                m_oDataFields = ptr->m_SXDIS;

            if(ptr->m_SXFORMATS != nullptr)
                m_oFormats = ptr->m_SXFORMATS;

            if(ptr->m_BrtTableStyleClient != nullptr)
                m_oPivotTableStyleInfo = ptr->m_BrtTableStyleClient;

            if(ptr->m_SXPIS != nullptr)
                m_oPageFields = ptr->m_SXPIS;
        }
    }
    void CPivotTableDefinition::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginSXView*>(obj.get());

        if(ptr != nullptr)
        {
            m_oApplyAlignmentFormats        = ptr->ibitAtrAlc;
            m_oApplyBorderFormats           = ptr->ibitAtrBdr;
            m_oApplyFontFormats             = ptr->ibitAtrFnt;
            m_oApplyNumberFormats           = ptr->ibitAtrNum;
            m_oApplyPatternFormats          = ptr->ibitAtrPat;
            m_oApplyWidthHeightFormats      = ptr->ibitAtrProt;
            m_oApplyAlignmentFormats        = ptr->ibitAtrAlc;
            m_oApplyAlignmentFormats        = ptr->ibitAtrAlc;

            if(ptr->fHideTotAnnotation)
                m_oAsteriskTotals           = ptr->fHideTotAnnotation;

            if(!ptr->fNotVisualTotals)
                m_oVisualTotals             = ptr->fNotVisualTotals;

            m_oAutoFormatId                 = ptr->itblAutoFmt;
            m_oCacheId                      = ptr->idCache;

            if(ptr->dwCrtFmtId)
                m_oChartFormat              = ptr->dwCrtFmtId;

            if(!ptr->fColGrand)
                m_oColGrandTotals           = ptr->fColGrand;

            if(!ptr->irstColHdrName.value().empty())
                m_oColHeaderCaption         = ptr->irstColHdrName.value();

            if(!ptr->fDefaultCompact)
                m_oCompact                  = ptr->fDefaultCompact;

            if(!ptr->fCompactData)
                m_oCompactData              = ptr->fCompactData;

            if(ptr->bVerSxMacro)
                m_oCreatedVersion           = ptr->bVerSxMacro;

            if(ptr->fDontUseCustomLists)
                m_oCustomListSort           = !ptr->fDontUseCustomLists;

            if(!ptr->irstData.value().empty())
                m_oDataCaption              = ptr->irstData.value();

           // m_oDataOnRows                   = ptr->fDefaultCompact;
            m_oDataPosition                 = ptr->ipos4Data;

            if(ptr->fDisableFList)
                m_oDisableFieldList         = ptr->fDisableFList;

            if(ptr->fEnableDataEd)
                m_oEditData                 = ptr->fEnableDataEd;

            if(!ptr->fEnableDrilldown)
                m_oEnableDrill              = ptr->fEnableDrilldown;

            if(!ptr->fEnableFieldDialog)
                m_oEnableFieldProperties    = ptr->fEnableFieldDialog;

            if(!ptr->fEnableWizard)
                m_oEnableWizard             = ptr->fEnableWizard;

            if(!ptr->irstErrorString.value().empty())
                    m_oErrorCaption         = ptr->irstErrorString.value();

            if(ptr->fNonDefaultSortInFlist)
                m_oFieldListSortAscending   = ptr->fNonDefaultSortInFlist;

            if(ptr->fPrintTitles)
                m_oFieldPrintTitles         = ptr->fPrintTitles;

            if(!ptr->irstGrand.value().empty())
                    m_oGrandTotalCaption    = ptr->irstGrand.value();

            if(!ptr->fNewDropZones)
                m_oGridDropZones            = !ptr->fNewDropZones;

            if(!ptr->fTurnOffImmersive)
                m_oImmersive                = ptr->fTurnOffImmersive;

            if(ptr->cIndentInc != 1)
                m_oIndent                   = ptr->cIndentInc;

            if(ptr->fRepeatItemsOnEachPrintedPage)
                m_oItemPrintTitles          = ptr->fRepeatItemsOnEachPrintedPage;

            //m_oMdxSubqueries                      = ptr->fDefaultCompact;

            if(ptr->fMergeLabels)
                m_oMergeItem                = ptr->fMergeLabels;

            if(ptr->bVerSxUpdateableMin)
                m_oMinRefreshableVersion    = ptr->bVerSxUpdateableMin;

            if(!ptr->irstNullString.value().empty())
                    m_oMissingCaption       = ptr->irstNullString.value();

            if(ptr->fSingleFilterPerField)
                m_oMultipleFieldFilters     = !ptr->fSingleFilterPerField;

            if(!ptr->irstName.value().empty())
                    m_oName                 = ptr->irstName.value();

            if(ptr->fDefaultOutline)
                m_oOutline                  = ptr->fDefaultOutline;

            if(ptr->fOutlineData)
                m_oOutlineData              = ptr->fOutlineData;

            if(ptr->fAcrossPageLay)
                m_oPageOverThenDown         = ptr->fAcrossPageLay;

            if(!ptr->irstPageFieldStyle.value().empty())
                    m_oPageStyle            = ptr->irstPageFieldStyle.value();

            if(ptr->cWrapPage)
                m_oPageWrap                 = ptr->cWrapPage;

            if(!ptr->irstTableStyle.value().empty())
                    m_oPivotTableStyle      = ptr->irstTableStyle.value();

            if(!ptr->fPreserveFormatting)
                m_oPreserveFormatting       = ptr->fPreserveFormatting;

            if(ptr->fPrintDrillIndicators)
                m_oPrintDrill               = ptr->fPrintDrillIndicators;

            if(ptr->fPublished)
                m_oPublished                = ptr->fPublished;

            if(!ptr->fRwGrand)
                m_oRowGrandTotals           = ptr->fRwGrand;

            if(!ptr->irstRwHdrName.value().empty())
                    m_oRowHeaderCaption     = ptr->irstRwHdrName.value();

            if(ptr->fNotViewCalculatedMembers)
                m_oShowCalcMbrs             = !ptr->fNotViewCalculatedMembers;

            if(ptr->fHideDDData)
                m_oShowDataDropDown         = !ptr->fHideDDData;

            if(ptr->fNoPivotTips)
                m_oShowDataTips             = !ptr->fNoPivotTips;

            if(ptr->fHideDrillIndicators)
                m_oShowDrill                = !ptr->fHideDrillIndicators;

            if(ptr->fNoStencil)
                m_oShowDropZones            = !ptr->fNoStencil;

            if(ptr->fIncludeEmptyCol)
                m_oShowEmptyCol             = ptr->fIncludeEmptyCol;

            if(ptr->fIncludeEmptyRw)
                m_oShowEmptyRow             = ptr->fIncludeEmptyRw;

            if(ptr->fDisplayErrorString)
                m_oShowError                = ptr->fDisplayErrorString;

            if(ptr->fNoHeaders)
                m_oShowHeaders              = !ptr->fNoHeaders;

            if(!ptr->fDisplayImmediateItems)
                m_oShowItems                = ptr->fDisplayImmediateItems;

            if(!ptr->fMemPropsInTips)
                m_oShowMemberPropertyTips   = ptr->fMemPropsInTips;

            if(!ptr->fDisplayNullString)
                m_oShowMissing              = ptr->fDisplayNullString;

            if(!ptr->fPageMultipleItemLabel)
                m_oShowMultipleLabel        = ptr->fPageMultipleItemLabel;

            if(ptr->fSubtotalHiddenPageItems)
                m_oSubtotalHiddenItems      = ptr->fSubtotalHiddenPageItems;

            if(!ptr->irstTag.value().empty())
                    m_oTag                  = ptr->irstTag.value();

            if(ptr->bVerSxLastUpdated)
                m_oUpdatedVersion           = ptr->bVerSxLastUpdated;

            if(ptr->fAutoFormat)
                m_oUseAutoFormatting        = ptr->fAutoFormat;

            if(!ptr->irstVacateStyle.value().empty())
                    m_oVacatedStyle         = ptr->irstVacateStyle.value();

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
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"outlineData",	m_oOutlineData )
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
    void CColumnRowFields::fromBin(XLS::BaseObjectPtr& obj)
    {
        if(obj->get_type() == XLS::typeISXVDRWS)
        {
            auto ptrISXVDRWS = static_cast<XLSB::ISXVDRWS*>(obj.get());

            if(ptrISXVDRWS != nullptr)
            {
                auto ptrBeginISXVDRws = static_cast<XLSB::BeginISXVDRws*>(ptrISXVDRWS->m_BrtBeginISXVDRws.get());

                if(ptrBeginISXVDRws != nullptr)
                {
                    m_oCount = ptrBeginISXVDRws->cisxvd;
                    for(auto &item : ptrBeginISXVDRws->rgisxvdrws)
                    {
                        auto field = new CField();
                        field->m_oX = item;
                        m_arrItems.push_back(field);
                    }
                }
            }
        }
        else if(obj->get_type() == XLS::typeISXVDCOLS)
        {
            auto ptrISXVDCOLS = static_cast<XLSB::ISXVDCOLS*>(obj.get());

            if(ptrISXVDCOLS != nullptr)
            {
                auto ptrBeginISXVDCols = static_cast<XLSB::BeginISXVDCols*>(ptrISXVDCOLS->m_BrtBeginISXVDCols.get());

                if(ptrBeginISXVDCols != nullptr)
                {
                    m_oCount = ptrBeginISXVDCols->cisxvd;
                    for(auto &item : ptrBeginISXVDCols->rgisxvdcols)
                    {
                        auto field = new CField();
                        field->m_oX = item;
                        m_arrItems.push_back(field);
                    }
                }
            }
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
    void CColumnRowItems::fromBin(XLS::BaseObjectPtr& obj)
    {
        if(obj->get_type() == XLS::typeSXLIRWS)
        {
            auto ptrSXLIRWS = static_cast<XLSB::SXLIRWS*>(obj.get());

            m_oCount = _UINT32(ptrSXLIRWS->m_arSXLI.size());

            for(auto &item : ptrSXLIRWS->m_arSXLI)
                m_arrItems.push_back(new CColumnRowItem(item));
        }
        else if(obj->get_type() == XLS::typeSXLICOLS)
        {
            auto ptrSXLICOLS = static_cast<XLSB::SXLICOLS*>(obj.get());

            m_oCount = _UINT32(ptrSXLICOLS->m_arSXLI.size());

            for(auto &item : ptrSXLICOLS->m_arSXLI)
                m_arrItems.push_back(new CColumnRowItem(item));
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
    void CColumnRowItem::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXLI*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginSXLI);

            auto ptrISXVIS = static_cast<XLSB::ISXVIS*>(ptr->m_ISXVIS.get());

            if(ptrISXVIS != nullptr)
            {
                auto ptrBrtBeginISXVIs = static_cast<XLSB::BeginISXVIs*>(ptrISXVIS->m_BrtBeginISXVIs.get());

                if(ptrBrtBeginISXVIs != nullptr)
                {
                    for(auto &item : ptrBrtBeginISXVIs->rgisxvis)
                    {
                        auto oSharedItemsIndex = new CSharedItemsIndex();
                        oSharedItemsIndex->m_oV = item;
                        m_arrItems.push_back(oSharedItemsIndex);
                    }
                }
            }
        }
    }
    void CColumnRowItem::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginSXLI*>(obj.get());

        if(ptr != nullptr)
        {
            m_oI = ptr->iData;
            m_oR = ptr->cSic;

            switch(ptr->itmtype.value().get())
            {
                case XLSB::PivotItemType::PITDATA:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeData;
                    break;
                case XLSB::PivotItemType::PITDEFAULT:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeDefault;
                    break;
                case XLSB::PivotItemType::PITSUM:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeSum;
                    break;
                case XLSB::PivotItemType::PITCOUNTA:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeCountA;
                    break;
                case XLSB::PivotItemType::PITAVG:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeAverage;
                    break;
                case XLSB::PivotItemType::PITMAX:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeMax;
                    break;
                case XLSB::PivotItemType::PITMIN:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeMin;
                    break;
                case XLSB::PivotItemType::PITPRODUCT:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeProduct;
                    break;
                case XLSB::PivotItemType::PITCOUNT:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeCount;
                    break;
                case XLSB::PivotItemType::PITSTDDEV:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeStdDev;
                    break;
                case XLSB::PivotItemType::PITSTDDEVP:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeStdDevP;
                    break;
                case XLSB::PivotItemType::PITVAR:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeVar;
                    break;
                case XLSB::PivotItemType::PITVARP:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeVarP;
                    break;
                case XLSB::PivotItemType::PITGRAND:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeGrandTotalt;
                    break;
                case XLSB::PivotItemType::PITBLANK:
                    m_oT = SimpleTypes::Spreadsheet::EPivotItemType::typeBlank;
                    break;
            }
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
    void CDataFields::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXDIS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arSXDI.size();

            for(auto &item : ptr->m_arSXDI)
            {
                m_arrItems.push_back(new CDataField(item));
            }
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
    void CDataField::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXDI*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginSXDI);
        }
    }
    void CDataField::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginSXDI*>(obj.get());

        if(ptr != nullptr)
        {
            if(ptr->isxvd != -1)
                m_oBaseField = ptr->isxvd;

            if(ptr->isxvi != 1048832)
                m_oBaseItem = ptr->isxvi;

            m_oFld          = ptr->isxvdData;
            m_oNumFmtId     = ptr->ifmt.ifmt;

            if(!ptr->stDisplayName.value().empty())
                m_oName     = ptr->stDisplayName.value();

            switch(ptr->df.value().get())
            {
                //case XLSB::ShowDataAs::NORMAL:
                    //m_oShowDataAs = SimpleTypes::Spreadsheet::EShowDataAs::dataAsNormal;
                    //break;
                case XLSB::ShowDataAs::DIFFERENCE_:
                    m_oShowDataAs = SimpleTypes::Spreadsheet::EShowDataAs::dataAsDifference;
                    break;
                case XLSB::ShowDataAs::PERCENT:
                    m_oShowDataAs = SimpleTypes::Spreadsheet::EShowDataAs::dataAsPercentOff;
                    break;
                case XLSB::ShowDataAs::PERCENTDIFF:
                    m_oShowDataAs = SimpleTypes::Spreadsheet::EShowDataAs::dataAsPercentDiff;
                    break;
                case XLSB::ShowDataAs::INDEX:
                    m_oShowDataAs = SimpleTypes::Spreadsheet::EShowDataAs::dataAsIndex;
                    break;
                case XLSB::ShowDataAs::PERCENTOFTOTAL:
                    m_oShowDataAs = SimpleTypes::Spreadsheet::EShowDataAs::dataAsPercentOfTotal;
                    break;
                case XLSB::ShowDataAs::PERCENTOFCOL:
                    m_oShowDataAs = SimpleTypes::Spreadsheet::EShowDataAs::dataAsPercentOfCol;
                    break;
                case XLSB::ShowDataAs::PERCENTOFROW:
                    m_oShowDataAs = SimpleTypes::Spreadsheet::EShowDataAs::dataAsPercentOfRow;
                    break;
                case XLSB::ShowDataAs::PERCENTOFRUNTOTAL:
                    m_oShowDataAs = SimpleTypes::Spreadsheet::EShowDataAs::dataAsRunTotal;
                    break;
            }

            switch(ptr->iiftab.value().get())
            {
                //case XLSB::DataConsolidationFunction::SUM:
                    //m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionSum;
                    //break;
                case XLSB::DataConsolidationFunction::COUNT:
                    m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionCount;
                    break;
                case XLSB::DataConsolidationFunction::AVERAGE:
                    m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionAverage;
                    break;
                case XLSB::DataConsolidationFunction::MAX:
                    m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionMaximum;
                    break;
                case XLSB::DataConsolidationFunction::MIN:
                    m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionMinimum;
                    break;
                case XLSB::DataConsolidationFunction::PRODUCT:
                    m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionProduct;
                    break;
                case XLSB::DataConsolidationFunction::COUNTNUM:
                    m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionCountNums;
                    break;
                case XLSB::DataConsolidationFunction::STDDEV:
                    m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionStdDev;
                    break;
                case XLSB::DataConsolidationFunction::STDDEVP:
                    m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionStdDevP;
                    break;
                case XLSB::DataConsolidationFunction::STDVAR:
                    m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionVariance;
                    break;
                case XLSB::DataConsolidationFunction::STDVARP:
                    m_oSubtotal = SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionVarP;
                    break;
            }
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
    void CPageFields::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXPIS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arSXPI.size();

            for(auto &item : ptr->m_arSXPI)
            {
                m_arrItems.push_back(new CPageField(item));
            }
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
        writer.WriteString(L"<pageField");
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
    void CPageField::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXPI*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginSXPI);
        }
    }
    void CPageField::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginSXPI*>(obj.get());

        if(ptr != nullptr)
        {
            m_oFld          = ptr->isxvd;

            if(ptr->isxvi != 0x001000FE)
                m_oItem     = ptr->isxvi;

            m_oHier = ptr->isxth;

            if(!ptr->irstUnique.value().empty())
                m_oName     = ptr->irstUnique.value();

            if(!ptr->irstDisplay.value().empty())
                m_oCap      = ptr->irstDisplay.value();
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
    void CFieldItems::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXVIS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arSXVI.size();

            for(auto &item : ptr->m_arSXVI)
            {
                m_arrItems.push_back(new CFieldItem(item));
            }
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
			WritingStringNullableAttrBool2(L"s",	m_oCharacter);
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
    void CFieldItem::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXVI*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginSXVI);
        }
    }
    void CFieldItem::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginSXVI*>(obj.get());

        if(ptr != nullptr)
        {
            if(ptr->fHasChildrenEst)
                m_oChild        = ptr->fHasChildrenEst;

            if(ptr->fDrilledMember)
                m_oExpanded     = ptr->fDrilledMember;

            if(!ptr->fCollapsedMember)
                m_oDrillAcross  = ptr->fCollapsedMember;

            if(ptr->fFormula)
                m_oCalculated   = ptr->fFormula;

            if(ptr->fHidden)
                m_oHidden       = ptr->fHidden;

            if(ptr->fMissing)
                m_oMissing      = ptr->fMissing;

            if(!ptr->displayName.value().empty())
                m_oUserCaption  = ptr->displayName.value();

            if(ptr->fOlapFilterSelected)
                m_oCharacter    = ptr->fOlapFilterSelected; //?

            if(ptr->fHideDetail)
                m_oHideDetails  = !ptr->fHideDetail;

            if(ptr->iCache != -1)
                m_oItemIndex    = ptr->iCache;

            switch(ptr->itmtype.value().get())
            {
                //case XLSB::PivotItemType::PITDATA:
                //    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeData;
                 //   break;
                case XLSB::PivotItemType::PITDEFAULT:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeDefault;
                    break;
                case XLSB::PivotItemType::PITSUM:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeSum;
                    break;
                case XLSB::PivotItemType::PITCOUNTA:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeCountA;
                    break;
                case XLSB::PivotItemType::PITAVG:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeAverage;
                    break;
                case XLSB::PivotItemType::PITMAX:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeMax;
                    break;
                case XLSB::PivotItemType::PITMIN:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeMin;
                    break;
                case XLSB::PivotItemType::PITPRODUCT:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeProduct;
                    break;
                case XLSB::PivotItemType::PITCOUNT:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeCount;
                    break;
                case XLSB::PivotItemType::PITSTDDEV:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeStdDev;
                    break;
                case XLSB::PivotItemType::PITSTDDEVP:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeStdDevP;
                    break;
                case XLSB::PivotItemType::PITVAR:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeVar;
                    break;
                case XLSB::PivotItemType::PITVARP:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeVarP;
                    break;
                case XLSB::PivotItemType::PITGRAND:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeGrandTotalt;
                    break;
                case XLSB::PivotItemType::PITBLANK:
                    m_oItemType = SimpleTypes::Spreadsheet::EPivotItemType::typeBlank;
                    break;
            }
        }
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
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"s", m_oCharacter )
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
    void CPivotFields::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXVDS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arSXVD.size();

            for(auto &item : ptr->m_arSXVD)
                m_arrItems.push_back(new CPivotField(item));
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
    void CPivotField::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXVD*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginSXVD);

            if(ptr->m_SXVIS != nullptr)
                m_oItems            = ptr->m_SXVIS;

            if(ptr->m_AUTOSORTSCOPE != nullptr)
                m_oAutoSortScope    = ptr->m_AUTOSORTSCOPE;

            //if(ptr->m_FRTSXVD != nullptr)
                //m_oExtLst           = ptr->m_FRTSXVD;
        }
    }
    void CPivotField::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginSXVD*>(obj.get());

        if(ptr != nullptr)
        {
            if(ptr->fDrilledLevel)
                m_oAllDrilled               = ptr->fDrilledLevel;

            if(ptr->fAutoShow)
                m_oAutoShow                 = ptr->fAutoShow;

            if(ptr->fAverage)
                m_oAvgSubtotal              = ptr->fAverage;

            if(ptr->sxaxis.bCol)
                m_oAxis = SimpleTypes::Spreadsheet::EPivotAxisType::axisCol;
            else if(ptr->sxaxis.bPage)
                m_oAxis = SimpleTypes::Spreadsheet::EPivotAxisType::axisPage;
            else if(ptr->sxaxis.bRw)
                m_oAxis = SimpleTypes::Spreadsheet::EPivotAxisType::axisRow;
            else if(ptr->sxaxis.bData)
                m_oAxis = SimpleTypes::Spreadsheet::EPivotAxisType::axisValues;

            if(!ptr->fCompact)
                m_oCompact                  = ptr->fCompact;

            if(ptr->fCounta)
                m_oCountASubtotal           = ptr->fCounta;

            if(ptr->fCount)
                m_oCountSubtotal            = ptr->fCount;
            //m_oDataField                    = ptr->fDrilledLevel;
            m_oDataSourceSort               = ptr->fTensorSort;

            if(ptr->fItemsDrilledByDefault)
                m_oDefaultAttributeDrillState = ptr->fItemsDrilledByDefault;

            if(!ptr->fDefault)
                m_oDefaultSubtotal          = ptr->fDefault;

            if(!ptr->fDragToHide)
                m_oDragOff                  = ptr->fDragToHide;

            if(!ptr->fDragToColumn)
                m_oDragToCol                = ptr->fDragToColumn;

            if(!ptr->fDragToData)
                m_oDragToData               = ptr->fDragToData;

            if(!ptr->fDragToPage)
                m_oDragToPage               = ptr->fDragToPage;

            if(!ptr->fDragToRow)
                m_oDragToRow                = ptr->fDragToRow;

            if(ptr->fHiddenLvl)
                m_oHiddenLevel              = ptr->fHiddenLvl;

            if(ptr->fHideNewItems)
                m_oHideNewItems             = ptr->fHideNewItems;

            if(ptr->fFilterInclusive)
                m_oIncludeNewItemsInFilter  = ptr->fFilterInclusive;

            if(ptr->fInsertBlankRow)
                m_oInsertBlankRow           = ptr->fInsertBlankRow;

            if(ptr->fPageBreaksBetweenItems)
                m_oInsertPageBreak          = ptr->fPageBreaksBetweenItems;

            if(ptr->citmAutoShow != 10)
                m_oItemPageCount            = ptr->citmAutoShow;

            if(ptr->fMax)
                m_oMaxSubtotal              = ptr->fMax;

            if(ptr->fHasAdvFilter)
                m_oMeasureFilter            = ptr->fHasAdvFilter;

            if(ptr->fMin)
                m_oMinSubtotal              = ptr->fMin;

            if(ptr->fEnableMultiplePageItems)
                m_oMultipleItemSelectionAllowed = ptr->fEnableMultiplePageItems;

            if(!ptr->irstName.value().empty())
                    m_oName                 = ptr->irstName.value();

            if(ptr->fNotAutoSortDft)
                m_oNonAutoSortDefault       = ptr->fNotAutoSortDft;

            m_oNumFmtId                     = ptr->ifmt;

            if(!ptr->fOutline)
                m_oOutline                  = ptr->fOutline;

            if(ptr->fProduct)
                m_oProductSubtotal          = ptr->fProduct;

            if(ptr->fAutoShow)
                m_oRankBy                   = ptr->isxdiAutoShow;

            if(ptr->fServerBased)
                m_oServerField              = ptr->fServerBased;

            if(!ptr->fShowAllItems)
                m_oShowAll                  = ptr->fShowAllItems;

            if(ptr->fHideDD)
                m_oShowDropDowns            = !ptr->fHideDD;

            if(ptr->fMemPropDisplayInCaption)
                m_oShowPropAsCaption        = ptr->fMemPropDisplayInCaption;

            if(ptr->fMemPropDisplayInReport)
                m_oShowPropCell             = ptr->fMemPropDisplayInReport;

            if(ptr->fMemPropDisplayInTip)
                m_oShowPropTip              = ptr->fMemPropDisplayInTip;

            if(!ptr->fAutoSort)
                m_oSortType                 = SimpleTypes::Spreadsheet::EFieldSortType::sortManual;
            else if(ptr->fAscendSort)
                m_oSortType                 = SimpleTypes::Spreadsheet::EFieldSortType::sortDescending;
            else
                m_oSortType                 = SimpleTypes::Spreadsheet::EFieldSortType::sortAscending;


            if(ptr->fStdevp)
                m_oStdDevPSubtotal          = ptr->fStdevp;

            if(ptr->fStdev)
                m_oStdDevSubtotal           = ptr->fStdev;

            if(!ptr->irstSub.value().empty())
                    m_oSubtotalCaption      = ptr->irstSub.value();

            if(!ptr->fSubtotalAtTop)
                m_oSubtotalTop              = ptr->fSubtotalAtTop;

            if(ptr->fSum)
                m_oSumSubtotal              = ptr->fSum;

            if(!ptr->fTopAutoShow)
                m_oTopAutoShow              = ptr->fTopAutoShow;

            if(!ptr->irstMemberPropertyCaption.value().empty())
                    m_oUniqueMemberProperty = ptr->irstMemberPropertyCaption.value();

            if(ptr->fVarp)
                m_oVarPSubtotal             = ptr->fVarp;

            if(ptr->fVar)
                m_oVarSubtotal              = ptr->fVar;
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
    void CReferences::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PRFILTERS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arPRFILTER.size();

            for(auto &item : ptr->m_arPRFILTER)
            {
                m_arrItems.push_back(new CReference(item));
            }
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
    void CReference::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PRFILTER*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPRFilter);

            for(auto &item : ptr->m_arPRFITEM)
            {
                auto ptrPRFITEM = static_cast<XLSB::PRFITEM*>(item.get());

                if(ptrPRFITEM != nullptr)
                {
                    m_oX = ptrPRFITEM->m_BrtBeginPRFItem;
                }
            }
        }
    }

    void CReference::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPRFilter*>(obj.get());

        if(ptr != nullptr)
        {
            m_oAvgSubtotal      = ptr->prFilter.itmtypeAVERAGE;
            //m_oByPosition       = ptr->prFilter.itmtypeAVERAGE;?itmtypeData
            m_oCountASubtotal   = ptr->prFilter.itmtypeCOUNTA;
            m_oCountSubtotal    = ptr->prFilter.itmtypeCOUNT;
            m_oDefaultSubtotal  = ptr->prFilter.itmtypeDEFAULT;
            m_oMaxSubtotal      = ptr->prFilter.itmtypeMAX;
            m_oMinSubtotal      = ptr->prFilter.itmtypeMIN;
            m_oProductSubtotal  = ptr->prFilter.itmtypePRODUCT;
            //m_oRelative         = ptr->prFilter.itmtypeAVERAGE;?itmtypeData
            m_oSelected         = ptr->prFilter.fSelected;
            m_oStdDevPSubtotal  = ptr->prFilter.itmtypeSTDEVP;
            m_oStdDevSubtotal   = ptr->prFilter.itmtypeSTDEV;
            m_oSumSubtotal      = ptr->prFilter.itmtypeSUM;
            m_oVarPSubtotal     = ptr->prFilter.itmtypeVARP;
            m_oVarSubtotal      = ptr->prFilter.itmtypeVAR;

            m_oField            = ptr->prFilter.isxvd;
            m_oCount            = ptr->prFilter.cItems;

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
    void CPivotTableFormats::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXFORMATS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arSXFORMAT.size();

            for(auto &item : ptr->m_arSXFORMAT)
            {
                m_arrItems.push_back(new CPivotTableFormat(item));
            }
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
    void CPivotTableFormat::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXFORMAT*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginSXFormat);

            if(ptr->m_PIVOTRULE != nullptr)
                m_oPivotArea = ptr->m_PIVOTRULE;
        }
    }
    void CPivotTableFormat::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginSXFormat*>(obj.get());

        if(ptr != nullptr)
        {
            //m_oAction = rlType ??
            m_oDxfId  = ptr->dxfid;
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
    void CAutoSortScope::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::AUTOSORTSCOPE*>(obj.get());

        if(ptr != nullptr)
        {
            if(ptr->m_PIVOTRULE != nullptr)
                m_oPivotArea = ptr->m_PIVOTRULE;
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
    void CPivotArea::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PIVOTRULE*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPRule);

            if(ptr->m_PRFILTERS != nullptr)
                m_oReferences = ptr->m_PRFILTERS;
        }
    }
    void CPivotArea::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPRule*>(obj.get());

        if(ptr != nullptr)
        {
            if(ptr->pruleheaderdata.sxaxis.bCol)
                m_oAxis = SimpleTypes::Spreadsheet::EPivotAxisType::axisCol;
            else if(ptr->pruleheaderdata.sxaxis.bPage)
                m_oAxis = SimpleTypes::Spreadsheet::EPivotAxisType::axisPage;
            else if(ptr->pruleheaderdata.sxaxis.bRw)
                m_oAxis = SimpleTypes::Spreadsheet::EPivotAxisType::axisRow;
            else if(ptr->pruleheaderdata.sxaxis.bData)
                m_oAxis = SimpleTypes::Spreadsheet::EPivotAxisType::axisValues;

            if(ptr->pruleheaderdata.fCacheBased)
                m_oCacheIndex               = ptr->pruleheaderdata.fCacheBased;

            if(ptr->pruleheaderdata.fFuzzy)
                m_oCollapsedLevelsAreSubtotals  = ptr->pruleheaderdata.fFuzzy; //fPart?

            if(!ptr->pruleheaderdata.fDataOnly)
                m_oDataOnly                 = ptr->pruleheaderdata.fDataOnly;

            m_oField                        = ptr->pruleheaderdata.isxvd;
            m_oFieldPosition                = ptr->pruleheaderdata.iDim;

            if(ptr->pruleheaderdata.fGrandCol)
                m_oGrandCol                 = ptr->pruleheaderdata.fGrandCol;
            if(ptr->pruleheaderdata.fGrandRw)
                m_oGrandRow                 = ptr->pruleheaderdata.fGrandRw;

            if(ptr->pruleheaderdata.fLabelOnly)
                m_oLabelOnly                = ptr->pruleheaderdata.fLabelOnly;

            if(!ptr->pruleheaderdata.rfxLoc.toString().empty())
                    m_oOffsetRef            = ptr->pruleheaderdata.rfxLoc.toString();

            if(!ptr->pruleheaderdata.fLineMode)
                m_oOutline                  = ptr->pruleheaderdata.fLineMode;

            switch(ptr->pruleheaderdata.isxrtype)
            {
                case 0x00:
                    m_oType = SimpleTypes::Spreadsheet::EPivotAreaType::areaNone;
                    break;
                //case 0x01:
                    //m_oType = SimpleTypes::Spreadsheet::EPivotAreaType::areaNormal;
                    //break;
                case 0x02:
                    m_oType = SimpleTypes::Spreadsheet::EPivotAreaType::areaData;
                    break;
                case 0x03:
                    m_oType = SimpleTypes::Spreadsheet::EPivotAreaType::areaAll;
                    break;
                case 0x04:
                    m_oType = SimpleTypes::Spreadsheet::EPivotAreaType::areaOrigin;
                    break;
                case 0x05:
                    m_oType = SimpleTypes::Spreadsheet::EPivotAreaType::areaFieldButton;
                    break;
                case 0x06:
                    m_oType = SimpleTypes::Spreadsheet::EPivotAreaType::areaTopEnd;
                    break;
            }
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
		writer.WriteString(L"<location");
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

    void CPivotTableLocation::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::SXLOCATION*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginSXLocation);
        }
    }

    void CPivotTableLocation::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginSXLocation*>(obj.get());

        if(ptr != nullptr)
        {
            m_oColPageCount     = ptr->ccolPage;
            m_oFirstDataCol     = ptr->colFirstData;
            m_oFirstDataRow     = ptr->rwFirstData;
            m_oFirstHeaderRow   = ptr->rwFirstHead;
            m_oRowPageCount     = ptr->crwPage;

            if(!ptr->rfxGeom.toString().empty())
                m_oRef = ptr->rfxGeom.toString();
        }
    }
//------------------------------------
	void CPivotTableStyleInfo::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<pivotTableStyleInfo");
            WritingStringNullableAttrEncodeXmlString2(L"name",	m_oName);
			WritingStringNullableAttrBool2(L"showColHeaders",	m_oShowColHeaders);
			WritingStringNullableAttrBool2(L"showColStripes",	m_oShowColStripes);
            WritingStringNullableAttrBool2(L"showLastColumn",	m_oShowLastColumn);
			WritingStringNullableAttrBool2(L"showRowHeaders",	m_oShowRowHeaders);
			WritingStringNullableAttrBool2(L"showRowStripes",	m_oShowRowStripes);
		writer.WriteString(L"/>");
	}
    void CPivotTableStyleInfo::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::TableStyleClient*>(obj.get());

        if(ptr != nullptr)
        {
            m_oShowColHeaders = ptr->fColumnHeaders;
            m_oShowRowHeaders = ptr->fRowHeaders;
            m_oShowColStripes = ptr->fColumnStripes;
            m_oShowRowStripes = ptr->fRowStripes;
            m_oShowLastColumn = ptr->fLastColumn;

            if(!ptr->stStyleName.value().empty())
                m_oName     = ptr->stStyleName.value();
        }
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
    void CPivotCacheDefinitionFile::readBin(const CPath& oPath)
    {
        CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
        if (xlsb)
        {
            XLS::BaseObjectPtr pivotCacheDefStream(new XLSB::PivotCacheDefStream());

            xlsb->ReadBin(oPath, pivotCacheDefStream.get());

            if (pivotCacheDefStream != nullptr)
            {
                //XLS::BaseObjectPtr ptr(static_cast<XLS::BaseObject*>(pivotCacheDefStream.get()), NullDeleter());
                //XLS::BaseObjectPtr ptr = boost::shared_ptr<XLS::BaseObject>(static_cast<XLS::BaseObject*>(pivotCacheDefStream.get()));
                m_oPivotCashDefinition = pivotCacheDefStream;
            }

            //pivotCacheDefStream.reset();
        }
    }

	void CPivotCacheDefinitionFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;

        if( m_oReadPath.GetExtention() == _T(".bin"))
        {
            readBin(m_oReadPath);
        }
		else
		{
			XmlUtils::CXmlLiteReader oReader;

			if (!oReader.FromFile(oPath.GetPath()))
				return;

			if (!oReader.ReadNextNode())
				return;

			m_oPivotCashDefinition = oReader;
		}

        IFileContainer::Read( oRootPath, oPath ); //в данном случае порядок считывания важен для xlsb
	}
	void CPivotCacheDefinitionFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if (bIsWritten) return;

		bIsWritten = true;
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
		WritingStringNullableAttrBool2(L"invalid",			m_oInvalid);
		WritingStringNullableAttrBool2(L"saveData",			m_oSaveData);
		WritingStringNullableAttrBool2(L"refreshOnLoad",	m_oRefreshOnLoad);
		WritingStringNullableAttrBool2(L"optimizeMemory",	m_oOptimizeMemory);
		WritingStringNullableAttrBool2(L"enableRefresh",	m_oEnableRefresh);
		WritingStringNullableAttrEncodeXmlString2(L"refreshedBy", m_oRefreshedBy);
		WritingStringNullableAttrString(L"refreshedDateIso", m_oRefreshedDateIso, m_oRefreshedDateIso->ToString());
		WritingStringNullableAttrBool2(L"backgroundQuery",	m_oBackgroundQuery);
		WritingStringNullableAttrInt(L"missingItemsLimit",	m_oMissingItemsLimit, m_oMissingItemsLimit->GetValue());
		WritingStringNullableAttrInt(L"createdVersion",		m_oCreatedVersion, m_oCreatedVersion->GetValue());
		WritingStringNullableAttrInt(L"refreshedVersion",	m_oRefreshedVersion, m_oRefreshedVersion->GetValue());
		WritingStringNullableAttrInt(L"minRefreshableVersion", m_oMinRefreshableVersion, m_oMinRefreshableVersion->GetValue());
		WritingStringNullableAttrInt(L"recordCount",		m_oRecordCount, m_oRecordCount->GetValue());
		WritingStringNullableAttrBool2(L"upgradeOnRefresh", m_oUpgradeOnRefresh);
		WritingStringNullableAttrBool2(L"tupleCache",		m_oTupleCache);
		WritingStringNullableAttrBool2(L"supportSubquery",	m_oSupportSubquery);
		WritingStringNullableAttrBool2(L"supportAdvancedDrill", m_oSupportAdvancedDrill);
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
    void CPivotCacheDefinition::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PivotCacheDefStream*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPivotCacheDef);

            if(ptr->m_PCDFIELDS != nullptr)
                m_oCacheFields = ptr->m_PCDFIELDS;

            if(ptr->m_PCDSOURCE != nullptr)
                m_oCacheSource = ptr->m_PCDSOURCE;

            if(ptr->m_FRTPIVOTCACHEDEF != nullptr)
                m_oExtLst      = ptr->m_FRTPIVOTCACHEDEF;
        }
    }
    void CPivotCacheDefinition::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPivotCacheDef*>(obj.get());

        if(ptr != nullptr)
        {
            if(ptr->fBackgroundQuery)
                m_oBackgroundQuery          = ptr->fBackgroundQuery;

            if(!ptr->fEnableRefresh)
                m_oEnableRefresh            = ptr->fEnableRefresh;

            if(!ptr->stRelIDRecords.value.value().empty())
                m_oRid                      = ptr->stRelIDRecords.value.value();

            if(ptr->fInvalid)
                m_oInvalid                  = ptr->fInvalid;

            if(ptr->bVerCacheCreated != 0)
                m_oCreatedVersion           = ptr->bVerCacheCreated;

            m_oMinRefreshableVersion        = ptr->bVerCacheRefreshableMin;
            m_oMissingItemsLimit            = ptr->citmGhostMax;

            if(ptr->fOptimizeCache)
                m_oOptimizeMemory           = ptr->fOptimizeCache;

            if(ptr->fSaveData)
                m_oRecordCount              = ptr->cRecords;

            if(!ptr->stRefreshedWho.value().empty())
                m_oRefreshedBy              = ptr->stRefreshedWho.value();

            m_oRefreshedDateIso             = std::to_wstring(ptr->xnumRefreshedDate.data.value);

            if(ptr->bVerCacheLastRefresh != 0)
                m_oRefreshedVersion         = ptr->bVerCacheLastRefresh;

            if(ptr->fRefreshOnLoad)
                m_oRefreshOnLoad            = ptr->fRefreshOnLoad;

            if(!ptr->fSaveData)
                m_oSaveData                 = ptr->fSaveData;

            if(ptr->fSupportAttribDrill)
                m_oSupportAdvancedDrill     = ptr->fSupportAttribDrill;

            if(ptr->fSupportSubquery)
                m_oSupportSubquery          = ptr->fSupportSubquery;

            if(ptr->fSheetData)
                m_oTupleCache               = ptr->fSheetData;

            if(ptr->fUpgradeOnRefresh)
                m_oUpgradeOnRefresh         = ptr->fUpgradeOnRefresh;
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
    void CPivotCacheFields::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDFIELDS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arPCDFIELD.size();

            for(auto &item : ptr->m_arPCDFIELD)
            {
                m_arrItems.push_back(new CPivotCacheField(item));
            }
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
    void CPivotCacheField::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDFIELD*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCDField);

            if(ptr->m_PCDFATBL != nullptr)
                m_oSharedItems = ptr->m_PCDFATBL;

            if(ptr->m_PCDFGROUP != nullptr)
                m_oFieldGroup = ptr->m_PCDFGROUP;
        }
    }
    void CPivotCacheField::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPCDField*>(obj.get());

        if(ptr != nullptr)
        {
            if(!ptr->stFldName.value().empty())
                m_oName                     = ptr->stFldName.value();

            if(!ptr->stFldCaption.value().empty())
                m_oCaption                  = ptr->stFldCaption.value();

            if(!ptr->fSrcField)
                m_oDatabaseField            = ptr->fSrcField;

            if(ptr->fServerBased)
                m_oServerField              = ptr->fServerBased;

            if(!ptr->fldFmla.getAssembledFormula().empty())
                m_oFormula                  = ptr->fldFmla.getAssembledFormula();

            if(ptr->ihdb != 0)
                m_oHierarchy                = ptr->ihdb;

            if(ptr->fOlapMemPropField)
                m_oMemberPropertyField      = ptr->fOlapMemPropField;

            if(!ptr->stMemPropName.value().empty())
                m_oPropertyName             = ptr->stMemPropName.value();

            if(ptr->wTypeSql != 0)
                m_oSqlType                  = ptr->wTypeSql;

            if(ptr->fCantGetUniqueItems)
                m_oUniqueList               = !ptr->fCantGetUniqueItems;

            if(ptr->isxtl != 0)
                m_oLevel                    = ptr->isxtl;

            m_oMappingCount                 = ptr->cIsxtmps; //?
            m_oNumFmtId                     = ptr->ifmt;
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
    void CSharedItems::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDFATBL*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCDFAtbl);

            for(auto &item : boost::adaptors::reverse(ptr->m_arSource))
            {
                if(item->get_type() == XLS::typePCDI)
                {
                    auto ptrPCDI = static_cast<XLSB::PCDI*>(item.get());

                    if(ptrPCDI->m_source != nullptr)
                    {
                        switch(ptrPCDI->m_source->get_type())
                        {
                            case XLS::typePCDIBoolean:
                                m_arrItems.push_back(new CPivotBooleanValue(ptrPCDI->m_source));
                                break;
                            case XLS::typePCDIDatetime:
                                m_arrItems.push_back(new CPivotDateTimeValue(ptrPCDI->m_source));
                                break;
                            case XLS::typePCDIError:
                                m_arrItems.push_back(new CPivotErrorValue(ptrPCDI->m_source));
                                break;
                            case XLS::typePCDIMissing:
                                m_arrItems.push_back(new CPivotNoValue(ptrPCDI->m_source));
                                break;
                            case XLS::typePCDINumber:
                                m_arrItems.push_back(new CPivotNumericValue(ptrPCDI->m_source));
                                break;
                            case XLS::typePCDIString:
                                m_arrItems.push_back(new CPivotCharacterValue(ptrPCDI->m_source));
                                break;
                            case XLS::typePCDIRUN:
                                {
                                    auto ptrPCDIRUN      = static_cast<XLSB::PCDIRUN*>(ptrPCDI->m_source.get());
                                    auto ptrBeginPCDIRun = static_cast<XLSB::BeginPCDIRun*>(ptrPCDIRUN->m_BrtBeginPCDIRun.get());
                                    if(ptrBeginPCDIRun != nullptr)
                                    {
                                        for(size_t i = 0; i < ptrBeginPCDIRun->cItems; ++i)
                                        {
                                            switch (ptrBeginPCDIRun->mdSxoper)
                                            {
                                                case 0x0001:
                                                {
                                                    auto num = new CPivotNumericValue();
                                                    num->m_oValue = ptrBeginPCDIRun->rgPCDINumber[i].data.value;
                                                    m_arrItems.push_back(num);
                                                }
                                                break;

                                                case 0x0002:
                                                {
                                                    auto str = new CPivotCharacterValue();
                                                    str->m_oValue = ptrBeginPCDIRun->rgPCDIString[i].value();
                                                    m_arrItems.push_back(str);
                                                }
                                                break;

                                                case 0x0010:
                                                {
                                                    auto err = new CPivotErrorValue();

                                                    switch(ptrBeginPCDIRun->rgPCDIError[i])
                                                    {
                                                        case 0x00: err->m_oValue = L"#NULL!"; break;
                                                        case 0x07: err->m_oValue = L"#DIV/0!"; break;
                                                        case 0x0F: err->m_oValue = L"#VALUE!"; break;
                                                        case 0x17: err->m_oValue = L"#REF!"; break;
                                                        case 0x1D: err->m_oValue = L"#NAME?"; break;
                                                        case 0x24: err->m_oValue = L"#NUM!"; break;
                                                        case 0x2A: err->m_oValue = L"#N/A"; break;
                                                        case 0x2B: err->m_oValue = L"#GETTING_DATA"; break;
                                                    }

                                                    m_arrItems.push_back(err);
                                                }
                                                break;

                                                case 0x0020:
                                                {
                                                    auto datetime = new CPivotDateTimeValue();
                                                    datetime->m_oValue = ptrBeginPCDIRun->rgPCDIDatetime[i].value();
                                                    m_arrItems.push_back(datetime);
                                                }
                                                break;

                                            }
                                        }
                                    }
                                }
                                break;
                        }
                    }
                }
                else if(item->get_type() == XLS::typePCDIA)
                {
                    auto ptrPCDIA = static_cast<XLSB::PCDIA*>(item.get());

                    if(ptrPCDIA->m_source != nullptr)
                    {
                        switch(ptrPCDIA->m_source->get_type())
                        {
                            case XLS::typePCDIABoolean:
                                m_arrItems.push_back(new CPivotBooleanValue(ptrPCDIA->m_source));
                                break;
                            case XLS::typePCDIADatetime:
                                m_arrItems.push_back(new CPivotDateTimeValue(ptrPCDIA->m_source));
                                break;
                            case XLS::typePCDIAError:
                                m_arrItems.push_back(new CPivotErrorValue(ptrPCDIA->m_source));
                                break;
                            case XLS::typePCDIAMissing:
                                m_arrItems.push_back(new CPivotNoValue(ptrPCDIA->m_source));
                                break;
                            case XLS::typePCDIANumber:
                                m_arrItems.push_back(new CPivotNumericValue(ptrPCDIA->m_source));
                                break;
                            case XLS::typePCDIAString:
                                m_arrItems.push_back(new CPivotCharacterValue(ptrPCDIA->m_source));
                                break;
                        }
                    }
                }
            }
        }
    }
    void CSharedItems::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPCDFAtbl*>(obj.get());

        if(ptr != nullptr)
        {
            m_oContainsBlank            = ptr->fHasBlankItem;
            m_oContainsDate             = ptr->fDateInField;
            m_oContainsInteger          = ptr->fIntField;
            m_oContainsMixedTypes       = ptr->fMixedTypesIgnoringBlanks; //?
            m_oContainsNonDate          = ptr->fNonDates;
            m_oContainsNumber           = ptr->fNumField;
            m_oContainsSemiMixedTypes   = ptr->fTextEtcField; //?
            m_oContainsString           = ptr->fHasTextItem; //?
            m_oLongText                 = ptr->fHasLongTextItem;
            m_oCount                    = ptr->citems;

            if(ptr->fDateInField && !ptr->fMixedTypesIgnoringBlanks && ptr->fNumMinMaxValid)
            {
                m_oMinDate              = std::to_wstring(ptr->xnumMin.data.value);
                m_oMaxDate              = std::to_wstring(ptr->xnumMax.data.value);
            }
            else if(ptr->fNumField && ptr->fNumMinMaxValid)
            {
                m_oMinValue             = ptr->xnumMin.data.value;
                m_oMaxValue             = ptr->xnumMax.data.value;
            }
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
    void CDiscreteGroupingProperties::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDFGDISCRETE*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arBrtPCDIIndex.size();

            for(auto &item : ptr->m_arBrtPCDIIndex)
            {
                m_arrItems.push_back(new CSharedItemsIndex(item));
            }
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
    void COLAPGroupItems::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDFGITEMS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arPCDI.size();

            for(auto &item : ptr->m_arPCDI)
            {
                auto ptrPCDI = static_cast<XLSB::PCDI*>(item.get());

                if(ptrPCDI->m_source != nullptr)
                {
                    switch(ptrPCDI->m_source->get_type())
                    {
                        case XLS::typePCDIBoolean:
                            m_arrItems.push_back(new CPivotBooleanValue(ptrPCDI->m_source));
                            break;
                        case XLS::typePCDIDatetime:
                            m_arrItems.push_back(new CPivotDateTimeValue(ptrPCDI->m_source));
                            break;
                        case XLS::typePCDIError:
                            m_arrItems.push_back(new CPivotErrorValue(ptrPCDI->m_source));
                            break;
                        case XLS::typePCDIMissing:
                            m_arrItems.push_back(new CPivotNoValue(ptrPCDI->m_source));
                            break;
                        case XLS::typePCDINumber:
                            m_arrItems.push_back(new CPivotNumericValue(ptrPCDI->m_source));
                            break;
                        case XLS::typePCDIString:
                            m_arrItems.push_back(new CPivotCharacterValue(ptrPCDI->m_source));
                            break;
                        case XLS::typePCDIRUN:
                            {
                                auto ptrPCDIRUN      = static_cast<XLSB::PCDIRUN*>(ptrPCDI->m_source.get());
                                auto ptrBeginPCDIRun = static_cast<XLSB::BeginPCDIRun*>(ptrPCDIRUN->m_BrtBeginPCDIRun.get());
                                if(ptrBeginPCDIRun != nullptr)
                                {
                                    for(size_t i = 0; i < ptrBeginPCDIRun->cItems; ++i)
                                    {
                                        switch (ptrBeginPCDIRun->mdSxoper)
                                        {
                                            case 0x0001:
                                            {
                                                auto num = new CPivotNumericValue();
                                                num->m_oValue = ptrBeginPCDIRun->rgPCDINumber[i].data.value;
                                                m_arrItems.push_back(num);
                                            }
                                            break;

                                            case 0x0002:
                                            {
                                                auto str = new CPivotCharacterValue();
                                                str->m_oValue = ptrBeginPCDIRun->rgPCDIString[i].value();
                                                m_arrItems.push_back(str);
                                            }
                                            break;

                                            case 0x0010:
                                            {
                                                auto err = new CPivotErrorValue();

                                                switch(ptrBeginPCDIRun->rgPCDIError[i])
                                                {
                                                    case 0x00: err->m_oValue = L"#NULL!"; break;
                                                    case 0x07: err->m_oValue = L"#DIV/0!"; break;
                                                    case 0x0F: err->m_oValue = L"#VALUE!"; break;
                                                    case 0x17: err->m_oValue = L"#REF!"; break;
                                                    case 0x1D: err->m_oValue = L"#NAME?"; break;
                                                    case 0x24: err->m_oValue = L"#NUM!"; break;
                                                    case 0x2A: err->m_oValue = L"#N/A"; break;
                                                    case 0x2B: err->m_oValue = L"#GETTING_DATA"; break;
                                                }

                                                m_arrItems.push_back(err);
                                            }
                                            break;

                                            case 0x0020:
                                            {
                                                auto datetime = new CPivotDateTimeValue();
                                                datetime->m_oValue = ptrBeginPCDIRun->rgPCDIDatetime[i].value();
                                                m_arrItems.push_back(datetime);
                                            }
                                            break;

                                        }
                                    }
                                }
                            }
                            break;
                    }
                }
            }
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
    void CRangeGroupingProperties::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDFGRANGE*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCDFGRange);
        }
    }
    void CRangeGroupingProperties::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPCDFGRange*>(obj.get());

        if(ptr != nullptr)
        {
            m_oAutoStart     = ptr->fAutoStart;
            m_oAutoEnd       = ptr->fAutoEnd;
            m_oGroupInterval = ptr->xnumBy.data.value;

            if(ptr->fDates)
            {
                m_oStartDate = std::to_wstring(ptr->xnumStart.data.value);
                m_oEndDate   = std::to_wstring(ptr->xnumEnd.data.value);
            }
            else
            {
                m_oStartNum  = ptr->xnumStart.data.value;
                m_oEndNum    = ptr->xnumEnd.data.value;
            }
            switch(ptr->iByType)
            {
                case 0x00:
                    m_oGroupBy = SimpleTypes::Spreadsheet::EValuesGroupBy::groupByNumericRanges;
                    break;
                case 0x01:
                    m_oGroupBy = SimpleTypes::Spreadsheet::EValuesGroupBy::groupBySeconds;
                    break;
                case 0x02:
                    m_oGroupBy = SimpleTypes::Spreadsheet::EValuesGroupBy::groupByMinutes;
                    break;
                case 0x03:
                    m_oGroupBy = SimpleTypes::Spreadsheet::EValuesGroupBy::groupByHours;
                    break;
                case 0x04:
                    m_oGroupBy = SimpleTypes::Spreadsheet::EValuesGroupBy::groupByDays;
                    break;
                case 0x05:
                    m_oGroupBy = SimpleTypes::Spreadsheet::EValuesGroupBy::groupByMonths;
                    break;
                case 0x06:
                    m_oGroupBy = SimpleTypes::Spreadsheet::EValuesGroupBy::groupByQuarters;
                    break;
                case 0x07:
                    m_oGroupBy = SimpleTypes::Spreadsheet::EValuesGroupBy::groupByYears;
                    break;
            }
        }
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
			if (m_arrItems.size() > 0)
			{
				WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			}
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
    void CPivotCharacterValue::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
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
    void CPivotCharacterValue::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        if(obj->get_type() == XLS::typePCDIString)
        {
            auto ptr = static_cast<XLSB::PCDIString*>(obj.get());

            if(ptr != nullptr)
            {
                m_oValue        = ptr->st.value();

				if (ptr->sxvcellextra)
				{
					m_oBold = ptr->sxvcellextra->fSrvFmtBold;
					m_oItalic = ptr->sxvcellextra->fSrvFmtItalic;
					m_oStrike = ptr->sxvcellextra->fSrvFmtStrikethrough;
					m_oUnderline = ptr->sxvcellextra->fSrvFmtUnderline;

					if (ptr->sxvcellextra->fSrvFmtNum)
						m_oFormatIndex = ptr->sxvcellextra->isfci;

					if (ptr->sxvcellextra->fSrvFmtBack)
						m_oBackColor = ptr->sxvcellextra->cvBack.toHex();

					if (ptr->sxvcellextra->fSrvFmtFore)
						m_oForeColor = ptr->sxvcellextra->cvFore.toHex();
				}
            }
        }
        else if(obj->get_type() == XLS::typePCDIAString)
        {
            auto ptr = static_cast<XLSB::PCDIAString*>(obj.get());

            if(ptr != nullptr)
            {
                m_oValue       = ptr->st.value();

                if(!ptr->info.stCaption.value().empty())
                    m_oCaption = ptr->info.stCaption.value();

                m_oCalculated  = ptr->info.fFmla;
                m_oUnused      = ptr->info.fGhost;
                m_oCount       = ptr->info.cIMemProps;

                for(auto item : ptr->info.rgIMemProps)
                {
                    auto index = new CMemberPropertyIndex();
                    index->m_oV = item;
                    m_arrItems.push_back(index);
                }
            }
        }
    }
//------------------------------------
	void CPivotErrorValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<e");
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			WritingStringNullableAttrEncodeXmlString2(L"v",	m_oValue);
			if (m_arrItems.size() > 0)
			{
				WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			}
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
    void CPivotErrorValue::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
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
    void CPivotErrorValue::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        if(obj->get_type() == XLS::typePCDIError)
        {
            auto ptr = static_cast<XLSB::PCDIError*>(obj.get());

            if(ptr != nullptr)
            {
                switch(ptr->err)
                {
                    case 0x00: m_oValue = L"#NULL!"; break;
                    case 0x07: m_oValue = L"#DIV/0!"; break;
                    case 0x0F: m_oValue = L"#VALUE!"; break;
                    case 0x17: m_oValue = L"#REF!"; break;
                    case 0x1D: m_oValue = L"#NAME?"; break;
                    case 0x24: m_oValue = L"#NUM!"; break;
                    case 0x2A: m_oValue = L"#N/A"; break;
                    case 0x2B: m_oValue = L"#GETTING_DATA"; break;
                }
				if (ptr->sxvcellextra)
				{
					m_oBold = ptr->sxvcellextra->fSrvFmtBold;
					m_oItalic = ptr->sxvcellextra->fSrvFmtItalic;
					m_oStrike = ptr->sxvcellextra->fSrvFmtStrikethrough;
					m_oUnderline = ptr->sxvcellextra->fSrvFmtUnderline;

					if (ptr->sxvcellextra->fSrvFmtNum)
						m_oFormatIndex = ptr->sxvcellextra->isfci;

					if (ptr->sxvcellextra->fSrvFmtBack)
						m_oBackColor = ptr->sxvcellextra->cvBack.toHex();

					if (ptr->sxvcellextra->fSrvFmtFore)
						m_oForeColor = ptr->sxvcellextra->cvFore.toHex();
				}
            }
        }

        else if(obj->get_type() == XLS::typePCDIAError)
        {
            auto ptr = static_cast<XLSB::PCDIAError*>(obj.get());

            if(ptr != nullptr)
            {
                switch(ptr->err)
                {
                    case 0x00: m_oValue = L"#NULL!"; break;
                    case 0x07: m_oValue = L"#DIV/0!"; break;
                    case 0x0F: m_oValue = L"#VALUE!"; break;
                    case 0x17: m_oValue = L"#REF!"; break;
                    case 0x1D: m_oValue = L"#NAME?"; break;
                    case 0x24: m_oValue = L"#NUM!"; break;
                    case 0x2A: m_oValue = L"#N/A"; break;
                    case 0x2B: m_oValue = L"#GETTING_DATA"; break;
                }

                if(!ptr->info.stCaption.value().empty())
                    m_oCaption = ptr->info.stCaption.value();

                m_oCalculated  = ptr->info.fFmla;
                m_oUnused      = ptr->info.fGhost;
                m_oCount       = ptr->info.cIMemProps;

                for(auto item : ptr->info.rgIMemProps)
                {
                    auto index = new CMemberPropertyIndex();
                    index->m_oV = item;
                    m_arrItems.push_back(index);
                }
            }
        }
    }
//------------------------------------
	void CPivotNumericValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<n");
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			WritingStringNullableAttrDouble2(L"v",	m_oValue);
			if (m_arrItems.size() > 0)
			{
				WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			}
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
    void CPivotNumericValue::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
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
    void CPivotNumericValue::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        if(obj->get_type() == XLS::typePCDINumber)
        {
            auto ptr = static_cast<XLSB::PCDINumber*>(obj.get());

            if(ptr != nullptr)
            {
                m_oValue = ptr->xnum.data.value;

				if (ptr->sxvcellextra)
				{
					m_oBold = ptr->sxvcellextra->fSrvFmtBold;
					m_oItalic = ptr->sxvcellextra->fSrvFmtItalic;
					m_oStrike = ptr->sxvcellextra->fSrvFmtStrikethrough;
					m_oUnderline = ptr->sxvcellextra->fSrvFmtUnderline;

					if (ptr->sxvcellextra->fSrvFmtNum)
						m_oFormatIndex = ptr->sxvcellextra->isfci;

					if (ptr->sxvcellextra->fSrvFmtBack)
						m_oBackColor = ptr->sxvcellextra->cvBack.toHex();

					if (ptr->sxvcellextra->fSrvFmtFore)
						m_oForeColor = ptr->sxvcellextra->cvFore.toHex();
				}
            }
        }

        else if(obj->get_type() == XLS::typePCDIANumber)
        {
            auto ptr = static_cast<XLSB::PCDIANumber*>(obj.get());

            if(ptr != nullptr)
            {
                m_oValue = ptr->xnum.data.value;

                if(!ptr->info.stCaption.value().empty())
                    m_oCaption = ptr->info.stCaption.value();

                m_oCalculated  = ptr->info.fFmla;
                m_oUnused      = ptr->info.fGhost;
                m_oCount       = ptr->info.cIMemProps;

                for(auto item : ptr->info.rgIMemProps)
                {
                    auto index = new CMemberPropertyIndex();
                    index->m_oV = item;
                    m_arrItems.push_back(index);
                }
            }
        }
    }
//------------------------------------
	void CPivotDateTimeValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<d");
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			WritingStringNullableAttrString(L"v",	m_oValue, m_oValue->ToString());
			if (m_arrItems.size() > 0)
			{
				WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			}
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
    void CPivotDateTimeValue::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
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
    void CPivotDateTimeValue::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        if(obj->get_type() == XLS::typePCDIDatetime)
        {
            auto ptr = static_cast<XLSB::PCDIDatetime*>(obj.get());

            if(ptr != nullptr)
            {
                m_oValue = ptr->datetime.value();
            }
        }
        else if(obj->get_type() == XLS::typePCDIADatetime)
        {
            auto ptr = static_cast<XLSB::PCDIADatetime*>(obj.get());

            if(ptr != nullptr)
            {
                m_oValue = ptr->datetime.value();

                if(!ptr->info.stCaption.value().empty())
                    m_oCaption = ptr->info.stCaption.value();

                m_oCalculated  = ptr->info.fFmla;
                m_oUnused      = ptr->info.fGhost;
                m_oCount       = ptr->info.cIMemProps;

                for(auto item : ptr->info.rgIMemProps)
                {
                    auto index = new CMemberPropertyIndex();
                    index->m_oV = item;
                    m_arrItems.push_back(index);
                }
            }
        }
    }
//------------------------------------
	void CPivotBooleanValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<b");
			WritingStringNullableAttrBool2(L"v",	m_oValue);
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			if (m_arrItems.size() > 0)
			{
				WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			}
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
    void CPivotBooleanValue::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
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
    void CPivotBooleanValue::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        if(obj->get_type() == XLS::typePCDIBoolean)
        {
            auto ptr = static_cast<XLSB::PCDIBoolean*>(obj.get());

            if(ptr != nullptr)
            {
                m_oValue = (bool)ptr->f;
            }
        }
        else if(obj->get_type() == XLS::typePCDIABoolean)
        {
            auto ptr = static_cast<XLSB::PCDIABoolean*>(obj.get());

            if(ptr != nullptr)
            {
                m_oValue = (bool)ptr->f;

                if(!ptr->info.stCaption.value().empty())
                    m_oCaption = ptr->info.stCaption.value();

                m_oCalculated  = ptr->info.fFmla;
                m_oUnused      = ptr->info.fGhost;
                m_oCount       = ptr->info.cIMemProps;

                for(auto item : ptr->info.rgIMemProps)
                {
                    auto index = new CMemberPropertyIndex();
                    index->m_oV = item;
                    m_arrItems.push_back(index);
                }
            }
        }
    }
//------------------------------------
	void CPivotNoValue::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<m");
			WritingStringNullableAttrEncodeXmlString2(L"c", m_oCaption);
			if (m_arrItems.size() > 0)
			{
				WritingStringAttrInt(L"cp", (int)m_arrItems.size());
			}
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
    void CPivotNoValue::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
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
    void CPivotNoValue::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        if(obj->get_type() == XLS::typePCDIMissing)
        {
            auto ptr = static_cast<XLSB::PCDIMissing*>(obj.get());

            if(ptr != nullptr)
            {
				if (ptr->sxvcellextra)
				{
					m_oBold = ptr->sxvcellextra->fSrvFmtBold;
					m_oItalic = ptr->sxvcellextra->fSrvFmtItalic;
					m_oStrike = ptr->sxvcellextra->fSrvFmtStrikethrough;
					m_oUnderline = ptr->sxvcellextra->fSrvFmtUnderline;

					if (ptr->sxvcellextra->fSrvFmtNum)
						m_oFormatIndex = ptr->sxvcellextra->isfci;

					if (ptr->sxvcellextra->fSrvFmtBack)
						m_oBackColor = ptr->sxvcellextra->cvBack.toHex();

					if (ptr->sxvcellextra->fSrvFmtFore)
						m_oForeColor = ptr->sxvcellextra->cvFore.toHex();
				}
            }
        }

        else if(obj->get_type() == XLS::typePCDIAMissing)
        {
            auto ptr = static_cast<XLSB::PCDIAMissing*>(obj.get());

            if(ptr != nullptr)
            {
                if(!ptr->info.stCaption.value().empty())
                    m_oCaption = ptr->info.stCaption.value();

                m_oCalculated  = ptr->info.fFmla;
                m_oUnused      = ptr->info.fGhost;
                m_oCount       = ptr->info.cIMemProps;

                for(auto item : ptr->info.rgIMemProps)
                {
                    auto index = new CMemberPropertyIndex();
                    index->m_oV = item;
                    m_arrItems.push_back(index);
                }
            }
        }
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
    void CPivotCacheSource::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDSOURCE*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCDSource);

            if(ptr->m_PCDSRANGE != nullptr)
                m_oWorksheetSource = ptr->m_PCDSRANGE;

            if(ptr->m_PCDSCONSOL != nullptr)
                m_oConsolidation = ptr->m_PCDSCONSOL;
        }
    }
    void CPivotCacheSource::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPCDSource*>(obj.get());

        if(ptr != nullptr)
        {
            switch(ptr->iSrcType)
            {
                case 0x00000000:
                    m_oType = SimpleTypes::Spreadsheet::ESourceCacheType::typeSourceWorksheet;
                    break;
                case 0x00000001:
                    m_oType = SimpleTypes::Spreadsheet::ESourceCacheType::typeSourceExternal;
                    break;
                case 0x00000002:
                    m_oType = SimpleTypes::Spreadsheet::ESourceCacheType::typeSourceConsolidation;
                    break;
                case 0x00000003:
                    m_oType = SimpleTypes::Spreadsheet::ESourceCacheType::typeSourceScenario;
                    break;
            }

            m_oConnectionId = ptr->dwConnID;

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
    void CWorksheetSource::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDSRANGE*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCDSRange);
        }
    }
    void CWorksheetSource::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPCDSRange*>(obj.get());

        if(ptr != nullptr)
        {
            if(!ptr->sheetName.value().empty())
                m_oSheet     = ptr->sheetName.value();

            if(!ptr->range.toString().empty())
                m_oRef       = ptr->range.toString();

            if(!ptr->namedRange.value().empty())
                m_oName      = ptr->namedRange.value();

            if(!ptr->relId.value.value().empty())
                m_oRid       = ptr->relId.value.value();
        }
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
    void CPageItemValues::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDSCPAGES*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arPCDSCPAGE.size();

            for(auto &item : ptr->m_arPCDSCPAGE)
            {
                m_arrItems.push_back(new CPageItems(item));
            }
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
    void CPageItems::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDSCPAGE*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arPCDSCPITEM.size();

            for(auto &item : ptr->m_arPCDSCPITEM)
            {
                m_arrItems.push_back(new CPageItem(item));
            }
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
    void CPageItem::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDSCPITEM*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCDSCPItem);
        }
    }
    void CPageItem::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPCDSCPItem*>(obj.get());

        if(ptr != nullptr)
        {
            if(!ptr->stName.value().empty())
                m_oName = ptr->stName.value();
        }
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
    void CRangeSets::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDSCSETS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arPCDSCSET.size();

            for(auto &item : ptr->m_arPCDSCSET)
            {
                m_arrItems.push_back(new CRangeSet(item));
            }
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
    void CRangeSet::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDSCSET*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCDSCSet);
        }
    }
    void CRangeSet::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPCDSCSet*>(obj.get());

        if(ptr != nullptr)
        {
            if(!ptr->irstSheet.value().empty())
                m_oSheet            = ptr->irstSheet.value();

            if(!ptr->rfx.toString().empty())
                m_oRef              = ptr->rfx.toString();

            if(!ptr->irstName.value().empty())
                m_oName             = ptr->irstName.value();

            if(!ptr->irstRelId.value.value().empty())
                m_oRid              = ptr->irstRelId.value.value();

            m_oI1                   = ptr->rgiItem[0];
            m_oI2                   = ptr->rgiItem[1];
            m_oI3                   = ptr->rgiItem[2];
            m_oI4                   = ptr->rgiItem[3];
        }
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
    void CConsolidationSource::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDSCONSOL*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCDSConsol);

            if(ptr->m_PCDSCPAGES != nullptr)
                m_oPages = ptr->m_PCDSCPAGES;

            if(ptr->m_PCDSCSETS != nullptr)
                m_oRangeSets = ptr->m_PCDSCSETS;
        }
    }
    void CConsolidationSource::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPCDSConsol*>(obj.get());

        if(ptr != nullptr)
        {
            if(!ptr->fAutoPage)
                m_oAutoPage = ptr->fAutoPage;
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
    void CFieldGroupProperties::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDFGROUP*>(obj.get());

        if(ptr != nullptr)
        {
            ReadAttributes(ptr->m_BrtBeginPCDFGroup);

            if(ptr->m_PCDFGDISCRETE != nullptr)
                m_oDiscretePr = ptr->m_PCDFGDISCRETE;

            if(ptr->m_PCDFGRANGE != nullptr)
                m_oRangePr = ptr->m_PCDFGRANGE;

            if(ptr->m_PCDFGITEMS != nullptr)
                m_oGroupItems = ptr->m_PCDFGITEMS;
        }
    }
    void CFieldGroupProperties::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginPCDFGroup*>(obj.get());

        if(ptr != nullptr)
        {
            m_oPar                 = ptr->ifdbParent;
            m_oBase                = ptr->ifdbBase;
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
    void CPivotCacheRecordsFile::readBin(const CPath& oPath)
    {
        CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
        if (xlsb)
        {
            XLSB::PivotCacheRecordsStreamPtr pivotCacheRecordsStream(new XLSB::PivotCacheRecordsStream);

            xlsb->ReadBin(oPath, pivotCacheRecordsStream.get());

            if (pivotCacheRecordsStream != nullptr)
            {
                if (pivotCacheRecordsStream->m_PIVOTCACHERECORDS != nullptr)
                    m_oPivotCacheRecords = pivotCacheRecordsStream->m_PIVOTCACHERECORDS;
            }

            //pivotCacheRecordsStream.reset();
        }
    }
	void CPivotCacheRecordsFile::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );

        if( m_oReadPath.GetExtention() == _T(".bin"))
        {
            readBin(m_oReadPath);
            return;
        }
		NSFile::CFileBinary::ReadAllBytes(oPath.GetPath(), &m_pData, m_nDataLength);
		
		return;

		//XmlUtils::CXmlLiteReader oReader;

		//if ( !oReader.FromFile( oPath.GetPath() ) )
		//	return;

		//if ( !oReader.ReadNextNode() )
		//	return;

		//m_oPivotCacheRecords = oReader;
	}
	void CPivotCacheRecordsFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		if(m_oPivotCacheRecords.IsInit())
		{
			std::wstring sPath = oPath.GetPath();

			if (false == m_oPivotCacheRecords->m_strOutputXml.empty())
			{
				NSFile::CFileBinary::SaveToFile(sPath, m_oPivotCacheRecords->m_strOutputXml);
			}
			else
			{
				NSStringUtils::CStringBuilder sXml;
				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
				m_oPivotCacheRecords->toXML(sXml);
				NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());
			}
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

		for (size_t i = 0; i < m_arrItems.size(); ++i)
		{
			if (m_arrItems[i])
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
    void CPivotCacheRecords::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PIVOTCACHERECORDS*>(obj.get());

        if(ptr != nullptr)
        {			
			NSStringUtils::CStringBuilder writer;
			writer.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			writer.WriteString(L"<pivotCacheRecords \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr16\" \
xmlns:xr16=\"http://schemas.microsoft.com/office/spreadsheetml/2017/revision16\"");
			WritingStringAttrInt(L"count", ptr->m_arPIVOTCACHERECORD.size());
			writer.WriteString(L">");

			//m_oCount = (_UINT32)ptr->m_arPIVOTCACHERECORD.size();

            //for(auto &item : ptr->m_arPIVOTCACHERECORD)
			while(false == ptr->m_arPIVOTCACHERECORD.empty())
            {
                //m_arrItems.push_back(new CPivotCacheRecord(item));
				
				CPivotCacheRecord xmlItem(ptr->m_arPIVOTCACHERECORD.front());
				xmlItem.toXML(writer);
				ptr->m_arPIVOTCACHERECORD.erase(ptr->m_arPIVOTCACHERECORD.begin());
            }
			ptr->m_arPIVOTCACHERECORD.clear();

			writer.WriteString(L"</pivotCacheRecords>");
			m_strOutputXml = writer.GetData();
			writer.Clear();
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
    void CPivotCacheRecord::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PIVOTCACHERECORD*>(obj.get());

        if(ptr != nullptr)
        {
            if(ptr->m_source != nullptr)
            {
                if(ptr->m_source->get_type() == XLS::typePIVOTCACHERECORDDT)
                {
                    auto ptr1 = static_cast<XLSB::PIVOTCACHERECORDDT*>(ptr->m_source.get());

                    if(ptr1 != nullptr)
                    {
                        for(auto &item : ptr1->m_arPCDIDT)
                        {
                            auto ptrPCDIDT = static_cast<XLSB::PCDIDT*>(item.get());
                            if(ptrPCDIDT->m_source != nullptr)
                            {
                                switch(ptrPCDIDT->m_source->get_type())
                                {
                                    case XLS::typePCDIBoolean:
                                        m_arrItems.push_back(new CPivotBooleanValue(ptrPCDIDT->m_source));
                                        break;
                                    case XLS::typePCDIDatetime:
                                        m_arrItems.push_back(new CPivotDateTimeValue(ptrPCDIDT->m_source));
                                        break;
                                    case XLS::typePCDIError:
                                        m_arrItems.push_back(new CPivotErrorValue(ptrPCDIDT->m_source));
                                        break;
                                    case XLS::typePCDIMissing:
                                        m_arrItems.push_back(new CPivotNoValue(ptrPCDIDT->m_source));
                                        break;
                                    case XLS::typePCDINumber:
                                        m_arrItems.push_back(new CPivotNumericValue(ptrPCDIDT->m_source));
                                        break;
                                    case XLS::typePCDIString:
                                        m_arrItems.push_back(new CPivotCharacterValue(ptrPCDIDT->m_source));
                                        break;
                                    case XLS::typePCDIIndex:
                                        m_arrItems.push_back(new CSharedItemsIndex(ptrPCDIDT->m_source));
                                        break;
                                }
                            }
                        }
                    }
                }
                else if(ptr->m_source->get_type() == XLS::typePCRRecord)
                {
                    auto ptr1 = static_cast<XLSB::PCRRecord*>(ptr->m_source.get());

                    if(ptr1 != nullptr)
                    {
                        for(const auto& item : ptr1->data)
                        switch(item.first)
                        {
                            case XLS::typePCDIIndex:
                            {
                                auto oSharedItemsIndex = new CSharedItemsIndex();
                                oSharedItemsIndex->m_oV = *boost::any_cast<_UINT32>(&item.second);
                                m_arrItems.push_back(oSharedItemsIndex);
                                break;
                            }
                            case XLS::typePCDINumber:
                            {
                                auto oCPivotNumericValue = new CPivotNumericValue();
                                oCPivotNumericValue->m_oValue = *boost::any_cast<double>(&item.second);
                                m_arrItems.push_back(oCPivotNumericValue);
                                break;
                            }
                            case XLS::typePCDIDatetime:
                            {
                                auto oCPivotDateTimeValue = new CPivotDateTimeValue();
                                oCPivotDateTimeValue->m_oValue = *boost::any_cast<std::wstring>(&item.second);
                                m_arrItems.push_back(oCPivotDateTimeValue);
                                break;
                            }
                            case XLS::typePCDIString:
                            {
                                auto oCPivotCharacterValue = new CPivotCharacterValue();
                                oCPivotCharacterValue->m_oValue = *boost::any_cast<std::wstring>(&item.second);
                                m_arrItems.push_back(oCPivotCharacterValue);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    void CSharedItemsIndex::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        if(obj->get_type() == XLS::typePCDIIndex)
        {
            auto ptr = static_cast<XLSB::PCDIIndex*>(obj.get());

            if(ptr != nullptr)
            {
                m_oV = ptr->iitem;
            }
        }
        else if(obj->get_type() == XLS::typeBeginPRFItem)
        {
            auto ptr = static_cast<XLSB::BeginPRFItem*>(obj.get());

            if(ptr != nullptr)
            {
                m_oV = ptr->iitem;
            }
        }
    }
} //Spreadsheet
} // namespace OOX
