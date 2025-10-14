#include "WriterContext.h"
#include "../HWPFile.h"
#include "../HWPXFile.h"

#include <sstream>
#include <iomanip>

namespace HWP
{
CWriterContext::CWriterContext()
	: m_eType(EHanType::NONE), m_pHWPFile(nullptr), m_pHWPXFile(nullptr)
{}

CWriterContext::~CWriterContext()
{
	Clear();
}

void CWriterContext::Clear()
{
	switch (m_eType)
	{
		case EHanType::HWP:
		{
			if (nullptr == m_pHWPFile)
				return;
			
			delete m_pHWPFile;
			m_pHWPFile = nullptr;
			break;
		}
		case EHanType::HWPX:
		{
			if (nullptr == m_pHWPXFile)
				return;
			
			delete m_pHWPXFile;
			m_pHWPXFile = nullptr;
			break;
		}
		case EHanType::HWPML:
		{
			if(nullptr == m_pHWPMLFile)
				return;
			
			delete m_pHWPMLFile;
			m_pHWPMLFile = nullptr;
			break;
		}
		default:
			break;
	}
	
	m_eType = EHanType::NONE;
}

EHanType CWriterContext::GetType()
{
	return m_eType;
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
		case EHanType::HWPML:
		{
			if (nullptr != m_pHWPMLFile)
				return m_pHWPMLFile->GetSections();
			
			break;
		}
	}

	return VECTOR<const CHWPSection*>();
}

EHanType CWriterContext::DetectHancom(const HWP_STRING& sPathToFile)
{
	bool bDetected = false;

	#define DETECT(hwp_class)\
	{\
		hwp_class* pTemp = new hwp_class(sPathToFile);\
		if (nullptr != pTemp)\
		{\
			if (pTemp->Detect())\
			{\
				bDetected = true;\
				pTemp->Close();\
			}\
			delete pTemp;\
		}\
	}

	DETECT(CHWPFile)

	if (bDetected)
		return EHanType::HWP;

	DETECT(CHWPXFile)

	if (bDetected)
		return EHanType::HWPX;

	DETECT(CHWPMLFile)

	if(bDetected)
		return EHanType::HWPML;

	return EHanType::NONE;
}

bool CWriterContext::Detect()
{
	switch(m_eType)
	{
		case EHanType::HWP:
			return (nullptr != m_pHWPFile) ? m_pHWPFile->Detect() : false;
		case EHanType::HWPX:
			return (nullptr != m_pHWPXFile) ? m_pHWPXFile->Detect() : false;
		case EHanType::HWPML:
			return (nullptr != m_pHWPMLFile) ? m_pHWPMLFile->Detect() : false;
		case EHanType::NONE:
			return false;
	}
}

bool CWriterContext::Open(const HWP_STRING& sPathToFile, EHanType eHanType)
{
	Clear();

	m_eType = eHanType;

	#define OPEN(variable_name, class_name)\
	variable_name = new class_name(sPathToFile);\
	if (nullptr == variable_name)\
		return false;\
	return variable_name->Open()
	
	switch (m_eType)
	{
		case EHanType::HWP:
		{
			OPEN(m_pHWPFile, CHWPFile);
		}
		case EHanType::HWPX:
		{
			OPEN(m_pHWPXFile, CHWPXFile);
		}
		case EHanType::HWPML:
		{
			OPEN(m_pHWPMLFile, CHWPMLFile);
		}
		case EHanType::NONE:
			break;
	}

	return false;
}

void CWriterContext::Close()
{
	#define CLOSE(variable_name)\
	if (nullptr != variable_name)\
		variable_name->Close();\
	break
	
	switch (m_eType)
	{
		case EHanType::HWP:
		{
			CLOSE(m_pHWPFile);
		}
		case EHanType::HWPX:
		{
			CLOSE(m_pHWPXFile);
		}
		case EHanType::HWPML:
		{
			CLOSE(m_pHWPMLFile);
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
		case EHanType::HWPML:
		{
			if (nullptr == m_pHWPMLFile)
				return nullptr;
			
			return m_pHWPMLFile->GetDocInfo();
		}
		default:
			return nullptr;
	}
}

const CHWPRecordBorderFill* CWriterContext::GetBorderFill(short shId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo)
		return nullptr;

	return (CHWPRecordBorderFill*)pDocInfo->GetBorderFill(shId);
}

const CHWPRecordParaShape* CWriterContext::GetParaShape(int nId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo)
		return nullptr;

	return (CHWPRecordParaShape*)pDocInfo->GetParaShape(nId);
}

const CHWPRecordStyle* CWriterContext::GetParaStyle(short shId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo)
		return nullptr;

	return (CHWPRecordStyle*)pDocInfo->GetStyle(shId);
}

const CHWPRecordCharShape* CWriterContext::GetCharShape(int nId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo)
		return nullptr;

	return (CHWPRecordCharShape*)pDocInfo->GetCharShape(nId);
}

const CHWPRecordNumbering* CWriterContext::GetNumbering(short shId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo)
		return nullptr;

	return (CHWPRecordNumbering*)pDocInfo->GetNumbering(shId - 1);
}

const CHWPRecordBullet* CWriterContext::GetBullet(short shId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo)
		return nullptr;

	return (CHWPRecordBullet*)pDocInfo->GetBullet(shId);
}

const CHwpRecordTabDef* CWriterContext::GetTabDef(short shId)
{
	const CHWPDocInfo* pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo)
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

bool CWriterContext::GetBinBytes(const HWP_STRING& sId, CHWPStream& oBuffer, HWP_STRING& sFileName)
{
	const CHWPDocInfo* pDocInfo = nullptr;

	pDocInfo = GetDocInfo();

	if (nullptr == pDocInfo)
		return false;

	const CHWPRecordBinData* pBinData = dynamic_cast<const CHWPRecordBinData*>(pDocInfo->GetBinData(sId));

	if (nullptr == pBinData)
		return false;

	if (EType::LINK == pBinData->GetType() ||
	    EType::EMBEDDING == pBinData->GetType())
	{
		switch (m_eType)
		{
			case EHanType::HWPX:
			{
				sFileName = NSFile::GetFileName(pBinData->GetPath());
				return m_pHWPXFile->GetChildStream(pBinData->GetPath(), oBuffer);
			}
			case EHanType::HWPML:
			{
				sFileName = pBinData->GetItemID() + L'.' + pBinData->GetFormat();
				return m_pHWPMLFile->GetBinData(pBinData->GetItemID(), oBuffer);
			}
			default:
				break;
		}
	}

	std::wostringstream oStringStream;

	switch (m_eType)
	{
		case EHanType::HWP:
		{
			oStringStream << L"BIN" << std::setw(4) << std::setfill(L'0') << std::hex << pBinData->GetBinDataID() << L"." << pBinData->GetFormat();
			sFileName = oStringStream.str();
			return m_pHWPFile->GetChildStream(oStringStream.str(), pBinData->GetCompressed(), oBuffer);
		}
		case EHanType::HWPX:
		{
			oStringStream << sId << L"." << pBinData->GetFormat();
			sFileName = oStringStream.str();
			return m_pHWPXFile->GetChildStream(L"BinData/" + oStringStream.str(), oBuffer);
		}
		default:
			break;
	}

	return false;
}

HWP_STRING CWriterContext::GetBinFormat(const HWP_STRING& sId)
{
	//TODO::реализовать
	return HWP_STRING();
}
}
