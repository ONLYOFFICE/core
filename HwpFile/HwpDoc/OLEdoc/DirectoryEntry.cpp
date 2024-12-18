#include "DirectoryEntry.h"

namespace HWP
{
HWP::CDirectoryEntry::CDirectoryEntry()
{}

void CDirectoryEntry::SetDirectoryEntryName(const STRING& sDirectoryEntryName)
{
	m_sDirectoryEntryName = sDirectoryEntryName;
}

void CDirectoryEntry::SetObjectType(int nObjectType)
{
	m_nObjectType = nObjectType;
}

void CDirectoryEntry::SetColorFlag(int nColorFlag)
{
	m_nColorFlag = nColorFlag;
}

void CDirectoryEntry::SetLeftSiblingID(int nLeftSiblingID)
{
	m_nLeftSiblingID = nLeftSiblingID;
}

void CDirectoryEntry::SetRightSiblingID(int nRightSiblingID)
{
	m_nRightSiblingID = nRightSiblingID;
}

void CDirectoryEntry::SetChildID(int nChildID)
{
	m_nChildID = nChildID;
}

void CDirectoryEntry::SetClsID1(const long long& lClsID1)
{
	m_lClsID1 = lClsID1;
}

void CDirectoryEntry::SetClsID2(const long long& lClsID2)
{
	m_lClsID2 = lClsID2;
}

void CDirectoryEntry::SetStateBit(int nStateBit)
{
	m_nStateBit = nStateBit;
}

void CDirectoryEntry::SetCreationTime(const long long& lCreationTime)
{
	m_lCreationTime = lCreationTime;
}

void CDirectoryEntry::SetModifiedTime(const long long& lModifiedTime)
{
	m_lModifiedTime = lModifiedTime;
}

void CDirectoryEntry::SetStartingSectorID(int nStartingSectorID)
{
	m_nStartingSectorID = nStartingSectorID;
}

void CDirectoryEntry::SetStreamSize(const long long& lStreamSize)
{
	m_lStreamSize = lStreamSize;
}

STRING CDirectoryEntry::GetDirectoryEntryName() const
{
	return m_sDirectoryEntryName;
}

int CDirectoryEntry::GetLeftSiblingID() const
{
	return m_nLeftSiblingID;
}

int CDirectoryEntry::GetRightSiblingID() const
{
	return m_nRightSiblingID;
}

int CDirectoryEntry::GetChildID() const
{
	return m_nChildID;
}

int CDirectoryEntry::GetStartingSectorID() const
{
	return m_nStartingSectorID;
}

long long CDirectoryEntry::GetStreamSize() const
{
	return m_lStreamSize;
}

VECTOR<int> CDirectoryEntry::GetSecNums() const
{
	return m_arSecNums;
}

VECTOR<int>& CDirectoryEntry::GetSecNums()
{
	return m_arSecNums;
}

int CDirectoryEntry::GetObjectType() const
{
	return m_nObjectType;
}
}
