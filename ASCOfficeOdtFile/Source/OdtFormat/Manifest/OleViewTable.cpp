
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "OleViewTable.h"
#include <boost/foreach.hpp>
#include <algorithm>
#include <boost/bind.hpp>
#include "exception\not_implement.h"
#include "Exception/log_range_error.h"


namespace Odt
{
	namespace Manifest
	{

		OleViewTable::OleViewTable()
		{
		}

		
		OleViewTable::~OleViewTable()
		{
		}

		
		OleViewTable::OleViewTable(const XML::XNode& node)
		{
			fromXML(node);
		}
		
		
		const OleViewTable& OleViewTable::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void OleViewTable::fromXML(const XML::XNode& node)
		{
			throw not_implement("OleViewTable fromXML");
		}
		
		
		const XML::XNode OleViewTable::toXML() const
		{
			throw not_implement("OleViewTable toXML");
		}


		const XML::XNode OleViewTable::toXML(const boost::filesystem::wpath& path) const
		{
			return XML::Write(m_table, path);
		}


		const boost::filesystem::wpath OleViewTable::oleView(const boost::filesystem::wpath& filename) const
		{
			std::vector<OleViewItem>::const_iterator iter = std::find_if(m_table.begin(), m_table.end(),
																			boost::bind(&OleViewItem::filename, _1) == filename.filename());
			if (iter == m_table.end())
			{
				if (!m_table.empty())
					return m_table.front().path();
				throw log_range_error("ole table");
			}
			return iter->path();
		}


		const boost::filesystem::wpath OleViewTable::addOleView(const boost::filesystem::wpath& filename)
		{
			m_table.push_back(filename);
			return filename.filename();
		}


		void OleViewTable::copy(const boost::filesystem::wpath& path) const
		{
			std::for_each(m_table.begin(), m_table.end(), boost::bind(&OleViewItem::copy, _1, path));
		}

	} // namespace Manifest
} // namespace Odt