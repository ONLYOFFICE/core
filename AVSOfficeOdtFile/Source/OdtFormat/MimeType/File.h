#pragma once
#ifndef ODT_MIMETYPE_FILE_INCLUDE_H_
#define ODT_MIMETYPE_FILE_INCLUDE_H_

#include "./../File.h"


namespace Odt
{
	namespace MimeType
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
			static const bool exist(const boost::filesystem::wpath& path);
		};
	} // namespace MimeType
} // namespace Odt

#endif // ODT_MIMETYPE_FILE_INCLUDE_H_