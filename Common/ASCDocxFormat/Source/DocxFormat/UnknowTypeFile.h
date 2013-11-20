#pragma once
#ifndef OOX_UNKNOW_TYPE_FILE_INCLUDE_H_
#define OOX_UNKNOW_TYPE_FILE_INCLUDE_H_

#include "File.h"


namespace OOX
{
	class UnknowTypeFile : public File
	{
	public:
		UnknowTypeFile();
		virtual ~UnknowTypeFile();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;
	};
} // namespace OOX

#endif // OOX_UNKNOW_TYPE_FILE_INCLUDE_H_