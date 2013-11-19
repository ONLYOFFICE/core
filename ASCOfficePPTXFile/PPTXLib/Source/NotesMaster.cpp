#include "NotesMaster.h"
#include "Exception/not_implement.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	NotesMaster::NotesMaster()
	{
	}

	NotesMaster::NotesMaster(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	NotesMaster::~NotesMaster()
	{
	}

	void NotesMaster::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		//FileContainer::read(filename, map);
		const XML::XDocument document(filename, true);

		cSld = document.Root.element("cSld");
		cSld->SetParentFilePointer(*this);

		clrMap = document.Root.element("clrMap");
		clrMap->SetParentFilePointer(*this);

		hf = document.Root.element("hf");
		if(hf.is_init())
			hf->SetParentFilePointer(*this);

		notesStyle = document.Root.element("notesStyle");
		if(notesStyle.is_init())
			notesStyle->SetParentFilePointer(*this);
	}

	void NotesMaster::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.p + "notesMaster",
			XML::XNamespace(ns.a) +
			XML::XNamespace(ns.r) +
			XML::Write(cSld) +
			XML::Write(clrMap) +
			XML::Write(hf) +
			XML::Write(notesStyle)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
		FileContainer::write(filename, directory, content);
	}

	const OOX::FileType NotesMaster::type() const
	{
		return OOX::FileTypes::NotesMaster;
	}

	const boost::filesystem::wpath NotesMaster::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath NotesMaster::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
} // namespace PPTX