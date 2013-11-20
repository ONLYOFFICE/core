
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Item.h"
#include "FileTypes.h"


namespace OOX
{

	Item::Item()
	{
	}


	Item::Item(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	Item::~Item()
	{
	}


	void Item::read(const boost::filesystem::wpath& filename)
	{
		const XML::XDocument document(filename);
		SelectedStyle = document.Root.attribute("SelectedStyle").value();
		StyleName			= document.Root.attribute("StyleName").value();
	}


	void Item::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.b + "Sources",
			XML::XAttribute("SelectedStyle", SelectedStyle) +
			XML::XAttribute("StyleName", StyleName)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
	}


	const FileType Item::type() const
	{
		return FileTypes::Item;
	}


	const boost::filesystem::wpath Item::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath Item::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX