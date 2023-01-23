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
#include "FileFactory.h"
#include "File.h"
#include "FileTypes.h"
#include "Rels.h"
#include "App.h"
#include "Core.h"
#include "Document.h"
#include "Settings/Settings.h"
#include "Settings/WebSettings.h"
#include "FontTable.h" 
#include "Styles.h"
#include "Bibliography.h"
#include "Footnote.h"
#include "Endnote.h"
#include "Media/Image.h"
#include "Media/ActiveX.h"
#include "Media/OleObject.h"
#include "Media/Audio.h"
#include "Media/Video.h"
#include "Media/VbaProject.h"
#include "Media/JsaProject.h"
#include "External/HyperLink.h"
#include "HeaderFooter.h"
#include "Numbering.h"
#include "Comments.h"
#include "UnknowTypeFile.h"
#include "Diagram/DiagramDrawing.h"
#include "Diagram/DiagramData.h"
#include "Diagram/DiagramColors.h"
#include "Diagram/DiagramLayout.h"
#include "Diagram/DiagramQuickStyle.h"
#include "VmlDrawing.h"
#include "CustomXml.h"

#include "../XlsxFormat/Chart/Chart.h"
#include "../XlsxFormat/Chart/ChartDrawing.h"
#include "../PPTXFormat/Theme.h"
#include "../PPTXFormat/Logic/HeadingVariant.h"

namespace OOX
{
	const smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath, const OOX::Rels::CRelationShip& oRelation, OOX::Document *pMain)
	{
		OOX::CPath oRelationFilename = oRelation.Filename();
		CPath oFileName;
		if(oRelationFilename.GetIsRoot() && oRootPath.GetPath().length() > 0)
			oFileName = oRootPath / oRelationFilename;
		else
			oFileName = oPath / oRelationFilename;

		if ( oRelation.Type() == FileTypes::Document || oRelation.Type() == FileTypes::DocumentMacro)
			return smart_ptr<OOX::File>(new CDocument( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::FontTable)
			return smart_ptr<OOX::File>(new CFontTable( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::Style)
			return smart_ptr<OOX::File>(new CStyles( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::Bibliography)
			return smart_ptr<OOX::File>(new CBibliography( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::FootNote)
			return smart_ptr<OOX::File>(new CFootnotes( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::EndNote)
			return smart_ptr<OOX::File>(new CEndnotes( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::WebSetting)
			return smart_ptr<OOX::File>(new CWebSettings( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::Numbering)
			return smart_ptr<OOX::File>(new CNumbering( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::Header)
			return smart_ptr<OOX::File>(new CHdrFtr( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::Footer)
			return smart_ptr<OOX::File>(new CHdrFtr( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::Comments)
			return smart_ptr<OOX::File>(new CComments( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::DocumentComments)
			return smart_ptr<OOX::File>(new CDocumentComments( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::CommentsExt )
			return smart_ptr<OOX::File>(new CCommentsExt( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::DocumentCommentsExt )
			return smart_ptr<OOX::File>(new CDocumentCommentsExt( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::CommentsExtensible )
			return smart_ptr<OOX::File>(new CCommentsExtensible( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::DocumentCommentsExtensible )
			return smart_ptr<OOX::File>(new CDocumentCommentsExtensible( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::CommentsIds )
			return smart_ptr<OOX::File>(new CCommentsIds( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::DocumentCommentsIds )
			return smart_ptr<OOX::File>(new CDocumentCommentsIds( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::CommentsUserData )
			return smart_ptr<OOX::File>(new CCommentsUserData( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::People )
			return smart_ptr<OOX::File>(new CPeople( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::DocumentPeople )
			return smart_ptr<OOX::File>(new CDocumentPeople( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::ImportedExternalContent )
			return smart_ptr<OOX::File>(new Media( pMain, oFileName, oRelation.IsExternal() ));
		else if (oRelation.Type() == FileTypes::GlossaryDocument)
			return smart_ptr<OOX::File>(new CDocument(pMain, oRootPath, oFileName));
//common		
		else if ( oRelation.Type() == FileTypes::Setting)
			return smart_ptr<OOX::File>(new CSettings( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::App )
			return smart_ptr<OOX::File>(new CApp( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::Core)
			return smart_ptr<OOX::File>(new CCore( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::CustomProperties)
		{
			PPTX::FileMap tmp;
			return smart_ptr<OOX::File>(new PPTX::CustomProperties( pMain, oFileName, tmp));
		}
		else if ( oRelation.Type() == FileTypes::Theme)
			return smart_ptr<OOX::File>(new PPTX::Theme( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::ThemeOverride)
			return smart_ptr<OOX::File>(new PPTX::Theme( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::HyperLink)
			return smart_ptr<OOX::File>(new HyperLink( pMain, oRelation.Target()));
		else if ( oRelation.Type() == FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OleObject( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::Image)
			return smart_ptr<OOX::File>(new Image( pMain, oFileName, oRelation.IsExternal() ));
		else if ( oRelation.Type() == FileTypes::Audio)
			return smart_ptr<OOX::File>(new Audio( pMain, oFileName, oRelation.IsExternal() ));
		else if ( oRelation.Type() == FileTypes::Video)
			return smart_ptr<OOX::File>(new Video( pMain, oFileName, oRelation.IsExternal() ));
		else if (oRelation.Type() == FileTypes::DiagramLayout)
			return smart_ptr<OOX::File>(new CDiagramLayout(pMain, oRootPath, oFileName));
		else if (oRelation.Type() == FileTypes::DiagramData)
			return smart_ptr<OOX::File>(new CDiagramData( pMain, oRootPath, oFileName ));
		else if (oRelation.Type() == FileTypes::DiagramDrawing)
			return smart_ptr<OOX::File>(new CDiagramDrawing( pMain, oRootPath, oFileName )); 
		else if (oRelation.Type() == FileTypes::DiagramColors)
			return smart_ptr<OOX::File>(new CDiagramColors(pMain, oRootPath, oFileName));
		else if (oRelation.Type() == FileTypes::DiagramQuickStyle)
			return smart_ptr<OOX::File>(new CDiagramQuickStyle(pMain, oRootPath, oFileName));
		else if (oRelation.Type() == FileTypes::MicrosoftOfficeUnknown) //ms package
			return smart_ptr<OOX::File>(new OleObject( pMain, oFileName, true ));
		else if ( oRelation.Type() == OOX::FileTypes::VmlDrawing )
			return smart_ptr<OOX::File>(new CVmlDrawing( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::ChartDrawing)
			return smart_ptr<OOX::File>(new CChartDrawing( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::Chart )
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartFile( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::ChartEx )
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartExFile( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::ChartStyle )
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartStyleFile( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::ChartColors )
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartColorsFile( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::ActiveX_xml)
			return smart_ptr<OOX::File>(new OOX::ActiveX_xml( pMain, oRootPath, oFileName));
		else if ( oRelation.Type() == OOX::FileTypes::ActiveX_bin)
			return smart_ptr<OOX::File>(new OOX::ActiveX_bin( pMain, oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::VbaProject)
			return smart_ptr<OOX::File>(new OOX::VbaProject( pMain, oRootPath, oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::VbaData)
			return smart_ptr<OOX::File>(new OOX::VbaData( pMain, oRootPath, oFileName));
		else if ( oRelation.Type() == FileTypes::JsaProject)
			return smart_ptr<OOX::File>(new JsaProject( pMain, oFileName ));
		else if ( oRelation.Type() == FileTypes::CustomXml)
			return smart_ptr<OOX::File>(new OOX::CCustomXML(pMain, oRootPath, oFileName));
		else if ( oRelation.Type() == FileTypes::CustomXmlProps)
			return smart_ptr<OOX::File>(new OOX::CCustomXMLProps(pMain, oFileName));

		return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
	}

	const smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath, OOX::Rels::CRelationShip* pRelation, OOX::Document* pMain)
	{
		if (pRelation == NULL) return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );

		OOX::CPath oRelationFilename = pRelation->Filename();
		CPath oFileName;
		if(oRelationFilename.GetIsRoot() && oRootPath.GetPath().length() > 0)
			oFileName = oRootPath / oRelationFilename;
		else
			oFileName = oPath / oRelationFilename;

		if ( pRelation->Type() == FileTypes::Document)
			return smart_ptr<OOX::File>(new CDocument( pMain, oRootPath, oFileName, FileTypes::Document));
		else if (pRelation->Type() == FileTypes::DocumentMacro)
			return smart_ptr<OOX::File>(new CDocument(pMain, oRootPath, oFileName, FileTypes::DocumentMacro));
		else if (pRelation->Type() == FileTypes::GlossaryDocument)
			return smart_ptr<OOX::File>(new CDocument(pMain, oRootPath, oFileName, FileTypes::GlossaryDocument));

		else if (pRelation->Type() == FileTypes::App)
			return smart_ptr<OOX::File>(new CApp(pMain, oFileName));
		else if (pRelation->Type() == FileTypes::Core)
			return smart_ptr<OOX::File>(new CCore(pMain, oFileName));
		else if ( pRelation->Type() == FileTypes::CustomProperties)
		{
			PPTX::FileMap tmp;
			return smart_ptr<OOX::File>(new PPTX::CustomProperties( pMain, oFileName, tmp ));
		}
		else if ( pRelation->Type() == FileTypes::Theme)
		{
			if(NSFile::CFileBinary::Exists(oFileName.GetPath()))
			{
				return smart_ptr<OOX::File>(new PPTX::Theme( pMain, oFileName ));
			}
			else
			{
				return smart_ptr<OOX::File>( new UnknowTypeFile(pMain) );
			}
		}
		else if ( pRelation->Type() == FileTypes::ThemeOverride)
			return smart_ptr<OOX::File>(new PPTX::Theme( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::Setting)
			return smart_ptr<OOX::File>(new CSettings( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::FontTable)
			return smart_ptr<OOX::File>(new CFontTable( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::Style)
			return smart_ptr<OOX::File>(new CStyles( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::Bibliography)
			return smart_ptr<OOX::File>(new CBibliography( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::FootNote)
			return smart_ptr<OOX::File>(new CFootnotes( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::EndNote)
			return smart_ptr<OOX::File>(new CEndnotes( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::WebSetting)
			return smart_ptr<OOX::File>(new CWebSettings( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::HyperLink)
			return smart_ptr<OOX::File>(new HyperLink( pMain, pRelation->Target()));
		else if ( pRelation->Type() == FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OleObject( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::Image)
			return smart_ptr<OOX::File>(new Image( pMain, oFileName, pRelation->IsExternal() ));
		else if ( pRelation->Type() == FileTypes::Audio)
			return smart_ptr<OOX::File>(new Audio( pMain, oFileName, pRelation->IsExternal() ));
		else if ( pRelation->Type() == FileTypes::Video)
			return smart_ptr<OOX::File>(new Video( pMain, oFileName, pRelation->IsExternal() ));
		else if ( pRelation->Type() == FileTypes::Numbering)
			return smart_ptr<OOX::File>(new CNumbering( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::Header)
			return smart_ptr<OOX::File>(new CHdrFtr( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::Footer)
			return smart_ptr<OOX::File>(new CHdrFtr( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::Comments)
			return smart_ptr<OOX::File>(new CComments( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::DocumentComments)
			return smart_ptr<OOX::File>(new CDocumentComments( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::CommentsExt )
			return smart_ptr<OOX::File>(new CCommentsExt( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::DocumentCommentsExt )
			return smart_ptr<OOX::File>(new CDocumentCommentsExt( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::CommentsExtensible )
			return smart_ptr<OOX::File>(new CCommentsExtensible( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::DocumentCommentsExtensible )
			return smart_ptr<OOX::File>(new CDocumentCommentsExtensible( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::CommentsIds)
			return smart_ptr<OOX::File>(new CCommentsIds( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::DocumentCommentsIds)
			return smart_ptr<OOX::File>(new CDocumentCommentsIds( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::CommentsUserData)
			return smart_ptr<OOX::File>(new CCommentsUserData( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::People )
			return smart_ptr<OOX::File>(new CPeople( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::DocumentPeople )
			return smart_ptr<OOX::File>(new CDocumentPeople( pMain, oFileName ));
		else if (pRelation->Type() == FileTypes::DiagramData)
			return smart_ptr<OOX::File>(new CDiagramData( pMain, oRootPath, oFileName ));
		else if (pRelation->Type() == FileTypes::DiagramDrawing)
			return smart_ptr<OOX::File>(new CDiagramDrawing( pMain, oRootPath, oFileName )); 
		else if (pRelation->Type() == FileTypes::DiagramLayout)
			return smart_ptr<OOX::File>(new CDiagramLayout(pMain, oRootPath, oFileName));
		else if (pRelation->Type() == FileTypes::DiagramColors)
			return smart_ptr<OOX::File>(new CDiagramColors(pMain, oRootPath, oFileName));
		else if (pRelation->Type() == FileTypes::DiagramQuickStyle)
			return smart_ptr<OOX::File>(new CDiagramQuickStyle(pMain, oRootPath, oFileName));
		else if ( pRelation->Type() == OOX::FileTypes::ChartDrawing)
			return smart_ptr<OOX::File>(new CChartDrawing( pMain, oRootPath, oFileName ));
		else if (pRelation->Type() == FileTypes::MicrosoftOfficeUnknown) //ms package
			return smart_ptr<OOX::File>(new OleObject( pMain, oFileName, true ));
		else if ( pRelation->Type() == OOX::FileTypes::Chart )
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartFile( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == OOX::FileTypes::ChartEx )
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartExFile( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == OOX::FileTypes::ChartStyle )
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartStyleFile( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == OOX::FileTypes::ChartColors)
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartColorsFile( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::ActiveX_xml)
			return smart_ptr<OOX::File>(new ActiveX_xml( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::ActiveX_bin)
			return smart_ptr<OOX::File>(new ActiveX_bin( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::VbaProject)
			return smart_ptr<OOX::File>(new OOX::VbaProject( pMain, oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::VbaData)
			return smart_ptr<OOX::File>(new OOX::VbaData( pMain, oRootPath, oFileName));
		else if ( pRelation->Type() == FileTypes::JsaProject)
			return smart_ptr<OOX::File>(new JsaProject( pMain, oFileName ));
		else if ( pRelation->Type() == FileTypes::CustomXml)
			return smart_ptr<OOX::File>(new OOX::CCustomXML(pMain, oRootPath, oFileName));
		else if ( pRelation->Type() == FileTypes::CustomXmlProps)
			return smart_ptr<OOX::File>(new OOX::CCustomXMLProps(pMain, oFileName));
		else if ( pRelation->Type() == FileTypes::ImportedExternalContent )
			return smart_ptr<OOX::File>(new Media( pMain, oFileName, pRelation->IsExternal() ));

		return smart_ptr<OOX::File>( new UnknowTypeFile(pMain, pRelation->Target()) );
	}

} // namespace OOX
