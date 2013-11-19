
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ExternalImage.h"
#include "..\FileTypes.h"


namespace OOX
{

	ExternalImage::ExternalImage()
	{
	}

	
	ExternalImage::ExternalImage(const boost::filesystem::wpath& uri)
	{
		read(uri);
	}


	ExternalImage::~ExternalImage()
	{
	}


	const FileType ExternalImage::type() const
	{
		return FileTypes::ExternalImage;
	}


	const boost::filesystem::wpath ExternalImage::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath ExternalImage::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX