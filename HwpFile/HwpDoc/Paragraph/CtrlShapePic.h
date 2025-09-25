#ifndef CTRLSHAPEPIC_H
#define CTRLSHAPEPIC_H

#include "CtrlGeneralShape.h"
#include "Point.h"

namespace HWP
{
enum class EImagePathType
{
	COMPOUND,
	OWPML,
	LINK,
	UNKNOWN
};

enum class EPicEffectType
{
	NONE,
	SHADOW,
	GLOW,
	SOFT_EDGE,
	REFLECT
};

class CPicColor
{
	int m_nSize;

	int m_nType;
	int m_nRGB;
public:
	CPicColor(CHWPStream& oBuffer, int nOff, int nSize);
	CPicColor(CXMLReader& oReader);
};

class CPicEffect
{
	EPicEffectType m_eType;
protected:
	int m_nSize;
public:
	CPicEffect(EPicEffectType eType);
	CPicEffect(int nType);

	int GetSize();
};

class CShadow : public CPicEffect
{
	int m_nStyle;
	int m_nTransparency;
	int m_nBlur;
	int m_nDirection;
	int m_nDistance;
	float m_fAngleX;
	float m_fAngleY;
	float m_fMagnifyX;
	float m_fMagnifyY;
	int m_nRotation;
	CPicColor *m_pColor;
public:
	CShadow(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize);
	CShadow(CXMLReader& oReader, int nVersion);
	~CShadow();
};

class CNeon : public CPicEffect
{
	float m_fTransparency;
	float m_fRadius;
	CPicColor *m_pColor;
public:
	CNeon(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize);
	CNeon(CXMLReader& oReader, int nVersion);
	~CNeon();
};

class CSoftEdge : public CPicEffect
{
	float m_fRadius;
public:
	CSoftEdge(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize);
	CSoftEdge(CXMLReader& oReader, int nVersion);
};

class CReflect : public CPicEffect
{
	int m_nStyle;
	float m_fRadius;
	float m_fDirection;
	float m_fDistance;
	float m_fAngleX;
	float m_fAngleY;
	float m_fMagnifyX;
	float m_fMagnifyY;
	int m_nRotateStyle;
	float m_fStartTrans;
	float m_fStartPos;
	float m_fEndTrans;
	float m_fEndPos;
	float m_fOffsetDirection;
public:
	CReflect(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize);
	CReflect(CXMLReader& oReader, int nVersion);
};

class CCtrlShapePic : public CCtrlGeneralShape
{
	int m_nBorderColor;
	int m_nBorderThick;
	int m_nBorderAttr;
	TPoint m_arBorderPoints[4];
	int m_nCropLeft;
	int m_nCropTop;
	int m_nCropRight;
	int m_nCropBottom;
	short m_arInnerSpaces[4];
	HWP_BYTE m_chBright;
	HWP_BYTE m_chContrast;
	HWP_BYTE m_chEffect;
	HWP_STRING m_sBinDataID;

	HWP_BYTE m_chBorderAlpha;
	int m_nInstanceID;
	int m_nPicEffectInfo;
	LIST<CPicEffect*> m_arPicEffect;

	int m_nIniPicWidth;
	int m_nIniPicHeight;
	HWP_BYTE m_chPicAlpha;
public:
	CCtrlShapePic();
	CCtrlShapePic(const HWP_STRING& sCtrlID);
	CCtrlShapePic(const CCtrlGeneralShape& oShape);
	CCtrlShapePic(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlShapePic(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion);
	~CCtrlShapePic();

	EShapeType GetShapeType() const override;

	HWP_STRING GetBinDataID() const;
	int GetPicWidth() const;
	int GetPicHeight() const;

	ELineStyle2 GetBorderLineStyle() const;
	HWP_BYTE GetBorderCompoundLineType() const;
	int GetBorderColor() const;
	int GetBorderThick() const;

	int GetImageRectWidth() const;
	int GetIMageRectHeight() const;

	static int ParseElement(CCtrlShapePic& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapePic& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEPIC_H
