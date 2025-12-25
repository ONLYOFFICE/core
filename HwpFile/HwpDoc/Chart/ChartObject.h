#ifndef CHARTOBJECT_H
#define CHARTOBJECT_H

#include "Types.h"
#include "../Common/Common.h"
#include "../HWPStream.h"

namespace HWP { namespace CHART
{
	class IChartObject
	{
		long m_lId;
		long m_lStoredtypeId;
	protected:
		IChartObject() = default;
	public:
		virtual bool Read(CHWPStream& oStream) = 0;
		// virtual ETypes GetType() const = 0;
	};
}}

#endif // CHARTOBJECT_H
