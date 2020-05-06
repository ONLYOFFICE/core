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
#include "DrawingExt.h"
#include "../../XlsxFormat/Worksheets/Sparkline.h"
#include "../../XlsxFormat/Table/Table.h"
#include "../../XlsxFormat/Table/Connections.h"
#include "../Diagram/DiagramData.h"
#include "../../XlsxFormat/Worksheets/ConditionalFormatting.h"
#include "../../XlsxFormat/Worksheets/DataValidation.h"
#include "../../XlsxFormat/Slicer/SlicerCache.h"
#include "../../XlsxFormat/Slicer/SlicerCacheExt.h"

namespace OOX
{
	namespace Drawing
	{
		CCompatExt::~CCompatExt()
		{
		}

		COfficeArtExtension::~COfficeArtExtension()
		{
			m_oSparklineGroups.reset();
			m_oAltTextTable.reset();
			m_oDataModelExt.reset();
			m_oCompatExt.reset();
			m_oDataValidations.reset();

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

            if ((m_sUri.IsInit()) && (*m_sUri == L"{63B3BB69-23CF-44E3-9099-C40C66FF867C}"	||
                                      *m_sUri == L"{05C60535-1F16-4fd2-B633-F4F36F0B64E0}"	|| 
									  *m_sUri == L"{504A1905-F514-4f6f-8877-14C23A59335A}"	|| 
									  *m_sUri == L"{78C0D931-6437-407d-A8EE-F0AAD7539E65}"	||
									  *m_sUri == L"{B025F937-C7B1-47D3-B67F-A62EFF666E3E}"	||
									  *m_sUri == L"{CCE6A557-97BC-4b89-ADB6-D9C93CAAB3DF}"	||
									  *m_sUri == L"{A8765BA9-456A-4dab-B4F3-ACF838C121DE}"	||
									  *m_sUri == L"{3A4CF648-6AED-40f4-86FF-DC5316D8AED3}"	||
									  *m_sUri == L"{BBE1A952-AA13-448e-AADC-164F8A28A991}"	||
									  *m_sUri == L"{46BE6895-7355-4a93-B00E-2C351335B9C9}"	||
									  *m_sUri == L"{EB79DEF2-80B8-43e5-95BD-54CBDDF9020C}"	||
									  *m_sUri == L"{03082B11-2C62-411c-B77F-237D8FCFBE4C}"	||
									  *m_sUri == L"{2F2917AC-EB37-4324-AD4E-5DD8C200BD13}"	||
									  *m_sUri == L"{470722E0-AACD-4C17-9CDC-17EF765DBC7E}"	||
									  *m_sUri == L"{46F421CA-312F-682f-3DD2-61675219B42D}"	||
									  *m_sUri == L"{DE250136-89BD-433C-8126-D09CA5730AF9}"	||
									  *m_sUri == L"http://schemas.microsoft.com/office/drawing/2008/diagram"))   
			{
				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
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
					else if (sName == L"conditionalFormattings")
					{
						if ( oReader.IsEmptyNode() )
							continue;

						int nCurDepth1 = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nCurDepth1 ) )
						{						
							m_arrConditionalFormatting.push_back(new OOX::Spreadsheet::CConditionalFormatting(oReader));
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
					else if (sName == L"slicerCachePivotTables")
					{
						if ( oReader.IsEmptyNode() )
							continue;

						int nCurDepth1 = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nCurDepth1 ) )
						{
							m_oSlicerCachePivotTables.push_back(new OOX::Spreadsheet::CSlicerCachePivotTable(oReader));
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
				}
			}
			else
			{
				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}
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
                sResult += L" uri=\"" + m_sUri.get2() + L"\"";
			}

			if (!m_sAdditionalNamespace.empty())
			{
				sResult += L" " + m_sAdditionalNamespace;
			}

			sResult += L">";

			if(m_oCompatExt.IsInit())
			{
				sResult += m_oCompatExt->toXML();
			}
			if(m_oSparklineGroups.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSparklineGroups->toXML(writer);
                sResult += writer.GetData().c_str();
			}
			if(m_oAltTextTable.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oAltTextTable->toXML(writer);
				sResult += writer.GetData().c_str();
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
			if(m_oDataValidations.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oDataValidations->toXML2(writer, true);
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerList.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerList->toXML(writer, L"x14:slicerList");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerListExt.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerListExt->toXML(writer, L"x14:slicerList");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerCaches.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerCaches->toXML(writer, L"slicerCaches", L"x14:");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerCachesExt.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerCachesExt->toXML(writer, L"slicerCaches", L"x15:");
				sResult += writer.GetData().c_str();
			}
			if(m_oDxfs.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oDxfs->toXML2(writer, L"x14:dxfs");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerStyles.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerStyles->toXML(writer, L"x14:slicerStyles");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerCachePivotTables.size() > 0)
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
			if(m_oTableSlicerCache.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oTableSlicerCache->toXML(writer, L"x15:tableSlicerCache");
				sResult += writer.GetData().c_str();
			}
			if(m_oSlicerCacheHideItemsWithNoData.IsInit())
			{
				NSStringUtils::CStringBuilder writer;
				m_oSlicerCacheHideItemsWithNoData->toXML(writer, L"x15:slicerCacheHideItemsWithNoData");
				sResult += writer.GetData().c_str();
			}
			if (m_oId.IsInit())
			{
				sResult += L"<" + sNamespace + L"id>" + m_oId.get2() + L"</" + sNamespace + L"id>";
			}
            sResult += L"</" + sNamespace + L"ext>";

			return sResult;
		}
	}
}
