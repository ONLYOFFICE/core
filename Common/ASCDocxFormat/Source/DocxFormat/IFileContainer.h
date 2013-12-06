#pragma once
#ifndef OOX_IFILE_CONTAINER_INCLUDE_H_
#define OOX_IFILE_CONTAINER_INCLUDE_H_

#include "IDictonary.h"
#include "RId.h"
#include "UnknowTypeFile.h"
#include "IFileBuilder.h"

#include "../../../../Common/DocxFormat/Source/Base/SmartPtr.h"

namespace OOX {class File;}
namespace OOX {class FileType;}
namespace OOX {namespace Rels {class File;}}
namespace OOX {namespace ContentTypes {class File;}}
namespace OOX {class Image;}
namespace OOX {class HyperLink;}
namespace OOX {class OleObject;}

namespace OOX
{
	class IFileContainer : public IDictonary<OOX::RId, NSCommon::smart_ptr<OOX::File> >
	{
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
		typedef std::pair<const OOX::RId, NSCommon::smart_ptr<OOX::File> > RIdFilePair;

	public:
		void extractPictures(const OOX::CPath& path) const;

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

		const NSCommon::smart_ptr<OOX::File> get(const FileType& type) const;
		const RId add(const NSCommon::smart_ptr<OOX::File>& file);
		void add(const OOX::RId rId, const NSCommon::smart_ptr<OOX::File>& file);

		File& find(const FileType& type);
		const File& find(const FileType& type) const;

		NSCommon::smart_ptr<OOX::File> operator [](const OOX::RId rId);
		const NSCommon::smart_ptr<OOX::File> operator [](const OOX::RId rId) const;

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