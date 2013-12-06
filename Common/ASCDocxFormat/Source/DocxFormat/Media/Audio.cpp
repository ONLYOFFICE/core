
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

} // namespace OOX