
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "XmlTable.h"
#include "exception\not_implement.h"


namespace Odt
{
	namespace Manifest
	{

		XmlTable::XmlTable()
		{
		}

		
		XmlTable::~XmlTable()
		{
		}


		XmlTable::XmlTable(const XML::XNode& node)
		{
			fromXML(node);
		}


		const XmlTable& XmlTable::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void XmlTable::fromXML(const XML::XNode& node)
		{
			throw not_implement("XMLTable fromXML");
		}


		const XML::XNode XmlTable::toXML() const
		{
			return XML::Write(m_table);
		}


		void XmlTable::add(const boost::filesystem::wpath& path, const boost::filesystem::wpath& filename)
		{
			if (boost::filesystem::exists(path/filename))
				m_table.push_back(XmlItem(filename));
		}

	} // namespace Manifest
} // namespace Odt