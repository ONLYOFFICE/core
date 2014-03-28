#ifndef _BUILD_BGRA_FRAME_
#define _BUILD_BGRA_FRAME_

#include <string>
#include "../common/Types.h"
#include "../cximage/CxImage/ximage.h"

class CBgraFrame
{
private:
	int m_lWidth;
	int m_lHeight;
	int m_lStride;

	BYTE* m_pData;

public:
	CBgraFrame()
	{
		Clear();
	}
	~CBgraFrame()
	{
		Destroy();
	}

private:
	inline void Destroy()
	{
		if (NULL != m_pData)
			delete []m_pData;

		Clear();
	}
	inline void Clear()
	{
		m_lWidth	= 0;
		m_lHeight	= 0;
		m_lStride	= 0;
		m_pData		= NULL;
	}

public:
	inline void ClearNoAttack()
	{
		Clear();
	}
	inline int get_Width()
	{
		return m_lWidth;
	}
	inline int get_Height()
	{
		return m_lHeight;
	}
	inline void put_Width(const int& lWidth)
	{
		m_lWidth = lWidth;
	}
	inline void put_Height(const int& lHeight)
	{
		m_lHeight = lHeight;
	}
	inline int get_Stride()
	{
		return m_lStride;
	}
	inline void put_Stride(const int& lStride)
	{
		m_lStride = lStride;
	}
	inline BYTE* get_Data()
	{
		return m_pData;
	}
	inline void put_Data(BYTE* pData)
	{
		m_pData = pData;
	}

public:
	bool OpenFile(const std::wstring& strFileName);
	bool SaveFile(const std::wstring& strFileName, uint32_t nFileType);

private:

	void CxImageToMediaFrame( CxImage& img );
};

#endif
