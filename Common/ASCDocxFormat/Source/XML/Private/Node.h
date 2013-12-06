#pragma once
#ifndef XML_PRIVATE_NODE_INCLUDE_H_
#define XML_PRIVATE_NODE_INCLUDE_H_

#include <string>
#include <list>

namespace XML
{
	namespace Private
	{
		class Node
		{
		public:
			virtual ~Node() = 0 {};

		public:
			virtual const bool isElement() const = 0;
			virtual const bool isText() const = 0;
			virtual const bool isComment() const = 0;

		public:
			virtual const std::string ToString() const = 0;
			virtual const std::wstring ToWString() const = 0;
			virtual void SaveToStringList(std::list<std::string>& strList)const = 0;
			virtual void SaveToWStringList(std::list<std::wstring>& strList)const = 0;
		};
	} // namespace Private
} // namespace XML

#endif // XML_PRIVATE_NODE_INCLUDE_H_