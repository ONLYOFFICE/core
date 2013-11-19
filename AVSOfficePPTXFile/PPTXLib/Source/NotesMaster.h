#pragma once
#ifndef PPTX_SLIDES_NOTESMASTER_INCLUDE_H_
#define PPTX_SLIDES_NOTESMASTER_INCLUDE_H_

#include <boost/filesystem.hpp>
#include "WrapperFile.h"
#include "FileContainer.h"
#include "property.h"
#include "nullable.h"
#include "XML.h"
#include "Logic/ClrMap.h"
#include "Logic/CSld.h"
#include "Logic/Hf.h"
#include "Logic/TextListStyle.h"

namespace PPTX
{
	class NotesMaster : public WrapperFile, public PPTX::FileContainer
	{
	public:
		NotesMaster();
		NotesMaster(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~NotesMaster();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		property<Logic::CSld> cSld;
		property<Logic::ClrMap> clrMap;
		nullable_property<Logic::HF> hf;
		nullable_property<Logic::TextListStyle> notesStyle;
	};
} // namespace PPTX

#endif // PPTX_SLIDES_NOTESMASTER_INCLUDE_H_