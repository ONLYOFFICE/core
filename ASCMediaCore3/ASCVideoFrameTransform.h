// AVSVideoFrameTransform.h : Declaration of the CAVSVideoFrameTransform

#pragma once
#include "resource.h"       // main symbols
#include "ASCUncompressedVideoFrame.h"
#include "./VideoFrameTransformLib/BaseTransform.h"


///////////////////////////////////////////////////////////////////////////////////
// IAVSVideoFrameTransform
[object, uuid("DEA8B89A-6350-49E0-890A-84377006633C"), dual, pointer_default(unique)]
__interface IAVSVideoFrameTransform : IDispatch
{
	[id(204000 + 1)] HRESULT SetVideoFormat([in]IUnknown* pVideoFormat);

	//см. "MediaDataDefine.h" константы с префиксом RESIZE_MODE_
	[id(204000 + 2), propget] HRESULT ResizeMode([out, retval] long* pVal);
	[id(204000 + 2), propput] HRESULT ResizeMode([in] long newVal);
	[id(204000 + 3), propget] HRESULT DeinterlaceEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(204000 + 3), propput] HRESULT DeinterlaceEnabled([in] VARIANT_BOOL newVal);
	//см. "MediaDataDefine.h" константы с префиксом DEINTERLACE_METHOD
	[id(204000 + 4), propget] HRESULT DeinterlaceMethod([out, retval] long* pVal);
	[id(204000 + 4), propput] HRESULT DeinterlaceMethod([in] long newVal);
	[id(204000 + 5), propget] HRESULT CropScale([out, retval] long* pVal);
	[id(204000 + 5), propput] HRESULT CropScale([in] long newVal);
	[id(204000 + 6), propget] HRESULT DstBorderGrayScale([out, retval] ULONG* pVal);
	[id(204000 + 6), propput] HRESULT DstBorderGrayScale([in] ULONG newVal);

	[id(204000 + 7)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(204000 + 8)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out,retval] VARIANT* ParamValue);
	[id(204000 + 9)] HRESULT TransformFrame([in] IUnknown* pInpFrame, [out,retval] IUnknown** pOutFrame);
	[id(204000 + 10)] HRESULT ReleaseBuffers(void);

};
// CAVSVideoFrameTransform
[coclass, uuid("E093D454-88E3-44B2-84DC-F4270861298D"), threading(apartment), vi_progid("AVSMediaCore3.AVSVideoFrameTransform"), progid("AVSMediaCore3.AVSVideoFrameTransform.1"), version(1.0)]
class ATL_NO_VTABLE CAVSVideoFrameTransform 
	: public IAVSVideoFrameTransform
	, public CBaseTransform
{
public:
	CAVSVideoFrameTransform()
	{
		ReleaseBuffers();
		//ATLTRACE("CAVSVideoFrameTransform::Constructor 0x%08X\n", this);

		GetDeinterlaceMethodeFromRegistry();

	}
	~CAVSVideoFrameTransform()
	{
		ReleaseBuffers();
		//ATLTRACE("CAVSVideoFrameTransform::Destructor 0x%08X\n", this);
	}
	STDMETHOD(SetVideoFormat)(IUnknown* pVideoFormat)
	{
		if (NULL==pVideoFormat)
			return S_OK;

		MediaFormat::IAVSVideoFormat *pFormat = NULL;
		pVideoFormat->QueryInterface(MediaFormat::IID_IAVSVideoFormat, (void**)&pFormat);
		if (NULL==pFormat)
			return S_OK;

		long lWidth, lHeight, lColorSpace, lAspectRatioX, lAspectRatioY;
		pFormat->get_Width(&lWidth);
		pFormat->get_Height(&lHeight);
		pFormat->get_ColorSpace(&lColorSpace);
		pFormat->get_AspectRatioX(&lAspectRatioX);
		pFormat->get_AspectRatioY(&lAspectRatioY);
		pFormat->Release();
		ATLTRACE("Format: Width: %d, Height: %d, ColorSpace: 0x%08X\n", lWidth, lHeight, lColorSpace);
		memset(&m_oDstFormat, 0, sizeof(SUncompressedVideoFrame));
		m_oDstFormat.Width = lWidth;
		m_oDstFormat.Height = lHeight;
		m_oDstFormat.ColorSpace = lColorSpace;
		m_oDstFormat.AspectX = lAspectRatioX;
		m_oDstFormat.AspectY = lAspectRatioY;
		m_oDstFormat.Interlaced = FALSE;
		m_oDstFormat.kx = 1.0;
		m_oDstFormat.ky = 1.0;
		SetDefaultStride(&m_oDstFormat);
		
		m_bIsNeedAplly = TRUE;
		return S_OK;
	}

	STDMETHOD(get_ResizeMode)(long* pVal)
	{
		*pVal = m_lResizeMode;
		return S_OK;
	}
	STDMETHOD(put_ResizeMode)(long newVal)
	{
		if ((newVal<0)||(newVal>=RESIZE_MODE_COUNT))
			return S_OK;
		m_lResizeMode = newVal;
		return S_OK;
	}
	STDMETHOD(get_DeinterlaceEnabled)(VARIANT_BOOL* pVal)
	{
		*pVal = m_bDeinterlaceEnabled ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_DeinterlaceEnabled)(VARIANT_BOOL newVal)
	{
		m_bDeinterlaceEnabled = (VARIANT_FALSE!=newVal);
		m_bIsNeedAplly = TRUE;
		return S_OK;
	}
	STDMETHOD(get_DeinterlaceMethod)(long* pVal)
	{
		*pVal = m_lDeinterlaceMethod;
		return S_OK;
	}
	STDMETHOD(put_DeinterlaceMethod)(long newVal)
	{
		if ((newVal<0)||(newVal>=DEINTERLACE_METHOD_COUNT))
			return S_OK;
		m_lDeinterlaceMethod = newVal;
		return S_OK;
	}
	STDMETHOD(get_CropScale)(long* pVal)
	{
		*pVal = m_lCropScale;
		return S_OK;
	}
	STDMETHOD(put_CropScale)(long newVal)
	{
		if ((newVal<0)||(newVal>100))
			return S_OK;
		m_bIsNeedAplly = true;
		m_lCropScale = newVal;
		return S_OK;
	}
	STDMETHOD(get_DstBorderGrayScale)(ULONG* pVal)
	{
		//*pVal = m_nRGBDstBorderGrayScale;
		*pVal = m_dwBGRDstBorderColor;
		return S_OK;
	}
	STDMETHOD(put_DstBorderGrayScale)(ULONG newVal)
	{
		//m_nRGBDstBorderGrayScale = (BYTE)newVal;
		//m_nYDstBorderGrayScale = m_nRGBDstBorderGrayScale;
		//m_nUDstBorderGrayScale = 0x80;
		//m_nVDstBorderGrayScale = 0x80;

		m_dwBGRDstBorderColor	= newVal;
		BYTE nRColor = (BYTE)(0xFF & m_dwBGRDstBorderColor);
		BYTE nGColor = (BYTE)(0xFF & (m_dwBGRDstBorderColor >> 8));
		BYTE nBColor = (BYTE)(0xFF & (m_dwBGRDstBorderColor >> 16));
		
		m_nYDstBorderColor		= 0.299*nRColor + 0.587*nGColor + 0.114*nBColor;
		m_nUDstBorderColor		= 0.565*(nBColor - m_nYDstBorderColor) + 0x80;
		m_nVDstBorderColor		= 0.713*(nRColor - m_nYDstBorderColor) + 0x80;
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName = ParamName;

		if (sParamName == _T("RotateAngle"))
		{
			//0,90,180,270 - на остальные пока забили
			m_lRotateAngle = ParamValue.lVal;
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

	STDMETHOD(TransformFrame)(IUnknown* pInpFrame, IUnknown** pOutFrame)
	{
		//ATLTRACE("CAVSVideoFrameTransform::TransformFrame begin 0x%08X\n", this);

		if ((NULL == pInpFrame)||(NULL == pOutFrame))
			return INVALIDOPERATION;

		IAVSUncompressedVideoFrame *piInpFrame = NULL;
		pInpFrame->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void **)&piInpFrame);
		if (NULL==piInpFrame)
			return INVALIDOPERATION;

		CAVSUncompressedVideoFrame*	pInpVideoFrame=(CAVSUncompressedVideoFrame*)piInpFrame;
		if (IsChangeArrayFormat(pInpVideoFrame))
		{
			m_oSrcFormat =  pInpVideoFrame->m_oVideoFrame;
			m_bIsNeedAplly=true;
		}
		if (m_bIsNeedAplly)
			ApplySettings();

		if (m_bIsNeedTransform)
		{
			//YV12 и I420 отличаются только порядком U и V плоскостей
			//YV12 - Y V U
			//I420 - Y U V
			//Используем для них ОДНО И ТОЖЕ ПРЕОБРАЗОВАНИЕ (работающие с Y U V порядком плоскостей),
			//для YV12 меняем местами ссылки на U и V плоскости в m_oSrcFormat или m_oDstFormat
			m_oSrcFormat.Plane[0] = pInpVideoFrame->m_oVideoFrame.Plane[0];
			m_oSrcFormat.Plane[1] = pInpVideoFrame->m_oVideoFrame.Plane[1];
			m_oSrcFormat.Plane[2] = pInpVideoFrame->m_oVideoFrame.Plane[2];
			m_oSrcFormat.Plane[3] = pInpVideoFrame->m_oVideoFrame.Plane[3];
			if (CSP_YV12 == (m_oSrcFormat.ColorSpace & CSP_COLOR_MASK))
			{
				m_oSrcFormat.Plane[1] = pInpVideoFrame->m_oVideoFrame.Plane[2];
				m_oSrcFormat.Plane[2] = pInpVideoFrame->m_oVideoFrame.Plane[1];
			}
			IAVSUncompressedVideoFrame*	piOutVideoFrame=NULL;
			CoCreateInstance(__uuidof(CAVSUncompressedVideoFrame), NULL, CLSCTX_ALL, __uuidof(IAVSUncompressedVideoFrame), (void**)&piOutVideoFrame);
			if (NULL==piOutVideoFrame)
				return MEMORY;

			VARIANT_BOOL vbTemp;
			piInpFrame->get_IsDiscontinuity(&vbTemp);
			piOutVideoFrame->put_IsDiscontinuity(vbTemp);
			piInpFrame->get_IsTimeStampExist(&vbTemp);
			if (VARIANT_FALSE!=vbTemp)
			{
				double dTimeStamp;
				piInpFrame->get_TimeStamp(&dTimeStamp);
				piOutVideoFrame->put_TimeStamp(dTimeStamp);
			}
			piInpFrame->get_IsDurationExist(&vbTemp);
			if (VARIANT_FALSE!=vbTemp)
			{
				double	dDuration;
				piInpFrame->get_Duration(&dDuration);
				piOutVideoFrame->put_Duration(dDuration);
			}

			CAVSUncompressedVideoFrame*	pcOutVideoFrame=(CAVSUncompressedVideoFrame*)piOutVideoFrame;
			pcOutVideoFrame->m_oVideoFrame =  m_oDstFormat;
			if ((0==m_oDstFormat.AspectX)||(0==m_oDstFormat.AspectY))
			{
				pcOutVideoFrame->m_oVideoFrame.AspectX = m_oSrcFormat.AspectX;
				pcOutVideoFrame->m_oVideoFrame.AspectY = m_oSrcFormat.AspectY;
			}
			pcOutVideoFrame->AllocateBuffer(-1);

			m_oDstFormat.Plane[0] = pcOutVideoFrame->m_oVideoFrame.Plane[0];
			m_oDstFormat.Plane[1] = pcOutVideoFrame->m_oVideoFrame.Plane[1];
			m_oDstFormat.Plane[2] = pcOutVideoFrame->m_oVideoFrame.Plane[2];
			m_oDstFormat.Plane[3] = pcOutVideoFrame->m_oVideoFrame.Plane[3];
			if (CSP_YV12 == (m_oDstFormat.ColorSpace & CSP_COLOR_MASK))
			{
				m_oDstFormat.Plane[1] = pcOutVideoFrame->m_oVideoFrame.Plane[2];
				m_oDstFormat.Plane[2] = pcOutVideoFrame->m_oVideoFrame.Plane[1];
			}

			switch(m_nTransformType)
			{
			case ttErrorTransform:
				ErrorTransform();
				break;
			case ttSimpleTransform:
				SimpleTransform();
				break;
			case ttResizeTransform:
				ResizeTransform();
				break;
			case ttDeinterlaceTransform:
				DeinterlaceTransform();
				break;
			case ttResizeDeinterlaceTransform:
				ResizeDeinterlaceTransform();
				break;				
			}

			if ((CSP_BGRA != (m_oSrcFormat.ColorSpace & CSP_COLOR_MASK)) &&
				(CSP_BGRA == (m_oDstFormat.ColorSpace & CSP_COLOR_MASK)))
			{
				long lWidth = m_oDstFormat.Width;
				long lHeight = m_oDstFormat.Height;
				long lstride = m_oDstFormat.Stride[0] - 4*m_oDstFormat.Width;
				LPBYTE ptrBuffer = m_oDstFormat.Plane[0] + 3;
				for (long lRow = 0; lRow < lHeight; lRow++)
				{
					for (long lCol=0; lCol < lWidth; lCol++, ptrBuffer+=4)
					{
						*ptrBuffer = 255;						
					}
					ptrBuffer += lstride;
				}
			}
			piOutVideoFrame->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)pOutFrame);
			piOutVideoFrame->Release();
		}
		else
		{
			piInpFrame->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)pOutFrame);
		}
		if (m_lRotateAngle == 90 || m_lRotateAngle == 180 || m_lRotateAngle == 270)
		{
			IAVSUncompressedVideoFrame* pOutFrameRotate = NULL;
			HRESULT hr = RotateFrame((IAVSUncompressedVideoFrame*)(*pOutFrame),&pOutFrameRotate);
			if (pOutFrameRotate)
			{
				(*pOutFrame)->Release();
				pOutFrameRotate->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)pOutFrame);
				pOutFrameRotate->Release();
			}
		}
		piInpFrame->Release();
		//ATLTRACE("CAVSVideoFrameTransform::TransformFrame end 0x%08X\n", this);
		return S_OK;
	}
	HRESULT  RotateFrame (IAVSUncompressedVideoFrame* pInpFrame, IAVSUncompressedVideoFrame** ppOutFrame)
	{
		if ( !(m_oDstFormat.ColorSpace & CSP_YV12 || m_oDstFormat.ColorSpace & CSP_I420))
		{
			//для остальных колор спйсов пока не реализовано
			pInpFrame->CreateDuplicate(DUBLICATE_TYPE_COPY,(IAVSMediaData**)ppOutFrame);
			return S_OK;
		}
		SUncompressedVideoFrame		m_oDstFormatRotate = m_oDstFormat;
		if (m_lRotateAngle == 90 || m_lRotateAngle == 270)
		{
			//поворот картинки делается ПОСЛЕ всех остальных преобраований
			//он задается только углом поворота, а не форматом			
		
			m_oDstFormatRotate.AspectX = m_oDstFormat.AspectY;
			m_oDstFormatRotate.AspectY = m_oDstFormat.AspectX;

			m_oDstFormatRotate.Width = m_oDstFormat.Height;
			m_oDstFormatRotate.Height = m_oDstFormat.Width;
		}

		*ppOutFrame=NULL;
		CoCreateInstance(__uuidof(CAVSUncompressedVideoFrame), NULL, CLSCTX_ALL, __uuidof(IAVSUncompressedVideoFrame), (void**)ppOutFrame);
		if (NULL==(*ppOutFrame))
			return MEMORY;

		VARIANT_BOOL vbTemp;
		pInpFrame->get_IsDiscontinuity(&vbTemp);
		(*ppOutFrame)->put_IsDiscontinuity(vbTemp);
		pInpFrame->get_IsTimeStampExist(&vbTemp);
		if (VARIANT_FALSE!=vbTemp)
		{
			double dTimeStamp;
			pInpFrame->get_TimeStamp(&dTimeStamp);
			(*ppOutFrame)->put_TimeStamp(dTimeStamp);
		}
		pInpFrame->get_IsDurationExist(&vbTemp);
		if (VARIANT_FALSE!=vbTemp)
		{
			double	dDuration;
			pInpFrame->get_Duration(&dDuration);
			(*ppOutFrame)->put_Duration(dDuration);
		}
		CAVSUncompressedVideoFrame*	pcInpFrame=(CAVSUncompressedVideoFrame*)(pInpFrame);
		CAVSUncompressedVideoFrame*	pcOutFrame=(CAVSUncompressedVideoFrame*)(*ppOutFrame);
		pcOutFrame->m_oVideoFrame =  m_oDstFormatRotate;

		pcOutFrame->AllocateBuffer(-1);

		int tmpColorSpace = m_oDstFormat.ColorSpace & CSP_COLOR_MASK;
		if (CSP_YV12 == tmpColorSpace)
		{
			int res = I420Rotate(
				pcInpFrame->m_oVideoFrame.Plane[0],pcInpFrame->m_oVideoFrame.Stride[0],
				pcInpFrame->m_oVideoFrame.Plane[2],pcInpFrame->m_oVideoFrame.Stride[2],
				pcInpFrame->m_oVideoFrame.Plane[1],pcInpFrame->m_oVideoFrame.Stride[1],

				pcOutFrame->m_oVideoFrame.Plane[0],pcOutFrame->m_oVideoFrame.Stride[0],
				pcOutFrame->m_oVideoFrame.Plane[2],pcOutFrame->m_oVideoFrame.Stride[2],
				pcOutFrame->m_oVideoFrame.Plane[1],pcOutFrame->m_oVideoFrame.Stride[1],

				pcInpFrame->m_oVideoFrame.Width,
				pcInpFrame->m_oVideoFrame.Height,

				m_lRotateAngle);		
		}
		else if (CSP_I420  == tmpColorSpace)
		{
			int res = I420Rotate(
				pcInpFrame->m_oVideoFrame.Plane[0],pcInpFrame->m_oVideoFrame.Stride[0],
				pcInpFrame->m_oVideoFrame.Plane[1],pcInpFrame->m_oVideoFrame.Stride[1],
				pcInpFrame->m_oVideoFrame.Plane[2],pcInpFrame->m_oVideoFrame.Stride[2],

				pcOutFrame->m_oVideoFrame.Plane[0],pcOutFrame->m_oVideoFrame.Stride[0],
				pcOutFrame->m_oVideoFrame.Plane[1],pcOutFrame->m_oVideoFrame.Stride[1],
				pcOutFrame->m_oVideoFrame.Plane[2],pcOutFrame->m_oVideoFrame.Stride[2],

				pcInpFrame->m_oVideoFrame.Width,
				pcInpFrame->m_oVideoFrame.Height,

				m_lRotateAngle);
		}
		else if (CSP_BGRA  == tmpColorSpace || CSP_RGBA  == tmpColorSpace || CSP_ABGR  == tmpColorSpace)
		{
			fastconvert_RGB32CopyMem(&pcInpFrame->m_oVideoFrame, &pcOutFrame->m_oVideoFrame, m_bMirror);
		}
		else if (CSP_BGR  == tmpColorSpace)
		{
			fastconvert_RGB24CopyMem(&pcInpFrame->m_oVideoFrame, &pcOutFrame->m_oVideoFrame, m_bMirror);
		}
		else if (CSP_YUY2 == tmpColorSpace)
		{
			fastconvert_YUY2CopyMem(&pcInpFrame->m_oVideoFrame, &pcOutFrame->m_oVideoFrame, m_bMirror);
		}
		else
		{

		}

		return S_OK;
	}

	STDMETHOD(ReleaseBuffers)(void)
	{
		FreeBuffers();

		memset(&m_oSrcFormat, 0, sizeof(SUncompressedVideoFrame));
		memset(&m_oDstFormat, 0, sizeof(SUncompressedVideoFrame));

		m_lResizeMode = 0;
		m_bDeinterlaceEnabled = TRUE;
		m_bRealNeedDeinterlace = FALSE;
		m_lDeinterlaceMethod = 0;

		m_lCropScale = 0;
		//m_nRGBDstBorderGrayScale = 0x00;
		//m_nYDstBorderGrayScale = 0x00;
		//m_nUDstBorderGrayScale = 0x80;
		//m_nVDstBorderGrayScale = 0x80;

		m_dwBGRDstBorderColor = 0x00;
		m_nYDstBorderColor = 0x00;
		m_nUDstBorderColor = 0x80;
		m_nVDstBorderColor = 0x80;

		m_arlResizeModes[0] = IPPI_INTER_NN;
		m_arlResizeModes[1] = IPPI_INTER_LINEAR;
		m_arlResizeModes[2] = IPPI_INTER_CUBIC;
		m_arlResizeModes[3] = IPPI_INTER_SUPER;

		m_bFillBackground = FALSE;

		m_nTransformType = ttErrorTransform;

		m_bIsNeedTransform=false;
		m_bIsNeedAplly=true;

		m_lInpColorSpace = CSP_I420;
		m_lOutColorSpace = CSP_I420;
		m_bMirror = FALSE;		

		m_lRotateAngle = 0;
		return S_OK;
	}
protected:
	enum TransformType
	{
		ttErrorTransform				= 0,
		ttSimpleTransform				= 1,
		ttResizeTransform				= 2,
		ttDeinterlaceTransform			= 3,
		ttResizeDeinterlaceTransform	= 4,
	};
	int m_nTransformType;

	BOOL __fastcall IsChangeArrayFormat(CAVSUncompressedVideoFrame* pVideoFrame);
	void __fastcall ApplySettings();

	void __fastcall FillBuffer(SUncompressedVideoFrame* pVideoFrame, BYTE* pBuffer);
	void __fastcall AllocBuffer(SUncompressedVideoFrame* pVideoFrame, SUncompressedVideoFrame* pFrame, long lColorSpace);
	void __fastcall FreeBuffer(SUncompressedVideoFrame *pVideoFrame);
	void __fastcall FreeBuffers();
	void __fastcall SetDefaultStride(SUncompressedVideoFrame *pVideoFrame);

	void ResizeDeinterlaceTransform();
	void DeinterlaceTransform();
	void ResizeTransform();
	void SimpleTransform();
	void ErrorTransform();

	void ResizeDeinterlaceAlloc();
	void DeinterlaceAlloc();
	void ResizeAlloc();
	void SimpleAlloc();
	void ErrorAlloc();

	void __fastcall RGB32Resize(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void __fastcall RGB24Resize(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void __fastcall YV12Resize(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
        void __fastcall YUY2Resize(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);

	void __fastcall RGB32ToRGB24(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame, bool Mirror);
	void __fastcall RGB24ToRGB32(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame, bool Mirror);

	void __fastcall RGB8ToRGB24(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame, bool Mirror);
	void __fastcall RGB8ToRGB32(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame, bool Mirror);

	void __fastcall RGB32Deinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void __fastcall RGB24Deinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void __fastcall YV12Deinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void __fastcall YUY2Deinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);

	long				m_arlResizeModes[4];
	long				m_lResizeMode;
	BOOL				m_bDeinterlaceEnabled;
	BOOL				m_bRealNeedDeinterlace;
	long				m_lDeinterlaceMethod;
	long				m_lCropScale;

	//BYTE				m_nRGBDstBorderGrayScale;
	//BYTE				m_nYDstBorderGrayScale;
	//BYTE				m_nUDstBorderGrayScale;
	//BYTE				m_nVDstBorderGrayScale;

	DWORD				m_dwBGRDstBorderColor;
	BYTE				m_nYDstBorderColor;
	BYTE				m_nUDstBorderColor;
	BYTE				m_nVDstBorderColor;

	BOOL				m_bIsNeedAplly;

	SUncompressedVideoFrame		m_oSrcFormat;
	SUncompressedVideoFrame		m_oDstFormat;

	SUncompressedVideoFrame		m_oVideoBuffer1;
	SUncompressedVideoFrame		m_oVideoBuffer2;
	SUncompressedVideoFrame		m_oVideoBuffer3;

	BOOL				m_bFillBackground;
	BOOL				m_bIsNeedTransform;

	long				m_lInpColorSpace;
	long				m_lOutColorSpace;
	bool				m_bMirror;

	long				m_lRotateAngle;

	private:
		
	#define kMaxStride (2560 * 4)

#if defined(_MSC_VER) && !defined(__CLR_VER)
	#define SIMD_ALIGNED(var) __declspec(align(16)) var
#endif
	int I420Rotate(const BYTE* src_y, int src_stride_y,
				   const BYTE* src_u, int src_stride_u,
				   const BYTE* src_v, int src_stride_v,
				   BYTE* dst_y, int dst_stride_y,
				   BYTE* dst_u, int dst_stride_u,
				   BYTE* dst_v, int dst_stride_v,
				   int width, int height,
				   int mode)
	{
	  if (!src_y || !src_u || !src_v || width <= 0 || height == 0 ||
		  !dst_y || !dst_u || !dst_v) 
	  {
		return -1;
	  }

	  int halfwidth = (width + 1) >> 1;
	  int halfheight = (height + 1) >> 1;

	  // Negative height means invert the image.
	  if (height < 0) 
	  {
			height = -height;
			halfheight = (height + 1) >> 1;
			src_y = src_y + (height - 1) * src_stride_y;
			src_u = src_u + (halfheight - 1) * src_stride_u;
			src_v = src_v + (halfheight - 1) * src_stride_v;
			src_stride_y = -src_stride_y;
			src_stride_u = -src_stride_u;
			src_stride_v = -src_stride_v;
	  }

	  switch (mode)
	  {
		case 90:
		  RotatePlane90(src_y, src_stride_y,
						dst_y, dst_stride_y,
						width, height);
		  RotatePlane90(src_u, src_stride_u,
						dst_u, dst_stride_u,
						halfwidth, halfheight);
		  RotatePlane90(src_v, src_stride_v,
						dst_v, dst_stride_v,
						halfwidth, halfheight);
		  return 0;
		case 270:
		  RotatePlane270(src_y, src_stride_y,
						 dst_y, dst_stride_y,
						 width, height);
		  RotatePlane270(src_u, src_stride_u,
						 dst_u, dst_stride_u,
						 halfwidth, halfheight);
		  RotatePlane270(src_v, src_stride_v,
						 dst_v, dst_stride_v,
						 halfwidth, halfheight);
		  return 0;
		case 180:
		  RotatePlane180(src_y, src_stride_y,
						 dst_y, dst_stride_y,
						 width, height);
		  RotatePlane180(src_u, src_stride_u,
						 dst_u, dst_stride_u,
						 halfwidth, halfheight);
		  RotatePlane180(src_v, src_stride_v,
						 dst_v, dst_stride_v,
						 halfwidth, halfheight);
		  return 0;
		default:
		  break;
	  }
	  return -1;
	}
	void RotatePlane90(const BYTE* src, int src_stride,
					   BYTE* dst, int dst_stride,
					   int width, int height) 
	{
		// Rotate by 90 is a transpose with the source read
		// from bottom to top.  So set the source pointer to the end
		// of the buffer and flip the sign of the source stride.
		src += src_stride * (height - 1);
		src_stride = -src_stride;
		TransposePlane(src, src_stride, dst, dst_stride, width, height);
	}

	void RotatePlane270(const BYTE* src, int src_stride,
						BYTE* dst, int dst_stride,
						int width, int height)
	{
		// Rotate by 270 is a transpose with the destination written
		// from bottom to top.  So set the destination pointer to the end
		// of the buffer and flip the sign of the destination stride.
		dst += dst_stride * (width - 1);
		dst_stride = -dst_stride;
		TransposePlane(src, src_stride, dst, dst_stride, width, height);
	}

	void RotatePlane180(const BYTE* src, int src_stride,
						BYTE* dst, int dst_stride,
						int width, int height) 
	{
		// в будущем можно сделать 

	//  void (*MirrorRow)(const BYTE* src, BYTE* dst, int width) = MirrorRow_C;
	//#if defined(HAS_MIRRORROW_SSE2)
	//  if (TestCpuFlag(kCpuHasSSE2) &&
	//	  IS_ALIGNED(width, 16) &&
	//	  IS_ALIGNED(src, 16) && IS_ALIGNED(src_stride, 16) &&
	//	  IS_ALIGNED(dst, 16) && IS_ALIGNED(dst_stride, 16)) {
	//	MirrorRow = MirrorRow_SSE2;
	//  }
	//#endif
	//#if defined(HAS_MIRRORROW_SSSE3)
	//  if (TestCpuFlag(kCpuHasSSSE3) &&
	//	  IS_ALIGNED(width, 16) &&
	//	  IS_ALIGNED(src, 16) && IS_ALIGNED(src_stride, 16) &&
	//	  IS_ALIGNED(dst, 16) && IS_ALIGNED(dst_stride, 16)) {
	//	MirrorRow = MirrorRow_SSSE3;
	//  }
	//#endif
	//  void (*CopyRow)(const BYTE* src, BYTE* dst, int width) = CopyRow_C;
	//#if defined(HAS_COPYROW_X86)
	//  if (TestCpuFlag(kCpuHasX86) && IS_ALIGNED(width, 4)) {
	//	CopyRow = CopyRow_X86;
	//  }
	//#endif
	//#if defined(HAS_COPYROW_SSE2)
	//  if (TestCpuFlag(kCpuHasSSE2) && IS_ALIGNED(width, 32) &&
	//	  IS_ALIGNED(src, 16) && IS_ALIGNED(src_stride, 16) &&
	//	  IS_ALIGNED(dst, 16) && IS_ALIGNED(dst_stride, 16)) {
	//	CopyRow = CopyRow_SSE2;
	//  }
	//#endif
	  if (width > kMaxStride) 
	  {
		return;
	  }
	  // Swap first and last row and mirror the content.  Uses a temporary row.
	  SIMD_ALIGNED(BYTE row[kMaxStride]);
	  const BYTE* src_bot = src + src_stride * (height - 1);
	  BYTE* dst_bot = dst + dst_stride * (height - 1);
	  int half_height = (height + 1) >> 1;
	  // Odd height will harmlessly mirror the middle row twice.
	  for (int y = 0; y < half_height; ++y)
	  {
			MirrorRow(src, row, width);  // Mirror first row into a buffer
			src += src_stride;
			MirrorRow(src_bot, dst, width);  // Mirror last row into first row
			dst += dst_stride;
			CopyRow(row, dst_bot, width);  // Copy first mirrored row into last
			src_bot -= src_stride;
			dst_bot -= dst_stride;
	  }
	}

	void MirrorRow(const BYTE* src, BYTE* dst, int width) 
	{
	  src += width - 1;
	  for (int x = 0; x < width - 1; x += 2) {
		dst[x] = src[0];
		dst[x + 1] = src[-1];
		src -= 2;
	  }
	  if (width & 1)
	  {
		dst[width - 1] = src[0];
	  }
	}

	void CopyRow(const BYTE* src, BYTE* dst, int count) 
	{
	  memcpy(dst, src, count);
	}

	void TransposePlane(const BYTE* src, int src_stride,
						BYTE* dst, int dst_stride,
						int width, int height) 
	{
		//void (*TransposeWx8)(const BYTE* src, int src_stride,
			//			   BYTE* dst, int dst_stride,
			//			   int width) = TransposeWx8_C;

		//#if defined(HAS_TRANSPOSE_WX8_SSSE3)
		//  if (TestCpuFlag(kCpuHasSSSE3) && IS_ALIGNED(width, 8)) {
		//	TransposeWx8 = TransposeWx8_SSSE3;
		//  }
		//#endif
		//#if defined(HAS_TRANSPOSE_WX8_FAST_SSSE3)
		//  if (TestCpuFlag(kCpuHasSSSE3) &&
		//	  IS_ALIGNED(width, 16) &&
		//	  IS_ALIGNED(src, 16) && IS_ALIGNED(src_stride, 16)) {
		//	TransposeWx8 = TransposeWx8_FAST_SSSE3;
		//  }
		//#endif

		// Work across the source in 8x8 tiles
		int i = height;
		while (i >= 8) 
		{
			TransposeWx8(src, src_stride, dst, dst_stride, width);
			src += 8 * src_stride;    // Go down 8 rows.
			dst += 8;                 // Move over 8 columns.
			i -= 8;
		}

		TransposeWxH(src, src_stride, dst, dst_stride, width, i);
	}

	static void TransposeWx8(const BYTE* src, int src_stride,
							   BYTE* dst, int dst_stride,
							   int width)
	{
		for (int i = 0; i < width; ++i) 
		{
			dst[0] = src[0 * src_stride];
			dst[1] = src[1 * src_stride];
			dst[2] = src[2 * src_stride];
			dst[3] = src[3 * src_stride];
			dst[4] = src[4 * src_stride];
			dst[5] = src[5 * src_stride];
			dst[6] = src[6 * src_stride];
			dst[7] = src[7 * src_stride];
			++src;
			dst += dst_stride;
		}
	}

	static void TransposeWxH(const BYTE* src, int src_stride,
							   BYTE* dst, int dst_stride,
							   int width, int height) 
	{
		for (int i = 0; i < width; ++i) 
		{
			for (int j = 0; j < height; ++j) 
			{
				dst[i * dst_stride + j] = src[j * src_stride + i];
			}
		}
	}

	#define AVS_COMMON _T("SOFTWARE\\AVS4YOU\\Common")

	void GetDeinterlaceMethodeFromRegistry()
	{
		m_lDeinterlaceMethod = 0; // default		

		bool bGotValidValue = false;

		HKEY keyHandle;
		LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, AVS_COMMON, 0, KEY_QUERY_VALUE, &keyHandle);
		if(  ERROR_SUCCESS == result )
		{
			long nMode;
			DWORD size1=sizeof(nMode);
			DWORD Type;
			if ( ERROR_SUCCESS == RegQueryValueEx( keyHandle, _T("DeinterlaceMethod"), NULL, &Type,
				(LPBYTE)&nMode,&size1) )
			{
				if ( DEINTERLACE_METHOD_BLEND == nMode ||
					 DEINTERLACE_METHOD_MIDDLE == nMode )
				{					
					m_lDeinterlaceMethod = nMode;
					bGotValidValue = true;
				}
				else
				{
					ATLTRACE2("Invalid impl mode got from registry: %d\n", nMode);
				}
			}
		}
		RegCloseKey(keyHandle);

		if ( !bGotValidValue )
		{
			// failed to get valid mode from registry, let's update it (with MFX_IMPL_AUTO)
			UpdateDeinterlaceMethodToRegistry();
		}
	}
	void UpdateDeinterlaceMethodToRegistry()
	{
		HKEY   hkey;
		DWORD  dwDisposition;
		LONG result = RegCreateKeyEx(HKEY_CURRENT_USER, AVS_COMMON, 0, NULL, 0, KEY_CREATE_SUB_KEY|KEY_SET_VALUE, NULL, &hkey, &dwDisposition);
		if ( ERROR_SUCCESS == result )
		{
			long nVal = m_lDeinterlaceMethod;
			result = RegSetValueEx(hkey, _T("DeinterlaceMethod"), 0, REG_DWORD, (PBYTE)&nVal, sizeof(DWORD));
			if ( ERROR_SUCCESS != result )
				ATLTRACE2(_T("Cannot update registry key value.\n"));
			RegCloseKey(hkey);
		}
		else
			ATLTRACE2(_T("Cannot open registry key for writing.\n"));
	}
};

