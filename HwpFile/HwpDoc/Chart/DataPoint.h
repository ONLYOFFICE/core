#ifndef DATAPOINT_H
#define DATAPOINT_H

#include "Backdrop.h"
#include "Coor.h"
#include "TextLayout.h"
#include "VtFont.h"
#include "Pen.h"
#include "Marker.h"
#include "Brush.h"
#include "VtPicture.h"
#include "Collection.h"

namespace HWP { namespace CHART
{
class CDataPointLabel : public IChartObject
{
	CBackdrop m_oBackdrop;
	CHART_INTEGER m_nComponent;
	CHART_BOOLEAN m_bCustom;
	CHART_INTEGER m_nLineStyle;
	CHART_INTEGER m_nLocationType;
	CCoor m_oOffset;
	CHART_STRING m_sPercentFormat;
	CHART_STRING m_sText;
	CTextLayout m_oTextLayout;
	CHART_INTEGER m_nTextLength;
	CHART_STRING m_sValueFormat;
	CVtFont m_oVtFont;
public:
	CDataPointLabel();

	bool Read(CChartStream& oStream) override;
};

class CDataPoint : public IChartObject
{
	CBrush m_oBrush;
	CDataPointLabel m_oDataPointLabel;
	CPen m_oEdgePen;
	CHART_SINGLE m_snOffset;
	CMarker m_oMarker;
	CVtPicture m_oVtPicture;
public:
	CDataPoint();

	bool Read(CChartStream& oStream) override;
};

using CDataPoints = CCollection<CDataPoint>;
}}

#endif // DATAPOINT_H
