#include "ViewProps.h"
#include "XML.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	ViewProps::ViewProps()
	{
	}

	ViewProps::ViewProps(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	ViewProps::~ViewProps()
	{
	}

	void ViewProps::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);

		const XML::XDocument document(filename, true);

		attrLastView = document.Root.attribute("lastView").value();
		attrShowComments = document.Root.attribute("showComments").value();
		
		GridSpacing = document.Root.element("gridSpacing");
		if(GridSpacing.is_init())
			GridSpacing->SetParentFilePointer(*this);

		NormalViewPr = document.Root.element("normalViewPr");
		if(NormalViewPr.is_init())
			NormalViewPr->SetParentFilePointer(*this);

		NotesTextViewPr = document.Root.element("notesTextViewPr");
		if(NotesTextViewPr.is_init())
			NotesTextViewPr->SetParentFilePointer(*this);

		NotesViewPr = document.Root.element("notesViewPr");
		if(NotesViewPr.is_init())
			NotesViewPr->SetParentFilePointer(*this);

		OutlineViewPr = document.Root.element("outlineViewPr");
		if(OutlineViewPr.is_init())
			OutlineViewPr->SetParentFilePointer(*this);

		SlideViewPr = document.Root.element("slideViewPr");
		if(SlideViewPr.is_init())
			SlideViewPr->SetParentFilePointer(*this);

		SorterViewPr = document.Root.element("sorterViewPr");
		if(SorterViewPr.is_init())
			SorterViewPr->SetParentFilePointer(*this);
	}

	void ViewProps::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.p+"viewPr",
			XML::XNamespace(ns.a) +
			XML::XNamespace(ns.r) +
			XML::XAttribute("lastView", attrLastView) +
			XML::XAttribute("showComments", attrShowComments) +
			XML::Write(NormalViewPr) +
			XML::Write(SlideViewPr)+
			XML::Write(OutlineViewPr)+
			XML::Write(NotesTextViewPr) +
			XML::Write(SorterViewPr) +
			XML::Write(NotesViewPr) +
			XML::Write(GridSpacing)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
		FileContainer::write(filename, directory, content);
	}

	const OOX::FileType ViewProps::type() const
	{
		return OOX::FileTypes::ViewProps;
	}

	const boost::filesystem::wpath ViewProps::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath ViewProps::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
} // namespace PPTX
