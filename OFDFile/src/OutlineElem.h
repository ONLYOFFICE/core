#ifndef OUTLINEELEM_H
#define OUTLINEELEM_H

#include "Action.h"

namespace OFD
{
class COutlineElem
{
	std::wstring m_wsTitle;
	int          m_nCount;
	bool         m_bExpanded;

	std::vector<const CAction*> m_arActions;
	std::vector<const COutlineElem*> m_arOutlines;
public:
	COutlineElem(CXmlReader& oReader);
	~COutlineElem();

	std::wstring GetTitle() const;

	std::vector<const CAction*>      GetActions()  const;
	std::vector<const COutlineElem*> GetOutlines() const;
private:
	void AddAction(const CAction* pAction);
	void AddOutlineElem(const COutlineElem* pOutlineElem);
};
}

#endif // OUTLINEELEM_H
