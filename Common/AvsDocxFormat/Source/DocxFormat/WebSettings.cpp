
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


	WebSettings::WebSettings(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	WebSettings::~WebSettings()
	{
	}


	void WebSettings::read(const boost::filesystem::wpath& filename)
	{
	}


	void WebSettings::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.w + "webSettings",
			XML::XElement(ns.w + "optimizeForBrowser")
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
	}


	const FileType WebSettings::type() const
	{
		return FileTypes::WebSetting;
	}


	const boost::filesystem::wpath WebSettings::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath WebSettings::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX