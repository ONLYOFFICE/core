#pragma once
#ifndef ODT_MANIFEST_XML_TABLE_INCLUDE_H_
#define ODT_MANIFEST_XML_TABLE_INCLUDE_H_

#include "BaseTable.h"
#include "XmlItem.h"


namespace Odt
{
	namespace Manifest
	{
		class XmlTable : public BaseTable<XmlItem>
		{
		public:
			XmlTable();
			virtual ~XmlTable();
			explicit XmlTable(const XML::XNode& node);
			const XmlTable& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			void add(const boost::filesystem::wpath& path, const boost::filesystem::wpath& filename);
		};
	} // namespace Manifest
} // namespace Odt

#endif // ODT_MANIFEST_XML_TABLE_INCLUDE_H_