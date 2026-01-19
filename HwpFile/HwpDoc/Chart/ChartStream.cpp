#include "ChartStream.h"

namespace HWP { namespace CHART
{
CChartStream::CChartStream()
	: m_pStream(nullptr), m_bExternStream(false)
{}

CChartStream::CChartStream(CHWPStream* pStream, bool bExtern)
	: m_pStream(pStream), m_bExternStream(bExtern)
{}

CChartStream::~CChartStream()
{
	if (nullptr != m_pStream && !m_bExternStream)
		delete m_pStream;
}

#define CHECK_VALID()\
	if (!Valid())\
		return false

bool CChartStream::ReadBoolean(CHART_BOOLEAN& bValue)
{
	CHECK_VALID();
	return m_pStream->ReadBool(bValue);
}

bool CChartStream::ReadInteger(CHART_INTEGER& nValue)
{
	CHECK_VALID();
	return m_pStream->ReadByte((HWP_BYTE&)nValue);
}

bool CChartStream::ReadLong(CHART_LONG& lValue)
{
	CHECK_VALID();
	return m_pStream->ReadInt(lValue);
}

bool CChartStream::ReadSingle(CHART_SINGLE& fValue)
{
	CHECK_VALID();
	return m_pStream->ReadFloat(fValue);
}

bool CChartStream::ReadDouble(CHART_DOUBLE& dValue)
{
	CHECK_VALID();
	return m_pStream->ReadDouble(dValue);
}

bool CChartStream::ReadString(HWP_STRING& sValue)
{
	CHECK_VALID();

	short shCount;
	if (!m_pStream->ReadShort(shCount))
		return false;

	return m_pStream->ReadString(sValue, shCount, EStringCharacter::ASCII);
}

void CChartStream::Skip(int nStep)
{
	if (nullptr != m_pStream)
		m_pStream->Skip(nStep);
}

bool CChartStream::Valid() const
{
	return nullptr != m_pStream && m_pStream->IsValid();
}
}}
