
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Image.h"
#include "..\FileTypes.h"

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

} // namespace OOX