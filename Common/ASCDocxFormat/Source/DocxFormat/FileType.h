#pragma once
#ifndef OOX_FILE_TYPE_INCLUDE_H_
#define OOX_FILE_TYPE_INCLUDE_H_

#include <string>

#include "../../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

namespace OOX
{
	class FileType
	{
	public:
		FileType(const OOX::CPath& defaultDirectory, const OOX::CPath& defaultFileName, const std::wstring& overrideType, const std::wstring& relationType);
		~FileType();

	public:
		const bool operator ==(const FileType& rhs) const;

	public:
		const std::wstring OverrideType() const;
		const std::wstring RelationType() const;
		const OOX::CPath DefaultDirectory() const;
		const OOX::CPath DefaultFileName() const;

	private:
		std::wstring	m_overrideType;
		std::wstring	m_relationType;
		OOX::CPath		m_defaultDirectory;
		OOX::CPath		m_defaultFileName;
	};

	const bool operator ==(const std::wstring& type, const FileType& file);
	const bool operator ==(const FileType& file, const std::wstring& type);
} // namespace OOX

#endif // OOX_FILE_TYPE_INCLUDE_H_