
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FileFactory.h"
#include "File.h"
#include "FileTypes.h"
#include "Rels/RelationShip.h"
#include "App.h"
#include "Core.h"
#include "Document.h"
#include "Theme/File.h"
#include "Settings/File.h"
#include "FontTable.h"
#include "Styles.h"
#include "Item.h"
#include "FootNote.h"
#include "EndNote.h"
#include "WebSettings.h"
#include "Media/Media.h"
#include "External\HyperLink.h"
#include "External\ExternalVideo.h"
#include "External\ExternalAudio.h"
#include "External\ExternalImage.h"
#include "Header.h"
#include "Footer.h"
#include "Numbering.h"
#include "UnknowTypeFile.h"

namespace OOX
{
	const NSCommon::smart_ptr<File> CreateFile(const OOX::CPath& path, const Rels::RelationShip& relation)
	{
		OOX::CPath filename = path / relation.filename();

		if (relation.type() == FileTypes::App)
			return NSCommon::smart_ptr<File>(new App(filename));
		else if (relation.type() == FileTypes::Core)
			return NSCommon::smart_ptr<File>(new Core(filename));
		else if (relation.type() == FileTypes::Document)
			return NSCommon::smart_ptr<File>(new Document(filename));
		else if (relation.type() == FileTypes::Theme)
			return NSCommon::smart_ptr<File>(new Theme::File(filename));
		else if (relation.type() == FileTypes::Setting)
			return NSCommon::smart_ptr<File>(new Settings::File(filename));
		else if (relation.type() == FileTypes::FontTable)
			return NSCommon::smart_ptr<File>(new FontTable(filename));
		else if (relation.type() == FileTypes::Style)
			return NSCommon::smart_ptr<File>(new Styles(filename));
		//else if (relation.type() == FileTypes::Item)				//	 TODO :  падение парсера - исправить
		//	return NSCommon::smart_ptr<File>(new Item(filename));
		else if (relation.type() == FileTypes::FootNote)
			return NSCommon::smart_ptr<File>(new FootNote(filename));
		else if (relation.type() == FileTypes::EndNote)
			return NSCommon::smart_ptr<File>(new EndNote(filename));
		else if (relation.type() == FileTypes::WebSetting)
			return NSCommon::smart_ptr<File>(new WebSettings(filename));
		else if (relation.type() == FileTypes::HyperLink)
			return NSCommon::smart_ptr<File>(new HyperLink(relation.target()));
		else if ((relation.type() == FileTypes::ExternalVideo) && (relation.isExternal()))
			return NSCommon::smart_ptr<File>(new ExternalVideo(relation.target()));
		else if ((relation.type() == FileTypes::ExternalAudio) && (relation.isExternal()))
			return NSCommon::smart_ptr<File>(new ExternalAudio(relation.target()));
		else if ((relation.type() == FileTypes::ExternalImage) && (relation.isExternal()))
			return NSCommon::smart_ptr<File>(new ExternalImage(relation.target()));
		else if (relation.type() == FileTypes::Image)
			return NSCommon::smart_ptr<File>(new Image(filename));
		else if (relation.type() == FileTypes::OleObject)
			return NSCommon::smart_ptr<File>(new OleObject(filename));
		else if (relation.type() == FileTypes::Audio)
			return NSCommon::smart_ptr<File>(new Audio(filename));
		else if (relation.type() == FileTypes::Video)
			return NSCommon::smart_ptr<File>(new Video(filename));
		else if (relation.type() == FileTypes::Numbering)
			return NSCommon::smart_ptr<File>(new Numbering(filename));
		else if (relation.type() == FileTypes::Header)
			return NSCommon::smart_ptr<File>(new Header(filename));
		else if (relation.type() == FileTypes::Footer)
			return NSCommon::smart_ptr<File>(new Footer(filename));

		return NSCommon::smart_ptr<File>(new UnknowTypeFile());
	}

} // namespace OOX