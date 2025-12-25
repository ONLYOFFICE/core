#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "Brush.h"
#include "Pen.h"
#include "Collection.h"

namespace HWP { namespace CHART
{
class CAttribute : public IChartObject
{
	using _type = HWP::CHART::CAttribute;

	CBrush m_oBrush;
	CPen m_oPen;
	HWP_STRING m_sText;
	double m_dValue;
public:
	CAttribute();

	bool Read(CHWPStream& oStream) override;
};

using CAttributes = CCollection<CAttribute>;
}}

#endif // ATTRIBUTE_H
