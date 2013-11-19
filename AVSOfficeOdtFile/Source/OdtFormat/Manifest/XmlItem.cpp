
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "XmlItem.h"
#include "TypeTable.h"
#include "exception\not_implement.h"


namespace Odt
{ 
	namespace Manifest
	{

		XmlItem::XmlItem()
		{
		}


		XmlItem::XmlItem(const boost::filesystem::wpath& filename)
			: m_filename(filename)
		{
		}


		XmlItem::~XmlItem()
		{
		}


		XmlItem::XmlItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const XmlItem& XmlItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void XmlItem::fromXML(const XML::XNode& node)
		{
			throw not_implement("XmlItem fromXML");
		}


		const XML::XNode XmlItem::toXML() const
		{
			static const TypeTable table;
			return
				XML::XElement(ns.manifest + "file-entry",
					XML::XAttribute(ns.manifest + "media-type", table[m_filename]) + 
					XML::XAttribute(ns.manifest + "full-path", m_filename)
				);
		}

	} // namespace Manifest
} // namespace Odt