#pragma once
#ifndef OOX_FILE_TYPE_INCLUDE_H_
#define OOX_FILE_TYPE_INCLUDE_H_

#include "../../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"

namespace PPTX
{
	class FileType
	{
	public:
		FileType(const OOX::CPath& defaultDirectory, const OOX::CPath& defaultFileName,
							const CString& overrideType, 
							const CString& relationType) : m_defaultDirectory(defaultDirectory),
			m_defaultFileName(defaultFileName),
			m_overrideType(overrideType),
			m_relationType(relationType)
		{
		}

		FileType(const WCHAR* defaultDirectory, const WCHAR* defaultFileName,
							const CString& overrideType, 
							const CString& relationType) : m_defaultDirectory(defaultDirectory, false),
			m_defaultFileName(defaultFileName, false),
			m_overrideType(overrideType),
			m_relationType(relationType)
		{
		}

		~FileType()
		{
		}

	public:
		const bool operator ==(const FileType& rhs) const
		{
			return (m_relationType == rhs.m_relationType);
		}

	public:
		inline const CString OverrideType() const
		{
			return m_overrideType;
		}
		inline const CString RelationType() const
		{
			return m_relationType;
		}
		inline const OOX::CPath DefaultDirectory() const
		{
			return m_defaultDirectory;
		}
		inline const OOX::CPath DefaultFileName() const
		{
			return m_defaultFileName;
		}

	private:
		CString		m_overrideType;
		CString		m_relationType;
	 OOX::CPath		m_defaultDirectory;
	 OOX::CPath		m_defaultFileName;
	};

	static const bool operator ==(const CString& type, const FileType& file)
	{
		return (type == file.RelationType());
	}
	static const bool operator ==(const FileType& file, const CString& type)
	{
		return (file.RelationType() == type);
	}
} // namespace PPTX

#endif // OOX_FILE_TYPE_INCLUDE_H_