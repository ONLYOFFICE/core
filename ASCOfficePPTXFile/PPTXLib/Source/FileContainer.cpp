#include "FileContainer.h"
#include <boost/foreach.hpp>
#include "DocxFormat/Rels/File.h"
#include "FileFactory.h"
#include "DocxFormat/ContentTypes/File.h"
#include "DocxFormat/FileType.h"
#include "DocxFormat/External/External.h"
#include "FileNameUtility.h"
#include "Exception\not_implement.h"
#include "WrapperFile.h"

namespace PPTX
{
	void FileContainer::read(const boost::filesystem::wpath& filename)
	{
		throw not_implement("not implement FileContainer.read");
	}

	void FileContainer::read(const OOX::Rels::File& rels, const boost::filesystem::wpath& path)
	{
		throw not_implement("not implement FileContainer.read");
	}

	void FileContainer::read(const boost::filesystem::wpath& filename, FileMap& map, IPPTXEvent* Event)
	{
		OOX::Rels::File rels(filename);
		const boost::filesystem::wpath path = filename.parent_path();
		read(rels, path, map, Event);
	}

	void FileContainer::read(const OOX::Rels::File& rels, const boost::filesystem::wpath& path, FileMap& map, IPPTXEvent* Event)
	{
		BOOST_FOREACH(const OOX::Rels::RelationShip& relation, *rels.Relations)
		{
			boost::filesystem::wpath normPath = path/relation.target();
			normPath.normalize();
			if(map.find(normPath) != map.end())
			{
				add(relation.rId(), map.find(normPath)->second);
			}
			else
			{
				long percent = Event->GetPercent();
				boost::shared_ptr<OOX::File> file = PPTX::FileFactory::CreateFilePPTX(path, relation, map);
				bool res = Event->Progress(0, percent + m_lPercent);
				if(res)
				{
					m_bCancelled = true;
					break;
				}
				map.add(normPath, file);
				add(relation.rId(), file);
				if(boost::shared_dynamic_cast<FileContainer, OOX::File>(file) != 0)
				{
					boost::shared_dynamic_cast<FileContainer, OOX::File>(file)->m_lPercent = m_lPercent;
					Event->AddPercent(m_lPercent);
					boost::shared_dynamic_cast<FileContainer, OOX::File>(file)->read(normPath, map, Event);
					m_bCancelled = boost::shared_dynamic_cast<FileContainer, OOX::File>(file)->m_bCancelled;
				}
			}
			//add(relation.rId(), PPTX::CreateFile(path, relation));
		}
	}

	void FileContainer::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content) const
	{
		OOX::Rels::File rels;
		const boost::filesystem::wpath current = filename.parent_path();
		write(rels, current, directory, content);
		rels.write(filename);
	}

	void FileContainer::write(OOX::Rels::File& rels, const boost::filesystem::wpath& curdir, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content) const
	{
		std::map<boost::filesystem::wpath, size_t> namepair;
		BOOST_FOREACH(const RIdFilePair& pair, m_container)
		{
			if (boost::shared_dynamic_cast<OOX::External, OOX::File>(pair.second) == 0)
			{
				boost::shared_ptr<PPTX::WrapperFile> file = boost::shared_dynamic_cast<PPTX::WrapperFile, OOX::File>(pair.second);
				if(file != 0)
				{
					if(file->GetWrittenStatus() == false)
					{
						const boost::filesystem::wpath defdir = pair.second->DefaultDirectory();
						boost::filesystem::wpath name = pair.second->DefaultFileName();

						name = name + max_name_index(curdir, name.string());

						boost::filesystem::create_directories(curdir / defdir);
						pair.second->write(curdir / defdir / name, directory / defdir, content);
						rels.registration(pair.first, pair.second->type(), defdir / name);
					}
					else
					{
						const boost::filesystem::wpath defdir = pair.second->DefaultDirectory();
						boost::filesystem::wpath name = file->GetWrittenFileName();

						rels.registration(pair.first, pair.second->type(), defdir / name);
					}
				}
				else
				{
					const boost::filesystem::wpath defdir = pair.second->DefaultDirectory();
					boost::filesystem::wpath name = pair.second->DefaultFileName();

					boost::filesystem::create_directories(curdir / defdir);
					pair.second->write(curdir / defdir / name, directory / defdir, content);
					rels.registration(pair.first, pair.second->type(), defdir / name);
				}
			}
			else
			{
//				const OOX::HyperLink hyperlink = static_cast<const OOX::HyperLink&>(*pair.second);
				rels.registration(pair.first, boost::shared_dynamic_cast<OOX::External, OOX::File>(pair.second));
			}
		}
	}

	void FileContainer::WrittenSetFalse()
	{
		BOOST_FOREACH(RIdFilePair& pair, m_container)
		{
			if(boost::shared_dynamic_cast<PPTX::WrapperFile, OOX::File>(pair.second) != 0)
			{
				if(boost::shared_dynamic_cast<PPTX::WrapperFile, OOX::File>(pair.second)->GetWrittenStatus())
				{
					(boost::shared_dynamic_cast<PPTX::WrapperFile, OOX::File>(pair.second))->WrittenSetFalse();
					if(boost::shared_dynamic_cast<PPTX::FileContainer, OOX::File>(pair.second) != 0)
						(boost::shared_dynamic_cast<PPTX::FileContainer, OOX::File>(pair.second))->WrittenSetFalse();
				}
			}
		}
	}
} // namespace PPTX