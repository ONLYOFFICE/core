#pragma once
#ifndef OOX_FILE_INCLUDE_H_
#define OOX_FILE_INCLUDE_H_

#include <string>

#include "NamespaceOwn.h"
#include "ContentTypes/File.h"
#include "FileType.h"

#include "../.././../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../.././../../Common/DocxFormat/Source/Xml/XmlUtils.h"

namespace OOX
{
	class File : public NamespaceOwn
	{
	public:
		File();
		virtual ~File();

	public:
		virtual void read(const OOX::CPath& filename) = 0;
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, ContentTypes::File& content) const = 0;

	public:
		virtual const OOX::FileType type() const = 0;
		virtual const OOX::CPath DefaultDirectory() const = 0;
		virtual const OOX::CPath DefaultFileName() const = 0;
	};
} // namespace OOX

#endif // OOX_FILE_INCLUDE_H_