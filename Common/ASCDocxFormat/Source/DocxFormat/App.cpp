
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "App.h"
#include "FileTypes.h"

namespace OOX
{
	App::App()
	{
	}

	App::App(const OOX::CPath& filename)
	{
		read(filename);
	}

	App::~App()
	{
	}

	void App::read(const OOX::CPath& filename)
	{
	}
	
	void App::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType App::type() const
	{
		return FileTypes::App;
	}

	const OOX::CPath App::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath App::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX