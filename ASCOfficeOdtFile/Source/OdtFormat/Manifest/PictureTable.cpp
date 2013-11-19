
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "PictureTable.h"
#include <boost/foreach.hpp>
#include <algorithm>
#include <boost/bind.hpp>
#include "exception\not_implement.h"
#include "Exception/log_range_error.h"


namespace Odt
{
	namespace Manifest
	{

		PictureTable::PictureTable()
		{
		}

		
		PictureTable::~PictureTable()
		{
		}

		
		PictureTable::PictureTable(const XML::XNode& node)
		{
			fromXML(node);
		}
		
		
		const PictureTable& PictureTable::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void PictureTable::fromXML(const XML::XNode& node)
		{
			throw not_implement("PictureTable fromXML");
		}
		
		
		const XML::XNode PictureTable::toXML() const
		{
			throw not_implement("PictureTable toXML");
		}


		const XML::XNode PictureTable::toXML(const boost::filesystem::wpath& path) const
		{
			return XML::Write(m_table, path);
		}


		const boost::filesystem::wpath PictureTable::picture(const boost::filesystem::wpath& filename) const
		{
			std::vector<PictureItem>::const_iterator iter = std::find_if(m_table.begin(), m_table.end(),
																			boost::bind(&PictureItem::filename, _1) == filename.filename());
			if (iter == m_table.end())
			{
				if (!m_table.empty())
					return m_table.front().path();
				throw log_range_error("picture table");
			}
			return iter->path();
		}


		bool PictureTable::hasPicture(const boost::filesystem::wpath& filename) const
		{
			std::vector<PictureItem>::const_iterator iter = std::find_if(m_table.begin(), m_table.end(),
																			boost::bind(&PictureItem::filename, _1) == filename.filename());
			if (iter == m_table.end())
				return false;
			return true;
		}


		const boost::filesystem::wpath PictureTable::addPicture(const boost::filesystem::wpath& filename)
		{
			m_table.push_back(filename);
			return L"Pictures/" + filename.filename();
		}


		void PictureTable::copy(const boost::filesystem::wpath& path) const
		{
			std::for_each(m_table.begin(), m_table.end(), boost::bind(&PictureItem::copy, _1, path));
		}

	} // namespace Manifest
} // namespace Odt