#pragma once
#ifndef ODT_MANIFEST_PICTURE_TABLE_INCLUDE_H_
#define ODT_MANIFEST_PICTURE_TABLE_INCLUDE_H_

#include "BaseTable.h"
#include "PictureItem.h"
#include <boost_filesystem_version.h>


namespace Odt
{
	namespace Manifest
	{
		class PictureTable : public BaseTable<PictureItem>
		{
		public:
			PictureTable();
			virtual ~PictureTable();
			explicit PictureTable(const XML::XNode& node);
			const PictureTable& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const XML::XNode toXML(const boost::filesystem::wpath& path) const;

		public:
			const boost::filesystem::wpath picture(const boost::filesystem::wpath& filename) const;
			bool hasPicture(const boost::filesystem::wpath& filename) const;
			const boost::filesystem::wpath addPicture(const boost::filesystem::wpath& filename);

		public:
			void copy(const boost::filesystem::wpath& path) const;
		};
	} // namespace Manifest
} // namespace Odt

#endif // ODT_MANIFEST_PICTURE_TABLE_INCLUDE_H_