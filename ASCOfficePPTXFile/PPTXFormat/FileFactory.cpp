//#include "./stdafx.h"

#include "FileFactory.h"
#include "DocxFormat/File.h"
#include "DocxFormat/FileTypes.h"
#include "DocxFormat/Rels/RelationShip.h"
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

#include "DocxFormat/Media/Image.h"
#include "DocxFormat/Media/Audio.h"
#include "DocxFormat/Media/Video.h"
#include "DocxFormat/Media/OleObject.h"
#include "DocxFormat/External/HyperLink.h"
#include "DocxFormat/External/ExternalImage.h"
#include "DocxFormat/External/ExternalAudio.h"
#include "DocxFormat/External/ExternalVideo.h"
#include "DocxFormat/Drawing/VmlDrawing.h"
#include "DocxFormat/Drawing/LegacyDiagramText.h"
#include "DocxFormat/UnknowTypeFile.h"

#include "FileMap.h"

namespace PPTX
{
	const smart_ptr<PPTX::File> FileFactory::CreateFilePPTX(const OOX::CPath& path, const PPTX::Rels::RelationShip& relation, FileMap& map)
	{
		OOX::CPath filename = path / relation.filename();
		
		if (relation.type() == PPTX::FileTypes::App)
			return smart_ptr<PPTX::File>(new PPTX::App(filename, map));
		else if (relation.type() == PPTX::FileTypes::Core)
			return smart_ptr<PPTX::File>(new PPTX::Core(filename, map));
		else if (relation.type() == PPTX::FileTypes::Presentation)
			return smart_ptr<PPTX::File>(new PPTX::Presentation(filename, map));
		else if (relation.type() == PPTX::FileTypes::Theme)
			return smart_ptr<PPTX::File>(new PPTX::Theme(filename, map));
		else if (relation.type() == PPTX::FileTypes::SlideMaster)
			return smart_ptr<PPTX::File>(new PPTX::SlideMaster(filename, map));
		else if (relation.type() == PPTX::FileTypes::SlideLayout)
			return smart_ptr<PPTX::File>(new PPTX::SlideLayout(filename, map));
		else if (relation.type() == PPTX::FileTypes::Slide)
			return smart_ptr<PPTX::File>(new PPTX::Slide(filename, map));
		else if (relation.type() == PPTX::FileTypes::HandoutMaster)
			return smart_ptr<PPTX::File>(new PPTX::HandoutMaster(filename, map));
		else if (relation.type() == PPTX::FileTypes::NotesMaster)
			return smart_ptr<PPTX::File>(new PPTX::NotesMaster(filename, map));
		else if (relation.type() == PPTX::FileTypes::NotesSlide)
			return smart_ptr<PPTX::File>(new PPTX::NotesSlide(filename, map));
		else if (relation.type() == PPTX::FileTypes::PresProps)
			return smart_ptr<PPTX::File>(new PPTX::PresProps(filename, map));
		else if (relation.type() == PPTX::FileTypes::ViewProps)
			return smart_ptr<PPTX::File>(new PPTX::ViewProps(filename, map));
		else if (relation.type() == PPTX::FileTypes::TableStyles)
			return smart_ptr<PPTX::File>(new PPTX::TableStyles(filename, map));
		else if (relation.type() == PPTX::FileTypes::LegacyDiagramText)
			return smart_ptr<PPTX::File>(new PPTX::LegacyDiagramText(filename));
		else if (relation.type() == PPTX::FileTypes::VmlDrawing)
			return smart_ptr<PPTX::File>(new PPTX::VmlDrawing(filename, map));
		else if (relation.type() == PPTX::FileTypes::HyperLink)
			return smart_ptr<PPTX::File>(new PPTX::HyperLink(relation.target()));
		else if ((relation.type() == PPTX::FileTypes::ExternalImage) && (relation.isExternal()))
			return smart_ptr<PPTX::File>(new PPTX::ExternalImage(relation.target()));
		else if ((relation.type() == PPTX::FileTypes::ExternalAudio) && (relation.isExternal()))
			return smart_ptr<PPTX::File>(new PPTX::ExternalAudio(relation.target()));
		else if ((relation.type() == PPTX::FileTypes::ExternalVideo) && (relation.isExternal()))
			return smart_ptr<PPTX::File>(new PPTX::ExternalVideo(relation.target()));
		else if (relation.type() == PPTX::FileTypes::Image)
			return smart_ptr<PPTX::File>(new PPTX::Image(filename));
		else if (relation.type() == PPTX::FileTypes::Audio)
			return smart_ptr<PPTX::File>(new PPTX::Audio(filename));
		else if (relation.type() == PPTX::FileTypes::Video)
			return smart_ptr<PPTX::File>(new PPTX::Video(filename));
		else if (relation.type() == PPTX::FileTypes::Media)				// FOR NONE OPTIMIZED PPTX FILES
			return smart_ptr<PPTX::File>(new PPTX::HyperLink(filename));
		else if (relation.type() == PPTX::FileTypes::Data) // нужен только filepath
			return smart_ptr<PPTX::File>(new PPTX::Image(filename));
		else if (relation.type() == PPTX::FileTypes::DrawingDiag)
			return smart_ptr<PPTX::File>(new PPTX::Image(filename)); // нужен только filepath
		else if (relation.type() == PPTX::FileTypes::Chart)
			return smart_ptr<PPTX::File>(new PPTX::Image(filename)); // нужен только filepath
		else if (relation.type() == PPTX::FileTypes::CommentAuthors)
			return smart_ptr<PPTX::File>(new PPTX::Authors(filename, map));
		else if (relation.type() == PPTX::FileTypes::SlideComments)
			return smart_ptr<PPTX::File>(new PPTX::Comments(filename, map));
		else if (relation.type() == PPTX::FileTypes::OleObject)
			return smart_ptr<PPTX::File>(new PPTX::OleObject(filename));

		return smart_ptr<PPTX::File>(new PPTX::UnknowTypeFile());
	}

	const smart_ptr<PPTX::File> FileFactory::CreateFilePPTX_OnlyMedia(const OOX::CPath& path, const PPTX::Rels::RelationShip& relation)
	{
		bool bIsDownload = false;
		CString strFile = relation.filename().GetPath();
		int n1 = strFile.Find(_T("www"));
		int n2 = strFile.Find(_T("http"));
		int n3 = strFile.Find(_T("ftp"));
		int n4 = strFile.Find(_T("https://"));

		if (((n1 >= 0) && (n1 < 10)) || ((n2 >= 0) && (n2 < 10)) || ((n3 >= 0) && (n3 < 10)) || ((n4 >= 0) && (n4 < 10)))
			bIsDownload = true;
		
		OOX::CPath filename = path / relation.filename();

		if (bIsDownload)
			filename = relation.filename();

		CString strT = relation.type();

		if (strT == PPTX::FileTypes::Image ||
			strT == PPTX::FileTypes::Data ||
			strT == PPTX::FileTypes::DrawingDiag ||
			strT == PPTX::FileTypes::Chart)
		{
			return smart_ptr<PPTX::File>(new PPTX::Image(filename));
		}
		else if(strT == PPTX::FileTypes::HyperLink)
			return smart_ptr<PPTX::File>(new PPTX::HyperLink(relation.target()));
		
		else if(strT == PPTX::FileTypes::LegacyDiagramText)
			return smart_ptr<PPTX::File>(new PPTX::LegacyDiagramText(filename));

		else if(strT == PPTX::FileTypes::OleObject)
			return smart_ptr<PPTX::File>(new PPTX::OleObject(filename));
		
		return smart_ptr<PPTX::File>(new PPTX::UnknowTypeFile());
	}

} // namespace PPTX