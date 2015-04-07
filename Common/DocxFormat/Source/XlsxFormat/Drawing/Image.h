#pragma once
#ifndef OOX_SPREADSHEETIMAGE_INCLUDE_H_
#define OOX_SPREADSHEETIMAGE_INCLUDE_H_

#include "../CommonInclude.h"

#include "../../DocxFormat/Media/Media.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class Image : public Media
		{
		public:
			Image()
			{
			}
			Image(const CPath& filename)
			{
				read(filename);
			}
			virtual ~Image()
			{
			}

		public:
			virtual void write(const CPath& filename, const CPath& directory, CContentTypes& content) const
			{
				content.Registration(type().RelationType(), directory, filename.GetFilename());
			}

		public:
			virtual const FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Image;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return m_filename.GetFilename();
			}
		};
	}
} // namespace OOX

#endif // OOX_SPREADSHEETIMAGE_INCLUDE_H_