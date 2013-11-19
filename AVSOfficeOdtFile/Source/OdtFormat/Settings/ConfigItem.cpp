
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ConfigItem.h"
#include "ConfigItemBool.h"
#include "COnfigItemShort.h"
#include "ConfigItemInt.h"
#include "ConfigItemString.h"
#include "ConfigItemBinary.h"
#include "./../FormatError.h"


namespace Odt
{
	namespace Settings
	{

		ConfigItem::ConfigItem()
		{
		}


		ConfigItem::~ConfigItem()
		{
		}


		ConfigItem::ConfigItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ConfigItem& ConfigItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ConfigItem::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			std::string strTypeValue = element->attribute("type").value().ToString();
			if (strTypeValue == "boolean")
					m_item.reset(new ConfigItemBool(node));
			else if (strTypeValue == "short")
				m_item.reset(new ConfigItemShort(node));
			else if ((strTypeValue == "int") || (strTypeValue == "long"))
				m_item.reset(new ConfigItemInt(node));
			else if (strTypeValue == "string")
				m_item.reset(new ConfigItemString(node));
			else if (strTypeValue == "base64Binary")
				m_item.reset(new ConfigItemBinary(node));
			else
				throw FormatError("bad element type");
		}


		const XML::XNode ConfigItem::toXML() const
		{
			return m_item->toXML();
		}

	} // namespace Settings
} // namespace Odt