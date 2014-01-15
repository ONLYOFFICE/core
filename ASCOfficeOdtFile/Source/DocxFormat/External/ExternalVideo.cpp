
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

	
	ExternalVideo::ExternalVideo(const boost::filesystem::wpath& uri)
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


	const boost::filesystem::wpath ExternalVideo::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath ExternalVideo::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX