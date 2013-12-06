
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

} // namespace OOX