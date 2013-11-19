#include "File.h"
#include "OdtFormat/Manifest/XmlTable.h"
#include "Log.h"

namespace Odp
{
	namespace Manifest
	{
		static const boost::filesystem::wpath s_directory = L"META-INF";
		static const boost::filesystem::wpath s_filename = L"META-INF/manifest.xml";

		File::File()
		{
		}

		File::~File()
		{
		}

		void File::read(const boost::filesystem::wpath& path)
		{
			m_picture.clear();
			const XML::XDocument document(path/s_filename);

			for (XML::const_element_iterator i = document.Root.Elements.begin(); i != document.Root.Elements.end(); ++i)
			{
				const XML::XElement element(*i);
				const boost::filesystem::wpath filename = element.attribute("full-path").value();

				if (filename.extension() != L".xml" && filename.extension() != L"." && !filename.extension().empty())
				{
					m_picture.push_back(Odt::Manifest::PictureItem(path/filename));
				}
			}
		}

		void File::write(const boost::filesystem::wpath& path) const
		{
			boost::filesystem::create_directories(path/s_directory);
			Odt::Manifest::XmlTable xmlTable;
			xmlTable.add(path, L"Pictures/");
			xmlTable.add(path, L"ObjectReplacements/");
			xmlTable.add(path, L"content.xml");
			xmlTable.add(path, L"styles.xml");
			xmlTable.add(path, L"meta.xml");
			xmlTable.add(path, L"settings.xml");

			XML::XElement(ns.manifest + "manifest",
				XML::XElement(ns.manifest + "file-entry",
					XML::XAttribute(ns.manifest + "media-type", "application/vnd.oasis.opendocument.presentation") +
					XML::XAttribute(ns.manifest + "version", "1.2") + 
					XML::XAttribute(ns.manifest + "full-path", "/")
				) + 
				XML::Write(xmlTable) + 
				XML::Write(m_picture, path)
			).Save(path/s_filename);
		}

		const bool File::isValid(const boost::filesystem::wpath& path) const
		{
			if (!boost::filesystem::exists(path/s_filename))
			{
				Log::warning(L"not exist " + (path/s_filename).string());
				return false;
			}

			const XML::XDocument document(path/s_filename);
			for (XML::const_element_iterator i = document.Root.Elements.begin(); i != document.Root.Elements.end(); ++i)
			{
				boost::filesystem::wpath file = XML::XElement(*i)->attribute("full-path").value();
				if (!boost::filesystem::exists(path/file))
				{
					Log::warning(L"not exist " + (path/file).string());
					return false;
				}
			}

			return 
				document.Root.element("full-path", "/").exist() &&
				document.Root.element("full-path", "content.xml").exist() &&
				document.Root.element("full-path", "styles.xml").exist() &&
				document.Root.element("full-path", "meta.xml").exist() &&
				document.Root.element("full-path", "settings.xml").exist();
		}

		const boost::filesystem::wpath File::picture(const boost::filesystem::wpath& filename) const
		{
			return m_picture.picture(filename);
		}

		const boost::filesystem::wpath File::addPicture(const boost::filesystem::wpath& filename)
		{
			return m_picture.addPicture(filename);
		}

		void File::extractPictures(const boost::filesystem::wpath& path) const
		{
			m_picture.copy(path);
		}
	} // namespace Manifest
} // namespace Odp
