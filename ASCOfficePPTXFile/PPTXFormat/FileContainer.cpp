//#include "./stdafx.h"

#include "FileContainer.h"
#include "DocxFormat/Rels/File.h"
#include "FileFactory.h"
#include "DocxFormat/ContentTypes/File.h"
#include "DocxFormat/FileType.h"
#include "DocxFormat/FileTypes.h"
#include "DocxFormat/External/HyperLink.h"
#include "WrapperFile.h"

#include <map>

namespace PPTX
{
	void FileContainer::read(const OOX::CPath& filename)
	{
		//not implement FileContainer.read
	}

	void FileContainer::read(const PPTX::Rels::File& rels, const OOX::CPath& path)
	{
		//not implement FileContainer.read
	}

	void FileContainer::read(const OOX::CPath& filename, FileMap& map, IPPTXEvent* Event)
	{
		PPTX::Rels::File rels(filename);
		OOX::CPath path = filename.GetDirectory();
		read(rels, path, map, Event);
	}

	void FileContainer::read(const PPTX::Rels::File& rels, const OOX::CPath& path, FileMap& map, IPPTXEvent* Event)
	{
		bool bIsSlide = false;
		PPTX::File* pSrcFile = dynamic_cast<PPTX::File*>(this);
		if (NULL != pSrcFile)
			bIsSlide = (pSrcFile->type() == PPTX::FileTypes::Slide) ? true : false;

		size_t nCount = rels.Relations.m_items.size();

        for (size_t i = 0; i < nCount; ++i)
		{
			const PPTX::Rels::RelationShip* pRelation = &(rels.Relations.m_items[i]);
			OOX::CPath normPath = path / pRelation->target();

            std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = map.find(normPath);

			if (bIsSlide && (pRelation->type() == PPTX::FileTypes::Slide))
			{
				long percent = Event->GetPercent();

				smart_ptr<PPTX::File> file = smart_ptr<PPTX::File>(new PPTX::HyperLink(pRelation->target()));

                bool res = Event->Progress(0, percent + m_lPercent);

                add(pRelation->rId(), file);

                if (res || m_bCancelled)
                {
                    m_bCancelled = true;
                    break;
                }
			}
			else
			{
                if (pPair != map.m_map.end())
				{
                    add(pRelation->rId(), pPair->second);
				}
				else
				{
					long percent = Event->GetPercent();

					smart_ptr<PPTX::File> file = PPTX::FileFactory::CreateFilePPTX(path, *pRelation, map);													
					
					map.add(normPath, file);
					add(pRelation->rId(), file);

					smart_ptr<FileContainer> pContainer = file.smart_dynamic_cast<FileContainer>();
                    bool res = Event->Progress(0, percent + m_lPercent);

                    if (pContainer.IsInit())
					{
						pContainer->m_lPercent = m_lPercent;
						Event->AddPercent(m_lPercent);

						pContainer->read(normPath, map, Event);
						m_bCancelled = pContainer->m_bCancelled;
					}

                    if (res || m_bCancelled)
                    {
                        m_bCancelled = true;
                        break;
                    }
                }
			}
		}
	}

	void FileContainer::write(const OOX::CPath& filename, const OOX::CPath& directory, PPTX::ContentTypes::File& content) const
	{
		PPTX::Rels::File rels;
		OOX::CPath current = filename.GetDirectory();
		write(rels, current, directory, content);
		rels.write(filename);
	}

	void FileContainer::write(PPTX::Rels::File& rels, const OOX::CPath& curdir, const OOX::CPath& directory, PPTX::ContentTypes::File& content) const
	{
		std::map<CString, size_t> mNamePair;
		for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = m_container.begin(); pPair != m_container.end(); ++pPair)
		{
			smart_ptr<PPTX::File>     pFile = pPair->second;
			smart_ptr<PPTX::External> pExt  = pFile.smart_dynamic_cast<PPTX::External>();
			if ( !pExt.IsInit() )
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
						rels.registration(pPair->first, pFile->type(), defdir / name);
					}
					else
					{
						OOX::CPath defdir	= pFile->DefaultDirectory();
						OOX::CPath name		= file->GetWrittenFileName();

						rels.registration(pPair->first, pFile->type(), defdir / name);
					}
				}
				else
				{
					OOX::CPath defdir	= pFile->DefaultDirectory();
					OOX::CPath name		= pFile->DefaultFileName();

					OOX::CSystemUtility::CreateDirectories(directory / defdir);
					pFile->write(directory / defdir / name, directory, content);
					rels.registration(pPair->first, pFile->type(), defdir / name);
				}
			}
			else
			{
				rels.registration(pPair->first, pExt);
			}
		}
	}

	void FileContainer::WrittenSetFalse()
	{
		for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = m_container.begin(); pPair != m_container.end(); ++pPair)
		{
			smart_ptr<PPTX::File> pFile = pPair->second;

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

	void CCommonRels::_read(const PPTX::Rels::File& rels, const OOX::CPath& path)
	{
		size_t nCount = rels.Relations.m_items.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			const PPTX::Rels::RelationShip* pRelation = &(rels.Relations.m_items[i]);

			smart_ptr<PPTX::File> _file = PPTX::FileFactory::CreateFilePPTX_OnlyMedia(path, *pRelation);
			add(pRelation->rId(), _file);	
		}
	}

	void CCommonRels::_read(const OOX::CPath& filename)
	{
		PPTX::Rels::File rels(filename);
		OOX::CPath path = filename.GetDirectory();
		_read(rels, path);
	}
} // namespace PPTX
