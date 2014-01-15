
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ItemProps.h"
#include "FileTypes.h"


namespace OOX
{

	ItemProps::ItemProps()
	{
	}


	ItemProps::ItemProps(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	ItemProps::~ItemProps()
	{
	}


	void ItemProps::read(const boost::filesystem::wpath& filename)
	{
		const	XML::XDocument document(filename);
		Refs		= document.Root.element("schemaRefs");
		ItemID	= document.Root.attribute("itemID").value();
	}


	void ItemProps::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.ds + "datastoreItem",
			XML::XAttribute(ns.ds + "itemID", ItemID) + 
			XML::Write(Refs)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
	}


	const FileType ItemProps::type() const
	{
		return FileTypes::ItemProp;
	}


	const boost::filesystem::wpath ItemProps::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath ItemProps::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX