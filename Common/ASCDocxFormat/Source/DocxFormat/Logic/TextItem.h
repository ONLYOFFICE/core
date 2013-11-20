#pragma once
#ifndef OOX_LOGIC_TEXT_ITEM_INCLUDE_H_
#define OOX_LOGIC_TEXT_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include <boost/shared_ptr.hpp>
#include "TextItemBase.h"

namespace OOX
{
	namespace Logic
	{
		class Paragraph;
		class Table;
		class Sdt;
		class BookmarkStartParagraph;
		class BookmarkEndParagraph;

		class TextItem : public WritingElement
		{
		public:
			TextItem();
			virtual ~TextItem();
			TextItem(const Paragraph& paragraph);
			TextItem(const Table& table);
			TextItem(const Sdt& sdt);
			TextItem(const BookmarkStartParagraph& bookmark);
			TextItem(const BookmarkEndParagraph& bookmark);
			explicit TextItem(const XML::XNode& node);
			const TextItem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			template<class T>	const bool is() const {	return typeid(*m_item) == typeid(T);}
			template<class T> T& as() {return static_cast<T&>(*m_item);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_item);}

			//public:			
			//	void inList();

		private:
			boost::shared_ptr<TextItemBase>		m_item;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TEXT_ITEM_INCLUDE_H_