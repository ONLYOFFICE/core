
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


	void UnknowTypeFile::read(const boost::filesystem::wpath& filename)
	{
	}


	void UnknowTypeFile::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
	}


	const FileType UnknowTypeFile::type() const
	{
		return FileTypes::Unknow;
	}


	const boost::filesystem::wpath UnknowTypeFile::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath UnknowTypeFile::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX