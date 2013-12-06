
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "IFileContainer.h"
#include "Rels/File.h"
#include "FileFactory.h"
#include "ContentTypes/File.h"
#include "FileType.h"
#include "External/External.h"
#include "External/HyperLink.h"
#include "Media/Media.h"
#include "FileTypes.h"

namespace OOX
{
	UnknowTypeFile IFileContainer::unknow;

	void IFileContainer::read(const OOX::CPath& filename)
	{
		OOX::Rels::File rels(filename);
		read(rels, OOX::CPath(filename.GetDirectory()));
	}

	void IFileContainer::read(const Rels::File& rels, const OOX::CPath& path)
	{
#ifndef NODOCX
		if (rels.Relations.IsInit())
		{
			for (std::vector<OOX::Rels::RelationShip>::const_iterator iter = rels.Relations.get().m_items.begin();
				iter != rels.Relations.get().m_items.end();
				++iter)
			{
				add((*iter).rId(), OOX::CreateFile(path, (*iter)));
			}
		}
#endif
	}

	void IFileContainer::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
		OOX::Rels::File rels;
		const OOX::CPath current = filename.GetDirectory();
		write(rels, current, directory, content);
		rels.write(filename);
	}

	void IFileContainer::write(Rels::File& rels, const OOX::CPath& curdir, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	void IFileContainer::Commit(const OOX::CPath& path)
	{		
	}

	void IFileContainer::Finalize(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content)
	{
	}
	
	void IFileContainer::Finalize(Rels::File& rels, const OOX::CPath& curdir, const OOX::CPath& directory, ContentTypes::File& content)
	{
	}

	void IFileContainer::extractPictures(const OOX::CPath& path) const
	{
		for (std::map<OOX::RId, NSCommon::smart_ptr<OOX::File>>::const_iterator iter = m_container.begin(); iter != m_container.end(); ++iter)
		{
			if (dynamic_cast<const Image*>((*iter).second.operator->()) != 0)
			{
				const Image& image = static_cast<const Image&>(*(*iter).second);
				image.copy_to(path);
			}
			else if (dynamic_cast<const IFileContainer*>((*iter).second.operator->()) != 0)
			{
				const IFileContainer& container = dynamic_cast<const IFileContainer&>(*(*iter).second);
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
			if (OOX::File* file = ((OOX::File*)m_container.find(rId)->second.operator->()))
			{
				if (const Image * image = dynamic_cast<const Image*>(file))
					return *image;                
				else
					throw std::runtime_error("invalid rel type");
			}
			else
			{
				throw std::runtime_error("invalid rel");
			}
		}
		else
		{
			throw std::runtime_error("invalid rId");
		}
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
		for (std::map<OOX::RId, NSCommon::smart_ptr<OOX::File>>::const_iterator iter = m_container.begin(); iter != m_container.end(); ++iter)
		{
			if ((*iter).second->type() == type)
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
			std::wstring type = m_container.find(rId)->second->type().RelationType();
			std::wstring name = m_container.find(rId)->second->type().DefaultFileName().GetPath();
			return (((type == OOX::FileTypes::ExternalAudio.RelationType()) || (type == OOX::FileTypes::ExternalImage.RelationType())
				|| (type == OOX::FileTypes::ExternalVideo.RelationType())) && (name == L""));
		}
		return true;
	}

	const NSCommon::smart_ptr<OOX::File> IFileContainer::get(const FileType& type) const
	{
		for (std::map<OOX::RId, NSCommon::smart_ptr<OOX::File>>::const_iterator iter = m_container.begin(); iter != m_container.end(); ++iter)
		{
			if ((*iter).second->type() == type)
				return (*iter).second;
		}
		return NSCommon::smart_ptr<OOX::File>(new UnknowTypeFile(unknow));
	}

	const RId IFileContainer::add(const NSCommon::smart_ptr<OOX::File>& file)
	{
		const RId rId = maxRId().next();
		add(rId, file);
		return rId;
	}

	void IFileContainer::add(const OOX::RId rId, const NSCommon::smart_ptr<OOX::File>& file)
	{
		IDictonary::add(rId, file);
	}

	File& IFileContainer::find(const FileType& type)
	{
		for (std::map<OOX::RId, NSCommon::smart_ptr<OOX::File>>::const_iterator iter = m_container.begin(); iter != m_container.end(); ++iter)
		{
			if ((*iter).second->type() == type)
				return *((OOX::File*)(*iter).second.operator->());
		}
		return unknow;
	}

	const File& IFileContainer::find(const FileType& type) const
	{
		for (std::map<OOX::RId, NSCommon::smart_ptr<OOX::File>>::const_iterator iter = m_container.begin(); iter != m_container.end(); ++iter)
		{
			if ((*iter).second->type() == type)
				return *(*iter).second.operator->();
		}
		return unknow;
	}

	NSCommon::smart_ptr<OOX::File> IFileContainer::operator [](const OOX::RId rId)
	{
		return m_container[rId];
	}

	const NSCommon::smart_ptr<OOX::File> IFileContainer::operator [](const OOX::RId rId) const
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
		for (std::map<OOX::RId, NSCommon::smart_ptr<OOX::File>>::const_iterator iter = m_container.begin(); iter != m_container.end(); ++iter)
		{
			if (rId < (*iter).first)
				rId = (*iter).first;
		}
		return rId;
	}

} // namespace OOX