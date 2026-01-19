#include "Fill.h"

namespace HWP { namespace CHART
{
CFill::CFill()
{

}

bool CFill::Read(CChartStream& oStream)
{
	return m_oBrush.Read(oStream) && /*&&*/
	       oStream.ReadInteger(m_nStyle) && m_oVtPicture.Read(oStream);
}
}}
