
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TypeTable.h"


namespace Odt
{
	namespace Manifest
	{

		TypeTable::TypeTable()
		{
			m_table.insert(std::make_pair(L".gif", "image/gif"));
			m_table.insert(std::make_pair(L".jpg", "image/jpeg"));
			m_table.insert(std::make_pair(L".jpeg", "image/jpeg"));
			m_table.insert(std::make_pair(L".png", "image/png"));
			m_table.insert(std::make_pair(L".tif", "image/tiff"));
			m_table.insert(std::make_pair(L".emf", "image/emf"));
			m_table.insert(std::make_pair(L".wmf", "image/wmf"));
			m_table.insert(std::make_pair(L".xml", "text/xml"));
		}


		const std::string TypeTable::type (const boost::filesystem::wpath& path) const
		{
			std::map<std::wstring, std::string>::const_iterator found = m_table.find(path.extension());
			if (found == m_table.end())
				return "";
			return found->second;
		}


		const std::string TypeTable::operator[](const boost::filesystem::wpath& path) const
		{
			return type(path);
		}

	} // namespace Manifest
} // namespace Odt