#include "HWPStream.h"

namespace HWP
{
CHWPStream::CHWPStream()
	: m_pBegin(nullptr), m_pCur(nullptr), m_pEnd(nullptr)
{

}

CHWPStream::CHWPStream(BYTE* pBuffer, unsigned int unSize)
	: m_pBegin(pBuffer), m_pCur(pBuffer), m_pEnd(pBuffer + unSize)
{}

void CHWPStream::SetStream(BYTE* pBuffer, unsigned int unSize)
{
	m_pBegin = pBuffer;
	m_pCur   = pBuffer;
	m_pEnd   = pBuffer + unSize;
}

BYTE* CHWPStream::GetCurPtr()
{
	return m_pCur;
}

void CHWPStream::Skip(unsigned int unStep)
{
	if (m_pCur + unStep >= m_pEnd)
		m_pCur = m_pEnd;
	else
		m_pCur += unStep;
}

bool CHWPStream::IsValid() const
{
	return nullptr != m_pBegin;
}

bool CHWPStream::IsEof() const
{
	return m_pCur >= m_pEnd;
}

unsigned int CHWPStream::GetLength() const
{
	return (unsigned int)(m_pEnd - m_pCur);
}

BYTE CHWPStream::operator[](unsigned int unPosition) const
{
	if (m_pCur >= m_pEnd)
		return *m_pEnd;

	return *(m_pCur + unPosition);
}
}
