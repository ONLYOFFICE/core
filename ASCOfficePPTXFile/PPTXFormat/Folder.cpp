//#include "./stdafx.h"

#include "Folder.h"
#include "DocxFormat/Rels/File.h"
#include "FileMap.h"
#include "DocxFormat/FileTypes.h"
#include "Presentation.h"
#include "Theme.h"
#include "SlideMaster.h"
#include "SlideLayout.h"
#include "Slide.h"
#include "NotesMaster.h"

#include <map>

namespace PPTX
{
	Folder::Folder()
	{		
	}

	Folder::Folder(const OOX::CPath& path, IPPTXEvent* Event)
	{
		read(path, Event);
	}

	void Folder::read(const OOX::CPath& path, IPPTXEvent* Event)
	{
		PPTX::Rels::File rels(path);
		PPTX::FileMap map;
		long files = CountFiles(path);
		if(files == 0)
			return;
		m_lPercent = 1000000 / files;
		FileContainer::read(rels, path, map, Event);
		if(m_bCancelled)
			return;

		POSITION pos = NULL;
		smart_ptr<PPTX::Presentation> _presentation = FileContainer::get(PPTX::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
		if (_presentation.is_init())
		{
			_presentation->commentAuthors = _presentation->get(PPTX::FileTypes::CommentAuthors).smart_dynamic_cast<PPTX::Authors>();
		}

        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const PPTX::FileType& curType = pPair->second->type();

            if (PPTX::FileTypes::ThemePPTX == curType)
            {
                smart_ptr<PPTX::Theme> pTheme = pPair->second.smart_dynamic_cast<PPTX::Theme>();
                if (pTheme.IsInit())
                    pTheme->Presentation = _presentation;
            }
        }

        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const PPTX::FileType& curType = pPair->second->type();

            if (PPTX::FileTypes::SlideMaster == curType)
            {
                smart_ptr<PPTX::SlideMaster> pointer = pPair->second.smart_dynamic_cast<PPTX::SlideMaster>();
                if (pointer.is_init())
                    pointer->ApplyRels();
            }

        }


        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const PPTX::FileType& curType = pPair->second->type();

            if (PPTX::FileTypes::SlideLayout == curType)
            {
                smart_ptr<PPTX::SlideLayout> pointer = pPair->second.smart_dynamic_cast<PPTX::SlideLayout>();
                if (pointer.is_init())
                    pointer->ApplyRels();
            }

        }

        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const PPTX::FileType& curType = pPair->second->type();

            if (PPTX::FileTypes::Slide == curType)
            {
                smart_ptr<PPTX::Slide> pointer = pPair->second.smart_dynamic_cast<PPTX::Slide>();
                if (pointer.is_init())
                    pointer->ApplyRels();
            }
        }


        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const PPTX::FileType& curType = pPair->second->type();
            if (PPTX::FileTypes::NotesMaster == curType)
            {
                smart_ptr<PPTX::NotesMaster> pointer = pPair->second.smart_dynamic_cast<PPTX::NotesMaster>();
                if (pointer.is_init())
                    pointer->ApplyRels();
            }
        }

		Event->Progress(0, 1000000);
	}

	void Folder::write(const OOX::CPath& path)
	{
		OOX::CSystemUtility::CreateDirectories(path);

		PPTX::Rels::File rels;
		PPTX::ContentTypes::File content;

		OOX::CPath dir = path;
		FileContainer::write(rels, path, dir, content);

		rels.write(path / _T("/"));
		content.write(path);
		FileContainer::WrittenSetFalse();
	}

	void Folder::createFromTemplate(const OOX::CPath& path)
	{
		//read(path);
	}

	const bool Folder::isValid(const OOX::CPath& path) const
	{
		return true;//FileContainer::exist(PPTX::FileTypes::Presentation);
	}

	void Folder::extractPictures(const OOX::CPath& path)
	{
		OOX::CSystemUtility::CreateDirectories(path);
		FileContainer::extractPictures(path);
	}

	void Folder::extractPictures(const OOX::CPath& source, const OOX::CPath& path)
	{
		//read(source);
		extractPictures(path);
	}

	long Folder::CountFiles(const OOX::CPath& path)
	{
		return OOX::CSystemUtility::GetFilesCount(path.GetDirectory(), true);
	}
} // namespace PPTX
