
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Header.h"
#include "exception\not_implement.h"
#include "FileTypes.h"


namespace OOX
{

	Header::Header()
	{
	}


	Header::Header(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	Header::~Header()
	{
	}


	void Header::read(const boost::filesystem::wpath& filename)
	{
		IFileContainer::read(filename);

		const XML::XDocument document(filename);
		XML::Fill(Items, document.Root, "p", "tbl", "sdt");
	}


	void Header::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.w + "hdr",
			XML::Write(Items)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		IFileContainer::write(filename, directory, content);
	}


	const FileType Header::type() const
	{
		return FileTypes::Header;
	}


	const boost::filesystem::wpath Header::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath Header::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOXFile