#pragma once
#ifndef OOX_FILE_INCLUDE_H_
#define OOX_FILE_INCLUDE_H_

#include "../../../Common/DocxFormat/Source/XML/XmlSimple.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

#include "FileType.h"
#include "ContentTypes/File.h"

namespace OOX
{
	class File
	{
	public:
		File(){}
		virtual ~File(){}

	public:
		virtual void read(const CPath& filename)																	= 0;
		virtual void write(const CPath& filename, const CPath& directory, ContentTypes::File& content) const		= 0;

	public:
		virtual const OOX::FileType type() const																	= 0;
		virtual const CPath DefaultDirectory() const																= 0;
		virtual const CPath DefaultFileName() const																	= 0;
	};
} // namespace OOX

#endif // OOX_FILE_INCLUDE_H_