#ifndef HWPPARGRAPH_H
#define HWPPARGRAPH_H

#include "LineSeg.h"
#include "RangeTag.h"
#include "Ctrl.h"

namespace HWP
{
class CHWPPargraph
{
	short m_shParaShapeID; // HWPTAG_PARA_HEADER
	short m_shParaStyleID; // HWPTAG_PARA_HEADER
	BYTE m_chBreakType;    // HWPTAG_PARA_HEADER

	CLineSeg *m_pLineSegs; // HWPTAG_PARA_LINE_SEG
	VECTOR<TRangeTag*> m_arRangeTags;  // HWPTAG_PARA_RANGE_TAG

	VECTOR<CCtrl*> m_arP; //HWPTAG_PARA_TEXT
public:
	CHWPPargraph();
	virtual ~CHWPPargraph();

	void SetLineSeg(CLineSeg* pLineSeg);

	void AddRangeTag(TRangeTag* pRangeTag);

	void AddCtrl(CCtrl* pCtrl);
	void AddCtrls(const LIST<CCtrl*>& arCtrls);

	void SetCtrl(CCtrl* pCtrl, unsigned int unIndex);

	VECTOR<CCtrl*> GetCtrls();

	unsigned int GetCountCtrls() const;

	static CHWPPargraph* Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int Parse(CHWPPargraph& oPara, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	CCtrl* FindFirstElement(const STRING& sID, bool bFullfilled, unsigned int& nIndex) const;
	CCtrl* FindLastElement(const STRING& sID);
};
}

#endif // HWPPARGRAPH_H
