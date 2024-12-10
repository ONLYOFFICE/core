#ifndef CTRLCOMMON_H
#define CTRLCOMMON_H

#include "../HWPStream.h"
#include "CapParagraph.h"
#include "Ctrl.h"
#include <vector>
#include <list>

namespace HWP
{

enum class EVRelTo
{
	PAPER,
	PAGE,
	PARA,
	null
};

enum class EHRelTo
{
	PAPER,
	PAGE,
	COLUMN,
	PARA,
	null
};

enum class EWidthRelTo
{
	PAPER,
	PAGE,
	COLUMN,
	PARA,
	ABSOLUTE,
	null
};

enum class EHeightRelTo
{
	PAPER,
	PAGE,
	ABSOLUTE,
	null
};

enum class EVertAlign
{
	TOP,
	CENTER,
	BOTTOM,
	INSIDE,
	OUTSIDE
};

EVertAlign GetVertAlign(int nValue)
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
	int m_nSize;

	int m_nObjAttr;
	bool m_bTreatAsChar;
	bool m_bAffectLSpacing;
	EVRelTo m_eVertRelTo;
	EVertAlign m_eVertAlign;
	EHRelTo m_eHorzRelTo;
	EHorzAlign m_eHorzAlign;
	bool m_bFlowWithText;
	bool m_bAllowOverlap;
	EWidthRelTo m_eWidthRelto;
	EHeightRelTo m_eHeightRelto;
	ETextWrap m_eTextWrap;
	BYTE m_chTextFlow;
	BYTE m_chNumeringType;

	int m_nVertOffset;
	int m_nHorzOffset;
	int m_nWidth;
	int m_nHeight;
	int m_nZOrder;
	short m_arOutMargin[4];
	int m_nObjInstanceID;
	int m_nBlockPageBreak;
	STRING m_sObjDesc;

	// std::list<CHWPPargraph*> m_arParas;
	int m_nCaptionAttr;
	int m_nCaptionWidth;
	int m_nCaptionSpacing;
	int m_nCaptionMaxW;
	std::list<CCapParagraph*> m_arCaption;

	EVertAlign m_eTextVerAlign;

	friend class CCtrlGeneralShape;
	friend class CCtrlShapeVideo;
	friend class CCtrlEqEdit;
	friend class CCtrlTalbe;
public:
	CCtrlCommon();
	CCtrlCommon(const STRING& sCtrlID);
	CCtrlCommon(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;

	static int ParseCtrl(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCaption(CCtrlCommon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLCOMMON_H
