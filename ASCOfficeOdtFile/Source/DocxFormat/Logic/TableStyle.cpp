
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableStyle.h"
#include <algorithm>
#include <boost/bind.hpp>

namespace OOX
{
	namespace Logic
	{
		TableStyle::TableStyle()
		{

		}

		TableStyle::~TableStyle()
		{

		}

		TableStyle::TableStyle(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableStyle& TableStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(m_items, element, "tblStylePr");
		}

		const bool TableStyle::exist(const std::string& type) const
		{
			return std::find_if(begin(), end(), boost::bind(&TableStyleProperties::Type, _1) == type) != end();
		}

		const TableStyleProperties& TableStyle::operator[](const std::string& type) const
		{
			return find(type);
		}

		TableStyleProperties& TableStyle::operator[](const std::string& type)
		{
			return find(type);
		}

		const TableStyleProperties& TableStyle::find(const std::string& type) const
		{
			return *Odt::find_if(begin(), end(), boost::bind(&TableStyleProperties::Type, _1) == type);
			//return *std::find_if(begin(), end(), boost::bind(&TableStyleProperties::Type, _1) == type);
		}

		TableStyleProperties& TableStyle::find(const std::string& type)
		{
			return *Odt::find_if(begin(), end(), boost::bind(&TableStyleProperties::Type, _1) == type);
			//return *std::find_if(begin(), end(), boost::bind(&TableStyleProperties::Type, _1) == type);
		}
	}//namespace Logic
} // namespace OOX