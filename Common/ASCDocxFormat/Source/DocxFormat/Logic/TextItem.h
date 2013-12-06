#pragma once
#ifndef OOX_LOGIC_TEXT_ITEM_INCLUDE_H_
#define OOX_LOGIC_TEXT_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include "TextItemBase.h"

#include "../../../../../Common/DocxFormat/Source/Base/SmartPtr.h"
#include "../.././../../Common/DocxFormat/Source/Base/Nullable.h"
#include "../.././../../Common/DocxFormat/Source/Xml/XmlUtils.h"

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
			TextItem(const XML::XNode& node);
			const TextItem& operator =(const XML::XNode& node);
			TextItem(XmlUtils::CXmlNode& node);
			const TextItem& operator =(XmlUtils::CXmlNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			virtual void fromXML(XmlUtils::CXmlNode& node);

		public:
			template<class T> const bool is() const {return typeid(*m_item) == typeid(T);}
			template<class T> T& as() {return static_cast<T&>(*m_item);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_item);}

		private:
			NSCommon::smart_ptr<TextItemBase> m_item;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TEXT_ITEM_INCLUDE_H_