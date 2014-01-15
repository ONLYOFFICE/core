
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Video.h"
#include "..\FileTypes.h"


namespace OOX
{

	Video::Video()
	{
	}


	Video::Video(const boost::filesystem::wpath& filename)
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


	const boost::filesystem::wpath Video::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath Video::DefaultFileName() const
	{
		return m_filename.filename();
	}

} // namespace OOX