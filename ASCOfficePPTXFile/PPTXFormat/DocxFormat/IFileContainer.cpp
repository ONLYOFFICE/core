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
#include "Media/Image.h"
#include "Media/OleObject.h"
#include "FileTypes.h"


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

		size_t nCount = rels.Relations.m_items.GetCount();
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
		CAtlMap<CString, size_t> namepair;

		POSITION pos = m_container.GetStartPosition();
		while (NULL != pos)
		{
			const CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.GetNext(pos);
			
			smart_ptr<PPTX::File> pFile		= pPair->m_value;
			smart_ptr<PPTX::External> pExt	= pFile.smart_dynamic_cast<PPTX::External>();

			if (!pExt.IsInit())
			{
				OOX::CPath defdir	= pFile->DefaultDirectory();
				OOX::CPath name		= pFile->DefaultFileName();

				CAtlMap<CString, size_t>::CPair* pNamePair = namepair.Lookup(name.m_strFilename);
				if (NULL == pNamePair)
					namepair.SetAt(name.m_strFilename, 1);
				else
					name = name + pNamePair->m_key;

				OOX::CSystemUtility::CreateDirectories(curdir / defdir);
				pFile->write(curdir / defdir / name, directory / defdir, content);
				rels.registration(pPair->m_key, pFile->type(), defdir / name);
			}
			else
			{
				//const HyperLink hyperlink = static_cast<const HyperLink&>(*pair.second);
				//rels.registration(pair.first, hyperlink);
				rels.registration(pPair->m_key, pExt);
			}
		}
	}


	void IFileContainer::Commit(const OOX::CPath& path)
	{
		CAtlMap<CString, size_t> namepair;

		POSITION pos = m_container.GetStartPosition();
		while (NULL != pos)
		{
			CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.GetNext(pos);
			
			smart_ptr<PPTX::File> pFile		= pPair->m_value;
			smart_ptr<PPTX::External> pExt	= pFile.smart_dynamic_cast<PPTX::External>();

			if (!pExt.IsInit())
			{
				OOX::CPath defdir	= pFile->DefaultDirectory();
				OOX::CPath name		= pFile->DefaultFileName();

				CAtlMap<CString, size_t>::CPair* pNamePair = namepair.Lookup(name.m_strFilename);
				if (NULL == pNamePair)
					namepair.SetAt(name.m_strFilename, 1);
				else
					name = name + pNamePair->m_key;

				OOX::CSystemUtility::CreateDirectories(path / defdir);
				
				smart_ptr<PPTX::IFileBuilder> fileBuilder = pPair->m_value.smart_dynamic_cast<PPTX::IFileBuilder>();
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
		CAtlMap<CString, size_t> namepair;

		POSITION pos = m_container.GetStartPosition();
		while (NULL != pos)
		{
			CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.GetNext(pos);
			
			smart_ptr<PPTX::File> pFile		= pPair->m_value;
			smart_ptr<PPTX::External> pExt	= pFile.smart_dynamic_cast<PPTX::External>();

			if (!pExt.IsInit())
			{
				OOX::CPath defdir	= pFile->DefaultDirectory();
				OOX::CPath name		= pFile->DefaultFileName();

				CAtlMap<CString, size_t>::CPair* pNamePair = namepair.Lookup(name.m_strFilename);
				if (NULL == pNamePair)
					namepair.SetAt(name.m_strFilename, 1);
				else
					name = name + pNamePair->m_key;

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

				rels.registration(pPair->m_key, pFile->type(), defdir / name);
			}
			else
			{
				rels.registration(pPair->m_key, pExt);
			}
		}
	}


	void IFileContainer::extractPictures(const OOX::CPath& path) const
	{
		POSITION pos = m_container.GetStartPosition();
		while (NULL != pos)
		{
			smart_ptr<PPTX::File> pFile		= m_container.GetNextValue(pos);

			smart_ptr<Image> pImage = pFile.smart_dynamic_cast<Image>();
			if (pImage.is_init())
			{
				pImage->copy_to(path);
				continue;
			}
			smart_ptr<IFileContainer> pExt = pFile.smart_dynamic_cast<IFileContainer>();
			if (pExt.is_init())
			{
				pExt->extractPictures(path);
				continue;
			}
		}
	}


	smart_ptr<Image> IFileContainer::image(const RId& rId) const
	{
		const CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.Lookup(rId.get());
		if (NULL == pPair)
			return smart_ptr<Image>();
		return pPair->m_value.smart_dynamic_cast<Image>();
	}

	smart_ptr<HyperLink> IFileContainer::hyperlink(const RId& rId) const
	{
		const CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.Lookup(rId.get());
		if (NULL == pPair)
			return smart_ptr<HyperLink>();
		return pPair->m_value.smart_dynamic_cast<HyperLink>();
	}

	smart_ptr<OleObject> IFileContainer::oleObject(const RId& rId) const
	{
		const CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.Lookup(rId.get());
		if (NULL == pPair)
			return smart_ptr<OleObject>();
		return pPair->m_value.smart_dynamic_cast<OleObject>();
	}

	const bool IFileContainer::exist(const FileType& type) const
	{
		POSITION pos = m_container.GetStartPosition();
		while (NULL != pos)
		{
			const CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.GetNext(pos);
			if (type == pPair->m_value->type())
				return true;
		}
		return false;
	}


	const bool IFileContainer::exist(const RId& rId) const
	{
		const CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.Lookup(rId.get());
		return (NULL != pPair);
	}


	const bool IFileContainer::isExternal(const PPTX::RId& rId) const
	{
		const CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.Lookup(rId.get());

		if (NULL != pPair)
		{
			CString type	= pPair->m_value->type().RelationType();
			CString name	= pPair->m_value->type().DefaultFileName().m_strFilename;
			
			return (((type == PPTX::FileTypes::ExternalAudio.RelationType()) || (type == PPTX::FileTypes::ExternalImage.RelationType())
				|| (type == PPTX::FileTypes::ExternalVideo.RelationType())) && (name == _T("")));
		}
		return true;
	}


	smart_ptr<PPTX::File> IFileContainer::get(const FileType& type)
	{
		POSITION pos = m_container.GetStartPosition();
		while (NULL != pos)
		{
			CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.GetNext(pos);
			if (type == pPair->m_value->type())
				return pPair->m_value;
		}
		return smart_ptr<PPTX::File>(new UnknowTypeFile(unknow));
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
		m_container.SetAt(rId.get(), file);
	}


	smart_ptr<PPTX::File> IFileContainer::find(const FileType& type) const
	{
		POSITION pos = m_container.GetStartPosition();
		while (NULL != pos)
		{
			const CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.GetNext(pos);
			if (type == pPair->m_value->type())
				return pPair->m_value;
		}
		return smart_ptr<PPTX::File>((PPTX::File*)new UnknowTypeFile());
	}

	smart_ptr<PPTX::File> IFileContainer::find(const PPTX::RId& rId) const
	{
		const CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.Lookup(rId.get());
		if (NULL != pPair)
			return pPair->m_value;

		smart_ptr<PPTX::File> pointer;
		return pointer;
	}


	smart_ptr<PPTX::File> IFileContainer::operator [](const PPTX::RId rId)
	{
		CAtlMap<CString, smart_ptr<PPTX::File>>::CPair* pPair = m_container.Lookup(rId.get());
		if (NULL != pPair)
			return pPair->m_value;

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
