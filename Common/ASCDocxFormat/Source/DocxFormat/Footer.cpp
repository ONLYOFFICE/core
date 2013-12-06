
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

	Footer::Footer(const OOX::CPath& filename)
	{
		read(filename);
	}

	Footer::~Footer()
	{
	}		

	void Footer::read(const OOX::CPath& filename)
	{
		IFileContainer::read(filename);
		//
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

		const XML::XDocument document(filename.GetPath());
		XML::Fill(m_items, document.Root, "p", "tbl", "sdt");
	}

	void Footer::write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
	{
	}

	const FileType Footer::type() const
	{
		return FileTypes::Footer;
	}

	const OOX::CPath Footer::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const OOX::CPath Footer::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX