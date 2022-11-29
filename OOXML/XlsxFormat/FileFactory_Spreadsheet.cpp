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
#include "CommonInclude.h"

#include "FileTypes_Spreadsheet.h"
#include "Xlsx.h"

#include "../PPTXFormat/Theme.h"
#include "../DocxFormat/VmlDrawing.h"
#include "../DocxFormat/Media/OleObject.h"
#include "../DocxFormat/Media/ActiveX.h"
#include "../DocxFormat/Media/VbaProject.h"

#include "SharedStrings/SharedStrings.h"
#include "Styles/Styles.h"
#include "Workbook/Workbook.h"
#include "Worksheets/Worksheet.h"
#include "CalcChain/CalcChain.h"
#include "WorkbookComments.h"
#include "Comments/ThreadedComments.h"
#include "Comments/Comments.h"
#include "Controls/Controls.h"

#include "Pivot/PivotTable.h"
#include "Pivot/PivotCacheDefinition.h"
#include "Pivot/PivotCacheRecords.h"
#include "Slicer/SlicerCache.h"
#include "Slicer/Slicer.h"
#include "NamedSheetViews/NamedSheetViews.h"

#include "Table/Table.h"
#include "Table/QueryTable.h"
#include "Table/Connections.h"
#include "ExternalLinks/ExternalLinks.h"
#include "ExternalLinks/ExternalLinkPath.h"
#include "../DocxFormat/Diagram/DiagramDrawing.h"
#include "../DocxFormat/Diagram/DiagramData.h"
#include "../DocxFormat/Diagram/DiagramColors.h"
#include "../DocxFormat/Diagram/DiagramLayout.h"
#include "../DocxFormat/Diagram/DiagramQuickStyle.h"
#include "Chart/ChartDrawing.h"

#ifdef CreateFile
#undef CreateFile
#endif

namespace OOX
{
	namespace Spreadsheet
	{
		smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath, const OOX::Rels::CRelationShip& oRelation, OOX::Document *pMain)
		{
			OOX::CPath	oRelationFilename = oRelation.Filename();
			CPath		oFileName;
			
			if(oRelationFilename.GetIsRoot() && oRootPath.GetPath().length() > 0)
				oFileName = oRootPath / oRelationFilename;
			else
				oFileName = oPath / oRelationFilename;

			if ( oRelation.Type() == FileTypes::Workbook || 
				 oRelation.Type() == FileTypes::WorkbookMacro)
				return smart_ptr<OOX::File>(new CWorkbook( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::SharedStrings )
				return smart_ptr<OOX::File>(new CSharedStrings( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Styles )
				return smart_ptr<OOX::File>(new CStyles( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Worksheet )
				return smart_ptr<OOX::File>(new CWorksheet( pMain, oRootPath, oFileName, oRelation.rId().ToString() ));
			else if ( oRelation.Type() == FileTypes::Drawings )
				return smart_ptr<OOX::File>(new CDrawing( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::CalcChain )
				return smart_ptr<OOX::File>(new CCalcChain( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Chartsheets )
                return smart_ptr<OOX::File>(new CWorksheet( pMain, oRootPath, oFileName, oRelation.rId().ToString(), true ));
			else if ( oRelation.Type() == FileTypes::Table )
				return smart_ptr<OOX::File>(new CTableFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::QueryTable )
				return smart_ptr<OOX::File>(new CQueryTableFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::PivotTable )
				return smart_ptr<OOX::File>(new CPivotTableFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::PivotCacheDefinition )
				return smart_ptr<OOX::File>(new CPivotCacheDefinitionFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::PivotCacheRecords )
				return smart_ptr<OOX::File>(new CPivotCacheRecordsFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::SlicerCache )
				return smart_ptr<OOX::File>(new CSlicerCacheFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Slicer )
				return smart_ptr<OOX::File>(new CSlicerFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::NamedSheetView )
				return smart_ptr<OOX::File>(new CNamedSheetViewFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Comments )
				return smart_ptr<OOX::File>(new CComments( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::ThreadedComments )
                return smart_ptr<OOX::File>(new CThreadedComments( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Persons)
				return smart_ptr<OOX::File>(new CPersonList( pMain, oRootPath, oFileName));			
			else if ( oRelation.Type() == FileTypes::WorkbookComments)
				return smart_ptr<OOX::File>(new WorkbookComments( pMain, oRootPath, oFileName));			
			else if ( oRelation.Type() == FileTypes::ExternalLinks )
                return smart_ptr<OOX::File>(new CExternalLink( pMain, oRootPath, oFileName, oRelation.rId().ToString() ));
			else if ( oRelation.Type() == FileTypes::Connections )
				return smart_ptr<OOX::File>(new CConnectionsFile( pMain, oRootPath, oFileName ));
			
			else if ( oRelation.Type() == OOX::FileTypes::Chart )
				return smart_ptr<OOX::File>(new CChartFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::ChartEx )
				return smart_ptr<OOX::File>(new CChartExFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::ChartStyle )
				return smart_ptr<OOX::File>(new CChartStyleFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::ChartColors )
				return smart_ptr<OOX::File>(new CChartColorsFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::ExternalLinkPath || 
					  oRelation.Type() == OOX::FileTypes::ExternalLinkPathMissing || 
					  oRelation.Type() == OOX::FileTypes::ExternalLinkPathStartup || 
					  oRelation.Type() == OOX::FileTypes::ExternalLinkPathAlternateStartup || 
					  oRelation.Type() == OOX::FileTypes::ExternalLinkLibrary)
			{// https://msdn.microsoft.com/en-us/library/ff531845(v=office.12).aspx
				return smart_ptr<OOX::File>(new ExternalLinkPath( pMain, oRelation.Target()));
			}
			else if (  oRelation.Type() == OOX::FileTypes::OleObject)
			{
				if (oRelation.IsExternal())
				{
					return smart_ptr<OOX::File>(new OOX::OleObject( pMain, oRelationFilename ));
				}
				else
				{
					return smart_ptr<OOX::File>(new OOX::OleObject( pMain, oFileName ));
				}
			}
			else if (	oRelation.Type() == OOX::FileTypes::DiagramData)
				return smart_ptr<OOX::File>(new OOX::CDiagramData( pMain, oRootPath, oFileName ));
			else if (	oRelation.Type() == OOX::FileTypes::DiagramDrawing)
				return smart_ptr<OOX::File>(new OOX::CDiagramDrawing( pMain, oRootPath, oFileName )); 
			else if (oRelation.Type() == OOX::FileTypes::DiagramLayout)
				return smart_ptr<OOX::File>(new OOX::CDiagramLayout(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == OOX::FileTypes::DiagramColors)
				return smart_ptr<OOX::File>(new OOX::CDiagramColors(pMain, oRootPath, oFileName));
			else if (oRelation.Type() == OOX::FileTypes::DiagramQuickStyle)
				return smart_ptr<OOX::File>(new OOX::CDiagramQuickStyle(pMain, oRootPath, oFileName));
			else if (	oRelation.Type() == OOX::FileTypes::MicrosoftOfficeUnknown) //ms package
				return smart_ptr<OOX::File>(new OOX::OleObject( pMain, oFileName, true ));
			else if (	oRelation.Type() == OOX::FileTypes::ActiveX_xml)
				return smart_ptr<OOX::File>(new OOX::ActiveX_xml( pMain, oRootPath, oFileName ));
			else if (	oRelation.Type() == OOX::FileTypes::ActiveX_bin)
				return smart_ptr<OOX::File>(new OOX::ActiveX_bin( pMain, oFileName ));
			else if (	oRelation.Type() == FileTypes::CtrlProp)
				return smart_ptr<OOX::File>(new CCtrlPropFile( pMain, oRootPath, oFileName ));
			else if (	oRelation.Type() == FileTypes::XlBinaryIndex)
				return smart_ptr<OOX::File>(new UnknowTypeFile(pMain)); // ????

			return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
		}
        smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath,  OOX::Rels::CRelationShip* pRelation, OOX::Document *pMain)
		{
			if (pRelation == NULL) return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
			
			OOX::CPath oRelationFilename = pRelation->Filename();
			CPath oFileName;
			if(oRelationFilename.GetIsRoot() && oRootPath.GetPath().length() > 0)
				oFileName = oRootPath / oRelationFilename;
			else
				oFileName = oPath / oRelationFilename;

			if ( pRelation->Type() == FileTypes::Workbook || 
				 pRelation->Type() == FileTypes::WorkbookMacro)
				return smart_ptr<OOX::File>(new CWorkbook( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::SharedStrings )
				return smart_ptr<OOX::File>(new CSharedStrings( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::Styles )
				return smart_ptr<OOX::File>(new CStyles( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::Worksheet )
                return smart_ptr<OOX::File>(new CWorksheet( pMain, oRootPath, oFileName, pRelation->rId().ToString() ));
			else if ( pRelation->Type() == OOX::FileTypes::Theme )
			{
				smart_ptr<OOX::File> pFile;
				if(NSFile::CFileBinary::Exists(oFileName.GetPath()))
				{
					pFile = smart_ptr<OOX::File>(new PPTX::Theme( pMain, oFileName ));
				}
				else
				{
					pFile = smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
				}
				CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
				if (xlsx)
				{
					xlsx->m_pTheme = pFile.smart_dynamic_cast<PPTX::Theme>();
				}
				return pFile;
			}
			else if ( pRelation->Type() == OOX::FileTypes::ThemeOverride )
				return smart_ptr<OOX::File>(new PPTX::Theme( pMain, oFileName ));
			else if ( pRelation->Type() == FileTypes::Drawings )
				return smart_ptr<OOX::File>(new CDrawing( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::CalcChain )
				return smart_ptr<OOX::File>(new CCalcChain( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::Image )
				return smart_ptr<OOX::File>(new Image( pMain, oFileName ));
			else if ( pRelation->Type() == FileTypes::Chartsheets )
                return smart_ptr<OOX::File>(new CWorksheet( pMain, oRootPath, oFileName, pRelation->rId().ToString(), true ));
			else if ( pRelation->Type() == FileTypes::Table )
				return smart_ptr<OOX::File>(new CTableFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::QueryTable )
				return smart_ptr<OOX::File>(new CQueryTableFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::PivotTable )
				return smart_ptr<OOX::File>(new CPivotTableFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::PivotCacheDefinition )
				return smart_ptr<OOX::File>(new CPivotCacheDefinitionFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::PivotCacheRecords )
				return smart_ptr<OOX::File>(new CPivotCacheRecordsFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::SlicerCache )
				return smart_ptr<OOX::File>(new CSlicerCacheFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::Slicer )
				return smart_ptr<OOX::File>(new CSlicerFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::NamedSheetView )
				return smart_ptr<OOX::File>(new CNamedSheetViewFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::VmlDrawing )
				return smart_ptr<OOX::File>(new CVmlDrawing( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::ChartDrawing)
				return smart_ptr<OOX::File>(new CChartDrawing( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::Comments )
				return smart_ptr<OOX::File>(new CComments( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::ThreadedComments )
				return smart_ptr<OOX::File>(new CThreadedComments( pMain, oRootPath, oFileName ));			
			else if ( pRelation->Type() == FileTypes::Persons)
				return smart_ptr<OOX::File>(new CPersonList( pMain, oRootPath, oFileName));			
			else if ( pRelation->Type() == OOX::FileTypes::Chart )
				return smart_ptr<OOX::File>(new CChartFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::ChartEx )
				return smart_ptr<OOX::File>(new CChartExFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::ChartStyle )
				return smart_ptr<OOX::File>(new CChartStyleFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::ChartColors )
				return smart_ptr<OOX::File>(new CChartColorsFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::ExternalLinks )
                return smart_ptr<OOX::File>(new CExternalLink( pMain, oRootPath, oFileName, pRelation->rId().ToString() ));
			else if (  pRelation->Type() == OOX::FileTypes::ExternalLinkPath || 
					  pRelation->Type() == OOX::FileTypes::ExternalLinkPathMissing || 
					  pRelation->Type() == OOX::FileTypes::ExternalLinkPathStartup || 
					  pRelation->Type() == OOX::FileTypes::ExternalLinkPathAlternateStartup || 
					  pRelation->Type() == OOX::FileTypes::ExternalLinkLibrary )
			{// https://msdn.microsoft.com/en-us/library/ff531845(v=office.12).aspx)
				return smart_ptr<OOX::File>(new ExternalLinkPath( pMain, oRelationFilename ));
			}
			else if (pRelation->Type() == FileTypes::Connections )
				return smart_ptr<OOX::File>(new CConnectionsFile( pMain, oRootPath, oFileName ));
			else if (pRelation->Type() == OOX::FileTypes::OleObject)
			{
				if (pRelation->IsExternal())
				{
					return smart_ptr<OOX::File>(new OOX::OleObject( pMain, oRelationFilename ));
				}
				else
				{
					return smart_ptr<OOX::File>(new OOX::OleObject( pMain, oFileName ));
				}
			}
			else if (pRelation->Type() == OOX::FileTypes::DiagramData)
				return smart_ptr<OOX::File>(new OOX::CDiagramData( pMain, oRootPath, oFileName ));
			else if (pRelation->Type() == OOX::FileTypes::DiagramDrawing)
				return smart_ptr<OOX::File>(new OOX::CDiagramDrawing( pMain, oRootPath, oFileName )); 
			else if (pRelation->Type() == OOX::FileTypes::DiagramLayout)
				return smart_ptr<OOX::File>(new OOX::CDiagramLayout(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == OOX::FileTypes::DiagramColors)
				return smart_ptr<OOX::File>(new OOX::CDiagramColors(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == OOX::FileTypes::DiagramQuickStyle)
				return smart_ptr<OOX::File>(new OOX::CDiagramQuickStyle(pMain, oRootPath, oFileName));
			else if (pRelation->Type() == OOX::FileTypes::MicrosoftOfficeUnknown) //ms package
				return smart_ptr<OOX::File>(new OOX::OleObject( pMain, oFileName, true ));
			else if (pRelation->Type() == OOX::FileTypes::ActiveX_xml)
				return smart_ptr<OOX::File>(new OOX::ActiveX_xml( pMain, oRootPath, oFileName ));
			else if (pRelation->Type() == OOX::FileTypes::ActiveX_bin)
				return smart_ptr<OOX::File>(new OOX::ActiveX_bin( pMain, oFileName ));
			else if (pRelation->Type() == FileTypes::CtrlProp)
				return smart_ptr<OOX::File>(new CCtrlPropFile( pMain, oRootPath, oFileName ));
			else if (pRelation->Type() == FileTypes::WorkbookComments)
				return smart_ptr<OOX::File>(new WorkbookComments( pMain, oRootPath, oFileName));
			else if (pRelation->Type() == FileTypes::XlBinaryIndex)
				return smart_ptr<OOX::File>(new UnknowTypeFile(pMain)); // ????

			return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
		}
	}
} // namespace OOX
