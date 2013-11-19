#pragma once
#ifndef ODT_MANIFEST_XML_ITEM_INCLUDE_H_
#define ODT_MANIFEST_XML_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include <boost_filesystem_version.h>


namespace Odt
{
	namespace Manifest
	{
		class XmlItem : public WritingElement
		{
		public:
			XmlItem();
			XmlItem(const boost::filesystem::wpath& filename);
			virtual ~XmlItem();
			explicit XmlItem(const XML::XNode& node);
			const XmlItem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			boost::filesystem::wpath m_filename;
		};
	} // namespace Manifest
} // namespace Odt

#endif // ODT_MANIFEST_XML_ITEM_INCLUDE_H_