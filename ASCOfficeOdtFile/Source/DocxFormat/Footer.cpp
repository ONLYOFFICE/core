
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Footer.h"
#include "exception\not_implement.h"
#include "FileTypes.h"


namespace OOX
{

	Footer::Footer()
	{
	}


	Footer::Footer(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	Footer::~Footer()
	{
	}		


	void Footer::read(const boost::filesystem::wpath& filename)
	{
		IFileContainer::read(filename);

		const XML::XDocument document(filename);
		XML::Fill(Items, document.Root, "p", "tbl", "sdt");
	}


	void Footer::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.w + "ftr",
			XML::Write(Items)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		IFileContainer::write(filename, directory, content);
	}


	const FileType Footer::type() const
	{
		return FileTypes::Footer;
	}


	const boost::filesystem::wpath Footer::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath Footer::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX