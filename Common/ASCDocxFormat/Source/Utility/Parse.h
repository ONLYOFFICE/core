#pragma once
#ifndef UTILITY_PARSE_INCLUDE_H_
#define UTILITY_PARSE_INCLUDE_H_

#include <string>

template<typename T> const T Parse(const std::string& str) {return str;}
template<typename T> const T Parse(const std::wstring& str) {return str;}

template<> const bool Parse<bool>(const std::string& str);
template<> const int Parse<int>(const std::string& str);
template<> const size_t Parse<size_t>(const std::string& str);
template<> const double Parse<double>(const std::string& str);
template<> const std::wstring Parse<std::wstring>(const std::string& str);

template<> const bool Parse<bool>(const std::wstring& str);
template<> const int Parse<int>(const std::wstring& str);
template<> const size_t Parse<size_t>(const std::wstring& str);
template<> const double Parse<double>(const std::wstring& str);
template<> const std::string Parse<std::string>(const std::wstring& str);

const int HexChar2Int(const char value);
const int HexString2Int(const std::string& value);
const int HexString2IntW(const std::wstring& value);

#endif // UTILITY_PARSE_INCLUDE_H_