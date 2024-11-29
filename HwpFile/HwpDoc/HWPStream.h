#ifndef HWPSTREAM_H
#define HWPSTREAM_H

namespace HWP
{
typedef char BYTE;

class CHWPStream
{
	BYTE* m_pBegin;
	BYTE* m_pCur;
	BYTE* m_pEnd;
public:
	CHWPStream();
	CHWPStream(BYTE* pBuffer, unsigned int unSize);

	void SetStream(BYTE* pBuffer, unsigned int unSize);

	BYTE* GetCurPtr();

	bool ReadInt(int& nValue);
	bool ReadColor(int& nValue);
	bool ReadShort(short& shValue);
	bool ReadByte(BYTE& chValue);

	void Skip(unsigned int unStep);

	bool IsValid() const;
	bool IsEof() const;
	unsigned int GetLength() const;

	BYTE operator[](unsigned int unPosition) const;
};

#define CHECK_FLAG(value, flag) (value & flag) == flag
}

#endif // HWPSTREAM_H
