#include "CNode.h"

namespace NSCSS
{
	CNode::CNode()
	{}

	CNode::CNode(const std::wstring& wsName, const std::wstring& wsClass, const std::wstring& wsId)
		: m_wsName(wsName), m_wsClass(wsClass), m_wsId(wsId)
	{}

	bool CNode::Empty() const
	{
		return m_wsName.empty() && m_wsClass.empty() && m_wsId.empty() && m_wsStyle.empty();
	}

	std::vector<std::wstring> CNode::GetData() const
	{
		std::vector<std::wstring> arValues;
		if (!m_wsClass.empty())
			arValues.push_back(m_wsClass);
		if (!m_wsName.empty())
			arValues.push_back(m_wsName);
		return arValues;
	}

	bool CNode::operator<(const CNode& oNode) const
	{
		if(m_wsName != oNode.m_wsName)
			return m_wsName < oNode.m_wsName;

		if(m_wsClass != oNode.m_wsClass)
			return m_wsClass < oNode.m_wsClass;

		if(m_wsId != oNode.m_wsId)
			return m_wsId < oNode.m_wsId;

		if(m_wsStyle != oNode.m_wsStyle)
			return m_wsStyle < oNode.m_wsStyle;

		if (m_mAttributes != oNode.m_mAttributes)
			return m_mAttributes < oNode.m_mAttributes;

		return false;
	}

	bool CNode::operator==(const CNode& oNode) const
	{
		return((m_wsId == oNode.m_wsId)       &&
			   (m_wsName == oNode.m_wsName)   &&
			   (m_wsClass == oNode.m_wsClass) &&
			  (m_wsStyle == oNode.m_wsStyle) &&
			  (m_mAttributes == oNode.m_mAttributes));
	}
}
