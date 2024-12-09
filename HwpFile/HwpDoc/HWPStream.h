#ifndef HWPSTREAM_H
#define HWPSTREAM_H

#include <string>
#include "Common.h"

namespace HWP
{
class CHWPStream
{
	BYTE* m_pBegin;
	BYTE* m_pCur;
	BYTE* m_pEnd;
	BYTE* m_pSavedPosition;
public:
	CHWPStream();
	CHWPStream(BYTE* pBuffer, unsigned int unSize);

	void SetStream(BYTE* pBuffer, unsigned int unSize);

	BYTE* GetCurPtr();

	bool ReadChar(CHAR& chValue);
	bool ReadFloat(float& fValue);
	bool ReadDouble(double& dValue);
	bool ReadInt(int& nValue);
	bool ReadColor(int& nValue);
	bool ReadShort(short& shValue);
	short ReadShort();
	bool ReadByte(BYTE& chValue);
	BYTE ReadByte();
	bool ReadString(STRING& sValue);
	bool ReadString(STRING& sValue, int nLength);

	void Skip(unsigned int unStep);

	bool CanRead(int nSize) const;
	bool IsValid() const;
	bool IsEof() const;
	unsigned int GetLength() const;

	void SavePosition();
	int GetDistanceToLastPos();

	BYTE operator[](unsigned int unPosition) const;
};

#define CHECK_FLAG(value, flag) ((value) & flag) == flag
}

#endif // HWPSTREAM_H
