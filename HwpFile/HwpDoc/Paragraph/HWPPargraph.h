#ifndef HWPPARGRAPH_H
#define HWPPARGRAPH_H

#include "LineSeg.h"
#include "RangeTag.h"
#include "Ctrl.h"
#include "../Common/XMLNode.h"

namespace HWP
{
enum class EParagraphType
{
	Normal,
	Cap,
	Cell
};

class CHWPPargraph : public IRef
{
	short m_shParaShapeID; // HWPTAG_PARA_HEADER
	short m_shParaStyleID; // HWPTAG_PARA_HEADER
	HWP_BYTE m_chBreakType;    // HWPTAG_PARA_HEADER

	CLineSeg *m_pLineSegs; // HWPTAG_PARA_LINE_SEG
	VECTOR<TRangeTag*> m_arRangeTags;  // HWPTAG_PARA_RANGE_TAG

	VECTOR<CCtrl*> m_arP; //HWPTAG_PARA_TEXT

	void ParseHWPParagraph(CXMLNode& oNode, int nCharShapeID, int nVersion);
public:
	CHWPPargraph();
	CHWPPargraph(CXMLNode& oNode, int nVersion);
	virtual ~CHWPPargraph();

	virtual EParagraphType GetType() const;

	void SetLineSeg(CLineSeg* pLineSeg);

	void AddRangeTag(TRangeTag* pRangeTag);

	void AddCtrl(CCtrl* pCtrl);
	void AddCtrls(const LIST<CCtrl*>& arCtrls);

	bool SetCtrl(CCtrl* pCtrl, unsigned int unIndex);

	VECTOR<CCtrl*>& GetCtrls();
	VECTOR<const CCtrl*> GetCtrls() const;

	unsigned int GetCountCtrls() const;
	short GetShapeID() const;
	short GetStyleID() const;
	HWP_BYTE GetBreakType() const;

	const CLineSeg* GetLineSeg() const;

	static CHWPPargraph* Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int Parse(CHWPPargraph& oPara, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	CCtrl* FindFirstElement(const HWP_STRING& sID, bool bFullfilled, unsigned int& nIndex) const;
	CCtrl* FindLastElement(const HWP_STRING& sID);

	int IndexOf(CCtrl* pCtrl);
};
}

#endif // HWPPARGRAPH_H
