#pragma once
#ifndef OOX_FILE_INCLUDE_H_
#define OOX_FILE_INCLUDE_H_

#include "../../../Common/DocxFormat/Source/XML/XmlSimple.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

#include "FileType.h"
#include "ContentTypes/File.h"

namespace PPTX
{
	class File
	{
	public:
		File(){}
		virtual ~File(){}

	public:
		virtual void read(const OOX::CPath& filename)																	= 0;
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const		= 0;

	public:
		virtual const PPTX::FileType type() const																	= 0;
		virtual const OOX::CPath DefaultDirectory() const																= 0;
		virtual const OOX::CPath DefaultFileName() const																	= 0;
	};
} // namespace PPTX

#endif // OOX_FILE_INCLUDE_H_