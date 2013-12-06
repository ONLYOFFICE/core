
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableStyle.h"
#include <algorithm>

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
			for (IEnumerable<OOX::Logic::TableStyleProperties>::const_iterator iter = begin(); iter != end(); ++iter)
			{
				if ((*iter).Type == type)
				{
					return true;
				}
			}

			return false;
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
			IEnumerable<OOX::Logic::TableStyleProperties>::const_iterator iter = begin();
			for (; iter != end(); ++iter)
			{
				if ((*iter).Type == type)
				{
					return (*iter);
				}
			}

			return (*iter);
		}

		TableStyleProperties& TableStyle::find(const std::string& type)
		{
			IEnumerable<OOX::Logic::TableStyleProperties>::iterator iter = begin();
			for (; iter != end(); ++iter)
			{
				if ((*iter).Type == type)
				{
					return (*iter);
				}
			}

			return (*iter);
		}
	}//namespace Logic
} // namespace OOX