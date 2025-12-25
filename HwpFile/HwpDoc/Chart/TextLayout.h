#ifndef TEXTLAYOUT_H
#define TEXTLAYOUT_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CTextLayout : public IChartObject
{
	bool m_bWordWrap;
	int m_nHorzAlignment;
	int m_nOrientation;
	int m_nVertAlignment;
public:
	CTextLayout();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // TEXTLAYOUT_H
