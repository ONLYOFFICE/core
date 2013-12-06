
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


	Item::Item(const OOX::CPath& filename)
	{
		read(filename);
	}


	Item::~Item()
	{
	}


	void Item::read(const OOX::CPath& filename)
	{
		const XML::XDocument document(filename);
		SelectedStyle = document.Root.attribute("SelectedStyle").value();
		StyleName			= document.Root.attribute("StyleName").value();
	}


	void Item::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
		////////XML::XElement(ns.b + "Sources",
		////////	XML::XAttribute("SelectedStyle", SelectedStyle) +
		////////	XML::XAttribute("StyleName", StyleName)
		////////).Save(filename);

		////////content.registration(type().OverrideType(), directory, filename);
	}


	const FileType Item::type() const
	{
		return FileTypes::Item;
	}


	const OOX::CPath Item::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const OOX::CPath Item::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX