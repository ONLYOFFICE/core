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

bool CHWPStream::ReadInt(int& nValue)
{
	if (m_pCur + 4 >= m_pEnd)
		return false;

	nValue = ((m_pCur[3] << 24) & 0xFF000000) | ((m_pCur[2] << 16) & 0x00FF0000) | ((m_pCur[1] << 8) & 0x0000FF00) | (m_pCur[0] & 0x000000FF);
	m_pCur += 4;

	return true;
}

bool CHWPStream::ReadColor(int& nValue)
{
	if (m_pCur + 4 >= m_pEnd)
		return false;

	nValue = ((m_pCur[3] << 24) & 0xFF000000) | ((m_pCur[0] << 16) & 0x00FF0000) | ((m_pCur[1] << 8) & 0x0000FF00) | (m_pCur[2] & 0x000000FF);
	m_pCur += 4;

	return true;
}

bool CHWPStream::ReadShort(short& shValue)
{
	if (m_pCur + 2 >= m_pEnd)
		return false;

	shValue = (short)(((m_pCur[1] << 8) & 0x0000FF00) | (m_pCur[0] & 0x000000FF));
	m_pCur += 2;

	return true;
}

bool CHWPStream::ReadByte(BYTE& chValue)
{
	if (m_pCur + 1 >= m_pEnd)
		return false;

	chValue = m_pCur[0];
	++m_pCur;

	return true;
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
