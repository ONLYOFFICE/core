#ifndef ODT_MANIFEST_TYPETABLE_INCLUDE_H_
#define ODT_MANIFEST_TYPETABLE_INCLUDE_H_

#include <boost/utility.hpp>
#include <string>
#include <map>
#include "boost_filesystem_version.h"


namespace Odt
{
	namespace Manifest
	{
		class TypeTable : private boost::noncopyable
		{
		public:
			TypeTable();
			const std::string type(const boost::filesystem::wpath& path) const;
			const std::string operator[](const boost::filesystem::wpath& path) const;

		private:
			std::map<std::wstring, std::string> m_table;
		};
	} // namespace Manifest
} // namespace Odt


#endif // ODT_MANIFEST_TYPETABLE_INCLUDE_H_