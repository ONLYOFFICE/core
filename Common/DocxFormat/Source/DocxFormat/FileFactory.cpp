/*
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
#include "External/HyperLink.h"
#include "External/ExternalVideo.h"
#include "External/ExternalAudio.h"
#include "External/ExternalImage.h"
#include "HeaderFooter.h"
#include "Numbering.h"
#include "Comments.h"
#include "UnknowTypeFile.h"
#include "Diagram/DiagramDrawing.h"
#include "Diagram/DiagramData.h"
#include "VmlDrawing.h"

#include "../XlsxFormat/Chart/Chart.h"
#include "../../../../ASCOfficePPTXFile/PPTXFormat/Theme.h"

namespace OOX
{
	const smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath, const OOX::Rels::CRelationShip& oRelation)
	{
		OOX::CPath oRelationFilename = oRelation.Filename();
		CPath oFileName;
		if(oRelationFilename.GetIsRoot() && oRootPath.GetPath().length() > 0)
			oFileName = oRootPath / oRelationFilename;
		else
			oFileName = oPath / oRelationFilename;

		if ( oRelation.Type() == FileTypes::Document || oRelation.Type() == FileTypes::DocumentMacro)
			return smart_ptr<OOX::File>(new CDocument( oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::FontTable)
			return smart_ptr<OOX::File>(new CFontTable( oFileName ));
		else if ( oRelation.Type() == FileTypes::Style)
			return smart_ptr<OOX::File>(new CStyles( oFileName ));
		else if ( oRelation.Type() == FileTypes::Bibliography)
			return smart_ptr<OOX::File>(new CBibliography( oFileName ));
		else if ( oRelation.Type() == FileTypes::FootNote)
			return smart_ptr<OOX::File>(new CFootnotes( oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::EndNote)
			return smart_ptr<OOX::File>(new CEndnotes( oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::WebSetting)
			return smart_ptr<OOX::File>(new CWebSettings( oFileName ));
		else if ( oRelation.Type() == FileTypes::Numbering)
			return smart_ptr<OOX::File>(new CNumbering( oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::Header)
			return smart_ptr<OOX::File>(new CHdrFtr( oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::Footer)
			return smart_ptr<OOX::File>(new CHdrFtr( oRootPath, oFileName ));
		else if ( oRelation.Type() == FileTypes::Comments)
			return smart_ptr<OOX::File>(new CComments( oFileName ));
		else if ( oRelation.Type() == FileTypes::CommentsExt )
			return smart_ptr<OOX::File>(new CCommentsExt( oFileName ));
		else if ( oRelation.Type() == FileTypes::People )
			return smart_ptr<OOX::File>(new CPeople( oFileName ));
//common		
		else if ( oRelation.Type() == FileTypes::Setting)
			return smart_ptr<OOX::File>(new CSettings( oFileName ));
		else if ( oRelation.Type() == FileTypes::App )
			return smart_ptr<OOX::File>(new CApp( oFileName ));
		else if ( oRelation.Type() == FileTypes::Core)
			return smart_ptr<OOX::File>(new CCore( oFileName ));
		else if ( oRelation.Type() == FileTypes::Theme)
			return smart_ptr<OOX::File>(new PPTX::Theme( oFileName ));
		else if ( oRelation.Type() == FileTypes::ThemeOverride)
			return smart_ptr<OOX::File>(new PPTX::Theme( oFileName ));
		else if ( oRelation.Type() == FileTypes::HyperLink)
			return smart_ptr<OOX::File>(new HyperLink( oRelation.Target()));
		else if (( oRelation.Type() == FileTypes::ExternalVideo ) && ( oRelation.IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalVideo( oRelation.Target()));
		else if (( oRelation.Type() == FileTypes::ExternalAudio ) && ( oRelation.IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalAudio( oRelation.Target()));
		else if (( oRelation.Type() == FileTypes::ExternalImage ) && ( oRelation.IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalImage( oRelation.Target()));
		else if ( oRelation.Type() == FileTypes::Image)
			return smart_ptr<OOX::File>(new Image( oFileName ));
		else if ( oRelation.Type() == FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OleObject( oFileName ));
		else if ( oRelation.Type() == FileTypes::Audio)
			return smart_ptr<OOX::File>(new Audio( oFileName ));
		else if ( oRelation.Type() == FileTypes::Video)
			return smart_ptr<OOX::File>(new Video( oFileName ));
		else if (oRelation.Type() == FileTypes::Data)			
			return smart_ptr<OOX::File>(new CDiagramData( oRootPath, oFileName ));
		else if (oRelation.Type() == FileTypes::DiagDrawing)
			return smart_ptr<OOX::File>(new CDiagramDrawing( oRootPath, oFileName )); 
		else if (oRelation.Type() == FileTypes::MicrosoftOfficeUnknown) //ms package
			return smart_ptr<OOX::File>(new OleObject( oFileName, true ));
		else if ( oRelation.Type() == OOX::FileTypes::VmlDrawing )
			return smart_ptr<OOX::File>(new CVmlDrawing( oRootPath, oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::Chart )
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartSpace( oRootPath, oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::ActiveX_xml)
			return smart_ptr<OOX::File>(new OOX::ActiveX_xml( oRootPath, oFileName));
		else if ( oRelation.Type() == OOX::FileTypes::ActiveX_bin)
			return smart_ptr<OOX::File>(new OOX::ActiveX_bin( oFileName ));
		else if ( oRelation.Type() == OOX::FileTypes::VbaProject)
			return smart_ptr<OOX::File>(new OOX::VbaProject( oRootPath, oFileName ));
		//else if ( oRelation.Type() == OOX::FileTypes::VbaData)
		//	return smart_ptr<OOX::File>(new OOX::VbaData( oFileName ));

		return smart_ptr<OOX::File>( new UnknowTypeFile() );
	}

	const smart_ptr<OOX::File> CreateFile(const OOX::CPath& oRootPath, const OOX::CPath& oPath, OOX::Rels::CRelationShip* pRelation)
	{
		if (pRelation == NULL) return smart_ptr<OOX::File>( new UnknowTypeFile() );

		OOX::CPath oRelationFilename = pRelation->Filename();
		CPath oFileName;
		if(oRelationFilename.GetIsRoot() && oRootPath.GetPath().length() > 0)
			oFileName = oRootPath / oRelationFilename;
		else
			oFileName = oPath / oRelationFilename;

		if ( pRelation->Type() == FileTypes::App )
			return smart_ptr<OOX::File>(new CApp( oFileName ));
		else if ( pRelation->Type() == FileTypes::Core)
			return smart_ptr<OOX::File>(new CCore( oFileName ));
		else if ( pRelation->Type() == FileTypes::Document || pRelation->Type() == FileTypes::DocumentMacro)
			return smart_ptr<OOX::File>(new CDocument( oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::Theme)
		{
			if(NSFile::CFileBinary::Exists(oFileName.GetPath()))
			{
				return smart_ptr<OOX::File>(new PPTX::Theme( oFileName ));
			}
			else
			{
				return smart_ptr<OOX::File>( new UnknowTypeFile() );
			}
		}
		else if ( pRelation->Type() == FileTypes::ThemeOverride)
			return smart_ptr<OOX::File>(new PPTX::Theme( oFileName ));
		else if ( pRelation->Type() == FileTypes::Setting)
			return smart_ptr<OOX::File>(new CSettings( oFileName ));
		else if ( pRelation->Type() == FileTypes::FontTable)
			return smart_ptr<OOX::File>(new CFontTable( oFileName ));
		else if ( pRelation->Type() == FileTypes::Style)
			return smart_ptr<OOX::File>(new CStyles( oFileName ));
		else if ( pRelation->Type() == FileTypes::Bibliography)
			return smart_ptr<OOX::File>(new CBibliography( oFileName ));
		else if ( pRelation->Type() == FileTypes::FootNote)
			return smart_ptr<OOX::File>(new CFootnotes( oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::EndNote)
			return smart_ptr<OOX::File>(new CEndnotes( oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::WebSetting)
			return smart_ptr<OOX::File>(new CWebSettings( oFileName ));
		else if ( pRelation->Type() == FileTypes::HyperLink)
			return smart_ptr<OOX::File>(new HyperLink( pRelation->Target()));
		else if (( pRelation->Type() == FileTypes::ExternalVideo ) && ( pRelation->IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalVideo( pRelation->Target()));
		else if (( pRelation->Type() == FileTypes::ExternalAudio ) && ( pRelation->IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalAudio( pRelation->Target()));
		else if (( pRelation->Type() == FileTypes::ExternalImage ) && ( pRelation->IsExternal() ))
			return smart_ptr<OOX::File>(new ExternalImage( pRelation->Target()));
		else if ( pRelation->Type() == FileTypes::Image)
			return smart_ptr<OOX::File>(new Image( oFileName ));
		else if ( pRelation->Type() == FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OleObject( oFileName ));
		else if ( pRelation->Type() == FileTypes::Audio)
			return smart_ptr<OOX::File>(new Audio( oFileName ));
		else if ( pRelation->Type() == FileTypes::Video)
			return smart_ptr<OOX::File>(new Video( oFileName ));
		else if ( pRelation->Type() == FileTypes::Numbering)
			return smart_ptr<OOX::File>(new CNumbering( oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::Header)
			return smart_ptr<OOX::File>(new CHdrFtr( oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::Footer)
			return smart_ptr<OOX::File>(new CHdrFtr( oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::Comments)
			return smart_ptr<OOX::File>(new CComments( oFileName ));
		else if ( pRelation->Type() == FileTypes::CommentsExt )
			return smart_ptr<OOX::File>(new CCommentsExt( oFileName ));
		else if ( pRelation->Type() == FileTypes::People )
			return smart_ptr<OOX::File>(new CPeople( oFileName ));
		else if (pRelation->Type() == FileTypes::Data)
			return smart_ptr<OOX::File>(new CDiagramData( oRootPath, oFileName ));
		else if (pRelation->Type() == FileTypes::DiagDrawing)
			return smart_ptr<OOX::File>(new CDiagramDrawing( oRootPath, oFileName )); 
		else if (pRelation->Type() == FileTypes::MicrosoftOfficeUnknown) //ms package
			return smart_ptr<OOX::File>(new OleObject( oFileName, true ));
		else if ( pRelation->Type() == OOX::FileTypes::Chart )
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartSpace( oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::ActiveX_xml)
			return smart_ptr<OOX::File>(new ActiveX_xml( oRootPath, oFileName ));
		else if ( pRelation->Type() == FileTypes::ActiveX_bin)
			return smart_ptr<OOX::File>(new ActiveX_bin( oFileName ));
		else if ( pRelation->Type() == FileTypes::VbaProject)
			return smart_ptr<OOX::File>(new OOX::VbaProject( oRootPath, oFileName ));
		//else if ( pRelation->Type() == FileTypes::VbaData)
		//	return smart_ptr<OOX::File>(new OOX::VbaData( oFileName ));

		return smart_ptr<OOX::File>( new UnknowTypeFile() );
	}

} // namespace OOX
