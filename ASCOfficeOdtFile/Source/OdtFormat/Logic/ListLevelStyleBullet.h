#pragma once
#ifndef ODT_LOGIC_LIST_LEVEL_STYLE_BULLET_INCLUDE_H_
#define ODT_LOGIC_LIST_LEVEL_STYLE_BULLET_INCLUDE_H_

#include "ListLevelStyleItem.h"
#include <string>
#include "property.h"


namespace Odt
{
	namespace Logic
	{
		class ListLevelStyleBullet : public ListLevelStyleItem
		{
		public:
			ListLevelStyleBullet();
			virtual ~ListLevelStyleBullet();
			explicit ListLevelStyleBullet(const XML::XNode& node);
			const ListLevelStyleBullet& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string>						Bullet;			
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_LIST_LEVEL_STYLE_BULLET_INCLUDE_H_