#ifndef HWPSTREAM_H
#define HWPSTREAM_H

#include <stack>
#include <string>
#include "Common.h"

namespace HWP
{
enum class EStringCharacter
{
	ASCII,
	UTF16,
	UTF32
};

class CHWPStream
{
	BYTE* m_pBegin;
	BYTE* m_pCur;
	BYTE* m_pEnd;
	std::stack<BYTE*> m_arSavedPositions;

	bool m_bExternalBuffer;
public:
	CHWPStream();
	CHWPStream(unsigned int unSize);
	CHWPStream(BYTE* pBuffer, unsigned int unSize, bool bExternalBuffer = true);
	~CHWPStream();

	void SetStream(BYTE* pBuffer, unsigned int unSize, bool bExternalBuffer = true);

	BYTE* GetCurPtr();
	unsigned int Tell() const;

	bool ReadChar(CHAR& chValue);
	bool ReadFloat(float& fValue);
	bool ReadDouble(double& dValue);
	bool ReadLong(long long& lValue);
	bool ReadInt(int& nValue);
	bool ReadColor(int& nValue);
	bool ReadShort(short& shValue);
	short ReadShort();
	bool ReadByte(BYTE& chValue);
	BYTE ReadByte();
	bool ReadString(STRING& sValue, EStringCharacter eCharacter);
	bool ReadString(STRING& sValue, int nLength, EStringCharacter eCharacter);
	bool ReadBytes(BYTE* pBytes, unsigned int unSize);

	void Skip(unsigned int unStep);
	void MoveToStart();
	void MoveTo(unsigned int unPosition);

	bool CanRead(int nSize = 1) const;
	bool IsValid() const;
	bool IsEof() const;
	unsigned int GetSize() const;

	void SavePosition();
	void RemoveLastSavedPos();
	int GetDistanceToLastPos(bool bRemoveLastPos = false);

	BYTE operator[](unsigned int unPosition) const;
};

#define CHECK_FLAG(value, flag) ((value) & flag) == flag
}

#endif // HWPSTREAM_H
