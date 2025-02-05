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
#include "../DocxFlat.h"
#include "../Document.h"

#include "DrawingExt.h"
#include "Drawing.h"
#include "../../XlsxFormat/Worksheets/Sparkline.h"
#include "../../XlsxFormat/Table/Table.h"
#include "../../XlsxFormat/Table/Connections.h"
#include "../Diagram/DiagramData.h"
#include "../../XlsxFormat/Worksheets/ConditionalFormatting.h"
#include "../../XlsxFormat/Worksheets/DataValidation.h"
#include "../../XlsxFormat/Slicer/SlicerCache.h"
#include "../../XlsxFormat/Slicer/SlicerCacheExt.h"
#include "../../XlsxFormat/Pivot/PivotCacheDefinitionExt.h"
#include "../../XlsxFormat/Styles/dxf.h"
#include "../../XlsxFormat/Chart/ChartSerialize.h"
#include "../../XlsxFormat/Worksheets/WorksheetChildOther.h"
#include "../../XlsxFormat/Timelines/Timeline.h"
#include "../../XlsxFormat/Workbook/Metadata.h"
#include "../../XlsxFormat/Workbook/Workbook.h"

#include "../Comments.h"

#include "../../XlsbFormat/Biff12_unions/FRTWORKSHEET.h"
#include "../../XlsbFormat/Biff12_unions/CONDITIONALFORMATTINGS.h"
#include "../../XlsbFormat/Biff12_unions/CONDITIONALFORMATTING14.h"
#include "../../XlsbFormat/Biff12_unions/FRTSTYLESHEET.h"
#include "../../XlsbFormat/Biff12_unions/STYLESHEET14.h"
#include "../../XlsbFormat/Biff12_unions/DXF14S.h"
#include "../../XlsbFormat/Biff12_unions/FRTTABLE.h"
#include "../../XlsbFormat/Biff12_unions/FRTQSI.h"
#include "../../XlsbFormat/Biff12_unions/FRTEXTCONNECTIONS.h"
#include "../../XlsbFormat/Biff12_unions/FRTSLICERCACHE.h"
#include "../../XlsbFormat/Biff12_unions/SLICERCACHEBOOKPIVOTTABLES.h"
#include "../../XlsbFormat/Biff12_unions/TABLESLICERCACHE.h"
#include "../../XlsbFormat/Biff12_unions/SLICERCACHECROSSFILTEREXT.h"
#include "../../XlsbFormat/Biff12_records/SlicerCacheBookPivotTables.h"
#include "../../XlsbFormat/Biff12_unions/SLICERSEX.h"
#include "../../XlsbFormat/Biff12_unions/TABLESLICERSEX.h"
#include "../../XlsbFormat/Biff12_unions/FRTWORKBOOK.h"
#include "../../XlsbFormat/Biff12_unions/FRTPIVOTCACHEDEF.h"
#include "../../XlsbFormat/Biff12_unions/FMD.h"
#include "../../XlsbFormat/Biff12_unions/DYNAMICARRAYMETADATA.h"
#include "../../XlsbFormat/Biff12_unions/RICHDATAMETADATA.h"
#include "../../XlsbFormat/Biff12_records/FRTBegin.h"

namespace OOX
{
	namespace Drawing
	{
		CCompatExt::CCompatExt()
		{
		}
		CCompatExt::~CCompatExt()
		{
		}
		void CCompatExt::fromXML(XmlUtils::CXmlNode& oNode)
		{
			//todo
		}
		void CCompatExt::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CCompatExt::toXML() const
		{
			return L"";
		}
		EElementType CCompatExt::getType() const
		{
			return OOX::et_a_compatExt;
		}
		void CCompatExt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, L"spid", m_sSpId )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}

		CDataModelExt::CDataModelExt()
		{
		}
		CDataModelExt::~CDataModelExt()
		{
		}
		void CDataModelExt::fromXML(XmlUtils::CXmlNode& oNode)
		{
			//todo
		}
		void CDataModelExt::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		std::wstring CDataModelExt::toXML() const
		{
			std::wstring sResult = L"<dsp:dataModelExt xmlns:dsp=\"http://schemas.microsoft.com/office/drawing/2008/diagram\"";
			if (m_oRelId.IsInit())
			{
				sResult += L" relId=\"" + m_oRelId->ToString() + L"\"";
			}
			sResult += L" minVer=\"http://schemas.openxmlformats.org/drawingml/2006/diagram\"/>";

			return sResult;
		}
		EElementType CDataModelExt::getType() const
		{
			return OOX::et_a_compatExt;
		}
		void CDataModelExt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, L"relId", m_oRelId )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}

		COfficeArtExtension::COfficeArtExtension()
		{
		}
		COfficeArtExtension::~COfficeArtExtension()
		{
			m_oSparklineGroups.reset();
			m_oAltTextTable.reset();
			m_oDataModelExt.reset();
			m_oCompatExt.reset();
			m_oDataValidations.reset();
			m_oConnection.reset();
			m_oDxfs.reset();
			m_oTableSlicerCache.reset();
			m_oSlicerList.reset();
			m_oSlicerListExt.reset();
			m_oSlicerCaches.reset();
			m_oSlicerCachesExt.reset();
			m_oSlicerStyles.reset();
			m_oTableSlicerCache.reset();
			m_oSlicerCacheHideItemsWithNoData.reset();
            m_oPivotCacheDefinitionExt.reset();
			m_oUserProtectedRanges.reset();
			m_oChartDataLabel.reset();
			m_oChartFiltering.reset();
			m_oTimelineRefs.reset();
			m_oTimelineCacheRefs.reset();

			for (size_t nIndex = 0; nIndex < m_arrConditionalFormatting.size(); ++nIndex)
			{
				delete m_arrConditionalFormatting[nIndex];
			}
			m_arrConditionalFormatting.clear();

			for (size_t nIndex = 0; nIndex < m_oSlicerCachePivotTables.size(); ++nIndex)
			{
				delete m_oSlicerCachePivotTables[nIndex];
			}
			m_oSlicerCachePivotTables.clear();
		}
		void COfficeArtExtension::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ((m_sUri.IsInit()) && *m_sUri == L"{CE6537A1-D6FC-4f65-9D91-7224C49458BB}")
			{// http://schemas.microsoft.com/office/drawing/2012/chart)
				m_oChartDataLabel.Init();
				m_oChartDataLabel->fromXML(oReader);
				return;
			}
			if ((m_sUri.IsInit()) && *m_sUri == L"{02D57815-91ED-43cb-92C2-25804820EDAC}")
			{// http://schemas.microsoft.com/office/drawing/2012/chart)
				m_oChartFiltering.Init();
				m_oChartFiltering->fromXML(oReader);
				return;
			}
            if ((m_sUri.IsInit()) && (	*m_sUri == L"{C3750BE0-5CA9-4D1C-82C7-79D762991C26}" ||
										*m_sUri == L"{63B3BB69-23CF-44E3-9099-C40C66FF867C}" ||
										*m_sUri == L"{05C60535-1F16-4fd2-B633-F4F36F0B64E0}" ||
										*m_sUri == L"{504A1905-F514-4f6f-8877-14C23A59335A}" ||
										*m_sUri == L"{78C0D931-6437-407d-A8EE-F0AAD7539E65}" ||
										*m_sUri == L"{B025F937-C7B1-47D3-B67F-A62EFF666E3E}" ||
										*m_sUri == L"{CCE6A557-97BC-4b89-ADB6-D9C93CAAB3DF}" ||
										*m_sUri == L"{A8765BA9-456A-4dab-B4F3-ACF838C121DE}" ||
										*m_sUri == L"{3A4CF648-6AED-40f4-86FF-DC5316D8AED3}" ||
										*m_sUri == L"{BBE1A952-AA13-448e-AADC-164F8A28A991}" ||
										*m_sUri == L"{46BE6895-7355-4a93-B00E-2C351335B9C9}" ||
										*m_sUri == L"{EB79DEF2-80B8-43e5-95BD-54CBDDF9020C}" ||
										*m_sUri == L"{03082B11-2C62-411c-B77F-237D8FCFBE4C}" ||
										*m_sUri == L"{2F2917AC-EB37-4324-AD4E-5DD8C200BD13}" ||
										*m_sUri == L"{470722E0-AACD-4C17-9CDC-17EF765DBC7E}" ||
										*m_sUri == L"{46F421CA-312F-682f-3DD2-61675219B42D}" ||
										*m_sUri == L"{DE250136-89BD-433C-8126-D09CA5730AF9}" ||
										*m_sUri == L"{19B8F6BF-5375-455C-9EA6-DF929625EA0E}" ||
										*m_sUri == L"{725AE2AE-9491-48be-B2B4-4EB974FC3084}" ||
										*m_sUri == L"{231B7EB2-2AFC-4442-B178-5FFDF5851E7C}" ||
										*m_sUri == L"{FCE6A71B-6B00-49CD-AB44-F6B1AE7CDE65}" ||
										*m_sUri == L"{56B9EC1D-385E-4148-901F-78D8002777C0}" ||
										*m_sUri == L"{7E03D99C-DC04-49d9-9315-930204A7B6E9}" ||
										*m_sUri == L"{D0CA8CA8-9F24-4464-BF8E-62219DCF47F9}" ||
										*m_sUri == L"{9260A510-F301-46a8-8635-F512D64BE5F5}" ||
										*m_sUri == L"{3e2802c4-a4d2-4d8b-9148-e3be6c30e623}" ||			
										*m_sUri == L"{bdbb8cdc-fa1e-496e-a857-3c3f30c029c3}" ||
                                        *m_sUri == L"{876F7934-8845-4945-9796-88D515C7AA90}" ||
										*m_sUri == L"http://schemas.microsoft.com/office/drawing/2008/diagram"))
			{
				int nCurDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nCurDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (sName == L"compatExt")//2.3.1.2 compatExt
					{	//attributes spid -https://msdn.microsoft.com/en-us/library/hh657207(v=office.12).aspx
						m_oCompatExt = oReader;
					}
					else if (sName == L"sparklineGroups")
					{
						m_oSparklineGroups = oReader;
					}
					else if (sName == L"dataModelExt")
					{
						m_oDataModelExt = oReader;
					}
					else if (sName == L"table")
					{
						m_oAltTextTable = oReader;
					}
					else if ((sName == L"conditionalFormattings") && (false == oReader.IsEmptyNode()))
					{
						int nCurDepth1 = oReader.GetDepth();
						while (oReader.ReadNextSiblingNode(nCurDepth1))
						{
							OOX::Spreadsheet::CConditionalFormatting* pConditionalFormatting = new OOX::Spreadsheet::CConditionalFormatting();
							*pConditionalFormatting = oReader;
							m_arrConditionalFormatting.push_back(pConditionalFormatting);
						}
					}
					else if (sName == L"dataValidations")
					{
						m_oDataValidations = oReader;
					}
					else if (sName == L"connection")
					{
						m_oConnection = oReader;
					}
					else if (sName == L"timelineRefs")
					{
						m_oTimelineRefs = oReader;
					}
					else if (sName == L"timelineCacheRefs")
					{
						m_oTimelineCacheRefs = oReader;
					}
					else if (sName == L"timelineStyles")
					{
						m_oTimelineStyles = oReader;
					}
					else if (sName == L"slicerList")
					{
						if (L"{A8765BA9-456A-4dab-B4F3-ACF838C121DE}" == *m_sUri)
						{
							m_oSlicerList = oReader;
						}
						else
						{
							m_oSlicerListExt = oReader;
						}
					}
					else if (sName == L"slicerCaches")
					{
						if (L"{BBE1A952-AA13-448e-AADC-164F8A28A991}" == *m_sUri)
						{
							m_oSlicerCaches = oReader;
						}
						else
						{
							m_oSlicerCachesExt = oReader;
						}
					}
					else if (sName == L"dxfs")
					{
						m_oDxfs = oReader;
					}
					else if (sName == L"slicerStyles")
					{
						m_oSlicerStyles = oReader;
					}
					else if ((sName == L"slicerCachePivotTables") && false == oReader.IsEmptyNode())
					{
						int nCurDepth1 = oReader.GetDepth();
						while (oReader.ReadNextSiblingNode(nCurDepth1))
						{
							OOX::Spreadsheet::CSlicerCachePivotTable* pSlicerCachePivotTable = new OOX::Spreadsheet::CSlicerCachePivotTable();
							*pSlicerCachePivotTable = oReader;
							m_oSlicerCachePivotTables.push_back(pSlicerCachePivotTable);
						}
					}
					else if (sName == L"tableSlicerCache")
					{
						m_oTableSlicerCache = oReader;
					}
					else if (sName == L"slicerCacheHideItemsWithNoData")
					{
						m_oSlicerCacheHideItemsWithNoData = oReader;
					}
					else if (sName == L"id")
					{
						m_oId = oReader.GetText2();
					}
					else if (sName == L"presenceInfo")
					{
						m_oPresenceInfo = oReader;
					}
					else if (sName == L"pivotCacheDefinition")
					{
						m_oPivotCacheDefinitionExt = oReader;
					}
					else if (sName == L"externalReference")
					{
						WritingElement_ReadAttributes_Start_No_NS(oReader)
							WritingElement_ReadAttributes_Read_if (oReader, L"fileKey", m_oFileKey)
							WritingElement_ReadAttributes_Read_else_if (oReader, L"instanceId", m_oInstanceId)
						WritingElement_ReadAttributes_End_No_NS(oReader)
					}
					else if (sName == L"userProtectedRanges")
					{
						m_oUserProtectedRanges = oReader;
					}
					else if (sName == L"externalLinksPr")
					{
						WritingElement_ReadAttributes_Start_No_NS(oReader)
							WritingElement_ReadAttributes_ReadSingle(oReader, L"autoRefresh", m_oExternalLinksAutoRefresh)
						WritingElement_ReadAttributes_End_No_NS(oReader)
					}
					else if ((sName == L"dataDisplayOptions16") && (false == oReader.IsEmptyNode()))
					{
						m_sAdditionalNamespace = L"xmlns:c16r3=\"http://schemas.microsoft.com/office/drawing/2017/03/chart\"";
						int nCurDepth1 = oReader.GetDepth();
						while (oReader.ReadNextSiblingNode(nCurDepth1))
						{
							std::wstring sName1 = XmlUtils::GetNameNoNS(oReader.GetName());
							if (sName1 == L"dispNaAsBlank")
							{
								WritingElement_ReadAttributes_Start_No_NS(oReader)
									WritingElement_ReadAttributes_ReadSingle(oReader, L"val", m_oDataDisplayNaAsBlank)
								WritingElement_ReadAttributes_End_No_NS(oReader)
							}
						}
					}
					else if (sName == L"dynamicArrayProperties")
					{
						m_oDynamicArrayProperties = oReader;
					}
					else if (sName == L"rvb")
					{
						m_oRichValueBlock = oReader;
					}
                    else if (sName == L"pivotCaches")
                    {
                        m_oWorkbookPivotCaches = oReader;
                        m_oWorkbookPivotCaches->pivotCaches14 = true;
                    }
				}
			}
			else
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
		}
		void COfficeArtExtension::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"uri", m_sUri );
		}
		void COfficeArtExtension::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring ns = L"a:";

			pWriter->WriteString(toXMLWithNS(ns));
		}
		EElementType COfficeArtExtension::getType() const
		{
			return OOX::et_a_ext;
		}
		std::wstring COfficeArtExtension::toXML() const
        {
            return toXMLWithNS(L"a:");
        }
        std::wstring COfficeArtExtension::toXMLWithNS(const std::wstring& sNamespace) const
		{
            std::wstring sResult = L"<" + sNamespace + L"ext";

            if ( m_sUri.IsInit() )
			{
                sResult += L" uri=\"" + *m_sUri + L"\"";
			}

			if (!m_sAdditionalNamespace.empty())
			{
				sResult += L" " + m_sAdditionalNamespace;
			}

			sResult += L">";

			if (m_oCompatExt.IsInit())
			{
				sResult += m_oCompatExt->toXML();
			}
			if (m_oUserProtectedRanges.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oUserProtectedRanges->toXML(writer);

				sResult += writer.GetData().c_str();
			}
			if (m_oSparklineGroups.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSparklineGroups->toXML(writer);
				sResult += writer.GetData().c_str();
			}
			if (m_oTimelineRefs.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oTimelineRefs->toXML(writer);
                sResult += writer.GetData().c_str();
			}
			if (m_oTimelineCacheRefs.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oTimelineCacheRefs->toXML(writer);
				sResult += writer.GetData().c_str();
			}
			if (m_oTimelineStyles.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oTimelineStyles->toXML(writer);
				sResult += writer.GetData().c_str();
			}
			if (m_oAltTextTable.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oAltTextTable->toXML(writer);
				sResult += writer.GetData().c_str();
			}
			if (m_oDataModelExt.IsInit())
			{
				sResult += m_oDataModelExt->toXML();
			}
			if (false == m_arrConditionalFormatting.empty())
			{
				sResult += L"<x14:conditionalFormattings>";
				NSStringUtils::CStringBuilder writer;
				for (size_t i = 0; i < m_arrConditionalFormatting.size(); i++)
				{
					m_arrConditionalFormatting[i]->toXML2(writer, true);
				}
				sResult += writer.GetData().c_str();
				sResult += L"</x14:conditionalFormattings>";
			}
			if (m_oDataValidations.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oDataValidations->toXML2(writer, true);
				sResult += writer.GetData().c_str();
			}
			if (m_oSlicerList.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerList->toXML(writer, L"x14:slicerList");
				sResult += writer.GetData().c_str();
			}
			if (m_oSlicerListExt.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerListExt->toXML(writer, L"x14:slicerList");
				sResult += writer.GetData().c_str();
			}
			if (m_oSlicerCaches.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerCaches->toXML(writer, L"slicerCaches", L"x14:");
				sResult += writer.GetData().c_str();
			}
			if (m_oSlicerCachesExt.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerCachesExt->toXML(writer, L"slicerCaches", L"x15:");
				sResult += writer.GetData().c_str();
			}
			if (m_oDxfs.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oDxfs->toXML2(writer, L"x14:dxfs");
				sResult += writer.GetData().c_str();
			}
			if (m_oSlicerStyles.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerStyles->toXML(writer, L"x14:slicerStyles");
				sResult += writer.GetData().c_str();
			}
			if (m_oSlicerCachePivotTables.size() > 0)
			{
				NSStringUtils::CStringBuilder writer;
				writer.StartNode(L"x15:slicerCachePivotTables");
				writer.StartAttributes();
				writer.EndAttributes();
				for(size_t i = 0; i < m_oSlicerCachePivotTables.size(); ++i)
				{
					m_oSlicerCachePivotTables[i]->toXML(writer, L"pivotTable");
				}
				writer.EndNode(L"x15:slicerCachePivotTables");
				sResult += writer.GetData().c_str();
			}
			if (m_oTableSlicerCache.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oTableSlicerCache->toXML(writer, L"x15:tableSlicerCache");
				sResult += writer.GetData().c_str();
			}
			if (m_oSlicerCacheHideItemsWithNoData.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerCacheHideItemsWithNoData->toXML(writer, L"x15:slicerCacheHideItemsWithNoData");
				sResult += writer.GetData().c_str();
			}
            if (m_oPivotCacheDefinitionExt.IsInit())
            {
                NSStringUtils::CStringBuilder writer;
                m_oPivotCacheDefinitionExt->toXML(writer, L"x14:pivotCacheDefinition");
                sResult += writer.GetData().c_str();
            }
			if (m_oFileKey.IsInit() || m_oInstanceId.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				writer.StartNode(L"externalReference");
				writer.StartAttributes();

				if (m_oFileKey.IsInit()) writer.WriteAttribute(L"fileKey", *m_oFileKey);
				if (m_oInstanceId.IsInit()) writer.WriteAttribute(L"instanceId", *m_oInstanceId);

				writer.EndAttributes();
				writer.EndNode(L"externalReference");
				sResult += writer.GetData().c_str();
			}
			if (m_oId.IsInit())
			{
				sResult += L"<" + sNamespace + L"id>" + *m_oId + L"</" + sNamespace + L"id>";
			}
			if (m_oChartDataLabel.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oChartDataLabel->toXMLEntry(L"c15", writer);
				sResult += writer.GetData().c_str();
			}
			if (m_oChartFiltering.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oChartFiltering->toXML(writer);
				sResult += writer.GetData().c_str();
			}
			if (m_oExternalLinksAutoRefresh.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				writer.StartNode(L"xxlnp:externalLinksPr");
				writer.StartAttributes();
					writer.WriteAttribute(L"autoRefresh", *m_oExternalLinksAutoRefresh);
				writer.EndAttributes();
				writer.EndNode(L"xxlnp:externalLinksPr");
				sResult += writer.GetData().c_str();
			}
			if (m_oDataDisplayNaAsBlank.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				writer.StartNode(L"c16r3:dataDisplayOptions16");
				writer.EndAttributes();
					writer.StartNode(L"c16r3:dispNaAsBlank");
						writer.StartAttributes();
							writer.WriteAttribute(L"val", *m_oDataDisplayNaAsBlank);
						writer.EndAttributesAndNode();
				writer.EndNode(L"c16r3:dataDisplayOptions16");
				sResult += writer.GetData().c_str();
			}
			if (m_oDynamicArrayProperties.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oDynamicArrayProperties->toXML(writer);
				sResult += writer.GetData().c_str();
			}
			if (m_oRichValueBlock.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oRichValueBlock->toXML(writer);
				sResult += writer.GetData().c_str();
			}
            if(m_oWorkbookPivotCaches.IsInit())
            {
                NSStringUtils::CStringBuilder writer;
                m_oWorkbookPivotCaches->toXML(writer);
                sResult += writer.GetData().c_str();
            }
			sResult += L"</" + sNamespace + L"ext>";

			return sResult;
		}
		void COfficeArtExtension::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if ( oReader, L"uri", m_sUri )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}

		COfficeArtExtensionList::COfficeArtExtensionList()
		{
		}
		COfficeArtExtensionList::~COfficeArtExtensionList()
		{
			for ( size_t nIndex = 0; nIndex < m_arrExt.size(); nIndex++ )
			{
				if ( m_arrExt[nIndex] ) delete m_arrExt[nIndex];
				m_arrExt[nIndex] = NULL;
			}
			m_arrExt.clear();
		}
		void COfficeArtExtensionList::fromXML(XmlUtils::CXmlNode& oNode)
		{
			// TO DO: Реализовать
		}
		void COfficeArtExtensionList::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( L"ext" == sName )
				{
					OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
					*oExt = oReader;

					if (oExt)
						m_arrExt.push_back( oExt );
				}
			}
		}
		void COfficeArtExtensionList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (m_arrExt.empty()) return;

			std::wstring ns = L"a:";

			if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX ||
				pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DOCX_GLOSSARY)		ns = L"wps:";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_XLSX)			ns = L"xdr:";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_GRAPHICS)		ns = L"a:";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_CHART_DRAWING)	ns = L"cdr:";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DIAGRAM)			ns = L"dgm:";
			else if (pWriter->m_lDocType == XMLWRITER_DOC_TYPE_DSP_DRAWING)		ns = L"dsp:";

			pWriter->StartNode(ns + L"extLst");
			pWriter->EndAttributes();

			for (size_t nIndex = 0; nIndex < m_arrExt.size(); nIndex++)
			{
				if (m_arrExt[nIndex])
					m_arrExt[nIndex]->toXmlWriter(pWriter);
			}

			pWriter->WriteNodeEnd(ns + L"extLst");
		}
		std::wstring COfficeArtExtensionList::toXML() const
		{
			return toXMLWithNS(L"a:");
		}
		std::wstring COfficeArtExtensionList::toXMLWithNS(const std::wstring& sNamespace) const
		{
			if (m_arrExt.empty()) return L"";

			std::wstring sResult = L"<" + sNamespace + L"extLst>";

			for ( size_t nIndex = 0; nIndex < m_arrExt.size(); nIndex++ )
			{
				if (m_arrExt[nIndex])
					sResult += m_arrExt[nIndex]->toXMLWithNS(sNamespace);
			}

			sResult += L"</" + sNamespace + L"extLst>";

			return sResult;
		}
		XLS::BaseObjectPtr COfficeArtExtensionList::toBinConnections()
		{
			auto ptr(new XLSB::FRTEXTCONNECTIONS);
			for(auto i:m_arrExt)
			{
				if(i->m_sUri == L"{DE250136-89BD-433C-8126-D09CA5730AF9}")
				{
					ptr->m_EXTCONN15 = i->m_oConnection->toBin15();
				}
			}
			return XLS::BaseObjectPtr{ptr};
		}
		XLS::BaseObjectPtr COfficeArtExtensionList::toBinWorkBook()
		{
			auto ptr(new XLSB::FRTWORKBOOK);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_arrExt.empty())
				return objectPtr;
			for(auto i:m_arrExt)
			{

				if(i->m_sUri == L"{46BE6895-7355-4a93-B00E-2C351335B9C9}")
				{
					ptr->m_TABLESLICERCACHEIDS = i->m_oSlicerCachesExt->toBinTable();
				}
				else if(i->m_sUri == L"{BBE1A952-AA13-448e-AADC-164F8A28A991}")
				{
					ptr->m_SLICERCACHEIDS = i->m_oSlicerCaches->toBin();
				}
                else if(i->m_sUri == L"{876F7934-8845-4945-9796-88D515C7AA90}")
                {
                    if(i->m_oWorkbookPivotCaches.IsInit())
                    ptr->m_SLICERCACHESPIVOTCACHEIDS = i->m_oWorkbookPivotCaches->toBin14();
                }

			}
			return objectPtr;
		}
		XLS::BaseObjectPtr COfficeArtExtensionList::toBinStyles()
		{
			 auto ptr(new XLSB::FRTSTYLESHEET);
			 XLS::BaseObjectPtr objectPtr(ptr);

                if (!m_arrExt.empty())
                {

					for(auto i:m_arrExt)
					{

						if(i->m_sUri == L"{EB79DEF2-80B8-43e5-95BD-54CBDDF9020C}")
						{
							ptr->m_STYLESHEET14 = i->m_oSlicerStyles->toBin();
						}
						else if(i->m_sUri == L"{46F421CA-312F-682F-3DD2-61675219B42D}")
						{
							ptr->m_DXF14S = i->m_oDxfs->toBin();
						}
					}

                }
			return objectPtr;
		}
		XLS::BaseObjectPtr COfficeArtExtensionList::toBinPivotCache()
		{
			auto ptr(new XLSB::FRTPIVOTCACHEDEF);

			auto ptr1(new XLSB::FRTBegin);
			XLSB::FRTProductVersion version;
			version.product = 0;
			version.version = 0x0F03;
			ptr1->productVersion = version;
			ptr->m_BrtFRTBegin = XLS::BaseObjectPtr{ptr1};

			XLS::BaseObjectPtr objectPtr(ptr);

				if (!m_arrExt.empty())
				{
					for(auto i:m_arrExt)
					{

						if(i->m_sUri == L"{725AE2AE-9491-48be-B2B4-4EB974FC3084}")
						{
							ptr->m_PCD14 = i->m_oPivotCacheDefinitionExt->toBin();
						}

					}
                }
			return objectPtr;
		}
		XLS::BaseObjectPtr COfficeArtExtensionList::toBinSlicerCache()
		{
			auto ptr(new XLSB::FRTSLICERCACHE);
			XLS::BaseObjectPtr objectPtr(ptr);
			if (!m_arrExt.empty())
			{
				for(auto i:m_arrExt)
				{
					if(i->m_sUri == L"{03082B11-2C62-411c-B77F-237D8FCFBE4C}")
					{
						auto ptr1(new XLSB::SLICERCACHEBOOKPIVOTTABLES);
						ptr->m_SLICERCACHEBOOKPIVOTTABLES = XLS::BaseObjectPtr{ptr1};
						auto ptr2(new XLSB::SlicerCacheBookPivotTables);
						ptr1->m_BrtSlicerCacheBookPivotTables = XLS::BaseObjectPtr{ptr2};
						
						auto ptr3(new XLSB::FRTBegin);
						ptr1->m_BrtFRTBegin = XLS::BaseObjectPtr{ptr3};
						XLSB::FRTProductVersion version;
						version.product = 0;
						version.version = 0x0F03;
						ptr3->productVersion = version;
						
						

						for(auto j:i->m_oSlicerCachePivotTables)
						{
							XLSB::SlicerCachePivotTable table;
							j->toBin(&table);
							ptr2->pivotTables.push_back(table);
						}
					}
					if(i->m_sUri == L"{2F2917AC-EB37-4324-AD4E-5DD8C200BD13}")
					{
						auto ptr1(new XLSB::TABLESLICERCACHE);

						auto ptr2(new XLSB::FRTBegin);
						ptr1->m_BrtFRTBegin = XLS::BaseObjectPtr{ptr2};
						XLSB::FRTProductVersion version;
						version.product = 0;
						version.version = 0x0F03;
						ptr2->productVersion = version;

						ptr->m_TABLESLICERCACHE = XLS::BaseObjectPtr{ptr1};
						ptr1->m_BrtBeginTableSlicerCache = i->m_oTableSlicerCache->toBin();
					}
					if(i->m_sUri == L"{470722E0-AACD-4C17-9CDC-17EF765DBC7E}")
					{
						auto ptr1(new XLSB::SLICERCACHECROSSFILTEREXT);
						
						auto ptr2(new XLSB::FRTBegin);
						ptr1->m_BrtFRTBegin = XLS::BaseObjectPtr{ptr2};
						XLSB::FRTProductVersion version;
						version.product = 0;
						version.version = 0;
						ptr2->productVersion = version;

						ptr->m_SLICERCACHECROSSFILTEREXT = XLS::BaseObjectPtr{ptr1};
						ptr1->m_BrtSlicerCacheHideItemsWithNoData = i->m_oSlicerCacheHideItemsWithNoData->toBin();
					}
				}
			}
			return objectPtr;
		}
		XLS::BaseObjectPtr COfficeArtExtensionList::toBinTable()
		{
			auto ptr(new XLSB::FRTTABLE);
			XLS::BaseObjectPtr objectPtr(ptr);
			auto frtBegin(new XLSB::FRTBegin);
			ptr->m_BrtFRTBegin = XLS::BaseObjectPtr{frtBegin};
				if (!m_arrExt.empty())
				{
					for(auto i:m_arrExt)
					{

						if(i->m_sUri == L"{504A1905-F514-4f6f-8877-14C23A59335A}")
						{
							ptr->m_BrtList14 = i->m_oAltTextTable->toBin();
						}

					}
                }
			return objectPtr;
		}
		XLS::BaseObjectPtr COfficeArtExtensionList::toBinWorksheet()
		{
			auto ptr(new XLSB::FRTWORKSHEET);
			XLS::BaseObjectPtr objectPtr(ptr);
			if(m_arrExt.empty())
				return objectPtr;
			for(auto i:m_arrExt)
			{
				if(i->m_sUri == L"{78C0D931-6437-407d-A8EE-F0AAD7539E65}")
				{
					auto formatPtr(new XLSB::CONDITIONALFORMATTINGS);
					ptr->m_CONDITIONALFORMATTINGS = XLS::BaseObjectPtr{formatPtr};
					for(auto j:i->m_arrConditionalFormatting)
					{
                        formatPtr->m_arCONDITIONALFORMATTING14.push_back(j->toBin14());
					}
				}
				else if(i->m_sUri == L"{CCE6A557-97BC-4B89-ADB6-D9C93CAAB3DF}")
				{
					ptr->m_DVALS14 = i->m_oDataValidations->toBin();
				}
				else if(i->m_sUri == L"{05C60535-1F16-4fd2-B633-F4F36F0B64E0}")
				{
					ptr->m_SPARKLINEGROUPS =  i->m_oSparklineGroups->toBin();
				}
				else if(i->m_sUri == L"{A8765BA9-456A-4dab-B4F3-ACF838C121DE}")
				{
					ptr->m_SLICERSEX = i->m_oSlicerList->toBin();
				}
				else if(i->m_sUri == L"{3A4CF648-6AED-40f4-86FF-DC5316D8AED3}")
				{
					if(i->m_oSlicerListExt.IsInit())
                        ptr->m_TABLESLICERSEX = i->m_oSlicerListExt->toBinTable();
				}
			}
            return objectPtr;
		}
		XLS::BaseObjectPtr COfficeArtExtensionList::toBinMetadata()
		{
			XLS::BaseObjectPtr objectPtr;
			if(m_arrExt.empty())
				return objectPtr;
			for(auto i:m_arrExt)
			{
                if(i->m_sUri == L"{bdbb8cdc-fa1e-496e-a857-3c3f30c029c3}")
				{
					auto ptr(new XLSB::FMD);
					objectPtr = XLS::BaseObjectPtr(ptr);
					auto ptr1(new XLSB::DYNAMICARRAYMETADATA);
					ptr->m_DYNAMICARRAYMETADATA = XLS::BaseObjectPtr{ptr1};
					ptr1->m_EndDynamicArrayPr = i->m_oDynamicArrayProperties->toBin();
				}
                else if(i->m_sUri == L"{3E2802C4-A4D2-4D8B-9148-E3BE6C30E623}"
                        || i->m_sUri == L"{3e2802c4-a4d2-4d8b-9148-e3be6c30e623}")
				{
					auto ptr(new XLSB::FMD);
					objectPtr = XLS::BaseObjectPtr(ptr);
					auto ptr1(new XLSB::RICHDATAMETADATA);
					ptr1->m_BeginRichValueBlock = i->m_oRichValueBlock->toBin();
				}
			}
			return objectPtr;
		}
		void COfficeArtExtensionList::fromBin(XLS::BaseObjectPtr& obj)
        {
            if (obj->get_type() == XLS::typeFRTWORKBOOK)
            {
                auto ptr = static_cast<XLSB::FRTWORKBOOK*>(obj.get());

                if (ptr != nullptr)
                {
                    if (ptr->m_TABLESLICERCACHEIDS != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{46BE6895-7355-4a93-B00E-2C351335B9C9}";
                        oExt->m_oSlicerCachesExt = ptr->m_TABLESLICERCACHEIDS;
						oExt->m_sAdditionalNamespace = L"xmlns:x15=\"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main\"";

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                    if (ptr->m_SLICERCACHEIDS != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{BBE1A952-AA13-448e-AADC-164F8A28A991}";
                        oExt->m_oSlicerCaches = ptr->m_SLICERCACHEIDS;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                    if (ptr->m_SLICERCACHESPIVOTCACHEIDS != nullptr)
                    {

                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{876F7934-8845-4945-9796-88D515C7AA90}";
						oExt->m_sAdditionalNamespace = L"xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"";
						oExt->m_oWorkbookPivotCaches.Init();
                        oExt->m_oWorkbookPivotCaches->pivotCaches14 = true;
                        oExt->m_oWorkbookPivotCaches->fromBin14(ptr->m_SLICERCACHESPIVOTCACHEIDS);
                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }
                }
            }

            else if (obj->get_type() == XLS::typeFRTWORKSHEET)
            {
                auto ptr = static_cast<XLSB::FRTWORKSHEET*>(obj.get());

                if (ptr != nullptr)
                {
                    if (ptr->m_CONDITIONALFORMATTINGS != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{78C0D931-6437-407d-A8EE-F0AAD7539E65}";

                        auto oCONDITIONALFORMATTINGS = static_cast<XLSB::CONDITIONALFORMATTINGS*>(ptr->m_CONDITIONALFORMATTINGS.get());
                        for(auto &item : oCONDITIONALFORMATTINGS->m_arCONDITIONALFORMATTING14)
                                oExt->m_arrConditionalFormatting.push_back(new OOX::Spreadsheet::CConditionalFormatting(item));

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                    if (ptr->m_DVALS14 != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{CCE6A557-97BC-4B89-ADB6-D9C93CAAB3DF}";
                        oExt->m_oDataValidations = ptr->m_DVALS14;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                    if (ptr->m_SPARKLINEGROUPS != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{05C60535-1F16-4fd2-B633-F4F36F0B64E0}";
                        oExt->m_oSparklineGroups = ptr->m_SPARKLINEGROUPS;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                    if (ptr->m_TABLESLICERSEX != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{A8765BA9-456A-4dab-B4F3-ACF838C121DE}";
                        oExt->m_oSlicerList = ptr->m_TABLESLICERSEX;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                    if (ptr->m_SLICERSEX != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{A8765BA9-456A-4dab-B4F3-ACF838C121DE}";
                        oExt->m_oSlicerList = ptr->m_SLICERSEX;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                }
            }

            else if (obj->get_type() == XLS::typeFRTSTYLESHEET)
            {
                auto ptr = static_cast<XLSB::FRTSTYLESHEET*>(obj.get());

                if (ptr != nullptr)
                {
                    if (ptr->m_STYLESHEET14 != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{EB79DEF2-80B8-43E5-95BD-54CBDDF9020C}";
                        oExt->m_oSlicerStyles = ptr->m_STYLESHEET14;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                    if (ptr->m_DXF14S != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{46F421CA-312F-682F-3DD2-61675219B42D}";
                        oExt->m_oDxfs = static_cast<XLSB::DXF14S*>(ptr->m_DXF14S.get())->m_arDXF14;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                }
            }

            else if (obj->get_type() == XLS::typeFRTTABLE)
            {
                auto ptr = static_cast<XLSB::FRTTABLE*>(obj.get());

                if (ptr != nullptr)
                {
                    if (ptr->m_BrtList14 != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{504A1905-F514-4f6f-8877-14C23A59335A}";
						oExt->m_sAdditionalNamespace = L"xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"";
                        oExt->m_oAltTextTable = ptr->m_BrtList14;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                }
            }

            else if (obj->get_type() == XLS::typeFRTQSI)
            {
                auto ptr = static_cast<XLSB::FRTQSI*>(obj.get());

                if (ptr != nullptr)
                {
                    if (ptr->m_BrtQsi15 != nullptr)
                    {
                        /*OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri == L"{883FBD77-0823-4A55-B5E3-86C4891E6966}";
                        oExt->m_o = ptr->m_BrtQsi15;

                        if (oExt)
                            m_arrExt.push_back( oExt );*/
                    }

                }
            }

            else if (obj->get_type() == XLS::typeFRTEXTCONNECTIONS)
            {
                auto ptr = static_cast<XLSB::FRTEXTCONNECTIONS*>(obj.get());

                if (ptr != nullptr)
                {
                    if (ptr->m_EXTCONN15 != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{DE250136-89BD-433C-8126-D09CA5730AF9}";
                        oExt->m_oConnection = ptr->m_EXTCONN15;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                }
            }

            else if (obj->get_type() == XLS::typeFRTSLICERCACHE)
            {
                auto ptr = static_cast<XLSB::FRTSLICERCACHE*>(obj.get());

                if (ptr != nullptr)
                {
                    if (ptr->m_SLICERCACHEBOOKPIVOTTABLES != nullptr)
                    {
                        auto ptr1 = static_cast<XLSB::SLICERCACHEBOOKPIVOTTABLES*>(ptr->m_SLICERCACHEBOOKPIVOTTABLES.get());
                        if (ptr1->m_BrtSlicerCacheBookPivotTables != nullptr)
                        {
                            OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                            oExt->m_sUri = L"{03082B11-2C62-411c-B77F-237D8FCFBE4C}";

                            auto ptrSCPT = static_cast<XLSB::SlicerCacheBookPivotTables*>(ptr1->m_BrtSlicerCacheBookPivotTables.get());
                            for(auto &item : ptrSCPT->pivotTables)
                            {
                                auto element = new OOX::Spreadsheet::CSlicerCachePivotTable();
                                element->fromBin(item);
                                oExt->m_oSlicerCachePivotTables.push_back(element);
                            }


                            if (oExt)
                                m_arrExt.push_back( oExt );
                        }
                    }

                    if (ptr->m_TABLESLICERCACHE != nullptr)
                    {
                        auto ptr1 = static_cast<XLSB::TABLESLICERCACHE*>(ptr->m_TABLESLICERCACHE.get());
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{2F2917AC-EB37-4324-AD4E-5DD8C200BD13}";
                        oExt->m_oTableSlicerCache = ptr1->m_BrtBeginTableSlicerCache;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                    if (ptr->m_SLICERCACHECROSSFILTEREXT != nullptr)
                    {
                        auto ptr1 = static_cast<XLSB::SLICERCACHECROSSFILTEREXT*>(ptr->m_SLICERCACHECROSSFILTEREXT.get());
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{470722E0-AACD-4C17-9CDC-17EF765DBC7E}";
                        oExt->m_oSlicerCacheHideItemsWithNoData = ptr1->m_BrtSlicerCacheHideItemsWithNoData;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }

                }
            }

            else if (obj->get_type() == XLS::typeFRTPIVOTCACHEDEF)
            {
                auto ptr = static_cast<XLSB::FRTPIVOTCACHEDEF*>(obj.get());

                if (ptr != nullptr)
                {
                    if (ptr->m_PCD14 != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{725AE2AE-9491-48be-B2B4-4EB974FC3084}";
                        oExt->m_sAdditionalNamespace = L"xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"";
                        oExt->m_oPivotCacheDefinitionExt = ptr->m_PCD14;

                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }
                }
            }
			else if (obj->get_type() == XLS::typeFMD)
            {
                auto ptr = static_cast<XLSB::FMD*>(obj.get());

                if (ptr != nullptr)
                {
                    if (ptr->m_DYNAMICARRAYMETADATA != nullptr)
                    {
                        OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{bdbb8cdc-fa1e-496e-a857-3c3f30c029c3}";
                        oExt->m_sAdditionalNamespace = L"xmlns:xda=\"http://schemas.microsoft.com/office/spreadsheetml/2017/dynamicarray\"";
						oExt->m_oDynamicArrayProperties = ptr->m_DYNAMICARRAYMETADATA;
                        if (oExt)
                            m_arrExt.push_back( oExt );
                    }
					else if(ptr->m_RICHDATAMETADATA != nullptr)
					{
						OOX::Drawing::COfficeArtExtension *oExt = new OOX::Drawing::COfficeArtExtension();
                        oExt->m_sUri = L"{3E2802C4-A4D2-4D8B-9148-E3BE6C30E623}";
                        oExt->m_sAdditionalNamespace = L"xmlns:xlrd=\"http://schemas.microsoft.com/office/spreadsheetml/2017/richdata\"";
						oExt->m_oRichValueBlock = ptr->m_RICHDATAMETADATA;
                        if (oExt)
                            m_arrExt.push_back( oExt );
					}
                }
            }
        }
		EElementType COfficeArtExtensionList::getType() const
		{
			return OOX::et_a_extLst;
		}
	}
}
