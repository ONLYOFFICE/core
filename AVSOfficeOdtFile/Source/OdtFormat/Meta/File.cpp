
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "File.h"


namespace Odt
{
	namespace Meta
	{
		static const boost::filesystem::wpath s_filename = L"meta.xml";

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
			m_version = document.Root.attribute("version").value();
			Statistic = document.Root.element("meta").element("document-statistic");
			m_editingDuration = "PT00H00M00S";
			m_editingCicles = 0;
			m_generation = document.Root.element("meta").element("generator").text();
		}


		void File::write(const boost::filesystem::wpath& path) const
		{
			XML::XElement(ns.office + "document-meta",
				XML::XAttribute(ns.office + "version", m_version) +
				XML::XElement(ns.office + "meta",
					XML::XElement(ns.meta + "creation-date", XML::XText(m_creation)) +
					XML::Write(Statistic) +
					XML::XElement(ns.dc + "date", XML::XText(m_date)) +
					XML::XElement(ns.meta + "editing-duration", XML::XText(m_editingDuration)) +
					XML::XElement(ns.meta + "editing-cycles", XML::XText(m_editingCicles)) +
					XML::XElement(ns.meta + "generator", XML::XText(m_generation))
				)
			).Save(path/s_filename);
		}


		const bool File::isValid(const boost::filesystem::wpath& path) const
		{
			return true;
		}

	} // namespace Meta
} // namespace Odt