#pragma once
#ifndef PPTX_PRESPROPS_FILE_INCLUDE_H_
#define PPTX_PRESPROPS_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "property.h"
#include "nullable.h"
#include "Logic/UniColor.h"
#include <list>

namespace PPTX
{
	class PresProps : public WrapperFile
	{
	public:
		PresProps();
		PresProps(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~PresProps();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		property<std::list<Logic::UniColor> > ClrMru;
		//prnPr (Printing Properties)
		//showPr (Presentation-wide Show Properties)
	};
} // namespace PPTX

#endif // PPTX_PRESPROPS_FILE_INCLUDE_H_