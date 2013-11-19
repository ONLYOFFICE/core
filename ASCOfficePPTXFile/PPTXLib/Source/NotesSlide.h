#pragma once
#ifndef PPTX_SLIDES_NOTESSLIDE_INCLUDE_H_
#define PPTX_SLIDES_NOTESSLIDE_INCLUDE_H_

#include <boost/filesystem.hpp>
#include "WrapperFile.h"
#include "FileContainer.h"
#include "property.h"
#include "nullable.h"
#include "XML.h"
#include "Logic/ClrMapOvr.h"
#include "Logic/CSld.h"

namespace PPTX
{
	class NotesSlide : public WrapperFile, public PPTX::FileContainer
	{
	public:
		NotesSlide();
		NotesSlide(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~NotesSlide();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		property<Logic::CSld> cSld;
		nullable_property<Logic::ClrMapOvr> clrMapOvr;

		nullable_property<bool> showMasterPhAnim;
		nullable_property<bool> showMasterSp;
	};
} // namespace PPTX

#endif // PPTX_SLIDES_NOTESSLIDE_INCLUDE_H_