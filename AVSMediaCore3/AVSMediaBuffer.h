#pragma once

class CMediaBuffer
{
protected:
	
	__declspec(align(32)) LONG m_dwRef;

	BYTE* m_pMediaBuffer;
	LONG m_lFrameSize;
	LONG m_lBufferSize;
public:
	
	CMediaBuffer()
	{
		m_dwRef = 1;
		m_pMediaBuffer = NULL;
		m_lFrameSize = 0;
		m_lBufferSize = 0;
	}
	~CMediaBuffer(void)
	{
		RELEASEHEAP(m_pMediaBuffer);
	}
	
	LONG __fastcall AddRef()
	{
		return InterlockedIncrement(&m_dwRef);
	}
	LONG __fastcall Release()
	{
		long lResult = InterlockedDecrement(&m_dwRef);
		if (lResult == 0)
			delete this;

		return lResult;
	}
	BOOL __fastcall SetBuffer(LONG lSize, BOOL bAlign64 = TRUE)
	{
		RELEASEHEAP(m_pMediaBuffer);

		m_lFrameSize = lSize;

		if (bAlign64)
			m_lBufferSize = ((lSize + 63) & 0xFFFFFFC0) + 64;
		else
			m_lBufferSize = lSize;

		if (m_lBufferSize == 0)
			return TRUE;

		//ATLTRACE("GetProcessHeap(): 0x%08X\n",GetProcessHeap());
		m_pMediaBuffer = (BYTE*)::HeapAlloc(GetProcessHeap(), NULL, m_lBufferSize);

		if (!m_pMediaBuffer)
			return FALSE;

		LONG lAlignSize = (m_lBufferSize - m_lFrameSize);

		if (lAlignSize > 0)
		{
			memset(m_pMediaBuffer + m_lFrameSize, 0xFF, lAlignSize);
		}

		return TRUE;
	}
	
	BYTE* GetBuffer()
	{
		return m_pMediaBuffer;
	}
	LONG GetBufferSize()
	{
		return m_lFrameSize;
	}
};
