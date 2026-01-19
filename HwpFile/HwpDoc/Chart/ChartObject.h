#ifndef CHARTOBJECT_H
#define CHARTOBJECT_H

#include "Types.h"
#include "ChartStream.h"

namespace HWP { namespace CHART
{
	class IChartObject
	{
		CHART_LONG m_lId;
		CHART_LONG m_lStoredtypeId;
	protected:
		IChartObject() = default;
	public:
		virtual bool Read(CChartStream& oStream) = 0;
		// virtual ETypes GetType() const = 0;
	};
}}

#endif // CHARTOBJECT_H
