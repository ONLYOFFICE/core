#pragma once
#ifndef PPTX_VIEWPROPS_FILE_INCLUDE_H_
#define PPTX_VIEWPROPS_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "FileContainer.h"

#include "property.h"
#include "nullable.h"
#include "Limit/LastView.h"
#include "ViewProps/GridSpacing.h"
#include "ViewProps/NormalViewPr.h"
#include "ViewProps/NotesTextViewPr.h"
#include "ViewProps/NotesViewPr.h"
#include "ViewProps/OutlineViewPr.h"
#include "ViewProps/SlideViewPr.h"
#include "ViewProps/SorterViewPr.h"

namespace PPTX
{
	class ViewProps : public WrapperFile, public PPTX::FileContainer
	{
	public:
		ViewProps();
		ViewProps(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~ViewProps();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

	public:
		nullable_property<std::string, Limit::LastView> attrLastView;
		nullable_property<bool> attrShowComments;
		nullable_property<nsViewProps::GridSpacing> GridSpacing;
		nullable_property<nsViewProps::NormalViewPr> NormalViewPr;
		nullable_property<nsViewProps::NotesTextViewPr> NotesTextViewPr;
		nullable_property<nsViewProps::NotesViewPr> NotesViewPr;
		nullable_property<nsViewProps::OutlineViewPr> OutlineViewPr;
		nullable_property<nsViewProps::SlideViewPr> SlideViewPr;
		nullable_property<nsViewProps::SorterViewPr> SorterViewPr;
	};
} // namespace PPTX

#endif // PPTX_VIEWPROPS_FILE_INCLUDE_H_