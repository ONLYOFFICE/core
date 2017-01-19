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


#include "../../Common/DocxFormat/Source/DocxFormat/File.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Rels.h"
#include "../../Common/DocxFormat/Source/DocxFormat/FileTypes.h"
#include "FileTypes.h"

#include "FileFactory.h"
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

#include "../../Common/DocxFormat/Source/DocxFormat/VmlDrawing.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramData.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/Image.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/Audio.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/Video.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/OleObject.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/HyperLink.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/ExternalImage.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/ExternalAudio.h"
#include "../../Common/DocxFormat/Source/DocxFormat/External/ExternalVideo.h"
#include "../../Common/DocxFormat/Source/DocxFormat/UnknowTypeFile.h"

#include "FileMap.h"

namespace PPTX
{
	const smart_ptr<OOX::File> FileFactory::CreateFilePPTX(const OOX::CPath& path, const OOX::Rels::CRelationShip& relation, FileMap& map)
	{
		OOX::CPath filename = path / relation.Filename();
		
		if (relation.Type() == OOX::Presentation::FileTypes::App)
			return smart_ptr<OOX::File>(new PPTX::App(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::Core)
			return smart_ptr<OOX::File>(new PPTX::Core(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::Presentation)
			return smart_ptr<OOX::File>(new PPTX::Presentation(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::Theme)
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
		else if (relation.Type() == OOX::Presentation::FileTypes::LegacyDiagramText)
			return smart_ptr<OOX::File>(new PPTX::LegacyDiagramText(filename));
		else if (relation.Type() == OOX::Presentation::FileTypes::VmlDrawing)
			return smart_ptr<OOX::File>(new OOX::CVmlDrawing(OOX::CPath(), filename));
		else if (relation.Type() == OOX::Presentation::FileTypes::Media)				// FOR NONE OPTIMIZED PPTX FILES
			return smart_ptr<OOX::File>(new OOX::HyperLink(filename));		
		else if (relation.Type() == OOX::FileTypes::Chart)
			return smart_ptr<OOX::File>(new OOX::Image(filename)); // нужен только filepath
		else if (relation.Type() == OOX::Presentation::FileTypes::CommentAuthors)
			return smart_ptr<OOX::File>(new PPTX::Authors(filename, map));
		else if (relation.Type() == OOX::Presentation::FileTypes::SlideComments)
			return smart_ptr<OOX::File>(new PPTX::Comments(filename, map));

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
		else if (relation.Type() == OOX::FileTypes::Video)
			return smart_ptr<OOX::File>(new OOX::Video(filename));
		else if (relation.Type() == OOX::FileTypes::Data) 
			return smart_ptr<OOX::File>(new OOX::CDiagramData(filename));
		else if (relation.Type() == OOX::FileTypes::DiagDrawing)	 // нужен только filepath	
			return smart_ptr<OOX::File>(new OOX::Image(filename)); // нужен только filepath
		else if (relation.Type() == OOX::FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OOX::OleObject(filename));

		return smart_ptr<OOX::File>(new OOX::UnknowTypeFile());
	}

	const smart_ptr<OOX::File> FileFactory::CreateFilePPTX_OnlyMedia(const OOX::CPath& path, const OOX::Rels::CRelationShip& relation)
	{
		bool bIsDownload = false;
		std::wstring strFile = relation.Filename().GetPath();
		int n1 = strFile.find(_T("www"));
		int n2 = strFile.find(_T("http"));
		int n3 = strFile.find(_T("ftp"));
		int n4 = strFile.find(_T("https://"));

        //если nI сранивать не с 0, то будут проблемы
        //потому что в инсталяции мы кладем файлы в /var/www...
        if (0 == n1 || 0 == n2 || 0 == n3 || 0 == n4)
			bIsDownload = true;
		
		OOX::CPath filename = path / relation.Filename();

		if (bIsDownload)
			filename = relation.Filename();

		std::wstring strT = relation.Type();

		if (strT == OOX::FileTypes::Image ||
			strT == OOX::FileTypes::DiagDrawing ||
			strT == OOX::FileTypes::Chart)
		{
			return smart_ptr<OOX::File>(new OOX::Image(filename));
		}
		else if(strT == OOX::FileTypes::Data)
			return smart_ptr<OOX::File>(new OOX::CDiagramData(relation.Target()));

		else if(strT == OOX::FileTypes::HyperLink)
			return smart_ptr<OOX::File>(new OOX::HyperLink(relation.Target()));
		
		else if(strT == OOX::FileTypes::LegacyDiagramText)
			return smart_ptr<OOX::File>(new PPTX::LegacyDiagramText(filename));

		else if(strT == OOX::FileTypes::OleObject)
			return smart_ptr<OOX::File>(new OOX::OleObject(filename));
		
		return smart_ptr<OOX::File>(new OOX::UnknowTypeFile());
	}

} // namespace PPTX
