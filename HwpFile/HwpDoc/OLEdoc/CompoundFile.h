#ifndef COMPOUNDFILE_H
#define COMPOUNDFILE_H

#include "Sector.h"
#include "DirectoryEntry.h"
#include "../HWPStream.h"
#include "../../../DesktopEditor/common/File.h"

namespace HWP
{
class CCompoundFile
{
	NSFile::CFileBinary m_oFile;
	int m_nMinorVersion;
	int m_nMajorVersion;
	int m_nSectorSize;
	int m_nShortSectorSize;
	int m_nNumDirectory;    // Support only in version 4
	int m_nNumSAT;
	int m_nFirstSecIDDirectory;
	int m_nMiniStreamCutoffSize;
	int m_nFirstSecIDSSAT;
	int m_nNumSSAT;
	int m_nFirstSecIDMSAT;
	int m_nNumMSAT;

	VECTOR<TSector> m_arSectors;
	VECTOR<int> m_arSATs;
	VECTOR<int> m_arSSATSecIDs;
	VECTOR<int> m_arDirectorySecIDs;
	VECTOR<int> m_arSStreamSecIDs;
	VECTOR<int> m_arSStreams;
	VECTOR<CDirectoryEntry*> m_arDirectoryEntries;
public:
	CCompoundFile(const HWP_STRING& sFileName);
	~CCompoundFile();

	const CDirectoryEntry* GetEntry(const HWP_STRING& sFileName) const;
	bool GetComponent(const HWP_STRING& sEntryName, CHWPStream& oBuffer);

	VECTOR<CDirectoryEntry*> GetChildEntries(const CDirectoryEntry* pBaseEntry) const;
	VECTOR<CDirectoryEntry*> GetChildEntries(const HWP_STRING& sBaseEntryName) const;

	bool Read(const CDirectoryEntry& oEntry, CHWPStream& oBuffer);
	bool Open();
	void Close();
private:
	void AddSiblings(VECTOR<int>& arIndexs, int nCurrentIndex) const;

	VECTOR<int> GetSecIDsFromSAT(int nSecID, int nSatIndex, int nSecIDSSAT);

	void ReadDirectorySector(int nSecID);
	void ReadSSATSector(int nSecID);
	void ReadMSATSector(int nSecID);

	void ParseSectors(int nSecID, CHWPStream& oBuffer);
	void ParseSATSector(int nSecID, CHWPStream& oBuffer);
	void ParseDirectorySector(CHWPStream& oBuffer);
	void ParseSSATSector(CHWPStream& oBuffer);
	void ParseMSATSector(CHWPStream& oBuffer);
	bool ParseHeader(CHWPStream& oBuffer);

	bool CheckSignature(CHWPStream& oBuffer);

	ESectorType LookupSectorType(CHWPStream& oBuffer);
};
}

#endif // COMPOUNDFILE_H
