#pragma once

#include "..\..\..\..\Common\GdiplusEx.h"

#include "..\..\AVSVideoStudio3\Common\MediaFormatDefine.h"
#include "..\Interfaces\AVSGraphicsRenderer.h"
#include "..\Interfaces\ImageSerializeObjects2.h"
#include "..\Interfaces\ImageFilesCache.h"

#include "IAnimationLayer.h" 
#include "AnimationFramework.h"
#include "..\Objects\AnimationLayer.h"

using namespace Animation;

// IAnimationTransformer
[
	object,
	uuid("E0B8D3F9-8A9E-47e1-A821-F7B797290852"),
	dual,	
	helpstring("IAnimationTransformer Interface"),
	pointer_default(unique)
]

__interface IAnimationTransformer : IDispatch
{
	[id(10000)] HRESULT LoadFromXml([in] BSTR Xml);

	[id(10100)]	HRESULT Draw([in] IAVSRenderer* pRenderer, [in] double dTime);
	[id(10200)]	HRESULT DrawOnMediaData([in] IUnknown* punkFrame, [in] double dTime);

	[id(20100)] HRESULT AddLayer([in] IUnknown* pUnk);
	[id(20200)] HRESULT AddXmlLayer([in] BSTR Xml);

	[id(30100)] HRESULT AddTransition([in] BSTR Xml);

	[id(80000)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(90000)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

[
	coclass,
	default(IAnimationTransformer),
	threading(apartment),
	vi_progid("AVSGraphics.AnimationTransformer"),
	progid("AVSGraphics.AnimationTransformer.1"),
	version(1.0),
	uuid("C83705F8-2313-4e2a-A43C-51152AA85C28"),
	helpstring("AnimationTransformer Class"),
	registration_script("control.rgs")
]

class ATL_NO_VTABLE CAnimationTransformer : public IAnimationTransformer
{
public:

	CAnimationTransformer ()
	{
		m_oInitGdiplus.Init();

		m_pRenderer			=	NULL;
		m_dTimeLineDuration	=	0.0;

		m_pImageCache		=	NULL;
		m_pFrameBufferREF	=	NULL;
		m_pFontManager		=	NULL;
	}

	~CAnimationTransformer ()
	{
		for ( long i = 0; i < (long)m_arrLayers.GetCount (); ++i )
			RELEASEINTERFACE ( m_arrLayers[i] );
	
		m_pFrameBufferREF	=	NULL;

		RELEASEINTERFACE(m_pRenderer);
		RELEASEINTERFACE(m_pFontManager);
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}


private:

	class CBufferedLayer
	{
	public:
		CBufferedLayer ()
		{
			m_bRefFrameMode	=	FALSE;

			m_nWidth		=	0;
			m_nHeight		=	0;

			m_pBuffer		=	NULL;
			m_pImage		=	NULL;
		}

		CBufferedLayer (IUnknown* pImg, CCacheStorage& oStorage)
		{
			m_bRefFrameMode	=	FALSE;

			m_nWidth		=	0;
			m_nHeight		=	0;

			m_pBuffer		=	NULL;
			m_pImage		=	NULL;

			SaveBuffer (pImg, oStorage);
		}

		~CBufferedLayer ()
		{
			RELEASEOBJECT (m_pBuffer);
			RELEASEINTERFACE (m_pImage);
		}

		inline void FromRefImage (IUnknown* pSrc)
		{
			m_bRefFrameMode	=	TRUE;
			m_pImage		=	(MediaCore::IAVSUncompressedVideoFrame*)pSrc;
		}

		//
		inline BOOL CopyBuffer (IUnknown* pSrc)
		{
			if (m_bRefFrameMode)
			{
				return CopyBuffer (m_pImage, pSrc);
			}

			MediaCore::IAVSUncompressedVideoFrame* pSource	=	NULL;
			pSrc->QueryInterface (&pSource);
			pSrc->Release ();

			if (pSource && GetBuffer())
			{
				LONG Width			=	0;		pSource->get_Width(&Width);
				LONG Height			=	0;		pSource->get_Height(&Height);
				BYTE* pBuffer		=	NULL;	pSource->get_Buffer(&pBuffer);

				if (Width == m_nWidth && Height == m_nHeight )
				{
					memcpy (pBuffer, GetBuffer (), Width * Height * 4 );

					return TRUE;
				}
			}

			return FALSE;			
		}


		inline IUnknown* GetImage ()
		{
			if (NULL == m_pImage)
			{
				if (NULL == m_pBuffer)
				{
					if (m_oBufFile.Status())
					{
						if ( SUCCEEDED ( CoCreateInstance ( __uuidof ( MediaCore::CAVSUncompressedVideoFrame ), NULL, CLSCTX_INPROC_SERVER, __uuidof ( MediaCore::IAVSUncompressedVideoFrame ), (void **)&m_pImage ) ) )
						{
							m_pImage->put_Width(m_nWidth);
							m_pImage->put_Height(m_nHeight);
							m_pImage->put_Stride(0, 4 * m_nWidth);

							m_pImage->put_AspectRatioX(m_nWidth);
							m_pImage->put_AspectRatioY(m_nHeight);

							m_pImage->put_ColorSpace (CSP_BGRA | CSP_VFLIP);
							m_pImage->AllocateBuffer(-1);

							BYTE* pBuffer = NULL;
							m_pImage->get_Buffer(&pBuffer);

							if (pBuffer)
							{
								if (m_oBufFile.ReadFileBuffer (pBuffer))
									return m_pImage;
							}

							RELEASEINTERFACE (m_pImage);
						}
					}
				}
				else
				{
					if ( SUCCEEDED ( CoCreateInstance ( __uuidof ( MediaCore::CAVSUncompressedVideoFrame ), NULL, CLSCTX_INPROC_SERVER, __uuidof ( MediaCore::IAVSUncompressedVideoFrame ), (void **)&m_pImage ) ) )
					{
						m_pImage->put_Width(m_nWidth);
						m_pImage->put_Height(m_nHeight);
						m_pImage->put_Stride(0, 4 * m_nWidth);

						m_pImage->put_AspectRatioX(m_nWidth);
						m_pImage->put_AspectRatioY(m_nHeight);

						m_pImage->put_ColorSpace (CSP_BGRA | CSP_VFLIP);
						m_pImage->AllocateBuffer(-1);

						BYTE* pBuffer = NULL;
						m_pImage->get_Buffer(&pBuffer);

						memcpy (pBuffer, m_pBuffer, m_nWidth * m_nHeight * 4);
					}
				}
			}

			return m_pImage;
		}

		inline BOOL ClearMemory ()
		{
			RELEASEOBJECT (m_pBuffer);
			RELEASEINTERFACE (m_pImage);

			return TRUE;
		}

	private:

		inline BOOL SaveBuffer (IUnknown* pImg, CCacheStorage& oStorage)
		{
			MediaCore::IAVSUncompressedVideoFrame* pImage = NULL;
			pImg->QueryInterface (&pImage);
			pImg->Release ();

			if (pImage)
			{
				BYTE* pBuffer = NULL;
				pImage->get_Buffer(&pBuffer);

				pImage->get_Width(&m_nWidth);
				pImage->get_Height(&m_nHeight);

				if (FALSE == m_oBufFile.Status())
				{
					m_oBufFile.SetFileName (oStorage.GenTempFilePath());
					m_oBufFile.DumpFrame (pImage, 4 * m_nWidth * m_nHeight);	//	если раньше было сохранение в файл, то дальше будет освобождение памяти
				}

				RELEASEOBJECT (m_pBuffer);
				m_pBuffer	=	new BYTE [ m_nWidth * m_nHeight * 4 ];
				if (m_pBuffer)
				{
					memcpy (m_pBuffer, pBuffer, m_nWidth * m_nHeight * 4);
				}

				return TRUE;
			}

			return FALSE;
		}

		inline BOOL LoadBuffer ()
		{
			if (NULL == m_pBuffer)
			{
				m_pBuffer	=	new BYTE [ m_nWidth * m_nHeight * 4 ];
				if (m_pBuffer)
				{
					return m_oBufFile.ReadFileBuffer (m_pBuffer);						
				}
			}

			return FALSE;
		}

		inline BYTE* GetBuffer ()
		{
			LoadBuffer ();

			if (m_pBuffer)
				return m_pBuffer;

			return NULL;
		}

		inline BOOL CopyBuffer (IUnknown* pSrc, IUnknown* pToImg)
		{
			if ( pSrc && pToImg )
			{
				MediaCore::IAVSUncompressedVideoFrame* pSource	=	NULL;
				pSrc->QueryInterface (&pSource);
				pSrc->Release ();

				MediaCore::IAVSUncompressedVideoFrame* pImage	=	NULL;
				pToImg->QueryInterface (&pImage);
				pToImg->Release ();

				if (pSource && pToImg)
				{
					LONG Width			=	0;		pSource->get_Width(&Width);
					LONG Height			=	0;		pSource->get_Height(&Height);
					LONG ColorSpace		=	0;		pSource->get_ColorSpace(&ColorSpace);
					BYTE* pBuffer		=	NULL;	pSource->get_Buffer(&pBuffer);
					LONG Stride			=	0;		pSource->get_Stride(0, &Stride);

					LONG ToWidth		=	0;		pImage->get_Width(&ToWidth);
					LONG ToHeight		=	0;		pImage->get_Height(&ToHeight);
					LONG ToColorSpace	=	0;		pImage->get_ColorSpace(&ToColorSpace);
					BYTE* pToBuffer		=	NULL;	pImage->get_Buffer(&pToBuffer);
					LONG ToStride		=	0;		pImage->get_Stride(0, &ToStride);

					// TODO : check color space RGBA				

					if (Width == ToWidth && Height == ToHeight && Stride == ToStride /*&& ColorSpace == ToColorSpace*/ )
					{
						memcpy (pToBuffer, pBuffer, Width * Height * 4 );

						return TRUE;
					}
				}
			}

			return FALSE;
		}

	private:

		BOOL		m_bRefFrameMode;

		BYTE*		m_pBuffer;
		MediaCore::IAVSUncompressedVideoFrame*	m_pImage;

		long		m_nWidth;
		long		m_nHeight;

		CCacheFile	m_oBufFile;
	};

	class CSBufferedLayers
	{
	public:
		CSBufferedLayers()
		{
			m_nLayer	=	-1;
		}

		~CSBufferedLayers()
		{
			if (m_arrLayers.GetCount())
			{
				POSITION pos = m_arrLayers.GetStartPosition();
				while (NULL != pos)
				{
					long nIndex	= m_arrLayers.GetKeyAt(pos);

					if (m_nLayer !=  nIndex)
					{
						CBufferedLayer* pLayer = m_arrLayers.GetValueAt (pos);
						if (pLayer)
						{
							RELEASEOBJECT (pLayer);
						}
					}

					m_arrLayers.GetNextValue(pos);
				}
			}
		}

		
		inline BOOL Update (long nIndex, CBufferedLayer* pLayer)
		{
			CBufferedLayer* pSLayer = Get (nIndex);
			if (pSLayer)
				RELEASEOBJECT(pSLayer);

			m_arrLayers.SetAt (nIndex, pLayer);

			return TRUE;
		}

		inline CBufferedLayer* Get (long nIndex)
		{
			CAtlMap < long, CBufferedLayer* >::CPair* pPair	=	m_arrLayers.Lookup ( nIndex );
			if (pPair)
			{
				m_nLayer	=	nIndex;

				return pPair->m_value;	
			}

			m_nLayer	=	-1;

			return NULL;
		}

		inline BOOL IsBuffered (long nIndex)
		{
			return ( NULL != m_arrLayers.Lookup ( nIndex ) );
		}

		inline long GetCount ()
		{
			return (long)m_arrLayers.GetCount();
		}

		inline void ClearBuffers ()
		{
			POSITION pos = m_arrLayers.GetStartPosition();
			while (NULL != pos)
			{
				long nIndex	= m_arrLayers.GetKeyAt(pos);

				if (m_nLayer !=  nIndex)
				{
					CBufferedLayer* pLayer = m_arrLayers.GetValueAt (pos);
					if (pLayer)
					{
						pLayer->ClearMemory ();
					}
				}
				
				m_arrLayers.GetNextValue(pos);
			}
		}

	private:

		long	m_nLayer;
		CAtlMap < long, CBufferedLayer* >	m_arrLayers;		//	STATIC SLIDES
	};

	class CCompose
	{
	public:
		CCompose () : m_pLayers (NULL), m_dBeginTime (0.0), m_dEndTime (0.0), m_nType (-1)
		{
			m_pTransforms	=	NULL;
			m_pFromImage	=	NULL;
			m_pToImage		=	NULL;
			m_pBuffer		=	NULL;
		}

		CCompose (double dBegin, double dEnd, long nType) : m_pLayers (NULL), m_dBeginTime (dBegin), m_dEndTime (dEnd), m_nType (nType)
		{
			m_pTransforms	=	NULL;
			m_pFromImage	=	NULL;
			m_pToImage		=	NULL;
			m_pBuffer		=	NULL;
		}

		~CCompose ()
		{
			RELEASEINTERFACE(m_pTransforms);
			RELEASEOBJECT(m_pBuffer);
			RELEASEOBJECT(m_pFromImage);
			RELEASEOBJECT(m_pToImage);
		}


		inline BOOL CopyBuffer (IUnknown* pSrc, double dTime)
		{
			if (DoEffect(dTime))
			{
				if (0.0 == dTime - m_dBeginTime)
				{
					if (m_pFromImage->GetImage ())
					{
						return m_pFromImage->CopyBuffer (pSrc);
					}
				}

				if (m_dEndTime == dTime)
				{
					if (m_pToImage->GetImage ())
					{
						return m_pToImage->CopyBuffer (pSrc);
					}
				}

				if (m_pBuffer)
				{
					return m_pBuffer->CopyBuffer (pSrc);
				}
			}

			return FALSE;
		}


		inline void SetSLayers (CSBufferedLayers* pLayers)
		{
			m_pLayers	=	pLayers;
		}

		inline BOOL InInterval (double dTime)
		{
			return (dTime >= m_dBeginTime && dTime <=m_dEndTime);
		}


		inline void ClearBuffers ()
		{
			if (m_pFromImage)
				m_pFromImage->ClearMemory ();

			if (m_pToImage)
				m_pToImage->ClearMemory ();

			if (m_pBuffer)
				m_pBuffer->ClearMemory ();

			RELEASEINTERFACE(m_pTransforms);
		}

		inline void SetFromImage (IUnknown* pImage, CCacheStorage& oStorage)
		{
			RELEASEOBJECT(m_pFromImage);
			m_pFromImage	=	new CBufferedLayer (pImage,oStorage);

			RELEASEOBJECT(m_pBuffer);
			m_pBuffer		=	new CBufferedLayer (pImage,oStorage);
		}

		inline void SetToImage (IUnknown* pImage, CCacheStorage& oStorage)
		{
			RELEASEOBJECT(m_pToImage);
			m_pToImage	=	new CBufferedLayer (pImage, oStorage);
		}
		//
		inline double GetBeginTime ()
		{
			return m_dBeginTime;
		}

		inline double GetEndTime ()
		{
			return m_dEndTime;
		}

		inline BOOL IsComplete ()
		{
			return (NULL != m_pFromImage && NULL != m_pToImage);
		}
#ifdef _DEBUG
		inline BOOL Debug_Clear ()
		{
			RELEASEOBJECT(m_pFromImage);
			RELEASEOBJECT(m_pToImage);

			return TRUE;
		}

#endif
	private:

		inline BOOL Setup ()
		{
			if (m_pTransforms)
				return TRUE;

			if ( SUCCEEDED (CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)&m_pTransforms) ) )
			{
				CStringW Effect;
				Effect.Format ( L"<VideoCompose Time=\"%f\" effectid=\"%d\" />", m_dEndTime - m_dBeginTime, m_nType );

				VARIANT_BOOL bStatus = VARIANT_FALSE;
				m_pTransforms->SetXml ( CComBSTR ( Effect ), &bStatus );

				if (VARIANT_TRUE == bStatus)
					return TRUE;

				RELEASEINTERFACE(m_pTransforms);
			}

			return FALSE;
		}

		inline BOOL DoEffect (double dTime)
		{
			Setup ();

			if (m_pTransforms)
			{
				m_pTransforms->SetTime (dTime - m_dBeginTime);

				VARIANT buff;	buff.vt	=	VT_UNKNOWN;	buff.punkVal	=	m_pBuffer->GetImage ();
				VARIANT from;	from.vt	=	VT_UNKNOWN;	from.punkVal	=	m_pFromImage->GetImage ();
				VARIANT to;		to.vt	=	VT_UNKNOWN;	to.punkVal		=	m_pToImage->GetImage ();
				
				m_pTransforms->SetSource(0, buff);
				m_pTransforms->SetSource(1, from);
				m_pTransforms->SetSource(2, to);

				VARIANT_BOOL bRes	=	VARIANT_FALSE;
				m_pTransforms->Transform (&bRes);

				if (VARIANT_TRUE == bRes)
				{
					VARIANT vImg;
					m_pTransforms->GetResult ( 0, &vImg );

					if ( vImg.punkVal )
					{
						m_pBuffer->GetImage()->Release ();
						m_pBuffer->FromRefImage (vImg.punkVal);

						return TRUE;
					}
				}
			}

			return FALSE;
		}

		BOOL SaveImage(IUnknown* punkFrame, BSTR bstrFile)
		{
			ImageStudio::IImageTransforms* pTransform = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL ,CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

			VARIANT var;
			var.vt = VT_UNKNOWN;
			var.punkVal = punkFrame;
			pTransform->SetSource(0, var);

			CString strXml = _T("<transforms><ImageFile-SaveAsPng destinationpath=\"") + (CString)bstrFile + _T("\" format=\"888\"/></transforms>");

			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			BSTR bsXml = strXml.AllocSysString();
			pTransform->SetXml(bsXml, &vbSuccess);
			SysFreeString(bsXml);

			pTransform->Transform(&vbSuccess);

			RELEASEINTERFACE(pTransform);

			return TRUE;
		}

	private:

		ImageStudio::IImageTransforms* m_pTransforms;

		CSBufferedLayers*	m_pLayers;

		CBufferedLayer*		m_pFromImage;
		CBufferedLayer*		m_pToImage;

		CBufferedLayer*		m_pBuffer;

		long				m_nType;
		double				m_dBeginTime;
		double				m_dEndTime;

		double				m_dCurTime;
	};

	class CComposerLayers 
	{
	public:
		CComposerLayers ()
		{
			m_nShowCompose	=	-1;
		}
		~CComposerLayers ()
		{
			for ( long i = 0; i < (long)m_arrComposes.GetCount (); ++i )
				RELEASEOBJECT ( m_arrComposes[i] );
		}

		inline BOOL Add ( CCompose* pCompose )
		{
			m_arrComposes.Add ( pCompose );

			return TRUE;
		}

		inline CCompose* Get (double dTime)
		{
			for ( long i = 0; i < (long)m_arrComposes.GetCount (); ++i )
			{
				if ( m_arrComposes[i]->InInterval(dTime) )
				{
					m_nShowCompose	=	i;
					return m_arrComposes[i];
				}
			}

			m_nShowCompose	=	-1;

			return NULL;
		}

		inline void ClearBuffers ()
		{
			//if ( m_nShowCompose >= 0 )
			//{
				for ( long i = 0; i < (long)m_arrComposes.GetCount (); ++i )
				{
					if (i == m_nShowCompose)
						continue;

					m_arrComposes[i]->ClearBuffers ();
				}
			//}
		}

	private:

		long					m_nShowCompose;
		CAtlArray<CCompose*>	m_arrComposes;
	};

public:

	STDMETHOD(LoadFromXml)(BSTR Xml)
	{
		return S_OK;
	}

	STDMETHOD(Draw)(IAVSRenderer* pRenderer, double dTime)
	{
		if ( pRenderer )
		{
			for ( int i = 0; i < (int)m_arrLayers.GetCount (); ++i )
				m_arrLayers[i]->Draw ( pRenderer, dTime );
		}

		return S_OK;
	}

	STDMETHOD(DrawOnMediaData)(IUnknown* punkFrame, double dTime)	
	{
		if (NULL == punkFrame)
			return S_FALSE;

		if (InternalCompose(punkFrame, dTime))
			return S_OK;

		long nDrawLayer		=	-1;
		long nLayer			=	-1;
		double dBeginTime	=	0.0;
		double dEndTime		=	0.0;

		for ( long i = 0; i < (long)m_arrLayers.GetCount (); ++i )
		{
			m_arrLayers[i]->get_BeginTime ( &dBeginTime );
			m_arrLayers[i]->get_EndTime ( &dEndTime );

			if ( (dTime > dBeginTime && dTime <= dEndTime) )
			{
				nDrawLayer	=	i;

				if ( m_oSBLayers.IsBuffered(i) )
				{
					nLayer	=	i;

					CBufferedLayer* pSLayer	= m_oSBLayers.Get(i);
					if (pSLayer)
					{
						if ( pSLayer->CopyBuffer (punkFrame))
						{
							m_oLComposer.ClearBuffers ();
							m_oSBLayers.ClearBuffers ();

							ClearLayerCache (i);

							return S_OK;
						}

						m_oSBLayers.Update (i, NULL);
					}
				}

				break;
			}
		}

		ClearLayerCache (nDrawLayer);

		InternalDrawToFrame (punkFrame, dTime);

		m_oLComposer.ClearBuffers ();
		
		StoreStaticLayer (punkFrame,nLayer);

		m_oSBLayers.ClearBuffers ();

		return S_OK;
	}

	STDMETHOD(AddLayer)(IUnknown* pUnk)	
	{
		if ( pUnk )
		{
			IAnimationLayer* pLayer = NULL;
			if ( SUCCEEDED ( pUnk->QueryInterface ( &pLayer ) ) )
			{
				pUnk->Release ();

				if ( pLayer )
				{
					pLayer->AddRef ();
					m_arrLayers.Add ( pLayer );

					CCOMAnimationLayer* pComLayer	=	static_cast<CCOMAnimationLayer*> (pLayer);
					if (pComLayer)
					{
						pComLayer->SetCacheManager (&m_oCommandsCache);
					}

					VARIANT_BOOL bStatic	=	VARIANT_FALSE;
					pLayer->get_IsStatic (&bStatic);
					if (VARIANT_TRUE == bStatic)
					{
						m_oSBLayers.Update ((long)m_arrLayers.GetCount()-1, NULL);

						VARIANT vt;
						vt.vt		=	VT_BOOL;
						vt.boolVal	=	VARIANT_FALSE;

						pLayer->SetAdditionalParam ( L"InternalCache", vt );
					}
				}
			}
		}

		return S_OK;
	}
	STDMETHOD(AddXmlLayer)(BSTR Xml)	
	{
		return S_OK;
	}


	STDMETHOD(AddTransition)(BSTR Xml)
	{
		XmlUtils::CXmlNode oXmlNode;
		if ( oXmlNode.FromXmlString ( (CString) Xml ) )
		{
			long Type		=	_tstol ( oXmlNode.GetAttributeOrValue ( _T("type"),	 _T("-1") ) );
			double Begin	=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("begin"), _T("0") ) );
			double End		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T("end"),	 _T("0") ) );

			if ( (End - Begin > 0.0) && Type >= 0 )
			{
				CCompose* pCompose	=	new CCompose (Begin, End, Type);
				if (pCompose)
				{
					pCompose->SetSLayers (&m_oSBLayers);
					m_oLComposer.Add ( pCompose );
				}
			}
		}

		return S_OK;
	}


	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		if ( CString ( L"FrameBuffer" ) == CComBSTR ( ParamName ) )
		{
			m_pFrameBufferREF	=	NULL;

			if ( ParamValue.vt	== VT_UNKNOWN && NULL != ParamValue.punkVal )
			{
				ParamValue.punkVal->QueryInterface (&m_pFrameBufferREF);
				ParamValue.punkVal->Release ();
			}
		}

		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		if ( CString ( L"FontManager" ) == CComBSTR(ParamName) )
		{
			ParamValue->vt				=	VT_UNKNOWN;
			ParamValue->punkVal			=	NULL;

			IAVSFontManager* pManager	=	GetFontManager ();
			if (NULL != pManager)
			{
				pManager->QueryInterface(&ParamValue->punkVal);
			}
		}

		return S_OK;
	}


protected:

	inline BOOL InternalDrawToFrame (IUnknown* punkFrame, double dTime)	//	команды отрисовались в картинку
	{
		int lWidth		= 0;
		int lHeight		= 0;
		int lStride		= 0;
		BYTE* pBuffer	= NULL;

		if (!COM_GetBGRAImageFromInterface(punkFrame, pBuffer, lWidth, lHeight, &lStride))
			return S_FALSE;

		if ( FALSE == UpdateDIB ( lWidth, lHeight ) )
			return S_FALSE;

		LONG lBufferSize = 4 * lWidth * lHeight;
		memset ( pBuffer, 0x0, lBufferSize);
		memcpy ( m_oSection.m_pBits, pBuffer, lBufferSize);

		UpdateRender ();

		if ( m_pRenderer )
		{
			for ( int i = 0; i < (int)m_arrLayers.GetCount (); ++i )
			{
				m_arrLayers[i]->Draw ( m_pRenderer, dTime );
			}
		}

		if (lStride < 0)
		{
			LONG lSize = 4 * lWidth * lHeight;
			memcpy(pBuffer, m_oSection.m_pBits, lSize);
		}
		else
		{
			BYTE* pSrc = m_oSection.m_pBits;
			LONG lSize = 4 * lWidth;
			BYTE* pDst = pBuffer + lSize * (lHeight - 1);

			for (int i = 0; i < lHeight; ++i)
			{
				memcpy(pDst, pSrc, lSize);
				pSrc += lSize;
				pDst -= lSize;
			}
		}

		return TRUE;
	}


	inline BOOL InternalCompose (IUnknown* pFrame, double dTime)
	{
		CCompose* pCompose	=	m_oLComposer.Get (dTime);
		if (pCompose)
		{
			if (FALSE == pCompose->IsComplete () )
			{
				InternalDrawToFrame (pFrame, pCompose->GetBeginTime() - 0.0001);
				pCompose->SetFromImage (pFrame, m_oCommandsCache);

				InternalDrawToFrame (pFrame, pCompose->GetEndTime());
				pCompose->SetToImage (pFrame, m_oCommandsCache);
			}

			if (pCompose->CopyBuffer (pFrame,dTime))
			{
#ifdef _DEBUG
				//pCompose->Debug_Clear ();
#endif
				return TRUE;
			}
		}

		return FALSE;
	}

	inline BOOL StoreStaticLayer (IUnknown* pFrame, long nInd)
	{
		if (-1 != nInd)
		{
			MediaCore::IAVSMediaData* pSource	=	NULL;
			pFrame->QueryInterface (&pSource);
			pFrame->Release ();

			if (pSource)
			{
				CBufferedLayer* pBuffer	= new CBufferedLayer (pSource, m_oCommandsCache);
				if (pBuffer)
				{
					m_oSBLayers.Update (nInd, pBuffer);

					return TRUE;
				}
			}
		}

		return FALSE;
	}

	inline BOOL ClearLayerCache (long nPassInd)
	{
		for ( long i = 0; i < (long)m_arrLayers.GetCount (); ++i )
		{
			if ( i == nPassInd )
				continue;

			m_arrLayers[i]->ClearCache ();
		}

		return TRUE;
	}

	BOOL UpdateDIB (long Width, long Height)
	{
		if ( (NULL == m_oSection.m_pBits) || (Width != m_oSection.m_lWidth || Height != m_oSection.m_lHeight) )
			m_oSection.Create(Width, Height);

		if ( NULL == m_oSection.m_pBits )
			return FALSE;

		return TRUE;
	}

	BOOL UpdateRender ()
	{
		if ( NULL == m_pRenderer )
		{
			if ( SUCCEEDED ( CoCreateInstance ( __uuidof ( CAVSGraphicsRenderer ), NULL, CLSCTX_ALL, __uuidof ( IAVSRenderer ), (void**)&m_pRenderer ) ) )
			{
				CAVSGraphicsRenderer* pGrRender = static_cast <CAVSGraphicsRenderer*> ( m_pRenderer );
				if ( pGrRender )
				{
					pGrRender->SetFontManager(GetFontManager ());

					NSDocumentViewer::CDoubleRect oRect;
					oRect.left		=	0;
					oRect.top		=	0;
					oRect.right		=	m_oSection.m_lWidth;
					oRect.bottom	=	m_oSection.m_lHeight;

					pGrRender->m_oRenderer.Create ( m_oSection.m_pBits, oRect, m_oSection.m_lWidth, m_oSection.m_lHeight, &m_oSection );

					RELEASEOBJECT ( m_pImageCache );
					m_pImageCache		=	new CImageFilesCache ();

					pGrRender->SetImageCache ( m_pImageCache );

					return TRUE;
				}

				return FALSE;
			}
		}

		return TRUE;
	}


	BOOL COM_GetBGRAImageFromInterface(IUnknown* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight, int* pStride = NULL, long* pAspectX = NULL, long* pAspectY = NULL)
	{
		/* @desc:
		int COM_GetBGRAImageFromInterface(IUnknown* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight)
		{
		из указанного интерфейса (pSource) вытаскивается картинка (pBGRA, nWidth, nHeight)
		в формате BGRA; если формат интерфейса не CSP_BGRA - конвертация формата не производится
		}
		*/

		if (NULL == pSource)
			return FALSE;

		// вытаскиваем из интерфейса uncompressed frame
		MediaCore::IAVSUncompressedVideoFrame* pUncompressedFrame = NULL;
		pSource->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pUncompressedFrame));
		if (NULL == pUncompressedFrame)
			return FALSE;

		// запрашиваем настройки картинки
		long lWidth			= 0; pUncompressedFrame->get_Width(&lWidth);
		long lHeight		= 0; pUncompressedFrame->get_Height(&lHeight);
		long lColorSpace	= 0; pUncompressedFrame->get_ColorSpace(&lColorSpace);
		long lStride		= 0; pUncompressedFrame->get_Stride(0, &lStride);
		long lBufferSize	= 0; pUncompressedFrame->get_BufferSize(&lBufferSize);
		BYTE* pSourceBuffer = NULL; pUncompressedFrame->get_Buffer(&pSourceBuffer);

		// проверяем совместимость кадра и формата
		if (CSP_BGRA != (CSP_COLOR_MASK & lColorSpace) || 4*lWidth != lStride || lWidth < 1 || lHeight < 1 || lBufferSize != 4*lWidth*lHeight || !pSourceBuffer)
		{
			pUncompressedFrame->Release();

			return FALSE;
		}

		if( pStride )
			*pStride = (lColorSpace & CSP_VFLIP) ? -lStride : lStride;

		if( pAspectX ) pUncompressedFrame->get_AspectRatioX( pAspectX );
		if( pAspectY ) pUncompressedFrame->get_AspectRatioY( pAspectY );

		// все хорошо, озвращаем наверх нужные параметры
		pBGRA	= pSourceBuffer;
		nWidth	= lWidth;
		nHeight = lHeight;

		pUncompressedFrame->Release();

		return TRUE;
	}




	inline IAVSFontManager* GetFontManager ()
	{
		if (NULL==m_pFontManager)
		{			
			if ( SUCCEEDED (CoCreateInstance(__uuidof(CAVSFontManager), NULL, CLSCTX_INPROC, __uuidof(IAVSFontManager), (void**)&m_pFontManager) ) )
			{
				m_pFontManager->Initialize(L"");
				m_pFontManager->SetDefaultFont( L"Arial" );

				VARIANT var;
				var.vt		=	VT_BOOL;
				var.boolVal	=	VARIANT_TRUE;
				m_pFontManager->SetAdditionalParam(L"UseDefaultFont", var);
			}
		}

		return m_pFontManager;
	}	

protected:

	CGdiPlusInit						m_oInitGdiplus;	

	CAtlArray <IAnimationLayer*>		m_arrLayers;

	IUnknown*							m_pFrameBufferREF;	//	REFERENCE

	CSBufferedLayers					m_oSBLayers;	
	CComposerLayers						m_oLComposer;

	double								m_dTimeLineDuration;

	NSDocumentViewer::CDIB				m_oSection;
	IAVSRenderer*						m_pRenderer;
	CImageFilesCache*					m_pImageCache;
	IAVSFontManager*					m_pFontManager;

	CCacheStorage						m_oCommandsCache;
};