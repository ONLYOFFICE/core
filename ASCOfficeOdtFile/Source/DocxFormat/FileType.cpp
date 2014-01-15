
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FileType.h"


namespace OOX
{

	FileType::FileType(	const boost::filesystem::wpath& defaultDirectory, 
											const boost::filesystem::wpath& defaultFileName,
											const std::string& overrideType, 
											const std::string& relationType)
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


	const std::string FileType::OverrideType() const
	{
		return m_overrideType;
	}


	const std::string FileType::RelationType() const
	{
		return m_relationType;
	}


	const boost::filesystem::wpath FileType::DefaultDirectory() const
	{
		return m_defaultDirectory;
	}


	const boost::filesystem::wpath FileType::DefaultFileName() const
	{
		return m_defaultFileName;
	}


	const bool operator ==(const std::string& type, const FileType& file)
	{
		return type == file.RelationType();
	}


	const bool operator ==(const FileType& file, const std::string& type)
	{
		return file.RelationType() == type;
	}

} // namespace OOX