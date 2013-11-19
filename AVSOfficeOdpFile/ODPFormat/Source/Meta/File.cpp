#include "File.h"

namespace Odp
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
			const XML::XDocument document(path/s_filename);
			version = document.Root.attribute("version").value();

			creation_date = document.Root.element("meta").element("creation-date").text();
			editing_duration = document.Root.element("meta").element("editing-duration").text();
			editing_cycles = document.Root.element("meta").element("editing-cycles").text();
			date = document.Root.element("meta").element("date").text();
			generator = document.Root.element("meta").element("generator").text();

			statistic = document.Root.element("meta").element("document-statistic");
			XML::Fill(user_defined, document.Root.element("meta"), "user-defined");
		}


		void File::write(const boost::filesystem::wpath& path) const
		{
			XML::XElement(ns.office + "document-meta",
				XML::XAttribute(ns.office + "version", version) +
				XML::XElement(ns.office + "meta",
					XML::XElement(ns.meta + "creation-date", XML::XText(creation_date)) +
					XML::XElement(ns.meta + "editing-duration", XML::XText(editing_duration)) +
					XML::XElement(ns.meta + "editing-cycles", XML::XText(editing_cycles)) +
					XML::XElement(ns.dc + "date", XML::XText(date)) +
					XML::XElement(ns.meta + "generator", XML::XText(generator)) +
					XML::Write(statistic) +
					XML::Write(user_defined)
				)
			).Save(path/s_filename);
		}


		const bool File::isValid(const boost::filesystem::wpath& path) const
		{
			return contain(path/s_filename);
		}

	} // namespace Meta
} // namespace Odp