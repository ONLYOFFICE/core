#include "CNode.h"

#ifdef CSS_CALCULATOR_WITH_XHTML
#include "CCompiledStyle.h"
#endif

namespace NSCSS
{
	CNode::CNode()
	#ifdef CSS_CALCULATOR_WITH_XHTML
		: m_pCompiledStyle(new CCompiledStyle())
    #endif
	{}

	CNode::CNode(const CNode& oNode)
		: m_wsName(oNode.m_wsName), m_wsClass(oNode.m_wsClass), m_wsId(oNode.m_wsId),
	      m_wsStyle(oNode.m_wsStyle), m_mAttributes(oNode.m_mAttributes)
	{
		#ifdef CSS_CALCULATOR_WITH_XHTML
		m_pCompiledStyle = new CCompiledStyle();
		*m_pCompiledStyle = *oNode.m_pCompiledStyle;
		#endif
	}

	CNode::CNode(const std::wstring& wsName, const std::wstring& wsClass, const std::wstring& wsId)
		: m_wsName(wsName), m_wsClass(wsClass), m_wsId(wsId)
	    #ifdef CSS_CALCULATOR_WITH_XHTML
	    , m_pCompiledStyle(new CCompiledStyle())
		#endif
	{}

	CNode::~CNode()
	{
		#ifdef CSS_CALCULATOR_WITH_XHTML
		if (nullptr != m_pCompiledStyle)
			delete m_pCompiledStyle;
		#endif
	}

	bool CNode::Empty() const
	{
		return m_wsName.empty() && m_wsClass.empty() && m_wsId.empty() && m_wsStyle.empty();
	}

	#ifdef CSS_CALCULATOR_WITH_XHTML
	void CNode::SetCompiledStyle(CCompiledStyle* pCompiledStyle)
	{
		if (nullptr != m_pCompiledStyle)
			delete m_pCompiledStyle;

		m_pCompiledStyle = new CCompiledStyle();
		*m_pCompiledStyle = *pCompiledStyle;
	}
	#endif

	void CNode::Clear()
	{
		m_wsName     .clear();
		m_wsClass    .clear();
		m_wsId       .clear();
		m_wsStyle    .clear();
		m_mAttributes.clear();
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
