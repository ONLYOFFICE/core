
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

	
	ExternalImage::ExternalImage(const OOX::CPath& uri)
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


	const OOX::CPath ExternalImage::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const OOX::CPath ExternalImage::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX