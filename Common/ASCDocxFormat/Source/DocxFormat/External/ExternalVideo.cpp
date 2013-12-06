
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ExternalVideo.h"
#include "..\FileTypes.h"


namespace OOX
{

	ExternalVideo::ExternalVideo()
	{
	}

	
	ExternalVideo::ExternalVideo(const OOX::CPath& uri)
	{
		read(uri);
	}


	ExternalVideo::~ExternalVideo()
	{
	}


	const FileType ExternalVideo::type() const
	{
		return FileTypes::ExternalVideo;
	}


	const OOX::CPath ExternalVideo::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const OOX::CPath ExternalVideo::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX