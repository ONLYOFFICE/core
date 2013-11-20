#pragma once
#ifndef OOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_
#define OOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_

#include <boost/utility.hpp>
#include <string>
#include <map>


namespace OOX
{
	namespace ContentTypes
	{
		class ExtensionTable : private boost::noncopyable
		{
		public:
			ExtensionTable();
			const std::string& operator[] (const std::wstring& extention) const;

		private:
			std::map<std::wstring, std::string>	m_table;
		};
	} // namespace ContentTypes
} // namespace OOX

#endif // OOX_CONTENT_TYPES_EXTENSION_TABLE_INCLUDE_H_