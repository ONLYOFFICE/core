
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "File.h"


namespace Odt
{
	namespace Styles
	{
		static const boost::filesystem::wpath s_filename = L"styles.xml";

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
			m_version				= document.Root.attribute("version").value();
			FontFaceDecls		= document.Root.element("font-face-decls");
			Styles					= document.Root.element("styles");
			AutomaticStyles	= document.Root.element("automatic-styles");
			MasterStyles		= document.Root.element("master-styles");
		}


		void File::write(const boost::filesystem::wpath& path) const
		{
			XML::XElement(ns.office + "document-styles",
				XML::XAttribute(ns.office + "version", m_version) +
				XML::Write(FontFaceDecls) +
				XML::Write(Styles) +
				XML::Write(AutomaticStyles) +
				XML::Write(MasterStyles)
			).Save(path/s_filename);
		}


		const bool File::isValid(const boost::filesystem::wpath& path) const
		{
			return true;
		}

	} // namespace Styles
} // namespace Odt