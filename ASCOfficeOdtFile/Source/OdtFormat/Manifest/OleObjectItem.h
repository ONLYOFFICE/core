#pragma once
#ifndef ODT_MANIFEST_OLE_OBJECT_ITEM_INCLUDE_H_
#define ODT_MANIFEST_OLE_OBJECT_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include <boost_filesystem_version.h>


namespace Odt
{
	namespace Manifest
	{
		class OleObjectItem : public WritingElement
		{
		public:
			OleObjectItem();
			OleObjectItem(const boost::filesystem::wpath& filename);
			virtual ~OleObjectItem();
			explicit OleObjectItem(const XML::XNode& node);
			const OleObjectItem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const XML::XNode toXML(const boost::filesystem::wpath& path) const;

		public:
			const boost::filesystem::wpath path() const;
			const std::wstring filename() const;

		public:
			void copy(const boost::filesystem::wpath& path) const;

		private:
			boost::filesystem::wpath m_filename;
		};
	} // namespace Manifest
} // namespace Odt

#endif // ODT_MANIFEST_OLE_OBJECT_ITEM_INCLUDE_H_