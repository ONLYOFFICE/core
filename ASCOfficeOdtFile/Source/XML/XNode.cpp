
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "XNode.h"
#include "XText.h"
#include "XComment.h"
#include "XElement.h"
#include "XContainer.h"


namespace XML
{

	XNode::XNode()
	{
	}


	XNode::XNode(const XText& text)
		: base(text)
	{
	}


	XNode::XNode(const XElement& element)
		: base(element)
	{
	}


	XNode::XNode(const XComment& comment)
		: base(comment)
	{
	}


	XNode::XNode(const XContainer& container)
		: base(container)
	{
	}


	const bool XNode::isElement() const
	{
		return m_ptr->isElement();
	}


	const bool XNode::isText() const
	{
		return m_ptr->isText();
	}


	const bool XNode::isComment() const
	{
		return m_ptr->isComment();
	}


	const std::string XNode::ToString() const
	{
		return m_ptr->ToString();
	}


	const std::wstring XNode::ToWString() const
	{
		return m_ptr->ToWString();
	}


	void XNode::SaveToStringList(std::list<std::string>& strList)const
	{
		m_ptr->SaveToStringList(strList);
	}


	void XNode::SaveToWStringList(std::list<std::wstring>& strList)const
	{
		m_ptr->SaveToWStringList(strList);
	}

} // namespace XML