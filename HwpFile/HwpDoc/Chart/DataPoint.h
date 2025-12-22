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

namespace HWP { namespace CHART
{
class CDataPointLabel
{
	CBackdrop m_oBackdrop;
	int m_nComponent;
	bool m_bCustom;
	int m_nLineStyle;
	int m_nLocationType;
	CCoor m_oOffset;
	HWP_STRING m_sPercentFormat;
	HWP_STRING m_sText;
	CTextLayout m_oTextLayout;
	int m_nTextLength;
	HWP_STRING m_sValueFormat;
	CVtFont m_oVtFont;
public:
	CDataPointLabel();
};

class CDataPoint
{
	CBrush m_oBrush;
	CDataPointLabel m_oDataPointLabel;
	CPen m_oEdgePen;
	SINGLE m_snOffset;
	CMarker m_oMarker;
	CVtPicture m_oVtPicture;
public:
	CDataPoint();
};

using CDataPoints = ICollection<CDataPoint>;
}}

#endif // DATAPOINT_H
