
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FontTable.h"
#include "FileTypes.h"

namespace OOX
{
	FontTable::FontTable()
	{

	}

	FontTable::FontTable(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}

	FontTable::~FontTable()
	{

	}

	void FontTable::read(const boost::filesystem::wpath& filename)
	{
		const XML::XDocument document(filename);
		XML::Fill(Fonts, document.Root);
	}

	void FontTable::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.w + "fonts", XML::Write(Fonts)).Save(filename);
		content.registration(type().OverrideType(), directory, filename);
	}

	const FileType FontTable::type() const
	{
		return FileTypes::FontTable;
	}

	const boost::filesystem::wpath FontTable::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath FontTable::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX