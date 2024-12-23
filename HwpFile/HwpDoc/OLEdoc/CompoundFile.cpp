#include "CompoundFile.h"

namespace HWP
{
CCompoundFile::CCompoundFile(const STRING& sFileName)
	: m_fFile(sFileName, std::ios::in | std::ios::binary), m_nSectorSize(512)
{}

CCompoundFile::~CCompoundFile()
{
	Close();

	CLEAR_ARRAY(CDirectoryEntry, m_arDirectoryEntries);
}

const CDirectoryEntry* CCompoundFile::GetEntry(const STRING& sFileName) const
{
	VECTOR<CDirectoryEntry*>::const_iterator itFound = std::find_if(m_arDirectoryEntries.cbegin(), m_arDirectoryEntries.cend(), [&sFileName](const CDirectoryEntry* pDirectoryEntry){ return sFileName == pDirectoryEntry->GetDirectoryEntryName();});

	if (m_arDirectoryEntries.cend() == itFound)
		return nullptr;

	return *itFound;
}

bool CCompoundFile::GetComponent(const STRING& sEntryName, CHWPStream& oBuffer)
{
	const CDirectoryEntry* pEntry = GetEntry(sEntryName);

	if (nullptr == pEntry)
		return false;

	return Read(*pEntry, oBuffer);
}

VECTOR<CDirectoryEntry*> CCompoundFile::GetChildEntries(const CDirectoryEntry* pBaseEntry)
{
	//TODO:: реализовать
	VECTOR<int> arEntryIdx;

	int nIndex = 0;
	if (nullptr == pBaseEntry)
	{
		if (!m_arDirectoryEntries.empty())
			nIndex = m_arDirectoryEntries.at(0)->GetChildID();
		else
			return VECTOR<CDirectoryEntry*>();
	}
	else
		nIndex = pBaseEntry->GetChildID();

	AddSiblings(arEntryIdx, nIndex);

	VECTOR<CDirectoryEntry*> arEntries;

	for (int nCurIndex : arEntryIdx)
	{
		if (nCurIndex < m_arDirectoryEntries.size())
			arEntries.push_back(m_arDirectoryEntries.at(nCurIndex));
	}

	return arEntries;
}

VECTOR<CDirectoryEntry*> CCompoundFile::GetChildEntries(const STRING& sBaseEntryName)
{
	VECTOR<CDirectoryEntry*>::const_iterator itFound = std::find_if(m_arDirectoryEntries.cbegin(), m_arDirectoryEntries.cend(),
	                                                                [&sBaseEntryName](const CDirectoryEntry* pDirectoryEntry)
	                                                                { return sBaseEntryName == pDirectoryEntry->GetDirectoryEntryName(); });

	if (m_arDirectoryEntries.cend() == itFound)
		return VECTOR<CDirectoryEntry*>();

	return GetChildEntries(*itFound);
}

bool CCompoundFile::Read(const CDirectoryEntry& oEntry, CHWPStream& oBuffer)
{
	BYTE *pBuffer = new(std::nothrow) BYTE[oEntry.GetStreamSize()];

	if (nullptr == pBuffer)
		return false;

	oBuffer.SetStream(pBuffer, oEntry.GetStreamSize(), false);

	VECTOR<int> arStreamContainerSectors;

	int nRemainSize = (int)oEntry.GetStreamSize();

	if (oEntry.GetStreamSize() < m_nMiniStreamCutoffSize)
	{
		if (m_arDirectoryEntries.empty())
			return false;

		arStreamContainerSectors = m_arDirectoryEntries.at(0)->GetSecNums();

		for (int nSecNum : oEntry.GetSecNums())
		{
			int nStreamIndex = nSecNum / (m_nSectorSize / 64);
			int nStreamOffset = nSecNum % (m_nSectorSize / 64);

			if (nStreamIndex >= arStreamContainerSectors.size())
				return false;

			int nSatID = arStreamContainerSectors.at(nStreamIndex);

			m_fFile.seekg((nSatID + 1) * m_nSectorSize + nStreamOffset * 64);
			m_fFile.read(oBuffer.GetCurPtr(), nRemainSize >= 64 ? 64 : nRemainSize);

			oBuffer.Skip(m_fFile.gcount());
			nRemainSize -= m_fFile.gcount();
		}
	}
	else
	{
		for (int nSecNum : oEntry.GetSecNums())
		{
			if (0xFFFFFFFE == nSecNum)
				continue;

			// readStream
			m_fFile.seekg((nSecNum + 1) * m_nSectorSize);
			m_fFile.read(oBuffer.GetCurPtr(), nRemainSize >= m_nSectorSize ? m_nSectorSize : nRemainSize);

			oBuffer.Skip(m_fFile.gcount());
			nRemainSize -= m_fFile.gcount();
		}
	}

	oBuffer.MoveToStart();

	return true;
}

bool CCompoundFile::Open()
{
	if (m_fFile.bad())
		return false;

	CHWPStream oBuffer(m_nSectorSize);

	if (!oBuffer.IsValid())
		return false;

	m_fFile.read(oBuffer.GetCurPtr(), m_nSectorSize);

	if (m_fFile.gcount() != m_nSectorSize || !ParseHeader(oBuffer))
		return false;

	if (0x0004 == m_nMajorVersion)
	{
		m_fFile.seekg(4096);
		m_nSectorSize = 4096;
	}

	// collect MSAT SecID
	int nSecID = m_nFirstSecIDMSAT;

	if (0xFFFFFFFE != nSecID && 0xFFFFFFFF != nSecID)
		ReadMSATSector(nSecID);

	// Directory
	nSecID = m_nFirstSecIDDirectory;
	m_arDirectorySecIDs.push_back(nSecID);

	while (0xFFFFFFFE != nSecID)
	{
		int nSatIndex = nSecID / (m_nSectorSize / 4);

		// collect Directory SecID
		if (nSatIndex >= m_arSATs.size())
			return false;

		int nSatID = m_arSATs.at(nSatIndex);
		VECTOR<int> arSecIDs = GetSecIDsFromSAT(nSatID, nSatIndex, nSecID);

		if (arSecIDs.empty())
			continue;

		for (int nSecID : arSecIDs)
		{
			if (m_arDirectorySecIDs.cend() == std::find(m_arDirectorySecIDs.cbegin(), m_arDirectorySecIDs.cend(), nSecID))
				m_arDirectorySecIDs.push_back(nSecID);
		}

		nSecID = arSecIDs.back();
	}

	// collect Directory Entries
	for (int nSecIDDirectory : m_arDirectorySecIDs)
	{
		if (0xFFFFFFFE != nSecIDDirectory)
			ReadDirectorySector(nSecIDDirectory);
	}

	// collect SSAT SecID
	int nLastSecID = m_nFirstSecIDSSAT;
	m_arSSATSecIDs.push_back(nLastSecID);

	while (0xFFFFFFFE != nLastSecID)
	{
		int nSatIndex = nLastSecID / (m_nSectorSize / 4);

		if (nSatIndex >= m_arSATs.size())
			return false;

		int nSatID = m_arSATs.at(nSatIndex);
		VECTOR<int> arSecIDs = GetSecIDsFromSAT(nSatID, nSatIndex, nLastSecID);

		if (arSecIDs.empty())
			continue;

		for (int nSecID : arSecIDs)
		{
			if (m_arSSATSecIDs.cend() == std::find(m_arSSATSecIDs.cbegin(), m_arSSATSecIDs.cend(), nSecID))
				m_arSSATSecIDs.push_back(nSecID);
		}

		nLastSecID = arSecIDs.back();
	}

	// collect SecID of Stream for each Directory entries
	for (CDirectoryEntry* pEntry : m_arDirectoryEntries)
	{
		if (0x05 == pEntry->GetObjectType()) // Root Storage
		{
			// Short Stream container Stream SecID
			nLastSecID = pEntry->GetStartingSectorID();
			VECTOR<int>& arEntrySecNums{pEntry->GetSecNums()};
			arEntrySecNums.push_back(nLastSecID);

			while (0xFFFFFFFE != nLastSecID)
			{
				int nContainerIndex = nLastSecID / (m_nSectorSize / 4);

				if (nContainerIndex >= m_arSATs.size())
					return false;

				int nSatID = m_arSATs.at(nContainerIndex);
				VECTOR<int> arSecIDs = GetSecIDsFromSAT(nSatID, nContainerIndex, nLastSecID);

				if (arSecIDs.empty())
					continue;

				for (int nSecID : arSecIDs)
				{
					if (arEntrySecNums.cend() == std::find(arEntrySecNums.cbegin(), arEntrySecNums.cend(), nSecID))
						arEntrySecNums.push_back(nSecID);
				}

				nLastSecID = arSecIDs.back();
			}
		}
		else if (0x02 == pEntry->GetObjectType()) // Stream
		{
			VECTOR<int>& arEntrySecNums{pEntry->GetSecNums()};
			arEntrySecNums.push_back(pEntry->GetStartingSectorID());

			if (pEntry->GetStreamSize() < m_nMiniStreamCutoffSize)
			{
				// ShortStream
				int nLastSSecID = pEntry->GetStartingSectorID();

				while (0xFFFFFFFE != nLastSSecID)
				{
					int nSSatIndex = nLastSSecID / (m_nSectorSize / 4);

					if (nSSatIndex >= m_arSSATSecIDs.size())
						return false;

					int nSSatID = m_arSSATSecIDs.at(nSSatIndex);

					VECTOR<int> arSecIDs = GetSecIDsFromSAT(nSSatID, nSSatIndex, nLastSSecID);

					if (arSecIDs.empty())
						continue;

					for (int nSecID : arSecIDs)
					{
						if (arEntrySecNums.cend() == std::find(arEntrySecNums.cbegin(), arEntrySecNums.cend(), nSecID))
							arEntrySecNums.push_back(nSecID);
					}

					nLastSSecID = arEntrySecNums.back();
				}
			}
			else
			{
				// Stream
				int nLastSectID = pEntry->GetStartingSectorID();

				while (0xFFFFFFFE != nLastSectID)
				{
					int nSatIndex = nLastSectID / (m_nSectorSize / 4);

					if (nSatIndex >= m_arSATs.size())
						return false;

					int nSatID = m_arSATs.at(nSatIndex);

					VECTOR<int> arSecIDs = GetSecIDsFromSAT(nSatID, nSatIndex, nLastSectID);

					if (arSecIDs.empty())
						continue;

					for (int nSecID : arSecIDs)
					{
						if (arEntrySecNums.cend() == std::find(arEntrySecNums.cbegin(), arEntrySecNums.cend(), nSecID))
							arEntrySecNums.push_back(nSecID);
					}

					nLastSectID = arEntrySecNums.back();
				}

			}
		}
		else
			continue;
	}

	return true;
}

void CCompoundFile::Close()
{
	if (m_fFile.is_open())
		m_fFile.close();
}

void CCompoundFile::AddSiblings(VECTOR<int>& arIndexs, int nCurrentIndex)
{
	if (-1 == nCurrentIndex)
		return;

	VECTOR<int>::iterator itFoundIndex = std::find(arIndexs.begin(), arIndexs.end(), nCurrentIndex);
	if (arIndexs.end() == itFoundIndex)
		arIndexs.push_back(nCurrentIndex);

	if (nCurrentIndex >= m_arDirectoryEntries.size())
		return;

	const int nLeftSibling  = m_arDirectoryEntries.size() > nCurrentIndex ? m_arDirectoryEntries.at(nCurrentIndex)->GetLeftSiblingID()  : -1;
	const int nRightSibling = m_arDirectoryEntries.size() > nCurrentIndex ? m_arDirectoryEntries.at(nCurrentIndex)->GetRightSiblingID() : -1;

	if (-1 != nRightSibling)
	{
		const int nElderIndex = itFoundIndex - arIndexs.begin();
		arIndexs.insert(arIndexs.begin() + nElderIndex + 1, nRightSibling);

		if (nRightSibling < m_arDirectoryEntries.size())
			AddSiblings(arIndexs, nRightSibling);
	}

	if (-1 != nLeftSibling)
	{
		const int nElderIndex = itFoundIndex - arIndexs.begin();
		arIndexs.insert(arIndexs.begin() + nElderIndex, nLeftSibling);

		if (nLeftSibling < m_arDirectoryEntries.size())
			AddSiblings(arIndexs, nLeftSibling);
	}
}

VECTOR<int> CCompoundFile::GetSecIDsFromSAT(int nSecID, int nSatIndex, int nSecIDSSAT)
{
	CHWPStream oBuffer(m_nSectorSize);

	if (!oBuffer.IsValid())
		return VECTOR<int>();

	m_fFile.seekg((nSecID + 1) * m_nSectorSize);
	m_fFile.read(oBuffer.GetCurPtr(), m_nSectorSize);

	if (m_fFile.gcount() != m_nSectorSize)
		return VECTOR<int>();

	int nCurrSecID = nSecIDSSAT;
	VECTOR<int> arSecIDs;

	oBuffer.MoveTo(nCurrSecID % (m_nSectorSize / 4) * 4);

	while (((m_nSectorSize / 4) * nSatIndex <= nCurrSecID) && (nCurrSecID < (m_nSectorSize / 4) * (nSatIndex + 1)))
	{
		oBuffer.ReadInt(nCurrSecID);
		arSecIDs.push_back(nCurrSecID);

		if (0xFFFFFFFE == nCurrSecID)
			break;

		oBuffer.MoveTo(nCurrSecID % (m_nSectorSize / 4) * 4);
	}

	return arSecIDs;
}

void CCompoundFile::ReadDirectorySector(int nSecID)
{
	CHWPStream oBuffer(m_nSectorSize);

	if (!oBuffer.IsValid())
		return;

	m_fFile.seekg((nSecID + 1) * m_nSectorSize);
	m_fFile.read(oBuffer.GetCurPtr(), m_nSectorSize);

	if (m_fFile.gcount() == m_nSectorSize)
		ParseDirectorySector(oBuffer);
}

void CCompoundFile::ReadSSATSector(int nSecID)
{
	CHWPStream oBuffer(m_nSectorSize);

	if (!oBuffer.IsValid())
		return;

	m_fFile.seekg((nSecID + 1) * m_nSectorSize);
	m_fFile.read(oBuffer.GetCurPtr(), m_nSectorSize);

	if (m_fFile.gcount() == m_nSectorSize)
		ParseSSATSector(oBuffer);
}

void CCompoundFile::ReadMSATSector(int nSecID)
{
	CHWPStream oBuffer(m_nSectorSize);

	if (!oBuffer.IsValid())
		return;

	m_fFile.seekg((nSecID + 1) * m_nSectorSize);
	m_fFile.read(oBuffer.GetCurPtr(), m_nSectorSize);

	if (m_fFile.gcount() == m_nSectorSize)
		ParseMSATSector(oBuffer);
}

void CCompoundFile::ParseSectors(int nSecID, CHWPStream& oBuffer)
{
	int nSectorType;
	oBuffer.ReadInt(nSectorType); // for DIFAT, FAT, MiniFAT,

	if (m_arSATs.cend() != std::find(m_arSATs.cbegin(), m_arSATs.cend(), nSecID))
		ParseSATSector(nSecID, oBuffer);

	if (nSecID < m_nFirstSecIDDirectory &&nSecID == m_nFirstSecIDSSAT)
		ParseSSATSector(oBuffer);
}

void CCompoundFile::ParseSATSector(int nSecID, CHWPStream& oBuffer)
{
	for (int nIndex = nSecID * (m_nSectorSize / 4); oBuffer.CanRead(4); ++nIndex)
	{
		int nNextSecID;
		oBuffer.ReadInt(nNextSecID);

		TSector oSectorInChain;
		oSectorInChain.m_nSectorNum = nIndex;

		switch ((unsigned int)nNextSecID)
		{
			case 0xFFFFFFFC: // DIFAT
			{
				oSectorInChain.m_eType = ESectorType::MSAT;
				break;
			}
			case 0xFFFFFFFD: // FAT
			{
				oSectorInChain.m_eType = ESectorType::SAT;
				break;
			}
			case 0xFFFFFFFA: // maximum regular sector number
			case 0xFFFFFFFE: // ENDOFCHAIN
			{
				oSectorInChain.m_eType = ESectorType::ENDOFCHAIN;
				// Directory
				// FAT
				// MiniFAT
				// DIFAT
				// Stream (User-Defined Data)
				// Range Lock
				break;
			}
			case 0xFFFFFFFF: // unallocated
			{
				oSectorInChain.m_eType = ESectorType::FREE;
				break;
			}
			case 0xFFFFFFFB: // Reserved for future use.
			{
				oSectorInChain.m_eType = ESectorType::CONTINUE;
				break;
			}
			default:
			{
				oSectorInChain.m_eType = ESectorType::CONTINUE;
				oSectorInChain.m_nNextNum = nNextSecID;
			}
		}

		if (0xFFFFFFFF > nNextSecID)
			m_arSectors.push_back(oSectorInChain);
	}
}

void CCompoundFile::ParseDirectorySector(CHWPStream& oBuffer)
{
	for (int nIndex = 0; nIndex <= m_nSectorSize - 128; nIndex += 128)
	{
		oBuffer.MoveTo(64 + nIndex);

		short shEntryNameLen;
		STRING sDirectiryEnryName;

		oBuffer.ReadShort(shEntryNameLen);
		oBuffer.MoveTo(nIndex);
		oBuffer.ReadString(sDirectiryEnryName, shEntryNameLen, EStringCharacter::UTF16);
		oBuffer.MoveTo(66 + nIndex);
		//
		int nObjectType = oBuffer.ReadByte() & 0xFF;
		int nColorFlag = oBuffer.ReadByte() & 0xFF;

		int nLeftSiblingID, nRightSiblingID, nChildID;
		oBuffer.ReadInt(nLeftSiblingID);
		oBuffer.ReadInt(nRightSiblingID);
		oBuffer.ReadInt(nChildID);

		long long lClsID1, lClsID2;
		oBuffer.ReadLong(lClsID1);
		oBuffer.ReadLong(lClsID2);

		int nStateBit;
		oBuffer.ReadInt(nStateBit);

		long long lCreationTime, lModifiedTime;
		oBuffer.ReadLong(lCreationTime);
		oBuffer.ReadLong(lModifiedTime);

		int nStartingSectorID;
		oBuffer.ReadInt(nStartingSectorID);

		long long lStreamSize;
		oBuffer.ReadLong(lStreamSize);

		CDirectoryEntry *pDirectoryEntry = new CDirectoryEntry();

		if (nullptr == pDirectoryEntry)
			continue;

		pDirectoryEntry->SetDirectoryEntryName(sDirectiryEnryName);
		pDirectoryEntry->SetObjectType(nObjectType);
		pDirectoryEntry->SetColorFlag(nColorFlag);
		pDirectoryEntry->SetLeftSiblingID(nLeftSiblingID);
		pDirectoryEntry->SetRightSiblingID(nRightSiblingID);
		pDirectoryEntry->SetChildID(nChildID);
		pDirectoryEntry->SetClsID1(lClsID1);
		pDirectoryEntry->SetClsID2(lClsID2);
		pDirectoryEntry->SetStateBit(nStateBit);
		pDirectoryEntry->SetCreationTime(lCreationTime);
		pDirectoryEntry->SetModifiedTime(lModifiedTime);
		pDirectoryEntry->SetStartingSectorID(nStartingSectorID);
		pDirectoryEntry->SetStreamSize(lStreamSize);

		m_arDirectoryEntries.push_back(pDirectoryEntry);
	}
}

void CCompoundFile::ParseSSATSector(CHWPStream& oBuffer)
{
	int nNextSectorId;

	while (oBuffer.CanRead(4))
	{
		oBuffer.ReadInt(nNextSectorId);

		if (0xFFFFFFFF != nNextSectorId)
			m_arSStreams.push_back(nNextSectorId);
	}
}

void CCompoundFile::ParseMSATSector(CHWPStream& oBuffer)
{
	int nSector;

	while (oBuffer.CanRead(4))
	{
		oBuffer.ReadInt(nSector);

		if (0xFFFFFFFF != nSector)
			m_arSATs.push_back(nSector);
	}

	int nNextSecIDMSAT;
	oBuffer.ReadInt(nNextSecIDMSAT);

	if (0xFFFFFFFE != nNextSecIDMSAT && 0xFFFFFFFF != nNextSecIDMSAT)
		ReadMSATSector(nNextSecIDMSAT);
}

bool CCompoundFile::ParseHeader(CHWPStream& oBuffer)
{
	CheckSignature(oBuffer);

	oBuffer.Skip(16); // Header CLSID
	m_nMinorVersion = oBuffer.ReadShort();

	if (0x003E != m_nMinorVersion)
		return false; //TODO:: возможно следует просто пропускать такие несоответсвия

	m_nMajorVersion = oBuffer.ReadShort();

	if (0x0003 != m_nMajorVersion && 0x0004 != m_nMajorVersion)
		return false;

	short shByteOrder = oBuffer.ReadShort();

	if (0xFFFE != (unsigned short)shByteOrder)
		return false;

	int nSectorShift = oBuffer.ReadShort();

	m_nSectorSize = std::pow(2., (double)nSectorShift);

	if ((0x0003 == m_nMajorVersion && 0x0009 != nSectorShift) ||
	    (0x0004 == m_nMajorVersion && 0x000C != nSectorShift))
		return false;

	nSectorShift = oBuffer.ReadShort();
	m_nShortSectorSize = std::pow(2., (double)nSectorShift);

	if (0x0006 != nSectorShift)
		return false;

	oBuffer.Skip(6); // reserved
	oBuffer.ReadInt(m_nNumDirectory);

	if (0x0003 == m_nMajorVersion && 0x0000 != m_nNumDirectory)
		return false;

	oBuffer.ReadInt(m_nNumSAT);
	oBuffer.ReadInt(m_nFirstSecIDDirectory);
	oBuffer.Skip(4); // Transaction Signature Number
	oBuffer.ReadInt(m_nMiniStreamCutoffSize);

	if (0x00001000 != m_nMiniStreamCutoffSize)
		return false;

	oBuffer.ReadInt(m_nFirstSecIDSSAT);
	oBuffer.ReadInt(m_nNumSSAT);
	oBuffer.ReadInt(m_nFirstSecIDMSAT);
	oBuffer.ReadInt(m_nNumMSAT);

	int nSector;
	while(oBuffer.Tell() < 512)
	{
		oBuffer.ReadInt(nSector);

		if (0xFFFFFFFF != nSector)
			m_arSATs.push_back(nSector);
	}

	return true;
}

bool CCompoundFile::CheckSignature(CHWPStream& oBuffer)
{
	if (!oBuffer.CanRead(8))
		return false;

	BYTE arBufSig[8];

	if (!oBuffer.ReadBytes(arBufSig, 8))
		return false;

	if ((BYTE)0xD0 == arBufSig[0] && (BYTE)0xCF == arBufSig[1] && (BYTE)0x11 == arBufSig[2] && (BYTE)0xE0 == arBufSig[3] &&
	    (BYTE)0xA1 == arBufSig[4] && (BYTE)0xB1 == arBufSig[5] && (BYTE)0x1A == arBufSig[6] && (BYTE)0xE1 == arBufSig[7])
		return true;

	return false;
}

ESectorType CCompoundFile::LookupSectorType(CHWPStream& oBuffer)
{
	int nSector;

	while (oBuffer.CanRead(4))
	{
		oBuffer.ReadInt(nSector);

		if (0xFFFFFFFC == nSector)
			return ESectorType::MSAT;
		else if (0xFFFFFFFD == nSector)
			return ESectorType::SAT;
	}

	return ESectorType::CONTINUE;
}
}
