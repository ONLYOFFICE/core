
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Audio.h"
#include "..\FileTypes.h"


namespace OOX
{

	Audio::Audio()
	{
	}


	Audio::Audio(const boost::filesystem::wpath& filename)
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


	const boost::filesystem::wpath Audio::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath Audio::DefaultFileName() const
	{
		return m_filename.filename();
	}

} // namespace OOX