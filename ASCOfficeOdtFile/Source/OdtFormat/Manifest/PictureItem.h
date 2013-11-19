#pragma once
#ifndef ODT_MANIFEST_PICTURE_ITEM_INCLUDE_H_
#define ODT_MANIFEST_PICTURE_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include <boost_filesystem_version.h>


namespace Odt
{
	namespace Manifest
	{
		class PictureItem : public WritingElement
		{
		public:
			PictureItem();
			PictureItem(const boost::filesystem::wpath& filename);
			virtual ~PictureItem();
			explicit PictureItem(const XML::XNode& node);
			const PictureItem& operator =(const XML::XNode& node);

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

#endif // ODT_MANIFEST_PICTURE_ITEM_INCLUDE_H_