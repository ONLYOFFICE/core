#pragma once
#ifndef ODT_MANIFEST_FILE_INCLUDE_H_
#define ODT_MANIFEST_FILE_INCLUDE_H_

#include "./../File.h"
#include "PictureTable.h"
#include "OleObjectTable.h"
#include "OleViewTable.h"
#include <boost_filesystem_version.h>


namespace Odt
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
			bool hasPicture(const boost::filesystem::wpath& filename) const;
			const boost::filesystem::wpath addPicture(const boost::filesystem::wpath& filename);

		public:
			const boost::filesystem::wpath oleObject(const boost::filesystem::wpath& filename) const;
			const boost::filesystem::wpath addOleObject(const boost::filesystem::wpath& filename);

		public:
			const boost::filesystem::wpath oleView(const boost::filesystem::wpath& filename) const;
			const boost::filesystem::wpath addOleView(const boost::filesystem::wpath& filename);

		public:
			void extractPictures(const boost::filesystem::wpath& path) const;

			bool isFolderCrypted()const;

		private:
			PictureTable	m_picture;
			OleObjectTable	m_oleObject;
			OleViewTable	m_oleView;
			bool m_bCrypted;
		};
	} // namespace Manifest
} // namespace Odt

#endif // ODT_MANIFEST_FILE_INCLUDE_H_