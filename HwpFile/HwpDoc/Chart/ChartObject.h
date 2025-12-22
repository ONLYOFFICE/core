#ifndef CHARTOBJECT_H
#define CHARTOBJECT_H

#include "Types.h"
#include "../Common/Common.h"

namespace HWP { namespace CHART
{
	class IChartObject
	{
		long m_lId;
		long m_lStoredtypeId;
	protected:
		IChartObject() = default;
	public:
		virtual ETypes GetType() const = 0;
	};
}}

#endif // CHARTOBJECT_H
