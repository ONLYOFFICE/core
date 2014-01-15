
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Media.h"
#include "..\FileTypes.h"
//#include "OfficeSvmFile.h"
//#include "SvmConverter.h"
#include "FileNameUtility.h"
#include <boost/algorithm/string/replace.hpp>


namespace OOX
{

	Media::Media()
	{
	}


	Media::Media(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	Media::~Media()
	{
	}


	void Media::read(const boost::filesystem::wpath& filename)
	{
		m_filename = filename;
	}


	void Media::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		std::wstring newFilename = filename.filename();
		boost::filesystem::wpath newFilePath = filename.parent_path();
		boost::replace_all(newFilename, L" ", L"_");
		if (boost::filesystem::exists(m_filename) && !boost::filesystem::exists(newFilePath/newFilename))	
		{
			//if (m_filename.extension() == L".svm")
			//	ConvertSvmToImage(m_filename, replace_extension(newFilePath/newFilename, L"png"));
			//else
				boost::filesystem::copy_file(m_filename, newFilePath/newFilename);
		}
		//if (m_filename.extension() == L".svm")
		//	content.Default->add(replace_extension(newFilePath/newFilename, L"png"));
		//else
			content.Default->add(m_filename);
	}


	const boost::filesystem::wpath Media::filename() const
	{
		return m_filename;
	}


	void Media::copy_to(const boost::filesystem::wpath& path) const
	{		
		std::wstring newFilename = m_filename.filename();
		boost::replace_all(newFilename, L" ", L"_");

		if (boost::filesystem::exists(m_filename) && !boost::filesystem::exists(path/m_filename.filename()))
			boost::filesystem::copy_file(m_filename, path/newFilename);
	}

} // namespace OOX