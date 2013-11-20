#pragma once
#ifndef OOX_IFILE_CONTAINER_INCLUDE_H_
#define OOX_IFILE_CONTAINER_INCLUDE_H_

#include "IDictonary.h"
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
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
	class IFileContainer : public IDictonary<OOX::RId, boost::shared_ptr<OOX::File> >
	{
	protected:
		void read(const boost::filesystem::wpath& filename);
		void read(const Rels::File& rels, const boost::filesystem::wpath& path);
		void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;
		void write(Rels::File& rels, const boost::filesystem::wpath& current, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	protected:
		void Commit(const boost::filesystem::wpath& path);
		void Finalize(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content);
		void Finalize(Rels::File& rels, const boost::filesystem::wpath& current, const boost::filesystem::wpath& directory, ContentTypes::File& content);

	public:
		typedef std::pair<const OOX::RId, boost::shared_ptr<OOX::File> > RIdFilePair;

	public:
		void extractPictures(const boost::filesystem::wpath& path) const;

	public:
		Image& image(const RId rId);
		const Image& image(const RId rId) const;

		HyperLink& hyperlink(const RId rId);
		const HyperLink& hyperlink(const RId rId) const;

		OleObject& oleObject(const RId rId);
		const OleObject& oleObject(const RId rId) const;

	public:
		template<typename T> const bool exist() const;
		const bool exist(const FileType& type) const;
		const bool exist(const OOX::RId& rId) const;
		const bool isExternal(const OOX::RId& rId) const;

		const boost::shared_ptr<OOX::File> get(const FileType& type) const;
		const RId add(const boost::shared_ptr<OOX::File>& file);
		void add(const OOX::RId rId, const boost::shared_ptr<OOX::File>& file);

		File& find(const FileType& type);
		const File& find(const FileType& type) const;

		boost::shared_ptr<OOX::File> operator [](const OOX::RId rId);
		const boost::shared_ptr<OOX::File> operator [](const OOX::RId rId) const;

		File& operator [](const FileType& type);
		const File& operator [](const FileType& type) const;

	 template<typename T> T& find();
	 template<typename T> const T& find() const;

	protected:
		static UnknowTypeFile unknow;

	private:
		const RId maxRId() const;
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

	template<typename T>
	const T& IFileContainer::find() const
	{
		T file;
		return dynamic_cast<const T&>(find(file.type()));
	}

} // namespace OOX

#endif // OOX_IFILE_CONTAINER_INCLUDE_H_