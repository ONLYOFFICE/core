
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Media.h"
#include "..\FileTypes.h"

namespace OOX
{
	Media::Media()
	{
	}

	Media::Media(const OOX::CPath& filename)
	{
		read(filename);
	}

	Media::~Media()
	{
	}

	void Media::read(const OOX::CPath& filename)
	{
		m_filename = filename;
	}

	void Media::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const OOX::CPath Media::filename() const
	{
		return m_filename;
	}

	void Media::copy_to(const OOX::CPath& path) const
	{		
		int c = 0;
	}

} 

namespace OOX
{
	Image::Image()
	{
	}

	Image::Image(const OOX::CPath& filename)
	{
		read(filename);
	}

	Image::~Image()
	{
	}

	void Image::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType Image::type() const
	{
		return FileTypes::Image;
	}

	const OOX::CPath Image::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath Image::DefaultFileName() const
	{
		return m_filename;
	}
}

namespace OOX
{
	Video::Video()
	{
	}

	Video::Video(const OOX::CPath& filename)
	{
		read(filename);
	}

	Video::~Video()
	{
	}

	const FileType Video::type() const
	{
		return FileTypes::Video;
	}

	const OOX::CPath Video::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath Video::DefaultFileName() const
	{
		return m_filename;
	}
}

namespace OOX
{
	Audio::Audio()
	{
	}

	Audio::Audio(const OOX::CPath& filename)
	{
		read(filename);
	}

	Audio::~Audio()
	{
	}

	const FileType Audio::type() const
	{
		return FileTypes::Audio;
	}

	const OOX::CPath Audio::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath Audio::DefaultFileName() const
	{
		return m_filename.GetPath();
	}
}

namespace OOX
{
	OleObject::OleObject()
	{
	}

	OleObject::OleObject(const OOX::CPath& filename)
	{
		read(filename);
	}

	OleObject::~OleObject()
	{
	}

	void OleObject::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType OleObject::type() const
	{
		return FileTypes::OleObject;
	}

	const OOX::CPath OleObject::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath OleObject::DefaultFileName() const
	{
		return m_filename;
	}
}
