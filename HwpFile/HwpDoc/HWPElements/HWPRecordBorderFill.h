#ifndef HWPRECORDBORDERFILL_H
#define HWPRECORDBORDERFILL_H

#include "../HWPDocInfo.h"
#include "HwpRecordTypes.h"
#include "../HWPStream.h"
#include <vector>

namespace HWP
{
struct TBorder
{
	ELineStyle2 m_eStyle;
	BYTE m_chWidth;
	int m_nColor;
};

enum class EImageFillType
{
	TILE,
	TILE_HORZ_TOP,
	TILE_HORZ_BOTTOM,
	TILE_VERT_LEFT,
	TILE_VERT_RIGHT,
	TOTAL,
	CENTER,
	CENTER_TOP,
	CENTER_BOTTOM,
	LEFT_CENTER,
	LEFT_TOP,
	LEFT_BOTTOM,
	RIGHT_CENTER,
	RIGHT_TOP,
	RIGHT_BOTTOM,
	ZOOM,
	NONE
};

enum class EGradFillType
{
	LINEAR = 1,
	RADIAL = 2,
	CONICAL = 3,
	SQUARE = 4
};

enum class EColorFillPattern
{
	NONE = -1,
	VERTICAL = 0, // - - -
	HORIZONTAL = 1, // |||||
	BACK_SLASH = 2, /* \\\\\ */
	SLASH = 3, // /////
	CROSS = 4, // +++++
	CROSS_DIAGONAL = 5 // xxxxx
};

class CFill
{
	int m_nSize;

	int m_nFillType;
	CHWPStream m_oExtraFill;

	int m_nFaceColor;
	int m_nHatchColor;
	EColorFillPattern m_eHatchStyle;

	EGradFillType m_eGradType;
	int m_nAngle;
	int m_nCenterX;
	int m_nCenterY;
	int m_nStep;
	int m_nColorNum;
	std::vector<int> m_arColors;

	EImageFillType m_eMode;
	BYTE m_chBright;
	BYTE m_chContrast;
	BYTE m_chEffect;
	STRING m_sBinItemID;
	BYTE m_chStepCenter;

	BYTE m_chAlpha;
public:
	CFill();
	CFill(CHWPStream& oBuffer, int nOff, int nSize);

	int GetSize() const;
	bool IsColorFill() const;
	bool IsGradFill() const;
	bool IsImageFill() const;
};

class CHWPRecordBorderFill : public CHWPRecord
{
	CHWPDocInfo *m_pParent;
	static constexpr const float LINE_THICK[] = { 0.1f, 0.12f, 0.15f, 0.2f, 0.25f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 1.0f, 1.5f, 2.0f, 3.0f, 4.0f, 5.0f };

	bool m_bThreeD;
	bool m_bShadow;
	BYTE m_chSlash;
	BYTE m_chBackSlash;
	BYTE m_chCrookedSlash;
	BYTE m_chCrookedBackSlash;
	bool m_bCounterSlash;
	bool m_bCounterBackSlash;
	bool m_bBreakCellSeparateLine;

	TBorder m_oLeft;
	TBorder m_oRight;
	TBorder m_oTop;
	TBorder m_oBottom;
	TBorder m_oDiagonal;
	CFill   m_oFill;

public:
	CHWPRecordBorderFill(int nTagNum, int nLevel, int nSize);
	CHWPRecordBorderFill(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDBORDERFILL_H
