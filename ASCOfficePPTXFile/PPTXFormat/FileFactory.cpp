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
#include "../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramData.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramDrawing.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/Image.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/Audio.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/Video.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/OleObject.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/ActiveX.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/VbaProject.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/HyperLink.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/ExternalImage.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/ExternalAudio.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/ExternalVideo.h"
#include "../../Common/DocxFormat/Source/DocxFormat/UnknowTypeFile.h"
//


namespace PPTX
{
	const smart_ptr<OOX::File> FileFactory::CreateFilePPTX(const OOX::CPath& filename, OOX::Rels::CRelationShip& relation, FileMap& map)
	{
		if (NSFile::CFileBinary::Exists(filename.GetPath()) == false)
		{
			return smart_ptr<OOX::File>(NULL);
		}
		
		if (relation.Type() == OOX::FileTypes::App)
			return smart_ptr<OOX::File>(new PPTX::App(filename, map));
		else if (relation.Type() == OOX::FileTypes::Core)
			return smart_ptr<OOX::File>(new PPTX::Core(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::Presentation)
			return smart_ptr<OOX::File>(new PPTX::Presentation(filename, map));
		else if (relation.Type() == OOX::FileTypes::Theme)
			return smart_ptr<OOX::File>(new PPTX::Theme(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::SlideMaster)
			return smart_ptr<OOX::File>(new PPTX::SlideMaster(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::SlideLayout)
			return smart_ptr<OOX::File>(new PPTX::SlideLayout(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::Slide)
			return smart_ptr<OOX::File>(new PPTX::Slide(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::HandoutMaster)
			return smart_ptr<OOX::File>(new PPTX::HandoutMaster(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::NotesMaster)
			return smart_ptr<OOX::File>(new PPTX::NotesMaster(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::NotesSlide)
			return smart_ptr<OOX::File>(new PPTX::NotesSlide(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::PresProps)
			return smart_ptr<OOX::File>(new PPTX::PresProps(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::ViewProps)
			return smart_ptr<OOX::File>(new PPTX::ViewProps(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::TableStyles)
			return smart_ptr<OOX::File>(new PPTX::TableStyles(filename, map));
		else if (relation.Type() == OOX::FileTypes::LegacyDiagramText)
			return smart_ptr<OOX::File>(new PPTX::LegacyDiagramText(filename));
		else if (relation.Type() == OOX::FileTypes::VmlDrawing)
			return smart_ptr<OOX::File>(new OOX::CVmlDrawing(OOX::CPath(), filename));
		else if (relation.Type() == OOX::Presentation::FileTypes::CommentAuthors)
			return smart_ptr<OOX::File>(new PPTX::Authors(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::SlideComments)
			return smart_ptr<OOX::File>(new PPTX::Comments(filename, map));

		else if (relation.Type() == OOX::FileTypes::Chart)
			return smart_ptr<OOX::File>(new OOX::Spreadsheet::CChartSpace(filename, filename));
		else if (relation.Type() == OOX::FileTypes::HyperLink)
			return smart_ptr<OOX::File>(new OOX::HyperLink(relation.Target()));
		else if ((relation.Type() == OOX::FileTypes::ExternalImage) && (relation.IsExternal()))
			return smart_ptr<OOX::File>(new OOX::ExternalImage(relation.Target()));
		else if ((relation.Type() == OOX::FileTypes::ExternalAudio) && (relation.IsExternal()))
			return smart_ptr<OOX::File>(new OOX::ExternalAudio(relation.Target()));
		else if ((relation.Type() == OOX::FileTypes::ExternalVideo) && (relation.IsExternal()))
			return smart_ptr<OOX::File>(new OOX::ExternalVideo(relation.Target()));
		else if (relation.Type() == OOX::FileTypes::Image)
			return smart_ptr<OOX::File>(new OOX::Image(filename));
		else if (relation.Type() == OOX::FileTypes::Audio)
			return smart_ptr<OOX::File>(new OOX::Audio(filename));
		else if (relation.Type() == OOX::FileTypes::Media)
			return smart_ptr<OOX::File>(new OOX::Media(filename));
		else if (relation.Type() == OOX::FileTypes::Video)
			return smart_ptr<OOX::File>(new OOX::Video(filename));
		else if (relation.Type() == OOX::FileTypes::Data) 
			return smart_ptr<OOX::File>(new OOX::CDiagramData(filename));
		else if (relation.Type() == OOX::FileTypes::DiagDrawing)	
			return smart_ptr<OOX::File>(new OOX::CDiagramDrawing(filename)); 
		else if (relation.Type() == OOX::FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OOX::OleObject(filename));
		else if (relation.Type() == OOX::FileTypes::MicrosoftOfficeUnknown) //ms package
			return smart_ptr<OOX::File>(new OOX::OleObject( filename, true ));

		return smart_ptr<OOX::File>(new OOX::UnknowTypeFile());
	}

} // namespace PPTX
