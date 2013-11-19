
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "OleObjectTable.h"
#include <boost/foreach.hpp>
#include <algorithm>
#include <boost/bind.hpp>
#include "exception\not_implement.h"
#include "Exception/log_range_error.h"


namespace Odt
{
	namespace Manifest
	{

		OleObjectTable::OleObjectTable()
		{
		}

		
		OleObjectTable::~OleObjectTable()
		{
		}

		
		OleObjectTable::OleObjectTable(const XML::XNode& node)
		{
			fromXML(node);
		}
		
		
		const OleObjectTable& OleObjectTable::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void OleObjectTable::fromXML(const XML::XNode& node)
		{
			throw not_implement("OleObjectTable fromXML");
		}
		
		
		const XML::XNode OleObjectTable::toXML() const
		{
			throw not_implement("OleObjectTable toXML");
		}


		const XML::XNode OleObjectTable::toXML(const boost::filesystem::wpath& path) const
		{
			return XML::Write(m_table, path);
		}


		const boost::filesystem::wpath OleObjectTable::oleObject(const boost::filesystem::wpath& filename) const
		{
			std::vector<OleObjectItem>::const_iterator iter = std::find_if(m_table.begin(), m_table.end(),
																			boost::bind(&OleObjectItem::filename, _1) == filename.filename());
			if (iter == m_table.end())
			{
				if (!m_table.empty())
					return m_table.front().path();
				throw log_range_error("ole object table");
			}
			return iter->path();
		}


		const boost::filesystem::wpath OleObjectTable::addOleObject(const boost::filesystem::wpath& filename)
		{
			m_table.push_back(filename);
			return filename.filename();
		}


		void OleObjectTable::copy(const boost::filesystem::wpath& path) const
		{
			std::for_each(m_table.begin(), m_table.end(), boost::bind(&OleObjectItem::copy, _1, path));
		}

	} // namespace Manifest
} // namespace Odt