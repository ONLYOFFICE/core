#pragma once
#ifndef OOX_LOGIC_LIST_INCLUDE_H_
#define OOX_LOGIC_LIST_INCLUDE_H_

//#include "TextItemBase.h"
#include <vector>
//#include "ListItem.h"
//#include "TextItem.h"
#include "Paragraph.h"


namespace OOX
{
	namespace Logic
	{
		class List //: public TextItemBase
		{
		public:
			List();
			virtual ~List();
			//explicit List(const XML::XNode& node);
			//const List& operator =(const XML::XNode& node);

		//public:
		//	virtual void fromXML(const XML::XNode& node);
		//	virtual const XML::XNode toXML() const;

		public:
			//void add(const XML::XNode& node);//добавление элемента с номером
			//void add(const TextItem& text);	 //добавление элемента без номера
			void add(const Paragraph& text);

		public:
			const bool elementFromThisList(const XML::XNode& node) const;

		public:			
			property<std::vector<Paragraph> >	Items;				
			//property<int>											Level;
			property<int>											Num;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_LIST_INCLUDE_H_