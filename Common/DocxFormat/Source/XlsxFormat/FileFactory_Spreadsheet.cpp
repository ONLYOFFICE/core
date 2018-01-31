﻿/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "../../../../ASCOfficePPTXFile/PPTXFormat/Theme.h"
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

#include "Table/Table.h"
#include "ExternalLinks/ExternalLinks.h"
#include "ExternalLinks/ExternalLinkPath.h"
#include "../DocxFormat/Diagram/DiagramDrawing.h"
#include "../DocxFormat/Diagram/DiagramData.h"

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
				return smart_ptr<OOX::File>(new CWorksheet( pMain, oRootPath, oFileName, oRelation.rId().ToString() ));
			else if ( oRelation.Type() == FileTypes::Table )
				return smart_ptr<OOX::File>(new CTableFile( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::Comments )
				return smart_ptr<OOX::File>(new CComments( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == FileTypes::ExternalLinks )
				return smart_ptr<OOX::File>(new CExternalLink( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::Chart )
				return smart_ptr<OOX::File>(new CChartSpace( pMain, oRootPath, oFileName ));
			else if ( oRelation.Type() == OOX::FileTypes::ExternalLinkPath)
				return smart_ptr<OOX::File>(new ExternalLinkPath( pMain, oRelation.Target()));
			else if ( oRelation.Type() == FileTypes::WorkbookComments)
				return smart_ptr<OOX::File>(new WorkbookComments( pMain, oRootPath, oFileName));
//common			
			//else if ( oRelation.Type() == OOX::FileTypes::VmlDrawing )
			//	return smart_ptr<OOX::File>(new CVmlDrawing( oRootPath, oFileName ));
			//else if ( oRelation.Type() == OOX::FileTypes::Theme )
			//	return smart_ptr<OOX::File>(new PPTX::Theme( oFileName ));
			//else if ( oRelation.Type() == OOX::FileTypes::Image )
			//	return smart_ptr<OOX::File>(new Image( oFileName ));
			//else if ( oRelation.Type() == OOX::FileTypes::ThemeOverride )
			//	return smart_ptr<OOX::File>(new PPTX::Theme( oFileName ));
			//else if (  oRelation.Type() == OOX::FileTypes::OleObject)
			//	return smart_ptr<OOX::File>(new OOX::OleObject( oFileName ));
			//else if (	oRelation.Type() == OOX::FileTypes::Data)			
			//	return smart_ptr<OOX::File>(new OOX::CDiagramData( oRootPath, oFileName ));
			//else if (	oRelation.Type() == OOX::FileTypes::DiagDrawing)
			//	return smart_ptr<OOX::File>(new OOX::CDiagramDrawing( oRootPath, oFileName )); 
			//else if (	oRelation.Type() == OOX::FileTypes::MicrosoftOfficeUnknown) //ms package
			//	return smart_ptr<OOX::File>(new OOX::OleObject( oFileName, true ));

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
				return smart_ptr<OOX::File>(new CWorksheet( pMain, oRootPath, oFileName, pRelation->rId().ToString() ));
			else if ( pRelation->Type() == FileTypes::Table )
				return smart_ptr<OOX::File>(new CTableFile( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::VmlDrawing )
				return smart_ptr<OOX::File>(new CVmlDrawing( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::Comments )
				return smart_ptr<OOX::File>(new CComments( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == OOX::FileTypes::Chart )
				return smart_ptr<OOX::File>(new CChartSpace( pMain, oRootPath, oFileName ));
			else if ( pRelation->Type() == FileTypes::ExternalLinks )
				return smart_ptr<OOX::File>(new CExternalLink( pMain, oRootPath, oFileName ));
			else if (  pRelation->Type() == OOX::FileTypes::ExternalLinkPath)
				return smart_ptr<OOX::File>(new ExternalLinkPath( pMain, oRelationFilename ));
			else if (  pRelation->Type() == OOX::FileTypes::OleObject)
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
			else if (	pRelation->Type() == OOX::FileTypes::Data)
				return smart_ptr<OOX::File>(new OOX::CDiagramData( pMain, oRootPath, oFileName ));
			else if (	pRelation->Type() == OOX::FileTypes::DiagDrawing)
				return smart_ptr<OOX::File>(new OOX::CDiagramDrawing( pMain, oRootPath, oFileName )); 
			else if (	pRelation->Type() == OOX::FileTypes::MicrosoftOfficeUnknown) //ms package
				return smart_ptr<OOX::File>(new OOX::OleObject( pMain, oFileName, true ));
			else if (	pRelation->Type() == OOX::FileTypes::ActiveX_xml)
				return smart_ptr<OOX::File>(new OOX::ActiveX_xml( pMain, oRootPath, oFileName ));
			else if (	pRelation->Type() == OOX::FileTypes::ActiveX_bin)
				return smart_ptr<OOX::File>(new OOX::ActiveX_bin( pMain, oFileName ));
			else if ( pRelation->Type() == FileTypes::WorkbookComments)
				return smart_ptr<OOX::File>(new WorkbookComments( pMain, oRootPath, oFileName));

			return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
		}
	}
} // namespace OOX
