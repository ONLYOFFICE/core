#ifndef HWPSTREAM_H
#define HWPSTREAM_H

#include <stack>
#include "Common/Common.h"

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
	HWP_BYTE* m_pBegin;
	HWP_BYTE* m_pCur;
	HWP_BYTE* m_pEnd;
	std::stack<HWP_BYTE*> m_arSavedPositions;

	bool m_bExternalBuffer;
public:
	CHWPStream();
	CHWPStream(unsigned long ulSize);
	CHWPStream(HWP_BYTE* pBuffer, unsigned long ulSize, bool bExternalBuffer = true);
	~CHWPStream();

	void Clear();
	void Copy(CHWPStream& oStream, unsigned long ulSize);

	void SetStream(HWP_BYTE* pBuffer, unsigned long ulSize, bool bExternalBuffer = true);

	HWP_BYTE* GetCurPtr();
	const HWP_BYTE* GetCurPtr() const;
	unsigned long Tell() const;
	unsigned long SizeToEnd() const;

	void Expand(unsigned long ulSize);

	bool ReadChar(HWP_CHAR& chValue);
	bool ReadFloat(float& fValue);
	bool ReadDouble(double& dValue);
	bool ReadLong(long long& lValue);
	bool ReadLong(long& lValue);
	bool ReadInt(int& nValue);
	bool ReadColor(int& nValue);
	bool ReadShort(short& shValue);
	short ReadShort();
	bool ReadByte(HWP_BYTE& chValue);
	HWP_BYTE ReadByte();
	bool ReadBool(bool& bValue);
	bool ReadString(HWP_STRING& sValue, EStringCharacter eCharacter);
	bool ReadString(HWP_STRING& sValue, int nLength, EStringCharacter eCharacter);
	unsigned long ReadBytes(HWP_BYTE* pBytes, unsigned long unSize);

	void Skip(int nStep);
	void MoveToStart();
	void MoveTo(unsigned int unPosition);

	bool CanRead(int nSize = 1) const;
	bool IsValid() const;
	bool IsEof() const;
	unsigned int GetSize() const;

	void SavePosition();
	void RemoveLastSavedPos();
	int GetDistanceToLastPos(bool bRemoveLastPos = false);

	HWP_BYTE operator[](unsigned int unPosition) const;

	bool WriteBytes(const HWP_BYTE* pBuffer, unsigned long ulSize);
};

#define CHECK_FLAG(value, flag) ((value) & flag) == flag
#define DEFAULT_STRING_CHARACTER EStringCharacter::UTF16
}

#endif // HWPSTREAM_H
