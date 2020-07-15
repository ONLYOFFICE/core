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
#include "FileMap.h"
#include "FileTypes.h"

#include "App.h"
#include "Core.h"
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

#include "../../Common/DocxFormat/Source/DocxFormat/Rels.h"
#include "../../Common/DocxFormat/Source/DocxFormat/FileTypes.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Chart/Chart.h"
#include "../../Common/DocxFormat/Source/DocxFormat/VmlDrawing.h"
#include "../../Common/DocxFormat/Source/DocxFormat/ChartDrawing.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramData.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramDrawing.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/Image.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/Audio.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/Video.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/OleObject.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/ActiveX.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/VbaProject.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/JsaProject.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/HyperLink.h"
#include "../../Common/DocxFormat/Source/DocxFormat/UnknowTypeFile.h"
//


namespace PPTX
{
	const smart_ptr<OOX::File> FileFactory::CreateFilePPTX(const OOX::CPath& filename, OOX::Rels::CRelationShip& relation, FileMap& map, OOX::Document *pMain)
	{
		if (NSFile::CFileBinary::Exists(filename.GetPath()) == false && !relation.IsExternal())
		{
			return smart_ptr<OOX::File>(NULL);
		}
		
		if (relation.Type() == OOX::FileTypes::App)
			return smart_ptr<OOX::File>(new PPTX::App(pMain, filename, map));
		else if (relation.Type() == OOX::FileTypes::Core)
			return smart_ptr<OOX::File>(new PPTX::Core(pMain, filename, map));
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
		else if (relation.Type() == OOX::Presentation::FileTypes::SlideComments)
			return smart_ptr<OOX::File>(new PPTX::Comments(pMain, filename, map));

		else if (relation.Type() == OOX::FileTypes::Chart)
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartSpace(pMain, filename, filename));
		else if (relation.Type() == OOX::FileTypes::ChartEx)
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartSpaceEx(pMain, filename, filename));
		else if (relation.Type() == OOX::FileTypes::ChartStyle)
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartStyle(pMain, filename, filename));
		else if (relation.Type() == OOX::FileTypes::ChartColorStyle)
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartColorStyle(pMain, filename, filename));
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
		else if (relation.Type() == OOX::FileTypes::Data) 
			return smart_ptr<OOX::File>(new OOX::CDiagramData(pMain, filename));
		else if (relation.Type() == OOX::FileTypes::DiagDrawing)	
			return smart_ptr<OOX::File>(new OOX::CDiagramDrawing(pMain, filename)); 
		else if (relation.Type() == OOX::FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OOX::OleObject(pMain, filename));
		else if (relation.Type() == OOX::FileTypes::MicrosoftOfficeUnknown) //ms package
			return smart_ptr<OOX::File>(new OOX::OleObject( pMain, filename, true ));
		else if (relation.Type() == OOX::FileTypes::VbaProject)
			return smart_ptr<OOX::File>(new OOX::VbaProject( pMain, filename, filename ));
		else if (relation.Type() == OOX::FileTypes::JsaProject)
			return smart_ptr<OOX::File>(new OOX::JsaProject( pMain, filename ));

		return smart_ptr<OOX::File>(new OOX::UnknowTypeFile(pMain));
	}

} // namespace PPTX
