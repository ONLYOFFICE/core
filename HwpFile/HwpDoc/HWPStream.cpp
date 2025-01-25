#include "HWPStream.h"

#include "../../DesktopEditor/common/StringExt.h"

namespace HWP
{
CHWPStream::CHWPStream()
	: m_pBegin(nullptr), m_pCur(nullptr), m_pEnd(nullptr), m_bExternalBuffer(false)
{}

CHWPStream::CHWPStream(unsigned long ulSize)
	: m_pBegin(nullptr), m_pCur(nullptr), m_pEnd(nullptr), m_bExternalBuffer(false)
{
	Expand(ulSize);
}

CHWPStream::CHWPStream(HWP_BYTE* pBuffer, unsigned long ulSize, bool bExternalBuffer)
	: m_pBegin(pBuffer), m_pCur(pBuffer), m_pEnd(pBuffer + ulSize), m_bExternalBuffer(bExternalBuffer)
{}

CHWPStream::~CHWPStream()
{
	Clear();
}

void CHWPStream::Clear()
{
	if (nullptr != m_pBegin && !m_bExternalBuffer)
		free(m_pBegin);

	m_pBegin = nullptr;
	m_pCur = nullptr;
	m_pEnd = nullptr;

	while (!m_arSavedPositions.empty())
		m_arSavedPositions.pop();
}

void CHWPStream::SetStream(HWP_BYTE* pBuffer, unsigned long ulSize, bool bExternalBuffer)
{
	m_pBegin = pBuffer;
	m_pCur   = pBuffer;
	m_pEnd   = pBuffer + ulSize;

	m_bExternalBuffer = bExternalBuffer;
}

HWP_BYTE* CHWPStream::GetCurPtr()
{
	return m_pCur;
}

unsigned long CHWPStream::Tell() const
{
	return (!IsValid()) ? 0 : m_pCur - m_pBegin;
}

unsigned long CHWPStream::SizeToEnd() const
{
	return (!IsValid() || IsEof()) ? 0 : m_pEnd - m_pCur;
}

void CHWPStream::Expand(unsigned long ulSize)
{
	if (nullptr != m_pBegin)
	{
		unsigned long ulCurrentPos = Tell();
		unsigned long ulNewSize = GetSize() + ulSize;

		m_pBegin = (HWP_BYTE*)realloc(m_pBegin, ulNewSize);
		m_pEnd = m_pBegin + ulNewSize;
		m_pCur = m_pBegin + ulCurrentPos;
	}
	else
	{
		m_pBegin = (HWP_BYTE*)malloc(ulSize);
		m_pBegin[0] = 0x11;
		m_pEnd = m_pBegin + ulSize;
		m_pCur = m_pBegin;
	}
}

bool CHWPStream::ReadChar(HWP_CHAR& chValue)
{
	if (m_pCur + 2 > m_pEnd)
		return false;

	chValue = (HWP_CHAR)(((m_pCur[1] << 8) & 0x0000FF00) | (m_pCur[0] & 0x000000FF));
	m_pCur += 2;

	return true;
}

bool CHWPStream::ReadFloat(float& fValue)
{
	//TODO:: Проверить
	int nValue;
	if (!ReadInt(nValue))
		return false;

	fValue = *(float*)&nValue;

	return true;
}

bool CHWPStream::ReadDouble(double& dValue)
{
	//TODO:: реализовать
	long long lValue;
	if (!ReadLong(lValue))
		return false;

	dValue = *(double*)&lValue;

	return true;
}

bool CHWPStream::ReadLong(long long& lValue)
{
	if (m_pCur + 8 > m_pEnd)
		return false;

	lValue = ((m_pCur[7] << 24) & 0xFF000000) | ((m_pCur[6 ]<< 16) & 0xFF0000) | ((m_pCur[5] << 8) & 0xFF00) | (m_pCur[4] & 0xFF);
	lValue = (lValue << 32) | ((m_pCur[3] << 24) & 0xFF000000) | ((m_pCur[2] << 16) & 0xFF0000) | ((m_pCur[1] << 8) & 0xFF00) | (m_pCur[0] & 0xFF);

	m_pCur += 8;

	return true;
}

bool CHWPStream::ReadInt(int& nValue)
{
	if (m_pCur + 4 > m_pEnd)
		return false;

	nValue = ((m_pCur[3] << 24) & 0xFF000000) | ((m_pCur[2] << 16) & 0x00FF0000) | ((m_pCur[1] << 8) & 0x0000FF00) | (m_pCur[0] & 0x000000FF);
	m_pCur += 4;

	return true;
}

bool CHWPStream::ReadColor(int& nValue)
{
	if (m_pCur + 4 > m_pEnd)
		return false;

	nValue = ((m_pCur[3] << 24) & 0xFF000000) | ((m_pCur[0] << 16) & 0x00FF0000) | ((m_pCur[1] << 8) & 0x0000FF00) | (m_pCur[2] & 0x000000FF);
	m_pCur += 4;

	return true;
}

bool CHWPStream::ReadShort(short& shValue)
{
	if (m_pCur + 2 > m_pEnd)
		return false;

	shValue = (short)(((m_pCur[1] << 8) & 0x0000FF00) | (m_pCur[0] & 0x000000FF));
	m_pCur += 2;

	return true;
}

short CHWPStream::ReadShort()
{
	if (m_pCur + 1 > m_pEnd)
		return 0;

	short shValue = (short)(((m_pCur[1] << 8) & 0x0000FF00) | (m_pCur[0] & 0x000000FF));
	m_pCur += 2;

	return shValue;
}

bool CHWPStream::ReadByte(HWP_BYTE& chValue)
{
	if (m_pCur + 1 > m_pEnd)
		return false;

	chValue = m_pCur[0];
	++m_pCur;

	return true;
}

HWP_BYTE CHWPStream::ReadByte()
{
	if (m_pCur + 1 > m_pEnd)
		return 0;

	HWP_BYTE chValue = m_pCur[0]++;
	++m_pCur;

	return chValue;
}

void Trim(HWP_STRING& sValue)
{
	HWP_STRING::const_iterator itBegin = std::find_if(sValue.cbegin(), sValue.cend(), [](wchar_t wChar){ return !iswcntrl(wChar); });

	if (itBegin != sValue.cend())
		sValue.erase(0, itBegin - sValue.cbegin());

	HWP_STRING::const_reverse_iterator itEnd = std::find_if(sValue.crbegin(), sValue.crend(), [](wchar_t wChar){ return !iswcntrl(wChar); });

	if (itEnd != sValue.crbegin())
		sValue.erase(itEnd.base());
}

bool CHWPStream::ReadString(HWP_STRING& sValue, EStringCharacter eCharacter)
{
	short shLen;
	if (!ReadShort(shLen))
		return false;

	return ReadString(sValue, (int)shLen * 2, eCharacter);
}

bool CHWPStream::ReadString(HWP_STRING& sValue, int nLength, EStringCharacter eCharacter)
{
	sValue.clear();

	if (0 == nLength)
		return true;

	if (!CanRead(nLength))
		nLength = m_pEnd - GetCurPtr();

	switch (eCharacter)
	{
		case EStringCharacter::ASCII:
		{
			sValue = NSStringExt::CConverter::GetUnicodeFromSingleByteString((unsigned char*)GetCurPtr(), nLength);
			break;
		}
		case EStringCharacter::UTF16:
		{
			sValue = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)GetCurPtr(), nLength / 2);
			break;
		}
		case EStringCharacter::UTF32:
		{
			sValue = NSStringExt::CConverter::GetUnicodeFromUTF32((unsigned int*)GetCurPtr(), nLength / 4);
			break;
		}
	}

	Trim(sValue);
	Skip(nLength);

	return true;
}

unsigned long CHWPStream::ReadBytes(HWP_BYTE* pBytes, unsigned long unSize)
{
	if (!IsValid() || IsEof())
		return 0;

	unsigned long ulNewSize = (std::min)(unSize, SizeToEnd());

	memcpy(pBytes, m_pCur, ulNewSize);
	m_pCur += ulNewSize;
	return ulNewSize;
}

void CHWPStream::Skip(int nStep)
{
	if (nStep > 0)
	{
		if (m_pCur + nStep > m_pEnd)
			m_pCur = m_pEnd;
		else
			m_pCur += nStep;
	}
	else if (nStep < 0)
	{
		if (m_pCur + nStep > m_pBegin)
			m_pCur += nStep;
		else
			m_pCur = m_pBegin;
	}
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
	return m_pCur > m_pEnd;
}

unsigned int CHWPStream::GetSize() const
{
	return (unsigned int)(m_pEnd - m_pBegin);
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

HWP_BYTE CHWPStream::operator[](unsigned int unPosition) const
{
	if (m_pCur > m_pEnd)
		return *(m_pEnd - 1);

	return *(m_pCur + unPosition);
}

bool CHWPStream::WriteBytes(const HWP_BYTE* pBuffer, unsigned long ulSize)
{
	unsigned long ulSizeToEnd = SizeToEnd();

	if (ulSize > ulSizeToEnd)
		Expand(ulSize - ulSizeToEnd);

	memcpy(m_pCur, pBuffer, ulSize);
	m_pCur += ulSize;
	return true;
}
}
