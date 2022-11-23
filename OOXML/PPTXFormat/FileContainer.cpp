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


#include "Folder.h"

#include "FileTypes.h"
#include "FileFactory.h"
#include "WrapperFile.h"

#include "../DocxFormat/Rels.h"
#include "../DocxFormat/ContentTypes.h"
#include "../DocxFormat/External/HyperLink.h"
#include "../DocxFormat/FileTypes.h"
#include "../../DesktopEditor/common/Directory.h"

#include "../DocxFormat/Media/Image.h"
#include "../DocxFormat/Media/OleObject.h"

#include <boost/unordered_map.hpp>

namespace PPTX
{
	static std::wstring arDefDirectories [9][2] = //in ppt Directory
	{
		{L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide",			L"slides"},
		{L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout",	L"slideLayouts"},
		{L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster",	L"slideMasters"},
		{L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesSlide",		L"notesSlides"},
		{L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster",	L"notesMasters"},
		{L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/handoutMaster",	L"handoutMasters"},		
		{L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments",		L"comments"},
		{L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/commentAuthors",	L""},
		{L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme",			L"theme"},
	};

	static std::wstring FindFileInDirectory(std::wstring directory, std::wstring filename)
	{
		if (directory.empty()) return L"";

		if (directory[directory.length() - 1] == FILE_SEPARATOR_CHAR)
			directory = directory.substr(0, directory.length() - 1);

		size_t pos_ppt = directory.rfind(L"ppt");
		if (std::wstring::npos != pos_ppt)
		{
			directory = directory.substr(0, pos_ppt - 1); //root directory
		}
        std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(directory, true);

        for (size_t i = 0 ; i < arrFiles.size(); i++)
		{
			if (std::wstring::npos != arrFiles[i].find(filename))
			{
				return arrFiles[i];
			}
		}
		return L"";
	}

	void FileContainer::read(const OOX::CPath& filename)
	{
	}

	void FileContainer::read(const OOX::CRels& rels, const OOX::CPath& path)
	{
	}
	const bool FileContainer::IsExist(const OOX::FileType& oType) const
	{
		for (size_t i = 0; i < m_arContainer.size(); ++i)
		{
			smart_ptr<OOX::File> pFile	= m_arContainer[i];

			if (oType == pFile->type())
				return true;
		}

		return false;
	}
	smart_ptr<OOX::File> FileContainer::Get(const OOX::FileType& oType)
	{
		for (size_t i = 0; i < m_arContainer.size(); ++i)
		{
			smart_ptr<OOX::File> &pFile	= m_arContainer[i];
			if (oType == pFile->type())
				return pFile;
		}

		return smart_ptr<OOX::File>(new OOX::UnknowTypeFile( m_oUnknown ));
	}

	void FileContainer::Get(const OOX::FileType& oType, std::vector<smart_ptr<OOX::File>> & files)
	{
		for (size_t i = 0; i < m_arContainer.size(); ++i)
		{
			smart_ptr<OOX::File> &pFile	= m_arContainer[i];
			
			if ( oType == pFile->type() )
				files.push_back(pFile);
		}
	}
	std::wstring FileContainer::GetImagePathNameFromRId(const OOX::RId& rid)const
	{
		smart_ptr<OOX::Image> p = IFileContainer::Get<OOX::Image>(rid);
		if (!p.is_init())
			return _T("");
		return p->filename().m_strFilename;
	}
	std::wstring FileContainer::GetLinkFromRId(const OOX::RId& rid)const
	{
		smart_ptr<OOX::External> pExt = Find(rid).smart_dynamic_cast<OOX::External>();
		if (pExt.IsInit())
			return pExt->Uri().m_strFilename;

		smart_ptr<OOX::Media> pMedia = Find(rid).smart_dynamic_cast<OOX::Media>();
		if (pMedia.IsInit())
			return pMedia->filename().m_strFilename;

		return _T("");
	}
	std::wstring FileContainer::GetOleFromRId(const OOX::RId& rid)const
	{
		smart_ptr<OOX::OleObject> p = IFileContainer::Get<OOX::OleObject>(rid);
		if (!p.is_init())
			return _T("");
		return p->filename().m_strFilename;
	}
	void FileContainer::read(const OOX::CRels& rels, const OOX::CPath& path, FileMap& map)
	{
		bool bIsSlide = false;
		OOX::File* pSrcFile = dynamic_cast<OOX::File*>(this);
		if (NULL != pSrcFile)
		{
			bIsSlide = (pSrcFile->type() == OOX::Presentation::FileTypes::Slide || 
						pSrcFile->type() == OOX::Presentation::FileTypes::NotesSlide) ? true : false;
		}

		for (size_t i = 0; i <  rels.m_arRelations.size(); ++i)
		{       
			OOX::Rels::CRelationShip* pRelation = rels.m_arRelations[i];

 			OOX::CPath normPath = CorrectPathRels(path, pRelation);

			std::map<std::wstring, smart_ptr<OOX::File>>::iterator pPair = map.find(normPath);

            if (bIsSlide && (pRelation->Type() == OOX::FileTypes::HyperLink ||
                             pRelation->Type() == OOX::Presentation::FileTypes::Slide))
			{// + external audio, video ... - в обычных ...

				smart_ptr<OOX::File> file = smart_ptr<OOX::File>(new OOX::HyperLink(OOX::IFileContainer::m_pMainDocument, pRelation->Target()));

				if (pRelation->Type() == OOX::Presentation::FileTypes::Slide)
				{
					OOX::HyperLink *link = dynamic_cast<OOX::HyperLink*>(file.GetPointer());
					if (link)
						link->bHyperlink = false;
				}

                normPath = pRelation->Target();
                Add(pRelation->rId(), file);

			}
			else
			{
                if (pPair != map.m_map.end())
				{
                    Add(pRelation->rId(), pPair->second);
				}
				else
				{
					smart_ptr<OOX::File> file = PPTX::FileFactory::CreateFilePPTX(normPath, *pRelation, map, OOX::IFileContainer::m_pMainDocument);

					if (file.IsInit() == false)
						continue;
					
					map.add(normPath, file);
					Add(pRelation->rId(), file);

					smart_ptr<FileContainer> pContainer = file.smart_dynamic_cast<FileContainer>();
                    
                    if (pContainer.IsInit())
					{
						pContainer->read(normPath, map);
					}
                }
			}
		}
	}

	void FileContainer::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content) const
	{
		return IFileContainer::Write(filename, directory, content);
		
		OOX::CRels rels;
		OOX::CPath current = filename.GetDirectory();
		write(rels, current, directory, content);
		rels.Write(filename);
	}

	void FileContainer::write(OOX::CRels& rels, const OOX::CPath& curdir, const OOX::CPath& directory, OOX::CContentTypes& content) const
	{
		std::map<std::wstring, size_t> mNamePair;
       
        for (boost::unordered_map<std::wstring, smart_ptr<OOX::File>>::const_iterator pPair = m_mapContainer.begin(); pPair != m_mapContainer.end(); ++pPair)
		{
			smart_ptr<OOX::File>		pFile = pPair->second;

			smart_ptr<OOX::External>	pExt	= pFile.smart_dynamic_cast<OOX::External>();
			smart_ptr<OOX::Media>		pMedia  = pFile.smart_dynamic_cast<OOX::Media>();

			bool bExternal = pExt.IsInit() || ((pMedia.IsInit()) && (pMedia->IsExternal()));
			if ( !bExternal )
			{
				smart_ptr<PPTX::WrapperFile> file = pFile.smart_dynamic_cast<PPTX::WrapperFile>();

				if (file.IsInit())
				{
					if (file->GetWrittenStatus() == false)
					{
						OOX::CPath defdir	= pFile->DefaultDirectory();
						OOX::CPath name		= pFile->DefaultFileName();

						//name = name + max_name_index(curdir, name.string());

						OOX::CSystemUtility::CreateDirectories(directory / defdir);
						pFile->write(directory / defdir / name, directory, content);
						rels.Registration(pPair->first, pFile->type(), defdir / name);
					}
					else
					{
						OOX::CPath defdir	= pFile->DefaultDirectory();
						OOX::CPath name		= file->GetWrittenFileName();

						rels.Registration(pPair->first, pFile->type(), defdir / name);
					}
				}
				else
				{
					OOX::CPath defdir	= pFile->DefaultDirectory();
					OOX::CPath name		= pFile->DefaultFileName();

					OOX::CSystemUtility::CreateDirectories(directory / defdir);
					pFile->write(directory / defdir / name, directory, content);
					
					rels.Registration(pPair->first, pFile->type(), defdir / name);
				}
			}
			else
			{
				rels.Registration(pPair->first, pExt);
			}
		}
	}

	void FileContainer::WrittenSetFalse()
	{
		for (size_t i = 0; i < m_arContainer.size(); i++)
		{
			smart_ptr<OOX::File> &pFile = m_arContainer[i];

			smart_ptr<PPTX::WrapperFile>	pWrapFile = pFile.smart_dynamic_cast<PPTX::WrapperFile>();
			smart_ptr<PPTX::FileContainer>	pWrapCont = pFile.smart_dynamic_cast<PPTX::FileContainer>();

			if (pWrapFile.is_init() && !pWrapFile->GetWrittenStatus())
			{
				pWrapFile->WrittenSetFalse();	

				if (pWrapCont.is_init())
				{
					pWrapCont->WrittenSetFalse();
				}
			}
		}
	}

	void FileContainer::read(const OOX::CPath& filename, FileMap& map)
	{
		OOX::CRels rels(filename);
		OOX::CPath path = filename.GetDirectory();
		read(rels, path, map);
	}

	OOX::CPath FileContainer::CorrectPathRels(const OOX::CPath& path, OOX::Rels::CRelationShip* relation )
	{
        if (relation->IsExternal()) return relation->Target();

		OOX::CPath filename = path / relation->Target();
		
		if ( NSFile::CFileBinary::Exists(filename.GetPath()) == true ) return filename;
		
		//tf22977542_win32.potx

		PPTX::Document *pPptxDocument = dynamic_cast<PPTX::Document *>(m_pMainDocument);

		if (m_pMainDocument)
		{
			OOX::CPath main_path(m_pMainDocument->m_sDocumentPath);
			filename = main_path / relation->Target();
			
			if (NSFile::CFileBinary::Exists(filename.GetPath()) == true) return filename;
		}
		//file_1_ (1).pptx			
		std::wstring strDefDirectory;
		for (int i = 0; i < 9; i++)
		{
			if (relation->Type() == arDefDirectories[i][0])
			{
				strDefDirectory = arDefDirectories[i][1];
				break;
			}
		}

		if (strDefDirectory.empty())
		{
			if (std::wstring::npos != relation->Type().find(L"image") ||
				std::wstring::npos != relation->Type().find(L"audio") ||
				std::wstring::npos != relation->Type().find(L"media"))
			{
				strDefDirectory = L"media";
			}
			else return OOX::CPath();
		}
		
		OOX::CPath new_filename = strDefDirectory + FILE_SEPARATOR_STR + relation->Filename().GetFilename();
		
		filename = path / new_filename;
		
		if (NSFile::CFileBinary::Exists(filename.GetPath()) == false) 
		{
			filename = FindFileInDirectory(path.GetPath(), relation->Filename().GetFilename()); // find true path by filename

			if (NSFile::CFileBinary::Exists(filename.GetPath()) == false) 
				return filename;
		}

		*relation = OOX::Rels::CRelationShip( relation->rId(), relation->Type(), filename);

		return filename;
	}

//---------------------------------------------------------------------------------------------------------------------------
	void WrapperFile::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content) const
	{
		m_WrittenFileName = filename.GetFilename();

		NSBinPptxRW::CXmlWriter oXmlWriter;
		toXmlWriter(&oXmlWriter);

		oXmlWriter.SaveToFile(filename.m_strFilename);

		content.Registration(type().OverrideType(), directory, m_WrittenFileName);
		m_written = true;
	}

} // namespace PPTX
