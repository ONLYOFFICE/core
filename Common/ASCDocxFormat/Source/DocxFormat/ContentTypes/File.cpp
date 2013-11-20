
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "File.h"
#include "./../FileTypes.h"


namespace OOX
{
	namespace ContentTypes
	{
		static const boost::filesystem::wpath s_filename = L"[Content_Types].xml";

		File::File()
		{
		}


		File::File(const boost::filesystem::wpath& path)
		{
			read(path);
		}


		File::~File()
		{
		}


		void File::read(const boost::filesystem::wpath& path)
		{
			const XML::XDocument document(path / s_filename);
			Default		= document.Root;
			Override	= document.Root;
		}


		void File::write(const boost::filesystem::wpath& path) const
		{
			XML::XElement("Types",
				XML::XNamespace("xmlns", "http://schemas.openxmlformats.org/package/2006/content-types") +
				XML::Write(Default) +
				XML::Write(Override)
			).Save(path / s_filename);
		}


		const bool File::isValid() const
		{
			return true;
		}


		void File::registration(const std::string& type, const boost::filesystem::wpath& directory, const boost::filesystem::wpath& filename)
		{
			Override->add(type, directory / filename.filename());
			Default->add(directory / filename.filename());
		}

	} // namespace ContentTypes
} // namespace OOX