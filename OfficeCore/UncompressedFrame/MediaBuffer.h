#pragma once

#define ADDREFINTERFACE(pinterface)\
{\
	if (pinterface!=NULL)\
	{\
		pinterface->AddRef();\
	}\
}
#define RELEASEINTERFACE(pinterface)\
{\
	if (pinterface!=NULL)\
	{\
		pinterface->Release();\
		pinterface=NULL;\
	}\
}
#define QUERYINTERFACE(pinterface, pinterface_res, iid)\
{\
	if (pinterface!=NULL)\
		pinterface->QueryInterface(iid, (void**)&pinterface_res);\
	else\
		pinterface_res=NULL;\
}
#define RELEASEMEM(pobject)\
{\
	if (pobject!=NULL)\
	{\
		free(pobject);\
		pobject=NULL;\
	}\
}
#define RELEASEOBJECT(pobject)\
{\
	if (pobject!=NULL)\
	{\
		delete pobject;\
		pobject=NULL;\
	}\
}
#define RELEASEARRAYOBJECTS(pobject)\
{\
	if (pobject!=NULL)\
	{\
		delete []pobject;\
		pobject=NULL;\
	}\
}
#define RELEASEHEAP(pmem)\
{\
	if (pmem!=NULL)\
	{\
		HeapFree(GetProcessHeap(), 0, pmem);\
		pmem=NULL;\
	}\
}
#define RELEASEARRAY(parray)\
{\
	if (parray!=NULL)\
	{\
		SafeArrayDestroy(parray);\
		parray=NULL;\
	}\
}
#define RELEASESYSSTRING(pstring)\
{\
	if (pstring!=NULL)\
	{\
		SysFreeString(pstring);\
		pstring=NULL;\
	}\
}
#define RELEASEHANDLE(phandle)\
{\
	if (phandle!=NULL)\
	{\
		CloseHandle(phandle);\
		phandle=NULL;\
	}\
}

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

//Color spaces
#define CSP_PLANAR   (1<< 0) /* 4:2:0 planar (==I420, except for pointers/strides) */
#define CSP_USER	  CSP_PLANAR
#define CSP_I420     (1<< 1) /* 4:2:0 planar */
#define CSP_YV12     (1<< 2) /* 4:2:0 planar */
#define CSP_YUY2     (1<< 3) /* 4:2:2 packed */
#define CSP_UYVY     (1<< 4) /* 4:2:2 packed */
#define CSP_YVYU     (1<< 5) /* 4:2:2 packed */
#define CSP_BGRA     (1<< 6) /* 32-bit bgra packed */
#define CSP_ABGR     (1<< 7) /* 32-bit abgr packed */
#define CSP_RGBA     (1<< 8) /* 32-bit rgba packed */
#define CSP_ARGB     (1<<15) /* 32-bit argb packed */
#define CSP_BGR      (1<< 9) /* 24-bit bgr packed */
#define CSP_RGB555   (1<<10) /* 16-bit rgb555 packed */
#define CSP_RGB565   (1<<11) /* 16-bit rgb565 packed */
#define CSP_SLICE    (1<<12) /* decoder only: 4:2:0 planar, per slice rendering */
#define CSP_INTERNAL (1<<13) /* decoder only: 4:2:0 planar, returns ptrs to internal buffers */
#define CSP_NULL     (1<<14) /* decoder only: dont output anything */
#define CSP_I422		 (1<<16) /* 4:2:2 planar. Add for MPEG2 Video*/
#define CSP_I444		 (1<<17) /* 4:4:4 planar. Add for MPEG2 Video*/
#define CSP_RGB8     (1<<18) /* decoder only: 8-bit rgb packed */
#define CSP_VFLIP    (1<<31) /* vertical flip mask */

#define CSP_COLOR_MASK	0x7FFFFFFF

#define PLANE_COUNT			4

struct SUncompressedVideoFrame
{
	BYTE* Plane[PLANE_COUNT];
	LONG Stride[PLANE_COUNT];

	LONG Width;
	LONG Height;

	LONG ColorSpace;
	LONG AspectX;
	LONG AspectY;
	BOOL Interlaced;

	double kx;
	double ky;

	SUncompressedVideoFrame()
		: Width(320)
		, Height(240)
		, ColorSpace(CSP_BGRA|CSP_VFLIP)
		, AspectX(0)
		, AspectY(1)
		, Interlaced(false)
		, kx(1.0)
		, ky(1.0)
	{
		for (int i=0;i<PLANE_COUNT;i++)
		{
            Plane[i] = NULL;
			Stride[i] = 0;
		}
	}
	SUncompressedVideoFrame& operator= (const SUncompressedVideoFrame& x)
	{
		for (int i=0;i<PLANE_COUNT;i++)
		{
            Plane[i] = x.Plane[i];
			Stride[i] = x.Stride[i];
		}

		Width = x.Width;
		Height = x.Height;

		ColorSpace = x.ColorSpace;
		AspectX = x.AspectX;
		AspectY = x.AspectY;
		Interlaced = x.Interlaced;

		kx = x.kx;
		ky = x.ky;

		return *this;
	}
};