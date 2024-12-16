#include "HWPFile.h"
#include "HWPDocInfo.h"

#include "../DesktopEditor/common/Directory.h"
#include <regex>

namespace HWP
{
CHWPFile::CHWPFile(const STRING& sFileName)
	: m_sFileName(sFileName), m_oOleFile(sFileName), m_oDocInfo(this)
{}

CHWPFile::~CHWPFile()
{
	CLEAR_ARRAY(CHWPSection, m_arBodyTexts);
	CLEAR_ARRAY(CHWPSection, m_arViewTexts);
}

std::vector<CHWPSection*> CHWPFile::GetSections()
{
	if (m_oFileHeader.Distributable())
		return m_arViewTexts;
	else
		return m_arBodyTexts;
}

const CCompoundFile* CHWPFile::GetOleFile() const
{
	return &m_oOleFile;
}

bool CHWPFile::Detect()
{
	// read CompoundFile structure
	if (!m_oOleFile.Open() || !GetFileHeader())
	{
		m_oOleFile.Close();
		return false;
	}

	return true;
}

bool CHWPFile::Open()
{
	if (m_oFileHeader.SignatureEmpty() || m_oFileHeader.VersionEmpty())
		Detect();

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

void CHWPFile::Close()
{
	m_oOleFile.Close();
}

void CHWPFile::SaveHWPComponent()
{
	ECompressed eCompressed = m_oFileHeader.Compressed() ? ECompressed::COMPRESS : ECompressed::NO_COMPRESS;

	// Save internal component for debugging purpose.
	// TODO:: перенести
}

bool CHWPFile::GetFileHeader()
{
	CHWPStream oBuffer;
	if (!GetComponent("FileHeader", oBuffer))
		return false;

	return m_oFileHeader.Parse(oBuffer);
}

const CHWPDocInfo* CHWPFile::GetDocInfo() const
{
	return &m_oDocInfo;
}

bool CHWPFile::GetDocInfo(int nVersion)
{
	CHWPStream oBuffer;
	if (m_oFileHeader.Compressed())
	{
		CHWPStream oTempBuffer;

		if (!GetComponent("DocInfo", oTempBuffer) && !Unzip(oTempBuffer, oBuffer))
			return false;
	}
	else
	{
		if (GetComponent("DocInfo", oBuffer))
			return false;
	}

	return m_oDocInfo.Parse(oBuffer, m_nVersion);
}

bool CHWPFile::GetComponent(const std::string& sEntryName, CHWPStream& oBuffer)
{
	return m_oOleFile.GetComponent(sEntryName, oBuffer);
}

//TODO:: написанно, что данные методы используются только для отображения в LibbreOffice
// проверить и если нужны будут, то реализовать
VECTOR<CDirectoryEntry*> CHWPFile::GetBinData()
{
	return VECTOR<CDirectoryEntry*>();
}

void CHWPFile::SetBinData(const std::vector<CDirectoryEntry*>& arBinData)
{

}

VECTOR<CHWPPargraph*> CHWPFile::GetParas()
{
	return VECTOR<CHWPPargraph*>();
}

void CHWPFile::AddParas(const std::vector<CHWPPargraph*>& arParas)
{

}
//------------

void CHWPFile::SaveChildEntries(const std::string& sBasePath, const std::string& sStorageName, ECompressed eCompressed)
{
	// TODO:: перенести
}

CDirectoryEntry* CHWPFile::FindChildEntry(const std::string& sBasePath, const CDirectoryEntry& oBaseEntry, const std::string& sEntryName)
{
	for (CDirectoryEntry* pEntry : m_oOleFile.GetChildEntries(&oBaseEntry))
	{
		if (0x01 == pEntry->GetObjectType())
		{
			//TODO:: проверить
			STRING sChildPath = sBasePath + FILE_SEPARATOR_CHAR + pEntry->GetDirectoryEntryName();
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

STRING CHWPFile::SaveChildEntry(const std::string& sRootPath, const std::string& sEntryName, ECompressed eCompressed)
{
	//TODO:: перенести
	return STRING();
}

bool CHWPFile::GetChildStream(const std::string& sEntryName, ECompressed eCompressed, CHWPStream& oBuffer)
{
	STRING sRegexStr = ".*" + STRING(FILE_SEPARATOR_STRA) + "([" + STRING(FILE_SEPARATOR_STRA) + "]+)$";

	STRING sShortFilename = std::regex_replace(m_sFileName, std::regex(sRegexStr), "$1");
	sShortFilename = std::regex_replace(sShortFilename, std::regex("(.*)\\.hwp$"), "$1");

	CDirectoryEntry *pTargetEntry = nullptr;

	VECTOR<CDirectoryEntry*> arEntries = m_oOleFile.GetChildEntries("Root Entry");

	for (CDirectoryEntry* pEntry : arEntries)
	{
		if (0x01 == pEntry->GetObjectType())
		{
			STRING sChildPath = sShortFilename + FILE_SEPARATOR_CHAR + pEntry->GetDirectoryEntryName();
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

bool CHWPFile::Unzip(CHWPStream& oInput, CHWPStream& oBuffer)
{
	//TODO:: реализовать
	return false;
}

bool CHWPFile::Decrypt(CHWPStream& oInput, CHWPStream& oBuffer)
{
	int nHeader;
	oInput.ReadInt(nHeader);

	int nTagNum = nHeader & 0x3FF; // 10 bits (0 - 9 bit)
	int nLevel = (nHeader & 0xFFC00) >> 10; // 10 bits (10-19 bit)
	int nSize = (nHeader & 0xFFF00000) >> 20; // 12 bits (20-31 bit)

	EHWPTag eTag = GetTagFromNum(nTagNum);

	if (EHWPTag::HWPTAG_DISTRIBUTE_DOC_DATA != eTag)
		return false;

	if (256 != nSize)
		return false;

	//TODO:: реализовать
	return false;
}

bool CHWPFile::GetBodyText(int nVersion)
{
	VECTOR<CDirectoryEntry*> arSections{m_oOleFile.GetChildEntries("BodyText")};

	for (const CDirectoryEntry* pSection : arSections)
	{
		CHWPSection *pHwpSection = new CHWPSection();
		CHWPStream oBuffer;

		if (m_oFileHeader.Compressed())
		{
			CHWPStream oTempBuffer;
			if (!m_oOleFile.Read(*pSection, oTempBuffer) && !Unzip(oTempBuffer, oBuffer))
				return false;
		}
		else if (!m_oOleFile.Read(*pSection, oBuffer))
			return false;

		pHwpSection->Parse(oBuffer, m_nVersion);
		m_arBodyTexts.push_back(pHwpSection);
	}

	return true;
}

bool CHWPFile::GetViewText(int nVersion)
{
	VECTOR<CDirectoryEntry*> arSections{m_oOleFile.GetChildEntries("ViewText")};

	for (const CDirectoryEntry* pSection : arSections)
	{
		CHWPSection *pHwpSection = new CHWPSection();
		CHWPStream oBuffer;

		if (m_oFileHeader.Compressed())
		{
			CHWPStream oTempDecryptBuffer, oTempBuffer;
			if (!m_oOleFile.Read(*pSection, oTempDecryptBuffer) && !!Decrypt(oTempDecryptBuffer, oTempBuffer) && !Unzip(oTempBuffer, oBuffer))
				return false;
		}
		else
		{
			CHWPStream oTempBuffer;

			if (!m_oOleFile.Read(*pSection, oTempBuffer) && !Decrypt(oTempBuffer, oBuffer))
				return false;
		}

		pHwpSection->Parse(oBuffer, m_nVersion);
		m_arViewTexts.push_back(pHwpSection);
	}

	return true;
}
}
