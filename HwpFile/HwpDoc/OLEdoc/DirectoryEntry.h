#ifndef DIRECTORYENTRY_H
#define DIRECTORYENTRY_H

#include "../Common/Common.h"

namespace HWP
{
class CDirectoryEntry
{
	HWP_STRING m_sDirectoryEntryName;
	int m_nObjectType;
	int m_nColorFlag;
	int m_nLeftSiblingID;
	int m_nRightSiblingID;
	int m_nChildID;
	long long m_lClsID1;
	long long m_lClsID2;
	int m_nStateBit;
	long long m_lCreationTime;
	long long m_lModifiedTime;
	int m_nStartingSectorID;
	long long m_lStreamSize;
	VECTOR<int> m_arSecNums;
public:
	CDirectoryEntry();

	void SetDirectoryEntryName(const HWP_STRING& sDirectoryEntryName);
	void SetObjectType(int nObjectType);
	void SetColorFlag(int nColorFlag);
	void SetLeftSiblingID(int nLeftSiblingID);
	void SetRightSiblingID(int nRightSiblingID);
	void SetChildID(int nChildID);
	void SetClsID1(const long long& lClsID1);
	void SetClsID2(const long long& lClsID2);
	void SetStateBit(int nStateBit);
	void SetCreationTime(const long long& lCreationTime);
	void SetModifiedTime(const long long& lModifiedTime);
	void SetStartingSectorID(int nStartingSectorID);
	void SetStreamSize(const long long& lStreamSize);

	int GetLeftSiblingID() const;
	int GetRightSiblingID() const;
	int GetChildID() const;
	int GetStartingSectorID() const;
	long long GetStreamSize() const;
	VECTOR<int> GetSecNums() const;

	VECTOR<int>& GetSecNums();

	int GetObjectType() const;
	HWP_STRING GetDirectoryEntryName() const;
};
}

#endif // DIRECTORYENTRY_H
