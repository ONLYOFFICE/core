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
	~CShadow();
};

class CNeon : public CPicEffect
{
	float m_fTransparency;
	float m_fRadius;
	CPicColor *m_pColor;
public:
	CNeon(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize);
	~CNeon();
};

class CSoftEdge : public CPicEffect
{
	float m_fRadius;
public:
	CSoftEdge(int nTypeNum, CHWPStream& oBuffer, int nOff, int nSize);
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
	BYTE m_chBright;
	BYTE m_chContrast;
	BYTE m_chEffect;
	STRING m_sBinDataID;

	BYTE m_chBorderAlpha;
	int m_nInstanceID;
	int m_nPicEffectInfo;
	std::list<CPicEffect*> m_arPicEffect;

	int m_nIniPicWidth;
	int m_nIniPicHeight;
	BYTE m_chPicAlpha;
public:
	CCtrlShapePic();
	CCtrlShapePic(const STRING& sCtrlID);
	CCtrlShapePic(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	~CCtrlShapePic();

	static int ParseElement(CCtrlShapePic& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseCtrl(CCtrlShapePic& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLSHAPEPIC_H
