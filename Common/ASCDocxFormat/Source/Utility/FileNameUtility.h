#pragma once
#ifndef UTILITY_FILE_NAME_UTILITY_INCLUDE_H_
#define UTILITY_FILE_NAME_UTILITY_INCLUDE_H_

#include <string>
#include <boost/filesystem.hpp>


const boost::filesystem::path operator +(boost::filesystem::path filename, const int index);
const boost::filesystem::wpath operator +(boost::filesystem::wpath filename, const int index);

const boost::filesystem::wpath replace_extension(const boost::filesystem::wpath& filename, const std::wstring& extension);
const boost::filesystem::wpath replace_extension(const boost::filesystem::wpath& filename, const std::wstring& old_extension, const std::wstring& new_extension);

const size_t max_name_index(const boost::filesystem::wpath& path, const std::wstring& filename);
const std::wstring max_name(const boost::filesystem::wpath& path, const std::wstring& filename);

#endif // UTILITY_FILE_NAME_UTILITY_INCLUDE_H_