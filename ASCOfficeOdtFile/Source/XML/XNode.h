#pragma once
#ifndef XML_XNODE_INCLUDE_H_
#define XML_XNODE_INCLUDE_H_

#include "Private/XPointer.h"
#include "Private/Node.h"
#include <string>
#include <list>


namespace XML
{
	class XText;
	class XElement;
	class XComment;
	class XContainer;

	class XNode : public Private::XPointer<Private::Node>
	{
	public:
		XNode();
		XNode(const XText& text);
		XNode(const XElement& element);
		XNode(const XComment& comment);
		XNode(const XContainer& container);

	public:
		const bool isElement() const;
		const bool isText() const;
		const bool isComment() const;

	public:
		const std::string ToString() const;
		const std::wstring ToWString() const;
		virtual void SaveToStringList(std::list<std::string>& strList)const;
		virtual void SaveToWStringList(std::list<std::wstring>& strList)const;
	};
} // namespace XML

#endif // XML_XNODE_INCLUDE_H_