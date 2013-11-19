#pragma once
#ifndef ODT_MANIFEST_OLE_VIEW_TABLE_INCLUDE_H_
#define ODT_MANIFEST_OLE_VIEW_TABLE_INCLUDE_H_

#include "BaseTable.h"
#include "OleViewItem.h"
#include <boost_filesystem_version.h>


namespace Odt
{
	namespace Manifest
	{
		class OleViewTable : public BaseTable<OleViewItem>
		{
		public:
			OleViewTable();
			virtual ~OleViewTable();
			explicit OleViewTable(const XML::XNode& node);
			const OleViewTable& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const XML::XNode toXML(const boost::filesystem::wpath& path) const;

		public:
			const boost::filesystem::wpath oleView(const boost::filesystem::wpath& filename) const;
			const boost::filesystem::wpath addOleView(const boost::filesystem::wpath& filename);

		public:
			void copy(const boost::filesystem::wpath& path) const;
		};
	} // namespace Manifest
} // namespace Odt

#endif // ODT_MANIFEST_OLE_VIEW_TABLE_INCLUDE_H_