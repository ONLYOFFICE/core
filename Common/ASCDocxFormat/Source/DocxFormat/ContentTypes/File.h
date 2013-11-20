#pragma once
#ifndef OOX_CONTENT_TYPES_FILE_INCLUDE_H_
#define OOX_CONTENT_TYPES_FILE_INCLUDE_H_

#include "property.h"
#include <vector>
#include "OverrideTable.h"
#include "DefaultTable.h"
#include "./../FileType.h"


namespace OOX
{
	namespace ContentTypes
	{
		class File
		{
		public:
			File();
			File(const boost::filesystem::wpath& path);
			virtual ~File();

		public:
			virtual void read(const boost::filesystem::wpath& path);
			virtual void write(const boost::filesystem::wpath& path) const;
			virtual const bool isValid() const;

		public:
			void registration(const std::string& type, const boost::filesystem::wpath& directory, const boost::filesystem::wpath& filename);

		public:
			property<OverrideTable> Override;
			property<DefaultTable>	Default;
		};
	} // namespace ContentTypes
} // namespace OOX

#endif // DOCX_CONTENT_TYPES_FILE_INCLUDE_H_