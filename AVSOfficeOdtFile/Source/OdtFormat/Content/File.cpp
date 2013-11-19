
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "File.h"
#include "XML.h"


namespace Odt
{
	namespace Content
	{
		static const boost::filesystem::wpath s_filename = L"content.xml";

		File::File()
		{
		}


		File::~File()
		{
		}


		const bool File::exist(const boost::filesystem::wpath& path)
		{
			return boost::filesystem::exists(path/s_filename);
		}


		void File::read(const boost::filesystem::wpath& path)
		{
			const XML::XDocument document(path/s_filename, true);
			m_version			= document.Root.attribute("version").value();
			FontFaceDecls	= document.Root.element("font-face-decls");
			NamedStyles = document.Root.element("automatic-styles");
			ListStyles  = document.Root.element("automatic-styles");
			Body = document.Root.element("body");
		}


		void File::write(const boost::filesystem::wpath& path) const
		{
			XML::XElement(ns.office + "document-content",
				XML::XAttribute(ns.office + "version", m_version) +
				XML::XElement(ns.office + "scripts") +
				XML::Write(FontFaceDecls) +
				XML::XElement(ns.office + "automatic-styles", 
					XML::Write(NamedStyles) +
					XML::Write(ListStyles)
				) +
				XML::Write(Body)
			).Save(path/s_filename);
		}
	

		const bool File::isValid(const boost::filesystem::wpath& path) const
		{
			return true;
		}


		const bool File::hasList() const
		{
			return Body->hasList();
		}


		const bool File::hasNote() const
		{
			return Body->hasNote();
		}

	} // namespace Content
} // namespace Odt