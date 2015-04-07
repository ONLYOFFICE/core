//#include "./stdafx.h"

#include "IFileContainer.h"
#include "Rels/File.h"

#ifndef NODOCX
//#include "FileFactory.h"
#endif

#include "ContentTypes/File.h"
#include "FileType.h"
#include "External/External.h"
#include "External/HyperLink.h"
#include "Drawing/LegacyDiagramText.h"
#include "Media/Image.h"
#include "Media/OleObject.h"
#include "FileTypes.h"

#include <map>
#include <vector>

namespace PPTX
{
	UnknowTypeFile IFileContainer::unknow;

	void IFileContainer::read(const OOX::CPath& filename)
	{
		PPTX::Rels::File rels(filename);
		read(rels, filename.GetDirectory());
	}


	void IFileContainer::read(const Rels::File& rels, const OOX::CPath& path)
	{
#ifndef NODOCX
		/*BOOST_FOREACH(const PPTX::Rels::RelationShip& relation, *rels.Relations)
		{
			add(relation.rId(), PPTX::CreateFile(path, relation));
		}*/

        size_t nCount = rels.Relations.m_items.size();
		for (size_t i = 0; i < nCount; ++i)
		{
            add(rels.Relations.m_items[i].rId(), PPTX::CreateFile(path, rels.Relations.m_items[i]));
		}
#endif
	}


	void IFileContainer::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
		PPTX::Rels::File rels;
		OOX::CPath current = filename.GetDirectory();
		write(rels, current, directory, content);
		rels.write(filename);
	}


	void IFileContainer::write(Rels::File& rels, const OOX::CPath& curdir, const OOX::CPath& directory, ContentTypes::File& content) const
	{
        std::map<CString, size_t> namepair;
        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.begin(); it != m_container.end(); ++it)
        {
            smart_ptr<PPTX::File>     pFile = it->second;
            smart_ptr<PPTX::External> pExt  = pFile.smart_dynamic_cast<PPTX::External>();

			if (!pExt.IsInit())
			{
				OOX::CPath defdir	= pFile->DefaultDirectory();
				OOX::CPath name		= pFile->DefaultFileName();

                std::map<CString, size_t>::const_iterator pNamePair = namepair.find( name.m_strFilename );
                if ( pNamePair == namepair.end())
                    namepair [name.m_strFilename] = 1;
                else
                    name = name + pNamePair->first;

				OOX::CSystemUtility::CreateDirectories(curdir / defdir);
				pFile->write(curdir / defdir / name, directory / defdir, content);
                rels.registration(it->first, pFile->type(), defdir / name);
			}
			else
			{
				//const HyperLink hyperlink = static_cast<const HyperLink&>(*pair.second);
				//rels.registration(pair.first, hyperlink);
                rels.registration(it->first, pExt);
			}
		}
	}


	void IFileContainer::Commit(const OOX::CPath& path)
	{
        std::map<CString, size_t> namepair;
        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.begin(); it != m_container.end(); ++it)
        {
            smart_ptr<PPTX::File>     pFile = it->second;
            smart_ptr<PPTX::External> pExt  = pFile.smart_dynamic_cast<PPTX::External>();

            if (!pExt.IsInit())
            {
                OOX::CPath defdir	= pFile->DefaultDirectory();
                OOX::CPath name		= pFile->DefaultFileName();

                std::map<CString, size_t>::const_iterator pNamePair = namepair.find( name.m_strFilename );
                if ( pNamePair == namepair.end())
                    namepair [name.m_strFilename] = 1;
                else
                    name = name + pNamePair->first;

				OOX::CSystemUtility::CreateDirectories(path / defdir);
				
                smart_ptr<PPTX::IFileBuilder> fileBuilder = it->second.smart_dynamic_cast<PPTX::IFileBuilder>();
				if (fileBuilder.is_init())
					fileBuilder->Commit(path / defdir / name);
			}
		}
	}


	void IFileContainer::Finalize(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content)
	{
		PPTX::Rels::File rels;
		OOX::CPath current = filename.GetDirectory();
		Finalize(rels, current, directory, content);
		rels.write(filename);
	}

	
	void IFileContainer::Finalize(Rels::File& rels, const OOX::CPath& curdir, const OOX::CPath& directory, ContentTypes::File& content)
	{
        std::map<CString, size_t> namepair;
        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.begin(); it != m_container.end(); ++it)
        {
            smart_ptr<PPTX::File>     pFile = it->second;
            smart_ptr<PPTX::External> pExt  = pFile.smart_dynamic_cast<PPTX::External>();

            if (!pExt.IsInit())
            {
                OOX::CPath defdir	= pFile->DefaultDirectory();
                OOX::CPath name		= pFile->DefaultFileName();

                std::map<CString, size_t>::const_iterator pNamePair = namepair.find( name.m_strFilename );
                if ( pNamePair == namepair.end())
                    namepair [name.m_strFilename] = 1;
                else
                    name = name + pNamePair->first;

				OOX::CSystemUtility::CreateDirectories(curdir / defdir);
				
				smart_ptr<PPTX::IFileBuilder> fileBuilder = pFile.smart_dynamic_cast<PPTX::IFileBuilder>(); 

				if ( fileBuilder.is_init() )
				{
					fileBuilder->Finalize(curdir / defdir / name, directory / defdir, content);
				}
				else
				{
					pFile->write(curdir / defdir / name, directory / defdir, content);
				}

                rels.registration(it->first, pFile->type(), defdir / name);
			}
			else
			{
                rels.registration(it->first, pExt);
			}
		}
	}


	void IFileContainer::extractPictures(const OOX::CPath& path) const
	{
        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.begin(); it != m_container.end(); ++it)
        {
            smart_ptr<PPTX::File> pFile  = it->second;

            smart_ptr<Image>     pImage = pFile.smart_dynamic_cast<Image>();
            if ( pImage.is_init() )
            {
                pImage->copy_to( path );
                continue;
            }

            smart_ptr<IFileContainer> pExt = pFile.smart_dynamic_cast<IFileContainer>();
            if ( pExt.is_init() )
            {
                pExt->extractPictures( path );
                continue;
            }
        }
	}


	smart_ptr<Image> IFileContainer::image(const RId& rId) const
	{
        std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = m_container.find(rId.get());
        if (pPair == m_container.end ())
            return smart_ptr<Image>();
        return pPair->second.smart_dynamic_cast<Image>();
	}

	smart_ptr<HyperLink> IFileContainer::hyperlink(const RId& rId) const
	{
        std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = m_container.find(rId.get());
        if (pPair == m_container.end ())
            return smart_ptr<HyperLink>();
        return pPair->second.smart_dynamic_cast<HyperLink>();
	}

	smart_ptr<OleObject> IFileContainer::oleObject(const RId& rId) const
	{
        std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = m_container.find(rId.get());
        if (pPair == m_container.end ())
            return smart_ptr<OleObject>();
        return pPair->second.smart_dynamic_cast<OleObject>();
	}
	smart_ptr<LegacyDiagramText> IFileContainer::legacyDiagramText(const RId& rId) const
	{
        std::map<CString, smart_ptr<PPTX::File>>::const_iterator pPair = m_container.find(rId.get());
        if (pPair == m_container.end ())
            return smart_ptr<LegacyDiagramText>();
        return pPair->second.smart_dynamic_cast<LegacyDiagramText>();
	}
	const bool IFileContainer::exist(const FileType& type) const
	{
        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.begin(); it != m_container.end(); ++it)
        {
            if (type == it->second->type())
                return true;
        }

        return false;
	}


	const bool IFileContainer::exist(const RId& rId) const
	{
        std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.find(rId.get());
        return (it != m_container.end());
	}


	const bool IFileContainer::isExternal(const PPTX::RId& rId) const
	{
        std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.find(rId.get());

        if (it != m_container.end())
        {
            CString type = it->second->type().RelationType();
            CString name = it->second->type().DefaultFileName().m_strFilename;

            return (((type == PPTX::FileTypes::ExternalAudio.RelationType()) || (type == PPTX::FileTypes::ExternalImage.RelationType())
                || (type == PPTX::FileTypes::ExternalVideo.RelationType())) && (name == _T("")));
        }
        return true;
	}


	smart_ptr<PPTX::File> IFileContainer::get(const FileType& type)
	{
        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.begin(); it != m_container.end(); ++it)
        {
            if (type == it->second->type())
                return it->second;
        }

        return smart_ptr<PPTX::File>(new UnknowTypeFile( unknow ));
	}


	const RId IFileContainer::add(const smart_ptr<PPTX::File>& file)
	{
		const RId rId = maxRId().next();
		add(rId, file);
		return rId;
	}


	void IFileContainer::add(const PPTX::RId rId, const smart_ptr<PPTX::File>& file)
	{
		//m_lMaxRid = max(m_lMaxRid, rId.get());
        m_container [rId.get()] = file;
	}


	smart_ptr<PPTX::File> IFileContainer::find(const FileType& type) const
	{
        for (std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.begin(); it != m_container.end(); ++it)
        {
            if (type == it->second->type())
                return it->second;

        }

		return smart_ptr<PPTX::File>((PPTX::File*)new UnknowTypeFile());
	}

	smart_ptr<PPTX::File> IFileContainer::find(const PPTX::RId& rId) const
	{
        std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.find(rId.get());

        if (it != m_container.end())
            return it->second;

		smart_ptr<PPTX::File> pointer;
		return pointer;
	}


	smart_ptr<PPTX::File> IFileContainer::operator [](const PPTX::RId rId)
	{
        std::map<CString, smart_ptr<PPTX::File>>::const_iterator it = m_container.find(rId.get());

        if (it != m_container.end())
            return it->second;

		smart_ptr<PPTX::File> pointer;
		return pointer;
	}


	smart_ptr<PPTX::File> IFileContainer::operator [](const FileType& type)
	{
		return find(type);
	}

	const RId IFileContainer::maxRId()
	{
		++m_lMaxRid;
		return RId(m_lMaxRid);
	}

} // namespace PPTX
