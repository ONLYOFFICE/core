
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FileType.h"

namespace OOX
{
	FileType::FileType(	const OOX::CPath& defaultDirectory, 
		const OOX::CPath& defaultFileName,
		const std::wstring& overrideType, 
		const std::wstring& relationType)
		: m_defaultDirectory(defaultDirectory),
		m_defaultFileName(defaultFileName),
		m_overrideType(overrideType),
		m_relationType(relationType)
	{
	}

	FileType::~FileType()
	{
	}

	const bool FileType::operator ==(const FileType& rhs) const
	{
		return m_relationType == rhs.m_relationType;

	}

	const std::wstring FileType::OverrideType() const
	{
		return m_overrideType;
	}

	const std::wstring FileType::RelationType() const
	{
		return m_relationType;
	}

	const OOX::CPath FileType::DefaultDirectory() const
	{
		return m_defaultDirectory;
	}

	const OOX::CPath FileType::DefaultFileName() const
	{
		return m_defaultFileName;
	}

	const bool operator ==(const std::wstring& type, const FileType& file)
	{
		return type == file.RelationType();
	}

	const bool operator ==(const FileType& file, const std::wstring& type)
	{
		return file.RelationType() == type;
	}

} // namespace OOX