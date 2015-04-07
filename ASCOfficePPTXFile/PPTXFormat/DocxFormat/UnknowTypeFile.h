#pragma once
#ifndef OOX_UNKNOW_TYPE_FILE_INCLUDE_H_
#define OOX_UNKNOW_TYPE_FILE_INCLUDE_H_

#include "File.h"
#include "FileTypes.h"


namespace PPTX
{
	class UnknowTypeFile : public File
	{
	public:
		UnknowTypeFile()
		{
		}
		virtual ~UnknowTypeFile()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename)
		{
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const
		{
		}

	public:
		virtual const FileType type() const
		{
			return FileTypes::Unknow;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
	};
} // namespace OOX

#endif // OOX_UNKNOW_TYPE_FILE_INCLUDE_H_