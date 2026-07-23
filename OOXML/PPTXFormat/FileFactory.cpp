/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "FileFactory.h"
#include "FileMap.h"
#include "FileTypes.h"

#include "Theme.h"
#include "HandoutMaster.h"
#include "Presentation.h"
#include "PresProps.h"
#include "SlideLayout.h"
#include "SlideMaster.h"
#include "Slide.h"
#include "CommentAuthors.h"
#include "Comments.h"
#include "TableStyles.h"
#include "ViewProps.h"
#include "NotesSlide.h"
#include "NotesMaster.h"
#include "LegacyDiagramText.h"
#include "./Logic/HeadingVariant.h"

#include "../DocxFormat/Rels.h"
#include "../DocxFormat/FileTypes.h"
#include "../XlsxFormat/Chart/Chart.h"
#include "../XlsxFormat/Chart/ChartDrawing.h"
#include "../DocxFormat/VmlDrawing.h"
#include "../DocxFormat/Diagram/DiagramData.h"
#include "../DocxFormat/Diagram/DiagramDrawing.h"
#include "../DocxFormat/Diagram/DiagramColors.h"
#include "../DocxFormat/Diagram/DiagramQuickStyle.h"
#include "../DocxFormat/Diagram/DiagramLayout.h"
#include "../DocxFormat/Media/Image.h"
#include "../DocxFormat/Media/Audio.h"
#include "../DocxFormat/Media/Video.h"
#include "../DocxFormat/Media/OleObject.h"
#include "../DocxFormat/Media/ActiveX.h"
#include "../DocxFormat/Media/VbaProject.h"
#include "../DocxFormat/Media/JsaProject.h"
#include "../DocxFormat/External/HyperLink.h"
#include "../DocxFormat/UnknowTypeFile.h"
#include "../DocxFormat/CustomXml.h"
#include "../DocxFormat/App.h"
#include "../DocxFormat/Core.h"

namespace PPTX
{
	const smart_ptr<OOX::File> FileFactory::CreateFilePPTX(const OOX::CPath& filename, OOX::Rels::CRelationShip& relation, FileMap& map, OOX::Document *pMain)
	{
		if (NSFile::CFileBinary::Exists(filename.GetPath()) == false && !relation.IsExternal())
		{
			return smart_ptr<OOX::File>(NULL);
		}
		
		if (relation.Type() == OOX::FileTypes::App)
			return smart_ptr<OOX::File>(new OOX::CApp(pMain, filename));
		else if (relation.Type() == OOX::FileTypes::Core)
			return smart_ptr<OOX::File>(new OOX::CCore(pMain, filename));
		else if (relation.Type() == OOX::FileTypes::CustomProperties)
			return smart_ptr<OOX::File>(new PPTX::CustomProperties(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::Presentation || 
				 relation.Type() == OOX::Presentation::FileTypes::PresentationMacro)
			return smart_ptr<OOX::File>(new PPTX::Presentation(pMain, filename, map));
		else if (relation.Type() == OOX::FileTypes::Theme)
			return smart_ptr<OOX::File>(new PPTX::Theme(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::SlideMaster)
			return smart_ptr<OOX::File>(new PPTX::SlideMaster(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::SlideLayout)
			return smart_ptr<OOX::File>(new PPTX::SlideLayout(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::Slide)
			return smart_ptr<OOX::File>(new PPTX::Slide(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::HandoutMaster)
			return smart_ptr<OOX::File>(new PPTX::HandoutMaster(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::NotesMaster)
			return smart_ptr<OOX::File>(new PPTX::NotesMaster(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::NotesSlide)
			return smart_ptr<OOX::File>(new PPTX::NotesSlide(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::PresProps)
			return smart_ptr<OOX::File>(new PPTX::PresProps(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::ViewProps)
			return smart_ptr<OOX::File>(new PPTX::ViewProps(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::TableStyles)
			return smart_ptr<OOX::File>(new PPTX::TableStyles(pMain, filename, map));
		else if (relation.Type() == OOX::FileTypes::LegacyDiagramText)
			return smart_ptr<OOX::File>(new PPTX::LegacyDiagramText(pMain, filename));
		else if (relation.Type() == OOX::FileTypes::VmlDrawing)
			return smart_ptr<OOX::File>(new OOX::CVmlDrawing(pMain, OOX::CPath(), filename));
		else if (relation.Type() == OOX::FileTypes::ChartDrawing)
			return smart_ptr<OOX::File>(new OOX::CChartDrawing(pMain, OOX::CPath(), filename));
		else if (relation.Type() == OOX::Presentation::FileTypes::CommentAuthors)
			return smart_ptr<OOX::File>(new PPTX::Authors(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::ModernCommentAuthors)
			return smart_ptr<OOX::File>(new PPTX::Authors(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::SlideComments)
			return smart_ptr<OOX::File>(new PPTX::Comments(pMain, filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::ModernComments)
			return smart_ptr<OOX::File>(new PPTX::Comments(pMain, filename, map));
		else if (relation.Type() == OOX::FileTypes::Chart)
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartFile(pMain, filename, filename));
		else if (relation.Type() == OOX::FileTypes::ChartEx)
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartExFile(pMain, filename, filename));
		else if (relation.Type() == OOX::FileTypes::ChartStyle)
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartStyleFile(pMain, filename, filename));
		else if (relation.Type() == OOX::FileTypes::ChartColors)
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartColorsFile(pMain, filename, filename));
		else if (relation.Type() == OOX::FileTypes::HyperLink)
			return smart_ptr<OOX::File>(new OOX::HyperLink(pMain, relation.Target()));
		else if (relation.Type() == OOX::FileTypes::Image)
			return smart_ptr<OOX::File>(new OOX::Image(pMain, filename, relation.IsExternal()));
		else if (relation.Type() == OOX::FileTypes::Audio)
			return smart_ptr<OOX::File>(new OOX::Audio(pMain, filename, relation.IsExternal()));
		else if (relation.Type() == OOX::FileTypes::Media)
			return smart_ptr<OOX::File>(new OOX::Media(pMain, filename, relation.IsExternal()));
		else if (relation.Type() == OOX::FileTypes::Video)
			return smart_ptr<OOX::File>(new OOX::Video(pMain, filename, relation.IsExternal()));
		else if (relation.Type() == OOX::FileTypes::DiagramData) 
			return smart_ptr<OOX::File>(new OOX::CDiagramData(pMain, filename));
		else if (relation.Type() == OOX::FileTypes::DiagramDrawing)	
			return smart_ptr<OOX::File>(new OOX::CDiagramDrawing(pMain, filename)); 
		else if (relation.Type() == OOX::FileTypes::DiagramColors)
			return smart_ptr<OOX::File>(new OOX::CDiagramColors(pMain, filename));
		else if (relation.Type() == OOX::FileTypes::DiagramLayout)
			return smart_ptr<OOX::File>(new OOX::CDiagramLayout(pMain, filename));
		else if (relation.Type() == OOX::FileTypes::DiagramQuickStyle)
			return smart_ptr<OOX::File>(new OOX::CDiagramQuickStyle(pMain, filename));
		else if (relation.Type() == OOX::FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OOX::OleObject(pMain, filename));
		else if (relation.Type() == OOX::FileTypes::MicrosoftOfficeUnknown) //ms package
			return smart_ptr<OOX::File>(new OOX::OleObject( pMain, filename, true ));
		else if (relation.Type() == OOX::FileTypes::VbaProject)
			return smart_ptr<OOX::File>(new OOX::VbaProject( pMain, filename, filename ));
		else if (relation.Type() == OOX::FileTypes::JsaProject)
			return smart_ptr<OOX::File>(new OOX::JsaProject( pMain, filename, relation.IsExternal()));
		else if (relation.Type() == OOX::FileTypes::CustomXml)
			return smart_ptr<OOX::File>(new OOX::CCustomXML(pMain, filename, filename));
		else if (relation.Type() == OOX::FileTypes::ActiveX_xml)
			return smart_ptr<OOX::File>(new OOX::ActiveX_xml(pMain, filename, filename));
		else if (relation.Type() == OOX::FileTypes::ActiveX_bin)
			return smart_ptr<OOX::File>(new OOX::ActiveX_bin(pMain, filename));

		return smart_ptr<OOX::File>(new OOX::UnknowTypeFile(pMain));
	}

} // namespace PPTX
