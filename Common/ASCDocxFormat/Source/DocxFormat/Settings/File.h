#pragma once
#ifndef OOX_SETTINGS_FILE_INCLUDE_H_
#define OOX_SETTINGS_FILE_INCLUDE_H_

#include "./../File.h"

namespace OOX
{
	namespace Settings
	{
		class File : public OOX::File
		{
		public:
			File();
			File(const OOX::CPath& filename);
			virtual ~File();

		public:
			virtual void read(const OOX::CPath& filename);
			virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const;

		public:
			virtual const FileType type() const;
			virtual const OOX::CPath DefaultDirectory() const;
			virtual const OOX::CPath DefaultFileName() const;
	};
	} // namespace Settings
} // namespace OOX

#endif // OOX_SETTING_FILE_INCLUDE_H_