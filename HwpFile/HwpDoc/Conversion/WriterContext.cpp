#include "WriterContext.h"
#include "../HWPFile_Private.h"

namespace HWP
{
CWriterContext::CWriterContext()
	: m_eType(EHanType::NONE), m_pHWPFile(nullptr)
{}

CWriterContext::~CWriterContext()
{
	if (nullptr != m_pHWPFile)
		delete m_pHWPFile;
}

VECTOR<CHWPSection*> CWriterContext::GetSections()
{
	switch(m_eType)
	{
		case EHanType::NONE: break;
		case EHanType::HWP:
		{
			if (nullptr != m_pHWPFile)
				return m_pHWPFile->GetSections();

			break;
		}
		case EHanType::HWPX:
			break;
	}

	return VECTOR<CHWPSection*>();
}

HWP_STRING CWriterContext::DetectHancom(const HWP_STRING& sPathToFile)
{
	HWP_STRING sDetectingType;

	CHWPFile_Private* pHwpTemp = new CHWPFile_Private(sPathToFile);
	if (nullptr != pHwpTemp)
	{
		if (pHwpTemp->Detect())
		{
			sDetectingType = L"HWP";
			pHwpTemp->Close();
		}

		delete pHwpTemp;
	}

	if (!sDetectingType.empty())
		return sDetectingType;

	//TODO:: добавить HWPX

	return sDetectingType;
}

bool CWriterContext::Detect()
{
	switch(m_eType)
	{
		case EHanType::HWP:
		{
			if (nullptr == m_pHWPFile)
				return false;

			return m_pHWPFile->Detect();
		}
		case EHanType::NONE:
		case EHanType::HWPX:
			return false;
	}
}

bool CWriterContext::Open(const HWP_STRING& sPathToFile, const HWP_STRING& sHanType)
{
	if (L"HWP" == sHanType)
	{
		m_eType = EHanType::HWP;
		m_pHWPFile = new CHWPFile_Private(sPathToFile);

		if (nullptr == m_pHWPFile)
			return false;

		return m_pHWPFile->Open();
	}

	return false;
}

void CWriterContext::Close()
{
	switch (m_eType)
	{
		case EHanType::HWP:
		{
			if (nullptr != m_pHWPFile)
				m_pHWPFile->Close();
			break;
		}
		case EHanType::NONE:
		case EHanType::HWPX:
			break;
	}
}

const CHWPDocInfo* CWriterContext::GetDocInfo()
{
	switch (m_eType)
	{
		case EHanType::HWP:
		{
			if (nullptr == m_pHWPFile)
				return nullptr;

			return m_pHWPFile->GetDocInfo();
		}
		case EHanType::NONE:
		case EHanType::HWPX:
			return nullptr;
	}
}

const CHWPRecordBorderFill* CWriterContext::GetBorderFill(short shId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo || 0 >= shId)
		return nullptr;

	return (CHWPRecordBorderFill*)pDocInfo->GetBorderFill(shId - 1);
}

const CHWPRecordParaShape* CWriterContext::GetParaShape(int nId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo || 0 >= nId)
		return nullptr;

	return (CHWPRecordParaShape*)pDocInfo->GetParaShape(nId);
}

const CHWPRecordStyle* CWriterContext::GetParaStyle(short shId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo || 0 >= shId)
		return nullptr;

	return (CHWPRecordStyle*)pDocInfo->GetStyle(shId);
}

const CHWPRecordCharShape* CWriterContext::GetCharShape(int nId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo || 0 >= nId)
		return nullptr;

	return (CHWPRecordCharShape*)pDocInfo->GetCharShape(nId);
}

const CHWPRecordNumbering* CWriterContext::GetNumbering(short shId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo || 0 >= shId)
		return nullptr;

	return (CHWPRecordNumbering*)pDocInfo->GetNumbering(shId);
}

const CHWPRecordBullet* CWriterContext::GetBullet(short shId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo || 0 >= shId)
		return nullptr;

	return (CHWPRecordBullet*)pDocInfo->GetBullet(shId - 1);
}

const CHwpRecordTabDef* CWriterContext::GetTabDef(short shId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo || 0 >= shId)
		return nullptr;

	return (CHwpRecordTabDef*)pDocInfo->GetTabDef(shId);
}

HWP_STRING CWriterContext::GetBinFilename(const HWP_STRING& sId)
{
	//TODO::реализовать
	return HWP_STRING();
}

bool CWriterContext::GetBinBytes(const HWP_STRING& sId, CHWPStream& oBuffer)
{
	//TODO::реализовать
	return false;
}

HWP_STRING CWriterContext::GetBinFormat(const HWP_STRING& sId)
{
	//TODO::реализовать
	return HWP_STRING();
}
}
