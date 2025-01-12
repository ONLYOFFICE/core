#ifndef CTRLCOMMON_H
#define CTRLCOMMON_H

#include "../HWPStream.h"
#include "CapParagraph.h"
#include "Ctrl.h"

namespace HWP
{

enum class EVRelTo
{
	PAPER,
	PAGE,
	PARA
};

enum class EHRelTo
{
	PAPER,
	PAGE,
	COLUMN,
	PARA
};

enum class EWidthRelTo
{
	PAPER,
	PAGE,
	COLUMN,
	PARA,
	ABSOLUTE
};

enum class EHeightRelTo
{
	PAPER,
	PAGE,
	ABSOLUTE
};

enum class EVertAlign
{
	TOP,
	CENTER,
	BOTTOM,
	INSIDE,
	OUTSIDE
};

inline EVertAlign GetVertAlign(int nValue)
{
	switch(static_cast<EVertAlign>(nValue))
	{
		case EVertAlign::CENTER:
		case EVertAlign::BOTTOM:
		case EVertAlign::INSIDE:
		case EVertAlign::OUTSIDE:
			return static_cast<EVertAlign>(nValue);
		case EVertAlign::TOP:
		default:
			return EVertAlign::TOP;
	}
}

enum class EHorzAlign
{
	LEFT,
	CENTER,
	RIGHT,
	INSIDE,
	OUTSIDE
};

enum class ETextWrap
{
	SQUARE,
	TOP_AND_BOTTOM,
	BEHIND_TEXT,
	IN_FRONT_OF_TEXT
};

class CCtrlCommon : public CCtrl
{
	int m_nObjAttr;
	bool m_bTreatAsChar;
	bool m_bAffectLSpacing;
	EVRelTo m_eVertRelTo;
	EVertAlign m_eVertAlign;
	EHRelTo m_eHorzRelTo;
	EHorzAlign m_eHorzAlign;
	bool m_bFlowWithText;
	bool m_bAllowOverlap;
	EWidthRelTo m_eWidthRelTo;
	EHeightRelTo m_eHeightRelTo;
	ETextWrap m_eTextWrap;
	HWP_BYTE m_chTextFlow;
	HWP_BYTE m_chNumeringType;

	int m_nVertOffset;
	int m_nHorzOffset;
	int m_nWidth;
	int m_nHeight;
	int m_nZOrder;
	short m_arOutMargin[4];
	int m_nObjInstanceID;
	int m_nBlockPageBreak;
	HWP_STRING m_sObjDesc;

	VECTOR<CHWPPargraph*> m_arParas;
	int m_nCaptionAttr;
	int m_nCaptionWidth;
	int m_nCaptionSpacing;
	int m_nCaptionMaxW;
	VECTOR<CCapParagraph*> m_arCaption;

	EVertAlign m_eTextVerAlign;

	friend class CCtrlGeneralShape;
	friend class CCtrlShapeVideo;
	friend class CCtrlEqEdit;
	friend class CCtrlTable;
public:
	CCtrlCommon();
	CCtrlCommon(const HWP_STRING& sCtrlID);
	CCtrlCommon(const CCtrlCommon& oCtrlCommon);
	CCtrlCommon(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlCommon(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion);
	virtual ~CCtrlCommon();

	ECtrlObjectType GetCtrlType() const override;

	void SetTextVerAlign(EVertAlign eVertAlign);

	void AddParagraph(CHWPPargraph* pParagraph);
	void AddCaption(CCapParagraph* pCapPara);

	CHWPPargraph* GetLastPara();
	unsigned int GetCountParagraphs() const;
	const CHWPPargraph* GetParagraphs(unsigned int unIndex) const;

	short GetLeftMargin() const;
	short GetTopMargin() const;
	short GetRightMargin() const;
	short GetBottomMargin() const;

	int GetHorzOffset() const;
	int GetVertOffset() const;
	EVRelTo GetVertRelTo() const;
	EHRelTo GetHorzRelTo() const;
	int GetCaptionWidth() const;
	bool CaptionsEmpty() const;
	HWP_STRING GetDesc() const;
	EWidthRelTo GetWidthRelTo() const;
	EHeightRelTo GetHeightRelTo() const;
	int GetWidth() const;
	int GetHeight() const;
	ETextWrap GetTextWrap() const;
	HWP_BYTE GetTextFlow() const;
	int GetZOrder() const;

	static int ParseCtrl(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCaption(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLCOMMON_H
