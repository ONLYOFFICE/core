
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Image.h"
#include "..\FileTypes.h"
#include <boost/algorithm/string/replace.hpp>
//#include "OfficeSvmFile.h"
//#include "SvmConverter.h"
#include "FileNameUtility.h"


namespace OOX
{

	Image::Image()
	{
	}


	Image::Image(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	Image::~Image()
	{
	}


	void Image::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		std::wstring newFilename = filename.filename();
		boost::filesystem::wpath newFilePath = filename.parent_path();
		boost::replace_all(newFilename, L" ", L"_");
		if (boost::filesystem::exists(m_filename) && !boost::filesystem::exists(newFilePath/newFilename))	
		{
			//if (m_filename.extension() == L".svm")
			//{
			//	ConvertSvmToImage(m_filename, replace_extension(newFilePath/newFilename, L"png"));
			//}
			//else 
			if (m_filename.extension() != L"")
			{
				boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
			}
			else
			{
				newFilename += L".wmf";
				boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
			}
		}
		//if (m_filename.extension() == L".svm")
		//	content.Default->add(replace_extension(newFilePath/newFilename, L"png"));
		//else	
			content.Default->add(newFilePath/newFilename);
	}


	const FileType Image::type() const
	{
		return FileTypes::Image;
	}


	const boost::filesystem::wpath Image::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath Image::DefaultFileName() const
	{
		return m_filename.filename();
	}

} // namespace OOX