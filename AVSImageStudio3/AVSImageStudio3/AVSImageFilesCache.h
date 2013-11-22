#pragma once

#include "resource.h"       // main symbols

#include <atlcoll.h>
#include "TemporaryCS.h"
#include <ImageTransformsCore.h>

[ object, uuid("708FDC4E-FE53-463c-8D0A-7BE60949EBC6"), dual, pointer_default(unique)]
__interface IAVSImageCache : IDispatch
{
	[id(10), propget]		HRESULT MaxSizes([out,retval] long *pVal);
	[id(10), propput]		HRESULT MaxSizes([in] long newVal);

	[id(20), propget]		HRESULT ResizeMode([out,retval] long *pVal);
	[id(20), propput]		HRESULT ResizeMode([in] long newVal);

	[id(30), propget]		HRESULT BackColor([out,retval] long *pVal);
	[id(30), propput]		HRESULT BackColor([in] long newVal);

	[id(100)]				HRESULT PrepareImage([in] BSTR bsFilePath, [in]long Width, [in] long Height);

	[id(150)]				HRESULT LoadImage([in] BSTR bsFilePath, [in]long Width, [in] long Height, [out, retval] IUnknown** ppunkVal);
	
	[id(200)]				HRESULT Clear();

	[id(300)]				HRESULT RemoveImage([in] BSTR bsFilePath);

	[id(400)]				HRESULT PrepareImage2([in] BSTR bsFilePath, [in]long Width, [in] long Height, [in] long lAspectX, [in] long lAspectY);
	[id(401)]				HRESULT LoadImage2([in] BSTR bsFilePath, [in]long Width, [in] long Height, [in] long lAspectX, [in] long lAspectY, [out, retval] IUnknown** ppunkVal);
};
//
//
//[ object, uuid("62DC32C9-39AA-47a7-9A92-AAEC9FEF775A"), dual, pointer_default(unique) ]
//__interface IAVSImageCache2 : IAVSImageCache
//{
//	[id (311000 + 9) ]	HRESULT GetMetadataXML ([in] BSTR bsFilePath, [ out, retval ] BSTR* pVal);
//};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
[ 
	coclass, uuid("114C5ACD-EA4D-4b52-8403-14D24BA60776"),
	default(IAVSImageCache),
	threading(apartment), 
	vi_progid("AVSImageStudio.ImageCache"), 
	progid("AVSImageStudio.ImageCache.1"), 
	version(1.0)
]
class ATL_NO_VTABLE CAVSImageCache
	: public IAVSImageCache
{
public:
	CAVSImageCache()
		: m_lMaxCacheSize(100*1024*1024)
		, m_lResizeMode(RESIZE_MODE_LINEAR)
		, m_dwBackColor(0)
	{
		// инициализируем gdiplus
		m_oGdiPlusInit.Init();
		//ATLTRACE("CAVSImageFilesCache::Create. this = 0x%08X, ThreadId = 0x%08X\n", this, GetCurrentThreadId());
		InitializeCriticalSection(&m_csImageFilesCache);
	}
	virtual ~CAVSImageCache()
	{
		//ATLTRACE("CAVSImageFilesCache::Release. this = 0x%08X, ThreadId = 0x%08X\n", this, GetCurrentThreadId());

		Clear();
		DeleteCriticalSection(&m_csImageFilesCache);
	}
	STDMETHOD(get_MaxSizes)(long *pVal)
	{
		*pVal = m_lMaxCacheSize;
		return S_OK;
	}
	STDMETHOD(put_MaxSizes)(long newVal)
	{
		m_lMaxCacheSize = newVal;
		return S_OK;
	}

	STDMETHOD(get_ResizeMode)(long *pVal)
	{
		*pVal = m_lResizeMode;
		return S_OK;
	}
	STDMETHOD(put_ResizeMode)(long newVal)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		if (m_lResizeMode == newVal)
			return S_OK;
		m_lResizeMode = newVal;
		ResetResizedImages();
		return S_OK;
	}

	STDMETHOD(get_BackColor)(long *pVal)
	{
		*pVal = m_dwBackColor;
		return S_OK;
	}
	STDMETHOD(put_BackColor)(long newVal)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		if (m_dwBackColor == newVal)
			return S_OK;
		m_dwBackColor = newVal;
		ResetResizedImages();
		return S_OK;
	}

	STDMETHOD(PrepareImage)(BSTR bsFilePath, long Width, long Height)
	{
		if ((-1!=Width) && (-1==Height))
			return S_FALSE;
		return PrepareImage2(bsFilePath, Width, Height, Width, Height);
	}
	STDMETHOD(LoadImage)(BSTR bsFilePath, long Width, long Height, IUnknown** ppunkVal)
	{
		if ((-1!=Width) && (-1!=Height))
			return S_FALSE;
		return LoadImage2(bsFilePath, Width, Height, Width, Height, ppunkVal);
	}
	STDMETHOD(GetMetadataXML)(BSTR bsFilePath, BSTR *bstrMetaData)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		*bstrMetaData = NULL;

		CString sFilePath; sFilePath = bsFilePath;
		CCachedImageStorage *pCachedImageStorage = GetCachedImageStorageExact(sFilePath);
		if (NULL==pCachedImageStorage)
			return S_OK;

		*bstrMetaData = pCachedImageStorage->GetMetaDataCopy();

		return S_OK;
	}
	STDMETHOD(PrepareImage2)(BSTR bsFilePath, long Width, long Height, long lAspectX, long lAspectY)
	{
		CTemporaryCS cs(&m_csImageFilesCache);

		CString sFilePath; sFilePath = bsFilePath;
		CCachedImageStorage *pCachedImageStorage = GetCachedImageStorage(sFilePath);
		if (NULL==pCachedImageStorage)
			return S_FALSE;
		if ((0>=Width) || (0>=Height))
			return S_OK;

		IUnknown *punkImage = GetImageCopy(pCachedImageStorage, sFilePath, Width, Height, lAspectX, lAspectY, m_lResizeMode, m_dwBackColor);
		RELEASEINTERFACE(punkImage);
		return S_OK;
	}

	STDMETHOD(LoadImage2)(BSTR bsFilePath, long Width, long Height, long lAspectX, long lAspectY, IUnknown** ppunkVal)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		*ppunkVal = NULL;

		CString sFilePath; sFilePath = bsFilePath;
		//ATLTRACE("CAVSImageFilesCache::LoadImage: this = 0x%08X, %s\n", this, sFilePath);
		CCachedImageStorage *pCachedImageStorage = GetCachedImageStorage(sFilePath);
		if (NULL==pCachedImageStorage)
			return S_FALSE;

		*ppunkVal = GetImageCopy(pCachedImageStorage, sFilePath, Width, Height, lAspectX, lAspectY, m_lResizeMode, m_dwBackColor);

		CheckCacheSize();

		return (NULL!=*ppunkVal) ? S_OK : S_FALSE;
	}
	
	STDMETHOD(Clear)()
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		ClearMap();
		//ATLTRACE("CAVSImageFilesCache::Clear. this = 0x%08X, ThreadId = 0x%08X\n", this, GetCurrentThreadId());
		return S_OK;
	}

	STDMETHOD(RemoveImage)(BSTR bsFilePath)
	{
		CTemporaryCS cs(&m_csImageFilesCache);
		CString sFilePath; sFilePath = bsFilePath;
		CAtlMap<CString, CCachedImageStorage *>::CPair *pPair = m_mapImages.Lookup(sFilePath);
		if (NULL==pPair)
			return S_OK;
		if (NULL!=pPair->m_value)
			delete pPair->m_value;
		m_mapImages.RemoveKey(sFilePath);
		return S_OK;
	}
protected:
	long m_lMaxCacheSize;
	long m_lResizeMode;
	DWORD m_dwBackColor;
	CGdiPlusInit m_oGdiPlusInit;

	CRITICAL_SECTION m_csImageFilesCache;

	class CCachedImageStorage
	{
	public:
		CCachedImageStorage(const SYSTEMTIME &oFileLastWriteTime)
			: m_lCacheSize(0)
			, m_pSourceImage(NULL)
			, m_lSourceWidth(0)
			, m_lSourceHeight(0)
			, m_oFileLastWriteTime(oFileLastWriteTime)
			, m_bstrMetaData(NULL)
		{
		}
		~CCachedImageStorage()
		{
			RELEASEINTERFACE(m_pSourceImage);
			ClearFramesArray();
			if (m_bstrMetaData)SysFreeString(m_bstrMetaData);
			m_bstrMetaData = NULL;
		}
		//
		long GetCacheSize() const
		{
			return m_lCacheSize;
		}
		//
		const SYSTEMTIME &GetFileLastWriteTime() const
		{
			return m_oFileLastWriteTime;
		}
		//
		long GetSourceWidth() const
		{
			return m_lSourceWidth;
		}
		long GetSourceHeight() const
		{
			return m_lSourceHeight;
		}
		//
		void SetMetaData(BSTR bstrMetaData)
		{
			if (m_bstrMetaData)
				SysFreeString(m_bstrMetaData);
			m_bstrMetaData = SysAllocString(bstrMetaData);
		}
		void SetImage(IUnknown *punkImage)
		{
			RELEASEINTERFACE(m_pSourceImage);
			ClearFramesArray();
			if (NULL==punkImage)
				return;
			punkImage->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pSourceImage);
			if (NULL==m_pSourceImage)
				return;
			long lSize = 0;
			m_pSourceImage->get_BufferSize(&lSize);
			m_lCacheSize += lSize;

			m_pSourceImage->get_Width(&m_lSourceWidth);
			m_pSourceImage->get_Height(&m_lSourceHeight);
		}
		BOOL HasSourceImage() const
		{
			return (NULL!=m_pSourceImage);
		}
		void ResetSourceImage()
		{
			if (NULL==m_pSourceImage)
				return;
			long lSize = 0;
			m_pSourceImage->get_DataSize(&lSize);
			m_lCacheSize -= lSize;
			RELEASEINTERFACE(m_pSourceImage);
		}
		void ResetResizedImage()
		{
			long lSize = 0;
			size_t nCount = m_arFrames.GetCount();
			for (size_t nIndex = 0; nIndex <  nCount;  nIndex++)
			{
				RELEASEINTERFACE(m_arFrames[nIndex]);
			}
			m_arFrames.RemoveAll();
			m_lCacheSize = 0;
			if (NULL!=m_pSourceImage)
				m_pSourceImage->get_DataSize(&m_lCacheSize);
		}
		//
		IUnknown *GetSourceImageCopy()
		{
			if (NULL==m_pSourceImage)
				return NULL;
			return CreateFramesCopy(m_pSourceImage);
		}
		BSTR GetMetaDataCopy()
		{
			if (NULL==m_bstrMetaData)
				return NULL;
			return SysAllocString(m_bstrMetaData);
		}
		IUnknown *GetImageCopy(long lWidth, long lHeight, long lAspectX, long lAspectY, long lResizeMode, DWORD dwBackColor)
		{
			long lFrameWidth = 0;
			long lFrameHeight = 0;
			long lFrameAspectX = 0;
			long lFrameAspectY = 0;
			bool bAspectEqual = false;

			if (NULL != m_pSourceImage)
			{
				m_pSourceImage->get_Width(&lFrameWidth);
				m_pSourceImage->get_Height(&lFrameHeight);
				m_pSourceImage->get_AspectRatioX(&lFrameAspectX);
				m_pSourceImage->get_AspectRatioY(&lFrameAspectY);
				if (0 == lAspectX && 1 == lAspectY)
				{
					lAspectX = lFrameWidth;
					lAspectY = lFrameHeight;
				}

				if (0 != lAspectY && 0 != lFrameAspectY)
					bAspectEqual = abs ((lAspectX / lAspectY) - (lFrameAspectX / lFrameAspectY)) < 0.01;

				if ((lFrameWidth==lWidth)&&(lFrameHeight==lHeight) && bAspectEqual)
					return CreateFramesCopy (m_pSourceImage);
			}
			size_t nCount = m_arFrames.GetCount();
			for (size_t nIndex = 0; nIndex <  nCount;  nIndex++)
			{
				if (NULL==m_arFrames[nIndex])
				{
					m_arFrames.RemoveAt(nIndex);
					nCount--; nIndex--;
					continue;
				}
				
				MediaCore::IAVSUncompressedVideoFrame *pFrame = m_arFrames[nIndex];
				pFrame->get_Width(&lFrameWidth);
				pFrame->get_Height(&lFrameHeight);
				pFrame->get_AspectRatioX(&lFrameAspectX);
				pFrame->get_AspectRatioY(&lFrameAspectY);
				if (0 == lAspectX && 1 == lAspectY)
				{
					lAspectX = GetSourceWidth();
					lAspectY = GetSourceHeight();
				}

				
				if (0 != lAspectY && 0 != lFrameAspectY)
					bAspectEqual = abs ((lAspectX / lAspectY) - (lFrameAspectX / lFrameAspectY)) < 0.01;

				if ((lFrameWidth==lWidth) && (lFrameHeight==lHeight) && bAspectEqual)
						return CreateFramesCopy(pFrame);

			}

			
			if (NULL==m_pSourceImage)
				return NULL;//запрашивают фрейм при этом исходного нет - масштабировать нечего возвращаем нул							

			//MediaCore::IAVSUncompressedVideoFrame *pFrame = ResizeFrame(m_pSourceImage, lWidth, lHeight, lAspectX, lAspectY, lResizeMode, dwBackColor);
			//привидим тип из MediaDataDefine.h к типу из TransformConstants.h
			long nImageStudioResizeMode;
			switch(lResizeMode)
			{
			case RESIZE_MODE_NEAREST: nImageStudioResizeMode = c_nResizeStretchNearest;break;
			case RESIZE_MODE_LINEAR: nImageStudioResizeMode = c_nResizeStretchBillinear;break;
			case RESIZE_MODE_CUBIC: nImageStudioResizeMode = c_nResizeStretchBicubic;break;
			case RESIZE_MODE_SUPER: nImageStudioResizeMode = c_nResizeStretchSuper;break;
			default: nImageStudioResizeMode = c_nResizeStretchBillinear;break;
			}
			//делаем Resize
			MediaCore::IAVSUncompressedVideoFrame *pFrame = NULL;
			ImageStudio::Transforms::Core::COM::COM_TransformResizeMedia( m_pSourceImage, (IUnknown**)&pFrame, lWidth, lHeight, dwBackColor, nImageStudioResizeMode, 1 );
			//выставляем аспект как это делалось в ResizeFrame
			if( NULL != pFrame )
			{
				if (0 == lAspectX && 1 == lAspectY)
				{
					lAspectX = GetSourceWidth();
					lAspectY = GetSourceHeight();
				}
				pFrame->put_AspectRatioX( lAspectX );
				pFrame->put_AspectRatioY( lAspectY );
			}

			if (NULL==pFrame)
				return NULL;
			long lSize = 0;
			pFrame->get_BufferSize(&lSize);
			m_lCacheSize += lSize;
			m_arFrames.Add(pFrame);
			return CreateFramesCopy(pFrame);
		}
	protected:
		MediaCore::IAVSUncompressedVideoFrame *m_pSourceImage;
		
		BSTR	m_bstrMetaData;

		SYSTEMTIME m_oFileLastWriteTime;
		long m_lSourceWidth;
		long m_lSourceHeight;
		CAtlArray<MediaCore::IAVSUncompressedVideoFrame *> m_arFrames;
		long m_lCacheSize;

		void ClearFramesArray()
		{
			size_t nCount = m_arFrames.GetCount();
			for (size_t nIndex = 0; nIndex <  nCount;  nIndex++)
			{
				RELEASEINTERFACE(m_arFrames[nIndex]);
			}
			m_arFrames.RemoveAll();
			m_lCacheSize = 0;
		}
		static IUnknown *CreateFramesCopy(MediaCore::IAVSUncompressedVideoFrame *pSrcFrame)
		{
			if (NULL==pSrcFrame)
				return NULL;
			MediaCore::IAVSMediaData *pMediaData = NULL;
			pSrcFrame->raw_CreateDuplicate(DUBLICATE_TYPE_COPY, &pMediaData);
			if (NULL==pMediaData)
				return NULL;
			IUnknown *punkResult = NULL;
			pMediaData->QueryInterface(__uuidof(IUnknown), (void**)&punkResult);
			pMediaData->Release();
			return punkResult;
		}
		static MediaCore::IAVSUncompressedVideoFrame *ResizeFrame(MediaCore::IAVSUncompressedVideoFrame *pSrcFrame, long lNewWidth, long lNewHeight, long lNewAspectX, long lNewAspectY, long lResizeMode, DWORD dwBackColor)
		{
			MediaCore::IAVSVideoFrameTransform *pFramesTransform = NULL;
			HRESULT hr = CoCreateInstance(MediaCore::CLSID_CAVSVideoFrameTransform, NULL, CLSCTX_ALL, MediaCore::IID_IAVSVideoFrameTransform, (void**)&pFramesTransform);
			if (NULL==pFramesTransform)
				return NULL;
			
			MediaFormat::IAVSVideoFormat *pDstFormat = NULL;
			CoCreateInstance(MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)&pDstFormat);
			if (NULL==pDstFormat)
			{
				pFramesTransform->Release();
				return NULL;
			}

			long lSrcColorSpace = 0;
			pSrcFrame->get_ColorSpace(&lSrcColorSpace);
			pDstFormat->put_ColorSpace(lSrcColorSpace);
			pDstFormat->put_Width(lNewWidth);
			pDstFormat->put_Height(lNewHeight);
			pDstFormat->put_AspectRatioX(lNewAspectX);
			pDstFormat->put_AspectRatioY(lNewAspectY);

			pFramesTransform->put_ResizeMode(lResizeMode);
			pFramesTransform->put_DstBorderGrayScale(dwBackColor);
			pFramesTransform->raw_SetVideoFormat(pDstFormat);

			IUnknown *punkDstFrame = NULL;
			pFramesTransform->raw_TransformFrame(pSrcFrame, &punkDstFrame);
			if (NULL==punkDstFrame)
				return NULL;

			MediaCore::IAVSUncompressedVideoFrame *pDstFrame = NULL;
			punkDstFrame->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pDstFrame);
			punkDstFrame->Release();

			return pDstFrame;
		}
	};
	CAtlMap<CString, CCachedImageStorage *> m_mapImages;

	//
	void CheckCacheSize()
	{
		size_t nSize = CalculateCachedImageStorageSize();
		//ATLTRACE("Cached size: %d\n", nSize);
		if ((m_lMaxCacheSize<0)||(nSize < (size_t)m_lMaxCacheSize))
			return;

		//вначале удаляем исходники, оставляем только картинки уже отмасштабированные
		POSITION pos = m_mapImages.GetStartPosition();
		while (NULL!=pos)
		{
			POSITION pos_cur = pos;
			CCachedImageStorage *pCachedImageStorage = m_mapImages.GetNextValue(pos);
			if ((NULL!=pCachedImageStorage) && (pCachedImageStorage->HasSourceImage()))
			{
				long lCurCacheSize = pCachedImageStorage->GetCacheSize();
				pCachedImageStorage->ResetSourceImage();
				lCurCacheSize -= pCachedImageStorage->GetCacheSize();
				ATLASSERT(lCurCacheSize>=0);
				ATLASSERT(nSize >= (size_t)lCurCacheSize);//?????? т.е. общий размер меньше, чем размер удаленного изображения
				nSize -= lCurCacheSize;
				if (nSize < (size_t)m_lMaxCacheSize)
					return;
				if (0 == pCachedImageStorage->GetCacheSize())
				{
					delete pCachedImageStorage;
					m_mapImages.RemoveAtPos(pos_cur);
				}
			}
		}
		//пробежались и удалили все исходные изображения - не помогла - будем удалять и отмасштабированные
		pos = m_mapImages.GetStartPosition();
		while (NULL!=pos)
		{
			POSITION pos_cur = pos;
			CCachedImageStorage *pCachedImageStorage = m_mapImages.GetNextValue(pos);
			if (NULL!=pCachedImageStorage)
			{
				long lCurCacheSize = pCachedImageStorage->GetCacheSize();
				delete pCachedImageStorage;
				m_mapImages.RemoveAtPos(pos_cur);


				ATLASSERT(lCurCacheSize>=0);
				ATLASSERT(nSize >= (size_t)lCurCacheSize);//?????? т.е. общий размер меньше, чем размер удаленного изображения
				nSize -= lCurCacheSize;
				if (nSize < (size_t)m_lMaxCacheSize)
					return;
			}
		}
	}
	size_t CalculateCachedImageStorageSize()
	{
		size_t nSize = 0;
		POSITION pos = m_mapImages.GetStartPosition();
		while (NULL!=pos)
		{
			CCachedImageStorage *pCachedImageStorage = m_mapImages.GetNextValue(pos);
			if (NULL!=pCachedImageStorage)
				nSize += (size_t)pCachedImageStorage->GetCacheSize();
		}
		return nSize;
	}
	IUnknown *GetImageCopy(CCachedImageStorage *pCachedImageStorage, const CString &sFilePath, long lWidth, long lHeight, long lNewAspectX, long lNewAspectY, long lResizeMode, DWORD dwBackColor)
	{
		if (NULL==pCachedImageStorage)
			return NULL;

		IUnknown *punkImage = NULL;
		if ((-1==lWidth) && (0<lHeight))
		{
			double dZoom = (double)lHeight / (double)pCachedImageStorage->GetSourceHeight();
			lWidth = (long)(dZoom * (double)pCachedImageStorage->GetSourceWidth());
			if (0 == lWidth)
				lWidth = 1;

			lNewAspectX = 0;//lWidth;//pCachedImageStorage->GetSourceWidth();
			lNewAspectY = 1;//lHeight;//pCachedImageStorage->GetSourceHeight();
			//чтобы если картинка не подгрузилась и ушли в ветку с перезагрузкой исходного файла
			//мы имели правильные аспекты
			punkImage = pCachedImageStorage->GetImageCopy(lWidth, lHeight, lNewAspectX, lNewAspectY, m_lResizeMode, m_dwBackColor);
		}
		else if ((0<lWidth) && (-1==lHeight))
		{
			double dZoom = (double)lWidth / (double)pCachedImageStorage->GetSourceWidth();
			lHeight = (long)(dZoom * (double)pCachedImageStorage->GetSourceHeight());
			if (0 == lHeight)
				lHeight = 1;
			lNewAspectX = 0;//lWidth;//pCachedImageStorage->GetSourceWidth();
			lNewAspectY = 1;//lHeight;//pCachedImageStorage->GetSourceHeight();
			//чтобы если картинка не подгрузилась и ушли в ветку с перезагрузкой исходного файла
			//мы имели правильные аспекты
			punkImage = pCachedImageStorage->GetImageCopy(lWidth, lHeight, lNewAspectX, lNewAspectY, m_lResizeMode, m_dwBackColor);
		}
		else if ((lWidth<=0) || (lHeight<=0))
			punkImage = pCachedImageStorage->GetSourceImageCopy();
		else
			punkImage = pCachedImageStorage->GetImageCopy(lWidth, lHeight, lNewAspectX, lNewAspectY, m_lResizeMode, m_dwBackColor);
		if ((NULL==punkImage) && (!pCachedImageStorage->HasSourceImage()))
		{

			IUnknown *punkSourceImage = LoadImageFromFile(sFilePath);
			if (NULL==punkSourceImage)
				return NULL;

			BSTR bstrMetaData =0;	
			ImageStudio::Transforms::Core::COM::COM_LoadMetaData(sFilePath,&bstrMetaData);
			
			pCachedImageStorage->SetImage(punkSourceImage);
			if(bstrMetaData)
			{
				pCachedImageStorage->SetMetaData(bstrMetaData);
				SysFreeString(bstrMetaData);
			}

			punkSourceImage->Release();
			
			if ((lWidth<=0) || (lHeight<=0))
				punkImage = pCachedImageStorage->GetSourceImageCopy();
			else
				punkImage = pCachedImageStorage->GetImageCopy(lWidth, lHeight, lNewAspectX, lNewAspectY, m_lResizeMode, m_dwBackColor);
		}
		return punkImage;
	}
	CCachedImageStorage *GetCachedImageStorage(const CString &sFilePath)
	{
		CCachedImageStorage *pCachedImageStorage = NULL;
		CAtlMap<CString, CCachedImageStorage *>::CPair *pPair = m_mapImages.Lookup(sFilePath);
		if (NULL!=pPair)
		{
			if (NULL!=pPair->m_value)
			{
				pCachedImageStorage = pPair->m_value;
				const SYSTEMTIME &oCachedTime = pCachedImageStorage->GetFileLastWriteTime();
				if (!IsValidSystemTime(oCachedTime))
				{
					delete pCachedImageStorage;
					pCachedImageStorage = NULL;
					m_mapImages.RemoveKey(sFilePath);
				}
				else
				{
					SYSTEMTIME oCurrentTime = GetFileLastWriteTime(sFilePath);
					if (IsValidSystemTime(oCurrentTime))
					{
						if ((oCachedTime.wYear != oCurrentTime.wYear)		||
							(oCachedTime.wMonth != oCurrentTime.wMonth)		||
							(oCachedTime.wDay != oCurrentTime.wDay)			||
							(oCachedTime.wHour != oCurrentTime.wHour)		||
							(oCachedTime.wMinute != oCurrentTime.wMinute)	||
							(oCachedTime.wSecond != oCurrentTime.wSecond))
						{
							delete pCachedImageStorage;
							pCachedImageStorage = NULL;
							m_mapImages.RemoveKey(sFilePath);
						}
					}
				}
			}
			else
				m_mapImages.RemoveKey(sFilePath);
		}
		if (NULL==pCachedImageStorage)
		{
			IUnknown *punkImage = LoadImageFromFile(sFilePath);

			if (NULL==punkImage)
				return NULL;

			pCachedImageStorage = new CCachedImageStorage(GetFileLastWriteTime(sFilePath));
			if (NULL==pCachedImageStorage)
			{
				punkImage->Release();
				return NULL;
			}

			BSTR bstrMetaData =0;	
			ImageStudio::Transforms::Core::COM::COM_LoadMetaData(sFilePath,&bstrMetaData);

			m_mapImages.SetAt(sFilePath, pCachedImageStorage);
			pCachedImageStorage->SetImage(punkImage);
			if (bstrMetaData)
			{
				pCachedImageStorage->SetMetaData(bstrMetaData);
				SysFreeString(bstrMetaData);
			}
			punkImage->Release();
		}
		return pCachedImageStorage;
	}	
	CCachedImageStorage *GetCachedImageStorageExact(const CString &sFilePath)
	{//return only cashed, else - NULL
		CCachedImageStorage *pCachedImageStorage = NULL;
		CAtlMap<CString, CCachedImageStorage *>::CPair *pPair = m_mapImages.Lookup(sFilePath);
		if (NULL!=pPair)
		{
			if (NULL!=pPair->m_value)
			{
				pCachedImageStorage = pPair->m_value;
				const SYSTEMTIME &oCachedTime = pCachedImageStorage->GetFileLastWriteTime();
				if (!IsValidSystemTime(oCachedTime))
				{
					delete pCachedImageStorage;
					pCachedImageStorage = NULL;
					m_mapImages.RemoveKey(sFilePath);
				}
				else
				{
					SYSTEMTIME oCurrentTime = GetFileLastWriteTime(sFilePath);
					if (IsValidSystemTime(oCurrentTime))
					{
						if ((oCachedTime.wYear != oCurrentTime.wYear)		||
							(oCachedTime.wMonth != oCurrentTime.wMonth)		||
							(oCachedTime.wDay != oCurrentTime.wDay)			||
							(oCachedTime.wHour != oCurrentTime.wHour)		||
							(oCachedTime.wMinute != oCurrentTime.wMinute)	||
							(oCachedTime.wSecond != oCurrentTime.wSecond))
						{
							delete pCachedImageStorage;
							pCachedImageStorage = NULL;
							m_mapImages.RemoveKey(sFilePath);
						}
					}
				}
			}
			else
				m_mapImages.RemoveKey(sFilePath);
		}
		return pCachedImageStorage;
	}	
	IUnknown *LoadImageFromFile(const CString &sFilePath)
	{
		IUnknown *punkImage = NULL;
		ImageStudio::Transforms::Core::COM::COM_LoadImage( &punkImage, sFilePath, 0);
		if (NULL==punkImage)
			return NULL;
		CheckCacheSize();
		return punkImage;
	}
	
	void ResetResizedImages()
	{
		POSITION pos = m_mapImages.GetStartPosition();
		while (NULL!=pos)
		{
			POSITION pos_cur = pos;
			CCachedImageStorage *pCachedImageStorage = m_mapImages.GetNextValue(pos);
			if (NULL!=pCachedImageStorage)
			{
				pCachedImageStorage->ResetResizedImage();
				if (!pCachedImageStorage->HasSourceImage())
				{
					delete pCachedImageStorage;
					m_mapImages.RemoveAtPos(pos_cur);
				}
			}
			else
				m_mapImages.RemoveAtPos(pos_cur);
		}
	}
	void ClearMap()
	{
		POSITION pos = m_mapImages.GetStartPosition();
		while (NULL!=pos)
		{
			CCachedImageStorage *pCachedImageStorage = m_mapImages.GetNextValue(pos);
			if (NULL!=pCachedImageStorage)
				delete pCachedImageStorage;
		}
		m_mapImages.RemoveAll();
	}
		
	static SYSTEMTIME GetFileLastWriteTime(const CString &sFilePath)
	{
		HANDLE hFile = CreateFile(sFilePath, FILE_READ_ATTRIBUTES, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		SYSTEMTIME oReturn;
		oReturn.wYear = oReturn.wMonth = oReturn.wDay = oReturn.wHour = oReturn.wMinute = oReturn.wSecond = 0xFFFF;
		if (NULL == hFile)
			return oReturn;
		//FILETIME ftCreationTime;
		//FILETIME ftLastAccessTime,
		FILETIME oLastWriteTime;
		GetFileTime(hFile, NULL, NULL, &oLastWriteTime);
		FileTimeToSystemTime(&oLastWriteTime, &oReturn);
		CloseHandle(hFile);
		return oReturn;
	}
	static BOOL IsValidSystemTime(const SYSTEMTIME &oTime)
	{
		return ((0xFFFF != oTime.wYear)			&&
				(0xFFFF != oTime.wMonth)		&& 
				(0xFFFF != oTime.wDay)			&& 
				(0xFFFF != oTime.wHour)			&& 
				(0xFFFF != oTime.wMinute)		&&
				(0xFFFF != oTime.wSecond));
	}
};
