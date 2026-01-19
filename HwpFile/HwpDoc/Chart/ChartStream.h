#ifndef CHARTSTREAM_H
#define CHARTSTREAM_H

#include "../HWPStream.h"
#include "CharCommon.h"

namespace HWP { namespace CHART
{
class CChartStream
{
	CHWPStream *m_pStream;
	bool m_bExternStream;
public:
	CChartStream();
	CChartStream(CHWPStream* pStream, bool bExtern = true);
	~CChartStream();

	bool ReadBoolean(CHART_BOOLEAN& bValue);
	bool ReadInteger(CHART_INTEGER& nValue);
	bool ReadLong(CHART_LONG& lValue);
	bool ReadSingle(CHART_SINGLE& fValue);
	bool ReadDouble(CHART_DOUBLE& dValue);
	bool ReadString(HWP_STRING& sValue);

	void Skip(int nStep);
private:
	bool Valid() const;
};
}}

#endif // CHARTSTREAM_H
