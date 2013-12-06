
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ExternalAudio.h"
#include "..\FileTypes.h"


namespace OOX
{

	ExternalAudio::ExternalAudio()
	{
	}

	
	ExternalAudio::ExternalAudio(const OOX::CPath& uri)
	{
		read(uri);
	}


	ExternalAudio::~ExternalAudio()
	{
	}


	const FileType ExternalAudio::type() const
	{
		return FileTypes::ExternalAudio;
	}


	const OOX::CPath ExternalAudio::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const OOX::CPath ExternalAudio::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX