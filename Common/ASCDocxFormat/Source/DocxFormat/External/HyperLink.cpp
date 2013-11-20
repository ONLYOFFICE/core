
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "HyperLink.h"
#include "..\FileTypes.h"


namespace OOX
{

	HyperLink::HyperLink()
	{
	}

	
	HyperLink::HyperLink(const boost::filesystem::wpath& uri)
	{
		read(uri);
	}


	HyperLink::~HyperLink()
	{
	}


	const FileType HyperLink::type() const
	{
		return FileTypes::HyperLink;
	}


	const boost::filesystem::wpath HyperLink::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath HyperLink::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX