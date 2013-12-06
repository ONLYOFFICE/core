#pragma once
#ifndef OOX_CONTENT_TYPES_FILE_INCLUDE_H_
#define OOX_CONTENT_TYPES_FILE_INCLUDE_H_

#include <vector>
#include "./../FileType.h"

namespace OOX
{
	namespace ContentTypes
	{
		class File
		{
		public:
			File();
			File(const OOX::CPath& path);
			virtual ~File();

		public:
			virtual void read(const OOX::CPath& path);
			virtual void write(const OOX::CPath& path) const;
			virtual const bool isValid() const;

		public:
			void registration(const std::string& type, const OOX::CPath& directory, const OOX::CPath& filename);
		};
	} // namespace ContentTypes
} // namespace OOX

#endif // DOCX_CONTENT_TYPES_FILE_INCLUDE_H_