
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

	
	ExternalAudio::ExternalAudio(const boost::filesystem::wpath& uri)
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


	const boost::filesystem::wpath ExternalAudio::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath ExternalAudio::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX