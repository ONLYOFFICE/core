#pragma once
#ifndef XML_PRIVATE_COMMENT_INCLUDE_H_
#define XML_PRIVATE_COMMENT_INCLUDE_H_

#include "Node.h"
#include <string>
#include "XString.h"
#include "property.h"


namespace XML
{
	namespace Private
	{
		class Comment : public Node
		{
		public:
			explicit Comment(const std::string& value);
			virtual ~Comment();

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
			Comment();
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_COMMENT_INCLUDE_H_