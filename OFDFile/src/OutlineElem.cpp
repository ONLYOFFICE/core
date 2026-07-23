#include "OutlineElem.h"

namespace OFD
{
COutlineElem::COutlineElem(CXmlReader& oReader)
	: m_nCount(0), m_bExpanded(true)
{
	if (oReader.MoveToFirstAttribute())
	{
		std::string sName;
		do
		{
			sName = oReader.GetNameA();

			if ("Title" == sName)
				m_wsTitle = oReader.GetText();
			else if ("Count" == sName)
				m_nCount = oReader.GetInteger(true);
			else if ("Expanded" == sName)
				m_bExpanded = oReader.GetBoolean(true);
		}while (oReader.MoveToNextAttribute());

		oReader.MoveToElement();
	}

	const int nDepth{oReader.GetDepth()};

	while (oReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:Actions" == oReader.GetNameA())
		{
			const int nActionDepth{oReader.GetDepth()};

			while (oReader.ReadNextSiblingNode(nActionDepth))
			{
				if("ofd:Action" == oReader.GetNameA())
					AddAction(new CAction(oReader));
			}
		}
		else if ("ofd:OutlineElem" == oReader.GetNameA())
			AddOutlineElem(new COutlineElem(oReader));
	}
}

COutlineElem::~COutlineElem()
{
	for (const CAction* pAction : m_arActions)
		delete pAction;
}

void COutlineElem::AddAction(const CAction* pAction)
{
	if (nullptr != pAction)
		m_arActions.push_back(pAction);
}

void COutlineElem::AddOutlineElem(const COutlineElem* pOutlineElem)
{
	if (nullptr != pOutlineElem)
		m_arOutlines.push_back(pOutlineElem);
}

std::wstring OFD::COutlineElem::GetTitle() const
{
	return m_wsTitle;
}

std::vector<const CAction*> COutlineElem::GetActions() const
{
	return m_arActions;
}

std::vector<const COutlineElem*> COutlineElem::GetOutlines() const
{
	return m_arOutlines;
}

}
