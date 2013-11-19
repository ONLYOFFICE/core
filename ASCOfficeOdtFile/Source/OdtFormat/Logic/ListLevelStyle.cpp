
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "./../FormatError.h"
#include "ListLevelStyle.h"
#include "ListLEvelStyleBullet.h"
#include "ListLevelStyleNumber.h"
#include "ListLevelStyleImage.h"


namespace Odt
{
	namespace Logic
	{

		ListLevelStyle::ListLevelStyle()
		{
		}


		ListLevelStyle::~ListLevelStyle()
		{
		}


		ListLevelStyle::ListLevelStyle(const ListLevelStyleNumber& listLevelStyleNumber)
			: m_item(new ListLevelStyleNumber(listLevelStyleNumber))
		{
		}
		
		
		ListLevelStyle::ListLevelStyle(const ListLevelStyleBullet& listLevelStyleBullet)
			: m_item(new ListLevelStyleBullet(listLevelStyleBullet))
		{
		}


		ListLevelStyle::ListLevelStyle(const ListLevelStyleImage& listLevelStyleImage)
			: m_item(new ListLevelStyleImage(listLevelStyleImage))
		{
		}


		ListLevelStyle::ListLevelStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ListLevelStyle& ListLevelStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}		


		void ListLevelStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			if (element->XName == "list-level-style-bullet")
				m_item.reset(new ListLevelStyleBullet(node));
			else if (element->XName == "list-level-style-number")
				m_item.reset(new ListLevelStyleNumber(node));
			else if (element->XName == "list-level-style-image")
				m_item.reset(new ListLevelStyleImage(node));
			else
				throw FormatError("not find list lebel style");
		}


		const XML::XNode ListLevelStyle::toXML() const
		{
			return m_item->toXML();
		}


		const int	ListLevelStyle::getLevel() const
		{
			return m_item->Level;
		}


		const Properties ListLevelStyle::getProperties() const
		{
			return m_item->Properties;
		}
	} // namespace Logic
} // namespace Odt