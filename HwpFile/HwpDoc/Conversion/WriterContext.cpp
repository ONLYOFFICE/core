#include "WriterContext.h"
#include "../HWPFile_Private.h"
#include "../HWPXFile_Private.h"

#include <sstream>
#include <iomanip>

namespace HWP
{
CWriterContext::CWriterContext()
	: m_eType(EHanType::NONE), m_pHWPFile(nullptr), m_pHWPXFile(nullptr)
{}

CWriterContext::~CWriterContext()
{
	if (nullptr != m_pHWPFile)
		delete m_pHWPFile;

	if (nullptr != m_pHWPXFile)
		delete m_pHWPXFile;
}

VECTOR<const CHWPSection*> CWriterContext::GetSections()
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
		{
			if(nullptr != m_pHWPXFile)
				return m_pHWPXFile->GetSections();

			break;
		}
	}

	return VECTOR<const CHWPSection*>();
}

EHanType CWriterContext::DetectHancom(const HWP_STRING& sPathToFile)
{
	bool bDetected = false;

	CHWPFile_Private* pHwpTemp = new CHWPFile_Private(sPathToFile);
	if (nullptr != pHwpTemp)
	{
		if (pHwpTemp->Detect())
		{
			bDetected = true;
			pHwpTemp->Close();
		}

		delete pHwpTemp;
	}

	if (bDetected)
		return EHanType::HWP;

	CHWPXFile_Private* pHwpxTemp = new CHWPXFile_Private(sPathToFile);
	if (nullptr != pHwpxTemp)
	{
		if (pHwpxTemp->Detect())
		{
			bDetected = true;
			pHwpxTemp->Close();
		}

		delete pHwpxTemp;
	}

	if (bDetected)
		return EHanType::HWPX;

	return EHanType::NONE;
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
		case EHanType::HWPX:
		{
			if (nullptr == m_pHWPXFile)
				return false;

			return m_pHWPXFile->Detect();
		}
		case EHanType::NONE:
			return false;
	}
}

bool CWriterContext::Open(const HWP_STRING& sPathToFile, EHanType eHanType)
{
	m_eType = eHanType;

	switch (m_eType)
	{
		case EHanType::HWP:
		{
			m_pHWPFile = new CHWPFile_Private(sPathToFile);

			if (nullptr == m_pHWPFile)
				return false;

			return m_pHWPFile->Open();
		}
		case EHanType::HWPX:
		{
			m_pHWPXFile = new CHWPXFile_Private(sPathToFile);

			if (nullptr == m_pHWPXFile)
				return false;

			return m_pHWPXFile->Open();
		}
		case EHanType::NONE:
			break;
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
		case EHanType::HWPX:
		{
			if (nullptr != m_pHWPXFile)
				m_pHWPXFile->Close();
			break;
		}
		case EHanType::NONE:
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
		case EHanType::HWPX:
		{
			if (nullptr == m_pHWPXFile)
				return nullptr;

			return m_pHWPXFile->GetDocInfo();
		}
		case EHanType::NONE:
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
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo)
		return HWP_STRING();

	const CHWPRecordBinData* pBinData = dynamic_cast<const CHWPRecordBinData*>(pDocInfo->GetBinData(sId));

	return (nullptr != pBinData) ? pBinData->GetPath() : HWP_STRING();
}

bool CWriterContext::GetBinBytes(const HWP_STRING& sId, CHWPStream& oBuffer, HWP_STRING& sFormat)
{
	const CHWPDocInfo* pDocInfo = nullptr;

	switch (m_eType)
	{
		case EHanType::HWP:
		{
			if (nullptr == m_pHWPFile)
				return false;

			pDocInfo = m_pHWPFile->GetDocInfo();

			if (nullptr == pDocInfo)
				return false;

			const CHWPRecordBinData* pBinData = dynamic_cast<const CHWPRecordBinData*>(pDocInfo->GetBinData(sId));

			if (nullptr == pBinData)
				return false;

			if (EType::LINK == pBinData->GetType())
			{
				NSFile::CFileBinary oFile;
				unsigned char *pBuffer = nullptr;
				unsigned long ulSize = 0;

				oFile.ReadAllBytes(pBinData->GetPath(), &pBuffer, ulSize);
				oBuffer.SetStream((HWP_BYTE*)pBuffer, ulSize, false);
				sFormat = NSFile::GetFileExtention(pBinData->GetPath());
			}
			else
			{
				std::wostringstream oStringStream;
				oStringStream << L"BIN" << std::setw(4) << std::setfill(L'0') << std::hex << pBinData->GetBinDataID() << L"." << pBinData->GetFormat();
				sFormat = pBinData->GetFormat();

				return m_pHWPFile->GetChildStream(oStringStream.str(), pBinData->GetCompressed(), oBuffer);
			}
		}
		case EHanType::HWPX:
		{
			if (nullptr == m_pHWPXFile)
				return false;

			pDocInfo = m_pHWPXFile->GetDocInfo();

			if (nullptr == pDocInfo)
				return false;

			const CHWPRecordBinData* pBinData = dynamic_cast<const CHWPRecordBinData*>(pDocInfo->GetBinData(sId));

			if (nullptr == pBinData)
				return false;

			//TODO:: реализовать
		}
		case EHanType::NONE:
			return false;
	}
}

HWP_STRING CWriterContext::GetBinFormat(const HWP_STRING& sId)
{
	//TODO::реализовать
	return HWP_STRING();
}
}
