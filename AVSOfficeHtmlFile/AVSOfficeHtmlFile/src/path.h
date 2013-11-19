#pragma once

#include <string>

std::wstring url_by_path( const std::wstring & path );

std::wstring full_from_relative( const std::wstring & doc_path,  const std::wstring & rel_path);

std::wstring set_slashes(const std::wstring & path);