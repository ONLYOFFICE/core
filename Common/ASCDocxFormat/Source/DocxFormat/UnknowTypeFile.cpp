
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "UnknowTypeFile.h"
#include "FileTypes.h"

namespace OOX
{
	UnknowTypeFile::UnknowTypeFile()
	{
	}

	UnknowTypeFile::~UnknowTypeFile()
	{
	}

	void UnknowTypeFile::read(const OOX::CPath& filename)
	{
	}

	void UnknowTypeFile::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType UnknowTypeFile::type() const
	{
		return FileTypes::Unknow;
	}

	const OOX::CPath UnknowTypeFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath UnknowTypeFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX