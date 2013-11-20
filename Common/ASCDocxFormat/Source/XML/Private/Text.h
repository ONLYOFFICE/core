#pragma once
#ifndef XML_PRIVATE_TEXT_INCLUDE_H_
#define XML_PRIVATE_TEXT_INCLUDE_H_

#include "Node.h"
#include <string>
#include "XString.h"
#include "property.h"


namespace XML
{
	namespace Private
	{
		class Text : public Node
		{
		public:
			explicit Text(const std::string& value);
			virtual ~Text();

		public:
			virtual const bool isElement() const;
			virtual const bool isText() const;
			virtual const bool isComment() const;

		public:
			virtual const std::string ToString() const;
			virtual const std::wstring ToWString() const;
			virtual void SaveToStringList(std::list<std::string>& strList)const;
			virtual void SaveToWStringList(std::list<std::wstring>& strList)const;

		public:
			XString Value;

		protected:
			Text();
		};
	} // namespace Private
} // namespace XML

#endif // XML_XTEXT_INCLUDE_H_