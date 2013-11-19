#pragma once
#ifndef PPTX_WRAPPER_FILE_INCLUDE_H_
#define PPTX_WRAPPER_FILE_INCLUDE_H_

#include "DocxFormat/File.h"
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include "FileMap.h"

namespace PPTX
{
	class WrapperFile : public OOX::File
	{
	public:
		WrapperFile();
		virtual ~WrapperFile();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map) = 0;

	protected:
		mutable bool m_written;
		mutable boost::filesystem::wpath m_WrittenFileName;
	public:
		bool GetWrittenStatus()const;
		void WrittenSetFalse();
		const boost::filesystem::wpath GetWrittenFileName()const;
	};
} // namespace PPTX

#endif // PPTX_WRAPPER_FILE_INCLUDE_H_