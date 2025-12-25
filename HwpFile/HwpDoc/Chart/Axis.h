#ifndef AXIS_H
#define AXIS_H

#include "Pen.h"
#include "VtFont.h"
#include "Backdrop.h"
#include "TextLayout.h"
#include "CategoryScale.h"
#include "DateScale.h"
#include "Intersection.h"
#include "Label.h"
#include "Tick.h"
#include "ValueScale.h"

namespace HWP { namespace CHART
{
class CAxisGrid : public IChartObject
{
	CPen m_oMajorPen;
	CPen m_oMinorPen;
public:
	CAxisGrid();

	bool Read(CHWPStream& oStream) override;
};

class CAxisScale : public IChartObject
{
	bool m_bHide;
	int m_nLogBase;
	HWP_STRING m_sPercentBasis;
	int m_nType;
public:
	CAxisScale();

	bool Read(CHWPStream& oStream) override;
};

class CAxisTitle : public IChartObject
{
	CBackdrop m_oBackdrop;
	HWP_STRING m_sText;
	CTextLayout m_oTextLayout;
	int m_nTextLength;
	bool m_bVisible;
	CVtFont m_oVtFont;
public:
	CAxisTitle();

	bool Read(CHWPStream& oStream) override;
};

class CAxis : public IChartObject
{
	CAxisGrid m_oAxisGrid;
	CAxisScale m_oAxisScale;
	CAxisTitle m_oAxisTitle;
	CCategoryScale m_oCategoryScale;
	CDateScale m_oDateScale;
	CIntersection m_oIntersection;
	CLabels m_oLabels;
	int m_nLabelLevelCount;
	CPen m_oPen;
	CTick m_oTick;
	CValueScale m_oValueScale;
public:
	CAxis();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // AXIS_H
