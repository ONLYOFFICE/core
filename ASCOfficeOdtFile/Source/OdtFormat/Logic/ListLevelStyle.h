#pragma once
#ifndef ODT_CONTENT_LIST_LEVEL_STYLE_INCLUDE_H_
#define ODT_CONTENT_LIST_LEVEL_STYLE_INCLUDE_H_

#include "./../WritingElement.h"
#include <boost/shared_ptr.hpp>
#include "ListLevelStyleItem.h"
#include "Properties.h"


namespace Odt
{
	namespace Logic
	{
		class ListLevelStyleNumber;
		class ListLevelStyleBullet;
		class ListLevelStyleImage;

		class ListLevelStyle : public WritingElement
		{
		public:
			ListLevelStyle();
			virtual ~ListLevelStyle();
			ListLevelStyle(const ListLevelStyleNumber& listLevelStyleNumber);
			ListLevelStyle(const ListLevelStyleBullet& listLevelStyleBullet);
			ListLevelStyle(const ListLevelStyleImage& listLevelStyleImage);
			explicit ListLevelStyle(const XML::XNode& node);
			const ListLevelStyle& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);		
			virtual const XML::XNode toXML() const;			

		public:
			template<class T>	const bool is() const {	return typeid(*m_item) == typeid(T);}
			template<class T> T& as() {return static_cast<T&>(*m_item);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_item);}

		public:
			const int				 getLevel() const;
			const Properties getProperties() const;

		private:
			boost::shared_ptr<ListLevelStyleItem> m_item;
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_CONTENT_LIST_LEVEL_STYLE_INCLUDE_H_