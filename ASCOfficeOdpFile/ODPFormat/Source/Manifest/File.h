#pragma once
#ifndef ODP_MANIFEST_FILE_INCLUDE_H_
#define ODP_MANIFEST_FILE_INCLUDE_H_

#include "OdtFormat/File.h"
#include "OdtFormat/Manifest/PictureTable.h"
#include <boost/filesystem.hpp>

namespace Odp
{
	namespace Manifest
	{
		class File : public Odt::File
		{
		public:
			File();
			virtual ~File();

		public:
			virtual void read(const boost::filesystem::wpath& path);
			virtual void write(const boost::filesystem::wpath& path) const;
			virtual const bool isValid(const boost::filesystem::wpath& path) const;

		public:
			const boost::filesystem::wpath picture(const boost::filesystem::wpath& filename) const;
			const boost::filesystem::wpath addPicture(const boost::filesystem::wpath& filename);

		public:
			void extractPictures(const boost::filesystem::wpath& path) const;

		private:
			Odt::Manifest::PictureTable	m_picture;
		};
	} // namespace Manifest
} // namespace Odp

#endif // ODP_MANIFEST_FILE_INCLUDE_H_