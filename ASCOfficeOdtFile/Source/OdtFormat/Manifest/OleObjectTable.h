#pragma once
#ifndef ODT_MANIFEST_OLE_OBJECT_TABLE_INCLUDE_H_
#define ODT_MANIFEST_OLE_OBJECT_TABLE_INCLUDE_H_

#include "BaseTable.h"
#include "OleObjectItem.h"
#include <boost_filesystem_version.h>


namespace Odt
{
	namespace Manifest
	{
		class OleObjectTable : public BaseTable<OleObjectItem>
		{
		public:
			OleObjectTable();
			virtual ~OleObjectTable();
			explicit OleObjectTable(const XML::XNode& node);
			const OleObjectTable& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const XML::XNode toXML(const boost::filesystem::wpath& path) const;

		public:
			const boost::filesystem::wpath oleObject(const boost::filesystem::wpath& filename) const;
			const boost::filesystem::wpath addOleObject(const boost::filesystem::wpath& filename);

		public:
			void copy(const boost::filesystem::wpath& path) const;
		};
	} // namespace Manifest
} // namespace Odt

#endif // ODT_MANIFEST_OLE_OBJECT_TABLE_INCLUDE_H_