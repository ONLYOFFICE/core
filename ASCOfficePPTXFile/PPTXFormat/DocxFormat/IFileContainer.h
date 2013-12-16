#pragma once
#ifndef OOX_IFILE_CONTAINER_INCLUDE_H_
#define OOX_IFILE_CONTAINER_INCLUDE_H_

#include "RId.h"
#include "UnknowTypeFile.h"
#include "IFileBuilder.h"

namespace OOX {class File;}
namespace OOX {class FileType;}
namespace OOX {namespace Rels {class File;}}
namespace OOX {namespace ContentTypes {class File;}}
namespace OOX {class Image;}
namespace OOX {class HyperLink;}
namespace OOX {class OleObject;}


namespace OOX
{
	class IFileContainer
	{
	public:
		IFileContainer()
		{
			m_lMaxRid = 0;
		}
		virtual ~IFileContainer()
		{
		}
	protected:
		CAtlMap<CString, smart_ptr<OOX::File>>	m_container;
		size_t m_lMaxRid;

	protected:
		void read(const OOX::CPath& filename);
		void read(const Rels::File& rels, const CPath& path);
		void write(const CPath& filename, const CPath& directory, ContentTypes::File& content) const;
		void write(Rels::File& rels, const CPath& current, const CPath& directory, ContentTypes::File& content) const;

	protected:
		void Commit(const CPath& path);
		void Finalize(const CPath& filename, const CPath& directory, ContentTypes::File& content);
		void Finalize(Rels::File& rels, const CPath& current, const CPath& directory, ContentTypes::File& content);

	public:
		void extractPictures(const CPath& path) const;

	public:
		virtual smart_ptr<Image> image(const RId& rId) const;

		virtual smart_ptr<HyperLink> hyperlink(const RId& rId) const;

		virtual smart_ptr<OleObject> oleObject(const RId& rId) const;
	public:
		template<typename T> const bool exist() const;
		const bool exist(const FileType& type) const;
		const bool exist(const OOX::RId& rId) const;
		const bool isExternal(const OOX::RId& rId) const;

		smart_ptr<OOX::File> get(const FileType& type);
		const RId add(const smart_ptr<OOX::File>& file);
		void add(const OOX::RId rId, const smart_ptr<OOX::File>& file);

		smart_ptr<OOX::File> find(const FileType& type) const;

		smart_ptr<OOX::File> find(const OOX::RId& type) const;

		smart_ptr<OOX::File> operator [](const OOX::RId rId);

		smart_ptr<OOX::File> operator [](const FileType& type);

		template<typename T> T& find();

	protected:
		static UnknowTypeFile unknow;

	private:
		const RId maxRId();
	};


	template<typename T>
	const bool IFileContainer::exist() const
	{
		T file;
		return exist(file.type());
	}

	template<typename T>
	T& IFileContainer::find()
	{
		T file;
		return dynamic_cast<T&>(find(file.type()));
	}

} // namespace OOX

#endif // OOX_IFILE_CONTAINER_INCLUDE_H_