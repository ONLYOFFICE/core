
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "File.h"
#include "./../FileTypes.h"


namespace OOX
{
	namespace Theme
	{

		File::File()
		{
		}


		File::File(const boost::filesystem::wpath& filename)
		{
			read(filename);
		}


		File::~File()
		{
		}


		void File::read(const boost::filesystem::wpath& filename)
		{
			const XML::XDocument document(filename);
			name = document.Root.attribute("name").value();
			themeElements			= document.Root.element("themeElements");
			objectDefaults		= document.Root.element("objectDefaults");
			extraClrSchemeLst = document.Root.element("extraClrSchemeLst");
		}


		void File::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
		{
			XML::XElement(ns.a + "theme",
				XML::XAttribute("name", name) +
				XML::Write(themeElements) +
				XML::Write(objectDefaults) +
				XML::Write(extraClrSchemeLst)
			).Save(filename);

			content.registration(type().OverrideType(), directory, filename);
		}


		const FileType File::type() const
		{
			return FileTypes::Theme;
		}


		const boost::filesystem::wpath File::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}


		const boost::filesystem::wpath File::DefaultFileName() const
		{
			return type().DefaultFileName();
		}


		const std::string File::GetMajorFont() const
		{
			if (themeElements.is_init() 
				&& themeElements->fontScheme.is_init()
				&& themeElements->fontScheme->majorFont.is_init())
				return themeElements->fontScheme->majorFont->latin.get_value_or("Times New Roman");
			else
				return "Times New Roman";
		}
		
		
		const std::string File::GetMinorFont() const
		{
			if (themeElements.is_init() 
				&& themeElements->fontScheme.is_init()
				&& themeElements->fontScheme->minorFont.is_init())
				return themeElements->fontScheme->minorFont->latin.get_value_or("Times New Roman");
			else
				return "Times New Roman";
		}

	} // namespace Theme
} // namespace OOX