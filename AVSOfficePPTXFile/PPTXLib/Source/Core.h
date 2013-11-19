#pragma once
#ifndef PPTX_CORE_FILE_INCLUDE_H_
#define PPTX_CORE_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "DocxFormat/DateTime.h"
#include "nullable.h"
#include "property.h"
#include "Limit/ContentStatus.h"

namespace PPTX
{
	class Core : public WrapperFile
	{
	public:
		Core();
		Core(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~Core();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		nullable_property<std::string> title;
		nullable_property<std::string> creator;
		nullable_property<std::string> lastModifiedBy;
		nullable_property<std::string> revision;
//			nullable_property<OOX::DateTime> created;
		nullable_property<std::string> created;
//			nullable_property<OOX::DateTime> modified;
		nullable_property<std::string> modified;
/*
		nullable_property<std::string> category;
		nullable_property<std::string, Limit::ContentStatus> contentStatus;
		nullable_property<std::string> description;
//identifier - ???
//			nullable_property<std::string> keywords;
		nullable_property<std::string> language;
		nullable_property<OOX::DateTime> lastPrinted;
		nullable_property<std::string> subject;
		nullable_property<std::string> version;
*/
	};
} // namespace PPTX

#endif // PPTX_CORE_FILE_INCLUDE_H_