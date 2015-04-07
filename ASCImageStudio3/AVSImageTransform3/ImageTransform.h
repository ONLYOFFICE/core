#pragma once

#include "resource.h"
#include <Registration.h>
#include <GdiPlusEx.h>
#include <ImageColors.h>
#include <ImageCore.h>
#include <TransformConstants.h>

[object, uuid("C7B357DA-00CB-4920-88D4-37AE40260A00"), oleautomation, dual, pointer_default(unique)]
__interface IImageTransform3 : IDispatch
{
	[id(101)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(102)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(201)] HRESULT SetSafeArray([in, satype("BYTE")] SAFEARRAY** Array);
	[id(202)] HRESULT SetSafeArrayPtr([in] BYTE* SafeArray);
	[id(203)] HRESULT SetMediaData([in] IUnknown* pInterfaceInput, [out] IUnknown** pInterfaceOutput);
	[id(221)] HRESULT GetSafeArray([out, satype("BYTE")] SAFEARRAY** Array);


	[id(305)] HRESULT SetMask([in, satype("BYTE")] SAFEARRAY** Array);
	[id(306)] HRESULT ClearMask(void);

	[id(501), propget] HRESULT UseMask([out, retval] VARIANT_BOOL* Mask);
	[id(501), propput] HRESULT UseMask([in] VARIANT_BOOL Mask);
	[id(502), propput] HRESULT BackColor([in] long Color);
	[id(502), propget] HRESULT BackColor([out, retval] long* Color);
	
	[id(1101)] HRESULT AdjustBrightness([in] double Completeness, [in] long Level);
	[id(1102)] HRESULT AdjustBrightnessEx([in] double Completeness, [in] long LevelR, [in] long LevelG, [in] long LevelB);
	[id(1103)] HRESULT AdjustContrast([in] double Completeness, [in] double Level);
	[id(1104)] HRESULT AdjustGamma([in] double Completeness, [in] double Level);
	[id(1105)] HRESULT AdjustEqualize([in] double Completeness);
	[id(1106)] HRESULT AdjustGrayscale([in] double Completeness, [in] VARIANT_BOOL Desaturate);
	[id(1107)] HRESULT AdjustInvert([in] double Completeness);
	[id(1108)] HRESULT AdjustAutocontrast([in] double Completeness);
	[id(1109)] HRESULT AdjustAutolevels([in] double Completeness);
	[id(1110)] HRESULT AdjustHue([in] double Completeness, [in] double Angle);
	[id(1111)] HRESULT AdjustSaturation([in] double Completeness, [in] double Level);
	[id(1112)] HRESULT AdjustPosterize([in] double Completeness, [in] long Levels);
	[id(1113)] HRESULT AdjustTemperature([in] double Completeness, [in] double Level);
	[id(1114)] HRESULT AdjustColorize([in] double Completeness, [in] long Red, [in] long Green, [in] long Blue);
	[id(1115)] HRESULT AdjustThreshold([in] double Completeness, [in] long Level, [in] VARIANT_BOOL Grayscale);

	[id(1201)] HRESULT EffectBlur([in] double Completeness, [in] double Level);
	[id(1202)] HRESULT EffectSharpen([in] double Completeness, [in] double Level);
	[id(1203)] HRESULT EffectMosaic([in] double Completeness, [in] long Size, [in] VARIANT_BOOL Simple);
	[id(1204)] HRESULT EffectGaussianBlur([in] double Completeness, [in] long Size);
	[id(1205)] HRESULT EffectNoise([in] double Completeness, [in] double Level, [in] VARIANT_BOOL Mono);
	[id(1206)] HRESULT EffectDiffuse([in] double Completeness, [in] double Distance);
	[id(1207)] HRESULT EffectMotionBlur([in] double Completeness, [in] double Angle, [in] double Distance);
	[id(1208)] HRESULT EffectEmboss([in] double Completeness, [in] double Angle, [in] double Distance, [in] double Amount);
	[id(1209)] HRESULT EffectMinimal([in] double Completeness, [in] long Size);
	[id(1210)] HRESULT EffectMaximal([in] double Completeness, [in] long Size);
	[id(1211)] HRESULT EffectDeinterlace([in] double Completeness, [in] VARIANT_BOOL Even, [in] VARIANT_BOOL Interpolate);
	[id(1212)] HRESULT EffectMedian([in] double Completeness, [in] long MedianType);
	[id(2213)] HRESULT EffectChromoKey([in] double Completeness, [in] long Color1, [in] long Color2);
	[id(2214)] HRESULT EffectChromoKey2([in] double Completeness, [in] long Color, [in] long Threshold);
	
	[id(1301)] HRESULT TransformFlip([in] double Completeness, [in] VARIANT_BOOL Vertical);
	[id(1302)] HRESULT TransformPerspective([in] double Completeness, [in] VARIANT_BOOL Vertical, [in] double Angle);
	[id(1303)] HRESULT TransformSkew([in] double Completeness, [in] VARIANT_BOOL Vertical, [in] double Angle);
	[id(1304)] HRESULT TransformShift([in] double Completeness, [in] long X, [in] long Y);
	[id(1305)] HRESULT TransformRotate([in] double Completeness, [in] double Angle);
	[id(1306)] HRESULT TransformResample([in] double Completeness, [in] long Times);
	[id(1307)] HRESULT TransformZoom([in] double Completeness, [in] long Zoom);
	[id(1308)] HRESULT TransformMirror([in] double Completeness, [in] double Level, [in] long Type);
	[id(1309)] HRESULT TransformResize([in, out, satype("BYTE")] SAFEARRAY** Output, [in] long Color, [in] long Type);
	[id(1310)] HRESULT TransformTwirl([in] double Completeness, [in] double Angle, [in] long Degree);
	[id(1311)] HRESULT TransformSphere([in] double Completeness, [in] long CenterX, [in] long CenterY, [in] long RadiusX, [in] long RadiusY, [in] double Degree);
	[id(1312)] HRESULT TransformCylinder([in] double Completeness, [in] long Center, [in] long Radius, [in] double Degree, [in] long Type);
	[id(1313)] HRESULT TransformResizeMedia([in, out] IUnknown** pMediaData, [in] long Color, [in] long Type);
	[id(1314)] HRESULT TransformResizeMedia2([in] long nNewWidth, [in] long nNewHeight, [out] IUnknown** pMediaData, [in] long Color, [in] long Type);
	[id(1315)] HRESULT TransformRotateMedia([in] double Completeness, [in] double Angle, [out] IUnknown** pMediaData);
	
	[id(1400)] HRESULT FilterPrewitt([in] double Completeness, [in] long Type);
	[id(1401)] HRESULT FilterScharr([in] double Completeness, [in] long Type);
	[id(1402)] HRESULT FilterSobel([in] double Completeness, [in] long Type);
	[id(1403)] HRESULT FilterSobelExt([in] double Completeness, [in] long Type);
	[id(1404)] HRESULT FilterRoberts([in] double Completeness, [in] long Type);
	[id(1405)] HRESULT FilterLaplace([in] double Completeness, long Type);
	[id(1406)] HRESULT FilterHipass([in] double Completeness, long Type);
	[id(1407)] HRESULT FilterLowpass([in] double Completeness, long Type);
	[id(1408)] HRESULT FilterBlur([in] double Completeness, long Size);
	[id(1409)] HRESULT FilterCanny([in] double Completeness, [in] double LowThresh, [in] double HighThresh);

	[id(1500)] HRESULT UtilitySwapChannels(BYTE nChannel0, BYTE nChannel1, BYTE nChannel2);

	[id(1601)] HRESULT DrawLine([in] double Completeness, [in] long X1, [in] long Y1, [in] long X2, [in] long Y2, [in] long Color, [in] long Alpha, [in] long Size);
	[id(1602)] HRESULT DrawRectangle([in] double Completeness, [in] long Left, [in] long Top, [in] long Right, [in] long Bottom, [in] long Color, [in] long Alpha, [in] VARIANT_BOOL Solid);
	[id(1603)] HRESULT DrawEllipse([in] double Completeness, [in] long Left, [in] long Top, [in] long Right, [in] long Bottom, [in] long Color, [in] long Alpha, [in] VARIANT_BOOL Solid);
	[id(1605)] HRESULT DrawInvRectangle([in] double Completeness, [in] long Left, [in] long Top, [in] long Right, [in] long Bottom, [in] long Color, [in] long Alpha);
	[id(1606)] HRESULT DrawInvEllipse([in] double Completeness, [in] long Left, [in] long Top, [in] long Right, [in] long Bottom, [in] long Color, [in] long Alpha);
	[id(1607)] HRESULT DrawImage([in] double Completeness, [in] long Left, [in] long Top, [in, satype("BYTE")] SAFEARRAY** Image, [in] long Color, [in] long Alpha);
	[id(1608)] HRESULT DrawImagePtr([in] double Completeness, [in] long Left, [in] long Top, [in] BYTE* Image, [in] long Color, [in] long Alpha);
	[id(1609)] HRESULT DrawText([in] double Completeness, [in] long Left, [in] long Top, [in] BSTR Text, [in] IUnknown* Font, [in] long Color, [in] long Alpha);
	[id(1610)] HRESULT DrawText2([in] double Completeness, [in] long Left, [in] long Top, [in] long Right, [in] long Bottom, [in] BSTR Text, [in] IUnknown* Font, [in] long Color, [in] long Alpha);
	[id(1611)] HRESULT DrawCredits([in] double Completeness, [in] BSTR Text, [in] IUnknown* Font, [in] long Color, [in] long Alpha, [in] long Type);
	[id(1612)] HRESULT DrawBorder([in] double Completeness, [in] long Color, [in] long Size, [in] long Type);
	[id(1613)] HRESULT DrawImageKey([in] double Completeness, [in] long Left, [in] long Top, [in, satype("BYTE")] SAFEARRAY** Image, [in] long Color1, [in] long Color2);
	[id(1614)] HRESULT DrawImageKey2([in] double Completeness, [in] long Left, [in] long Top, [in, satype("BYTE")] SAFEARRAY** Image, [in] long Color, [in] long Threshold);
	[id(1615)] HRESULT DrawMovObject([in] double Completeness, [in] long Type, [in] long Divisions, [in] long Color, [in] long Alpha);
	[id(1616)] HRESULT DrawImageMedia([in] double Completeness, [in] long Left, [in] long Top, [in] IUnknown* pMediaData, [in] long Color, [in] long Alpha);
};
	
[coclass, uuid("7A3065D9-7A1B-4a2b-828F-35BCAD01EAEF"), threading(apartment), vi_progid("AVSImageTransform.ImageTransform3"), progid("AVSImageTransform.ImageTransform3.1"), version(1.0)]
class ATL_NO_VTABLE ImageTransform3 : public IImageTransform3
{
public:

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CComVariant ParamVal = ParamValue;
		CComBSTR ParamStr = ParamName;

		if (ParamStr == L"ChromaThreshold1")
		{
			ParamVal.ChangeType(VT_I4, NULL); 

			m_nAdvancedChromaThreshold1 = ParamVal.lVal;
		}
		else if (ParamStr == L"ChromaThreshold2")
		{
			ParamVal.ChangeType(VT_I4, NULL); 

			m_nAdvancedChromaThreshold2 = ParamVal.lVal;
		}
		else if (ParamStr == L"CropZoom")
		{
			ParamVal.ChangeType(VT_I4, NULL); 

			m_nAdvancedCropZoom = ParamVal.lVal;
		}
		else if (ParamStr == L"ZoomOffsetX")
		{
			ParamVal.ChangeType(VT_R8, NULL); 

			m_dAdvancedZoomOffsetX = ParamVal.dblVal;
		}
		else if (ParamStr == L"ZoomOffsetY")
		{
			ParamVal.ChangeType(VT_R8, NULL); 

			m_dAdvancedZoomOffsetY = ParamVal.dblVal;
		}

		return S_OK;
	}

	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CComBSTR ParamStr = ParamName;
		double   tmpVals[2];

		(*ParamValue).vt = VT_R8;

		if (ParamStr == L"ChromaThreshold1")
		{
			(*ParamValue).vt = VT_I4;
			(*ParamValue).lVal = (LONG)m_nAdvancedChromaThreshold1;
		}
		else if (ParamStr == L"ChromaThreshold2")
		{
			(*ParamValue).vt = VT_I4;
			(*ParamValue).lVal = (LONG)m_nAdvancedChromaThreshold2;
		}
		else if (ParamStr == L"CropZoom")
		{
			(*ParamValue).vt = VT_I4;
			(*ParamValue).lVal = (LONG)m_nAdvancedCropZoom;
		}
		else if (ParamStr == L"ZoomOffsetX")
			(*ParamValue).dblVal = (DOUBLE)m_dAdvancedZoomOffsetX;
		else if (ParamStr == L"ZoomOffsetY")
			(*ParamValue).dblVal = (DOUBLE)m_dAdvancedZoomOffsetY;

		else if (ParamStr == L"AutoContrast")
			GetAutoContrast(0,&(*ParamValue).dblVal);
		else if (ParamStr == L"AutoLevelR")
			GetAutoLevels(0,&(*ParamValue).dblVal,&tmpVals[0],&tmpVals[1]);
		else if (ParamStr == L"AutoLevelG")
			GetAutoLevels(0,&tmpVals[0],&(*ParamValue).dblVal,&tmpVals[1]);
		else if (ParamStr == L"AutoLevelB")
			GetAutoLevels(0,&tmpVals[0],&tmpVals[1],&(*ParamValue).dblVal);
		else if (ParamStr == L"AutoBrightness")
			GetAutoBrightness(0,&(*ParamValue).dblVal);
		else if (ParamStr == L"AutoGamma")
			GetAutoGamma(0,&(*ParamValue).dblVal);
		else if (ParamStr == L"AutoHue")
			GetAutoHue(0,&(*ParamValue).dblVal);
		else if (ParamStr == L"AutoSaturation")
			GetAutoSaturation(0,&(*ParamValue).dblVal);
		else if (ParamStr == L"AutoPosterize")
		{
			long tmpL;
			GetAutoPosterize(0,&tmpL);
			(*ParamValue).dblVal = tmpL;
		}


		return S_OK;

	}
	
	STDMETHOD(SetSafeArray)(SAFEARRAY** Array)
	{
		// check for valid array
		if (!Array)
			return S_FALSE;

		// extract image
		if (!ExtractImage(*Array, m_pPixels, m_nWidth, m_nHeight))
			return S_FALSE;

		m_dAspect = 1.0;
		// compute image size
		m_nSize = 4*m_nWidth*m_nHeight;
		m_IppiRoiSize.width = m_nWidth;
		m_IppiRoiSize.height = m_nHeight;
		m_nIppiStep = 4*m_nWidth;

		// save data
		if (m_nSize > m_nDataSize)
		{
			if (m_pData)
			{
				delete[] m_pData;
				m_pData = 0;
			}
			m_pData = new BYTE[m_nSize];
			m_nDataSize = m_nSize;
		}

		// copy memory
		m_oMemoryUtils.memcpy(m_pData, m_pPixels, m_nSize);

		// save pointer
		m_pPixels = m_pData;

		// all ok
		return S_OK;
	}
	STDMETHOD(SetSafeArrayPtr)(BYTE* SafeArray)
	{
		// extract image
		if (!ExtractImage2(SafeArray, m_pPixels, m_nWidth, m_nHeight))
			return S_FALSE;

		m_dAspect = 1.0;

		// compute image size
		m_nSize = 4*m_nWidth*m_nHeight;
		m_IppiRoiSize.width = m_nWidth;
		m_IppiRoiSize.height = m_nHeight;
		m_nIppiStep = 4*m_nWidth;

		// all ok
		return S_OK;
	}
	STDMETHOD(SetMediaData)(IUnknown* pInterfaceInput, IUnknown** pInterfaceOutput)
	{
		if (!pInterfaceInput || !pInterfaceOutput)
			return S_FALSE;

		*pInterfaceOutput = NULL;

		if (!m_oMediaData.Create(pInterfaceInput, FALSE) || !m_oMediaData.IsValidBGRA())
			return S_FALSE;

		(*pInterfaceOutput) = m_oMediaData.GetMediaData(TRUE);

		m_pPixels = m_oMediaData.GetBuffer();
		m_nWidth = m_oMediaData.GetWidth();
		m_nHeight = m_oMediaData.GetHeight();
		m_nSize = 4*m_nWidth*m_nHeight;
		m_IppiRoiSize.width = m_nWidth;
		m_IppiRoiSize.height = m_nHeight;
		m_nIppiStep = 4*m_nWidth;

		m_dAspect = 1.0;
		MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
		pInterfaceInput->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);
		if (NULL != pFrame)
		{
			long lAspectX = pFrame->AspectRatioX;
			long lAspectY = pFrame->AspectRatioY;

			if ((0 == lAspectX) || (0 == lAspectY))
			{
				m_dAspect = 1;
			}
			else
			{
				m_dAspect = (double)lAspectX / lAspectY;
				if (m_dAspect == ((double)m_nWidth / m_nHeight))
					m_dAspect = 1.0;
			}
			RELEASEINTERFACE(pFrame);
		}

		return S_OK;
	}
	STDMETHOD(GetSafeArray)(SAFEARRAY** Array)
	{
		// extract image
		if (!CompactImage(Array, m_pPixels, m_nWidth, m_nHeight))
			return S_FALSE;

		// all ok
		return S_OK;
	}
	
	STDMETHOD(SetMask)(SAFEARRAY** Array)
	{
		Set_Mask(Array);
		return S_OK;
	}
	STDMETHOD(ClearMask)(void)
	{
		Clear_Mask();
		return S_OK;
	}
	
	STDMETHOD(get_UseMask)(VARIANT_BOOL* Mask)
	{
		*Mask = (m_bMask) ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(put_UseMask)(VARIANT_BOOL Mask)
	{
		m_bMask = (Mask != VARIANT_FALSE);

		return S_OK;
	}
	STDMETHOD(put_BackColor)(long Color)
	{
		m_nBackColor = Color;
		
		return S_OK;
	}
	STDMETHOD(get_BackColor)(long* Color)
	{
		*Color = m_nBackColor;
		
		return S_OK;
	}
	
	STDMETHOD(AdjustBrightness)(double Completeness, long Level);
	STDMETHOD(AdjustBrightnessEx)(double Completeness, long LevelR, long LevelG, long LevelB);
	STDMETHOD(AdjustContrast)(double Completeness, double Level);
	STDMETHOD(AdjustGamma)(double Completeness, double Level);
	STDMETHOD(AdjustEqualize)(double Completeness);
	STDMETHOD(AdjustGrayscale)(double Completeness, VARIANT_BOOL Desaturate);
	STDMETHOD(AdjustInvert)(double Completeness);
	STDMETHOD(AdjustAutocontrast)(double Completeness);
	STDMETHOD(AdjustAutolevels)(double Completeness);
	STDMETHOD(AdjustHue)(double Completeness, double Angle);
	STDMETHOD(AdjustSaturation)(double Completeness, double Level);
	STDMETHOD(AdjustPosterize)(double Completeness, long Levels);
	STDMETHOD(AdjustTemperature)(double Completeness, double Level);
	STDMETHOD(AdjustColorize)(double Completeness, long Red, long Green, long Blue);
	STDMETHOD(AdjustThreshold)(double Completeness, long Level, VARIANT_BOOL Grayscale);

	STDMETHOD(EffectBlur)(double Completeness, double Level);
	STDMETHOD(EffectSharpen)(double Completeness, double Level);
	STDMETHOD(EffectMosaic)(double Completeness, long Size, VARIANT_BOOL Simple);
	STDMETHOD(EffectGaussianBlur)(double Completeness, long Size);
	STDMETHOD(EffectNoise)(double Completeness, double Level, VARIANT_BOOL Mono);
	STDMETHOD(EffectDiffuse)(double Completeness, double Distance);
	STDMETHOD(EffectMotionBlur)(double Completeness, double Angle, double Distance);
	STDMETHOD(EffectEmboss)(double Completeness, double Angle, double Distance, double Amount);
	STDMETHOD(EffectMinimal)(double Completeness, long Size);
	STDMETHOD(EffectMaximal)(double Completeness, long Size);
	STDMETHOD(EffectDeinterlace)(double Completeness, VARIANT_BOOL Even, VARIANT_BOOL Interpolate);
	STDMETHOD(EffectMedian)(double Completeness, long MedianType);
	STDMETHOD(EffectChromoKey)(double Completeness, long Color1, long Color2);
	STDMETHOD(EffectChromoKey2)(double Completeness, long Color, long Threshold);
	
	STDMETHOD(TransformFlip)(double Completeness, VARIANT_BOOL Vertical);
	STDMETHOD(TransformPerspective)(double Completeness, VARIANT_BOOL Vertical, double Angle);
	STDMETHOD(TransformSkew)(double Completeness, VARIANT_BOOL Vertical, double Angle);
	STDMETHOD(TransformShift)(double Completeness, long X, long Y);
	STDMETHOD(TransformRotate)(double Completeness, double Angle);
	STDMETHOD(TransformResample)(double Completeness, long Times);
	STDMETHOD(TransformZoom)(double Completeness, long Zoom);
	STDMETHOD(TransformMirror)(double Completeness, double Level, long Type);
	STDMETHOD(TransformResize)(SAFEARRAY** Output, long Color, long Type);
	STDMETHOD(TransformResizeMedia)(IUnknown** pInterface, long Color, long Type);
	STDMETHOD(TransformResizeMedia2)(long nNewWidth, long nNewHeight, IUnknown** pMediaData, long Color, long Type);
	STDMETHOD(TransformRotateMedia)(double Completeness, double Angle, IUnknown** pMediaData);
	STDMETHOD(TransformTwirl)(double Completeness, double Angle, long Degree);
	STDMETHOD(TransformSphere)(double Completeness, long CenterX, long CenterY, long RadiusX, long RadiusY, double Degree);
	STDMETHOD(TransformCylinder)(double Completeness, long Center, long Radius, double Degree, long Type);
	
	STDMETHOD(FilterPrewitt)(double Completeness, long Type);
	STDMETHOD(FilterScharr)(double Completeness, long Type);
	STDMETHOD(FilterSobel)(double Completeness, long Type);
	STDMETHOD(FilterSobelExt)(double Completeness, long Type);
	STDMETHOD(FilterRoberts)(double Completeness, long Type);
	STDMETHOD(FilterLaplace)(double Completeness, long Type);
	STDMETHOD(FilterHipass)(double Completeness, long Type);
	STDMETHOD(FilterLowpass)(double Completeness, long Type);
	STDMETHOD(FilterBlur)(double Completeness, long Size);
	STDMETHOD(FilterCanny)(double Completeness, double LowThresh, double HighThresh);
	
	// NewImageChannel[0] = OldImageChannel[nChannel0];
	// NewImageChannel[1] = OldImageChannel[nChannel1];
	// NewImageChannel[2] = OldImageChannel[nChannel2];
	STDMETHOD(UtilitySwapChannels)(BYTE nChannel0, BYTE nChannel1, BYTE nChannel2);
	
	STDMETHOD(DrawLine)(double Completeness, long X1, long Y1, long X2, long Y2, long Color, long Alpha, long Size);
	STDMETHOD(DrawRectangle)(double Completeness, long X1, long Y1, long X2, long Y2, long Color, long Alpha, VARIANT_BOOL Solid);
	STDMETHOD(DrawEllipse)(double Completeness, long X1, long Y1, long X2, long Y2, long Color, long Alpha, VARIANT_BOOL Solid);
	STDMETHOD(DrawInvRectangle)(double Completeness, long X1, long Y1, long X2, long Y2, long Color, long Alpha);
	STDMETHOD(DrawInvEllipse)(double Completeness, long X1, long Y1, long X2, long Y2, long Color, long Alpha);
	STDMETHOD(DrawImagePtr)(double Completeness, long X1, long Y1, BYTE* Image, long Color, long Alpha);
	STDMETHOD(DrawImage)(double Completeness, long X1, long Y1, SAFEARRAY** Image, long Color, long Alpha);
	STDMETHOD(DrawImageMedia)(double Completeness, long X1, long Y1, IUnknown* pInterface, long Color, long Alpha);
	STDMETHOD(DrawImageKey)(double Completeness, long Left, long Top, SAFEARRAY** Image, long Color1, long Color2);
	STDMETHOD(DrawImageKey2)(double Completeness, long Left, long Top, SAFEARRAY** Image, long Color, long Threshold);
	STDMETHOD(DrawText)(double Completeness, long X1, long Y1, BSTR Text, IUnknown* Font, long Color, long Alpha);
	STDMETHOD(DrawText2)(double Completeness, long X1, long Y1, long X2, long Y2, BSTR Text, IUnknown* Font, long Color, long Alpha);
	STDMETHOD(DrawCredits)(double Completeness, BSTR Text, IUnknown* Font, long Color, long Alpha, long Type);
	STDMETHOD(DrawBorder)(double Completeness, long Color, long Size, long Type);
	STDMETHOD(DrawMovObject)(double Completeness, long Type, long Divisions, long Color, long Alpha);
	
public:
	
	// general
	int m_nWidth;
	int m_nHeight;
	int m_nSize;
	BYTE* m_pPixels;
	BYTE* m_pBackup;
	Bitmap* m_pBitmap;
	int m_nBackColor;

	// нужен для resize (учитывается только в MediaData), т.к. в SAFEARRAY его нету
	double m_dAspect;

	// additional params
	int m_nAdvancedChromaThreshold1;
	int m_nAdvancedChromaThreshold2;
	int m_nAdvancedCropZoom;
	
	double m_dAdvancedZoomOffsetX;
	double m_dAdvancedZoomOffsetY;

	// intel's code
	IppiSize m_IppiRoiSize;
	int m_nIppiStep;

	// mask
	BYTE* m_pMask;
	BOOL m_bMask;

	// allocated size
	BYTE* m_pData;
	int m_nDataSize;

	// composition
	int m_nSizeX;
	int m_nSizeY;

	CGdiPlusInit m_oGdiPlus;
	MemoryUtils::CMemoryUtils m_oMemoryUtils;

	ImageStudio::Core::MediaData m_oMediaData;
	
public:
	
	ImageTransform3()
	{
		m_nWidth = 0;
		m_nHeight = 0;
		m_pPixels = 0;
		m_pBackup = 0;
		m_pBitmap = 0;
		m_nBackColor = RGBA(0, 0, 0, 255);
		m_nDataSize = 4*800*600;
		m_pData = new BYTE[m_nDataSize];
		m_bMask = FALSE;
		m_pMask = 0;
		m_IppiRoiSize.width = 0;
		m_IppiRoiSize.height = 0;
		m_nIppiStep = 0;
		m_nAdvancedChromaThreshold1 = 16;
		m_nAdvancedChromaThreshold2 = 16;
		m_nAdvancedCropZoom = 100;
		m_dAdvancedZoomOffsetX = 0;
		m_dAdvancedZoomOffsetY = 0;
		m_dAspect = 1.0;

		m_oGdiPlus.Init();

		// initialize intel's library
		ippStaticInitBest();
	}
	~ImageTransform3()
	{
		if (m_pBackup)
			delete[] m_pBackup;
		if (m_pBitmap)
			delete m_pBitmap;
		if (m_pData)
			delete[] m_pData;
		if (m_pMask)
			delete[] m_pMask;
	}
	
	inline BOOL IsValid()
	{ 
		m_nSize = 4*m_nWidth*m_nHeight;
		
		if (!m_pPixels || m_nWidth < 2 || m_nHeight < 2)
			return FALSE;

		BeginMask();

		return TRUE;
	}
	inline double Clamp(double v)
	{
		return (v < 0) ? 0 : (v > 1) ? 1 : v; 
	}
	inline BYTE Byte(double value)
	{
		return (value < 0) ? 0 : (value > 255) ? 255 : (BYTE)value; 
	}
	inline BYTE Lightness(double r, double g, double b)
	{ 
		return (BYTE)(0.5*(max(r, max(g, b)) + min(r, min(g, b)))); 
	}
	inline BYTE Intensity(double r, double g, double b)
	{
		return (BYTE)(0.3*r + 0.59*g + 0.11*b);
	}
	inline double Random()
	{ 
		return 2.0*(rand() % 1000)/1000 - 1.0; 
	}
	inline void BeginEffect()
	{
		m_pBackup = new BYTE[m_nSize];
		
		m_oMemoryUtils.memcpy(m_pBackup, m_pPixels, m_nSize);
	}
	inline void EndEffect()
	{
		delete[] m_pBackup; 
		
		m_pBackup = 0; 
	}
	inline void BeginDraw()
	{
		ApplyFlip(m_pPixels, m_nWidth, m_nHeight);
		
		m_pBitmap = new Bitmap(m_nWidth, m_nHeight, m_nWidth*4, PixelFormat32bppRGB, m_pPixels);
	}
	inline void EndDraw()
	{
		delete m_pBitmap; 
		
		m_pBitmap = 0; 
		
		ApplyFlip(m_pPixels, m_nWidth, m_nHeight); 
		
		ApplyClear(m_pPixels, m_nSize); 
	}
	inline void ApplyRegister()
	{
		if (!CRegistratorClient::IsRegistered())
		{
			CRegistratorClient::Register();
			/*
			for (int index = 0; index < 4*m_nWidth*m_nHeight; ++index)
			{
				m_pPixels[index - index % 32] = 0;
			}
			*/
		}

		EndMask();	
	}
	inline void Set_Mask(SAFEARRAY** pArray)
	{
		if (m_pMask)
		{
			delete[] m_pMask;
			m_pMask = 0;
		}

		// TODO: m_nWidth -> pArray->rgsabound...

		long bound;
		SafeArrayGetUBound((*pArray), 2, &bound);
		int nWidth = bound + 1;

		SafeArrayGetUBound((*pArray), 3, &bound);
		int nHeight = bound + 1;
		//int nWidth = (*pArray)->rgsabound[0].cElements;
		//int nHeight = (*pArray)->rgsabound[1].cElements;
		m_nWidth;
		m_nHeight;
		m_pMask = new BYTE[4*nWidth*nHeight];

		BYTE* pData = (BYTE*)((*pArray)->pvData);
		BYTE* pMask = m_pMask;

		for (int index = 0; index < nWidth*nHeight; ++index, pData += 4, pMask += 4)
			pMask[3] = pData[0];

		m_bMask = TRUE;
	}
	inline void BeginMask()
	{
		if (!m_bMask)
			return;

		if (!m_pMask)
			return;

		BYTE* pData = m_pPixels;
		BYTE* pMask = m_pMask;

		for (int index = 0; index < m_nWidth*m_nHeight; ++index, pData += 4, pMask += 4)
		{
			pMask[0] = pData[0];
			pMask[1] = pData[1];
			pMask[2] = pData[2];
		}	  
	}
	inline void Clear_Mask()
	{
		if (m_pMask)
			delete[] m_pMask;

		m_pMask = 0;
		m_bMask = FALSE;
	}
	inline void EndMask()
	{
		if (!m_bMask)
			return;

		BYTE* pData = m_pPixels;
		BYTE* pMask = m_pMask;

		for (int index = 0; index < m_nWidth*m_nHeight; ++index, pData += 4, pMask += 4)
		{
			if (pMask[3] > 0)
				continue;

			pData[0] = pMask[0];
			pData[1] = pMask[1];
			pData[2] = pMask[2];
		}
	}
	
	BOOL CompactImage(SAFEARRAY** Array, BYTE* data, int width, int height);
	BOOL ExtractImage(SAFEARRAY* Array, BYTE*& data, int& width, int& height);
	BOOL ExtractImage2(BYTE* Array, BYTE*& data, int& width, int& height);
	BOOL ApplyTable(BYTE* Table);
	BOOL ApplyTable(BYTE* TableR, BYTE* TableG, BYTE* TableB);
	BOOL ApplyFlip(BYTE* pPixels, int nWidth, int nHeight);
	BOOL ApplyClear(BYTE* pPixels, int nSize);
	BOOL ApplyColor(BYTE* pPixels, int nSize, int nColor, int nAlpha);
	BOOL ApplyCompose(double dFrame, long nType, int nColor, int nAlpha);
	BOOL ConvertToHls(float fRed, float fGreen, float fBlue, float& fHue, float& fLightness, float& fSaturation);
	BOOL ConvertToRgb(float fHue, float fLightness, float fSaturation, float& fRed, float& fGreen, float& fBlue);
	BOOL ConvertToValue(float fValue1, float fValue2, float fHue, float& fValue);
	BYTE GetPointNearest(double dPointX, double dPointY, BYTE* pData, int nWidth, int nHeight, int nChannel, int nChannels, BYTE nColor);
	BYTE GetPointBillinear(double dPointX, double dPointY, BYTE* pData, int nWidth, int nHeight, int nChannel, int nChannels, BYTE nColor);
	void GetRadiusAndAngle(double x, double y, double cx, double cy, double& r, double& a);
	void ClearImage(BYTE* pData, int nSize);
	void BGRA_FillAlpha(BYTE* pBGRA, int nWidth, int nHeight, BYTE nAlphaValue)
	{
		if (NULL == pBGRA || nWidth < 1 || nHeight < 1)
			return;

		int nSize = nWidth*nHeight;
		BYTE* pAlpha = pBGRA + 3;

		while (nSize > 0)
		{
			*pAlpha = nAlphaValue;

			nSize--;
			pAlpha += 4;
		}
	}

	BOOL WipeDoor(double Completeness, int Type, int Color, int Alpha);
	BOOL WipeSide(double Completeness, int Type, int Color, int Alpha);
	BOOL WipeCenter(double Completeness, int Type, int Color, int Alpha);
	BOOL WipeStrips(double Completeness, int Type, int Color, int Alpha);
	BOOL PushDoor(double Completeness, int Type, int Color, int Alpha);
	BOOL PushSide(double Completeness, int Type, int Color, int Alpha);
	BOOL PushStrips(double Completeness, int Type, int Color, int Alpha);
	BOOL FxMosaic(double Completeness, int Type, int Color, int Alpha);

	BOOL MirrorVertical(double Completeness, int Type, double Level);
	BOOL MirrorHorizontal(double Completeness, int Type, double Level);
	STDMETHOD(TransformResize)(BYTE* pData, long lWidth, long lHeight,  long Color, long Type);
	HRESULT DrawImage(double Frame, long X1, long Y1, BYTE* pData, int nWidth, int nHeight, long Color, long Alpha)
	{
		if (!IsValid())
			return S_FALSE;

		if (!Alpha)
			return S_OK;

		double dAlpha1 = Frame*Alpha;

		// clamp values
		Frame = Clamp(Frame);
		BYTE* AlphaCanal = pData + 3;

		for (int index = 0; index < nWidth*nHeight; ++index, AlphaCanal += 4)
			*AlphaCanal = dAlpha1;

		BITMAPINFO videoBitmapInfo;
		videoBitmapInfo.bmiHeader.biPlanes = 1;
		videoBitmapInfo.bmiHeader.biBitCount = 32;
		videoBitmapInfo.bmiHeader.biCompression = 0;
		videoBitmapInfo.bmiHeader.biSizeImage = 0;
		videoBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
		videoBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
		videoBitmapInfo.bmiHeader.biClrUsed = 0;
		videoBitmapInfo.bmiHeader.biClrImportant = 0;
		videoBitmapInfo.bmiHeader.biSize = 40;	
		videoBitmapInfo.bmiHeader.biWidth = m_nWidth;
		videoBitmapInfo.bmiHeader.biHeight = m_nHeight;

		Bitmap videoBitmap(&videoBitmapInfo, m_pPixels);
	
		Bitmap imageBitmap(nWidth, nHeight, nWidth*4, PixelFormat32bppARGB, pData);

		Graphics* graphics = Graphics::FromImage(&videoBitmap);

		imageBitmap.RotateFlip(Rotate180FlipNone);
		imageBitmap.RotateFlip(Rotate180FlipY);

		ImageAttributes attr;
		Gdiplus::Color col1; col1.SetValue(Color);	
		ARGB argb = col1.MakeARGB(0, GetRValue(Color), GetGValue(Color), GetBValue(Color));
		col1.SetValue(argb);	
		attr.SetColorKey(col1, col1);

		Rect destRect(X1, Y1, imageBitmap.GetWidth(), imageBitmap.GetHeight());

		graphics->DrawImage(&imageBitmap, destRect, 0, 0, imageBitmap.GetWidth(), imageBitmap.GetHeight(), UnitPixel, &attr);
			  
		graphics->Flush();

		delete graphics;

		ApplyRegister();
				
		return S_OK;
	}


	LRESULT GetAutoContrast(double Completeness, double* Level);
	LRESULT AdjustLevels( double Completeness, double LevelR, double LevelG, double LevelB);
	LRESULT GetAutoLevels(double Completeness, double* LevelR, double* LevelG, double* LevelB);

	LRESULT GetAutoBrightness(double Completeness, double* Level);
	LRESULT GetAutoGamma(double Completeness, double* Level);
	LRESULT GetAutoHue(double Completeness, double* Angle);
	LRESULT GetAutoSaturation(double Completeness, double* Level);
	LRESULT GetAutoPosterize(double Completeness, long* Levels);
};
