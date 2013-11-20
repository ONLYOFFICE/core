#pragma once
#ifndef OOX_FILE_TYPE_INCLUDE_H_
#define OOX_FILE_TYPE_INCLUDE_H_

#include <string>
#include <boost/filesystem.hpp>


namespace OOX
{
	class FileType
	{
	public:
		FileType(	const boost::filesystem::wpath& defaultDirectory,
							const boost::filesystem::wpath& defaultFileName,
							const std::string& overrideType, 
							const std::string& relationType);
		~FileType();

	public:
		const bool operator ==(const FileType& rhs) const;

	public:
		const std::string OverrideType() const;
		const std::string RelationType() const;
		const boost::filesystem::wpath DefaultDirectory() const;
		const boost::filesystem::wpath DefaultFileName() const;

	private:
		std::string								m_overrideType;
		std::string								m_relationType;
		boost::filesystem::wpath	m_defaultDirectory;
		boost::filesystem::wpath	m_defaultFileName;
	};

	const bool operator ==(const std::string& type, const FileType& file);
	const bool operator ==(const FileType& file, const std::string& type);
} // namespace OOX

#endif // OOX_FILE_TYPE_INCLUDE_H_