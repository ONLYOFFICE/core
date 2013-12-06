
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


	ItemProps::ItemProps(const OOX::CPath& filename)
	{
		read(filename);
	}


	ItemProps::~ItemProps()
	{
	}


	void ItemProps::read(const OOX::CPath& filename)
	{
		const	XML::XDocument document(filename);
		//Refs		= document.Root.element("schemaRefs");
		ItemID	= document.Root.attribute("itemID").value();
	}


	void ItemProps::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
		//XML::XElement(ns.ds + "datastoreItem",
		//	XML::XAttribute(ns.ds + "itemID", ItemID) + 
		//	XML::Write(Refs)
		//).Save(filename);

		//content.registration(type().OverrideType(), directory, filename);
	}


	const FileType ItemProps::type() const
	{
		return FileTypes::ItemProp;
	}


	const OOX::CPath ItemProps::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const OOX::CPath ItemProps::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX