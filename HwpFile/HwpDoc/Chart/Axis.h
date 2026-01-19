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

	bool Read(CChartStream& oStream) override;
};

class CAxisScale : public IChartObject
{
	CHART_BOOLEAN m_bHide;
	CHART_INTEGER m_nLogBase;
	CHART_STRING m_sPercentBasis;
	CHART_INTEGER m_nType;
public:
	CAxisScale();

	bool Read(CChartStream& oStream) override;
};

class CAxisTitle : public IChartObject
{
	CBackdrop m_oBackdrop;
	CHART_STRING m_sText;
	CTextLayout m_oTextLayout;
	CHART_INTEGER m_nTextLength;
	CHART_BOOLEAN m_bVisible;
	CVtFont m_oVtFont;
public:
	CAxisTitle();

	bool Read(CChartStream& oStream) override;
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
	CHART_INTEGER m_nLabelLevelCount;
	CPen m_oPen;
	CTick m_oTick;
	CValueScale m_oValueScale;
public:
	CAxis();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // AXIS_H
