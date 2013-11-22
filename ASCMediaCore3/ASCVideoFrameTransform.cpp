// AVSVideoFrameTransform.cpp : Implementation of CAVSVideoFrameTransform

#include "stdafx.h"
#include "ASCVideoFrameTransform.h"
#include <math.h>


// CAVSVideoFrameTransform
BOOL __fastcall CAVSVideoFrameTransform::IsChangeArrayFormat(CAVSUncompressedVideoFrame* pVideoFrame)
{
	if (pVideoFrame->m_oVideoFrame.Width!=m_oSrcFormat.Width)
		return true;
	if (pVideoFrame->m_oVideoFrame.Height!=m_oSrcFormat.Height)
		return true;
	if (pVideoFrame->m_oVideoFrame.ColorSpace!=m_oSrcFormat.ColorSpace)
		return true;
	BOOL m_bInReaspect = ((0!=pVideoFrame->m_oVideoFrame.AspectX)&&(0!=pVideoFrame->m_oVideoFrame.AspectY));
	BOOL m_bOutReaspect  = ((0!=m_oSrcFormat.AspectX)&&(0!=m_oSrcFormat.AspectY));
	if (m_bInReaspect!=m_bOutReaspect)
		return TRUE;
	if (m_bInReaspect)
	{
		if ((pVideoFrame->m_oVideoFrame.AspectX != m_oSrcFormat.AspectX)||
			(pVideoFrame->m_oVideoFrame.AspectY != m_oSrcFormat.AspectY))
			return TRUE;
	}
	if (pVideoFrame->m_oVideoFrame.Interlaced != m_oSrcFormat.Interlaced)
		return TRUE;
	if (pVideoFrame->m_oVideoFrame.Stride[0] != m_oSrcFormat.Stride[0])
		return TRUE;

	switch (m_oSrcFormat.ColorSpace & CSP_COLOR_MASK)
	{
	case CSP_I420:
	case CSP_YV12:
		if (pVideoFrame->m_oVideoFrame.Stride[1] != m_oSrcFormat.Stride[1])
			return TRUE;
		if (pVideoFrame->m_oVideoFrame.Stride[2] != m_oSrcFormat.Stride[2])
			return TRUE;
	}
	return FALSE;
}

void __fastcall CAVSVideoFrameTransform::ApplySettings()
{
/*
	double	dRealWidth;
	double	dRealHeight;

	if ((0==m_oSrcFormat.AspectX)||(0==m_oSrcFormat.AspectY)||(0==m_oDstFormat.AspectX)||(0==m_oDstFormat.AspectY))
	{
//Преобразование к правильному разрешению
		dRealWidth  = m_oDstFormat.Width;
		dRealHeight = m_oDstFormat.Height;
		m_bFillBackground = FALSE;
	} 
	else
	{
		double	dInpRate = (double)m_oSrcFormat.AspectX/m_oSrcFormat.AspectY;
		double	dOutRate = (double)m_oDstFormat.AspectX/m_oDstFormat.AspectY;

		dRealWidth = m_oSrcFormat.Height * dInpRate;
		dRealHeight = m_oSrcFormat.Height;

		double	KX = ((double)m_oDstFormat.Height*dOutRate)/dRealWidth;
		double	KY = (double)m_oDstFormat.Width/(dRealHeight*dOutRate);

		
		m_lCropScale = (m_lCropScale<0||m_lCropScale>100) ? 0 : m_lCropScale;
		if ((dRealHeight*KX) > m_oDstFormat.Height)
		{
			//dRealWidth*KY может быть равно нулю?
			dRealHeight = m_oDstFormat.Height*((m_oDstFormat.Width - dRealWidth*KY) * m_lCropScale/100.0 + dRealWidth*KY)/(dRealWidth*KY);
			dRealWidth = dRealWidth*KY+(m_oDstFormat.Width - dRealWidth*KY)*m_lCropScale/100.0;
		} 
		else
		{
			dRealWidth = m_oDstFormat.Width*((m_oDstFormat.Height - dRealHeight*KX) *m_lCropScale/100.0 + dRealHeight*KX)/(dRealHeight*KX);
			dRealHeight = dRealHeight*KX+(m_oDstFormat.Height - dRealHeight*KX) * m_lCropScale/100.0;
		}
		if (m_lCropScale!=100)
		{
			if ((dRealWidth==m_oDstFormat.Width)&&(dRealHeight==m_oDstFormat.Height))
				m_bFillBackground=FALSE;
			else
				m_bFillBackground=TRUE;
		}
		else
			m_bFillBackground=FALSE;
	}

	m_oDstFormat.kx = (double)dRealWidth/m_oSrcFormat.Width;
	m_oDstFormat.ky = (double)dRealHeight/m_oSrcFormat.Height;
*/

	if ((0==m_oDstFormat.AspectX)&&(1==m_oDstFormat.AspectY))
	{
		m_oDstFormat.kx = (double)m_oDstFormat.Width/(double)m_oSrcFormat.Width;
		m_oDstFormat.ky = (double)m_oDstFormat.Height/(double)m_oSrcFormat.Height;
		m_bFillBackground = TRUE;//
	}
	else
	{
		long lSrcAspectX = m_oSrcFormat.AspectX;
		long lSrcAspectY = m_oSrcFormat.AspectY;
		long lDstAspectX = m_oDstFormat.AspectX;
		long lDstAspectY = m_oDstFormat.AspectY;
		if ((0>=m_oSrcFormat.AspectX)||(0>=m_oSrcFormat.AspectY))
		{
			lSrcAspectX = m_oSrcFormat.Width;
			lSrcAspectY = m_oSrcFormat.Height;
		} 
		if ((0>=m_oDstFormat.AspectX)||(0>=m_oDstFormat.AspectY))
		{
			lDstAspectX = m_oDstFormat.Width;
			lDstAspectY = m_oDstFormat.Height;
		}

		m_lCropScale = (m_lCropScale<0||m_lCropScale>100) ? 0 : m_lCropScale;

		double dGenKoef = (1.0 - (double)m_lCropScale/100.0)*min((double)lDstAspectX/(double)lSrcAspectX, (double)lDstAspectY/(double)lSrcAspectY);
		dGenKoef += (double)m_lCropScale/100.0*max((double)lDstAspectX/(double)lSrcAspectX, (double)lDstAspectY/(double)lSrcAspectY);

		m_oDstFormat.kx = (double)lSrcAspectX/(double)m_oSrcFormat.Width * dGenKoef * (double)m_oDstFormat.Width/(double)lDstAspectX;
		m_oDstFormat.ky = (double)lSrcAspectY/(double)m_oSrcFormat.Height * dGenKoef * (double)m_oDstFormat.Height/(double)lDstAspectY;

		m_bFillBackground = (0 != abs(m_oDstFormat.Width - m_oDstFormat.kx * m_oSrcFormat.Width))||
							(0 != abs(m_oDstFormat.Height - m_oDstFormat.ky * m_oSrcFormat.Height));
	}
	//два действительных числа сравниваем на точное равенство?
	BOOL bReSizeNeed = ((abs(1.0 - m_oDstFormat.kx)>0.01)|| (abs(1.0 - m_oDstFormat.ky)>0.01)||
		(m_oSrcFormat.Width!=m_oDstFormat.Width)||(m_oSrcFormat.Height!=m_oDstFormat.Height));

	m_lInpColorSpace = m_oSrcFormat.ColorSpace & CSP_COLOR_MASK;
	m_lOutColorSpace = m_oDstFormat.ColorSpace & CSP_COLOR_MASK;
	m_bMirror = (0!=((m_oSrcFormat.ColorSpace & CSP_VFLIP) ^ (m_oDstFormat.ColorSpace & CSP_VFLIP)));

	m_bRealNeedDeinterlace = FALSE;
	m_bRealNeedDeinterlace = m_bDeinterlaceEnabled && m_oSrcFormat.Interlaced;
	if (m_bRealNeedDeinterlace)
		m_oDstFormat.Interlaced = FALSE;
	else
		m_oDstFormat.Interlaced = m_oSrcFormat.Interlaced;

	FreeBuffers();

	if (m_bRealNeedDeinterlace)
	{
		if (bReSizeNeed)
		{
			m_nTransformType = ttResizeDeinterlaceTransform;
			ResizeDeinterlaceAlloc();
		}
		else
		{
			m_nTransformType = ttDeinterlaceTransform;
			DeinterlaceAlloc();
		}
	}
	else
	{
		if (bReSizeNeed)
		{
			m_nTransformType = ttResizeTransform;
			ResizeAlloc();
		}
		else
		{
			m_nTransformType = ttSimpleTransform;
			SimpleAlloc();
		}
	}

	m_bIsNeedTransform = FALSE;
	if (bReSizeNeed)
		m_bIsNeedTransform = TRUE;
	if (m_oSrcFormat.ColorSpace != m_oDstFormat.ColorSpace)
		m_bIsNeedTransform = TRUE;
	if (m_bRealNeedDeinterlace)
		m_bIsNeedTransform = TRUE;
	if ((m_oSrcFormat.Stride[0] != m_oDstFormat.Stride[0]))
		m_bIsNeedTransform = TRUE;


	switch (m_oSrcFormat.ColorSpace&CSP_COLOR_MASK)
	{
	case CSP_I420:
	case CSP_YV12:
		if (m_oSrcFormat.Stride[1]!=m_oDstFormat.Stride[1])
			m_bIsNeedTransform = TRUE;
		if (m_oSrcFormat.Stride[2]!=m_oDstFormat.Stride[2])
			m_bIsNeedTransform = TRUE;
	}	

	m_bIsNeedAplly = FALSE;
}

void __fastcall CAVSVideoFrameTransform::FillBuffer(SUncompressedVideoFrame* pVideoFrame, BYTE* pBuffer)
{
	switch (pVideoFrame->ColorSpace & CSP_COLOR_MASK)
	{
	case CSP_I420:
	case CSP_YV12:
		pVideoFrame->Plane[0] = pBuffer;
		pVideoFrame->Plane[1] = (BYTE*)pBuffer + pVideoFrame->Stride[0]*pVideoFrame->Height;
		pVideoFrame->Plane[2] = (BYTE*)pBuffer + pVideoFrame->Stride[0]*pVideoFrame->Height + pVideoFrame->Stride[1]*pVideoFrame->Height/2;
		break;
	//case CSP_YV12:
	//	pVideoFrame->Plane[0] = pBuffer;
	//	pVideoFrame->Plane[1] = (BYTE*)pBuffer + pVideoFrame->Stride[0]*pVideoFrame->Height + pVideoFrame->Stride[1]*pVideoFrame->Height/2;
	//	pVideoFrame->Plane[2] = (BYTE*)pBuffer + pVideoFrame->Stride[0]*pVideoFrame->Height;
	//	break;
	case CSP_YUY2:
	case CSP_UYVY:
	case CSP_YVYU:

	case CSP_RGB555:
	case CSP_RGB565:

	case CSP_BGR:

	case CSP_BGRA:
	case CSP_ABGR:
	case CSP_RGBA:
	case CSP_ARGB:
		pVideoFrame->Plane[0] = pBuffer;
		break;
	}
}
void __fastcall CAVSVideoFrameTransform::AllocBuffer(SUncompressedVideoFrame* pVideoFrame, SUncompressedVideoFrame* pFrame, long lColorSpace)
{
	*pVideoFrame = *pFrame;
	pVideoFrame->ColorSpace = lColorSpace;
	SetDefaultStride(pVideoFrame);
	int nDataSize = pVideoFrame->Stride[0] * pVideoFrame->Height;

	switch (lColorSpace & CSP_COLOR_MASK)
	{
	case CSP_I420:
	case CSP_YV12:
		FillBuffer(pVideoFrame, (BYTE*)::HeapAlloc(GetProcessHeap(), NULL, nDataSize + (pVideoFrame->Stride[1] + pVideoFrame->Stride[2])*pVideoFrame->Height/2));
		break;
	case CSP_YUY2:
	case CSP_UYVY:
	case CSP_YVYU:

	case CSP_RGB555:
	case CSP_RGB565:

	case CSP_BGR:

	case CSP_BGRA:
	case CSP_ABGR:
	case CSP_RGBA:
	case CSP_ARGB:
		FillBuffer(pVideoFrame, (BYTE*)::HeapAlloc(GetProcessHeap(), NULL, nDataSize));
		break;
	}
}
void __fastcall CAVSVideoFrameTransform::FreeBuffer(SUncompressedVideoFrame *pVideoFrame)
{
	if (NULL!=pVideoFrame->Plane[0])
	{
		::HeapFree(GetProcessHeap(), NULL, pVideoFrame->Plane[0]);
		pVideoFrame->Plane[0] = NULL;
	}
}
void __fastcall CAVSVideoFrameTransform::FreeBuffers()
{
	if (NULL!=m_oVideoBuffer1.Plane[0])
		FreeBuffer(&m_oVideoBuffer1);
	if (m_oVideoBuffer2.Plane[0]!=NULL)
		FreeBuffer(&m_oVideoBuffer2);
	if (m_oVideoBuffer3.Plane[0]!=NULL)
		FreeBuffer(&m_oVideoBuffer3);
}
void __fastcall CAVSVideoFrameTransform::SetDefaultStride(SUncompressedVideoFrame* pVideoFrame)
{
	switch (pVideoFrame->ColorSpace & CSP_COLOR_MASK)
	{
	case CSP_I420:
	case CSP_YV12:
		pVideoFrame->Stride[0] = pVideoFrame->Width;
		pVideoFrame->Stride[1] = pVideoFrame->Width/2;
		pVideoFrame->Stride[2] = pVideoFrame->Width/2;
		break;
	case CSP_YUY2:
	case CSP_UYVY:
	case CSP_YVYU:

	case CSP_RGB555:
	case CSP_RGB565:
		pVideoFrame->Stride[0] = 2*pVideoFrame->Width;
		break;
	case CSP_BGR:
		pVideoFrame->Stride[0] = 3*pVideoFrame->Width;
		break;
	case CSP_BGRA:
	case CSP_ABGR:
	case CSP_RGBA:
	case CSP_ARGB: 
		pVideoFrame->Stride[0] = 4*pVideoFrame->Width;
		break;
	}
}

//
void CAVSVideoFrameTransform::ErrorTransform()
{
	throw("Method 'ApllySettings' must be called\n");
}
void CAVSVideoFrameTransform::SimpleTransform()
{
	switch (m_lOutColorSpace)
	{
	case CSP_I420:
	case CSP_YV12:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			fastconvert_yv12_to_yv12(m_oDstFormat.Plane[0], m_oDstFormat.Plane[1], m_oDstFormat.Plane[2], m_oDstFormat.Stride[0], m_oDstFormat.Stride[1],
				m_oSrcFormat.Plane[0], m_oSrcFormat.Plane[1], m_oSrcFormat.Plane[2], m_oSrcFormat.Stride[0], m_oSrcFormat.Stride[1],
				m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror);
			fastconvert_emms();
			break;
		case CSP_YUY2:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oDstFormat.Plane[0], m_oDstFormat.Plane[1], m_oDstFormat.Plane[2], m_oDstFormat.Stride[0], m_oDstFormat.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);
			fastconvert_emms();
			break;
		case CSP_BGRA:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oDstFormat.Plane[0], m_oDstFormat.Plane[1], m_oDstFormat.Plane[2], m_oDstFormat.Stride[0], m_oDstFormat.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_bgra_to_yv12, fastconvert_bgra_to_yv12_c, 4);
			fastconvert_emms();
			break;
		case CSP_BGR:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oDstFormat.Plane[0], m_oDstFormat.Plane[1], m_oDstFormat.Plane[2], m_oDstFormat.Stride[0], m_oDstFormat.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_bgr_to_yv12, fastconvert_bgr_to_yv12_c, 3);
			fastconvert_emms();
			break;
		}
		break;
	case CSP_YUY2:
		switch(m_lInpColorSpace)
		{
			case CSP_I420:
			case CSP_YV12:
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oSrcFormat.Plane[0], m_oSrcFormat.Plane[1], m_oSrcFormat.Plane[2],
				m_oSrcFormat.Stride[0], m_oSrcFormat.Stride[1], 
				m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();
				break;

			case CSP_YUY2:
				fastconvert_YUY2CopyMem(&m_oSrcFormat, &m_oDstFormat, m_bMirror);
				break;

			case CSP_BGRA:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], 
				m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_bgra_to_yv12, fastconvert_bgra_to_yv12_c, 4);
			fastconvert_emms();

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], 
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, false,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();
				break;

			case CSP_BGR:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1],
				m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_bgr_to_yv12, fastconvert_bgr_to_yv12_c, 3);
			fastconvert_emms();

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], 
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, false,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();
				break;
		}		
		break;
	case CSP_BGRA:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oSrcFormat.Plane[0], m_oSrcFormat.Plane[1], m_oSrcFormat.Plane[2],
				m_oSrcFormat.Stride[0], m_oSrcFormat.Stride[1], 
				m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_yv12_to_bgra, fastconvert_yv12_to_bgra_c, 4);
			fastconvert_emms();
			break;
		case CSP_YUY2:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1],
				m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1],
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, false,
				fastconvert_yv12_to_bgra, fastconvert_yv12_to_bgra_c, 4);
			fastconvert_emms();
			break;
		case CSP_BGRA:
			fastconvert_RGB32CopyMem(&m_oSrcFormat, &m_oDstFormat, m_bMirror);
			break;
		case CSP_BGR:
			RGB24ToRGB32(&m_oSrcFormat, &m_oDstFormat, m_bMirror);
			break;
		case CSP_RGB8:
			RGB8ToRGB32(&m_oSrcFormat, &m_oDstFormat, m_bMirror);
			break;
		case CSP_RGB16:
			//RGB16ToRGB32(&m_oSrcFormat, &m_oDstFormat, m_bMirror);
			break;
		}
		break;

	case CSP_BGR:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oSrcFormat.Plane[0], m_oSrcFormat.Plane[1], m_oSrcFormat.Plane[2], m_oSrcFormat.Stride[0], m_oSrcFormat.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_yv12_to_bgr, fastconvert_yv12_to_bgr_c, 3);
			fastconvert_emms();
			break;
		case CSP_YUY2:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1],
				m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1],
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, false,
				fastconvert_yv12_to_bgr, fastconvert_yv12_to_bgr_c, 3);
			fastconvert_emms();
			break;
		case CSP_BGRA:
			RGB32ToRGB24(&m_oSrcFormat, &m_oDstFormat, m_bMirror);
			break;
		case CSP_BGR:
			fastconvert_RGB24CopyMem(&m_oSrcFormat, &m_oDstFormat, m_bMirror);
			break;
		case CSP_RGB8:
			RGB8ToRGB24(&m_oSrcFormat, &m_oDstFormat, m_bMirror);
			break;
		case CSP_RGB16:
			//RGB16ToRGB24(&m_oSrcFormat, &m_oDstFormat, m_bMirror);
			break;
		}
		break;
	}
}
void CAVSVideoFrameTransform::ResizeTransform()
{
	switch (m_lOutColorSpace)
	{
	case CSP_I420:
	case CSP_YV12:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			if (m_bMirror)
			{
				fastconvert_yv12_to_yv12(m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1],
					m_oSrcFormat.Plane[0], m_oSrcFormat.Plane[1], m_oSrcFormat.Plane[2], m_oSrcFormat.Stride[0], m_oSrcFormat.Stride[1],
					m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror);
				fastconvert_emms();
				YV12Resize(&m_oVideoBuffer1, &m_oDstFormat);
			} else
				YV12Resize(&m_oSrcFormat, &m_oDstFormat);
			break;
		case CSP_YUY2:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);
			fastconvert_emms();
			YV12Resize(&m_oVideoBuffer1, &m_oDstFormat);
			break;
		case CSP_BGRA:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_bgra_to_yv12, fastconvert_bgra_to_yv12_c, 4);
			fastconvert_emms();
			YV12Resize(&m_oVideoBuffer1, &m_oDstFormat);
			break;
		case CSP_BGR:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_bgr_to_yv12, fastconvert_bgr_to_yv12_c, 3);
			fastconvert_emms();
			YV12Resize(&m_oVideoBuffer1, &m_oDstFormat);
			break;
		}
		break;

	case CSP_YUY2:
		switch(m_lInpColorSpace)
		{
			case CSP_I420:
			case CSP_YV12:
			YV12Resize(&m_oSrcFormat, &m_oVideoBuffer1);
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], 
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();
				break;
			case CSP_YUY2:
				if (m_bMirror)
				{
				  fastconvert_YUY2CopyMem(&m_oSrcFormat, &m_oVideoBuffer1, m_bMirror);
				  YUY2Resize(&m_oVideoBuffer1, &m_oDstFormat);
				}
				else
				{
			      YUY2Resize(&m_oSrcFormat, &m_oDstFormat);
				}
				break;
			case CSP_BGRA:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], 
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_bgra_to_yv12, fastconvert_bgra_to_yv12_c, 4);
			fastconvert_emms();
			YV12Resize(&m_oVideoBuffer1, &m_oVideoBuffer2);
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1], 
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, false,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();

				break;
			case CSP_BGR:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1],
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_bgr_to_yv12, fastconvert_bgr_to_yv12_c, 3);
			fastconvert_emms();

			YV12Resize(&m_oVideoBuffer1, &m_oVideoBuffer2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1], 
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, false,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();
				break;
		}
		break;

	case CSP_BGRA:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			YV12Resize(&m_oSrcFormat, &m_oVideoBuffer1);
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_yv12_to_bgra, fastconvert_yv12_to_bgra_c, 4);
			fastconvert_emms();
			break;
		case CSP_YUY2:
     		YUY2Resize(&m_oSrcFormat, &m_oVideoBuffer1);

			fastconvert_safe_packed_conv(m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1],
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1],
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, false,
				fastconvert_yv12_to_bgra, fastconvert_yv12_to_bgra_c, 4);
			fastconvert_emms();
			break;

		case CSP_BGRA:
			if (m_bMirror)
			{
				fastconvert_RGB32CopyMem(&m_oSrcFormat, &m_oVideoBuffer1, m_bMirror);
				RGB32Resize(&m_oVideoBuffer1, &m_oDstFormat);
			}
			else
				RGB32Resize(&m_oSrcFormat, &m_oDstFormat);
			break;
		case CSP_BGR:
			RGB24Resize(&m_oSrcFormat, &m_oVideoBuffer1);
			RGB24ToRGB32(&m_oVideoBuffer1, &m_oDstFormat, m_bMirror);
			break;
		}
		break;

	case CSP_BGR:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			YV12Resize(&m_oSrcFormat, &m_oVideoBuffer1);
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_yv12_to_bgr, fastconvert_yv12_to_bgr_c, 3);
			fastconvert_emms();
			break;

		case CSP_YUY2:
     		YUY2Resize(&m_oSrcFormat, &m_oVideoBuffer1);

			fastconvert_safe_packed_conv(m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1],
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1],
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, false,
				fastconvert_yv12_to_bgr, fastconvert_yv12_to_bgr_c, 3);
			fastconvert_emms();
			break;

		case CSP_BGRA:
			RGB32ToRGB24(&m_oSrcFormat, &m_oVideoBuffer1, m_bMirror);
			RGB24Resize(&m_oVideoBuffer1, &m_oDstFormat);
			break;
		case CSP_BGR:
			if (m_bMirror)
			{
				fastconvert_RGB24CopyMem(&m_oSrcFormat, &m_oVideoBuffer1, m_bMirror);
				RGB24Resize(&m_oVideoBuffer1, &m_oDstFormat);
			}
			else
				RGB24Resize(&m_oSrcFormat, &m_oDstFormat);
			break;
		}
		break;
	}
}
void CAVSVideoFrameTransform::DeinterlaceTransform()
{
	switch (m_lOutColorSpace)
	{
	case CSP_I420:
	case CSP_YV12:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			if (m_bMirror)
			{
				YV12Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
				fastconvert_yv12_to_yv12(m_oDstFormat.Plane[0], m_oDstFormat.Plane[1], m_oDstFormat.Plane[2], m_oDstFormat.Stride[0], m_oDstFormat.Stride[1],
					m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1],
					m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror);
				fastconvert_emms();
			} else
				YV12Deinterlace(&m_oSrcFormat, &m_oDstFormat);
			break;
		case CSP_YUY2:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);
			fastconvert_emms();
			YV12Deinterlace(&m_oVideoBuffer1, &m_oDstFormat);
			break;
		case CSP_BGRA:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_bgra_to_yv12, fastconvert_bgra_to_yv12_c, 4);
			fastconvert_emms();
			YV12Deinterlace(&m_oVideoBuffer1, &m_oDstFormat);
			break;
		case CSP_BGR:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_bgr_to_yv12, fastconvert_bgr_to_yv12_c, 3);
			fastconvert_emms();
			YV12Deinterlace(&m_oVideoBuffer1, &m_oDstFormat);
			break;
		}
		break;

	case CSP_YUY2:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			YV12Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
		    fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], 
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();
			break;

		case CSP_YUY2:
			if (m_bMirror)
			{
			  fastconvert_YUY2CopyMem(&m_oSrcFormat, &m_oVideoBuffer1, m_bMirror);
              YUY2Deinterlace(&m_oVideoBuffer1, &m_oDstFormat);
			}
			else
			{
              YUY2Deinterlace(&m_oSrcFormat, &m_oDstFormat);
			}
			break;

		case CSP_BGRA:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], 
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_bgra_to_yv12, fastconvert_bgra_to_yv12_c, 4);
			fastconvert_emms();

			YV12Deinterlace(&m_oVideoBuffer1, &m_oVideoBuffer2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1], 
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, false,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();
			break;

		case CSP_BGR:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], 
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_bgr_to_yv12, fastconvert_bgr_to_yv12_c, 3);
			fastconvert_emms();

			YV12Deinterlace(&m_oVideoBuffer1, &m_oVideoBuffer2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1], 
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, false,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();
			break;
		}
		break;

	case CSP_BGRA:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			YV12Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_yv12_to_bgra, fastconvert_yv12_to_bgra_c, 4);
			fastconvert_emms();
			break;
		case CSP_YUY2:
     		YUY2Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);

			fastconvert_safe_packed_conv(m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1],
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1],
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, false,
				fastconvert_yv12_to_bgra, fastconvert_yv12_to_bgra_c, 4);
			fastconvert_emms();
			break;

		case CSP_BGRA:
			if (m_bMirror)
			{
				RGB32Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
				fastconvert_RGB32CopyMem(&m_oVideoBuffer1, &m_oDstFormat, m_bMirror);
			} else
				RGB32Deinterlace(&m_oSrcFormat, &m_oDstFormat);
			break;
		case CSP_BGR:
			RGB24Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			RGB24ToRGB32(&m_oVideoBuffer1, &m_oDstFormat, m_bMirror);
			break;
		}
		break;

	case CSP_BGR:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			YV12Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_yv12_to_bgr, fastconvert_yv12_to_bgr_c, 3);
			fastconvert_emms();
			break;

		case CSP_YUY2:
     		YUY2Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);

			fastconvert_safe_packed_conv(m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1],
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1],
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, false,
				fastconvert_yv12_to_bgr, fastconvert_yv12_to_bgr_c, 3);
			fastconvert_emms();
			break;

		case CSP_BGRA:
			RGB32Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			RGB32ToRGB24(&m_oVideoBuffer1, &m_oDstFormat, m_bMirror);
			break;
		case CSP_BGR:
			if (m_bMirror)
			{
				RGB24Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
				fastconvert_RGB24CopyMem(&m_oVideoBuffer1, &m_oDstFormat, m_bMirror);
			} else
				RGB24Deinterlace(&m_oSrcFormat, &m_oDstFormat);
			break;
		}
		break;
	}
}

void CAVSVideoFrameTransform::ResizeDeinterlaceTransform()
{
	switch (m_lOutColorSpace)
	{
	case CSP_I420:
	case CSP_YV12:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			YV12Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			if (m_bMirror)
			{
				fastconvert_yv12_to_yv12(m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2], m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1],
					m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1],
					m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror);
				fastconvert_emms();
				YV12Resize(&m_oVideoBuffer2, &m_oDstFormat);
			} else
				YV12Resize(&m_oVideoBuffer1, &m_oDstFormat);
			break;
		case CSP_YUY2:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);
			fastconvert_emms();
			YV12Deinterlace(&m_oVideoBuffer1, &m_oVideoBuffer2);
			YV12Resize(&m_oVideoBuffer2, &m_oDstFormat);
			break;
		case CSP_BGRA:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_bgra_to_yv12, fastconvert_bgra_to_yv12_c, 4);
			fastconvert_emms();
			YV12Deinterlace(&m_oVideoBuffer1, &m_oVideoBuffer2);
			YV12Resize(&m_oVideoBuffer2, &m_oDstFormat);
			break;
		case CSP_BGR:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2], m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], m_oSrcFormat.Width, m_oSrcFormat.Height, m_bMirror,
				fastconvert_bgr_to_yv12, fastconvert_bgr_to_yv12_c, 4);
			fastconvert_emms();
			YV12Deinterlace(&m_oVideoBuffer1, &m_oVideoBuffer2);
			YV12Resize(&m_oVideoBuffer2, &m_oDstFormat);
			break;
		}
		break;

	case CSP_YUY2:
		switch(m_lInpColorSpace)
		{
			case CSP_I420:
			case CSP_YV12:
			YV12Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			YV12Resize(&m_oVideoBuffer1, &m_oVideoBuffer2);
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2],
				m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1], 
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, m_bMirror,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();
				break;
			case CSP_YUY2:
			  if (m_bMirror)
			  {
			    fastconvert_YUY2CopyMem(&m_oSrcFormat, &m_oVideoBuffer1, m_bMirror);
			    YUY2Deinterlace(&m_oVideoBuffer1, &m_oVideoBuffer2);
                YUY2Resize(&m_oVideoBuffer2, &m_oDstFormat);
			  }
			  else
			  {
                YUY2Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
                YUY2Resize(&m_oVideoBuffer1, &m_oDstFormat);
			  }
			  break;
			case CSP_BGRA:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1], 
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_bgra_to_yv12, fastconvert_bgra_to_yv12_c, 4);
			fastconvert_emms();

            YV12Deinterlace(&m_oVideoBuffer1, &m_oVideoBuffer2);
			YV12Resize(&m_oVideoBuffer2, &m_oVideoBuffer3);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer3.Plane[0], m_oVideoBuffer3.Plane[1], m_oVideoBuffer3.Plane[2],
				m_oVideoBuffer3.Stride[0], m_oVideoBuffer3.Stride[1], 
				m_oVideoBuffer3.Width, m_oVideoBuffer3.Height, false,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();

				break;
			case CSP_BGR:
			fastconvert_safe_packed_conv(m_oSrcFormat.Plane[0], m_oSrcFormat.Stride[0],
				m_oVideoBuffer1.Plane[0], m_oVideoBuffer1.Plane[1], m_oVideoBuffer1.Plane[2],
				m_oVideoBuffer1.Stride[0], m_oVideoBuffer1.Stride[1],
				m_oVideoBuffer1.Width, m_oVideoBuffer1.Height, m_bMirror,
				fastconvert_bgr_to_yv12, fastconvert_bgr_to_yv12_c, 3);
			fastconvert_emms();

            YV12Deinterlace(&m_oVideoBuffer1, &m_oVideoBuffer2);
			YV12Resize(&m_oVideoBuffer2, &m_oVideoBuffer3);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer3.Plane[0], m_oVideoBuffer3.Plane[1], m_oVideoBuffer3.Plane[2],
				m_oVideoBuffer3.Stride[0], m_oVideoBuffer3.Stride[1], 
				m_oVideoBuffer3.Width, m_oVideoBuffer3.Height, false,
				fastconvert_yv12_to_yuyv, fastconvert_yv12_to_yuyv_c, 2);
			fastconvert_emms();
				break;
		}
		break;

	case CSP_BGRA:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			YV12Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			YV12Resize(&m_oVideoBuffer1, &m_oVideoBuffer2);
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2], m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1], m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, m_bMirror,
				fastconvert_yv12_to_bgra, fastconvert_yv12_to_bgra_c, 4);
			fastconvert_emms();
			break;

		case CSP_YUY2:		     		
			YUY2Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			YUY2Resize(&m_oVideoBuffer1, &m_oVideoBuffer2);

			fastconvert_safe_packed_conv(m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Stride[0],
				m_oVideoBuffer3.Plane[0], m_oVideoBuffer3.Plane[1], m_oVideoBuffer3.Plane[2],
				m_oVideoBuffer3.Stride[0], m_oVideoBuffer3.Stride[1],
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer3.Plane[0], m_oVideoBuffer3.Plane[1], m_oVideoBuffer3.Plane[2],
				m_oVideoBuffer3.Stride[0], m_oVideoBuffer3.Stride[1],
				m_oVideoBuffer3.Width, m_oVideoBuffer3.Height, false,
				fastconvert_yv12_to_bgra, fastconvert_yv12_to_bgra_c, 4);
			fastconvert_emms();
			break;

		case CSP_BGRA:
			RGB32Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			if (m_bMirror)
			{
				fastconvert_RGB32CopyMem(&m_oVideoBuffer1, &m_oVideoBuffer2, m_bMirror);
				RGB32Resize(&m_oVideoBuffer2, &m_oDstFormat);
			} else
				RGB32Resize(&m_oVideoBuffer1, &m_oDstFormat);
			break;
		case CSP_BGR:
			RGB24Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			RGB24Resize(&m_oVideoBuffer1, &m_oVideoBuffer2);
			RGB24ToRGB32(&m_oVideoBuffer2, &m_oDstFormat, m_bMirror);
			break;
		}
		break;

	case CSP_BGR:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			YV12Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			YV12Resize(&m_oVideoBuffer1, &m_oVideoBuffer2);
			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Plane[1], m_oVideoBuffer2.Plane[2], m_oVideoBuffer2.Stride[0], m_oVideoBuffer2.Stride[1], m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, m_bMirror,
				fastconvert_yv12_to_bgr, fastconvert_yv12_to_bgr_c, 3);
			fastconvert_emms();
			break;

		case CSP_YUY2:		     		
			YUY2Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			YUY2Resize(&m_oVideoBuffer1, &m_oVideoBuffer2);

			fastconvert_safe_packed_conv(m_oVideoBuffer2.Plane[0], m_oVideoBuffer2.Stride[0],
				m_oVideoBuffer3.Plane[0], m_oVideoBuffer3.Plane[1], m_oVideoBuffer3.Plane[2],
				m_oVideoBuffer3.Stride[0], m_oVideoBuffer3.Stride[1],
				m_oVideoBuffer2.Width, m_oVideoBuffer2.Height, m_bMirror,
				fastconvert_yuyv_to_yv12, fastconvert_yuyv_to_yv12_c, 2);

			fastconvert_safe_packed_conv(m_oDstFormat.Plane[0], m_oDstFormat.Stride[0],
				m_oVideoBuffer3.Plane[0], m_oVideoBuffer3.Plane[1], m_oVideoBuffer3.Plane[2],
				m_oVideoBuffer3.Stride[0], m_oVideoBuffer3.Stride[1],
				m_oVideoBuffer3.Width, m_oVideoBuffer3.Height, false,
				fastconvert_yv12_to_bgr, fastconvert_yv12_to_bgr_c, 3);
			fastconvert_emms();
			break;

		case CSP_BGRA:
			RGB32Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			RGB32ToRGB24(&m_oVideoBuffer1, &m_oVideoBuffer2, m_bMirror);
			RGB24Resize(&m_oVideoBuffer2, &m_oDstFormat);
			break;
		case CSP_BGR:
			RGB24Deinterlace(&m_oSrcFormat, &m_oVideoBuffer1);
			if (m_bMirror)
			{
				fastconvert_RGB24CopyMem(&m_oVideoBuffer1, &m_oVideoBuffer2, m_bMirror);
				RGB24Resize(&m_oVideoBuffer2, &m_oDstFormat);
			} else
				RGB24Resize(&m_oVideoBuffer1, &m_oDstFormat);
			break;
		}
		break;
	}
}
//	 
void CAVSVideoFrameTransform::ResizeDeinterlaceAlloc()
{
	switch (m_lOutColorSpace)
	{
	case CSP_I420:
	case CSP_YV12:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			if (m_bMirror)
				AllocBuffer(&m_oVideoBuffer2, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_YUY2:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_BGRA:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_BGR:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		}
		break;
	case CSP_YUY2:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			break;
		case CSP_YUY2:
            if (m_bMirror)
			{
			  AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			  AllocBuffer(&m_oVideoBuffer2, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			}
			else
			{
			  AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			}
			break;
		case CSP_BGRA:
		case CSP_BGR:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, CSP_YV12);
			AllocBuffer(&m_oVideoBuffer2, &m_oSrcFormat, CSP_YV12);
			AllocBuffer(&m_oVideoBuffer3, &m_oDstFormat, CSP_YV12);
			break;
		}
		break;

	case CSP_BGR:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			break;
			
		case CSP_YUY2:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer3, &m_oDstFormat, CSP_YV12);
			break;

		case CSP_BGRA:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_BGR:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			if (m_bMirror)
				AllocBuffer(&m_oVideoBuffer2, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		}
		break;

	case CSP_BGRA:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			break;

		case CSP_YUY2:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer3, &m_oDstFormat, CSP_YV12);
			break;

		case CSP_BGRA:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			if (m_bMirror)
				AllocBuffer(&m_oVideoBuffer2, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_BGR:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			break;
		}
		break;
	}
}

void CAVSVideoFrameTransform::DeinterlaceAlloc()
{
	switch (m_lOutColorSpace)
	{
	case CSP_I420:
	case CSP_YV12:
		switch(m_lInpColorSpace)
		{
		case CSP_YV12:
			if (m_bMirror)
				AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			break;
		case CSP_YUY2:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_BGRA:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_BGR:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		}
		break;

	case CSP_YUY2:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			AllocBuffer( &m_oVideoBuffer1, &m_oSrcFormat,  m_oSrcFormat.ColorSpace );
			break;
		case CSP_YUY2:
			if (m_bMirror)
			   AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			break;
		case CSP_BGRA:
		case CSP_BGR:	
			AllocBuffer( &m_oVideoBuffer1, &m_oSrcFormat, CSP_YV12 );
			AllocBuffer( &m_oVideoBuffer2, &m_oSrcFormat, CSP_YV12 );
			break;
		}
		break;

	case CSP_BGR:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			break;

		case CSP_YUY2:
            AllocBuffer(&m_oVideoBuffer1, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, CSP_YV12);
			break;

		case CSP_BGRA:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			break;
		case CSP_BGR:
			if (m_bMirror)
				AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			break;
		}
		break;

	case CSP_BGRA:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			break;

		case CSP_YUY2:
            AllocBuffer(&m_oVideoBuffer1, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, CSP_YV12);
			break;

		case CSP_BGRA:
			if (m_bMirror)
				AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			break;
		case CSP_BGR:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			break;
		}
		break;
	}
}
void CAVSVideoFrameTransform::ResizeAlloc()
{
	switch (m_lOutColorSpace)
	{
	case CSP_I420:
	case CSP_YV12:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			if (m_bMirror)
				AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_YUY2:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_BGRA:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_BGR:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		}
		break;
	case CSP_YUY2:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			AllocBuffer(&m_oVideoBuffer1, &m_oDstFormat, CSP_YV12);
			break;
		case CSP_YUY2:
			if(m_bMirror)
				AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oSrcFormat.ColorSpace);
			break;
		case CSP_BGRA:
		case CSP_BGR:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, CSP_YV12);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, CSP_YV12);
			break;
		}
		break;
	case CSP_BGR:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			AllocBuffer(&m_oVideoBuffer1, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			break;

		case CSP_YUY2:
            AllocBuffer(&m_oVideoBuffer1, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, CSP_YV12);
			break;

		case CSP_BGRA:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_BGR:
			if (m_bMirror)
				AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		}
		break;

	case CSP_BGRA:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			AllocBuffer(&m_oVideoBuffer1, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			break;

		case CSP_YUY2:
            AllocBuffer(&m_oVideoBuffer1, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			AllocBuffer(&m_oVideoBuffer2, &m_oDstFormat, CSP_YV12);
			break;

		case CSP_BGRA:
			if (m_bMirror)
				AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, m_oDstFormat.ColorSpace);
			break;
		case CSP_BGR:
			AllocBuffer(&m_oVideoBuffer1, &m_oDstFormat, m_oSrcFormat.ColorSpace);
			break;
		}
		break;
	}
}
void CAVSVideoFrameTransform::SimpleAlloc()
{
	switch (m_lOutColorSpace)
	{
	case CSP_I420:
	case CSP_YV12:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			break;
		case CSP_YUY2:
			break;
		case CSP_BGRA:
			break;
		case CSP_BGR:
			break;
		}
		break;

	case CSP_YUY2:
		switch(m_lInpColorSpace)
		{
               case CSP_YUY2:
                        break;
		case CSP_I420:
		case CSP_YV12: 
		case CSP_BGRA:
		case CSP_BGR:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, CSP_YV12);
			break;
		}
		break;

	case CSP_BGR:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			break;
		case CSP_YUY2:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, CSP_I420);
			break;
		case CSP_BGRA:
			break;
		case CSP_BGR:
			break;
		}
		break;

	case CSP_BGRA:
		switch(m_lInpColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			break;
		case CSP_YUY2:
			AllocBuffer(&m_oVideoBuffer1, &m_oSrcFormat, CSP_I420);
			break;
		case CSP_BGRA:
			break;
		case CSP_BGR:
			break;
		}
		break;
	}
}
void CAVSVideoFrameTransform::ErrorAlloc()
{
	//TODO отсутствует в Димином коде
}

//	 
void __fastcall CAVSVideoFrameTransform::RGB32Resize(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame)
{
	long lDstWidth		= (long)(pInpFrame->Width*pOutFrame->kx);	
	long lDstHeight		= (long)(pInpFrame->Height*pOutFrame->ky);
	long lDstLeft		= (pOutFrame->Width - lDstWidth)/2;
	long lDstTop		= (pOutFrame->Height - lDstHeight)/2;

	if (m_bFillBackground)
	{
		//TODO надо поправить аналогично тому как сделано YUV
		//m_dwBGRDstBorderColor
		long lStride = pOutFrame->Stride[0];
		LPBYTE pTemp = new BYTE[lStride];
		if (NULL!=pTemp)
		{
			long lColCount = pOutFrame->Stride[0]/4;
			LPDWORD pTempPtr = (LPDWORD)pTemp;
			DWORD dwTemp =	 (0xFF & (m_dwBGRDstBorderColor>>16))	| 
							 (0x00FF00 & m_dwBGRDstBorderColor)		|
							((0xFF & m_dwBGRDstBorderColor)<<16)	|
							 0xFF000000;//Добавлено по приказу СА
			for (long lCol = 0; lCol < lColCount; lCol++, pTempPtr++)
			{
				*pTempPtr = dwTemp;
			}

			LPBYTE pRowPtr = pOutFrame->Plane[0];
			for (long lRow = 0; lRow < pOutFrame->Height; lRow++, pRowPtr += lStride)
			{
				memcpy(pRowPtr, pTemp, lStride);
			}
			delete []pTemp;
		}
	}

	IppiSize	SizeIn={pInpFrame->Width, pInpFrame->Height};
	IppiSize	DstROI={min(lDstWidth, pOutFrame->Width), min(lDstHeight, pOutFrame->Height)};
	IppiRect	SrcROI={0, 0, pInpFrame->Width, pInpFrame->Height};

	Ipp8u* pDst = (Ipp8u*)pOutFrame->Plane[0];
	Ipp8u* pSrc = (Ipp8u*)pInpFrame->Plane[0];
	if (lDstLeft>0)
		pDst += 4*lDstLeft;
	else
		pSrc += 4*((long)((double)-lDstLeft/pOutFrame->kx));
	if (lDstTop>0)
		pDst += lDstTop*pOutFrame->Stride[0];
	else
		pSrc += ((((long)((double)-lDstTop/pOutFrame->ky))>>1)<<1)*pInpFrame->Stride[0];

	const bool cbSizeUp = (pOutFrame->Width > pInpFrame->Width) || (pOutFrame->Height > pInpFrame->Height);

	if (IPPI_INTER_SUPER == m_arlResizeModes[m_lResizeMode] && cbSizeUp)
	{
		ippiResize_8u_C4R(	(Ipp8u*)pInpFrame->Plane[0], SizeIn, pInpFrame->Stride[0], SrcROI, 
			pDst, pOutFrame->Stride[0], DstROI,
			pOutFrame->kx, pOutFrame->ky, IPPI_INTER_CUBIC);		
	}
	else
	{
		
		ippiResize_8u_C4R(	(Ipp8u*)pInpFrame->Plane[0], SizeIn, pInpFrame->Stride[0], SrcROI, 
			pDst, pOutFrame->Stride[0], DstROI,
			pOutFrame->kx, pOutFrame->ky, m_arlResizeModes[m_lResizeMode]);

	}

	return;
/*
	if (m_bFillBackground)
	{
		//TODO надо поправить аналогично тому как сделано YUV
		//m_dwBGRDstBorderColor
		long lStride = pOutFrame->Stride[0];
		LPBYTE pTemp = new BYTE[lStride];
		if (NULL!=pTemp)
		{
			long lColCount = pOutFrame->Stride[0]/4;
			LPDWORD pTempPtr = (LPDWORD)pTemp;
			DWORD dwTemp =	 (0xFF & (m_dwBGRDstBorderColor>>16))	| 
							 (0x00FF00 & m_dwBGRDstBorderColor)		|
							((0xFF & m_dwBGRDstBorderColor)<<16);
			for (long lCol = 0; lCol < lColCount; lCol++, pTempPtr++)
			{
				*pTempPtr = dwTemp;
			}

			LPBYTE pRowPtr = pOutFrame->Plane[0];
			for (long lRow = 0; lRow < pOutFrame->Height; lRow++, pRowPtr += lStride)
			{
				memcpy(pRowPtr, pTemp, lStride);
			}
			delete []pTemp;
		}
		//memset(pOutFrame->Plane[0], m_nRGBDstBorderGrayScale, pOutFrame->Stride[0]*pOutFrame->Height);
	}

	IppiRect	SrcROI={0, 0, pInpFrame->Width, pInpFrame->Height};
	IppiSize	SizeIn={pInpFrame->Width, pInpFrame->Height};
	IppiSize	DstROI={pOutFrame->Width, pOutFrame->Height};
	
	ippiResizeCenter_8u_AC4R((Ipp8u*)pInpFrame->Plane[0], SizeIn, pInpFrame->Stride[0], SrcROI,
							(Ipp8u*)pOutFrame->Plane[0], pOutFrame->Stride[0], DstROI,
							pOutFrame->kx, pOutFrame->ky,
							(double)pInpFrame->Width/2, (double)pInpFrame->Height/2, m_arlResizeModes[m_lResizeMode]);
*/
}
void __fastcall CAVSVideoFrameTransform::RGB24Resize(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame)
{
	long lDstWidth		= (long)(pInpFrame->Width*pOutFrame->kx);	
	long lDstHeight		= (long)(pInpFrame->Height*pOutFrame->ky);
	long lDstLeft		= (pOutFrame->Width - lDstWidth)/2;
	long lDstTop		= (pOutFrame->Height - lDstHeight)/2;

	if (m_bFillBackground)
	{
		//TODO надо поправить аналогично тому как сделано YUV
		//m_dwBGRDstBorderColor
		BYTE nB = (BYTE)((m_dwBGRDstBorderColor>>16) & 0xFF);
		BYTE nG = (BYTE)((m_dwBGRDstBorderColor>>8) & 0xFF);
		BYTE nR = (BYTE)((m_dwBGRDstBorderColor) & 0xFF);
		long lStride = pOutFrame->Stride[0];
		LPBYTE pTemp = new BYTE[lStride];
		if (NULL!=pTemp)
		{
			long lColCount = pOutFrame->Stride[0]/3;
			LPBYTE pTempPtr = (LPBYTE)pTemp;
			for (long lCol = 0; lCol < lColCount; lCol++)
			{
				*pTempPtr = nB; pTempPtr++;
				*pTempPtr = nG; pTempPtr++;
				*pTempPtr = nR; pTempPtr++;
			}

			LPBYTE pRowPtr = pOutFrame->Plane[0];
			for (long lRow = 0; lRow < pOutFrame->Height; lRow++, pRowPtr += lStride)
			{
				memcpy(pRowPtr, pTemp, lStride);
			}
			delete []pTemp;
		}
	}

	IppiSize	SizeIn={pInpFrame->Width, pInpFrame->Height};
	IppiSize	DstROI={min(lDstWidth, pOutFrame->Width), min(lDstHeight, pOutFrame->Height)};
	IppiRect	SrcROI={0, 0, pInpFrame->Width, pInpFrame->Height};

	Ipp8u* pDst = (Ipp8u*)pOutFrame->Plane[0];
	Ipp8u* pSrc = (Ipp8u*)pInpFrame->Plane[0];
	if (lDstLeft>0)
		pDst += 3*lDstLeft;
	else
		pSrc += 3*((long)((double)-lDstLeft/pOutFrame->kx));
	if (lDstTop>0)
		pDst += lDstTop*pOutFrame->Stride[0];
	else
		pSrc += ((((long)((double)-lDstTop/pOutFrame->ky))>>1)<<1)*pInpFrame->Stride[0];

	

	const bool cbSizeUp = (pOutFrame->Width > pInpFrame->Width) || (pOutFrame->Height > pInpFrame->Height);

	if (IPPI_INTER_SUPER == m_arlResizeModes[m_lResizeMode] && cbSizeUp)
	{
		ippiResize_8u_C3R(	pSrc, SizeIn, pInpFrame->Stride[0], SrcROI, 
			pDst, pOutFrame->Stride[0], DstROI,
			pOutFrame->kx, pOutFrame->ky, IPPI_INTER_CUBIC);
	}
	else
	{
		ippiResize_8u_C3R(	pSrc, SizeIn, pInpFrame->Stride[0], SrcROI, 
			pDst, pOutFrame->Stride[0], DstROI,
			pOutFrame->kx, pOutFrame->ky, m_arlResizeModes[m_lResizeMode]);

	}

	return;
/*
	if (m_bFillBackground)
	{
		//TODO надо поправить аналогично тому как сделано YUV
		//m_dwBGRDstBorderColor
		long lStride = pOutFrame->Stride[0];
		LPBYTE pTemp = new BYTE[lStride];
		if (NULL!=pTemp)
		{
			long lColCount = pOutFrame->Stride[0]/3;
			LPBYTE pTempPtr = pTemp;
			BYTE nRColor = (BYTE)(0xFF & m_dwBGRDstBorderColor);
			BYTE nGColor = (BYTE)(0xFF & (m_dwBGRDstBorderColor >> 8));
			BYTE nBColor = (BYTE)(0xFF & (m_dwBGRDstBorderColor >> 16));
			for (long lCol = 0; lCol < lColCount; lCol++)
			{
				*pTempPtr = nBColor; pTempPtr++;
				*pTempPtr = nGColor; pTempPtr++;
				*pTempPtr = nRColor; pTempPtr++;
			}

			LPBYTE pRowPtr = pOutFrame->Plane[0];
			for (long lRow = 0; lRow < pOutFrame->Height; lRow++, pRowPtr += lStride)
			{
				memcpy(pRowPtr, pTemp, lStride);
			}
			delete []pTemp;
		}

		//memset(pOutFrame->Plane[0], m_nRGBDstBorderGrayScale, pOutFrame->Stride[0]*pOutFrame->Height);
	}

	IppiRect	SrcROI={0, 0, pInpFrame->Width, pInpFrame->Height};
	IppiSize	SizeIn={pInpFrame->Width, pInpFrame->Height};
	IppiSize	DstROI={pOutFrame->Width, pOutFrame->Height};

	//ATLTRACE("InpFramePtr: 0x%08X, InpFrameW: %d, InpFrameH: %d, InpFrameStride: %d\n",
	//	pInpFrame->Plane[0], pInpFrame->Width, pInpFrame->Height, pInpFrame->Stride[0]);
	//ATLTRACE("OutFramePtr: 0x%08X, OutFrameW: %d, OutFrameH: %d, OutFrameStride: %d\n",
	//	pOutFrame->Plane[0], pOutFrame->Width, pOutFrame->Height, pOutFrame->Stride[0]);
	//ATLTRACE("OutFrameKX: %f, OutFrameKY: %f\n", pOutFrame->kx, pOutFrame->ky);

	ippiResizeCenter_8u_C3R((Ipp8u*)pInpFrame->Plane[0], SizeIn, pInpFrame->Stride[0], SrcROI,
							(Ipp8u*)pOutFrame->Plane[0], pOutFrame->Stride[0], DstROI,
							pOutFrame->kx, pOutFrame->ky,
							(double)pInpFrame->Width/2, (double)pInpFrame->Height/2, m_arlResizeModes[m_lResizeMode]);
*/
}
void __fastcall CAVSVideoFrameTransform::YV12Resize(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame)
{
	long lYDstWidth		= (((long)(pInpFrame->Width*pOutFrame->kx))>>1)<<1;	
	long lYDstHeight	= (((long)(pInpFrame->Height*pOutFrame->ky))>>1)<<1;
	long lYDstLeft		= (((pOutFrame->Width - lYDstWidth)/2)>>1)<<1;
	long lYDstTop		= (((pOutFrame->Height - lYDstHeight)/2)>>1)<<1;

	long lUVDstLeft		= lYDstLeft/2;
	long lUVDstWidth	= lYDstWidth/2;
	long lUVDstTop		= lYDstTop/2;
	long lUVDstHeight	= lYDstHeight/2;

	if (m_bFillBackground)
	{
		if (lYDstTop>0)
		{
			//memset(pOutFrame->Plane[0], m_nYDstBorderGrayScale, pOutFrame->Stride[0]*lYDstTop);
			//memset(pOutFrame->Plane[1], m_nUDstBorderGrayScale, pOutFrame->Stride[1]*lUVDstTop);
			//memset(pOutFrame->Plane[2], m_nVDstBorderGrayScale, pOutFrame->Stride[2]*lUVDstTop);
			memset(pOutFrame->Plane[0], m_nYDstBorderColor, pOutFrame->Stride[0]*lYDstTop);
			memset(pOutFrame->Plane[1], m_nUDstBorderColor, pOutFrame->Stride[1]*lUVDstTop);
			memset(pOutFrame->Plane[2], m_nVDstBorderColor, pOutFrame->Stride[2]*lUVDstTop);
		}
		if (lYDstTop+lYDstHeight<pOutFrame->Height)
		{
			memset(	pOutFrame->Plane[0] + (lYDstTop+lYDstHeight)*pOutFrame->Stride[0], 
					//m_nYDstBorderGrayScale, 
					m_nYDstBorderColor,
					pOutFrame->Stride[0]*(pOutFrame->Height - lYDstTop - lYDstHeight));
			memset(	pOutFrame->Plane[1] + (lUVDstTop+lUVDstHeight)*pOutFrame->Stride[1], 
					//m_nUDstBorderGrayScale, 
					m_nUDstBorderColor, 
					pOutFrame->Stride[1]*(pOutFrame->Height/2 - lUVDstTop - lUVDstHeight));
			memset(	pOutFrame->Plane[2] + (lUVDstTop+lUVDstHeight)*pOutFrame->Stride[2],
					//m_nVDstBorderGrayScale, 
					m_nVDstBorderColor, 
					pOutFrame->Stride[2]*(pOutFrame->Height/2 - lUVDstTop - lUVDstHeight));
		}
		if (lYDstLeft>0)
		{
			LPBYTE pDstY = pOutFrame->Plane[0];
			if (lYDstTop>0)
				pDstY += lYDstTop * pOutFrame->Stride[0];
			long lTemp = min(lYDstHeight, pOutFrame->Height);
			for (long lRow = 0; lRow<lTemp; lRow++, pDstY+=pOutFrame->Stride[0])
			{
				//memset(pDstY, m_nYDstBorderGrayScale, lYDstLeft);
				memset(pDstY, m_nYDstBorderColor, lYDstLeft);
			}
			LPBYTE pDstU = pOutFrame->Plane[1];
			LPBYTE pDstV = pOutFrame->Plane[2];
			if (lUVDstTop>0)
			{
				pDstU += lUVDstTop * pOutFrame->Stride[1];
				pDstV += lUVDstTop * pOutFrame->Stride[2];
			}
			lTemp = min(lUVDstHeight, pOutFrame->Height/2);
			for (long lRow = 0; lRow<lTemp ; lRow++, pDstU+=pOutFrame->Stride[1], pDstV+=pOutFrame->Stride[2])
			{
				//memset(pDstU, m_nUDstBorderGrayScale, lUVDstLeft);
				//memset(pDstV, m_nVDstBorderGrayScale, lUVDstLeft);
				memset(pDstU, m_nUDstBorderColor, lUVDstLeft);
				memset(pDstV, m_nVDstBorderColor, lUVDstLeft);
			}
		}
		if (lYDstLeft + lYDstWidth<pOutFrame->Width)
		{
			LPBYTE pDstY = pOutFrame->Plane[0] + lYDstLeft + lYDstWidth;
			if (lYDstTop>0)
				pDstY += lYDstTop * pOutFrame->Stride[0];
			long lSize = pOutFrame->Width - (lYDstLeft + lYDstWidth);
			long lTemp = min(lYDstHeight, pOutFrame->Height);
			for (long lRow = 0; lRow<lTemp; lRow++, pDstY+=pOutFrame->Stride[0])
			{
				//memset(pDstY, m_nYDstBorderGrayScale, lSize);
				memset(pDstY, m_nYDstBorderColor, lSize);
			}
			LPBYTE pDstU = pOutFrame->Plane[1] + lUVDstLeft + lUVDstWidth;
			LPBYTE pDstV = pOutFrame->Plane[2] + lUVDstLeft + lUVDstWidth;
			if (lUVDstTop>0)
			{
				pDstU += lUVDstTop * pOutFrame->Stride[1];
				pDstV += lUVDstTop * pOutFrame->Stride[2];
			}
			lSize = pOutFrame->Width/2 - (lUVDstLeft + lUVDstWidth);
			lTemp = min(lUVDstHeight, pOutFrame->Height/2);
			for (long lRow = 0; lRow<lTemp; lRow++, pDstU+=pOutFrame->Stride[1], pDstV+=pOutFrame->Stride[2])
			{
				//memset(pDstU, m_nUDstBorderGrayScale, lSize);
				//memset(pDstV, m_nVDstBorderGrayScale, lSize);
				memset(pDstU, m_nUDstBorderColor, lSize);
				memset(pDstV, m_nVDstBorderColor, lSize);
			}
		}
	}


	IppiSize	SizeIn={pInpFrame->Width, pInpFrame->Height};
	IppiSize	DstROI={min(lYDstWidth, pOutFrame->Width), min(lYDstHeight, pOutFrame->Height)};
	IppiRect	SrcROI={0, 0, pInpFrame->Width, pInpFrame->Height};

	Ipp8u* pDst = (Ipp8u*)pOutFrame->Plane[0];
	Ipp8u* pSrc = (Ipp8u*)pInpFrame->Plane[0];
	
	if (lYDstLeft>0)
		pDst += lYDstLeft;
	else
		pSrc += (((long)((double)-lYDstLeft/pOutFrame->kx))>>1)<<1;
	if (lYDstTop>0)
		pDst += lYDstTop*pOutFrame->Stride[0];
	else
		pSrc += ((((long)((double)-lYDstTop/pOutFrame->ky))>>1)<<1)*pInpFrame->Stride[0];
	if ((0==SrcROI.width) || (0==SrcROI.height))
		return;
	
	const bool cbSizeUp = (pOutFrame->Width > pInpFrame->Width) || (pOutFrame->Height > pInpFrame->Height);

	if (IPPI_INTER_SUPER == m_arlResizeModes[m_lResizeMode] && cbSizeUp)
	{
		ippiResize_8u_C1R(	pSrc, SizeIn, pInpFrame->Stride[0], SrcROI, 
			pDst, pOutFrame->Stride[0], DstROI,
			pOutFrame->kx, pOutFrame->ky, IPPI_INTER_CUBIC);
	}
	else
	{
		ippiResize_8u_C1R(	pSrc, SizeIn, pInpFrame->Stride[0], SrcROI, 
			pDst, pOutFrame->Stride[0], DstROI,
			pOutFrame->kx, pOutFrame->ky, m_arlResizeModes[m_lResizeMode]);

	}

	

	SrcROI.x/=2;
	SrcROI.y/=2;
	SrcROI.width/=2;
	SrcROI.height/=2;
	SizeIn.width/=2;
	SizeIn.height/=2;
	DstROI.width/=2;
	DstROI.height/=2;

	if ((0==SrcROI.width) || (0==SrcROI.height))
		return;
	pDst = (Ipp8u*)pOutFrame->Plane[1];
	pSrc = (Ipp8u*)pInpFrame->Plane[1];
	if (lUVDstLeft>0)
		pDst += (lYDstLeft/2);
	else
		pSrc += (((long)((double)-lYDstLeft/pOutFrame->kx))>>1);
	if (lUVDstTop>0)
		pDst += (lYDstTop/2)*pOutFrame->Stride[1];
	else
		pSrc += ((((long)((double)-lYDstTop/pOutFrame->ky))>>1) * pInpFrame->Stride[1]);
	
	if (IPPI_INTER_SUPER == m_arlResizeModes[m_lResizeMode] && cbSizeUp)
	{
		ippiResize_8u_C1R(	pSrc, SizeIn, pInpFrame->Stride[1], SrcROI, 
			pDst, pOutFrame->Stride[1], DstROI,
			pOutFrame->kx, pOutFrame->ky, IPPI_INTER_CUBIC);
	}
	else
	{
		ippiResize_8u_C1R(	pSrc, SizeIn, pInpFrame->Stride[1], SrcROI, 
			pDst, pOutFrame->Stride[1], DstROI,
			pOutFrame->kx, pOutFrame->ky, m_arlResizeModes[m_lResizeMode]);
	}

	pDst = (Ipp8u*)pOutFrame->Plane[2];
	pSrc = (Ipp8u*)pInpFrame->Plane[2];
	if (lUVDstLeft>0)
		pDst += lUVDstLeft;
	else
		pSrc += (((long)((double)-lYDstLeft/pOutFrame->kx))>>1);
	if (lUVDstTop>0)
		pDst += lUVDstTop*pOutFrame->Stride[2];
	else
		pSrc += (((long)((double)-lYDstTop/pOutFrame->ky))>>1) * pInpFrame->Stride[2];


	if (IPPI_INTER_SUPER == m_arlResizeModes[m_lResizeMode] && cbSizeUp)
	{
		ippiResize_8u_C1R(	pSrc, SizeIn, pInpFrame->Stride[2], SrcROI, 
			pDst, pOutFrame->Stride[2], DstROI,
			pOutFrame->kx, pOutFrame->ky, IPPI_INTER_CUBIC);
	}
	else
	{
		ippiResize_8u_C1R(	pSrc, SizeIn, pInpFrame->Stride[2], SrcROI, 
			pDst, pOutFrame->Stride[2], DstROI,
			pOutFrame->kx, pOutFrame->ky, m_arlResizeModes[m_lResizeMode]);
	}
/*
	if (m_bFillBackground)
	{
		memset(pOutFrame->Plane[0], m_nYDstBorderGrayScale, pOutFrame->Stride[0]*pOutFrame->Height);
		memset(pOutFrame->Plane[1], m_nUDstBorderGrayScale, pOutFrame->Stride[1]*pOutFrame->Height/2);
		memset(pOutFrame->Plane[2], m_nVDstBorderGrayScale, pOutFrame->Stride[2]*pOutFrame->Height/2);
	}

	IppiRect	SrcROI={0, 0, pInpFrame->Width, pInpFrame->Height};
	IppiSize	SizeIn={pInpFrame->Width, pInpFrame->Height};
	IppiSize	DstROI={pOutFrame->Width, pOutFrame->Height};

	ippiResizeCenter_8u_C1R((Ipp8u*)pInpFrame->Plane[0], SizeIn, pInpFrame->Stride[0], SrcROI,
							(Ipp8u*)pOutFrame->Plane[0], pOutFrame->Stride[0], DstROI,
							pOutFrame->kx, pOutFrame->ky,
							(double)pInpFrame->Width/2, (double)pInpFrame->Height/2, m_arlResizeModes[m_lResizeMode]);
	SrcROI.width/=2;
	SrcROI.height/=2;
	SizeIn.width/=2;
	SizeIn.height/=2;
	DstROI.width/=2;
	DstROI.height/=2;

	ippiResizeCenter_8u_C1R((Ipp8u*)pInpFrame->Plane[1], SizeIn, pInpFrame->Stride[1], SrcROI,
							(Ipp8u*)pOutFrame->Plane[1], pOutFrame->Stride[1], DstROI,
							pOutFrame->kx, pOutFrame->ky,
							(double)pInpFrame->Width/4, (double)pInpFrame->Height/4, m_arlResizeModes[m_lResizeMode]);

	ippiResizeCenter_8u_C1R((Ipp8u*)pInpFrame->Plane[2], SizeIn, pInpFrame->Stride[2], SrcROI,
							(Ipp8u*)pOutFrame->Plane[2], pOutFrame->Stride[2], DstROI,
							pOutFrame->kx, pOutFrame->ky,
							(double)pInpFrame->Width/4, (double)pInpFrame->Height/4, m_arlResizeModes[m_lResizeMode]);

*/
}
void __fastcall CAVSVideoFrameTransform::YUY2Resize(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame)
{   
	if (m_bFillBackground)
	{
		//memset(pOutFrame->Plane[0], m_nRGBDstBorderGrayScale, pOutFrame->Stride[0]*pOutFrame->Height);
		//TODO надо поправить аналогично тому как сделано YUV
		long lColCount = pOutFrame->Width/2;
		long lRowCount = pOutFrame->Height;
		long lPitch = pOutFrame->Stride[0] - pOutFrame->Width*2;
		LPBYTE pTemp = pOutFrame->Plane[0];
		for (long lRowIndex = 0; lRowIndex<lRowCount; lRowIndex++)
		{
			for (long lColIndex = 0; lColIndex<lColCount; lColIndex++)
			{
				*pTemp = m_nYDstBorderColor; pTemp++;
				*pTemp = m_nUDstBorderColor; pTemp++;
				*pTemp = m_nYDstBorderColor; pTemp++;
				*pTemp = m_nVDstBorderColor; pTemp++;
			}
			pTemp += lPitch;
		}
	}

	IppiRect	SrcROI={0, 0, pInpFrame->Width/2, pInpFrame->Height};
	IppiSize	SizeIn={pInpFrame->Width/2, pInpFrame->Height};
	IppiSize	DstROI={pOutFrame->Width/2, pOutFrame->Height};

	const bool cbSizeUp = (pOutFrame->Width > pInpFrame->Width) || (pOutFrame->Height > pInpFrame->Height);
	
	if (IPPI_INTER_SUPER == m_arlResizeModes[m_lResizeMode] && cbSizeUp)
	{
		ippiResizeCenter_8u_C4R((Ipp8u*)pInpFrame->Plane[0], SizeIn, pInpFrame->Stride[0], SrcROI,
			(Ipp8u*)pOutFrame->Plane[0], pOutFrame->Stride[0], DstROI,
			pOutFrame->kx, pOutFrame->ky,
			(double)pInpFrame->Width/4, (double)pInpFrame->Height/2, IPPI_INTER_CUBIC);
	}
	else
	{
		ippiResizeCenter_8u_C4R((Ipp8u*)pInpFrame->Plane[0], SizeIn, pInpFrame->Stride[0], SrcROI,
			(Ipp8u*)pOutFrame->Plane[0], pOutFrame->Stride[0], DstROI,
			pOutFrame->kx, pOutFrame->ky,
			(double)pInpFrame->Width/4, (double)pInpFrame->Height/2, m_arlResizeModes[m_lResizeMode]);
	}

	
	return;
}

void __fastcall CAVSVideoFrameTransform::RGB32ToRGB24(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame, bool m_bMirror)
{
	BYTE*	pDst=(BYTE*)pOutFrame->Plane[0];
	BYTE*	pSrc=(BYTE*)pInpFrame->Plane[0];

	BYTE*	pDstPos;
	BYTE*	pSrcPos;

	if (m_bMirror)
	{
		pDst+=(pOutFrame->Height-1)*pOutFrame->Stride[0];
		for (int i=0; i<pOutFrame->Height;i++)
		{
			pDstPos = pDst;
			pSrcPos = pSrc;
			for (int j=0; j<pOutFrame->Width-1; j++)
			{
				*(DWORD*)pDstPos=*(DWORD*)pSrcPos;
				pSrcPos+=4;
				pDstPos+=3;
			}
			*pDstPos=*pSrcPos;
			*(pDstPos+1)=*(pSrcPos+1);
			*(pDstPos+2)=*(pSrcPos+2);
			pSrc += pInpFrame->Stride[0];
			pDst -= pOutFrame->Stride[0];
		}
	}
	else
	{
		for (int i=0; i<pOutFrame->Height;i++)
		{
			pDstPos = pDst;
			pSrcPos = pSrc;
			for (int j=0; j<pOutFrame->Width-1; j++)
			{
				*(DWORD*)pDstPos=*(DWORD*)pSrcPos;
				pSrcPos+=4;
				pDstPos+=3;
			}
			*pDstPos=*pSrcPos;
			*(pDstPos+1) = *(pSrcPos+1);
			*(pDstPos+2) = *(pSrcPos+2);
			pSrc+= pInpFrame->Stride[0];
			pDst+= pOutFrame->Stride[0];
		}
	}
}

static _inline int GetBitValue( int n, int bitPosition )
{
   return ((n >> bitPosition) & 1);
}

void __fastcall CAVSVideoFrameTransform::RGB8ToRGB24(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame, bool m_bMirror)
{
	//Bit    7  6  5  4  3  2  1  0
	//Data   R  R  R  G  G  G  B  B

	BYTE*	pDst=(BYTE*)pOutFrame->Plane[0];
	BYTE*	pSrc=(BYTE*)pInpFrame->Plane[0];

	BYTE*	pDstPos;
	BYTE*	pSrcPos;
	if (m_bMirror)
	{
		pDst+=(pOutFrame->Height-1)*pOutFrame->Stride[0];
		for (int i=0; i<pOutFrame->Height;i++)
		{
			pDstPos = pDst;
			pSrcPos = pSrc;
			for (int j=0; j<pOutFrame->Width/*-1*/; j++)
			{
				pDstPos[0] = ((*pSrcPos) >> 5) * 32;
				pDstPos[1] = (((*pSrcPos) >> 2) << 3) * 32;
				pDstPos[2] = ((*pSrcPos) << 6) * 64;

				pSrcPos+=1;
				pDstPos+=3;
			}
			//*pDstPos = *pSrcPos;
			//*(pDstPos+1) = *(pSrcPos+1);
			//*(pDstPos+2) = *(pSrcPos+2);
			//*(pDstPos+3) = 0;
			pSrc += pInpFrame->Stride[0];
			pDst -= pOutFrame->Stride[0];
		}
	} else
	{
		for (int i=0; i<pOutFrame->Height;i++)
		{
			pDstPos = pDst;
			pSrcPos = pSrc;
			for (int j=0; j<pOutFrame->Width/*-1*/; j++)
			{
				pDstPos[0] = ((*pSrcPos) >> 5) * 32;
				pDstPos[1] = (((*pSrcPos) >> 2) << 3) * 32;
				pDstPos[2] = ((*pSrcPos) << 6) * 64;

				pSrcPos+=1;
				pDstPos+=3;
			}
			//*pDstPos=*pSrcPos;
			//*(pDstPos+1)=*(pSrcPos+1);
			//*(pDstPos+2)=*(pSrcPos+2);
			//*(pDstPos+3)=0;
			pSrc+=pInpFrame->Stride[0];
			pDst+=pOutFrame->Stride[0];
		}
	}
}
void __fastcall CAVSVideoFrameTransform::RGB8ToRGB32(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame, bool m_bMirror)
{
	BYTE*	pDst=(BYTE*)pOutFrame->Plane[0];
	BYTE*	pSrc=(BYTE*)pInpFrame->Plane[0];

	BYTE*	pDstPos;
	BYTE*	pSrcPos;
	if (m_bMirror)
	{
		pDst+=(pOutFrame->Height-1)*pOutFrame->Stride[0];
		for (int i=0; i<pOutFrame->Height;i++)
		{
			pDstPos = pDst;
			pSrcPos = pSrc;
			for (int j=0; j<pOutFrame->Width/*-1*/; j++)
			{
				pDstPos[0] = ((*pSrcPos) >> 5) * 32;
				pDstPos[1] = (((*pSrcPos) >> 2) << 3) * 32;
				pDstPos[2] = ((*pSrcPos) << 6) * 64;

				pSrcPos+=1;
				pDstPos+=4;
			}
			//*pDstPos = *pSrcPos;
			//*(pDstPos+1) = *(pSrcPos+1);
			//*(pDstPos+2) = *(pSrcPos+2);
			//*(pDstPos+3) = 0;
			pSrc += pInpFrame->Stride[0];
			pDst -= pOutFrame->Stride[0];
		}
	} else
	{
		for (int i=0; i<pOutFrame->Height;i++)
		{
			pDstPos = pDst;
			pSrcPos = pSrc;
			for (int j=0; j<pOutFrame->Width/*-1*/; j++)
			{
				pDstPos[0] = ((*pSrcPos) >> 5) * 32;
				pDstPos[1] = (((*pSrcPos) >> 2) << 3) * 32;
				pDstPos[2] = ((*pSrcPos) << 6) * 64;

				pSrcPos+=1;
				pDstPos+=4;
			}
			//*pDstPos=*pSrcPos;
			//*(pDstPos+1)=*(pSrcPos+1);
			//*(pDstPos+2)=*(pSrcPos+2);
			//*(pDstPos+3)=0;
			pSrc+=pInpFrame->Stride[0];
			pDst+=pOutFrame->Stride[0];
		}
	}
}
void __fastcall CAVSVideoFrameTransform::RGB24ToRGB32(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame, bool m_bMirror)
{
	BYTE*	pDst=(BYTE*)pOutFrame->Plane[0];
	BYTE*	pSrc=(BYTE*)pInpFrame->Plane[0];

	BYTE*	pDstPos;
	BYTE*	pSrcPos;
	if (m_bMirror)
	{
		pDst+=(pOutFrame->Height-1)*pOutFrame->Stride[0];
		for (int i=0; i<pOutFrame->Height;i++)
		{
			pDstPos = pDst;
			pSrcPos = pSrc;
			for (int j=0; j<pOutFrame->Width-1; j++)
			{
				*(DWORD*)pDstPos=*(DWORD*)pSrcPos;
				*(DWORD*)pDstPos&=0x00FFFFFF;
				pSrcPos+=3;
				pDstPos+=4;
			}
			*pDstPos = *pSrcPos;
			*(pDstPos+1) = *(pSrcPos+1);
			*(pDstPos+2) = *(pSrcPos+2);
			*(pDstPos+3) = 0;
			pSrc += pInpFrame->Stride[0];
			pDst -= pOutFrame->Stride[0];
		}
	} else
	{
		for (int i=0; i<pOutFrame->Height;i++)
		{
			pDstPos = pDst;
			pSrcPos = pSrc;
			for (int j=0; j<pOutFrame->Width-1; j++)
			{
				*(DWORD*)pDstPos=*(DWORD*)pSrcPos;
				*(DWORD*)pDstPos&=0x00FFFFFF;
				pSrcPos+=3;
				pDstPos+=4;
			}
			*pDstPos=*pSrcPos;
			*(pDstPos+1)=*(pSrcPos+1);
			*(pDstPos+2)=*(pSrcPos+2);
			*(pDstPos+3)=0;
			pSrc+=pInpFrame->Stride[0];
			pDst+=pOutFrame->Stride[0];
		}
	}
}
//				
void __fastcall CAVSVideoFrameTransform::RGB32Deinterlace(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame)
{
	switch (m_lDeinterlaceMethod)
	{
	case DEINTERLACE_METHOD_BLEND:
		fastconvert_RGB32BlendDeinterlace(pInpFrame, pOutFrame);
		break;
	case DEINTERLACE_METHOD_MIDDLE:
		fastconvert_RGB32MiddleDeinterlace(pInpFrame, pOutFrame);
		break;
	}
	fastconvert_emms();
}
void __fastcall CAVSVideoFrameTransform::RGB24Deinterlace(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame)
{
	switch (m_lDeinterlaceMethod)
	{
	case DEINTERLACE_METHOD_BLEND:
		fastconvert_RGB24BlendDeinterlace(pInpFrame, pOutFrame);
		break;
	case DEINTERLACE_METHOD_MIDDLE:
		fastconvert_RGB24MiddleDeinterlace(pInpFrame, pOutFrame);
		break;
	}
	fastconvert_emms();
}
void __fastcall CAVSVideoFrameTransform::YV12Deinterlace(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame)
{
	switch (m_lDeinterlaceMethod)
	{
	case DEINTERLACE_METHOD_BLEND:
		fastconvert_YV12BlendDeinterlace(pInpFrame, pOutFrame);
		break;
	case DEINTERLACE_METHOD_MIDDLE:
		fastconvert_YV12MiddleDeinterlace(pInpFrame, pOutFrame);
		break;
	}
	fastconvert_emms();
}
void __fastcall CAVSVideoFrameTransform::YUY2Deinterlace(SUncompressedVideoFrame* pInpFrame, SUncompressedVideoFrame* pOutFrame)
{
	switch (m_lDeinterlaceMethod)
	{
	case DEINTERLACE_METHOD_BLEND:
		fastconvert_YUY2BlendDeinterlace(pInpFrame, pOutFrame);
		break;
	case DEINTERLACE_METHOD_MIDDLE:
		fastconvert_YUY2MiddleDeinterlace(pInpFrame, pOutFrame);
		break;
	}
	fastconvert_emms();
}
