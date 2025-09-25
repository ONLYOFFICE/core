#ifndef HWPRECORDBORDERFILL_H
#define HWPRECORDBORDERFILL_H

#include "../HWPDocInfo.h"
#include "HwpRecordTypes.h"
#include "../HWPStream.h"
#include "../Common/XMLReader.h"
#include <vector>

namespace HWP
{
struct TBorder
{
	ELineStyle2 m_eStyle;
	HWP_BYTE m_chWidth;
	int m_nColor;

	void Read(CXMLReader& oReader, EHanType eType);
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

class CFill : public IRef
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
	HWP_BYTE m_chBright;
	HWP_BYTE m_chContrast;
	HWP_BYTE m_chEffect;
	HWP_STRING m_sBinItemID;
	HWP_BYTE m_chStepCenter;

	HWP_BYTE m_chAlpha;

	void ReadWinBrush(CXMLReader& oReader, EHanType eType);
	void ReadGradation(CXMLReader& oReader, EHanType eType);
	void ReadImgBrush(CXMLReader& oReader, EHanType eType);
public:
	CFill();
	CFill(CHWPStream& oBuffer, int nOff, int nSize);
	CFill(CXMLReader& oReader, EHanType eType);

	int GetSize() const;
	bool NoneFill() const;
	bool ColorFill() const;
	bool GradFill() const;
	bool ImageFill() const;

	int GetFaceColor() const;

	EGradFillType GetGradFillType() const;
	int GetGradAngle() const;
	int GetGradCenterX() const;
	int GetGradCenterY() const;
	int GetGradStep() const;
	int GetGradColorNum() const;
	std::vector<int> GetGradColors() const;

	HWP_STRING GetBinItemID() const;
};

class CHWPRecordBorderFill : public CHWPRecord
{
	static constexpr const float LINE_THICK[] = { 0.1f, 0.12f, 0.15f, 0.2f, 0.25f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 1.0f, 1.5f, 2.0f, 3.0f, 4.0f, 5.0f };

	bool m_bThreeD;
	bool m_bShadow;
	HWP_BYTE m_chSlash;
	HWP_BYTE m_chBackSlash;
	HWP_BYTE m_chCrookedSlash;
	HWP_BYTE m_chCrookedBackSlash;
	bool m_bCounterSlash;
	bool m_bCounterBackSlash;
	bool m_bBreakCellSeparateLine;

	TBorder m_oLeft;
	TBorder m_oRight;
	TBorder m_oTop;
	TBorder m_oBottom;
	TBorder m_oDiagonal;
	CFill   *m_pFill;
public:
	CHWPRecordBorderFill(int nTagNum, int nLevel, int nSize);
	CHWPRecordBorderFill(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordBorderFill(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType);
	~CHWPRecordBorderFill();

	TBorder GetLeftBorder() const;
	TBorder GetRightBorder() const;
	TBorder GetTopBorder() const;
	TBorder GetBottomBorder() const;

	const CFill* GetFill() const;
};
}

#endif // HWPRECORDBORDERFILL_H
