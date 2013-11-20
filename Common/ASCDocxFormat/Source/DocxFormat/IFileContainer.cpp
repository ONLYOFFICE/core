
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "IFileContainer.h"
#include <boost/foreach.hpp>
#include "Rels/File.h"
#include "FileFactory.h"
#include "ContentTypes/File.h"
#include "FileType.h"
#include "External\External.h"
#include "External\HyperLink.h"
#include "Media\Image.h"
#include "Media\OleObject.h"
#include "FileNameUtility.h"
#include "FileTypes.h"


namespace OOX
{
	UnknowTypeFile IFileContainer::unknow;

	void IFileContainer::read(const boost::filesystem::wpath& filename)
	{
		OOX::Rels::File rels(filename);
		const boost::filesystem::wpath path = filename.parent_path();
		read(rels, path);
	}


	void IFileContainer::read(const Rels::File& rels, const boost::filesystem::wpath& path)
	{
#ifndef NODOCX
		BOOST_FOREACH(const OOX::Rels::RelationShip& relation, *rels.Relations)
		{
			add(relation.rId(), OOX::CreateFile(path, relation));
		}
#endif
	}


	void IFileContainer::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		OOX::Rels::File rels;
		const boost::filesystem::wpath current = filename.parent_path();
		write(rels, current, directory, content);
		rels.write(filename);
	}


	void IFileContainer::write(Rels::File& rels, const boost::filesystem::wpath& curdir, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		std::map<boost::filesystem::wpath, size_t> namepair;
		BOOST_FOREACH(const RIdFilePair& pair, m_container)
		{
//			if (dynamic_cast<const External*>(pair.second.get()) == 0)
			if (boost::shared_dynamic_cast<External, OOX::File>(pair.second) == 0)
			{
				const boost::filesystem::wpath defdir = pair.second->DefaultDirectory();
				boost::filesystem::wpath name = pair.second->DefaultFileName();
				if (namepair[name]++ != 0)
					name = name + namepair[name];

				boost::filesystem::create_directories(curdir / defdir);
				pair.second->write(curdir / defdir / name, directory / defdir, content);
				rels.registration(pair.first, pair.second->type(), defdir / name);
			}
			else
			{
//				const HyperLink hyperlink = static_cast<const HyperLink&>(*pair.second);
//				rels.registration(pair.first, hyperlink);
				rels.registration(pair.first, boost::shared_dynamic_cast<External, OOX::File>(pair.second));
			}
		}
	}


	void IFileContainer::Commit(const boost::filesystem::wpath& path)
	{
		std::map<boost::filesystem::wpath, size_t> namepair;
		BOOST_FOREACH(const RIdFilePair& pair, m_container)
		{
			if (boost::shared_dynamic_cast<External, OOX::File>(pair.second) == 0)
			{
				const boost::filesystem::wpath defdir = pair.second->DefaultDirectory();
				boost::filesystem::wpath name = pair.second->DefaultFileName();
				if (namepair[name]++ != 0)
					name = name + namepair[name];

				boost::filesystem::create_directories(path / defdir);

				OOX::IFileBuilder* fileBuilder = dynamic_cast<OOX::IFileBuilder*>(pair.second.get());
				
				if ( fileBuilder != NULL )
				{
					fileBuilder->Commit(path / defdir / name);
				}
			}
		}
	}


	void IFileContainer::Finalize(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content)
	{
		OOX::Rels::File rels;
		const boost::filesystem::wpath current = filename.parent_path();
		Finalize(rels, current, directory, content);
		rels.write(filename);
	}

	
	void IFileContainer::Finalize(Rels::File& rels, const boost::filesystem::wpath& curdir, const boost::filesystem::wpath& directory, ContentTypes::File& content)
	{
		std::map<boost::filesystem::wpath, size_t> namepair;
		BOOST_FOREACH(const RIdFilePair& pair, m_container)
		{
			if (boost::shared_dynamic_cast<External, OOX::File>(pair.second) == 0)
			{
				const boost::filesystem::wpath defdir = pair.second->DefaultDirectory();
				boost::filesystem::wpath name = pair.second->DefaultFileName();
				if (namepair[name]++ != 0)
					name = name + namepair[name];

				boost::filesystem::create_directories(curdir / defdir);

				OOX::IFileBuilder* fileBuilder = dynamic_cast<OOX::IFileBuilder*>(pair.second.get());

				if ( fileBuilder != NULL )
				{
					fileBuilder->Finalize(curdir / defdir / name, directory / defdir, content);
				}
				else
				{
					pair.second->write(curdir / defdir / name, directory / defdir, content);
				}

				rels.registration(pair.first, pair.second->type(), defdir / name);
			}
			else
			{
				rels.registration(pair.first, boost::shared_dynamic_cast<External, OOX::File>(pair.second));
			}
		}
	}


	void IFileContainer::extractPictures(const boost::filesystem::wpath& path) const
	{
		BOOST_FOREACH(const RIdFilePair& pair, m_container)
		{
			if (dynamic_cast<const Image*>(pair.second.get()) != 0)
			{
				const Image& image = static_cast<const Image&>(*pair.second);
				image.copy_to(path);
			}
			else if (dynamic_cast<const IFileContainer*>(pair.second.get()) != 0)
			{
				const IFileContainer& container = dynamic_cast<const IFileContainer&>(*pair.second);
				container.extractPictures(path);
			}
		}
	}


	Image& IFileContainer::image(const RId rId)
	{
        // pog: no-const by th use of const
        return const_cast<Image&>( static_cast<const IFileContainer*>(this)->image(rId) ); 
		//return dynamic_cast<Image&>(*m_container[rId]);
	}


	const Image& IFileContainer::image(const RId rId) const
	{
        if (m_container.count(rId))
        {
            
            if (OOX::File * file = m_container.find(rId)->second.get())
            {
                if (const Image * image = dynamic_cast<const Image*>(file))
                    return *image;                
                else
                    throw std::runtime_error("invalid rel type");
            
            }
            else
                throw std::runtime_error("invalid rel");
        }
        else
            throw std::runtime_error("invalid rId");
	}



	HyperLink& IFileContainer::hyperlink(const RId rId)
	{
		return dynamic_cast<HyperLink&>(*m_container[rId]);
	}


	const HyperLink& IFileContainer::hyperlink(const RId rId) const
	{
		return dynamic_cast<const HyperLink&>(*m_container.find(rId)->second);
	}


	OleObject& IFileContainer::oleObject(const RId rId)
	{
		return dynamic_cast<OleObject&>(*m_container[rId]);
	}


	const OleObject& IFileContainer::oleObject(const RId rId) const
	{
		return dynamic_cast<const OleObject&>(*m_container.find(rId)->second);
	}


	const bool IFileContainer::exist(const FileType& type) const
	{
		BOOST_FOREACH(const RIdFilePair& pair, m_container)
		{
			if (pair.second->type() == type)
				return true;
		}
		return false;
	}


	const bool IFileContainer::exist(const RId& rId) const
	{
		return m_container.find(rId) != m_container.end();
	}


	const bool IFileContainer::isExternal(const OOX::RId& rId) const
	{
		if(m_container.find(rId) != m_container.end())
		{
			std::string type = m_container.find(rId)->second->type().RelationType();
			std::wstring name = m_container.find(rId)->second->type().DefaultFileName().string();
			return (((type == OOX::FileTypes::ExternalAudio.RelationType()) || (type == OOX::FileTypes::ExternalImage.RelationType())
				|| (type == OOX::FileTypes::ExternalVideo.RelationType())) && (name == L""));
		}
		return true;
	}


	const boost::shared_ptr<OOX::File> IFileContainer::get(const FileType& type) const
	{
		BOOST_FOREACH(const RIdFilePair& pair, m_container)
		{
			if (pair.second->type() == type)
				return pair.second;
		}
		return boost::shared_ptr<OOX::File>(new UnknowTypeFile(unknow));
	}


	const RId IFileContainer::add(const boost::shared_ptr<OOX::File>& file)
	{
		const RId rId = maxRId().next();
		add(rId, file);
		return rId;
	}


	void IFileContainer::add(const OOX::RId rId, const boost::shared_ptr<OOX::File>& file)
	{
		IDictonary::add(rId, file);
	}


	File& IFileContainer::find(const FileType& type)
	{
		BOOST_FOREACH(const RIdFilePair& pair, m_container)
		{
			if (pair.second->type() == type)
				return *pair.second;
		}
		return unknow;
	}


	const File& IFileContainer::find(const FileType& type) const
	{
		BOOST_FOREACH(const RIdFilePair& pair, m_container)
		{
			if (pair.second->type() == type)
				return *pair.second;
		}
		return unknow;
	}


	boost::shared_ptr<OOX::File> IFileContainer::operator [](const OOX::RId rId)
	{
		return m_container[rId];
	}


	const boost::shared_ptr<OOX::File> IFileContainer::operator [](const OOX::RId rId) const
	{
		return m_container.find(rId)->second;
	}


	File& IFileContainer::operator [](const FileType& type)
	{
		return find(type);
	}


	const File& IFileContainer::operator [](const FileType& type) const
	{
		return find(type);
	}


	const RId IFileContainer::maxRId() const
	{
		RId rId;
		BOOST_FOREACH(const RIdFilePair& pair, m_container)
		{
			if (rId < pair.first)
				rId = pair.first;
		}
		return rId;
	}

} // namespace OOX