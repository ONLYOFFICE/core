#pragma once

namespace NSOfficeDrawing
{
	#define BINARY_READER_CHECK_OUT_RANGE(index, count)     \
	if (index > count)										\
	{														\
		index = count;										\
		return 0;											\
	}

	class CBinaryReader
	{
	private:
		BYTE* m_pBuffer;
		DWORD m_lCount;

		DWORD m_lOrigin;

	public:
		CBinaryReader(BYTE* pBuffer, DWORD lCount)
		{
			m_pBuffer = pBuffer;
			m_lCount = lCount;

			m_lOrigin = 0;
		}
		~CBinaryReader()
		{
			m_pBuffer = NULL;
			m_lCount = 0;
		}

	public:
		LONG ReadLONG()
		{
			DWORD lOldOrigin = m_lOrigin;
			m_lOrigin += 4;

			BINARY_READER_CHECK_OUT_RANGE(m_lOrigin, m_lCount)

			return *(LONG*)(m_pBuffer + lOldOrigin);
		}
		DWORD ReadDWORD()
		{
			DWORD lOldOrigin = m_lOrigin;
			m_lOrigin += 4;

			BINARY_READER_CHECK_OUT_RANGE(m_lOrigin, m_lCount)

			return *(DWORD*)(m_pBuffer + lOldOrigin);
		}
		WORD ReadWORD()
		{
			DWORD lOldOrigin = m_lOrigin;
			m_lOrigin += 2;

			BINARY_READER_CHECK_OUT_RANGE(m_lOrigin, m_lCount)

			return *(WORD*)(m_pBuffer + lOldOrigin);
		}
		SHORT ReadSHORT()
		{
			DWORD lOldOrigin = m_lOrigin;
			m_lOrigin += 2;

			BINARY_READER_CHECK_OUT_RANGE(m_lOrigin, m_lCount)

			return *(SHORT*)(m_pBuffer + lOldOrigin);
		}
		double ReadDOUBLE()
		{
			DWORD lOldOrigin = m_lOrigin;
			m_lOrigin += sizeof(double);

			BINARY_READER_CHECK_OUT_RANGE(m_lOrigin, m_lCount)

			return *(SHORT*)(m_pBuffer + lOldOrigin);
		}
	};
}
