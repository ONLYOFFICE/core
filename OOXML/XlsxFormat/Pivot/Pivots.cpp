/*
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
#include "PivotTable.h"
#include "PivotCacheDefinition.h"
#include "PivotCacheRecords.h"

#include "../../XlsbFormat/Xlsb.h"
#include "../../XlsbFormat/PivotCacheRecordsStream.h"
#include "../../XlsbFormat/PivotTableStream.h"
#include "../../XlsbFormat/PivotCacheDefStream.h"
#include "../../XlsbFormat/Biff12_unions/PIVOTCACHERECORDS.h"
#include "../../XlsbFormat/Biff12_records/BeginPivotCacheRecords.h"
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
#include "../../XlsbFormat/Biff12_records/BeginSXVDs.h"
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
#include "../../XlsbFormat/Biff12_records/BeginSXLIRws.h"
#include "../../XlsbFormat/Biff12_unions/SXLICOLS.h"
#include "../../XlsbFormat/Biff12_records/BeginSXLICols.h"
#include "../../XlsbFormat/Biff12_unions/SXLI.h"
#include "../../XlsbFormat/Biff12_unions/ISXVIS.h"
#include "../../XlsbFormat/Biff12_records/BeginSXLI.h"
#include "../../XlsbFormat/Biff12_records/BeginISXVIs.h"
#include "../../XlsbFormat/Biff12_unions/SXDIS.h"
#include "../../XlsbFormat/Biff12_unions/SXDI.h"
#include "../../XlsbFormat/Biff12_records/BeginSXDI.h"
#include "../../XlsbFormat/Biff12_records/BeginSXDIs.h"
#include "../../XlsbFormat/Biff12_unions/SXFORMATS.h"
#include "../../XlsbFormat/Biff12_unions/SXFORMAT.h"
#include "../../XlsbFormat/Biff12_records/BeginSXFormat.h"
#include "../../XlsbFormat/Biff12_records/TableStyleClient.h"
#include "../../XlsbFormat/Biff12_unions/SXPIS.h"
#include "../../XlsbFormat/Biff12_unions/SXPI.h"
#include "../../XlsbFormat/Biff12_records/BeginSXPI.h"
#include "../../XlsbFormat/Biff12_records/BeginPivotCacheDef.h"
#include "../../XlsbFormat/Biff12_unions/PCDFIELDS.h"
#include "../../XlsbFormat/Biff12_records/BeginPCDFields.h"
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
#include "../../XlsbFormat/Biff12_unions/FRTSXVIEW.h"
#include "../../XlsbFormat/Biff12_unions/SXVIEW14.h"
#include "../../XlsbFormat/Biff12_records/BeginSXView14.h"
#include "../../XlsbFormat/Biff12_unions/PNAMES.h"
#include "../../XlsbFormat/Biff12_unions/PNAME.h"

#include <boost/range/adaptor/reversed.hpp>

#include "../../DocxFormat/Drawing/DrawingExt.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Word.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"
#include "../ComplexTypes_Spreadsheet.h"

#include "../../Binary/XlsbFormat/FileTypes_SpreadsheetBin.h"

#include <codecvt>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "../../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheReader.h"
#include "../../../MsBinaryFile/XlsFile/Format/Binary/CFStreamCacheWriter.h"
namespace OOX
{
namespace Spreadsheet
{

	std::wstring getDateFromExcelTime(double excelDate)
	{
		boost::gregorian::date date(1899, boost::gregorian::Dec, 30);
		XLSB::PCDIDateTime datetime;
		_UINT64 days = std::floor(excelDate);
		excelDate -= days;
		date += boost::gregorian::date_duration(days);
		datetime.yr = date.year();
		datetime.mon = date.month();
		datetime.dom = date.day();
		if(excelDate > 0)
		{
			excelDate *= 24;
			datetime.hr = std::floor(excelDate);
			excelDate -= datetime.hr;
			excelDate *= 60;
			datetime.min = std::floor(excelDate);
			excelDate -= datetime.min;
			datetime.sec = std::floor(excelDate*60);
		}
		return datetime.value();
	
	}

	double getExcelTimeFromDate(std::wstring Date)
	{
        boost::gregorian::date StartDate(1899, boost::gregorian::Dec, 30);
		XLSB::PCDIDateTime datetime;
		datetime.fromString(Date);
        boost::gregorian::date date(datetime.yr + 1900, datetime.mon + 1, datetime.dom);
		auto duration = date - StartDate;
        return duration.days();
	}

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
	XLS::BaseObjectPtr CPivotTableFile::WriteBin() const
	{
		return m_oPivotTableDefinition->toBin();
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
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
        if ((xlsb) && (xlsb->m_bWriteToXlsb) && m_oPivotTableDefinition.IsInit())
		{
			XLS::BaseObjectPtr object = WriteBin();
			xlsb->WriteBin(oPath, object.get());
		}
		else
		{
			if(m_oPivotTableDefinition.IsInit())
			{
				NSStringUtils::CStringBuilder sXml;

				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
				m_oPivotTableDefinition->toXML(sXml);

				std::wstring sPath = oPath.GetPath();
				NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());
			}
			else if(m_nDataLength > 0 && m_pData)
			{
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(oPath.GetPath());
				oFile.WriteFile(m_pData, m_nDataLength);
				oFile.CloseFile();
			}
		}
		oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		IFileContainer::Write( oPath, oDirectory, oContent );
	}
		const OOX::FileType CPivotTableFile::type() const
		{
			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if ((xlsb) && (xlsb->m_bWriteToXlsb))
			{
				return OOX::SpreadsheetBin::FileTypes::PivotTableBin;
			}
				return OOX::Spreadsheet::FileTypes::PivotTable;
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
        if(m_oPivotHierarchies.IsInit())
            m_oPivotHierarchies->toXML(writer);
		if(m_oPivotTableStyleInfo.IsInit())
			m_oPivotTableStyleInfo->toXML(writer);

		//if(m_oFilters.IsInit())
		//	m_oFilters->toXML(writer);
        if(m_oRowHierarchiesUsage.IsInit())
            m_oRowHierarchiesUsage->toXML(writer);
        if(m_oColHierarchiesUsage.IsInit())
            m_oColHierarchiesUsage->toXML(writer);

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
            else if (L"pivotHierarchies" == sName)		m_oPivotHierarchies = oReader;
			//else if (L"filters" == sName)				m_oFilters = oReader;
			//else if (L"chartFormats" == sName)		m_oChartFormats = oReader;

			//else if (L"conditionalFormats" == sName)	m_oConditionalFormats = oReader;
			else if (L"extLst" == sName)				m_oExtLst = oReader;
            else if (L"colHierarchiesUsage" == sName)
            {
                m_oColHierarchiesUsage = oReader;
                if(m_oColHierarchiesUsage.IsInit())
                    m_oColHierarchiesUsage->m_oRowHierarchy = false;
            }
            else if (L"rowHierarchiesUsage" == sName)
            {
                m_oRowHierarchiesUsage = oReader;
                if(m_oRowHierarchiesUsage.IsInit())
                    m_oRowHierarchiesUsage->m_oRowHierarchy = true;
            }
		}
	}
	XLS::BaseObjectPtr CPivotTableDefinition::toBin()
	{
		auto ptr(new XLSB::PivotTableStream);
		XLS::BaseObjectPtr objectPtr(ptr);
		ptr->m_BrtBeginSXView = writeAttributes();
		if(m_oLocation.IsInit())
			ptr->m_SXLOCATION = m_oLocation->toBin();
        if(m_oPivotFields.IsInit())
            ptr->m_SXVDS = m_oPivotFields->toBin();
        if(m_oRowFields.IsInit())
            ptr->m_ISXVDRWS = m_oRowFields->toBinRows();
        if(m_oColFields.IsInit())
            ptr->m_ISXVDCOLS = m_oColFields->toBinCols();
        if(m_oRowItems.IsInit())
            ptr->m_SXLIRWS = m_oRowItems->toBinRows();
        if(m_oColItems.IsInit())
            ptr->m_SXLICOLS = m_oColItems->toBinCols();

        if(m_oDataFields.IsInit())
            ptr->m_SXDIS = m_oDataFields->toBin();
        if(m_oFormats.IsInit())
            ptr->m_SXFORMATS = m_oFormats->toBin();
        if(m_oPivotTableStyleInfo.IsInit())
            ptr->m_BrtTableStyleClient = m_oPivotTableStyleInfo->toBin();
        if(m_oPageFields.IsInit())
            ptr->m_SXPIS = m_oPageFields->toBin();
        if(m_oPivotHierarchies.IsInit())
            ptr->m_SXTHS = m_oPivotHierarchies->toBin();
        if(m_oColHierarchiesUsage.IsInit())
            ptr->m_ISXTHCOLS = m_oColHierarchiesUsage->toBin();
        if(m_oRowHierarchiesUsage.IsInit())
            ptr->m_ISXTHRWS = m_oRowHierarchiesUsage->toBin();

		/*auto frt(new XLSB::FRTSXVIEW);
		auto sxview14(new XLSB::SXVIEW14);
		auto beginsxview(new XLSB::BeginSXView14);
		ptr->m_FRTSXVIEW = XLS::BaseObjectPtr{frt};
		frt->m_SXVIEW14 = XLS::BaseObjectPtr{sxview14};
		sxview14->m_BrtBeginSXView14 = XLS::BaseObjectPtr{beginsxview};
		beginsxview->fAutoApply = false;
		beginsxview->fCalcMembersInAdvFilters = false;
		beginsxview->fEnableWB = false;
		beginsxview->fFillDownLabelsDefault = false;
		beginsxview->fShowValuesRow = false;
		beginsxview->sxma.value() = 0;
		beginsxview->irstAltText = 0xFFFFFFFF;
		beginsxview->irstAltTextSummary = 0xFFFFFFFF;
		beginsxview->irstWeight = 0xFFFFFFFF;*/

		return objectPtr;
	}
	XLS::BaseObjectPtr CPivotTableDefinition::writeAttributes()
	{
		auto ptr(new XLSB::BeginSXView);
		XLS::BaseObjectPtr objectPtr(ptr);

		if (m_oApplyBorderFormats.IsInit())
			ptr->ibitAtrBdr = m_oApplyBorderFormats.get();
		else
			ptr->ibitAtrBdr = false;
		if (m_oApplyFontFormats.IsInit())
			ptr->ibitAtrFnt = m_oApplyFontFormats.get();
		else
			ptr->ibitAtrFnt = false;
		if (m_oApplyNumberFormats.IsInit())
			ptr->ibitAtrNum = m_oApplyNumberFormats.get();
		else
			ptr->ibitAtrNum = false;
		if (m_oApplyPatternFormats.IsInit())
			ptr->ibitAtrPat = m_oApplyPatternFormats.get();
		else
			ptr->ibitAtrPat = false;
		if (m_oApplyWidthHeightFormats.IsInit())
			ptr->ibitAtrProt = m_oApplyWidthHeightFormats.get();
		else
			ptr->ibitAtrProt = false;
		if (m_oApplyAlignmentFormats.IsInit())
			ptr->ibitAtrAlc = m_oApplyAlignmentFormats.get();
		else
			ptr->ibitAtrAlc = false;

		if (m_oAsteriskTotals.IsInit())
		 	ptr->fHideTotAnnotation = m_oAsteriskTotals.get();
		else
			ptr->fHideTotAnnotation = false;
		if (m_oVisualTotals.IsInit())
			ptr->fNotVisualTotals = m_oVisualTotals.get();
		else
			ptr->fNotVisualTotals = false;
		if (m_oAutoFormatId.IsInit())
			ptr->itblAutoFmt = m_oAutoFormatId->GetValue();
		else
			ptr->itblAutoFmt = 0;
		if (m_oCacheId.IsInit())
			ptr->idCache = m_oCacheId->GetValue();
		if (m_oChartFormat.IsInit())
			ptr->dwCrtFmtId = m_oChartFormat->GetValue();
		else
			ptr->dwCrtFmtId = 0;
		if (m_oColGrandTotals.IsInit())
			ptr->fColGrand = m_oColGrandTotals.get();

		if (m_oColHeaderCaption.IsInit()) ptr->irstColHdrName = m_oColHeaderCaption.get();
		else
			ptr->fUseColHdrName = false;
		if (m_oCompact.IsInit()) 
			ptr->fDefaultCompact = m_oCompact.get();
		else
			ptr->fDefaultCompact = false;
		if (m_oCompactData.IsInit()) 
			ptr->fCompactData = m_oCompactData.get();
		else
			ptr->fCompactData = false;
		if (m_oCreatedVersion.IsInit())
		 	ptr->bVerSxMacro = m_oCreatedVersion->GetValue();
		else
			ptr->bVerSxMacro = false;
		if (m_oCustomListSort.IsInit()) 
			ptr->fDontUseCustomLists = !m_oCustomListSort.get();
		else
			ptr->fDontUseCustomLists = false;
		if (m_oDataCaption.IsInit()) 
			ptr->irstData = m_oDataCaption.get();
		else
			ptr->irstData = 0xFFFFFFFF;
		if (m_oDataOnRows.IsInit()) 
			ptr->fDefaultCompact = m_oDataOnRows.get();
		else
			ptr->fDefaultCompact = false;
		if (m_oDataPosition.IsInit()) 
			ptr->ipos4Data = m_oDataPosition->GetValue();
		else	
			ptr->ipos4Data = -1;
		if (m_oDisableFieldList.IsInit()) 
			ptr->fDisableFList = m_oDisableFieldList.get();
		else 
			ptr->fDisableFList = false;
		if (m_oEditData.IsInit()) 
			ptr->fEnableDataEd = m_oEditData.get();
		else
			ptr->fEnableDataEd = false;
		if (m_oEnableDrill.IsInit()) 
			ptr->fEnableDrilldown = m_oEnableDrill.get();
		else
			ptr->fEnableDrilldown = false;
		if (m_oEnableFieldProperties.IsInit()) 
			ptr->fEnableFieldDialog = m_oEnableFieldProperties.get();
		else
			ptr->fEnableFieldDialog = false;
		if (m_oEnableWizard.IsInit()) 
			ptr->fEnableWizard = m_oEnableWizard.get();
		else
			ptr->fEnableWizard = false;
		if (m_oErrorCaption.IsInit()) 
			ptr->irstErrorString = m_oErrorCaption.get();
		else
		{
			ptr->fEmptyDisplayErrorString = true;
			ptr->fDisplayErrorString = false;
		}
		if (m_oFieldListSortAscending.IsInit()) 
			ptr->fNonDefaultSortInFlist = m_oFieldListSortAscending.get();
		else 
			ptr->fNonDefaultSortInFlist = false;
		if (m_oFieldPrintTitles.IsInit()) 
			ptr->fPrintTitles = m_oFieldPrintTitles.get();
		else 
			ptr->fPrintTitles = false;
		if (m_oGrandTotalCaption.IsInit()) 
			ptr->irstGrand = m_oGrandTotalCaption.get();
		else
			ptr->fDisplayGrand = false;
		if (m_oGridDropZones.IsInit()) 
			ptr->fNewDropZones = !m_oGridDropZones.get();
		else
			ptr->fNewDropZones = false;
		if (m_oImmersive.IsInit()) 
			ptr->fTurnOffImmersive = m_oImmersive.get();
		else 
			ptr->fTurnOffImmersive = false;
		if (m_oIndent.IsInit()) 
			ptr->cIndentInc = m_oIndent->GetValue();
		else
			ptr->cIndentInc = false;
		if (m_oItemPrintTitles.IsInit()) 
			ptr->fRepeatItemsOnEachPrintedPage = m_oItemPrintTitles.get();
		else
			ptr->fRepeatItemsOnEachPrintedPage = false;
		if (m_oMdxSubqueries.IsInit()) 
			ptr->fDefaultCompact = m_oMdxSubqueries.get();
		else
			ptr->fDefaultCompact = false;
		if (m_oMergeItem.IsInit()) 
			ptr->fMergeLabels = m_oMergeItem.get();
		else 
			ptr->fMergeLabels = false;
		if (m_oMinRefreshableVersion.IsInit()) 
			ptr->bVerSxUpdateableMin = m_oMinRefreshableVersion->GetValue();
		if (m_oMissingCaption.IsInit()) 
			ptr->irstNullString = m_oMissingCaption.get();
		else
			ptr->fEmptyDisplayNullString = true;
		if (m_oMultipleFieldFilters.IsInit()) 
			ptr->fSingleFilterPerField = !m_oMultipleFieldFilters.get();
		if (m_oName.IsInit()) ptr->irstName = m_oName.get();
		else
			ptr->irstName = 0xFFFFFFFF;
		if (m_oOutline.IsInit()) 
			ptr->fDefaultOutline = m_oOutline.get();
		if (m_oOutlineData.IsInit()) 
			ptr->fOutlineData = m_oOutlineData.get();
		else
			ptr->fOutlineData = false;
		if (m_oPageOverThenDown.IsInit())
			ptr->fAcrossPageLay = m_oPageOverThenDown.get();
		else 
			ptr->fAcrossPageLay = false;
		if (m_oPageStyle.IsInit()) 
			ptr->irstPageFieldStyle = m_oPageStyle.get();
		else
			ptr->fDisplayPageFieldStyle = false;
		if (m_oPageWrap.IsInit()) 
			ptr->cWrapPage = m_oPageWrap->GetValue();
		else
			ptr->cWrapPage = 0;
		if (m_oPivotTableStyle.IsInit()) 
			ptr->irstTableStyle = m_oPivotTableStyle.get();
		else
			ptr->fDisplayTableStyle = false;
		if (m_oPreserveFormatting.IsInit()) 
			ptr->fPreserveFormatting = m_oPreserveFormatting.get();
		if (m_oPrintDrill.IsInit()) 
			ptr->fPrintDrillIndicators = m_oPrintDrill.get();
		else 
			ptr->fPrintDrillIndicators = false;
		if (m_oPublished.IsInit()) 
			ptr->fPublished = m_oPublished.get();
		else 
			ptr->fPublished = false;
		if (m_oRowGrandTotals.IsInit()) 
			ptr->fRwGrand = m_oRowGrandTotals.get();
		if (m_oRowHeaderCaption.IsInit()) 
			ptr->irstRwHdrName = m_oRowHeaderCaption.get();
		else
			ptr->fUseRwHdrName = false;
		if (m_oShowCalcMbrs.IsInit()) 
			ptr->fNotViewCalculatedMembers = !m_oShowCalcMbrs.get();
		else 
			ptr->fNotViewCalculatedMembers = false;
		if (m_oShowDataDropDown.IsInit()) 
			ptr->fHideDDData = !m_oShowDataDropDown.get();
		else 
			ptr->fHideDDData = false;
		if (m_oShowDataTips.IsInit()) 
			ptr->fNoPivotTips = !m_oShowDataTips.get();
		else 
			ptr->fNoPivotTips = false;
		if (m_oShowDrill.IsInit()) 
			ptr->fHideDrillIndicators = !m_oShowDrill.get();
		else 
			ptr->fHideDrillIndicators = false;
		if (m_oShowDropZones.IsInit()) 
			ptr->fNoStencil = !m_oShowDropZones.get();
		else 
			ptr->fNoStencil = false;
		if (m_oShowEmptyCol.IsInit()) 
			ptr->fIncludeEmptyCol = m_oShowEmptyCol.get();
		else 
			ptr->fIncludeEmptyCol = false;
		if (m_oShowEmptyRow.IsInit()) 
			ptr->fIncludeEmptyRw = m_oShowEmptyRow.get();
		else 
			ptr->fIncludeEmptyRw = false;
		if (m_oShowError.IsInit()) 
			ptr->fDisplayErrorString = m_oShowError.get();
		if (m_oShowHeaders.IsInit())
			ptr->fNoHeaders = !m_oShowHeaders.get();
		else 
			ptr->fNoHeaders = false;
		if (m_oShowItems.IsInit()) 
			ptr->fDisplayImmediateItems = m_oShowItems.get();
		if (m_oShowMemberPropertyTips.IsInit()) 
			ptr->fMemPropsInTips = m_oShowMemberPropertyTips.get();
		if (m_oShowMissing.IsInit()) 
			ptr->fDisplayNullString = m_oShowMissing.get();
		if (m_oShowMultipleLabel.IsInit()) 
			ptr->fPageMultipleItemLabel = m_oShowMultipleLabel.get();
		if (m_oSubtotalHiddenItems.IsInit()) 
			ptr->fSubtotalHiddenPageItems = m_oSubtotalHiddenItems.get();
		else 
			ptr->fSubtotalHiddenPageItems = false;
		if (m_oTag.IsInit()) 
			ptr->irstTag = m_oTag.get();
		else
			ptr->fDisplayTag = false;
		if (m_oUpdatedVersion.IsInit()) 
			ptr->bVerSxLastUpdated = m_oUpdatedVersion->GetValue();
		if (m_oUseAutoFormatting.IsInit()) 
			ptr->fAutoFormat = m_oUseAutoFormatting.get();
		if (m_oVacatedStyle.IsInit()) 
			ptr->irstVacateStyle = m_oVacatedStyle.get();
		else
			ptr->fDisplayVacateStyle = false;
		ptr->sxaxis4Data = 2;
		ptr->fReenterOnLoadOnce = false;

		return objectPtr;
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

            if(ptr->m_SXTHS != nullptr)
                m_oPivotHierarchies = ptr->m_SXTHS;
            if(ptr->m_ISXTHCOLS != nullptr)
            {
                m_oColHierarchiesUsage.Init();
                m_oColHierarchiesUsage->m_oRowHierarchy = false;
                m_oColHierarchiesUsage->fromBin(ptr->m_ISXTHCOLS);
            }
            if(ptr->m_ISXTHRWS != nullptr)
            {
                m_oRowHierarchiesUsage.Init();
                m_oRowHierarchiesUsage->m_oRowHierarchy = true;
                m_oRowHierarchiesUsage->fromBin(ptr->m_ISXTHRWS);
            }
			if(ptr->m_FRTSXVIEW)
			{
				auto result = static_cast<XLSB::FRTSXVIEW*>(ptr->m_FRTSXVIEW.get());
				auto result2 = static_cast<XLSB::SXVIEW14*>(result->m_SXVIEW14.get());
				auto result3 = static_cast<XLSB::BeginSXView14*>(result2->m_BrtBeginSXView14.get());
				auto result4 = result3;
			}
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
            if(ptr->ipos4Data > 0)
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
			{
				CField* pField = new CField();
				*pField = oReader;
				m_arrItems.push_back(pField);
			}
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
	XLS::BaseObjectPtr CColumnRowFields::toBinRows()
	{
		auto ptr(new XLSB::ISXVDRWS);
		XLS::BaseObjectPtr objectPtr(ptr);
		auto ptr1(new XLSB::BeginISXVDRws);
		ptr->m_BrtBeginISXVDRws = XLS::BaseObjectPtr{ptr1};
		if(m_oCount.IsInit())
			ptr1->cisxvd = m_oCount->GetValue();
		else
			ptr1->cisxvd = m_arrItems.size();
		for(auto i:m_arrItems)
			ptr1->rgisxvdrws.push_back(i->m_oX.get());
		return objectPtr;
	}
	XLS::BaseObjectPtr CColumnRowFields::toBinCols()
	{
		auto ptr(new XLSB::ISXVDCOLS);
		XLS::BaseObjectPtr objectPtr(ptr);
		auto ptr1(new XLSB::BeginISXVDCols);
		ptr->m_BrtBeginISXVDCols = XLS::BaseObjectPtr{ptr1};
		if(m_oCount.IsInit())
			ptr1->cisxvd = m_oCount->GetValue();
		else
			ptr1->cisxvd = m_arrItems.size();
		for(auto i:m_arrItems)
			ptr1->rgisxvdcols.push_back(i->m_oX.get());
		return objectPtr;
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
			{
				CColumnRowItem* pColumnRowItem = new CColumnRowItem();
				*pColumnRowItem = oReader;
				m_arrItems.push_back(pColumnRowItem);
			}
		}
	}
	XLS::BaseObjectPtr CColumnRowItems::toBinRows()
	{
		auto ptr(new XLSB::SXLIRWS);
		auto ptr1(new XLSB::BeginSXLIRws);
		ptr1->csxlis = m_arrItems.size();
		ptr->m_BrtBeginSXLIRws = XLS::BaseObjectPtr{ptr1};
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arSXLI.push_back(i->toBin());
		return objectPtr;
	}
	XLS::BaseObjectPtr CColumnRowItems::toBinCols()
	{
		auto ptr(new XLSB::SXLICOLS);
		auto ptr1(new XLSB::BeginSXLICols);
		ptr1->csxlis = m_arrItems.size();
		ptr->m_BrtBeginSXLICols = XLS::BaseObjectPtr{ptr1};
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arSXLI.push_back(i->toBin());
		return objectPtr;
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
				m_arrItems.push_back(PPTX::CreatePtrXmlContent<CSharedItemsIndex>(oReader));
		}
	}
	XLS::BaseObjectPtr CColumnRowItem::toBin()
	{
		auto ptr(new XLSB::SXLI);

		XLS::BaseObjectPtr objectPtr(ptr);
		auto ptr1(new XLSB::BeginSXLI);
        ptr1->cisxvis = m_arrItems.size();
		ptr->m_BrtBeginSXLI = XLS::BaseObjectPtr{ptr1};
		if(m_oI.IsInit())
			ptr1->iData = m_oI->GetValue();
        else
            ptr1->iData = 0;
		if(m_oR.IsInit())
			ptr1->cSic = m_oR->GetValue();
		else
			ptr1->cSic = 0;
		if(m_oT.IsInit())
		{
			if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeData)
				ptr1->itmtype = XLSB::PivotItemType::PITDATA;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeDefault)
				ptr1->itmtype = XLSB::PivotItemType::PITDEFAULT;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeSum)
				ptr1->itmtype = XLSB::PivotItemType::PITSUM;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeCountA)
				ptr1->itmtype = XLSB::PivotItemType::PITCOUNTA;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeAverage)
				ptr1->itmtype = XLSB::PivotItemType::PITAVG;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeMax)
				ptr1->itmtype = XLSB::PivotItemType::PITMAX;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeMin)
				ptr1->itmtype = XLSB::PivotItemType::PITMIN;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeProduct)
				ptr1->itmtype = XLSB::PivotItemType::PITPRODUCT;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeCount)
				ptr1->itmtype = XLSB::PivotItemType::PITCOUNT;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeStdDev)
				ptr1->itmtype = XLSB::PivotItemType::PITSTDDEV;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeStdDevP)
				ptr1->itmtype = XLSB::PivotItemType::PITSTDDEVP;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeVar)
				ptr1->itmtype = XLSB::PivotItemType::PITVAR;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeVarP)
				ptr1->itmtype = XLSB::PivotItemType::PITVARP;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeGrandTotalt)
				ptr1->itmtype = XLSB::PivotItemType::PITGRAND;
			else if (m_oT == SimpleTypes::Spreadsheet::EPivotItemType::typeBlank)
				ptr1->itmtype = XLSB::PivotItemType::PITBLANK;
		}
		else
		{
			ptr1->itmtype = XLSB::PivotItemType::PITDEFAULT;
		}

		if(ptr1->cisxvis > 0)
		{

			auto ptr2(new XLSB::ISXVIS(ptr1->cisxvis));
			ptr2->_cisxvis = ptr1->cisxvis;

			ptr->m_ISXVIS = XLS::BaseObjectPtr{ptr2};
			auto ptr3(new XLSB::BeginISXVIs(ptr1->cisxvis));
			ptr2->m_BrtBeginISXVIs = XLS::BaseObjectPtr{ptr3};

			for(auto i:m_arrItems)
				if(i->m_oV.IsInit())
					ptr3->rgisxvis.push_back(i->m_oV->GetValue());
				else
					ptr3->rgisxvis.push_back(0);
		}
		return objectPtr;
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
			{
				CDataField* pDataField = new CDataField();
				*pDataField = oReader;
				m_arrItems.push_back(pDataField);
			}
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
	XLS::BaseObjectPtr CDataFields::toBin()
	{
		auto ptr(new XLSB::SXDIS);
		auto ptr1(new XLSB::BeginSXDIs);
        ptr->m_BrtBeginSXDIs = XLS::BaseObjectPtr{ptr1};
		ptr1->csxdis = m_arrItems.size();
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arSXDI.push_back(i->toBin());
		return objectPtr;
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
	XLS::BaseObjectPtr CDataField::toBin()
	{
		auto ptr1(new XLSB::SXDI);
		XLS::BaseObjectPtr objectPtr(ptr1);
		auto ptr(new XLSB::BeginSXDI);
		ptr1->m_BrtBeginSXDI = XLS::BaseObjectPtr{ptr};

		if(m_oBaseField.IsInit())
			ptr->isxvd = m_oBaseField.get();
        else
            ptr->isxvd = 0;
		if(m_oBaseItem.IsInit())
			ptr->isxvi = m_oBaseItem->GetValue();
        else
            ptr->isxvi = 0;
		if(m_oFld.IsInit())
			ptr->isxvdData = m_oFld->GetValue();
        else
            ptr->isxvdData = false;
		if(m_oNumFmtId.IsInit())
			ptr->ifmt.ifmt = m_oNumFmtId->GetValue();
		else
			ptr->ifmt.ifmt = 0;
		if(m_oName.IsInit())
			{
				ptr->stDisplayName = m_oName.get();
				ptr->fLoadDisplayName = true;
			}
		else
			ptr->fLoadDisplayName = false;
		if(m_oShowDataAs.IsInit())
		{
			if (m_oShowDataAs == SimpleTypes::Spreadsheet::EShowDataAs::dataAsNormal)
				ptr->df = XLSB::ShowDataAs::NORMAL;
			else if (m_oShowDataAs == SimpleTypes::Spreadsheet::EShowDataAs::dataAsDifference)
				ptr->df = XLSB::ShowDataAs::DIFFERENCE_;
			else if (m_oShowDataAs == SimpleTypes::Spreadsheet::EShowDataAs::dataAsPercentOff)
				ptr->df = XLSB::ShowDataAs::PERCENT;
			else if (m_oShowDataAs == SimpleTypes::Spreadsheet::EShowDataAs::dataAsPercentDiff)
				ptr->df = XLSB::ShowDataAs::PERCENTDIFF;
			else if (m_oShowDataAs == SimpleTypes::Spreadsheet::EShowDataAs::dataAsIndex)
				ptr->df = XLSB::ShowDataAs::INDEX;
			else if (m_oShowDataAs == SimpleTypes::Spreadsheet::EShowDataAs::dataAsPercentOfTotal)
				ptr->df = XLSB::ShowDataAs::PERCENTOFTOTAL;
			else if (m_oShowDataAs == SimpleTypes::Spreadsheet::EShowDataAs::dataAsPercentOfCol)
				ptr->df = XLSB::ShowDataAs::PERCENTOFCOL;
			else if (m_oShowDataAs == SimpleTypes::Spreadsheet::EShowDataAs::dataAsPercentOfRow)
				ptr->df = XLSB::ShowDataAs::PERCENTOFROW;
			else if (m_oShowDataAs == SimpleTypes::Spreadsheet::EShowDataAs::dataAsRunTotal)
				ptr->df = XLSB::ShowDataAs::PERCENTOFRUNTOTAL;
		}
		else
		{
			ptr->df = XLSB::ShowDataAs::NORMAL;
		}
		if(m_oSubtotal.IsInit())
		{
			if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionSum)
				ptr->iiftab = XLSB::DataConsolidationFunction::SUM;
			else if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionCount)
				ptr->iiftab = XLSB::DataConsolidationFunction::COUNT;
			else if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionAverage)
				ptr->iiftab = XLSB::DataConsolidationFunction::AVERAGE;
			else if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionMaximum)
				ptr->iiftab = XLSB::DataConsolidationFunction::MAX;
			else if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionMinimum)
				ptr->iiftab = XLSB::DataConsolidationFunction::MIN;
			else if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionProduct)
				ptr->iiftab = XLSB::DataConsolidationFunction::PRODUCT;
			else if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionCountNums)
				ptr->iiftab = XLSB::DataConsolidationFunction::COUNTNUM;
			else if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionStdDev)
				ptr->iiftab = XLSB::DataConsolidationFunction::STDDEV;
			else if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionStdDevP)
				ptr->iiftab = XLSB::DataConsolidationFunction::STDDEVP;
			else if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionVariance)
				ptr->iiftab = XLSB::DataConsolidationFunction::STDVAR;
			else if (m_oSubtotal == SimpleTypes::Spreadsheet::EDataConsolidateFunction::functionVarP)
				ptr->iiftab = XLSB::DataConsolidationFunction::STDVARP;
		}
		else
		{
			ptr->iiftab = XLSB::DataConsolidationFunction::SUM;
		}
		return objectPtr;
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
			{
				CPageField* pPageField = new CPageField();
				*pPageField = oReader;
				m_arrItems.push_back(pPageField);
			}
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
	XLS::BaseObjectPtr CPageFields::toBin()
	{
		auto ptr(new XLSB::SXPIS);
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arSXPI.push_back(i->toBin());
		return objectPtr;
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
	XLS::BaseObjectPtr CPageField::toBin()
	{
		auto ptr1(new XLSB::SXPI);
		XLS::BaseObjectPtr objectPtr(ptr1);
		auto ptr(new XLSB::BeginSXPI);
		ptr1->m_BrtBeginSXPI = XLS::BaseObjectPtr{ptr};

		if(m_oFld.IsInit())
			ptr->isxvd = m_oFld.get();
		if(m_oItem.IsInit())
			ptr->isxvi = m_oItem->GetValue();
		if(m_oHier.IsInit())
			ptr->isxth = m_oHier.get();
		if(m_oName.IsInit())
        {
			ptr->irstUnique = m_oName.get();
            ptr->fUnique = true;
        }
		if(m_oCap.IsInit())
        {
			ptr->irstDisplay = m_oCap.get();
            ptr->fDisplay = true;
        }
		return objectPtr;
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
			{
				CFieldItem* pFieldItem = new CFieldItem();
				*pFieldItem = oReader;
				m_arrItems.push_back(pFieldItem);
			}
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
	XLS::BaseObjectPtr CFieldItems::toBin()
	{
		auto ptr(new XLSB::SXVIS);
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arSXVI.push_back(i->toBin());
		return objectPtr;
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
	XLS::BaseObjectPtr CFieldItem::toBin()// fix name init
	{
		auto ptr1(new XLSB::SXVI);
		XLS::BaseObjectPtr objectPtr(ptr1);

		auto ptr(new XLSB::BeginSXVI);
		ptr1->m_BrtBeginSXVI = XLS::BaseObjectPtr{ptr};

		if(m_oChild.IsInit())
			ptr->fHasChildrenEst = m_oChild.get();
        else
            ptr->fHasChildrenEst = false;
		if(m_oExpanded.IsInit())
			ptr->fDrilledMember = m_oExpanded.get();
        else
            ptr->fDrilledMember = false;
		if(m_oDrillAcross.IsInit())
			ptr->fCollapsedMember = m_oDrillAcross.get();
        else
            ptr->fCollapsedMember = false;
		if(m_oCalculated.IsInit())
			ptr->fFormula = m_oCalculated.get();
        else
            ptr->fFormula = false;
		if(m_oHidden.IsInit())
			ptr->fHidden = m_oHidden.get();
        else
            ptr->fHidden = false;
		if(m_oMissing.IsInit())
			ptr->fMissing = m_oMissing.get();
        else
            ptr->fMissing = false;
		if(m_oUserCaption.IsInit())
		{
			ptr->displayName = m_oUserCaption.get();
			ptr->fDisplayName = true;
		}
		else
		{
			ptr->fDisplayName = false;
		}
		if(m_oCharacter.IsInit())
			ptr->fOlapFilterSelected = m_oCharacter.get();
        else
            ptr->fOlapFilterSelected = false;
		if(m_oHideDetails.IsInit())
			ptr->fHideDetail = m_oHideDetails.get();
        else
            ptr->fHideDetail = false;
		if(m_oItemIndex.IsInit())
			ptr->iCache = m_oItemIndex->GetValue();
        else
            ptr->iCache = -1;
		if(m_oItemType.IsInit())
		{
			if(m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeData)
				ptr->itmtype = XLSB::PivotItemType::PITDATA;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeDefault)
				ptr->itmtype = XLSB::PivotItemType::PITDEFAULT;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeSum)
				ptr->itmtype = XLSB::PivotItemType::PITSUM;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeCountA)
				ptr->itmtype = XLSB::PivotItemType::PITCOUNTA;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeAverage)
				ptr->itmtype = XLSB::PivotItemType::PITAVG;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeMax)
				ptr->itmtype = XLSB::PivotItemType::PITMAX;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeMin)
				ptr->itmtype = XLSB::PivotItemType::PITMIN;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeProduct)
				ptr->itmtype = XLSB::PivotItemType::PITPRODUCT;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeCount)
				ptr->itmtype = XLSB::PivotItemType::PITCOUNT;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeStdDev)
				ptr->itmtype = XLSB::PivotItemType::PITSTDDEV;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeStdDevP)
				ptr->itmtype = XLSB::PivotItemType::PITSTDDEVP;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeVar)
				ptr->itmtype = XLSB::PivotItemType::PITVAR;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeVarP)
				ptr->itmtype = XLSB::PivotItemType::PITVARP;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeGrandTotalt)
				ptr->itmtype = XLSB::PivotItemType::PITGRAND;
			else if (m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeBlank)
				ptr->itmtype = XLSB::PivotItemType::PITBLANK;
		}
        else
            ptr->itmtype = XLSB::PivotItemType::PITDATA;
		return objectPtr;
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
			{
				CPivotField* pPivotField = new CPivotField();
				*pPivotField = oReader;
				m_arrItems.push_back(pPivotField);
			}
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
	XLS::BaseObjectPtr CPivotFields::toBin()
	{
		auto ptr(new XLSB::SXVDS);
		auto ptr1(new XLSB::BeginSXVDs);
		ptr1->csxvds = m_arrItems.size();
		ptr->m_BrtBeginSXVDs = XLS::BaseObjectPtr{ptr1};
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arSXVD.push_back(i->toBin());
		return objectPtr;
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

		if(m_oItems.IsInit())
			m_oItems->toXML(writer);

        if(m_oAutoSortScope.IsInit())
            m_oAutoSortScope->toXML(writer);

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
	XLS::BaseObjectPtr CPivotField::toBin()
	{
		auto ptr(new XLSB::SXVD);
		XLS::BaseObjectPtr objectPtr(ptr);
		ptr->m_BrtBeginSXVD = writeAttributes();
		if(m_oItems.IsInit())
			ptr->m_SXVIS = m_oItems->toBin();
		if(m_oAutoSortScope.IsInit())
			ptr->m_AUTOSORTSCOPE = m_oAutoSortScope->toBin();
		return objectPtr;
	}
	XLS::BaseObjectPtr CPivotField::writeAttributes()
	{
		auto ptr(new XLSB::BeginSXVD);
		XLS::BaseObjectPtr objectPtr(ptr);

		if(m_oAllDrilled.IsInit())
        	ptr->fDrilledLevel = m_oAllDrilled.get();
		else
			ptr->fDrilledLevel = false;

		if(m_oAutoShow.IsInit())
            ptr->fAutoShow = m_oAutoShow.get();
		else
			ptr->fAutoShow = false;

		if(m_oAvgSubtotal.IsInit())
            ptr->fAverage = m_oAvgSubtotal.get();
		else
			ptr->fAverage = false;
		
		ptr->sxaxis.bCol = false;
		ptr->sxaxis.bPage = false;
		ptr->sxaxis.bRw = false;
		ptr->sxaxis.bData = false;
		
		if(m_oAxis.IsInit())
		{
			if(m_oAxis == SimpleTypes::Spreadsheet::EPivotAxisType::axisCol)
				ptr->sxaxis.bCol = true;
			else if(m_oAxis == SimpleTypes::Spreadsheet::EPivotAxisType::axisPage)
				ptr->sxaxis.bPage = true;
			else if(m_oAxis == SimpleTypes::Spreadsheet::EPivotAxisType::axisRow)
				ptr->sxaxis.bRw = true;
			else if(m_oAxis == SimpleTypes::Spreadsheet::EPivotAxisType::axisValues)
				ptr->sxaxis.bData = true;
		}
		if(m_oCompact.IsInit())
            ptr->fCompact = m_oCompact.get();
		else
            ptr->fCompact = true;

		if(m_oCountASubtotal.IsInit())
			ptr->fCounta = m_oCountASubtotal.get();
		else
			ptr->fCounta = false;

		if(m_oCountSubtotal.IsInit())
			ptr->fCount = m_oCountSubtotal.get();
		else
			ptr->fCount = false;
		if(m_oDataField.IsInit())
			ptr->sxaxis.bData = m_oDataField.get();

		if(m_oDataSourceSort.IsInit())
			ptr->fTensorSort = m_oDataSourceSort.get();
		else
			ptr->fTensorSort = false;
		if(m_oDefaultAttributeDrillState.IsInit())
			ptr->fItemsDrilledByDefault = m_oDefaultAttributeDrillState.get();
		else
			ptr->fItemsDrilledByDefault = false;
		if(m_oDragOff.IsInit())
			ptr->fDragToHide = m_oDragOff.get();
		else
            ptr->fDragToHide = true;

		if (m_oDragToCol.IsInit()) 
			ptr->fDragToColumn = m_oDragToCol.get();
		else
            ptr->fDragToColumn = true;
		if (m_oDragToData.IsInit()) 
			ptr->fDragToData = m_oDragToData.get();
		else
            ptr->fDragToData = true;
		if (m_oDragToPage.IsInit()) 
			ptr->fDragToPage = m_oDragToPage.get();
		else
            ptr->fDragToPage = true;
		if (m_oDragToRow.IsInit()) 
			ptr->fDragToRow = m_oDragToRow.get();
		else
            ptr->fDragToRow = true;
		if (m_oHiddenLevel.IsInit()) 
			ptr->fHiddenLvl = m_oHiddenLevel.get();
		else
			ptr->fHiddenLvl = false;
		if (m_oHideNewItems.IsInit()) 
			ptr->fHideNewItems = m_oHideNewItems.get();
		else
			ptr->fHideNewItems = false;
		if (m_oIncludeNewItemsInFilter.IsInit()) 
			ptr->fFilterInclusive = m_oIncludeNewItemsInFilter.get();
		else
            ptr->fFilterInclusive = true;
		if (m_oInsertBlankRow.IsInit()) 
			ptr->fInsertBlankRow = m_oInsertBlankRow.get();
		else
			ptr->fInsertBlankRow = false;
		if (m_oInsertPageBreak.IsInit()) 
			ptr->fPageBreaksBetweenItems = m_oInsertPageBreak.get();
		else
			ptr->fPageBreaksBetweenItems = false;

		if(m_oItemPageCount.IsInit())
            ptr->citmAutoShow = m_oItemPageCount->GetValue();
		else
            ptr->citmAutoShow = 10;

		if(m_oMaxSubtotal.IsInit())
			ptr->fMax = m_oMaxSubtotal.get();
		else
			ptr->fMax = false;

		if(m_oMeasureFilter.IsInit())
			ptr->fHasAdvFilter = m_oMeasureFilter.get();
		else
            ptr->fHasAdvFilter = false;

		if(m_oMinSubtotal.IsInit())
			ptr->fMin = m_oMinSubtotal.get();
		else
			ptr->fMin = false;

		if(m_oMultipleItemSelectionAllowed.IsInit())
			ptr->fEnableMultiplePageItems = m_oMultipleItemSelectionAllowed.get();
		else
			ptr->fEnableMultiplePageItems = false;

		if(m_oName.IsInit())
			ptr->irstName = m_oName.get();
		else
			ptr->fDisplayName = false;


		if (m_oNonAutoSortDefault.IsInit())
			ptr->fNotAutoSortDft = m_oNonAutoSortDefault.get();
		else 
			ptr->fNotAutoSortDft = false;

		if (m_oNumFmtId.IsInit())
			ptr->ifmt = m_oNumFmtId->GetValue();
		else
            ptr->ifmt = 0;

		if (m_oOutline.IsInit())
			ptr->fOutline = m_oOutline.get();
		else
            ptr->fOutline = true;

		if (m_oProductSubtotal.IsInit())
			ptr->fProduct = m_oProductSubtotal.get();
		else
			ptr->fProduct = false;

		if (m_oRankBy.IsInit())
			ptr->isxdiAutoShow = m_oRankBy->GetValue();
		else
            ptr->isxdiAutoShow = -1;

		if (m_oServerField.IsInit())
			ptr->fServerBased = m_oServerField.get();
        else
            ptr->fServerBased = false;

		if (m_oShowAll.IsInit())
			ptr->fShowAllItems = m_oShowAll.get();
		else
			ptr->fShowAllItems = false;

		if (m_oShowDropDowns.IsInit())
			ptr->fHideDD = !m_oShowDropDowns.get();
        else
            ptr->fHideDD = false;

		if (m_oShowPropAsCaption.IsInit())
			ptr->fMemPropDisplayInCaption = m_oShowPropAsCaption.get();
        else
            ptr->fMemPropDisplayInCaption = false;

		if (m_oShowPropCell.IsInit())
			ptr->fMemPropDisplayInReport = m_oShowPropCell.get();
        else
            ptr->fMemPropDisplayInReport = false;

		if (m_oShowPropTip.IsInit())
			ptr->fMemPropDisplayInTip = m_oShowPropTip.get();
		else
			ptr->fMemPropDisplayInTip = false;
        if(m_oNonAutoSortDefault.IsInit())
            ptr->fAutoSort = !m_oNonAutoSortDefault.get();
        else
            ptr->fAutoSort = false;
		if(m_oSortType.IsInit())
		{
            if(m_oSortType->GetValue() == SimpleTypes::Spreadsheet::EFieldSortType::sortAscending)
                ptr->fAscendSort = true;
            else
                ptr->fAscendSort = false;
		}
        else
             ptr->fAscendSort = false;

		if (m_oStdDevPSubtotal.IsInit())
			ptr->fStdevp = m_oStdDevPSubtotal.get();
        else
            ptr->fStdevp = false;

		if (m_oStdDevSubtotal.IsInit())
			ptr->fStdev = m_oStdDevSubtotal.get();
        else
            ptr->fStdev = false;

		if (m_oSubtotalCaption.IsInit())
			ptr->irstSub = m_oSubtotalCaption.get();
		else
			ptr->fDisplaySub = false;

		if (m_oSubtotalTop.IsInit())
			ptr->fSubtotalAtTop = !m_oSubtotalTop.get();
        else
            ptr->fSubtotalAtTop = true;

		if (m_oSumSubtotal.IsInit())
			ptr->fSum = m_oSumSubtotal.get();
		else
			ptr->fSum = false;

		if (m_oTopAutoShow.IsInit())
			ptr->fTopAutoShow = !m_oTopAutoShow.get();
        else
            ptr->fTopAutoShow = true;

		if (m_oUniqueMemberProperty.IsInit())
			ptr->irstMemberPropertyCaption = m_oUniqueMemberProperty.get();
		else
			ptr->fUseMemPropCaption = false;

		if (m_oVarPSubtotal.IsInit())
			ptr->fVarp = m_oVarPSubtotal.get();
        else
            ptr->fVarp = false;

		if (m_oVarSubtotal.IsInit())
			ptr->fVar = m_oVarSubtotal.get();
		else
            ptr->fVar = false;
		ptr->fDefault = false;
		if(m_oItems.IsInit())
		{
			for(auto i: m_oItems->m_arrItems)
			{
				if(i->m_oItemType.IsInit() && i->m_oItemType == SimpleTypes::Spreadsheet::EPivotItemType::typeDefault)
					ptr->fDefault = true;
			}
		}

		return objectPtr;
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
            if(ptr->sxaxis.bData)
                m_oDataField = ptr->sxaxis.bData;
            if(!ptr->fCompact)
                m_oCompact                  = ptr->fCompact;

            if(ptr->fCounta)
                m_oCountASubtotal           = ptr->fCounta;

            if(ptr->fCount)
                m_oCountSubtotal            = ptr->fCount;

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
			{
				CReference* pReference = new CReference();
				*pReference = oReader;
				m_arrItems.push_back(pReference);
			}
		}
	}
	XLS::BaseObjectPtr CReferences::toBin()
	{
		auto ptr(new XLSB::PRFILTERS);
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arPRFILTER.push_back(i->toBin());
		return objectPtr;
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
	XLS::BaseObjectPtr CReference::toBin()
	{
		auto ptr(new XLSB::PRFILTER);
		XLS::BaseObjectPtr objectPtr(ptr);
		ptr->m_BrtBeginPRFilter = writeAttributes();
        if(m_oX.IsInit())
            ptr->m_arPRFITEM.push_back(m_oX->toBinPrfItem());
		return objectPtr;
	}
	XLS::BaseObjectPtr CReference::writeAttributes()
	{
		auto ptr(new XLSB::BeginPRFilter);
		XLS::BaseObjectPtr objectPtr(ptr);
		if (m_oAvgSubtotal.IsInit())
    		ptr->prFilter.itmtypeAVERAGE = m_oAvgSubtotal.get();
		else
			ptr->prFilter.itmtypeAVERAGE = false;

		if (m_oCountASubtotal.IsInit())
			ptr->prFilter.itmtypeCOUNTA = m_oCountASubtotal.get();
		else
			ptr->prFilter.itmtypeCOUNTA = false;

		if (m_oCountSubtotal.IsInit())
			ptr->prFilter.itmtypeCOUNT = m_oCountSubtotal.get();
		else
			ptr->prFilter.itmtypeCOUNT = false;

		if (m_oDefaultSubtotal.IsInit())
			ptr->prFilter.itmtypeDEFAULT = m_oDefaultSubtotal.get();
		else
			ptr->prFilter.itmtypeDEFAULT = false;

		if (m_oMaxSubtotal.IsInit())
			ptr->prFilter.itmtypeMAX = m_oMaxSubtotal.get();
		else
			ptr->prFilter.itmtypeMAX = false;

		if (m_oMinSubtotal.IsInit())
			ptr->prFilter.itmtypeMIN = m_oMinSubtotal.get();
		else
			ptr->prFilter.itmtypeMIN = false;

		if (m_oProductSubtotal.IsInit())
			ptr->prFilter.itmtypePRODUCT = m_oProductSubtotal.get();
		else
			ptr->prFilter.itmtypePRODUCT = false;

		if (m_oRelative.IsInit())
			ptr->prFilter.itmtypeAVERAGE = m_oRelative.get();
		else
			ptr->prFilter.itmtypeAVERAGE = false;

		if (m_oSelected.IsInit())
			ptr->prFilter.fSelected = m_oSelected.get();
		else
			ptr->prFilter.fSelected = false;

		if (m_oStdDevPSubtotal.IsInit())
			ptr->prFilter.itmtypeSTDEVP = m_oStdDevPSubtotal.get();
		else
			ptr->prFilter.itmtypeSTDEVP = false;

		if (m_oStdDevSubtotal.IsInit())
			ptr->prFilter.itmtypeSTDEV = m_oStdDevSubtotal.get();
		else
			ptr->prFilter.itmtypeSTDEV = false;

		if (m_oSumSubtotal.IsInit())
			ptr->prFilter.itmtypeSUM = m_oSumSubtotal.get();
		else
			ptr->prFilter.itmtypeSUM = false;

		if (m_oVarPSubtotal.IsInit())
			ptr->prFilter.itmtypeVARP = m_oVarPSubtotal.get();
		else
			ptr->prFilter.itmtypeVARP  = false;

		if (m_oVarSubtotal.IsInit())
			ptr->prFilter.itmtypeVAR = m_oVarSubtotal.get();
		else
			ptr->prFilter.itmtypeVAR = false;

		if (m_oField.IsInit())
			ptr->prFilter.isxvd = m_oField->GetValue();
		else
			ptr->prFilter.isxvd = 0;

		if (m_oCount.IsInit())
			ptr->prFilter.cItems = m_oCount->GetValue();
		else
			ptr->prFilter.cItems = 0;
		return objectPtr;
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
			{
				CPivotTableFormat* pPivotTableFormat = new CPivotTableFormat();
				*pPivotTableFormat = oReader;
				m_arrItems.push_back(pPivotTableFormat);
			}
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
	XLS::BaseObjectPtr CPivotTableFormats::toBin()
	{
		auto ptr(new XLSB::SXFORMATS);
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arSXFORMAT.push_back(i->toBin());
		return objectPtr;
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
		writer.WriteString(L"<format");
			WritingStringNullableAttrString(L"action", m_oAction, m_oAction->ToString());
			WritingStringNullableAttrString(L"dxfId", m_oDxfId, m_oDxfId->ToString());
		writer.WriteString(L">");

		if(m_oPivotArea.IsInit())
		{
			m_oPivotArea->toXML(writer);
		}

		writer.WriteString(L"</format>");
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
	XLS::BaseObjectPtr CPivotTableFormat::toBin()
	{
		auto ptr(new XLSB::SXFORMAT);
		XLS::BaseObjectPtr objectPtr(ptr);

        auto ptr1(new XLSB::BeginSXFormat);
        ptr->m_BrtBeginSXFormat = XLS::BaseObjectPtr{ptr1};
        if(m_oDxfId.IsInit())
            ptr1->dxfid = m_oDxfId->GetValue();
        else
            ptr1->dxfid = 0;
        if(m_oAction.IsInit())
            ptr1->rlType = m_oAction->GetValue();
        else
            ptr1->rlType = 0;

		if(m_oPivotArea.IsInit())
			ptr->m_PIVOTRULE = m_oPivotArea->toBin();
		return objectPtr;
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
	XLS::BaseObjectPtr CAutoSortScope::toBin()
	{
		auto ptr(new XLSB::AUTOSORTSCOPE);
		XLS::BaseObjectPtr objectPtr(ptr);
		ptr->m_PIVOTRULE = m_oPivotArea->toBin();
		return objectPtr;
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
	XLS::BaseObjectPtr CPivotArea::toBin()
	{
		auto ptr(new XLSB::PIVOTRULE);
		XLS::BaseObjectPtr objectPtr(ptr);
		ptr->m_BrtBeginPRule = writeAttribures();
		if(m_oReferences.IsInit())
			ptr->m_PRFILTERS = m_oReferences->toBin();
		return objectPtr;
	}
	XLS::BaseObjectPtr CPivotArea::writeAttribures()
	{
		auto ptr(new XLSB::BeginPRule);
		XLS::BaseObjectPtr objectPtr(ptr);
        ptr->pruleheaderdata.sxaxis.bCol = false;
        ptr->pruleheaderdata.sxaxis.bPage = false;
        ptr->pruleheaderdata.sxaxis.bRw = false;
        ptr->pruleheaderdata.sxaxis.bData = false;
		if(m_oAxis.IsInit())
		{
			if(m_oAxis == SimpleTypes::Spreadsheet::EPivotAxisType::axisCol)
				ptr->pruleheaderdata.sxaxis.bCol = true;
			else if(m_oAxis == SimpleTypes::Spreadsheet::EPivotAxisType::axisPage)
				ptr->pruleheaderdata.sxaxis.bPage = true;
			else if(m_oAxis == SimpleTypes::Spreadsheet::EPivotAxisType::axisRow)
				ptr->pruleheaderdata.sxaxis.bRw = true;
			else if(m_oAxis == SimpleTypes::Spreadsheet::EPivotAxisType::axisValues)
				ptr->pruleheaderdata.sxaxis.bData = true;
		}
		if(m_oCacheIndex.IsInit())
			ptr->pruleheaderdata.fCacheBased = m_oCacheIndex.get();
        else
            ptr->pruleheaderdata.fCacheBased = false;
		if(m_oCollapsedLevelsAreSubtotals.IsInit())
			ptr->pruleheaderdata.fFuzzy = m_oCollapsedLevelsAreSubtotals.get();
        else
            ptr->pruleheaderdata.fFuzzy = false;
		if(m_oDataOnly.IsInit())
			ptr->pruleheaderdata.fDataOnly = m_oDataOnly.get();
        else
            ptr->pruleheaderdata.fDataOnly = true;
		if(m_oField.IsInit())
			ptr->pruleheaderdata.isxvd = m_oField.get();
        else
            ptr->pruleheaderdata.isxvd = -1;
		if(m_oFieldPosition.IsInit())
			ptr->pruleheaderdata.iDim = m_oFieldPosition->GetValue();
        else
            ptr->pruleheaderdata.iDim = 0;
		if(m_oGrandCol.IsInit())
			ptr->pruleheaderdata.fGrandCol = m_oGrandCol.get();
        else
            ptr->pruleheaderdata.fGrandCol = false;
		if(m_oGrandRow.IsInit())
			ptr->pruleheaderdata.fGrandRw = m_oGrandRow.get();
        else
            ptr->pruleheaderdata.fGrandRw = false;
		if(m_oLabelOnly.IsInit())
			ptr->pruleheaderdata.fLabelOnly = m_oLabelOnly.get();
        else
            ptr->pruleheaderdata.fLabelOnly = false;
		if(m_oOffsetRef.IsInit())
			ptr->pruleheaderdata.rfxLoc = m_oOffsetRef.get();
		if(m_oOutline.IsInit())
			ptr->pruleheaderdata.fLineMode = m_oOutline.get();
        else
            ptr->pruleheaderdata.fLineMode = false;
        ptr->pruleheaderdata.fPart = false;
		if(m_oType.IsInit())
		{
			if (m_oType == SimpleTypes::Spreadsheet::EPivotAreaType::areaNone)
				ptr->pruleheaderdata.isxrtype = 0x00;
			else if (m_oType == SimpleTypes::Spreadsheet::EPivotAreaType::areaNormal)
				ptr->pruleheaderdata.isxrtype = 0x01;
			else if (m_oType == SimpleTypes::Spreadsheet::EPivotAreaType::areaData)
				ptr->pruleheaderdata.isxrtype = 0x02;
			else if (m_oType == SimpleTypes::Spreadsheet::EPivotAreaType::areaAll)
				ptr->pruleheaderdata.isxrtype = 0x03;
			else if (m_oType == SimpleTypes::Spreadsheet::EPivotAreaType::areaOrigin)
				ptr->pruleheaderdata.isxrtype = 0x04;
			else if (m_oType == SimpleTypes::Spreadsheet::EPivotAreaType::areaFieldButton)
				ptr->pruleheaderdata.isxrtype = 0x05;
			else if (m_oType == SimpleTypes::Spreadsheet::EPivotAreaType::areaTopEnd)
				ptr->pruleheaderdata.isxrtype = 0x06;
		}
        else
            ptr->pruleheaderdata.isxrtype = 0x01;
		return objectPtr;
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

            if(!ptr->pruleheaderdata.rfxLoc.toString().empty()
            && (ptr->pruleheaderdata.rfxLoc.rowFirst!=0 || ptr->pruleheaderdata.rfxLoc.columnFirst!=0))
                    m_oOffsetRef            = ptr->pruleheaderdata.rfxLoc.toString(true, true);

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

	XLS::BaseObjectPtr CPivotTableLocation::toBin()
	{
		auto ptr1(new XLSB::SXLOCATION);
		XLS::BaseObjectPtr objectPtr(ptr1);
		auto ptr(new XLSB::BeginSXLocation);
		ptr1->m_BrtBeginSXLocation = XLS::BaseObjectPtr{ptr};

		if(m_oColPageCount.IsInit())
			ptr->ccolPage = m_oColPageCount->GetValue();
        else
            ptr->ccolPage = 0;
		if(m_oFirstDataCol.IsInit())
			ptr->colFirstData = m_oFirstDataCol->GetValue();
		if(m_oFirstDataRow.IsInit())
			ptr->rwFirstData = m_oFirstDataRow->GetValue();
		if(m_oFirstHeaderRow.IsInit())
			ptr->rwFirstHead = m_oFirstHeaderRow->GetValue();
		if(m_oRowPageCount.IsInit())
			ptr->crwPage = m_oRowPageCount->GetValue();
        else
            ptr->crwPage = 0;
		if(m_oRef.IsInit())
			ptr->rfxGeom = m_oRef.get();
		return objectPtr;
	}

    void CPivotTableLocation::ReadAttributes(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::BeginSXLocation*>(obj.get());

        if(ptr != nullptr)
        {
            m_oColPageCount     = ptr->ccolPage;
            m_oFirstDataCol     = ptr->colFirstData;
            m_oFirstHeaderRow   = ptr->rwFirstHead;

            m_oFirstDataRow     = ptr->rwFirstData;

            m_oRowPageCount     = ptr->crwPage;

            if(!ptr->rfxGeom.toString().empty())
                m_oRef = ptr->rfxGeom.toString(true, true);
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
	XLS::BaseObjectPtr CPivotTableStyleInfo::toBin()
	{
		auto ptr(new XLSB::TableStyleClient);
		XLS::BaseObjectPtr objectPtr(ptr);
		if(m_oShowColHeaders.IsInit())
			ptr->fColumnHeaders = m_oShowColHeaders.get();
		if(m_oShowRowHeaders.IsInit())
			ptr->fRowHeaders = m_oShowRowHeaders.get();
		if(m_oShowColStripes.IsInit())
			ptr->fColumnStripes = m_oShowColStripes.get();
		else
			ptr->fColumnStripes = false;
		if(m_oShowRowStripes.IsInit())
			ptr->fRowStripes = m_oShowRowStripes.get();
		else
			ptr->fRowStripes = false;
		if(m_oShowLastColumn.IsInit())
			ptr->fLastColumn = m_oShowLastColumn.get();
		if(m_oName.IsInit())
			ptr->stStyleName = m_oName.get();
		else
			ptr->stStyleName = 0xFFFFFFFF;

		return objectPtr;
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
	XLS::BaseObjectPtr CPivotCacheDefinitionFile::WriteBin() const
	{

        if(m_oPivotCashDefinition.IsInit())
        {
            auto pivotCacheDefStream = m_oPivotCashDefinition->toBin();
            return pivotCacheDefStream;
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
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
        if ((xlsb) && (xlsb->m_bWriteToXlsb) && m_oPivotCashDefinition.IsInit())
		{
			XLS::BaseObjectPtr object = WriteBin();
			xlsb->WriteBin(oPath, object.get());
		}
		else
		{
			if(m_oPivotCashDefinition.IsInit())
			{
				NSStringUtils::CStringBuilder sXml;

				sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
				m_oPivotCashDefinition->toXML(sXml);

				std::wstring sPath = oPath.GetPath();
				NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());
			}
			else if(m_nDataLength > 0 && m_pData)
			{
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(oPath.GetPath());
				oFile.WriteFile(m_pData, m_nDataLength);
				oFile.CloseFile();
			}
		}
		oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		IFileContainer::Write( oPath, oDirectory, oContent );
	}
	const OOX::FileType CPivotCacheDefinitionFile::type() const
	{
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
		if ((xlsb) && (xlsb->m_bWriteToXlsb))
		{
			return OOX::SpreadsheetBin::FileTypes::PivotCacheDefinitionBin;
		}
		return OOX::Spreadsheet::FileTypes::PivotCacheDefinition;
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
        if(m_oHierarchies.IsInit())
            m_oHierarchies->toXML(writer);
        if(m_oDimensions.IsInit())
            m_oDimensions->toXML(writer);
        if(m_oMeasureGroups.IsInit())
            m_oMeasureGroups->toXML(writer);
        if(m_oMaps.IsInit())
            m_oMaps->toXML(writer);
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
			else if (L"cacheHierarchies" == sName)	m_oHierarchies = oReader;
            else if (L"dimensions" == sName)	m_oDimensions = oReader;
            else if (L"maps" == sName)          m_oMaps = oReader;
            else if (L"measureGroups" == sName) m_oMeasureGroups = oReader;
			else if (L"extLst" == sName)		m_oExtLst = oReader;
		}
	}
	XLS::BaseObjectPtr CPivotCacheDefinition::toBin()
	{
		auto ptr(new XLSB::PivotCacheDefStream);
		XLS::BaseObjectPtr objectPtr(ptr);
		ptr->m_BrtBeginPivotCacheDef = writeAttributes();
        if(m_oCacheFields.IsInit())
            ptr->m_PCDFIELDS = m_oCacheFields->toBin();
		if(m_oCacheSource.IsInit())
			ptr->m_PCDSOURCE = m_oCacheSource->toBin();
        if(m_oHierarchies.IsInit())
            ptr->m_PCDHIERARCHIES = m_oHierarchies->toBin();
        if(m_oDimensions.IsInit())
            ptr->m_DIMS = m_oDimensions->toBin();
        if(m_oMeasureGroups.IsInit())
            ptr->m_MGS = m_oMeasureGroups->toBin();
        if(m_oMaps.IsInit())
            ptr->m_MGMAPS = m_oMaps->toBin();
		if(m_oExtLst.IsInit())
			ptr->m_FRTPIVOTCACHEDEF = m_oExtLst->toBinPivotCache();
		return objectPtr;
	}
	XLS::BaseObjectPtr CPivotCacheDefinition::writeAttributes()
	{
		auto ptr(new XLSB::BeginPivotCacheDef);
		XLS::BaseObjectPtr objectPtr(ptr);
		if (m_oBackgroundQuery.IsInit())
    		ptr->fBackgroundQuery = m_oBackgroundQuery.get();
		else
			ptr->fBackgroundQuery = false;

		if (m_oEnableRefresh.IsInit())
			ptr->fEnableRefresh = !m_oEnableRefresh.get();

		if (m_oRid.IsInit())
			ptr->stRelIDRecords.value = m_oRid->GetValue();
		else
			ptr->fLoadRelIDRecords = false;

		if (m_oInvalid.IsInit())
			ptr->fInvalid = m_oInvalid.get();
		else
			ptr->fInvalid = false;

		if (m_oCreatedVersion.IsInit())
			ptr->bVerCacheCreated = m_oCreatedVersion->GetValue();

		if (m_oMinRefreshableVersion.IsInit())
			ptr->bVerCacheRefreshableMin = m_oMinRefreshableVersion->GetValue();
		else 
			ptr->bVerCacheRefreshableMin = 0;
		if (m_oMissingItemsLimit.IsInit())
			ptr->citmGhostMax = m_oMissingItemsLimit->GetValue();
		else
			ptr->citmGhostMax = -1;

		if (m_oOptimizeMemory.IsInit())
			ptr->fOptimizeCache = m_oOptimizeMemory.get();
		else
			ptr->fOptimizeCache = false;

		if (m_oRecordCount.IsInit())
			ptr->cRecords = m_oRecordCount->GetValue();

		if (m_oRefreshedBy.IsInit())
            ptr->stRefreshedWho = m_oRefreshedBy.get();
		else
			ptr->stRefreshedWho = L"Aspose";

		if (m_oRefreshedDateIso.IsInit())
			ptr->xnumRefreshedDate.data.value = std::stod(m_oRefreshedDateIso->GetValue());

		if (m_oRefreshedVersion.IsInit())
			ptr->bVerCacheLastRefresh = m_oRefreshedVersion->GetValue();

		if (m_oRefreshOnLoad.IsInit())
			ptr->fRefreshOnLoad = m_oRefreshOnLoad.get();
		else
			ptr->fRefreshOnLoad = false;

		if (m_oSaveData.IsInit())
			ptr->fSaveData = m_oSaveData.get();

		if (m_oSupportAdvancedDrill.IsInit())
			ptr->fSupportAttribDrill = m_oSupportAdvancedDrill.get();
		else
			ptr->fSupportAttribDrill = false;

		if (m_oSupportSubquery.IsInit())
			ptr->fSupportSubquery = m_oSupportSubquery.get();
		else
			ptr->fSupportSubquery = false;

		if (m_oTupleCache.IsInit())
			ptr->fSheetData = m_oTupleCache.get();
		else
			ptr->fSheetData = false;

		if (m_oUpgradeOnRefresh.IsInit())
			ptr->fUpgradeOnRefresh = m_oUpgradeOnRefresh.get();
		else
			ptr->fUpgradeOnRefresh = false;

		return objectPtr;
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
				
            if(ptr->m_PCDHIERARCHIES != nullptr)
                m_oHierarchies = ptr->m_PCDHIERARCHIES;

            if(ptr->m_DIMS != nullptr)
                m_oDimensions = ptr->m_DIMS;

            if(ptr->m_MGS != nullptr)
                m_oMeasureGroups = ptr->m_MGS;

            if(ptr->m_MGMAPS != nullptr)
                m_oMaps = ptr->m_MGMAPS;
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
			if(ptr->citmGhostMax > 0)
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
		writer.WriteString(L"<cacheFields");
		WritingStringAttrInt(L"count", (int)m_arrItems.size());
        if(m_arrItems.empty())
        {
            writer.WriteString(L"/>");
            return;
        }

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

            if ( L"cacheField" == sName )
			{
				CPivotCacheField* pPivotCacheField = new CPivotCacheField();
				*pPivotCacheField = oReader;
				m_arrItems.push_back(pPivotCacheField);
			}
		}
	}
	XLS::BaseObjectPtr CPivotCacheFields::toBin()
	{
		auto ptr(new XLSB::PCDFIELDS);
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arPCDFIELD.push_back(i->toBin());
		return objectPtr;
	}
    void CPivotCacheFields::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDFIELDS*>(obj.get());

        if(ptr != nullptr)
        {
            m_oCount = (_UINT32)ptr->m_arPCDFIELD.size();

			for(auto &item : ptr->m_arPCDFIELD)
            {
                auto field = static_cast<XLSB::PCDFIELD*>(item.get());
				if(field == nullptr)
					break;
				auto BeginField = static_cast<XLSB::BeginPCDField*>(field->m_BrtBeginPCDField.get());
				if(BeginField == nullptr)
					break;
				ptr->global_info->arPivotCacheSxNames.push_back(BeginField->stFldName);
            }

            for(auto &item : ptr->m_arPCDFIELD)
            {
                m_arrItems.push_back(new CPivotCacheField(item));
            }
			ptr->global_info->arPivotCacheSxNames.clear();
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
            WritingStringNullableAttrBool2(L"databaseField",	m_oDatabaseField);
        if(!m_oSharedItems.IsInit() && !m_oFieldGroup.IsInit())
        {
            writer.WriteString(L"/>");
            return;
        }
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
	XLS::BaseObjectPtr CPivotCacheField::toBin()
	{
		auto ptr(new XLSB::PCDFIELD);
		XLS::BaseObjectPtr objectPtr(ptr);

		ptr->m_BrtBeginPCDField = writeAttributes();
        if(m_oSharedItems.IsInit() && !m_oSharedItems->m_arrItems.empty())
			ptr->m_PCDFATBL = m_oSharedItems->toBin();
		if(m_oFieldGroup.IsInit())
			ptr->m_PCDFGROUP = m_oFieldGroup->toBin();
		return objectPtr;
	}
	XLS::BaseObjectPtr CPivotCacheField::writeAttributes()
	{
		auto ptr(new XLSB::BeginPCDField);
		XLS::BaseObjectPtr objectPtr(ptr);

        if (m_oName.IsInit())
            ptr->stFldName = m_oName.get();

		if (m_oCaption.IsInit())
            ptr->stFldCaption = m_oCaption.get();
		else
			ptr->fCaption = false;

		if (m_oDatabaseField.IsInit())
			ptr->fSrcField = m_oDatabaseField.get();
		else
            ptr->fSrcField = true;

		if (m_oServerField.IsInit())
			ptr->fServerBased = m_oServerField.get();
		else
			ptr->fServerBased = false;

		if (m_oFormula.IsInit())
			ptr->fldFmla = m_oFormula.get();
		else
		{
			ptr->fLoadFmla = false;
			ptr->fldFmla.cSxName = 0;
		}

		if (m_oHierarchy.IsInit())
			ptr->ihdb = m_oHierarchy.get();
		else
			ptr->ihdb = 0;

		if (m_oMemberPropertyField.IsInit())
			ptr->fOlapMemPropField = m_oMemberPropertyField.get();
		else
			ptr->fOlapMemPropField = false;

		if (m_oPropertyName.IsInit())
			ptr->stMemPropName.value() = m_oPropertyName.get();
		else
			ptr->fLoadPropName = false;

		if (m_oSqlType.IsInit())
			ptr->wTypeSql = m_oSqlType.get();
		else
			ptr->wTypeSql = 0;

		if (m_oUniqueList.IsInit())
			ptr->fCantGetUniqueItems = !m_oUniqueList.get();
		else
			ptr->fCantGetUniqueItems = false;

		if (m_oLevel.IsInit())
			ptr->isxtl = m_oLevel->GetValue();
		else
			ptr->isxtl = 0;
		if(m_oMappingCount.IsInit())
			ptr->cIsxtmps = m_oMappingCount->GetValue();
		else
			ptr->cIsxtmps = 0;
		if(m_oNumFmtId.IsInit())
			ptr->ifmt = m_oNumFmtId->GetValue();
		else
            ptr->ifmt = 0;
		
		ptr->cbRgisxtmp = 0;

		return objectPtr;
	}
    void CPivotCacheField::fromBin(XLS::BaseObjectPtr& obj)
    {
        auto ptr = static_cast<XLSB::PCDFIELD*>(obj.get());

        if(ptr != nullptr)
        {	
			if(ptr->m_PNAMES != nullptr)
            {
                XLSB::PNAMES* names = static_cast<XLSB::PNAMES*>(ptr->m_PNAMES.get());
				for(auto i:names->m_arPNAME)
				{
					auto name = static_cast<XLSB::PNAME*>(i.get());
					XLS::_sx_name nameStruct;
					nameStruct.name = name->m_BrtBeginPName;
					ptr->global_info->arPivotSxNames.push_back(nameStruct);

				}
            }

            ReadAttributes(ptr->m_BrtBeginPCDField);

            if(ptr->m_PCDFATBL != nullptr)
                m_oSharedItems = ptr->m_PCDFATBL;

            if(ptr->m_PCDFGROUP != nullptr)
                m_oFieldGroup = ptr->m_PCDFGROUP;

			if(!ptr->global_info->arPivotSxNames.empty())
				ptr->global_info->arPivotSxNames.clear();
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
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"numFmtId", m_oNumFmtId )
		WritingElement_ReadAttributes_End( oReader )
	}
//------------------------------------
	void CSharedItems::toXML(NSStringUtils::CStringBuilder& writer) const
	{
		writer.WriteString(L"<sharedItems");
            WritingStringNullableAttrBool2(L"containsSemiMixedTypes",	m_oContainsSemiMixedTypes);
            WritingStringNullableAttrBool2(L"containsNonDate",	m_oContainsNonDate);
            WritingStringNullableAttrBool2(L"containsDate",	m_oContainsDate);
            WritingStringNullableAttrBool2(L"containsString",	m_oContainsString);
			WritingStringNullableAttrBool2(L"containsBlank",	m_oContainsBlank);
            WritingStringNullableAttrBool2(L"containsMixedTypes",	m_oContainsMixedTypes);
            WritingStringNullableAttrBool2(L"containsNumber",	m_oContainsNumber);
			WritingStringNullableAttrBool2(L"containsInteger",	m_oContainsInteger);
			WritingStringNullableAttrDouble2(L"minValue",	m_oMinValue);
			WritingStringNullableAttrDouble2(L"maxValue",	m_oMaxValue);
			WritingStringNullableAttrString(L"minDate", m_oMinDate, m_oMinDate->ToString());
			WritingStringNullableAttrString(L"maxDate", m_oMaxDate, m_oMaxDate->ToString());
            if(!m_arrItems.empty())
            {
                WritingStringAttrInt(L"count", (int)m_arrItems.size());
            }
            WritingStringNullableAttrBool2(L"longText",	m_oLongText);
        if(m_arrItems.empty())
        {
            writer.WriteString(L"/>");
            return;
        }
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
			{
				CPivotBooleanValue* pPivotBooleanValue = new CPivotBooleanValue();
				*pPivotBooleanValue = oReader;
				m_arrItems.push_back(pPivotBooleanValue);
			}
			else if ( L"d" == sName )
			{
				CPivotDateTimeValue* pPivotDateTimeValue = new CPivotDateTimeValue();
				*pPivotDateTimeValue = oReader;
				m_arrItems.push_back(pPivotDateTimeValue);
			}
			else if ( L"e" == sName )
			{
				CPivotErrorValue* pPivotErrorValue = new CPivotErrorValue();
				*pPivotErrorValue = oReader;
				m_arrItems.push_back(pPivotErrorValue);
			}
			else if ( L"m" == sName )
			{
				CPivotNoValue* pPivotNoValue = new CPivotNoValue();
				*pPivotNoValue = oReader;
				m_arrItems.push_back(pPivotNoValue);
			}
			else if ( L"n" == sName )
			{
				CPivotNumericValue* pPivotNumericValue = new CPivotNumericValue();
				*pPivotNumericValue = oReader;
				m_arrItems.push_back(pPivotNumericValue);
			}
			else if ( L"s" == sName )
			{
				CPivotCharacterValue* pPivotCharacterValue = new CPivotCharacterValue();
				*pPivotCharacterValue = oReader;
				m_arrItems.push_back(pPivotCharacterValue);
			}
		}
	}
	XLS::BaseObjectPtr CSharedItems::toBin()
	{
		auto ptr(new XLSB::PCDFATBL);
		XLS::BaseObjectPtr objectPtr(ptr);
		bool hasBolean = false;
		bool hasStr = false;
		bool hasDate = false;
		bool hasMissing = false;
		bool hasNumber = false;
		bool hasError = false;

		for(auto i:m_arrItems)
		{
            
            if(i->getType() == et_x_PivotBooleanValue)
			{
				auto valueBool = static_cast<CPivotBooleanValue*>(i);
				XLS::BaseObjectPtr element = valueBool->toBin();
				ptr->m_arSource.push_back(element);
				hasBolean = true;
				continue;
			}
            else if(i->getType() == et_x_PivotNoValue)
			{
				auto noVal = static_cast<CPivotNoValue*>(i);
				XLS::BaseObjectPtr element = noVal->toBin();
				ptr->m_arSource.push_back(element);
				hasMissing = true;
				continue;
			}
            else if(i->getType() == et_x_PivotNumericValue)
			{
				auto numVal = static_cast<CPivotNumericValue*>(i);
				XLS::BaseObjectPtr element = numVal->toBin();
				ptr->m_arSource.push_back(element);
				hasNumber = true;
				continue;
			}
            else if(i->getType() == et_x_PivotCharacterValue)
			{
				auto charVal = static_cast<CPivotCharacterValue*>(i);
				XLS::BaseObjectPtr element = charVal->toBin();
				ptr->m_arSource.push_back(element);
				hasStr = true;
				continue;
			}
            else if(i->getType() == et_x_PivotDateTimeValue)
			{
				auto dateValue = static_cast<CPivotDateTimeValue*>(i);
				XLS::BaseObjectPtr element = dateValue->toBin();
				ptr->m_arSource.push_back(element);
				hasDate = true;
				continue;
			}
            else if(i->getType() == et_x_PivotErrorValue)
			{
				auto errorVal = static_cast<CPivotErrorValue*>(i);
				XLS::BaseObjectPtr element = errorVal->toBin();
				ptr->m_arSource.push_back(element);
				hasError = true;
				continue;
			}
			else
			{
				auto missingVal(new XLSB::PCDIMissing);
				XLS::BaseObjectPtr element(missingVal);
				ptr->m_arSource.push_back(element);
				hasMissing = true;
				continue;
			}
		}
		unsigned char flags = 0;
		SETBIT(flags,0, hasBolean);
		SETBIT(flags,1, hasStr);
		SETBIT(flags,2, hasDate);
		SETBIT(flags,3, hasMissing);
		SETBIT(flags,4, hasNumber);
		SETBIT(flags,5, hasError);
		ptr->m_BrtBeginPCDFAtbl = writeAttributes(flags);
		return objectPtr;
	}

	XLS::BaseObjectPtr CSharedItems::writeAttributes(const unsigned char flags)
	{
		auto ptr(new XLSB::BeginPCDFAtbl);
		XLS::BaseObjectPtr objectPtr(ptr);

		if(m_oContainsBlank.IsInit())
		  	ptr->fHasBlankItem = m_oContainsBlank.get();
        else
            ptr->fHasBlankItem = false;
		if(m_oContainsDate.IsInit())
            ptr->fDateInField = m_oContainsDate.get();
        else
            ptr->fDateInField = false;
		if(m_oContainsInteger.IsInit())
            ptr->fIntField = m_oContainsInteger.get();
        else
            ptr->fIntField = false;
		if(m_oContainsMixedTypes.IsInit())
            ptr->fMixedTypesIgnoringBlanks = m_oContainsMixedTypes.get();
        else
            ptr->fMixedTypesIgnoringBlanks = false;
		if(m_oContainsNonDate.IsInit())
			ptr->fNonDates = m_oContainsNonDate.get();
        else
            ptr->fNonDates = false;
		if(m_oContainsNumber.IsInit())
			ptr->fNumField = m_oContainsNumber.get();
        else
            ptr->fNumField = false;
		if(m_oContainsSemiMixedTypes.IsInit())
			ptr->fTextEtcField = m_oContainsSemiMixedTypes.get();
        else
            ptr->fTextEtcField = false;
        if(m_oContainsString.IsInit())
			ptr->fHasTextItem = m_oContainsString.get();
        else
            ptr->fHasTextItem = false;
		if(m_oLongText.IsInit())
			ptr->fHasLongTextItem = m_oLongText.get();
        else
            ptr->fHasLongTextItem = false;
		if(m_oCount.IsInit())
			ptr->citems = m_oCount->GetValue();
		else
			ptr->citems = 0;
		if(m_oMinDate.IsInit() && m_oMaxDate.IsInit())
		{
			ptr->xnumMin.data.value = getExcelTimeFromDate(m_oMinDate->GetValue());
			ptr->xnumMax.data.value = getExcelTimeFromDate(m_oMaxDate->GetValue());
		}
		else if(m_oMinValue.IsInit() && m_oMaxValue.IsInit())
		{
			if(m_oMinValue.IsInit())
				ptr->xnumMin.data.value = m_oMinValue.get();
			if(m_oMaxValue.IsInit())
				ptr->xnumMax.data.value = m_oMaxValue.get();
		}
		else
		{
			ptr->fNumMinMaxValid = false;
		}
		bool hasBolean = GETBIT(flags,0);
		bool hasStr = GETBIT(flags,1);
		bool hasDate = GETBIT(flags,2);
		bool hasMissing = GETBIT(flags,3);
		bool hasNumber = GETBIT(flags,4);
		bool hasError = GETBIT(flags,5);
		if(!hasDate)
			ptr->fNonDates = true;
		if(!hasDate && hasNumber)
			ptr->fNumField = true;
		if(hasStr || hasError || hasBolean)
		{
			ptr->fTextEtcField = true;
			ptr->fHasTextItem = true;
		}
		if(hasMissing)
		{
			ptr->fHasBlankItem = true;
			ptr->fTextEtcField = true;
		}
		if(hasDate && hasNumber || hasNumber && ptr->fHasTextItem ||hasDate &&  ptr->fHasTextItem)
		{
			ptr->fMixedTypesIgnoringBlanks = true;
		}
		return objectPtr;
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
			if(m_oCount->GetValue() != m_arrItems.size())
				*m_oCount = m_arrItems.size();
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
                m_oMinDate              = getDateFromExcelTime(ptr->xnumMin.data.value);
                m_oMaxDate              = getDateFromExcelTime(ptr->xnumMax.data.value);
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
				m_arrItems.push_back(PPTX::CreatePtrXmlContent<CSharedItemsIndex>(oReader));
		}
	}
	XLS::BaseObjectPtr CDiscreteGroupingProperties::toBin()
	{
		auto ptr(new XLSB::PCDFGDISCRETE);
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arBrtPCDIIndex.push_back(i->toBinItemIndex());
		return objectPtr;
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
			{
				CPivotBooleanValue* pPivotBooleanValue = new CPivotBooleanValue();
				*pPivotBooleanValue = oReader;
				m_arrItems.push_back(pPivotBooleanValue);
			}
			else if ( L"d" == sName )
			{
				CPivotDateTimeValue* pPivotDateTimeValue = new CPivotDateTimeValue();
				*pPivotDateTimeValue = oReader;
				m_arrItems.push_back(pPivotDateTimeValue);
			}
			else if ( L"e" == sName )
			{
				CPivotErrorValue* pPivotErrorValue = new CPivotErrorValue();
				*pPivotErrorValue = oReader;
				m_arrItems.push_back(pPivotErrorValue);
			}
			else if ( L"m" == sName )
			{
				CPivotNoValue* pPivotNoValue = new CPivotNoValue();
				*pPivotNoValue = oReader;
				m_arrItems.push_back(pPivotNoValue);
			}
			else if ( L"n" == sName )
			{
				CPivotNumericValue* pPivotNumericValue = new CPivotNumericValue();
				*pPivotNumericValue = oReader;
				m_arrItems.push_back(pPivotNumericValue);
			}
			else if ( L"s" == sName )
			{
				CPivotCharacterValue* pPivotCharacterValue = new CPivotCharacterValue();
				*pPivotCharacterValue = oReader;
				m_arrItems.push_back(pPivotCharacterValue);
			}
		}
	}
	XLS::BaseObjectPtr COLAPGroupItems::toBin()
	{
		auto ptr(new XLSB::PCDFGITEMS);
		XLS::BaseObjectPtr  objectPtr(ptr);
		for(auto i:m_arrItems)
		{
            switch(i->getType())
            {
                case et_x_PivotBooleanValue:
                {
                    auto boolVal = static_cast<CPivotBooleanValue*>(i);
                    auto ptr1(new XLSB::PCDI);
                    ptr1->m_source = boolVal->toBin();
                    ptr->m_arPCDI.push_back(XLS::BaseObjectPtr{ptr1});
                    break;
                }
                case et_x_PivotDateTimeValue:
                {
                    auto dataValue = static_cast<CPivotDateTimeValue*>(i);
                    auto ptr1(new XLSB::PCDI);
                    ptr1->m_source = dataValue->toBin();
                    ptr->m_arPCDI.push_back(XLS::BaseObjectPtr{ptr1});
                    break;
                }
                case et_x_PivotErrorValue:
                {
                    auto errorValue = static_cast<CPivotErrorValue*>(i);
                    auto ptr1(new XLSB::PCDI);
                    ptr1->m_source = errorValue->toBin();
                    ptr->m_arPCDI.push_back(XLS::BaseObjectPtr{ptr1});
                    break;
                }
                case et_x_PivotNoValue:
                {
                    auto noVal = static_cast<CPivotNoValue*>(i);
                    auto ptr1(new XLSB::PCDI);
                    ptr1->m_source = noVal->toBin();
                    ptr->m_arPCDI.push_back(XLS::BaseObjectPtr{ptr1});
                    break;
                }
                case et_x_PivotNumericValue:
                {
                    auto numericVal = static_cast<CPivotNumericValue*>(i);
                    auto ptr1(new XLSB::PCDI);
                    ptr1->m_source = numericVal->toBin();
                    ptr->m_arPCDI.push_back(XLS::BaseObjectPtr{ptr1});
                    break;
                }
                case et_x_PivotCharacterValue:
                {
                    auto charVal = static_cast<CPivotCharacterValue*>(i);
                    auto ptr1(new XLSB::PCDI);
                    ptr1->m_source = charVal->toBin();
                    ptr->m_arPCDI.push_back(XLS::BaseObjectPtr{ptr1});
                    break;
                }
                default:
                    break;
            }
		}
		return objectPtr;
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
        writer.WriteString(L"<rangePr");
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
	XLS::BaseObjectPtr CRangeGroupingProperties::toBin()
	{
		auto ptr1(new XLSB::PCDFGRANGE);
		XLS::BaseObjectPtr objectPtr(ptr1);
		auto ptr(new XLSB::BeginPCDFGRange);
		ptr1->m_BrtBeginPCDFGRange = XLS::BaseObjectPtr{ptr};

		if(m_oAutoStart.IsInit())
			ptr->fAutoStart = m_oAutoStart.get();
		if(m_oAutoEnd.IsInit())
			ptr->fAutoEnd = m_oAutoEnd.get();
		if(m_oGroupInterval.IsInit())
			ptr->xnumBy.data.value = m_oGroupInterval.get();
		else
			ptr->xnumBy.data.value = 1;
		if(m_oStartDate.IsInit() && m_oEndDate.IsInit())
		{
			ptr->xnumStart.data.value = getExcelTimeFromDate(m_oStartDate->GetValue());
			ptr->xnumEnd.data.value = getExcelTimeFromDate(m_oEndDate->GetValue());
		}
		else
		{
			if(m_oStartNum.IsInit())
				ptr->xnumStart.data.value = m_oStartNum.get();
			if(m_oEndNum.IsInit())
				ptr->xnumEnd.data.value = m_oEndNum.get();
		}
		ptr->iByType = 0x00;
		if(m_oGroupBy.IsInit())
		{
			if (m_oGroupBy == SimpleTypes::Spreadsheet::EValuesGroupBy::groupByNumericRanges) ptr->iByType = 0x00;
			else if (m_oGroupBy == SimpleTypes::Spreadsheet::EValuesGroupBy::groupBySeconds) ptr->iByType = 0x01;
			else if (m_oGroupBy == SimpleTypes::Spreadsheet::EValuesGroupBy::groupByMinutes) ptr->iByType = 0x02;
			else if (m_oGroupBy == SimpleTypes::Spreadsheet::EValuesGroupBy::groupByHours) ptr->iByType = 0x03;
			else if (m_oGroupBy == SimpleTypes::Spreadsheet::EValuesGroupBy::groupByDays) ptr->iByType = 0x04;
			else if (m_oGroupBy == SimpleTypes::Spreadsheet::EValuesGroupBy::groupByMonths) ptr->iByType = 0x05;
			else if (m_oGroupBy == SimpleTypes::Spreadsheet::EValuesGroupBy::groupByQuarters) ptr->iByType = 0x06;
			else if (m_oGroupBy == SimpleTypes::Spreadsheet::EValuesGroupBy::groupByYears) ptr->iByType = 0x07;
		}
		if(ptr->iByType > 0x00)
			ptr->fDates = true;
		return objectPtr;
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
                m_oStartDate = getDateFromExcelTime(ptr->xnumStart.data.value);
                m_oEndDate   = getDateFromExcelTime(ptr->xnumEnd.data.value);
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
            WritingElement_ReadAttributes_Read_if		( oReader, L"groupBy", m_oGroupBy )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"autoStart", m_oAutoStart )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"autoEnd", m_oAutoEnd )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"startDate", m_oStartDate )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"endDate", m_oEndDate )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"startNum", m_oStartNum )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"endNum", m_oEndNum )
            WritingElement_ReadAttributes_Read_else_if	( oReader, L"groupInterval", m_oGroupInterval )
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
		if(!m_arrItems.empty())
		{
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
		else
		writer.WriteString(L"/>");
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
				m_arrItems.push_back(PPTX::CreatePtrXmlContent<CMemberPropertyIndex>(oReader));
		}
	}
	XLS::BaseObjectPtr CPivotCharacterValue::toBin()
	{
		if(m_oCalculated.IsInit() || m_oUnused.IsInit() || m_oCount.IsInit() || !m_arrItems.empty() || m_oCaption.IsInit())
		{
			auto ptr(new XLSB::PCDIAString);
			auto ptr1(new XLSB::PCDIA);
            ptr1->m_source = XLS::BaseObjectPtr{ptr1};
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_oValue.IsInit())
				ptr->st = m_oValue.get();
			else
				ptr->st.setSize(0);
			if(m_oCaption.IsInit())
				ptr->info.stCaption = m_oCaption.get();
			else
				ptr->info.fCaption = false;
			if(m_oCalculated.IsInit())
				ptr->info.fFmla = m_oCalculated.get();
			else
				ptr->info.fFmla = false;
			if(m_oUnused.IsInit())
				ptr->info.fGhost = m_oUnused.get();
			else
				ptr->info.fGhost = false;
			if(m_oCount.IsInit())
				ptr->info.cIMemProps = m_oCount->GetValue();
			else
				ptr->info.cIMemProps = m_arrItems.size();
			if(m_oValue.IsInit())
				ptr->st = m_oValue.get();
			else
				ptr->st.setSize(0); 
			for(auto i:m_arrItems)
				ptr->info.rgIMemProps.push_back(i->m_oV.get());
			return objectPtr;
		}
		else
		{
			auto ptr(new XLSB::PCDIString);
			auto ptr1(new XLSB::PCDI);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			if(m_oValue.IsInit())
				ptr->st = m_oValue.get();
			return objectPtr;
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
				if(ptr->info.fFmla)
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
		if(!m_arrItems.empty())
		{
			writer.WriteString(L">");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			writer.WriteString(L"</e>");
		}
		else
		{
			writer.WriteString(L"/>");
		}
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
				m_arrItems.push_back(PPTX::CreatePtrXmlContent<CMemberPropertyIndex>(oReader));
		}
	}
	XLS::BaseObjectPtr CPivotErrorValue::toBin()
	{
		if(m_oCalculated.IsInit() || m_oUnused.IsInit() || m_oCount.IsInit() || !m_arrItems.empty() || m_oCaption.IsInit())
		{
			auto ptr(new XLSB::PCDIAError);
			auto ptr1(new XLSB::PCDIA);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			if(m_oValue.IsInit())
			{
				if (m_oValue == L"#NULL!") ptr->err = 0x00;
				else if (m_oValue == L"#DIV/0!") ptr->err = 0x07;
				else if (m_oValue == L"#VALUE!") ptr->err = 0x0F;
				else if (m_oValue == L"#REF!") ptr->err = 0x17;
				else if (m_oValue == L"#NAME?") ptr->err = 0x1D;
				else if (m_oValue == L"#NUM!") ptr->err = 0x24;
				else if (m_oValue == L"#N/A") ptr->err = 0x2A;
				else if (m_oValue == L"#GETTING_DATA") ptr->err = 0x2B;
			}
			if(m_oCaption.IsInit())
				ptr->info.stCaption = m_oCaption.get();
			else
				ptr->info.fCaption = false;
			if(m_oCalculated.IsInit())
				ptr->info.fFmla = m_oCalculated.get();
			else
				ptr->info.fFmla = false;
			if(m_oUnused.IsInit())
				ptr->info.fGhost = m_oUnused.get();
			else
				ptr->info.fGhost = false;
			if(m_oCount.IsInit())
				ptr->info.cIMemProps = m_oCount->GetValue();
			else
				ptr->info.cIMemProps = m_arrItems.size();
			for(auto i:m_arrItems)
				ptr->info.rgIMemProps.push_back(i->m_oV.get());
			return objectPtr;
		}
		else
		{
			auto ptr(new XLSB::PCDIError);
			auto ptr1(new XLSB::PCDI);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			if(m_oValue.IsInit())
			{
				if (m_oValue == L"#NULL!") ptr->err = 0x00;
				else if (m_oValue == L"#DIV/0!") ptr->err = 0x07;
				else if (m_oValue == L"#VALUE!") ptr->err = 0x0F;
				else if (m_oValue == L"#REF!") ptr->err = 0x17;
				else if (m_oValue == L"#NAME?") ptr->err = 0x1D;
				else if (m_oValue == L"#NUM!") ptr->err = 0x24;
				else if (m_oValue == L"#N/A") ptr->err = 0x2A;
				else if (m_oValue == L"#GETTING_DATA") ptr->err = 0x2B;
			}
			return objectPtr;
		}
	}
    void CPivotErrorValue::toBin(XLS::StreamCacheWriterPtr& writer)
    {
         auto record = writer->getNextRecord(XLSB::rt_PCDIError);
         BYTE errVal = 0;
         if(m_oValue.IsInit())
         {
             if (m_oValue == L"#NULL!") errVal = 0x00;
             else if (m_oValue == L"#DIV/0!") errVal = 0x07;
             else if (m_oValue == L"#VALUE!") errVal = 0x0F;
             else if (m_oValue == L"#REF!") errVal = 0x17;
             else if (m_oValue == L"#NAME?") errVal = 0x1D;
             else if (m_oValue == L"#NUM!") errVal = 0x24;
             else if (m_oValue == L"#N/A") errVal = 0x2A;
             else if (m_oValue == L"#GETTING_DATA") errVal = 0x2B;
         }
         *record << errVal;
         writer->storeNextRecord(record);
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
				if(ptr->info.fFmla)
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
		if(!m_arrItems.empty())
		{
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
		else
			writer.WriteString(L"/>");
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
				m_arrItems.push_back(PPTX::CreatePtrXmlContent<CMemberPropertyIndex>(oReader));
		}
	}
    void CPivotNumericValue::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
    }
	XLS::BaseObjectPtr CPivotNumericValue::toBin()
	{
		if(m_oCalculated.IsInit() || m_oUnused.IsInit() || m_oCount.IsInit() || !m_arrItems.empty() || m_oCaption.IsInit())
		{
			auto ptr(new XLSB::PCDIANumber);
			auto ptr1(new XLSB::PCDIA);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			if(m_oCaption.IsInit())
				ptr->info.stCaption = m_oCaption.get();
			else
				ptr->info.fCaption = false;
			if(m_oCalculated.IsInit())
				ptr->info.fFmla = m_oCalculated.get();
			else
				ptr->info.fFmla = false;
			if(m_oUnused.IsInit())
				ptr->info.fGhost = m_oUnused.get();
			else
				ptr->info.fGhost = false;
			if(m_oCount.IsInit())
				ptr->info.cIMemProps = m_oCount->GetValue();
			else
				ptr->info.cIMemProps = m_arrItems.size();
			if(m_oValue.IsInit())
				ptr->xnum.data.value = m_oValue.get();
			for(auto i:m_arrItems)
				ptr->info.rgIMemProps.push_back(i->m_oV.get());
			return objectPtr;
		}
		else
		{
			auto ptr(new XLSB::PCDINumber);
            auto ptr1(new XLSB::PCDI);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			if(m_oValue.IsInit())
				ptr->xnum.data.value = m_oValue.get();
			return objectPtr;
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
				if(ptr->info.fFmla)
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
        if(m_arrItems.empty())
        {
            writer.WriteString(L"/>");
            return;
        }
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
				m_arrItems.push_back(PPTX::CreatePtrXmlContent<CMemberPropertyIndex>(oReader));
		}
	}
	XLS::BaseObjectPtr CPivotDateTimeValue::toBin()
	{
		if(m_oCalculated.IsInit() || m_oUnused.IsInit() || m_oCount.IsInit() || !m_arrItems.empty() || m_oCaption.IsInit())
		{
			auto ptr(new XLSB::PCDIADatetime);
			auto ptr1(new XLSB::PCDIA);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			if(m_oCaption.IsInit())
				ptr->info.stCaption = m_oCaption.get();
			else
				ptr->info.fCaption = false;
			if(m_oCalculated.IsInit())
				ptr->info.fFmla = m_oCalculated.get();
			else
				ptr->info.fFmla = false;
			if(m_oUnused.IsInit())
				ptr->info.fGhost = m_oUnused.get();
			else
				ptr->info.fGhost = false;
			if(m_oCount.IsInit())
				ptr->info.cIMemProps = m_oCount->GetValue();
			if(m_oValue.IsInit())
				ptr->datetime.fromString(m_oValue->GetValue());
            ptr->datetime.yr += 1900;
            ptr->datetime.mon += 1;
			for(auto i:m_arrItems)
				ptr->info.rgIMemProps.push_back(i->m_oV.get());
			return objectPtr;
		}
		else
		{
			auto ptr(new XLSB::PCDIDatetime);
			auto ptr1(new XLSB::PCDI);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			if(m_oValue.IsInit())
				ptr->datetime.fromString(m_oValue->GetValue());
            ptr->datetime.yr += 1900;
            ptr->datetime.mon += 1;

			return objectPtr;
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
				if(ptr->info.fFmla)
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
        if(m_arrItems.empty())
        {
            writer.WriteString(L"/>");
            return;
        }
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
				m_arrItems.push_back(PPTX::CreatePtrXmlContent<CMemberPropertyIndex>(oReader));
		}
    }
	XLS::BaseObjectPtr CPivotBooleanValue::toBin()
	{
		if(m_oCalculated.IsInit() || m_oUnused.IsInit() || m_oCount.IsInit() || !m_arrItems.empty() || m_oCaption.IsInit())
		{
			auto ptr(new XLSB::PCDIABoolean);
			auto ptr1(new XLSB::PCDIA);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			if(m_oValue.IsInit())
				ptr->f = m_oValue.get();
			if(m_oCalculated.IsInit())
				ptr->info.fFmla = m_oCalculated.get();
			else 
				ptr->info.fFmla = false;
			if(m_oCaption.IsInit())
				ptr->info.stCaption = m_oCaption.get();
			else 
				ptr->info.fCaption = false;
			if(m_oUnused.IsInit())
				ptr->info.fGhost = m_oUnused.get();
			else
				ptr->info.fGhost = false;
			if(m_oCount.IsInit())
				ptr->info.cIMemProps = m_oCount->GetValue();
			else
				ptr->info.cIMemProps = m_arrItems.size();
			for(auto i:m_arrItems)
				ptr->info.rgIMemProps.push_back(i->m_oV.get());
			return objectPtr;
		}
		else
		{
			auto ptr(new XLSB::PCDIBoolean);
			auto ptr1(new XLSB::PCDI);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			nullable_bool boolVal;
			if(m_oValue.IsInit())
				boolVal = m_oValue.get();
			if(boolVal.IsInit())
				ptr->f = boolVal.get();
			else
				ptr->f = false;
			return objectPtr;
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
				if(ptr->info.fFmla)
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
		if(!m_arrItems.empty())
		{
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
		else
			writer.WriteString(L"/>");
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
				m_arrItems.push_back(PPTX::CreatePtrXmlContent<CMemberPropertyIndex>(oReader));
		}
	}
    void CPivotNoValue::fromBin(XLS::BaseObjectPtr& obj)
    {
        ReadAttributes(obj);
    }
	XLS::BaseObjectPtr CPivotNoValue::toBin()
	{
		if(m_arrItems.empty() || m_oBold.IsInit() || m_oItalic.IsInit() || m_oStrike.IsInit() || m_oUnderline.IsInit() || m_oFormatIndex.IsInit()
			|| m_oBackColor.IsInit() || m_oForeColor.IsInit())
		{
			auto ptr(new XLSB::PCDIMissing);
			auto ptr1(new XLSB::PCDI);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			return objectPtr;
		}
		else
		{
			auto ptr(new XLSB::PCDIAMissing);
			auto ptr1(new XLSB::PCDIA);
            ptr1->m_source = XLS::BaseObjectPtr{ptr};
			XLS::BaseObjectPtr objectPtr(ptr1);
			if(m_oCaption.IsInit())
				ptr->info.stCaption = m_oCaption.get();
			if(m_oCalculated.IsInit())
				ptr->info.fFmla = m_oCalculated.get();
			if(m_oUnused.IsInit())
				ptr->info.fGhost = m_oUnused.get();
			if(m_oCount.IsInit())
				ptr->info.cIMemProps = m_oCount->GetValue();
			for(auto i:m_arrItems)
				ptr->info.rgIMemProps.push_back(i->m_oV.get());
			return objectPtr;
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
				if(ptr->info.fFmla)
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
			WritingStringNullableAttrString(L"type", m_oType, m_oType->ToString());
            WritingStringNullableAttrInt(L"connectionId", m_oConnectionId, m_oConnectionId->GetValue());
        if(!m_oWorksheetSource.IsInit() && !m_oConsolidation.IsInit())
        {
            writer.WriteString(L"/>");
            return;
        }
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
	XLS::BaseObjectPtr CPivotCacheSource::toBin()
	{
		auto ptr(new XLSB::PCDSOURCE);
		XLS::BaseObjectPtr objectPtr(ptr);
		auto ptr1(new XLSB::BeginPCDSource);
		ptr->m_BrtBeginPCDSource = XLS::BaseObjectPtr{ptr1};
		if(m_oType.IsInit())
		{
			if(m_oType == SimpleTypes::Spreadsheet::ESourceCacheType::typeSourceWorksheet)
				ptr1->iSrcType = 0x00000000;
			if(m_oType == SimpleTypes::Spreadsheet::ESourceCacheType::typeSourceExternal)
				ptr1->iSrcType = 0x00000001;
			if(m_oType == SimpleTypes::Spreadsheet::ESourceCacheType::typeSourceConsolidation)
				ptr1->iSrcType = 0x00000002;
			if(m_oType == SimpleTypes::Spreadsheet::ESourceCacheType::typeSourceScenario)
				ptr1->iSrcType = 0x00000003;
		}
		if(m_oConnectionId.IsInit())
            ptr1->dwConnID = m_oConnectionId->GetValue();
        else
            ptr1->dwConnID = 0;

		if(m_oWorksheetSource.IsInit())
			ptr->m_PCDSRANGE = m_oWorksheetSource->toBin();
		if(m_oConsolidation.IsInit())
			ptr->m_PCDSCONSOL = m_oConsolidation->toBin();
		return objectPtr;
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
	XLS::BaseObjectPtr CWorksheetSource::toBin()
	{
		auto ptr1(new XLSB::PCDSRANGE);
		XLS::BaseObjectPtr objectPtr(ptr1);
		auto ptr(new XLSB::BeginPCDSRange);
		ptr1->m_BrtBeginPCDSRange = XLS::BaseObjectPtr{ptr};

		if(m_oSheet.IsInit())
			ptr->sheetName = m_oSheet.get();
		else
			ptr->fLoadSheet = false;
		if(m_oRef.IsInit())
			ptr->range.fromString(m_oRef.get());
		if(m_oName.IsInit())
			ptr->namedRange = m_oName.get();
		else
			ptr->fName = false;
		if(m_oRid.IsInit())
			ptr->relId.value = m_oRid->GetValue();
		else
			ptr->fLoadRelId = false;
		ptr->fBuiltIn = false;
		return objectPtr;
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

            if(!ptr->range.toString().empty() && ptr->range.toString(true, true) != L"A1")
                m_oRef       = ptr->range.toString(true, true);

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
			{
				CPageItems* pPageItems = new CPageItems();
				*pPageItems = oReader;
				m_arrItems.push_back(pPageItems);
			}
		}
	}
	XLS::BaseObjectPtr CPageItemValues::toBin()
	{
		auto ptr(new XLSB::PCDSCPAGES);
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arPCDSCPAGE.push_back(i->toBin());
		return objectPtr;
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
			{
				CPageItem* pPageItem = new CPageItem();
				*pPageItem = oReader;
				m_arrItems.push_back(pPageItem);
			}
		}
	}
	XLS::BaseObjectPtr CPageItems::toBin()
	{
		auto ptr(new XLSB::PCDSCPAGE);
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arPCDSCPITEM.push_back(i->toBin());
		return objectPtr;
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
	XLS::BaseObjectPtr CPageItem::toBin()
	{
		auto ptr(new XLSB::PCDSCPITEM);
		XLS::BaseObjectPtr objectPtr(ptr);
		auto ptr1(new XLSB::BeginPCDSCPItem);
		ptr->m_BrtBeginPCDSCPItem = XLS::BaseObjectPtr{ptr1};
		if(m_oName.IsInit())
			ptr1->stName = m_oName.get();

		return objectPtr;
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
			{
				CRangeSet* pRangeSet = new CRangeSet();
				*pRangeSet = oReader;
				m_arrItems.push_back(pRangeSet);
			}
		}
	}
	XLS::BaseObjectPtr CRangeSets::toBin()
	{
		auto ptr(new XLSB::PCDSCSETS);
		XLS::BaseObjectPtr objectPtr(ptr);
		for(auto i:m_arrItems)
			ptr->m_arPCDSCSET.push_back(i->toBin());
		return objectPtr;
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
	XLS::BaseObjectPtr CRangeSet::toBin()
	{
		auto ptr(new XLSB::BeginPCDSCSet);
		XLS::BaseObjectPtr objectPtr(ptr);
		if(m_oSheet.IsInit())
		{
			ptr->fLoadSheet = true;
			ptr->irstSheet = m_oSheet.get();
		}
		else
			ptr->fLoadSheet = false;
		if(m_oRef.IsInit())
			ptr->rfx = m_oRef.get();
		if(m_oName.IsInit())
		{
			ptr->irstName = m_oName.get();
			ptr->fName = true;
		}
		else
			ptr->fName = false;
		if(m_oRid.IsInit())
		{
			ptr->fLoadRelId = true;
			ptr->irstRelId.value = m_oRid->GetValue();
		}
		else
			ptr->fLoadRelId = false;
		if(m_oI1.IsInit())
			ptr->rgiItem[0] = m_oI1->GetValue();
		if(m_oI2.IsInit())
			ptr->rgiItem[1] = m_oI2->GetValue();
		if(m_oI3.IsInit())
			ptr->rgiItem[2] = m_oI3->GetValue();
		if(m_oI4.IsInit())
			ptr->rgiItem[3] = m_oI4->GetValue();
		return objectPtr;
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
                m_oRef              = ptr->rfx.toString(true, true);

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
	XLS::BaseObjectPtr CConsolidationSource::toBin()
	{
		auto ptr(new XLSB::PCDSCONSOL);
		XLS::BaseObjectPtr objectPtr(ptr);
		auto ptr1(new XLSB::BeginPCDSConsol);
		ptr->m_BrtBeginPCDSConsol = XLS::BaseObjectPtr{ptr1};
		if(m_oAutoPage.IsInit())
			ptr1->fAutoPage = m_oAutoPage.get();
		if(m_oPages.IsInit())
			ptr->m_PCDSCPAGES = m_oPages->toBin();
		if(m_oRangeSets.IsInit())
			ptr->m_PCDSCSETS = m_oRangeSets->toBin();
		return objectPtr;
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
			WritingStringNullableAttrInt(L"par", m_oPar, m_oPar->GetValue());
            WritingStringNullableAttrInt(L"base", m_oBase, m_oBase->GetValue());
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
	XLS::BaseObjectPtr CFieldGroupProperties::toBin()
	{
		auto ptr(new XLSB::PCDFGROUP);
		XLS::BaseObjectPtr objectPtr(ptr);

		auto ptr1(new XLSB::BeginPCDFGroup);
		ptr->m_BrtBeginPCDFGroup = XLS::BaseObjectPtr{ptr1};
		if(m_oPar.IsInit())
			ptr1->ifdbParent = m_oPar->GetValue();
		else
            ptr1->ifdbParent = -1;
		if(m_oBase.IsInit())
			ptr1->ifdbBase = m_oBase->GetValue();
		else
			ptr1->ifdbBase = -1;

		if(m_oDiscretePr.IsInit())
			ptr->m_PCDFGDISCRETE = m_oDiscretePr->toBin();
		if(m_oRangePr.IsInit())
			ptr->m_PCDFGRANGE = m_oRangePr->toBin();
		if(m_oGroupItems.IsInit())
			ptr->m_PCDFGITEMS = m_oGroupItems->toBin();
		return objectPtr;
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
            BYTE* fileStream = 0;
            auto fileReader = xlsb->GetFileReader(oPath, fileStream);
            ///todo чтение записей из стрима
            m_oPivotCacheRecords.Init();
            m_oPivotCacheRecords->fromBin(fileReader);
            delete[] fileStream;
            /*XLSB::PivotCacheRecordsStreamPtr pivotCacheRecordsStream(new XLSB::PivotCacheRecordsStream);

            xlsb->ReadBin(oPath, pivotCacheRecordsStream.get());

            if (pivotCacheRecordsStream != nullptr)
            {
                if (pivotCacheRecordsStream->m_PIVOTCACHERECORDS != nullptr)
                    m_oPivotCacheRecords = pivotCacheRecordsStream->m_PIVOTCACHERECORDS;
            }*/

            //pivotCacheRecordsStream.reset();
        }
    }
	XLS::BaseObjectPtr CPivotCacheRecordsFile::WriteBin() const
	{
		auto pivotCacheRecordsStream(new XLSB::PivotCacheRecordsStream);
		if(m_oPivotCacheRecords.IsInit())
			pivotCacheRecordsStream->m_PIVOTCACHERECORDS = m_oPivotCacheRecords->toBin();
		else if(m_nDataLength > 0 && m_pData)
		{
            CPivotCacheRecords records;
            XmlUtils::CXmlLiteReader reader;
			auto wstringData = prepareData();
            reader.FromString(wstringData);
			reader.ReadNextNode();
            records.fromXML(reader);
            pivotCacheRecordsStream->m_PIVOTCACHERECORDS = records.toBin();
		}
		return XLS::BaseObjectPtr{pivotCacheRecordsStream};
	}
    void CPivotCacheRecordsFile::WriteBin(XLS::StreamCacheWriterPtr& writer) const
    {
        if(m_oPivotCacheRecords.IsInit())
            m_oPivotCacheRecords->toBin(writer);
        else
        {
            CPivotCacheRecords records;
            XmlUtils::CXmlLiteReader reader;
            auto wstringData = prepareData();
            reader.FromString(wstringData);
            reader.ReadNextNode();
            records.ReadAttributes(reader);
            {
                auto record = writer->getNextRecord(XLSB::rt_BeginPivotCacheRecords);
                _UINT32 size = 0;
                if(records.m_oCount.IsInit())
                    size = records.m_oCount->GetValue();
                *record << size;
                 writer->storeNextRecord(record);
            }
            if ( reader.IsEmptyNode() )
                return;
            int nCurDepth = reader.GetDepth();
            while( reader.ReadNextSiblingNode( nCurDepth ) )
            {
                std::wstring sName = XmlUtils::GetNameNoNS(reader.GetName());

                if (L"r" == sName)
                {
                    CPivotCacheRecord pPivotCacheRecord;
                    pPivotCacheRecord.fromXML(reader);
                    pPivotCacheRecord.toBin(writer);

                }
                else if (L"extLst" == sName)
                    records.m_oExtLst = reader;
            }
            {
                auto record = writer->getNextRecord(XLSB::rt_EndPivotCacheRecords);
                writer->storeNextRecord(record);
            }

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
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if ((xlsb) && (xlsb->m_bWriteToXlsb))
			{
                auto sreamWriter = xlsb->GetFileWriter(oPath);
                WriteBin(sreamWriter);
                xlsb->WriteSreamCache(sreamWriter);

			}
		else
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

			}
			else if(m_nDataLength > 0 && m_pData)
			{
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(oPath.GetPath());
				oFile.WriteFile(m_pData, m_nDataLength);
				oFile.CloseFile();

			}
		}
		oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		IFileContainer::Write( oPath, oDirectory, oContent );
	}
	const OOX::FileType CPivotCacheRecordsFile::type() const
	{
		CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
		if ((xlsb) && (xlsb->m_bWriteToXlsb))
		{
			return OOX::SpreadsheetBin::FileTypes::PivotCacheRecordsBin;
		}
		return OOX::Spreadsheet::FileTypes::PivotCacheRecords;
	}
	std::wstring CPivotCacheRecordsFile::prepareData() const
	{

	std::string stringData(reinterpret_cast<char*>(m_pData), m_nDataLength);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    return converter.from_bytes(stringData);
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
			{
				CPivotCacheRecord* pPivotCacheRecord = new CPivotCacheRecord();
				*pPivotCacheRecord = oReader;
				m_arrItems.push_back(pPivotCacheRecord);
			}
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
    void CPivotCacheRecords::fromBin(XLS::StreamCacheReaderPtr& reader)
    {
        auto type = reader->getNextRecordType();
        if(type == XLSB::rt_BeginPivotCacheRecords)
        {
            _UINT32 recordsCount;
            {
                auto BeginRecords = reader->getNextRecord(XLSB::rt_BeginPivotCacheRecords);
                *BeginRecords >> recordsCount;
                m_oCount = recordsCount;
            }

            NSStringUtils::CStringBuilder writer;
            writer.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
            writer.WriteString(L"<pivotCacheRecords \
xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
mc:Ignorable=\"xr16\" \
xmlns:xr16=\"http://schemas.microsoft.com/office/spreadsheetml/2017/revision16\"");
            WritingStringAttrInt(L"count", recordsCount);
            writer.WriteString(L">");

            while(recordsCount > 0)
            {
                type = reader->getNextRecordType();
                if(type!= XLSB::rt_PCRRecord && type != XLSB::rt_PCRRecordDt)
                    break;
                CPivotCacheRecord xmlItem;
                xmlItem.fromBin(reader);
                xmlItem.toXML(writer);
                recordsCount--;
            }
        }
    }
	XLS::BaseObjectPtr CPivotCacheRecords::toBin()
	{
		auto ptr(new XLSB::PIVOTCACHERECORDS);
		XLS::BaseObjectPtr objectPtr(ptr);
		auto ptr1(new XLSB::BeginPivotCacheRecords);
		ptr1->crecords = m_arrItems.size();
		ptr->m_BrtBeginPivotCacheRecords = XLS::BaseObjectPtr{ptr1};
		for(auto i:m_arrItems)
			ptr->m_arPIVOTCACHERECORD.push_back(i->toBin());

		return objectPtr;
	}
    void CPivotCacheRecords::toBin(XLS::StreamCacheWriterPtr& writer)
    {

        {
            auto record = writer->getNextRecord(XLSB::rt_BeginPivotCacheRecords);
            _UINT32 size = m_arrItems.size();
            *record << size;
             writer->storeNextRecord(record);
        }
        for(auto i:m_arrItems)
        {
            i->toBin(writer);
        }
        {
            auto record = writer->getNextRecord(XLSB::rt_EndPivotCacheRecords);
            writer->storeNextRecord(record);
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
			{
				CPivotBooleanValue* pPivotBooleanValue = new CPivotBooleanValue();
				*pPivotBooleanValue = oReader;
				m_arrItems.push_back(pPivotBooleanValue);
			}
			else if ( L"d" == sName )
			{
				CPivotDateTimeValue* pPivotDateTimeValue = new CPivotDateTimeValue();
				*pPivotDateTimeValue = oReader;
				m_arrItems.push_back(pPivotDateTimeValue);
			}
			else if ( L"e" == sName )
			{
				CPivotErrorValue* pPivotErrorValue = new CPivotErrorValue();
				*pPivotErrorValue = oReader;
				m_arrItems.push_back(pPivotErrorValue);
			}
			else if ( L"m" == sName )
			{
				CPivotNoValue *pPivotNoValue = new CPivotNoValue();
				*pPivotNoValue = oReader;
				m_arrItems.push_back(pPivotNoValue);
			}
			else if ( L"n" == sName )
			{
				CPivotNumericValue *pPivotNumericValue = new CPivotNumericValue();
				*pPivotNumericValue = oReader;
				m_arrItems.push_back(pPivotNumericValue);
			}
			else if ( L"s" == sName )
			{
				CPivotCharacterValue *pPivotCharacterValue = new CPivotCharacterValue();
				*pPivotCharacterValue = oReader;
				m_arrItems.push_back(pPivotCharacterValue);
			}
			else if ( L"x" == sName )
				m_arrItems.push_back(PPTX::CreatePtrXmlContent<CSharedItemsIndex>(oReader));
		}
	}
	XLS::BaseObjectPtr CPivotCacheRecord::toBin()
	{
		auto ptr(new XLSB::PIVOTCACHERECORD);
		auto ptr1(new XLSB::PIVOTCACHERECORDDT);
		XLS::BaseObjectPtr objectPtr(ptr);
		ptr->m_source = XLS::BaseObjectPtr{ptr1};
		for(auto i:m_arrItems)
		{
			auto ptrPCDIDT(new XLSB::PCDIDT);


			if(i->getType() == et_x_PivotBooleanValue)
			{
                auto boolValue = static_cast<CPivotBooleanValue*>(i);
				ptrPCDIDT->m_source =  boolValue->toBin();
				ptr1->m_arPCDIDT.push_back(XLS::BaseObjectPtr{ptrPCDIDT});
				continue;
			}
			
			else if(i->getType() == et_x_PivotDateTimeValue)
			{
				auto dataValue = static_cast<CPivotDateTimeValue*>(i);
				ptrPCDIDT->m_source =  dataValue->toBin();
				ptr1->m_arPCDIDT.push_back(XLS::BaseObjectPtr{ptrPCDIDT});
				continue;
			}
			
			else if(i->getType() == et_x_PivotErrorValue)
			{
				auto errorValue = static_cast<CPivotErrorValue*>(i);
				ptrPCDIDT->m_source =  errorValue->toBin();
				ptr1->m_arPCDIDT.push_back(XLS::BaseObjectPtr{ptrPCDIDT});
				continue;
			}
			
			else if(i->getType() == et_x_PivotNoValue)
			{
				auto noValue = static_cast<CPivotNoValue*>(i);
				ptrPCDIDT->m_source =  noValue->toBin();
				ptr1->m_arPCDIDT.push_back(XLS::BaseObjectPtr{ptrPCDIDT});
				continue;
			}
			
			else if(i->getType() == et_x_PivotNumericValue)
			{
				auto numValue = static_cast<CPivotNumericValue*>(i);
				ptrPCDIDT->m_source =  numValue->toBin();
				ptr1->m_arPCDIDT.push_back(XLS::BaseObjectPtr{ptrPCDIDT});
				continue;
			}
			
			else if(i->getType() == et_x_PivotCharacterValue)
			{
				auto charValue = static_cast<CPivotCharacterValue*>(i);
				ptrPCDIDT->m_source =  charValue->toBin();
				ptr1->m_arPCDIDT.push_back(XLS::BaseObjectPtr{ptrPCDIDT});
				continue;
			}
			
			else if(i->getType() == et_x_SharedItemsIndex)
			{
				auto itemIndex = static_cast<CSharedItemsIndex*>(i);
				ptrPCDIDT->m_source =  itemIndex->toBinItemIndex();
				ptr1->m_arPCDIDT.push_back(XLS::BaseObjectPtr{ptrPCDIDT});
				continue;
			}
		}
		return objectPtr;
	}
    void CPivotCacheRecord::toBin(XLS::StreamCacheWriterPtr& writer)
    {
        {
            auto record = writer->getNextRecord(XLSB::rt_PCRRecordDt);
            writer->storeNextRecord(record);
        }
        for(auto i:m_arrItems)
        {
            auto elemType = i->getType();
            switch(elemType)
            {
                case et_x_PivotBooleanValue:
                {
                    auto boolValue = static_cast<CPivotBooleanValue*>(i);
                    auto record = writer->getNextRecord(XLSB::rt_PCDIBoolean);
                    BYTE recordVal = 0;
                    if(boolValue->m_oValue.IsInit())
                        recordVal = boolValue->m_oValue.get();
                    *record << recordVal;
                    writer->storeNextRecord(record);
                    continue;
                }
                case et_x_PivotDateTimeValue:
                {
                    auto dataValue = static_cast<CPivotDateTimeValue*>(i);
                    auto record = writer->getNextRecord(XLSB::rt_PCDIDatetime);
                    XLSB::PCDIDateTime recordVal;
                    if(dataValue->m_oValue.IsInit())
                        recordVal.fromString(dataValue->m_oValue->GetValue());
                    *record << recordVal;
                    writer->storeNextRecord(record);
                    continue;
                }
                case et_x_PivotErrorValue:
                {
                    auto errorValue = static_cast<CPivotErrorValue*>(i);
                    errorValue->toBin(writer);
                    continue;
                }
                case et_x_PivotNoValue:
                {
                    auto record = writer->getNextRecord(XLSB::rt_PCDIMissing);
                    writer->storeNextRecord(record);
                    continue;
                }
                case et_x_PivotNumericValue:
                {
                    auto numValue = static_cast<CPivotNumericValue*>(i);
                    auto record = writer->getNextRecord(XLSB::rt_PCDINumber);
                    XLS::Xnum recordVal;
                    if(numValue->m_oValue.IsInit())
                        recordVal.data.value = numValue->m_oValue.get();
                    *record << recordVal;
                    writer->storeNextRecord(record);
                    continue;
                }
                case et_x_PivotCharacterValue:
                {
                    auto charValue = static_cast<CPivotCharacterValue*>(i);
                    auto record = writer->getNextRecord(XLSB::rt_PCDIString);
                    XLSB::XLWideString recordVal;
                    if(charValue->m_oValue.IsInit())
                        recordVal = charValue->m_oValue.get();
                    *record << recordVal;
                    writer->storeNextRecord(record);
                    continue;
                }
                case et_x_SharedItemsIndex:
                {
                    auto itemIndex = static_cast<CSharedItemsIndex*>(i);
                    auto record = writer->getNextRecord(XLSB::rt_PCDIIndex);
                    _UINT32 recordVal = 0;
                    if(itemIndex->m_oV.IsInit())
                        recordVal = itemIndex->m_oV->GetValue();
                    *record << recordVal;
                    writer->storeNextRecord(record);
                    continue;
                }
                default:
                    continue;
            }
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
    void CPivotCacheRecord::fromBin(XLS::StreamCacheReaderPtr& reader)
    {
        auto recordType = reader->getNextRecordType();
        if(recordType == XLSB::rt_PCRRecordDt)
        {
            reader->SkipRecord(false);
            recordType = reader->getNextRecordType();
            while(1)
            {
                switch(recordType)
                {
                    case XLSB::rt_PCDIMissing:
                    {
                         reader->SkipRecord(false);
                         recordType = reader->getNextRecordType();
                         m_arrItems.push_back(new CPivotNoValue);
                         break;
                    }
                    case XLSB::rt_PCDINumber:
                    {
                         auto record = reader->getNextRecord(XLSB::rt_PCDINumber);
                         XLS::Xnum number;
                         *record >>number;
                         auto numValue = new CPivotNumericValue;
                         numValue->m_oValue = number.data.value;
                         m_arrItems.push_back(numValue);
                         break;
                    }
                    case XLSB::rt_PCDIBoolean:
                    {
                         auto record = reader->getNextRecord(XLSB::rt_PCDIBoolean);
                         BYTE boolval;
                         *record >>boolval;
                         auto BoleanValue = new CPivotBooleanValue;
                         BoleanValue->m_oValue = boolval;
                         m_arrItems.push_back(BoleanValue);
                         break;
                    }
                    case XLSB::rt_PCDIString:
                    {
                         auto record = reader->getNextRecord(XLSB::rt_PCDIString);
                         XLSB::XLWideString StringVal;
                         *record >>StringVal;
                         auto CharacterValue = new CPivotCharacterValue;
                         CharacterValue->m_oValue = StringVal;
                         m_arrItems.push_back(CharacterValue);
                         break;
                    }
                    case XLSB::rt_PCDIDatetime:
                    {
                         auto record = reader->getNextRecord(XLSB::rt_PCDIDatetime);
                         XLSB::PCDIDateTime DateVal;
                         *record >>DateVal;
                         auto DatetimeValue = new CPivotDateTimeValue;
                         DatetimeValue->m_oValue = DateVal.value();
                         m_arrItems.push_back(DatetimeValue);
                         break;
                    }
                    case XLSB::rt_PCDIIndex:
                    {
                         auto record = reader->getNextRecord(XLSB::rt_PCDIIndex);
                         _UINT32 iitem;
                         *record >>iitem;
                         auto IndexValue = new CSharedItemsIndex;
                         IndexValue->m_oV = iitem;
                         m_arrItems.push_back(IndexValue);
                         break;
                    }
                    case XLSB::rt_PCDIError:
                    {
                         auto record = reader->getNextRecord(XLSB::rt_PCDIError);
                         BYTE erorIndex;
                         *record >>erorIndex;
                         auto ErrValue = new CPivotErrorValue;
                         switch(erorIndex)
                         {
                             case 0x00: ErrValue->m_oValue = L"#NULL!"; break;
                             case 0x07: ErrValue->m_oValue = L"#DIV/0!"; break;
                             case 0x0F: ErrValue->m_oValue = L"#VALUE!"; break;
                             case 0x17: ErrValue->m_oValue = L"#REF!"; break;
                             case 0x1D: ErrValue->m_oValue = L"#NAME?"; break;
                             case 0x24: ErrValue->m_oValue = L"#NUM!"; break;
                             case 0x2A: ErrValue->m_oValue = L"#N/A"; break;
                             case 0x2B: ErrValue->m_oValue = L"#GETTING_DATA"; break;
                         }
                         m_arrItems.push_back(ErrValue);
                         break;
                    }
                    default:
                        return;
                }
                recordType = reader->getNextRecordType();
            }
        }
        else if(recordType == XLSB::rt_PCRRecord)
        {
            auto record = reader->getNextRecord(XLSB::rt_PCRRecord);
            if (record->checkFitReadSafe(1))
            {
                auto arrPivotCacheRecordType = record->getGlobalWorkbookInfo()->pivotCacheRecordType.find(record->getGlobalWorkbookInfo()->currentPivotCacheRecord - 1);
                if (arrPivotCacheRecordType != record->getGlobalWorkbookInfo()->pivotCacheRecordType.end())
                {
                    for(const auto& item : arrPivotCacheRecordType->second)
                    switch (item)
                    {
                        case XLS::typePCDIIndex:
                        {
                            _UINT32 iitem;
                            *record >>iitem;
                            auto IndexValue = new CSharedItemsIndex;
                            IndexValue->m_oV = iitem;
                            m_arrItems.push_back(IndexValue);
                            break;
                        }
                        case XLS::typePCDINumber:
                        {
                            XLS::Xnum number;
                            *record >>number;
                            auto numValue = new CPivotNumericValue;
                            numValue->m_oValue = number.data.value;
                            m_arrItems.push_back(numValue);
                            break;
                        }
                        case XLS::typePCDIDatetime:
                        {
                            XLSB::PCDIDateTime DateVal;
                            *record >>DateVal;
                            auto DatetimeValue = new CPivotDateTimeValue;
                            DatetimeValue->m_oValue = DateVal.value();
                            m_arrItems.push_back(DatetimeValue);
                            break;
                        }
                        case XLS::typePCDIString:
                        {
                             XLSB::XLWideString StringVal;
                             *record >>StringVal;
                             auto CharacterValue = new CPivotCharacterValue;
                             CharacterValue->m_oValue = StringVal;
                             m_arrItems.push_back(CharacterValue);
                             break;
                        }
                        default:
                            break;
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

	XLS::BaseObjectPtr CSharedItemsIndex::toBinItemIndex()
	{
		auto ptr(new XLSB::PCDIIndex);
		XLS::BaseObjectPtr objectPtr(ptr);
		if(m_oV.IsInit())
		ptr->iitem = m_oV->GetValue();
		return objectPtr;
	}

	XLS::BaseObjectPtr CSharedItemsIndex::toBinPrfItem()
	{
		auto ptr(new XLSB::BeginPRFItem);
		XLS::BaseObjectPtr objectPtr(ptr);
		if(m_oV.IsInit())
		ptr->iitem = m_oV->GetValue();
		return objectPtr;
	}
} //Spreadsheet
} // namespace OOX
