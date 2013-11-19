
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "File.h"


namespace Odt
{
	namespace Settings
	{
		static const boost::filesystem::wpath s_filename = L"settings.xml";

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
			boost::filesystem::wpath pathSettings = path/s_filename;
			bool bFileExists = boost::filesystem::exists(pathSettings);
			if (!bFileExists)
				return;
			
			const XML::XDocument document(pathSettings, true);
			m_version		= document.Root.attribute("version").value();
			m_settings	= document.Root.element("settings");
		}


		void File::write(const boost::filesystem::wpath& path) const
		{
			XML::XElement(ns.office + "document-settings",
				XML::XAttribute(ns.office + "version", m_version) +
				XML::Write(m_settings)
			).Save(path/s_filename);
		}


		const bool File::isValid(const boost::filesystem::wpath& path) const
		{
			return true;
		}

	} // namespace Settings
} // namespace Odt