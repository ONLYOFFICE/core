#include "./stdafx.h"

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
#include "DocxFormat/External/HyperLink.h"
#include "DocxFormat/External/ExternalImage.h"
#include "DocxFormat/External/ExternalAudio.h"
#include "DocxFormat/External/ExternalVideo.h"
#include "VmlDrawing.h"
#include "DocxFormat/UnknowTypeFile.h"
#include "FileMap.h"

namespace PPTX
{
	const smart_ptr<OOX::File> FileFactory::CreateFilePPTX(const OOX::CPath& path, const OOX::Rels::RelationShip& relation, FileMap& map)
	{
		OOX::CPath filename = path / relation.filename();
		
		if (relation.type() == OOX::FileTypes::App)
			return smart_ptr<OOX::File>(new PPTX::App(filename, map));
		else if (relation.type() == OOX::FileTypes::Core)
			return smart_ptr<OOX::File>(new PPTX::Core(filename, map));
		else if (relation.type() == OOX::FileTypes::Presentation)
			return smart_ptr<OOX::File>(new PPTX::Presentation(filename, map));
		else if (relation.type() == OOX::FileTypes::Theme)
			return smart_ptr<OOX::File>(new PPTX::Theme(filename, map));
		else if (relation.type() == OOX::FileTypes::SlideMaster)
			return smart_ptr<OOX::File>(new PPTX::SlideMaster(filename, map));
		else if (relation.type() == OOX::FileTypes::SlideLayout)
			return smart_ptr<OOX::File>(new PPTX::SlideLayout(filename, map));
		else if (relation.type() == OOX::FileTypes::Slide)
			return smart_ptr<OOX::File>(new PPTX::Slide(filename, map));
		else if (relation.type() == OOX::FileTypes::HandoutMaster)
			return smart_ptr<OOX::File>(new PPTX::HandoutMaster(filename, map));
		else if (relation.type() == OOX::FileTypes::NotesMaster)
			return smart_ptr<OOX::File>(new PPTX::NotesMaster(filename, map));
		else if (relation.type() == OOX::FileTypes::NotesSlide)
			return smart_ptr<OOX::File>(new PPTX::NotesSlide(filename, map));
		else if (relation.type() == OOX::FileTypes::PresProps)
			return smart_ptr<OOX::File>(new PPTX::PresProps(filename, map));
		else if (relation.type() == OOX::FileTypes::ViewProps)
			return smart_ptr<OOX::File>(new PPTX::ViewProps(filename, map));
		else if (relation.type() == OOX::FileTypes::TableStyles)
			return smart_ptr<OOX::File>(new PPTX::TableStyles(filename, map));
		else if (relation.type() == OOX::FileTypes::VmlDrawing)
			return smart_ptr<OOX::File>(new PPTX::VmlDrawing(filename, map));
		else if (relation.type() == OOX::FileTypes::HyperLink)
			return smart_ptr<OOX::File>(new OOX::HyperLink(relation.target()));
		else if ((relation.type() == OOX::FileTypes::ExternalImage) && (relation.isExternal()))
			return smart_ptr<OOX::File>(new OOX::ExternalImage(relation.target()));
		else if ((relation.type() == OOX::FileTypes::ExternalAudio) && (relation.isExternal()))
			return smart_ptr<OOX::File>(new OOX::ExternalAudio(relation.target()));
		else if ((relation.type() == OOX::FileTypes::ExternalVideo) && (relation.isExternal()))
			return smart_ptr<OOX::File>(new OOX::ExternalVideo(relation.target()));
		else if (relation.type() == OOX::FileTypes::Image)
			return smart_ptr<OOX::File>(new OOX::Image(filename));
		else if (relation.type() == OOX::FileTypes::Audio)
			return smart_ptr<OOX::File>(new OOX::Audio(filename));
		else if (relation.type() == OOX::FileTypes::Video)
			return smart_ptr<OOX::File>(new OOX::Video(filename));
		else if (relation.type() == OOX::FileTypes::Media)				// FOR NONE OPTIMIZED PPTX FILES
			return smart_ptr<OOX::File>(new OOX::HyperLink(filename));
		else if (relation.type() == OOX::FileTypes::Data) // нужен только filepath
			return smart_ptr<OOX::File>(new OOX::Image(filename));
		else if (relation.type() == OOX::FileTypes::DrawingDiag)
			return smart_ptr<OOX::File>(new OOX::Image(filename)); // нужен только filepath
		else if (relation.type() == OOX::FileTypes::Chart)
			return smart_ptr<OOX::File>(new OOX::Image(filename)); // нужен только filepath
		else if (relation.type() == OOX::FileTypes::CommentAuthors)
			return smart_ptr<OOX::File>(new PPTX::Authors(filename, map));
		else if (relation.type() == OOX::FileTypes::SlideComments)
			return smart_ptr<OOX::File>(new PPTX::Comments(filename, map));

		return smart_ptr<OOX::File>(new OOX::UnknowTypeFile());
	}

	const smart_ptr<OOX::File> FileFactory::CreateFilePPTX_OnlyMedia(const OOX::CPath& path, const OOX::Rels::RelationShip& relation)
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

		if (strT == OOX::FileTypes::Image ||
			strT == OOX::FileTypes::Data ||
			strT == OOX::FileTypes::DrawingDiag ||
			strT == OOX::FileTypes::Chart)
		{
			return smart_ptr<OOX::File>(new OOX::Image(filename));
		}
		
		return smart_ptr<OOX::File>(new OOX::UnknowTypeFile());
	}

} // namespace PPTX