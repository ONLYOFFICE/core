#include "File.h"
#include "XML.h"

namespace Odp
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
			const XML::XDocument document(path/s_filename);
			version = document.Root.attribute("version").value();
			NamedStyles = document.Root.element("automatic-styles");
			ListStyles  = document.Root.element("automatic-styles");
			body = document.Root.element("body");
		}

		void File::write(const boost::filesystem::wpath& path) const
		{
			XML::XElement(ns.office + "document-content",
				XML::XNamespace(ns.office) +
				XML::XNamespace(ns.style) +
				XML::XNamespace(ns.text) +
				XML::XNamespace(ns.table) +
				XML::XNamespace(ns.draw) +
				XML::XNamespace(ns.fo) +
				XML::XNamespace(ns.xlink) +
				XML::XNamespace(ns.dc) +
				XML::XNamespace(ns.meta) +
				XML::XNamespace(ns.number) +
				XML::XNamespace(ns.presentation) +
				XML::XNamespace(ns.svg) +
				XML::XNamespace(ns.chart) +
				XML::XNamespace(ns.dr3d) +
				XML::XNamespace(ns.math) +
				XML::XNamespace(ns.form) +
				XML::XNamespace(ns.script) +
				XML::XNamespace(ns.ooo) +
				XML::XNamespace(ns.ooow) +
				XML::XNamespace(ns.oooc) +
				XML::XNamespace(ns.dom) +
				XML::XNamespace(ns.xforms) +
				XML::XNamespace(ns.xsd) +
				XML::XNamespace(ns.xsi) +
				XML::XNamespace(ns.smil) +
				XML::XNamespace(ns.anim) +
				XML::XNamespace(ns.rpt) +
				XML::XNamespace(ns.of) +
				XML::XNamespace(ns.rdfa) +
				XML::XNamespace(ns.field) +
				XML::XNamespace(ns.formx) +
				XML::XAttribute(ns.office + "version", version) +
				XML::XElement(ns.office + "scripts") +
				XML::XElement(ns.office + "automatic-styles", 
					XML::Write(NamedStyles) +
					XML::Write(ListStyles)
				) +
				XML::Write(body)
			).Save(path/s_filename);
		}

		const bool File::isValid(const boost::filesystem::wpath& path) const
		{
			return true;//contain(path/s_filename);
		}
	} // namespace Content
} // namespace Odp