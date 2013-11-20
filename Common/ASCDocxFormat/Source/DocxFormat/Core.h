#pragma once
#ifndef OOX_CORE_INCLUDE_H_
#define OOX_CORE_INCLUDE_H_

#include "File.h"
#include "DateTime.h"
#include "nullable.h"
#include "property.h"


namespace OOX
{
	class Core : public OOX::File
	{
	public:
		Core();
		Core(const boost::filesystem::wpath& filename);
		virtual ~Core();

	public:
		virtual void read(const boost::filesystem::wpath& filename);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const;

	public:
		virtual const FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	private:
		nullable<std::string>		m_title;
		nullable<std::string>		m_subject;
		nullable<std::string>		m_creator;
		nullable<std::string>		m_keywords;
		nullable<std::string>		m_description;
		nullable<std::string>		m_lastModifiedBy;
		nullable<int>				m_revision;
		nullable<OOX::DateTime>				m_created;
		nullable<OOX::DateTime>				m_modified;
	};
} // namespace OOX

#endif // OOX_CORE_INCLUDE_H_