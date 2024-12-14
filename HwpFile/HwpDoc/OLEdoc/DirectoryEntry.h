#ifndef DIRECTORYENTRY_H
#define DIRECTORYENTRY_H

#include "../Common.h"

namespace HWP
{
class CDirectoryEntry
{
	STRING m_sDirectoryEntryName;
	int m_nObjectType;
	int m_nColorFlag;
	int m_nLeftSiblingID;
	int m_nRightSiblingID;
	int m_nChildID;
	long m_lClsID1;
	long m_lClsID2;
	int m_nStateBit;
	long m_lCreationTime;
	long m_lModifiedTime;
	int m_nStartingSectorID;
	long m_lStreamSize;
	VECTOR<int> m_arSecNums;
public:
	CDirectoryEntry();

	void SetDirectoryEntryName(const STRING& sDirectoryEntryName);
	void SetObjectType(int nObjectType);
	void SetColorFlag(int nColorFlag);
	void SetLeftSiblingID(int nLeftSiblingID);
	void SetRightSiblingID(int nRightSiblingID);
	void SetChildID(int nChildID);
	void SetClsID1(const long& lClsID1);
	void SetClsID2(const long& lClsID2);
	void SetStateBit(int nStateBit);
	void SetCreationTime(const long& lCreationTime);
	void SetModifiedTime(const long& lModifiedTime);
	void SetStartingSectorID(int nStartingSectorID);
	void SetStreamSize(const long& lStreamSize);

	int GetLeftSiblingID() const;
	int GetRightSiblingID() const;
	int GetChildID() const;
	int GetStartingSectorID() const;
	long GetStreamSize() const;
	VECTOR<int> GetSecNums() const;

	VECTOR<int>& GetSecNums();

	int GetObjectType() const;
	STRING GetDirectoryEntryName() const;
};
}

#endif // DIRECTORYENTRY_H
