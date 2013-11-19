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
	const boost::shared_ptr<OOX::File> FileFactory::CreateFilePPTX(const boost::filesystem::wpath& path, const OOX::Rels::RelationShip& relation, FileMap& map)
	{
		boost::filesystem::wpath filename = path / relation.filename();
		filename.normalize();

		if (relation.type() == OOX::FileTypes::App)
			return boost::shared_ptr<OOX::File>(new PPTX::App(filename, map));
		else if (relation.type() == OOX::FileTypes::Core)
			return boost::shared_ptr<OOX::File>(new PPTX::Core(filename, map));
		else if (relation.type() == OOX::FileTypes::Presentation)
			return boost::shared_ptr<OOX::File>(new PPTX::Presentation(filename, map));
		else if (relation.type() == OOX::FileTypes::Theme)
			return boost::shared_ptr<OOX::File>(new PPTX::Theme(filename, map));
		else if (relation.type() == OOX::FileTypes::SlideMaster)
			return boost::shared_ptr<OOX::File>(new PPTX::SlideMaster(filename, map));
		else if (relation.type() == OOX::FileTypes::SlideLayout)
			return boost::shared_ptr<OOX::File>(new PPTX::SlideLayout(filename, map));
		else if (relation.type() == OOX::FileTypes::Slide)
			return boost::shared_ptr<OOX::File>(new PPTX::Slide(filename, map));
		else if (relation.type() == OOX::FileTypes::HandoutMaster)
			return boost::shared_ptr<OOX::File>(new PPTX::HandoutMaster(filename, map));
		else if (relation.type() == OOX::FileTypes::NotesMaster)
			return boost::shared_ptr<OOX::File>(new PPTX::NotesMaster(filename, map));
		else if (relation.type() == OOX::FileTypes::NotesSlide)
			return boost::shared_ptr<OOX::File>(new PPTX::NotesSlide(filename, map));
		else if (relation.type() == OOX::FileTypes::PresProps)
			return boost::shared_ptr<OOX::File>(new PPTX::PresProps(filename, map));
		else if (relation.type() == OOX::FileTypes::ViewProps)
			return boost::shared_ptr<OOX::File>(new PPTX::ViewProps(filename, map));
		else if (relation.type() == OOX::FileTypes::TableStyles)
			return boost::shared_ptr<OOX::File>(new PPTX::TableStyles(filename, map));
		else if (relation.type() == OOX::FileTypes::VmlDrawing)
			return boost::shared_ptr<OOX::File>(new PPTX::VmlDrawing(filename, map));
		else if (relation.type() == OOX::FileTypes::HyperLink)
			return boost::shared_ptr<OOX::File>(new OOX::HyperLink(relation.target()));
		else if ((relation.type() == OOX::FileTypes::ExternalImage) && (relation.isExternal()))
			return boost::shared_ptr<OOX::File>(new OOX::ExternalImage(relation.target()));
		else if ((relation.type() == OOX::FileTypes::ExternalAudio) && (relation.isExternal()))
			return boost::shared_ptr<OOX::File>(new OOX::ExternalAudio(relation.target()));
		else if ((relation.type() == OOX::FileTypes::ExternalVideo) && (relation.isExternal()))
			return boost::shared_ptr<OOX::File>(new OOX::ExternalVideo(relation.target()));
		else if (relation.type() == OOX::FileTypes::Image)
			return boost::shared_ptr<OOX::File>(new OOX::Image(filename));
		else if (relation.type() == OOX::FileTypes::Audio)
			return boost::shared_ptr<OOX::File>(new OOX::Audio(filename));
		else if (relation.type() == OOX::FileTypes::Video)
			return boost::shared_ptr<OOX::File>(new OOX::Video(filename));

		return boost::shared_ptr<OOX::File>(new OOX::UnknowTypeFile());
	}

} // namespace PPTX