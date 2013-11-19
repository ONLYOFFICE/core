#include "Folder.h"
#include "DocxFormat/Rels/File.h"
#include "FileMap.h"
#include "DocxFormat/FileTypes.h"
#include "Presentation.h"
#include "Theme.h"
#include "SlideMaster.h"
#include "SlideLayout.h"
#include "Slide.h"
#include <boost/filesystem/operations.hpp>

namespace PPTX
{
	Folder::Folder()
	{		
	}

	Folder::Folder(const boost::filesystem::wpath& path, IPPTXEvent* Event)
	{
		read(path, Event);
	}

	void Folder::read(const boost::filesystem::wpath& path, IPPTXEvent* Event)
	{
		OOX::Rels::File rels(path / L"/");
		PPTX::FileMap map;
		long files = CountFiles(path);
		if(files == 0)
			return;
		m_lPercent = 1000000 / files;
		FileContainer::read(rels, path, map, Event);
		if(m_bCancelled)
			return;

		for(FileMap::iterator i = map.begin(); i != map.end(); i++)
		{
			if(i->second->type() == OOX::FileTypes::ThemePPTX)
				boost::shared_dynamic_cast<PPTX::Theme, OOX::File>(i->second)->Presentation =
					boost::shared_dynamic_cast<PPTX::Presentation, OOX::File>(FileContainer::get(OOX::FileTypes::Presentation));
		}

		for(FileMap::iterator i = map.begin(); i != map.end(); i++)
		{
			if(i->second->type() == OOX::FileTypes::SlideMaster)
				boost::shared_dynamic_cast<PPTX::SlideMaster, OOX::File>(i->second)->ApplyRels();
		}

		for(FileMap::iterator i = map.begin(); i != map.end(); i++)
		{
			if(i->second->type() == OOX::FileTypes::SlideLayout)
				boost::shared_dynamic_cast<PPTX::SlideLayout, OOX::File>(i->second)->ApplyRels();
		}

		for(FileMap::iterator i = map.begin(); i != map.end(); i++)
		{
			if(i->second->type() == OOX::FileTypes::Slide)
				boost::shared_dynamic_cast<PPTX::Slide, OOX::File>(i->second)->ApplyRels();
		}

		Event->Progress(0, 1000000);
	}

	void Folder::write(const boost::filesystem::wpath& path)
	{
		boost::filesystem::create_directories(path);

		OOX::Rels::File rels;
		OOX::ContentTypes::File content;

		FileContainer::write(rels, path, L"", content);

		rels.write(path / L"/");
		content.write(path);
		FileContainer::WrittenSetFalse();
	}

	void Folder::createFromTemplate(const boost::filesystem::wpath& path)
	{
//		read(path);
	}

	const bool Folder::isValid(const boost::filesystem::wpath& path) const
	{
		return true;//FileContainer::exist(OOX::FileTypes::Presentation);
	}

	void Folder::extractPictures(const boost::filesystem::wpath& path)
	{
		boost::filesystem::create_directories(path);
		FileContainer::extractPictures(path);
	}

	void Folder::extractPictures(const boost::filesystem::wpath& source, const boost::filesystem::wpath& path)
	{
//		read(source);
		extractPictures(path);
	}

	long Folder::CountFiles(const boost::filesystem::wpath& path)
	{
		if(!boost::filesystem::exists(path))
			return 0;
		long res = 0;
		boost::filesystem::wdirectory_iterator end_iter;
		for(boost::filesystem::wdirectory_iterator iter(path); iter != end_iter; iter++)
		{
			if(boost::filesystem::exists(*iter))
			{
				if(boost::filesystem::is_directory(*iter))
				{
					res += CountFiles(*iter);
				}
				else
				{
					res++;
				}
			}
		}
		return res;
//		return 1000;
	}
} // namespace PPTX