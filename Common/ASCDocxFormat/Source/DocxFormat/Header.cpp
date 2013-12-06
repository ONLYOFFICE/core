
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

	Header::Header(const OOX::CPath& filename)
	{
		read(filename);
	}

	Header::~Header()
	{
	}

	void Header::read(const OOX::CPath& filename)
	{
		IFileContainer::read(filename);

		//XmlUtils::CXmlNode oMainNode;
		//oMainNode.FromXmlFile(filename.GetPath(), true);

		//XmlUtils::CXmlNodes oChilds;
		//if ( oMainNode.GetNodes( _T("*"), oChilds ) )
		//{
		//	XmlUtils::CXmlNode oItem;
		//	for ( int nIndex = 0; nIndex < oChilds.GetCount(); ++nIndex )
		//	{
		//		if ( oChilds.GetAt( nIndex, oItem ) )
		//		{
		//			CString sName = oItem.GetName();

		//			Logic::TextItem item;

		//			if ( _T("w:p") == sName )
		//				item.fromXML(oItem);
		//			else if ( _T("w:sdt") == sName )
		//				item.fromXML(oItem);
		//			else if ( _T("w:tbl") == sName )
		//				item.fromXML(oItem);
		//			
		//			m_items.push_back(item);
		//		}
		//	}
		//}

		const XML::XDocument document(filename);
		XML::Fill(m_items, document.Root, "p", "tbl", "sdt");
	}

	void Header::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType Header::type() const
	{
		return FileTypes::Header;
	}

	const OOX::CPath Header::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath Header::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOXFile