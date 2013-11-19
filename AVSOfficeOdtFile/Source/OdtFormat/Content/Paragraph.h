#pragma once
#ifndef ODT_CONTENT_PARAGRAPH_INCLUDE_H_
#define ODT_CONTENT_PARAGRAPH_INCLUDE_H_

#include "TextItem.h"
#include <vector>
#include "ParagraphItem.h"
#include "property.h"
#include "nullable.h"


namespace Odt
{
	namespace Content
	{
		class Paragraph : public TextItem
		{
		public:
			Paragraph();
			virtual ~Paragraph();
			explicit Paragraph(const XML::XNode& node);
			const Paragraph& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			void AddText(const std::string& text);

		public:
			virtual const bool hasList() const;
			virtual const bool hasNote() const;

		public:
			nullable_property<std::string>					Style;
			property<std::vector<ParagraphItem> >		Items;
			
		private:
			std::string			m_type;
			nullable<int>		m_outlineLevel;
			bool						m_hasNote;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_PARAGRAPH_INCLUDE_H_