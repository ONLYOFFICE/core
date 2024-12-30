#include "HWPFile_Private.h"
#include "HWPDocInfo.h"

#include "../OfficeUtils/src/OfficeUtils.h"
#include "../DesktopEditor/common/Directory.h"

#include <regex>

#define DEFAULT_BUFFER_SIZE 8096

namespace HWP
{
CHWPFile_Private::CHWPFile_Private(const HWP_STRING& sFileName)
	: m_sFileName(sFileName), m_oOleFile(sFileName), m_oDocInfo(this)
{}

CHWPFile_Private::~CHWPFile_Private()
{
	CLEAR_ARRAY(CHWPSection, m_arBodyTexts);
	CLEAR_ARRAY(CHWPSection, m_arViewTexts);
}

VECTOR<CHWPSection*> CHWPFile_Private::GetSections()
{
	if (m_oFileHeader.Distributable())
		return m_arViewTexts;
	else
		return m_arBodyTexts;
}

const CCompoundFile* CHWPFile_Private::GetOleFile() const
{
	return &m_oOleFile;
}

bool CHWPFile_Private::Detect()
{
	// read CompoundFile structure
	if (!m_oOleFile.Open() || !GetFileHeader())
	{
		m_oOleFile.Close();
		return false;
	}

	return true;
}

bool CHWPFile_Private::Open()
{
	if ((m_oFileHeader.SignatureEmpty() || m_oFileHeader.VersionEmpty()) && !Detect())
		return false;

	//TODO:: добавить отдельный метод StringToInt
	m_nVersion = std::stoi(m_oFileHeader.GetVersion());

	//TODO:: проверить данный момент
	if (m_oFileHeader.PasswordEncrypted())
		return false;

	if (!GetDocInfo(m_nVersion))
		return false;

	if (!m_oFileHeader.Distributable() && !GetBodyText(m_nVersion))
		return false;

	if (!m_oFileHeader.Distributable() && !GetViewText(m_nVersion))
		return false;

	return true;
}

void CHWPFile_Private::Close()
{
	m_oOleFile.Close();
}

void CHWPFile_Private::SaveHWPComponent()
{
	ECompressed eCompressed = m_oFileHeader.Compressed() ? ECompressed::COMPRESS : ECompressed::NO_COMPRESS;

	// Save internal component for debugging purpose.
	// TODO:: перенести
}

bool CHWPFile_Private::GetFileHeader()
{
	CHWPStream oBuffer;
	if (!GetComponent(L"FileHeader", oBuffer))
		return false;

	return m_oFileHeader.Parse(oBuffer);
}

const CHWPDocInfo* CHWPFile_Private::GetDocInfo() const
{
	return &m_oDocInfo;
}

bool CHWPFile_Private::GetDocInfo(int nVersion)
{
	CHWPStream oBuffer;
	if (m_oFileHeader.Compressed())
	{
		CHWPStream oTempBuffer;

		if (!GetComponent(L"DocInfo", oTempBuffer) || !Unzip(oTempBuffer, oBuffer))
			return false;
	}
	else
	{
		if (GetComponent(L"DocInfo", oBuffer))
			return false;
	}

	return m_oDocInfo.Parse(oBuffer, m_nVersion);
}

bool CHWPFile_Private::GetComponent(const HWP_STRING& sEntryName, CHWPStream& oBuffer)
{
	return m_oOleFile.GetComponent(sEntryName, oBuffer);
}

//TODO:: написанно, что данные методы используются только для отображения в LibbreOffice
// проверить и если нужны будут, то реализовать
VECTOR<CDirectoryEntry*> CHWPFile_Private::GetBinData()
{
	return VECTOR<CDirectoryEntry*>();
}

void CHWPFile_Private::SetBinData(const std::vector<CDirectoryEntry*>& arBinData)
{

}

VECTOR<CHWPPargraph*> CHWPFile_Private::GetParas()
{
	return VECTOR<CHWPPargraph*>();
}

void CHWPFile_Private::AddParas(const std::vector<CHWPPargraph*>& arParas)
{

}
//------------

void CHWPFile_Private::SaveChildEntries(const HWP_STRING& sBasePath, const HWP_STRING& sStorageName, ECompressed eCompressed)
{
	// TODO:: перенести
}

CDirectoryEntry* CHWPFile_Private::FindChildEntry(const HWP_STRING& sBasePath, const CDirectoryEntry& oBaseEntry, const HWP_STRING& sEntryName) const
{
	for (CDirectoryEntry* pEntry : m_oOleFile.GetChildEntries(&oBaseEntry))
	{
		if (0x01 == pEntry->GetObjectType())
		{
			//TODO:: проверить
			HWP_STRING sChildPath = sBasePath + FILE_SEPARATOR_STR + pEntry->GetDirectoryEntryName();
			return FindChildEntry(sChildPath, *pEntry, sEntryName);
		}
		else
		{
			if (sEntryName == pEntry->GetDirectoryEntryName())
				return pEntry;
		}
	}

	return nullptr;
}

HWP_STRING CHWPFile_Private::SaveChildEntry(const HWP_STRING& sRootPath, const HWP_STRING& sEntryName, ECompressed eCompressed)
{
	//TODO:: перенести
	return HWP_STRING();
}

bool CHWPFile_Private::GetChildStream(const HWP_STRING& sEntryName, ECompressed eCompressed, CHWPStream& oBuffer)
{
	// HWP_STRING sRegexStr = L".*" + HWP_STRING(FILE_SEPARATOR_STR) + L"([" + HWP_STRING(FILE_SEPARATOR_STR) + L"]+)$";

	// HWP_STRING sShortFilename = std::regex_replace(m_sFileName, std::wregex(sRegexStr), L"$1");
	// sShortFilename = std::regex_replace(sShortFilename, std::wregex(L"(.*)\\.hwp$"), L"$1");

	CDirectoryEntry *pTargetEntry = nullptr;

	VECTOR<CDirectoryEntry*> arEntries = m_oOleFile.GetChildEntries(L"Root Entry");

	for (CDirectoryEntry* pEntry : arEntries)
	{
		if (0x01 == pEntry->GetObjectType())
		{
			HWP_STRING sChildPath = /*sShortFilename + FILE_SEPARATOR_STR + */pEntry->GetDirectoryEntryName();
			pTargetEntry = FindChildEntry(sChildPath, *pEntry, sEntryName);

			if (nullptr != pTargetEntry)
				break;
		}
		else
		{
			if (sEntryName == pEntry->GetDirectoryEntryName())
			{
				pTargetEntry = pEntry;
				break;
			}
		}
	}

	if (nullptr == pTargetEntry)
		return false;

	if (ECompressed::COMPRESS == eCompressed || (ECompressed::FOLLOW_STORAGE == eCompressed && m_oFileHeader.Compressed()))
	{
		CHWPStream oTempBuffer;
		if (!m_oOleFile.Read(*pTargetEntry, oTempBuffer))
			return false;

		return Unzip(oTempBuffer, oBuffer);
	}
	else
		return m_oOleFile.Read(*pTargetEntry, oBuffer);

	return false;
}

bool CHWPFile_Private::Unzip(CHWPStream& oInput, CHWPStream& oBuffer)
{
	unsigned char* pInBuffer = new(std::nothrow) unsigned char[DEFAULT_BUFFER_SIZE];

	if (nullptr == pInBuffer)
		return false;

	unsigned char* pOutBuffer = new(std::nothrow) unsigned char[DEFAULT_BUFFER_SIZE];

	if (nullptr == pOutBuffer)
	{
		delete[] pInBuffer;
		return false;
	}

	CInflate oInflater;

	oInflater.SetOut(pOutBuffer, DEFAULT_BUFFER_SIZE);
	oInflater.Init2();
	oInflater.SetIn(pInBuffer, 0);

	int nRes = DEFLATE_OK;

	while (true)
	{
		const unsigned int unSize = oInput.ReadBytes((HWP_BYTE*)pInBuffer, DEFAULT_BUFFER_SIZE);

		oInflater.SetIn(pInBuffer, unSize);

		if (0 == unSize)
			break;

		while (oInflater.GetAvailIn() > 0)
		{
			nRes = oInflater.Process(DEFLATE_NO_FLUSH);

			if (DEFLATE_OK != nRes && DEFLATE_STREAM_END != nRes)
				break;

			if (oInflater.GetAvailOut() == 0)
			{
				oBuffer.WriteBytes((HWP_BYTE*)pOutBuffer, DEFAULT_BUFFER_SIZE);
				oInflater.SetOut(pOutBuffer, DEFAULT_BUFFER_SIZE);
			}

			if (DEFLATE_STREAM_END == nRes)
				break;
		}
	}

	bool bEnd = false;

	while (DEFLATE_OK == nRes || DEFLATE_STREAM_END == nRes)
	{
		nRes = oInflater.Process(DEFLATE_FINISH);

		if (DEFLATE_OK != nRes && DEFLATE_STREAM_END != nRes)
		{
			oInflater.End();
		}

		if (DEFLATE_STREAM_END == nRes)
			bEnd = true;

		if (oInflater.GetAvailOut() < DEFAULT_BUFFER_SIZE)
		{
			unsigned long ulSize = DEFAULT_BUFFER_SIZE - oInflater.GetAvailOut();
			oBuffer.WriteBytes((HWP_BYTE*)pOutBuffer, ulSize);
			oInflater.SetOut(pOutBuffer, DEFAULT_BUFFER_SIZE);
		}

		if (bEnd)
			break;
	}

	oInflater.End();

	delete[] pInBuffer;
	delete[] pOutBuffer;

	oBuffer.MoveToStart();

	return DEFLATE_OK == nRes || DEFLATE_STREAM_END == nRes;
}

bool CHWPFile_Private::Decrypt(CHWPStream& oInput, CHWPStream& oBuffer)
{
	int nHeader;
	oInput.ReadInt(nHeader);

	int nTagNum = nHeader & 0x3FF; // 10 bits (0 - 9 bit)
	int nLevel = (nHeader & 0xFFC00) >> 10; // 10 bits (10-19 bit)
	int nSize = (nHeader & 0xFFF00000) >> 20; // 12 bits (20-31 bit)

	EHWPTag eTag = GetTagFromNum(nTagNum);

	if (HWPTAG_DISTRIBUTE_DOC_DATA != eTag)
		return false;

	if (256 != nSize)
		return false;

	//TODO:: реализовать
	return false;
}

bool CHWPFile_Private::GetBodyText(int nVersion)
{
	VECTOR<CDirectoryEntry*> arSections{m_oOleFile.GetChildEntries(L"BodyText")};

	for (const CDirectoryEntry* pSection : arSections)
	{
		CHWPSection *pHwpSection = new CHWPSection();
		CHWPStream oBuffer;

		if (m_oFileHeader.Compressed())
		{
			CHWPStream oTempBuffer;
			if (!m_oOleFile.Read(*pSection, oTempBuffer) || !Unzip(oTempBuffer, oBuffer))
			{
				delete pHwpSection;
				return false;
			}
		}
		else if (!m_oOleFile.Read(*pSection, oBuffer))
		{
			delete pHwpSection;
			return false;
		}

		pHwpSection->Parse(oBuffer, m_nVersion);
		m_arBodyTexts.push_back(pHwpSection);
	}

	return true;
}

bool CHWPFile_Private::GetViewText(int nVersion)
{
	VECTOR<CDirectoryEntry*> arSections{m_oOleFile.GetChildEntries(L"ViewText")};

	for (const CDirectoryEntry* pSection : arSections)
	{
		CHWPSection *pHwpSection = new CHWPSection();
		CHWPStream oBuffer;

		if (m_oFileHeader.Compressed())
		{
			CHWPStream oTempDecryptBuffer, oTempBuffer;
			if (!m_oOleFile.Read(*pSection, oTempDecryptBuffer) || !Decrypt(oTempDecryptBuffer, oTempBuffer) || !Unzip(oTempBuffer, oBuffer))
				return false;
		}
		else
		{
			CHWPStream oTempBuffer;

			if (!m_oOleFile.Read(*pSection, oTempBuffer) || !Decrypt(oTempBuffer, oBuffer))
			{
				delete pHwpSection;
				return false;
			}
		}

		pHwpSection->Parse(oBuffer, m_nVersion);
		m_arViewTexts.push_back(pHwpSection);
	}

	return true;
}
}
