#pragma once
#ifndef OOX_IFILE_CONTAINER_INCLUDE_H_
#define OOX_IFILE_CONTAINER_INCLUDE_H_

#include "RId.h"
#include "UnknowTypeFile.h"
#include "IFileBuilder.h"

namespace PPTX {class File;}
namespace PPTX {class FileType;}
namespace PPTX {namespace Rels {class File;}}
namespace PPTX {namespace ContentTypes {class File;}}
namespace PPTX {class Image;}
namespace PPTX {class HyperLink;}
namespace PPTX {class OleObject;}


namespace PPTX
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
        std::map<CString, smart_ptr<PPTX::File>>	m_container;
		size_t m_lMaxRid;

	protected:
		void read(const OOX::CPath& filename);
		void read(const Rels::File& rels, const OOX::CPath& path);
		void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;
		void write(Rels::File& rels, const OOX::CPath& current, const OOX::CPath& directory, ContentTypes::File& content) const;

	protected:
		void Commit(const OOX::CPath& path);
		void Finalize(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content);
		void Finalize(Rels::File& rels, const OOX::CPath& current, const OOX::CPath& directory, ContentTypes::File& content);

	public:
		void extractPictures(const OOX::CPath& path) const;

	public:
		virtual smart_ptr<Image> image(const RId& rId) const;

		virtual smart_ptr<HyperLink> hyperlink(const RId& rId) const;

		virtual smart_ptr<OleObject> oleObject(const RId& rId) const;
	public:
		template<typename T> const bool exist() const;
		const bool exist(const FileType& type) const;
		const bool exist(const PPTX::RId& rId) const;
		const bool isExternal(const PPTX::RId& rId) const;

		smart_ptr<PPTX::File> get(const FileType& type);
		const RId add(const smart_ptr<PPTX::File>& file);
		void add(const PPTX::RId rId, const smart_ptr<PPTX::File>& file);

		smart_ptr<PPTX::File> find(const FileType& type) const;

		smart_ptr<PPTX::File> find(const PPTX::RId& type) const;

		smart_ptr<PPTX::File> operator [](const PPTX::RId rId);

		smart_ptr<PPTX::File> operator [](const FileType& type);

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

} // namespace PPTX

#endif // OOX_IFILE_CONTAINER_INCLUDE_H_
