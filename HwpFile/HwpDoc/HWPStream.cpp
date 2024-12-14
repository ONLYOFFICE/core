#include "HWPStream.h"

namespace HWP
{
CHWPStream::CHWPStream()
	: m_pBegin(nullptr), m_pCur(nullptr), m_pEnd(nullptr), m_bExternalBuffer(true)
{}

CHWPStream::CHWPStream(unsigned int unSize)
	: m_bExternalBuffer(false)
{
	m_pBegin = new(std::nothrow) BYTE[unSize];
	m_pCur = m_pBegin;
	m_pEnd = m_pBegin + unSize;
}

CHWPStream::CHWPStream(BYTE* pBuffer, unsigned int unSize, bool bExternalBuffer)
	: m_pBegin(pBuffer), m_pCur(pBuffer), m_pEnd(pBuffer + unSize), m_bExternalBuffer(bExternalBuffer)
{}

CHWPStream::~CHWPStream()
{
	if (nullptr != m_pBegin && !m_bExternalBuffer)
		delete[] m_pBegin;
}

void CHWPStream::SetStream(BYTE* pBuffer, unsigned int unSize, bool bExternalBuffer)
{
	m_pBegin = pBuffer;
	m_pCur   = pBuffer;
	m_pEnd   = pBuffer + unSize;

	m_bExternalBuffer = bExternalBuffer;
}

BYTE* CHWPStream::GetCurPtr()
{
	return m_pCur;
}

unsigned int CHWPStream::Tell() const
{
	return m_pCur - m_pBegin;
}

bool CHWPStream::ReadChar(CHAR& chValue)
{
	//TODO:: реализовать
	Skip(2);
	return true;
}

bool CHWPStream::ReadFloat(float& fValue)
{
	//TODO:: реализовать
	Skip(4);
	return true;
}

bool CHWPStream::ReadDouble(double& dValue)
{
	//TODO:: реализовать
	Skip(8);
	return true;
}

bool CHWPStream::ReadLong(long& lValue)
{
	//TODO:: реализовать
	Skip(8);
	return true;
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

short CHWPStream::ReadShort()
{
	if (m_pCur + 2 >= m_pEnd)
		return 0;

	short shValue = (short)(((m_pCur[1] << 8) & 0x0000FF00) | (m_pCur[0] & 0x000000FF));
	m_pCur += 2;

	return shValue;
}

bool CHWPStream::ReadByte(BYTE& chValue)
{
	if (m_pCur + 1 >= m_pEnd)
		return false;

	chValue = m_pCur[0];
	++m_pCur;

	return true;
}

BYTE CHWPStream::ReadByte()
{
	if (m_pCur + 1 >= m_pEnd)
		return 0;

	BYTE chValue = m_pCur[0]++;
	++m_pCur;

	return chValue;
}

bool CHWPStream::ReadString(STRING& sValue)
{
	sValue.clear();

	short shLen;
	if (!ReadShort(shLen))
		return false;

	if (0 == shLen)
		return true;

	int nLen = (int)shLen * 2;

	if (!CanRead(nLen))
		nLen = m_pEnd - GetCurPtr();

	sValue = STRING(GetCurPtr(), nLen); //TODO:: StandardCharsets.UTF_16LE

	Skip(nLen);

	return true;
}

bool CHWPStream::ReadString(STRING& sValue, int nLength)
{
	sValue.clear();

	if (0 == nLength)
		return true;

	if (!CanRead(nLength))
		nLength = m_pEnd - GetCurPtr();

	sValue = STRING(GetCurPtr(), nLength); //TODO:: StandardCharsets.UTF_16LE

	Skip(nLength);

	return true;
}

bool CHWPStream::ReadBytes(char* pBytes, unsigned int unSize)
{
	if (nullptr == pBytes || !CanRead(unSize))
		return false;

	memcpy(pBytes, m_pCur, unSize);
	return true;
}

void CHWPStream::Skip(unsigned int unStep)
{
	if (m_pCur + unStep >= m_pEnd)
		m_pCur = m_pEnd;
	else
		m_pCur += unStep;
}

void CHWPStream::MoveToStart()
{
	m_pCur = m_pBegin;

	while (!m_arSavedPositions.empty())
		m_arSavedPositions.pop();
}

void CHWPStream::MoveTo(unsigned int unPosition)
{
	if (unPosition < m_pEnd - m_pBegin)
		m_pCur = m_pBegin + unPosition;
	else
		m_pCur = m_pEnd;
}

bool CHWPStream::CanRead(int nSize) const
{
	if (!IsValid())
		return false;

	return m_pCur + nSize <= m_pEnd;
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

void CHWPStream::SavePosition()
{
	m_arSavedPositions.push(m_pCur);
}

void CHWPStream::RemoveLastSavedPos()
{
	if (!m_arSavedPositions.empty())
		m_arSavedPositions.pop();
}

int CHWPStream::GetDistanceToLastPos(bool bRemoveLastPos)
{
	int nDistance = (!m_arSavedPositions.empty()) ? m_pCur - m_arSavedPositions.top() : 0;

	if (bRemoveLastPos)
		RemoveLastSavedPos();

	return nDistance;
}

BYTE CHWPStream::operator[](unsigned int unPosition) const
{
	if (m_pCur >= m_pEnd)
		return *m_pEnd;

	return *(m_pCur + unPosition);
}
}
