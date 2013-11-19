#include "File.h"

namespace Odp
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
			const XML::XDocument document(path/s_filename);
			m_version = document.Root.attribute("version").value();

			FillImages = document.Root.element("styles");
			Gradients = document.Root.element("styles");
			Markers = document.Root.element("styles");
			DefaultStyles = document.Root.element("styles");
			Styles = document.Root.element("styles");
			TableTemplates = document.Root.element("styles");
			//Layouts = document.Root.element("styles");

			PageLayouts = document.Root.element("automatic-styles");
			AutomaticStyles = document.Root.element("automatic-styles");
			ListStyles = document.Root.element("automatic-styles");

			//LayerSet = document.Root.element("master-styles").element("layer-set");
			//HandoutMaster = document.Root.element("master-styles").element("handout-master");
			MasterPages = document.Root.element("master-styles");
		}

		void File::write(const boost::filesystem::wpath& path) const
		{
			XML::XElement(ns.office + "document-styles",
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
				XML::XAttribute(ns.office + "version", m_version) +
				XML::XElement(ns.office + "styles",
					XML::Write(Gradients) +
					XML::Write(FillImages) +
					XML::Write(Markers) +
					XML::Write(DefaultStyles) +
					XML::Write(Styles) +
					XML::Write(TableTemplates)// +
					//XML::Write(Layouts)
				) +
				XML::XElement(ns.office + "automatic-styles",
					XML::Write(PageLayouts) +
					XML::Write(AutomaticStyles) +
					XML::Write(ListStyles)
				) +
				XML::XElement(ns.office + "master-styles",
					//XML::XElement(ns.draw + "layer-set",
					//	XML::Write(LayerSet)
					//) +
					//XML::Write(HandoutMaster) +
					XML::Write(MasterPages)
				)
			).Save(path/s_filename);
		}

		const bool File::isValid(const boost::filesystem::wpath& path) const
		{
			return true;//contain(path/s_filename);
		}
	} // namespace Styles
} // namespace Odt