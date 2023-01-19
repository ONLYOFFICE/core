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

#include "../DocxFormat/Rels.h"
#include "../DocxFormat/FileTypes.h"

#include "FileTypes.h"
#include "Folder.h"
#include "FileMap.h"
#include "Presentation.h"
#include "Theme.h"
#include "SlideMaster.h"
#include "SlideLayout.h"
#include "Slide.h"
#include "NotesMaster.h"

#include <map>

namespace PPTX
{
	Document::Document() : FileContainer(this)
	{		
	}

	Document::Document(const OOX::CPath& path) : FileContainer(this)
	{
		read(path);
	}

	bool Document::read(const OOX::CPath& path)
	{
		OOX::CRels rels(path);
		PPTX::FileMap map;
        long files = CountFiles(path);
		if(files == 0)
            return false;

		m_sDocumentPath = path.GetPath();

		FileContainer::read(rels, path, map);

		smart_ptr<PPTX::Presentation> _presentation = FileContainer::Get(OOX::Presentation::FileTypes::Presentation).smart_dynamic_cast<PPTX::Presentation>();
		if (_presentation.is_init())
		{
			_presentation->commentAuthors = _presentation->Get(OOX::Presentation::FileTypes::CommentAuthors).smart_dynamic_cast<PPTX::Authors>();
			
			if (_presentation->IsExist(OOX::FileTypes::VbaProject))
			{
				_presentation->m_bMacroEnabled	= true;
				_presentation->m_pVbaProject = _presentation->Get(OOX::FileTypes::VbaProject).smart_dynamic_cast<OOX::VbaProject>();
			}
			if (_presentation->IsExist(OOX::FileTypes::JsaProject))
			{
				_presentation->m_pJsaProject = _presentation->Get(OOX::FileTypes::JsaProject).smart_dynamic_cast<OOX::JsaProject>();
			}
			_presentation->comments = _presentation->Get(OOX::Presentation::FileTypes::SlideComments).smart_dynamic_cast<PPTX::Comments>();
		}

        for (std::map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const OOX::FileType& curType = pPair->second->type();

            if (OOX::FileTypes::Theme == curType)
            {
                smart_ptr<PPTX::Theme> pTheme = pPair->second.smart_dynamic_cast<PPTX::Theme>();
                if (pTheme.IsInit())
                    pTheme->presentation = _presentation;
            }
        }

        for (std::map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const OOX::FileType& curType = pPair->second->type();

            if (OOX::Presentation::FileTypes::SlideMaster == curType)
            {
                smart_ptr<PPTX::SlideMaster> pointer = pPair->second.smart_dynamic_cast<PPTX::SlideMaster>();
                if (pointer.is_init())
                    pointer->ApplyRels();
            }

        }

        for (std::map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const OOX::FileType& curType = pPair->second->type();

            if (OOX::Presentation::FileTypes::SlideLayout == curType)
            {
                smart_ptr<PPTX::SlideLayout> pointer = pPair->second.smart_dynamic_cast<PPTX::SlideLayout>();
                if (pointer.is_init())
                    pointer->ApplyRels();
            }

        }

        for (std::map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const OOX::FileType& curType = pPair->second->type();

            if (OOX::Presentation::FileTypes::Slide == curType)
            {
                smart_ptr<PPTX::Slide> pointer = pPair->second.smart_dynamic_cast<PPTX::Slide>();
                if (pointer.is_init())
                    pointer->ApplyRels();
            }
        }

        for (std::map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const OOX::FileType& curType = pPair->second->type();
           
			if (OOX::Presentation::FileTypes::NotesMaster == curType)
            {
                smart_ptr<PPTX::NotesMaster> pointer = pPair->second.smart_dynamic_cast<PPTX::NotesMaster>();
                if (pointer.is_init())
                    pointer->ApplyRels();
            }
        }

        for (std::map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = map.m_map.begin(); pPair != map.m_map.end(); ++pPair)
        {
            const OOX::FileType& curType = pPair->second->type();
           
			if (OOX::Presentation::FileTypes::NotesSlide == curType)
            {
                smart_ptr<PPTX::NotesSlide> pointer = pPair->second.smart_dynamic_cast<PPTX::NotesSlide>();
                if (pointer.is_init())
                    pointer->ApplyRels();
            }
        }
		return true;
	}

	void Document::write(const OOX::CPath& path)
	{
		OOX::CSystemUtility::CreateDirectories(path);

		OOX::CRels rels;
		OOX::CContentTypes content;

		OOX::CPath dir = path;
		FileContainer::write(rels, path, dir, content);

		rels.Write(path / FILE_SEPARATOR_STR);
		content.Write(path);
		FileContainer::WrittenSetFalse();
	}

	void Document::createFromTemplate(const OOX::CPath& path)
	{
		//read(path);
	}

	const bool Document::isValid(const OOX::CPath& path) const
	{
		return true;//FileContainer::exist(OOX::Presentation::FileTypes::Presentation);
	}

	//void Document::extractPictures(const OOX::CPath& path)
	//{
	//	OOX::CSystemUtility::CreateDirectories(path);
	//	FileContainer::ExtractPictures(path);
	//}

	//void Document::extractPictures(const OOX::CPath& source, const OOX::CPath& path)
	//{
	//	//read(source);
	//	extractPictures(path);
	//}

	long Document::CountFiles(const OOX::CPath& path)
	{
		return OOX::CSystemUtility::GetFilesCount(path.GetDirectory(), true);
	}
} // namespace PPTX
