
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "WebSettings.h"
#include "FileTypes.h"

namespace OOX
{
	WebSettings::WebSettings()
	{
	}

	WebSettings::WebSettings(const OOX::CPath& filename)
	{
		read(filename);
	}

	WebSettings::~WebSettings()
	{
	}

	void WebSettings::read(const OOX::CPath& filename)
	{
	}

	void WebSettings::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType WebSettings::type() const
	{
		return FileTypes::WebSetting;
	}

	const OOX::CPath WebSettings::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath WebSettings::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX