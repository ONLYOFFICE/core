#include "PresProps.h"
#include "XML.h"
#include "DocxFormat/FileTypes.h"

namespace PPTX
{
	PresProps::PresProps()
	{
	}

	PresProps::PresProps(const boost::filesystem::wpath& filename, FileMap& map)
	{
		read(filename, map);
	}

	PresProps::~PresProps()
	{
	}

	void PresProps::read(const boost::filesystem::wpath& filename, FileMap& map)
	{
		const XML::XDocument document(filename, true);
		ClrMru->clear();

		if(document.Root.element("clrMru").exist())
			XML::Fill(ClrMru, document.Root.element("clrMru"));
	}

	void PresProps::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const
	{
		XML::XElement(ns.p+"presentationPr",
				XML::XNamespace(ns.a) +
				XML::XNamespace(ns.r) +
				XML::XElement(ns.p + "clrMru",	XML::Write(ClrMru))
			).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
	}

	const OOX::FileType PresProps::type() const
	{
		return OOX::FileTypes::PresProps;
	}

	const boost::filesystem::wpath PresProps::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}

	const boost::filesystem::wpath PresProps::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
} // namespace PPTX