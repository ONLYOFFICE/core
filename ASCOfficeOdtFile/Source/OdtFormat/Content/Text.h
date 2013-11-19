#pragma once
#ifndef ODT_CONTENT_TEXT_INCLUDE_H_
#define ODT_CONTENT_TEXT_INCLUDE_H_

#include "./../WritingElement.h"
#include <boost/shared_ptr.hpp>
#include "TextItem.h"


namespace Odt
{
	namespace Content
	{
		class Paragraph;
		class Table;
		class List;
		class TableOfContent;
		class IndexTitle;
		class Section;

		class Text : public WritingElement
		{
		public:
			Text();
			virtual ~Text();
			Text(const Paragraph& paragraph);
			Text(const Table& table);
			Text(const List& list);
			Text(const TableOfContent& tableOfContent);
			Text(const IndexTitle& indexTitle);
			Text(const Section& section);
			explicit Text(const XML::XNode& node);
			const Text& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool hasNote() const;
			const bool hasList() const;

		public:
			template<class T>	const bool is() const {	return typeid(*m_item) == typeid(T);}
			template<class T> T& as() {return static_cast<T&>(*m_item);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_item);}

		private:
			boost::shared_ptr<TextItem>		m_item;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TEXT_INCLUDE_H_