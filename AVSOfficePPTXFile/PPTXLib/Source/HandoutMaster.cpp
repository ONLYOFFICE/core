#include "HandoutMaster.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	HandoutMaster::HandoutMaster()
	{
	}

	HandoutMaster::HandoutMaster(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	HandoutMaster::~HandoutMaster()
	{
	}

	void HandoutMaster::read(const boost::filesystem::wpath& filename, FileMap& map)
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
	}

	void HandoutMaster::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.p + "handoutMaster",
			XML::XNamespace(ns.a) +
			XML::XNamespace(ns.r) +
			XML::Write(cSld) +
			XML::Write(clrMap) +
			XML::Write(hf)
		).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
		m_written = true;
		m_WrittenFileName = filename.filename();
		FileContainer::write(filename, directory, content);
	}

	const OOX::FileType HandoutMaster::type() const
	{
		return OOX::FileTypes::HandoutMaster;
	}

	const boost::filesystem::wpath HandoutMaster::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath HandoutMaster::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
} // namespace PPTX