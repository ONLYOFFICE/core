
// auto inserted precompiled begin
#include "precompiled_utility.h"
// auto inserted precompiled end

#include "FileNameUtility.h"
#include "ToString.h"


const boost::filesystem::path operator +(boost::filesystem::path filename, const int index)
{
	const std::string extension = filename.extension();
	return filename.replace_extension().string() + ToString(index) + extension;
}


const boost::filesystem::wpath operator +(boost::filesystem::wpath filename, const int index)
{
	const std::wstring extension = filename.extension();
	return filename.replace_extension().string() + ToWString(index) + extension;
}


const boost::filesystem::wpath replace_extension(const boost::filesystem::wpath& filename, const std::wstring& extension)
{
	boost::filesystem::wpath convert = filename;
	convert.replace_extension(extension);
	return convert;
}


const boost::filesystem::wpath replace_extension(const boost::filesystem::wpath& filename, const std::wstring& old_extension, const std::wstring& new_extension)
{
	if (filename.extension() == L"." + old_extension)
		return replace_extension(filename, new_extension);
	return filename;
}

const size_t max_name_index(const boost::filesystem::wpath& path, const std::wstring& filename)
{
	boost::filesystem::wpath file(filename);
	size_t index = 1;
	while(boost::filesystem::exists(path/(file + index)))
	{
		index++;
	}

	return index;
}


const std::wstring max_name(const boost::filesystem::wpath& path, const std::wstring& filename)
{
	boost::filesystem::wpath file(filename);
	file = file + max_name_index(path, filename);
	return file.string();
}
