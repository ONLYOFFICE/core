#include "Shadow.h"

namespace HWP { namespace CHART
{
CShadow::CShadow()
{

}

bool CShadow::Read(CHWPStream& oStream)
{
	return m_oBrush.Read(oStream) && m_oOffset.Read(oStream) && oStream.ReadInt(m_nStyle);
}
}}
