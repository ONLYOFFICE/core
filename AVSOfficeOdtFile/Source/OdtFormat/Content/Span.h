#pragma once
#ifndef ODT_CONTENT_SPAN_INCLUDE_H_
#define ODT_CONTENT_SPAN_INCLUDE_H_

#include "ParagraphItemBase.h"
#include "property.h"
#include <string>
#include <vector>
#include "nullable_property.h"
#include "ParagraphItem.h"


namespace Odt
{
	namespace Content
	{
		class Span : public ParagraphItemBase
		{
		public:
			Span();
			virtual ~Span();
			explicit Span(const XML::XNode& node);
			explicit Span(const std::string& text);
			Span(const std::string& style, const std::string& text);
			const Span& operator =(const XML::XNode& node);
			const Span& operator =(const std::string& text);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual void fromTxt(const std::string& text);
			void fromTxt(const std::string& style, std::string text);
			virtual const XML::XNode toXML() const;
			virtual const std::string toTxt() const;

		public:
			template<typename T> void add() {Items->push_back(ParagraphItem(new T()));}
			template<typename T> void add(const T& item) {Items->push_back(ParagraphItem(new T(item)));};

		public:
			nullable_property<std::string>					Style;
			property<std::vector<ParagraphItem> >		Items;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TEXT_INCLUDE_H_