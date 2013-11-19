#include "NotesSlide.h"
#include "Exception/not_implement.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	NotesSlide::NotesSlide()
	{
	}

	NotesSlide::NotesSlide(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	NotesSlide::~NotesSlide()
	{
	}

	void NotesSlide::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);
		const XML::XDocument document(filename, true);

		showMasterPhAnim = document.Root.attribute("showMasterPhAnim").value();
		showMasterSp = document.Root.attribute("showMasterSp").value();

		cSld = document.Root.element("cSld");
		cSld->SetParentFilePointer(*this);

		clrMapOvr = document.Root.element("clrMapOvr");
		if(clrMapOvr.is_init())
			clrMapOvr->SetParentFilePointer(*this);
	}

	void NotesSlide::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.p + "notes",
			XML::XNamespace(ns.a) +
			XML::XNamespace(ns.r) +
			XML::XAttribute("showMasterPhAnim", showMasterPhAnim) +
			XML::XAttribute("showMasterSp", showMasterSp) +
			XML::Write(cSld) +
			XML::Write(clrMapOvr)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
		FileContainer::write(filename, directory, content);
	}

	const OOX::FileType NotesSlide::type() const
	{
		return OOX::FileTypes::NotesSlide;
	}

	const boost::filesystem::wpath NotesSlide::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath NotesSlide::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
} // namespace PPTX