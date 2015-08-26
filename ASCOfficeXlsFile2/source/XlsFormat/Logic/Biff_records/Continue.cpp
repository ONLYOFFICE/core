
#include "Continue.h"

namespace XLS
{

Continue::Continue()
{
	m_pData		= NULL;
	m_iDataSize = 0;
}


Continue::~Continue()
{
	if (m_pData)
	{
		delete []m_pData;
	}
	m_pData		= NULL;	
	m_iDataSize = 0;
}


BaseObjectPtr Continue::clone()
{
	Continue* n = new Continue(*this);
	if (n)
	{
		n->m_iDataSize	= m_iDataSize;
		if (m_pData)
		{
			n->m_pData		= new char[m_iDataSize];	
			memcpy(n->m_pData, m_pData, m_iDataSize);
		}
	}

	return BaseObjectPtr(n);
}


void Continue::writeFields(CFRecord& record)
{
#pragma message("####################### Continue record is not implemented")
	Log::error("Continue record is not implemented.");
}


void Continue::readFields(CFRecord& record)
{
	m_iDataSize = record.getDataSize();
	m_pData		= new char[m_iDataSize];	
	memcpy(m_pData, record.getData(), m_iDataSize);

	record.skipNunBytes(m_iDataSize);
}

} // namespace XLS

