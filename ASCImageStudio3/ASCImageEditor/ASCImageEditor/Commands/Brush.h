#ifndef __BRUSH_COMM_
#define __BRUSH_COMM_

#include "..//CommandManager.h"
#include "AtlSimpColl.h"

/************************************************************************/
/* BRUSH COMMAND   -----   ID = 104										*/
/************************************************************************/

//#define round(x) floor(x + 0.5)

const int c_nParamIndexLevel_BrushSize		= 0;
const int c_nParamIndexLevel_BrushSoft		= 1;
const int c_nParamIndexLevel_Brightness		= 2;
const int c_nParamIndexLevel_Contrast		= 3;
const int c_nParamIndexLevel_Blur			= 4;
const int c_nParamIndexLevel_Sharpen		= 5;
const int c_nParamIndexLevel_Saturation		= 6;
const int c_nParamIndexLevel_Posterize		= 7;
const int c_nParamIndexLevel_Glow			= 8;
const int c_nParamIndexLevel_Gamma			= 9;
const int c_nParamIndexLevel_Erase			= 10;

//void SaveImage(CString strPath, BYTE* pImage, LONG lWidth, LONG lHeight)
//{
//	ImageFile::IImageFile3Ptr oFile;
//	oFile.CreateInstance(ImageFile::CLSID_ImageFile3);
//
//	MediaCore::IAVSUncompressedVideoFramePtr oFrame;
//	oFrame.CreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame);
//	
//	oFrame->put_ColorSpace(64);
//	
//	oFrame->put_Width(lWidth);
//	oFrame->put_Height(lHeight);
//
//	oFrame->put_AspectRatioX(lWidth);
//	oFrame->put_AspectRatioY(lHeight);
//
//	oFrame->put_Stride(0, 4 * lWidth);
//
//	oFrame->AllocateBuffer(-1);
//
//	BYTE* pBuffer;
//	oFrame->get_Buffer(&pBuffer);
//
//	LONG l;
//	oFrame->get_BufferSize(&l);
//
//	memcpy(pBuffer, pImage, 4 * lWidth * lHeight);
//
//	BSTR bstrFile = strPath.AllocSysString();
//	VARIANT_BOOL bRes = VARIANT_FALSE;
//
//	IUnknown* p = (IUnknown*)oFrame.GetInterfacePtr();
//	oFile->SaveImage2(&p, 3, bstrFile, &bRes);
//	SysFreeString(bstrFile);
//}
//
#define   BRUSH_MOTION_PATH   FALSE

class CBrushMask
{
private:
	enum
	{
		FLAG_UPDATE_NONE = 0,
		FLAG_UPDATE_SIZE = 1,
		FLAG_UPDATE_SOFT = 2,
		FLAG_UPDATE_ALL = FLAG_UPDATE_SIZE | FLAG_UPDATE_SOFT
	};

	BYTE* m_pBuffer;
	int m_nSize;
	int m_nSoft;
	int m_nFlags; // 0 - ok, 1 - resize, 2 - resoft

public:
	enum
	{
		SizeMin = 0,
		SizeMax = 0x7fff,
		SoftMin = 0,
		SoftMax = 100
	};

public:
	CBrushMask()
	{
		m_pBuffer = NULL;
		m_nSize = 0;
		m_nSoft = 0;
		m_nFlags = FLAG_UPDATE_NONE;
	}

	CBrushMask( int size, int soft )
	{
		m_pBuffer = NULL;
		m_nSize = ClampSize( size );
		m_nSoft = ClampSoft( soft );
		m_nFlags = FLAG_UPDATE_ALL;
	}

	CBrushMask( const CBrushMask& out )
	{
		m_pBuffer = NULL;
		m_nSize = out.m_nSize;
		m_nSoft = out.m_nSoft;
		m_nFlags = FLAG_UPDATE_ALL;
	}

	const CBrushMask& operator = (const CBrushMask& out)
	{
		ReleaseBuffer();

		m_nSize = out.m_nSize;
		m_nSoft = out.m_nSoft;
		m_nFlags = FLAG_UPDATE_ALL;

		return *this;
	}

	~CBrushMask()
	{
		if( m_pBuffer )
			delete [] m_pBuffer;
	}

	BYTE* GetBuffer()
	{
		if( m_nFlags == FLAG_UPDATE_NONE )
			return m_pBuffer;

		return Update();
	}

	int GetSize() const
	{
		return m_nSize;
	}

	int GetSoft() const
	{
		return m_nSoft;
	}

	BOOL SetSize( int size )
	{
		size = ClampSize( size );

		if( m_nSize != size )
		{
			m_nSize = size;
			m_nFlags |= FLAG_UPDATE_SIZE;
			return TRUE;
		}
		return FALSE;
	}

	BOOL SetSoft( int soft )
	{
		soft = ClampSoft( soft );

		if( m_nSoft != soft )
		{
			m_nSoft = soft;
			m_nFlags |= FLAG_UPDATE_SOFT;
			return TRUE;
		}
		return FALSE;
	}

private:
	static int ClampSize( int size )
	{
		if( size < SizeMin ) size = SizeMin;
		if( size > SizeMax ) size = SizeMax;

		return size;
	}

	static int ClampSoft( int soft )
	{
		if( soft < SoftMin ) soft = SoftMin;
		if( soft > SoftMax ) soft = SoftMax;

		return soft;
	}

	void ReleaseBuffer()
	{
		if( m_pBuffer )
		{
			delete [] m_pBuffer;
			m_pBuffer = NULL;
		}
	}

	BYTE* Update()
	{
		if( (m_nFlags & FLAG_UPDATE_SIZE) )
		{
			m_nFlags = FLAG_UPDATE_NONE;

			ReleaseBuffer();

			if( m_nSize )
			{
				m_pBuffer = new BYTE[(m_nSize * 2) * (m_nSize * 2)];
				m_nFlags = m_pBuffer ? FLAG_UPDATE_SOFT : FLAG_UPDATE_SIZE;
			}
		}

		if( (m_nFlags & FLAG_UPDATE_SOFT) )
		{
			m_nFlags = FLAG_UPDATE_NONE;

			BYTE* pMask = m_pBuffer;
			int nSize = m_nSize;
			int nSoft = m_nSoft;

			LONG lCount = 2 * nSize;
			double dL = 0;

			double dKoef = 0.75; // вес, определяет притяжение параболы к прямой
			double dKoef2 = 0.4;

			double dVal = (nSoft >= 50) ? dKoef2 : (1 - dKoef2);
			dVal *= nSize;

			double dY = dVal + dKoef * nSize * (nSoft - 50.0) / 50.0;

			double dA = dY / (dVal * dVal - dVal * nSize);
			double dB = 1 - dA * nSize;
			double dFactor = 255.0 / nSize;

			// y = A * x * x + B * x
			int nSqrSize = nSize * nSize;

			for (long y = -nSize; y < nSize; ++y)
			{
				int dy = y * y;

				for (long x = -nSize; x < nSize; ++x, ++pMask)
				{
					int dx = x * x;
					if( (dx + dy) > nSqrSize )
					{
						*pMask = 0;
						continue;
					}
					
					double dL = nSize - sqrt( (double)(dx + dy) );
					//pMask[j * lCount + i] = 127 - (SHORT)((127 - nSoft) * dL / nSize);

					int val = (int)(dFactor * (dA * dL * dL + dB * dL));
					*pMask = max(0, min(255, val));
				}
			}
		}
		
		return m_pBuffer;
	}

};



class CBrush : public IUndoRedoCallback, public ITimerHandler, public IChangesSubscriber
{
//public:
//	enum BrushType
//	{
//		btMin			= 0,
//		btBrightness	= 0,
//		btContrast		= 1,
//		btBlur			= 2,
//		btSharpen		= 3,
//		btSaturation	= 4,
//		btPosterize		= 5,
//		btGlow			= 6,
//		btGamma			= 7,
//		btDiff			= 8,
//		btMax			= 8
//	};


public:	
	class CBrushSession
	{
	private:
		BYTE* m_pData;
		RECT m_oBounds;

	public:
		LONG m_lFirstBlock;
		LONG m_lLastBlock;

	public:
		CBrushSession()
		{
			m_pData = NULL;
		}
		~CBrushSession()
		{
			RELEASEARRAYOBJECTS(m_pData);
		}

	public:
		void Create(BYTE* pImage, const RECT& oBounds, const LONG& lWidthSrc)
		{
			m_oBounds.left		= oBounds.left;
			m_oBounds.top		= oBounds.top;
			m_oBounds.right		= oBounds.right;
			m_oBounds.bottom	= oBounds.bottom;

			if ((m_oBounds.left >= m_oBounds.right) || (m_oBounds.top >= m_oBounds.bottom))
			{
				m_pData = NULL;
				return;
			}

			LONG lWidth		= m_oBounds.right	- m_oBounds.left;
			LONG lHeigth	= m_oBounds.bottom	- m_oBounds.top;

			LONG lStride	= 4 * lWidth;
			LONG lStrideSrc = 4 * lWidthSrc;
			
			m_pData = new BYTE[lStride * lHeigth];

			BYTE* pBufferDst = m_pData;
			BYTE* pBufferSrc = pImage + m_oBounds.top * lStrideSrc + 4 * m_oBounds.left;

			for (long j = 0; j < lHeigth; ++j)
			{
				memcpy(pBufferDst, pBufferSrc, lStride);

				pBufferDst += lStride;
				pBufferSrc += lStrideSrc;
			}
		}

		void Apply(BYTE* pImage, LONG lWidthSrc)
		{
			if (NULL == m_pData)
				return;
			
			LONG lWidth		= m_oBounds.right	- m_oBounds.left;
			LONG lHeigth	= m_oBounds.bottom	- m_oBounds.top;

			LONG lStride	= 4 * lWidth;
			LONG lStrideSrc = 4 * lWidthSrc;

			BYTE* pBufferDst = m_pData;
			BYTE* pBufferSrc = pImage + m_oBounds.top * lStrideSrc + 4 * m_oBounds.left;

			for (long j = 0; j < lHeigth; ++j)
			{
				memcpy(pBufferSrc, pBufferDst, lStride);

				pBufferDst += lStride;
				pBufferSrc += lStrideSrc;
			}
		}
	};
	// классы, для собственной реализации undo-redo
	class CHistoryBlockBase
	{
	public:
		enum HistoryType
		{
			htMemory		= 0,
			htBackground	= 1,
			htLevel			= 2,
			htType			= 3
		};

	public:
		HistoryType m_eType;
	
	public:
		CHistoryBlockBase()
		{
		}
		~CHistoryBlockBase()
		{
		}
	};

	class CHistoryBlockMemory : public CHistoryBlockBase
	{
	public:
		BYTE* m_pDataChanges;
		RECT m_oBounds;

	public:
		CHistoryBlockMemory(BYTE* pChangesData, const RECT& oChangesRect, const LONG& lWidthSrc)
		{
			m_eType = htMemory;
			
			m_oBounds.left		= oChangesRect.left;
			m_oBounds.top		= oChangesRect.top;
			m_oBounds.right		= oChangesRect.right;
			m_oBounds.bottom	= oChangesRect.bottom;

			LONG lWidth		= m_oBounds.right - m_oBounds.left;
			LONG lHeigth	= m_oBounds.bottom - m_oBounds.top;
			
			m_pDataChanges = new BYTE[lWidth * lHeigth];

			BYTE* pBufferDst = m_pDataChanges;
			BYTE* pBufferSrc = pChangesData + m_oBounds.top * lWidthSrc + m_oBounds.left;

			/*IppiSize size;
			size.width = lWidth;
			size.width = lHeigth;
			ippiCopy_8u_C1R(pBufferSrc, lWidthSrc, m_pDataChanges, lWidth, size);*/

			for (long j = 0; j < lHeigth; ++j)
			{
				memcpy(pBufferDst, pBufferSrc, lWidth);

				pBufferDst += lWidth;
				pBufferSrc += lWidthSrc;
			}
		}
		~CHistoryBlockMemory()
		{
			RELEASEOBJECT(m_pDataChanges);
		}

		void Undo(BYTE* pChanges, LONG lWidth, LONG lHeight)
		{
			int nSrcStep = m_oBounds.right - m_oBounds.left;
			int nDstStep = lWidth;
		
			LONG lStrideIndex = lWidth - m_oBounds.right + m_oBounds.left;

			IppiSize size;
			size.width = m_oBounds.right - m_oBounds.left;
			size.height = m_oBounds.bottom - m_oBounds.top;
			
			BYTE* pDst = pChanges + lWidth * m_oBounds.top + m_oBounds.left;
			BYTE* pSrc = m_pDataChanges;

			//ippiSub_8u_C1IRSfs(m_pDataChanges, nSrcStep, pDst, nDstStep, size, 1);
			for (long j = m_oBounds.top; j < m_oBounds.bottom; ++j)
			{
				for (long i = m_oBounds.left; i < m_oBounds.right; ++i, ++pSrc, ++pDst)
				{
					*pDst = *pDst - *pSrc;
				}

				pDst += lStrideIndex;
			}
		}
		void Redo(BYTE* pChanges, LONG lWidth, LONG lHeight)
		{
			int nSrcStep = m_oBounds.right - m_oBounds.left;
			int nDstStep = lWidth;
		
			LONG lStrideIndex = lWidth - m_oBounds.right + m_oBounds.left;

			IppiSize size;
			size.width = m_oBounds.right - m_oBounds.left;
			size.height = m_oBounds.bottom - m_oBounds.top;
			
			BYTE* pDst = pChanges + lWidth * m_oBounds.top + m_oBounds.left;
			BYTE* pSrc = m_pDataChanges;

			//ippiAdd_8u_C1IRSfs(m_pDataChanges, nSrcStep, pDst, nDstStep, size, 1);
			for (long j = m_oBounds.top; j < m_oBounds.bottom; ++j)
			{
				for (long i = m_oBounds.left; i < m_oBounds.right; ++i, ++pSrc, ++pDst)
				{
					*pDst = *pDst + *pSrc;
				}

				pDst += lStrideIndex;
			}
		}
	};

	class CHistoryBlockBackground : public CHistoryBlockBase
	{
	public:
		CHistoryBlockBackground()
		{
			m_eType = htBackground;
		}
		~CHistoryBlockBackground()
		{
		}
	};

	class CHistoryBlockType : public CHistoryBlockBase
	{
	public:
		CBrush::BrushType m_eBrushType;

	public:
		CHistoryBlockType(CBrush::BrushType eBrushType)
		{
			m_eType = htType;
			m_eBrushType = eBrushType;
		}
		~CHistoryBlockType()
		{
		}
	};

	class CHistoryBlockLevel : public CHistoryBlockBase
	{
	public:
		int m_nLevelEffectBrightness;
		int m_nLevelEffectContrast;
		int m_nLevelEffectBlur;
		int m_nLevelEffectSharpen;
		int m_nLevelEffectSaturation;
		int m_nLevelEffectPosterize;
		int m_nLevelEffectGlow;
		int m_nLevelEffectGamma;

	public:
		CHistoryBlockLevel()
		{
			m_eType = htLevel;

			m_nLevelEffectBrightness	= 25;
			m_nLevelEffectContrast		= 0;
			m_nLevelEffectBlur			= 0;
			m_nLevelEffectSharpen		= 0;
			m_nLevelEffectSaturation	= 0;
			m_nLevelEffectPosterize		= 0;
			m_nLevelEffectGlow			= 0;
			m_nLevelEffectGamma			= 0;
		}
		~CHistoryBlockLevel()
		{
		}
	};

	// класс для хранения ректов изменений
	class CBoundManager
	{
	public:
		RECT m_oBounds;

	public:
		CBoundManager()
		{
			Clear();
		}
		~CBoundManager()
		{
		}

	public:
		void AddRECT(RECT oChanges)
		{
			m_oBounds.left		= max(0, min(m_oBounds.left, oChanges.left));
			m_oBounds.top		= max(0, min(m_oBounds.top, oChanges.top));
			m_oBounds.right		= max(m_oBounds.right, oChanges.right);
			m_oBounds.bottom	= max(m_oBounds.bottom, oChanges.bottom);
		}
		void Clear()
		{
			m_oBounds.left		= 0xFFFF;
			m_oBounds.top		= 0xFFFF;
			m_oBounds.right		= 0;
			m_oBounds.bottom	= 0;
		}
		BOOL IsEmpty()
		{
			return ((m_oBounds.right <= m_oBounds.left) && (m_oBounds.bottom <= m_oBounds.top));
		}
	};


public:

	CBrush() : m_arHistory()
	{
		m_lCurrentBlock = -1;
		//CreateMask();

		m_pManager = NULL;

		m_lWidth = 0;
		m_lHeight = 0;

		m_pImageSLOW			= NULL;
		m_pImageBrushSLOW		= NULL;
		m_pImageSLOW_COPY		= NULL;
		m_pImageBrushSLOW_COPY  = NULL;

		m_pImageFAST			= NULL;
		m_pImageBrushFAST		= NULL;

		m_pCurrentCommand		= NULL;
		m_lCurrentSession = -1;

		m_bIsEnabled = true;
		m_bIsRedrawDataMouseMove = false;

		m_bEraseMode = FALSE;

		ClearLevels();
	}
	~CBrush()
	{
		m_pManager = NULL;
		Destroy();
		ClearUndo();
	}

private:
	// флаг для инициализации
	//static BOOL m_bIsMaskInit;
	// маска для БЫСТРОЙ интерполяции
	//static BYTE m_pMaskInterpolation[256 * 256 * 128];
	// маска для БЫСТРОГО учета предыдущего
	//static BYTE m_pMaskChanged[128 * 128];

public:
	// своя история
	CAtlArray<CHistoryBlockBase*> m_arHistory;
	LONG m_lCurrentBlock;

private:
	IAppManager* m_pManager;

private:
	// картинки тоже должны быть здесь...
	BYTE* m_pImageSLOW;				// это - просто указатель
	BYTE* m_pImageBrushSLOW;

	BYTE* m_pImageSLOW_COPY;		// это - просто указатель
	BYTE* m_pImageBrushSLOW_COPY;

	BYTE* m_pImageFirstSLOW_COPY; // необходимо для EraseMode

	LONG m_lWidth;
	LONG m_lHeight;
	
	// FAST...
	BYTE* m_pImageFAST;				// нужен только во время работы браша - т.е. на Destroy - удалять
	BYTE* m_pImageBrushFAST;		// его создание / удаление - по мере требования
	
	LONG m_lWidthFAST;
	LONG m_lHeightFAST;

	// изменения...
	BYTE* m_pChanges;	
	BYTE* m_pChangedCurrent;		// просто указатель
	BYTE* m_pChangesFAST;			// его создание / удаление - по мере необходимости

	// текущие изменения
	CBrush::CBoundManager m_oBounds;
	CBrush::CBoundManager m_oHistoryBounds;

	bool m_bIsThumbnailUpdateFlag;
	bool m_bIsFoneRecalc;

	// текущая команда
	CCommand* m_pCurrentCommand;

	// так... вот это - для undo - redo, когда не находимся в том браше
	CAtlArray<CBrushSession*> m_arSessions;

	LONG m_lLastBlock;
	LONG m_lCurrentSession;

	BOOL m_bWaitSlowCopy;

public:
	
	//int m_nLevel;					// уровень эффекта
	//int m_nLevelEffect;			// -100..100

	// значения эффекта для каждого типа ------------------------
	int m_nLevelBrightness;
	int m_nLevelBrightnessOld;
	int m_nLevelEffectBrightness;

	int m_nLevelContrast;
	int m_nLevelContrastOld;
	int m_nLevelEffectContrast;

	int m_nLevelBlur;
	int m_nLevelBlurOld;
	int m_nLevelEffectBlur;

	int m_nLevelSharpen;
	int m_nLevelSharpenOld;
	int m_nLevelEffectSharpen;

	int m_nLevelSaturation;
	int m_nLevelSaturationOld;
	int m_nLevelEffectSaturation;

	int m_nLevelPosterize;
	int m_nLevelPosterizeOld;
	int m_nLevelEffectPosterize;

	int m_nLevelGlow;
	int m_nLevelGlowOld;
	int m_nLevelEffectGlow;

	int m_nLevelGamma;
	int m_nLevelGammaOld;
	int m_nLevelEffectGamma;
	
	int m_bEraseMode;

	BOOL m_bUpdateLevels;

	bool m_bIsEmptyParameters;

	// ---------------------------------------------------------

	bool m_bWait;

	// теперь флаг для Enabled
	bool m_bIsEnabled;
	bool m_bIsRedrawDataMouseMove;

	BOOL SetBrightness( int level )
	{
		if( m_nLevelBrightness != level )
		{
			m_nLevelBrightness = level;
			m_bUpdateLevels = TRUE;
			return TRUE;
		}
		return FALSE;
	}

	BOOL SetContrast( int level )
	{
		if( m_nLevelContrast != level )
		{
			m_nLevelContrast = level;
			m_bUpdateLevels = TRUE;
			return TRUE;
		}
		return FALSE;
	}

	BOOL SetBlur( int level )
	{
		if( m_nLevelBlur != level )
		{
			m_nLevelBlur = level;
			m_bUpdateLevels = TRUE;
			return TRUE;
		}
		return FALSE;
	}

	BOOL SetSharpen( int level )
	{
		if( m_nLevelSharpen != level )
		{
			m_nLevelSharpen = level;
			m_bUpdateLevels = TRUE;
			return TRUE;
		}
		return FALSE;
	}

	BOOL SetSaturation( int level )
	{
		if( m_nLevelSaturation != level )
		{
			m_nLevelSaturation = level;
			m_bUpdateLevels = TRUE;
			return TRUE;
		}
		return FALSE;
	}

	BOOL SetPosterize( int level )
	{
		if( m_nLevelPosterize != level )
		{
			m_nLevelPosterize = level;
			m_bUpdateLevels = TRUE;
			return TRUE;
		}
		return FALSE;
	}

	BOOL SetGlow( int level )
	{
		if( m_nLevelGlow != level )
		{
			m_nLevelGlow = level;
			m_bUpdateLevels = TRUE;
			return TRUE;
		}
		return FALSE;
	}

	BOOL SetGamma( int level )
	{
		if( m_nLevelGamma != level )
		{
			m_nLevelGamma = level;
			m_bUpdateLevels = TRUE;
			return TRUE;
		}
		return FALSE;
	}


public:

	void ClearLevels()
	{
		m_bIsEmptyParameters		= true;
		
		m_nLevelEffectBrightness	= 25;
		m_nLevelEffectContrast		= 0;
		m_nLevelEffectBlur			= 0;
		m_nLevelEffectSharpen		= 0;
		m_nLevelEffectSaturation	= 0;
		m_nLevelEffectPosterize		= 0;
		m_nLevelEffectGlow			= 0;
		m_nLevelEffectGamma			= 0;

		LevelEffectToLevel();
	}

	virtual void OnUndo(int nStepCount, bool Reset)
	{
		if (!m_bIsEnabled)
			return;

		if (-1 == m_lCurrentBlock)
			return;

		Enable(false);
		
		m_pManager->GetLayersManager()->GetCurrentLayer()->SetFreezeLayer(true);
		
		// определим, в какую сессию мы попали
		LONG lOldBlock = m_lCurrentBlock;
		
		CheckCurrentSession();
		LONG lSessionOld = m_lCurrentSession;

		m_lCurrentBlock -= nStepCount;
		CheckCurrentSession();
		LONG lSessionNew = m_lCurrentSession;
		
		LONG lFirstBlock = CheckFirstBlock();

		BOOL bIsFound = FALSE;

		BOOL bIsRecalc = FALSE;
		if (lSessionOld != lSessionNew)
		{
			// попали на другую сессию...
			// сначала определим, какой уловень и тип (чтоб два раза не применять эффект)
			
			CHistoryBlockLevel oBlockLevel;
			
			for (int i = m_lCurrentBlock; i > lFirstBlock; --i)
			{
				if (NULL != m_arHistory[i])
				{
					if (CHistoryBlockBase::htLevel == m_arHistory[i]->m_eType)
					{
						CHistoryBlockLevel* pBlockLevel = static_cast<CHistoryBlockLevel*>(m_arHistory[i]);
						
						if (NULL != pBlockLevel)
						{
							bIsRecalc = CheckEqualLevels(pBlockLevel);
							break;
						}
					}
				}
				if (i == (lFirstBlock + 1))
				{
					bIsRecalc = CheckEqualLevels(&oBlockLevel);
				}
			}

			// возьмем теперь SLOW - и применим к нему блок без изменений
			m_pImageSLOW = const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, m_lWidth, m_lHeight));
			
			if (-1 != m_lCurrentSession)
			{
				m_arSessions[m_lCurrentSession]->Apply(m_pImageSLOW, m_lWidth);
			}
			else
			{
				// значит попали в первую
				m_arSessions[0]->Apply(m_pImageSLOW, m_lWidth);
			}

			if (Reset)
			{
				if ((-1 != m_lCurrentSession) && (m_lCurrentSession < (LONG)m_arSessions.GetCount()))
				{
					m_lCurrentBlock = m_arSessions[m_lCurrentSession]->m_lFirstBlock;
				}
				RecalcSlowCopy();
				m_pManager->GetLayersManager()->GetCurrentLayer()->SetFreezeLayer(false);
				Enable(true);
				return;
			}
			
			// нужно пересчитать SLOW_COPY - делаем это сами, так как заморозили слой
			RecalcSlowCopy();
			Init2();

			// не выходим, так как нужно применить все блоки
			bIsFound = TRUE;
		}

		if (Reset)
		{
			if ((-1 != m_lCurrentSession) && (m_lCurrentSession < (LONG)m_arSessions.GetCount()))
			{
				if (IsBad())
				{
					m_pImageSLOW		= const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, m_lWidth, m_lHeight));
					m_pImageSLOW_COPY	= const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW_COPY, m_lWidth, m_lHeight));
				}
				
				m_arSessions[m_lCurrentSession]->Apply(m_pImageSLOW, m_lWidth);
				m_lCurrentBlock = m_arSessions[m_lCurrentSession]->m_lFirstBlock;
				RecalcSlowCopy();
			}
			
			m_pManager->GetLayersManager()->GetCurrentLayer()->SetFreezeLayer(false);
			Enable(true);
			return;
		}

		// здесь все картинки у нас готовы, все что нужно - пересчитано.
		bIsRecalc = FALSE;
		CBoundManager oManager;
		
		if (!bIsFound)
		{
			CHistoryBlockLevel oBlockLevel;
			
			for (int i = m_lCurrentBlock; i > lFirstBlock; --i)
			{
				if (NULL != m_arHistory[i])
				{
					if (CHistoryBlockBase::htLevel == m_arHistory[i]->m_eType)
					{
						CHistoryBlockLevel* pBlockLevel = static_cast<CHistoryBlockLevel*>(m_arHistory[i]);
						
						if (NULL != pBlockLevel)
						{
							bIsRecalc = CheckEqualLevels(pBlockLevel);
							break;
						}
					}
				}

				if (i == (lFirstBlock + 1))
				{
					bIsRecalc = CheckEqualLevels(&oBlockLevel);
				}
			}

			for (LONG lIndex = lOldBlock; lIndex > m_lCurrentBlock; --lIndex)
			{
				if (CHistoryBlockBase::htMemory == m_arHistory[lIndex]->m_eType)
				{
					CHistoryBlockMemory* pBlock = static_cast<CHistoryBlockMemory*>(m_arHistory[lIndex]);
					if (NULL != pBlock)
					{
						pBlock->Undo(m_pChanges, m_lWidth, m_lHeight);
					}
				}
			}

			for (LONG i = lFirstBlock + 1; i <= m_lCurrentBlock; ++i)
			{
				if (CHistoryBlockBase::htMemory == m_arHistory[i]->m_eType)
				{
					CHistoryBlockMemory* pMem = static_cast<CHistoryBlockMemory*>(m_arHistory[i]);
					if (NULL != pMem)
					{
						oManager.AddRECT(pMem->m_oBounds);
					}
				}
			}

			// если текущего браша нету, то нужно заапдейтить SLOW
			if (m_lCurrentSession >= 0 && (m_lCurrentSession < (LONG)m_arSessions.GetCount()))
			{
				m_arSessions[m_lCurrentSession]->Apply(m_pImageSLOW, m_lWidth);
			}

			if (!bIsRecalc)
			{
				RecalcSlowCopy();
				if (!oManager.IsEmpty())
				{
					ApplyChangedTypeOnScan0(oManager.m_oBounds);
				}
			}
			else
			{
				//LevelEffectToLevel(); - это уже сделано
				ChangeType();
			}
		}
		else
		{
			// попали в новую сессию - всегда будем изменения посылать
			// вот тут посмотреть!!!
			//FireEvent(2, m_nLevelEffect);
			//FireEvent(0, (int)m_eType);

			for (LONG i = lFirstBlock + 1; i <= m_lCurrentBlock; ++i)
			{
				if (CHistoryBlockBase::htMemory == m_arHistory[i]->m_eType)
				{
					CHistoryBlockMemory* pMem = static_cast<CHistoryBlockMemory*>(m_arHistory[i]);
					if (NULL != pMem)
					{
						oManager.AddRECT(pMem->m_oBounds);
						pMem->Redo(m_pChanges, m_lWidth, m_lHeight);
					}
				}
			}

			if (!oManager.IsEmpty())
			{
				ApplyChangedTypeOnScan0(oManager.m_oBounds);
			}
		}

		BOOL bIsApplyToSLOW = (NULL == m_pCurrentCommand);
		if (bIsApplyToSLOW)
		{
			UpdateBounds(m_oBounds, lFirstBlock, m_lCurrentBlock);
			ApplyChangedTypeOnSLOW();
			m_pManager->GetLayersManager()->GetCurrentLayer()->SetFreezeLayer(false);
		}

		Enable(true);

		UpdateThumbnail(TRUE);
		m_pManager->GetView()->InvalidateView(true);
		return;
	}

	virtual void OnRedo(int nStepCount)
	{
		if (!m_bIsEnabled)
			return;
		
		Enable(false);
		
		m_pManager->GetLayersManager()->GetCurrentLayer()->SetFreezeLayer(true);
		
		// определим, в какую сессию мы попали
		LONG lOldBlock = m_lCurrentBlock;
		
		CheckCurrentSession(TRUE);
		LONG lSessionOld = m_lCurrentSession;

		m_lCurrentBlock += nStepCount;
		CheckCurrentSession(TRUE);
		LONG lSessionNew = m_lCurrentSession;

		// теперь поверим - может это переход на last??
		if (lSessionOld != -1)
		{
			LONG lLast = m_arSessions[lSessionOld]->m_lLastBlock;
			if (lLast == m_lCurrentBlock)
			{
				m_lCurrentSession	= lSessionOld;
				lSessionNew			= lSessionOld;
			}
		}
		
		LONG lFirstBlock = CheckFirstBlock();

		BOOL bIsFound = FALSE;

		if (lSessionOld != lSessionNew)
		{
			// попали на другую сессию...
			// сначала определим, какой уловень и тип (чтоб два раза не применять эффект)
			CHistoryBlockLevel oBlockLevel;
			
			for (int i = m_lCurrentBlock; i > lFirstBlock; --i)
			{
				if (NULL != m_arHistory[i])
				{
					if (CHistoryBlockBase::htLevel == m_arHistory[i]->m_eType)
					{
						CHistoryBlockLevel* pBlockLevel = static_cast<CHistoryBlockLevel*>(m_arHistory[i]);
						
						if (NULL != pBlockLevel)
						{
							CheckEqualLevels(pBlockLevel);
							break;
						}
					}
				}

				if (i == (lFirstBlock + 1))
				{
					CheckEqualLevels(&oBlockLevel);
				}
			}

			// возьмем теперь SLOW - и применим к нему блок без изменений
			if (NULL != m_pCurrentCommand)
			{
				if (-1 != lSessionOld)
				{
					CBoundManager oMan;
					UpdateBounds(oMan, m_arSessions[lSessionOld]->m_lFirstBlock, m_arSessions[lSessionOld]->m_lLastBlock);
					ApplyChangedTypeOnSLOW(oMan.m_oBounds);
				}
			}
			m_pImageSLOW = const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, m_lWidth, m_lHeight));
			
			if (-1 != m_lCurrentSession)
			{
				m_arSessions[m_lCurrentSession]->Apply(m_pImageSLOW, m_lWidth);
			}
			
			// нужно пересчитать SLOW_COPY - делаем это сами, так как заморозили слой
			RecalcSlowCopy();
			Init2();

			// не выходим, так как нужно применить все блоки
			bIsFound = TRUE;
		}
		
		BOOL bIsRecalc = FALSE;
		CBoundManager oManager;
		
		if (!bIsFound)
		{
			if (IsBad())
			{
				Init2();
			}

			CHistoryBlockLevel oBlockLevel;
			
			// найдем параметры браша
			for (int i = m_lCurrentBlock; i > lFirstBlock; --i)
			{
				if (NULL != m_arHistory[i])
				{
					if (CHistoryBlockBase::htLevel == m_arHistory[i]->m_eType)
					{
						CHistoryBlockLevel* pBlockLevel = static_cast<CHistoryBlockLevel*>(m_arHistory[i]);
						
						if (NULL != pBlockLevel)
						{
							bIsRecalc = CheckEqualLevels(pBlockLevel);
							break;
						}
					}
				}

				if (i == (lFirstBlock + 1))
				{
					bIsRecalc = CheckEqualLevels(&oBlockLevel);
				}
			}

			for (LONG lIndex = lOldBlock + 1; lIndex <= m_lCurrentBlock; ++lIndex)
			{
				if (CHistoryBlockBase::htMemory == m_arHistory[lIndex]->m_eType)
				{
					CHistoryBlockMemory* pBlock = static_cast<CHistoryBlockMemory*>(m_arHistory[lIndex]);
					if (NULL != pBlock)
					{
						if (bIsRecalc)
						{
							pBlock->Redo(m_pChanges, m_lWidth, m_lHeight);
						}
						else
						{
							ApplyToImageRedo(pBlock);
						}
					}
				}
			}

			// если текущего браша нету, то нужно заапдейтить SLOW
			if (m_lCurrentSession >= 0 && (m_lCurrentSession < (LONG)m_arSessions.GetCount()))
			{
				m_arSessions[m_lCurrentSession]->Apply(m_pImageSLOW, m_lWidth);
			}

			if (bIsRecalc)
			{
				UpdateBounds(m_oBounds, lFirstBlock, m_lCurrentBlock);

				LevelEffectToLevel();
				ChangeType();
			}
		}
		else
		{
			// попали в новую сессию - всегда будем изменения посылать
			// вот тут посмотреть!!!
			//FireEvent(2, m_nLevelEffect);
			//FireEvent(0, (int)m_eType);

			for (LONG i = lFirstBlock + 1; i <= m_lCurrentBlock; ++i)
			{
				if (CHistoryBlockBase::htMemory == m_arHistory[i]->m_eType)
				{
					CHistoryBlockMemory* pMem = static_cast<CHistoryBlockMemory*>(m_arHistory[i]);
					if (NULL != pMem)
					{
						oManager.AddRECT(pMem->m_oBounds);
						pMem->Redo(m_pChanges, m_lWidth, m_lHeight);
					}
				}
			}

			if (!oManager.IsEmpty())
			{
				ApplyChangedTypeOnScan0(oManager.m_oBounds);
			}
		}

		BOOL bIsApplyToSLOW = (NULL == m_pCurrentCommand);
		if (bIsApplyToSLOW)
		{
			UpdateBounds(m_oBounds, lFirstBlock, m_lCurrentBlock);
			ApplyChangedTypeOnSLOW();
			m_pManager->GetLayersManager()->GetCurrentLayer()->SetFreezeLayer(false);			
		}

		Enable(true);

		UpdateThumbnail(TRUE);
		m_pManager->GetView()->InvalidateView(true);
		return;
	}

	virtual void ClearRedo()
	{		
		RemoveRedoSessions();
		
		if (m_lCurrentBlock >= ((LONG)m_arHistory.GetCount() - 1))
			return;

		size_t nCount = m_arHistory.GetCount() - m_lCurrentBlock - 1;

		for (LONG nIndex = m_lCurrentBlock + 1; nIndex < (LONG)m_arHistory.GetCount(); ++nIndex)
		{
			CHistoryBlockBase* pBase = m_arHistory[nIndex];
			RELEASEOBJECT(pBase);
		}

		if (0 < nCount)
		{
			m_arHistory.RemoveAt(m_lCurrentBlock + 1, nCount);
		}
	}

	virtual void ClearUndo()
	{
		RemoveUndoSessions();
		
		for (size_t nIndex = 0; nIndex < m_arHistory.GetCount(); ++nIndex)
		{
			CHistoryBlockBase* pBase = m_arHistory[nIndex];
			RELEASEOBJECT(pBase);
		}

		m_lCurrentBlock = -1;
		m_arHistory.RemoveAll();

		// вот надо ли??
		Destroy();
	}

public:

	//void ApplyBrushToSourceImage(BrushType eType, int lLevel, BYTE* pImage, LONG lWidth, LONG lHeight)
	//{
	//	switch (eType)
	//	{
	//	case btBrightness:
	//		{
	//			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustBrightness(pImage, lWidth, lHeight, 1.0, lLevel);
	//			break;
	//		}
	//	case btContrast:
	//		{
	//			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustContrast(pImage, lWidth, lHeight, 1.0, lLevel);
	//			break;
	//		}
	//	case btBlur:
	//		{
	//			//ImageStudio::Transforms::Core::BGRA::BGRA_EffectBlur(pImage, lWidth, lHeight, 1.0, lLevel);
	//			ImageStudio::Transforms::Core::BGRA::BGRA_IPPEffectGaussianBlur(pImage, lWidth, lHeight, 1.0, lLevel);
	//			break;
	//		}
	//	case btSharpen:
	//		{
	//			ImageStudio::Transforms::Core::BGRA::BGRA_EffectSharpen(pImage, lWidth, lHeight, 1.0, lLevel);
	//			break;
	//		}
	//	case btSaturation:
	//		{
	//			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustSaturation(pImage, lWidth, lHeight, 1.0, lLevel);
	//			break;
	//		}
	//	case btPosterize:
	//		{
	//			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustPosterize(pImage, lWidth, lHeight, 1.0, lLevel);
	//			break;
	//		}
	//	case btGlow:
	//		{
	//			ImageStudio::Transforms::Core::BGRA::BGRA_EffectGlow(pImage, lWidth, lHeight, 1.0, lLevel);
	//			break;
	//		}
	//	case btGamma:
	//		{
	//			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustGamma(pImage, lWidth, lHeight, 1.0, lLevel);
	//			break;
	//		}
	//		
	//	default:
	//		break;
	//	};
	//}

	void ApplyAllBrushes(BYTE* pImage, LONG lWidth, LONG lHeight)
	{
		if (0 != m_nLevelBrightness)
		{
			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustBrightness(pImage, lWidth, lHeight, 1.0, m_nLevelBrightness);
		}
		if (0 != m_nLevelContrast)
		{
			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustContrast(pImage, lWidth, lHeight, 1.0, m_nLevelContrast);
		}
		if (0 != m_nLevelBlur)
		{
			ImageStudio::Transforms::Core::BGRA::BGRA_IPPEffectGaussianBlur2(pImage, lWidth, lHeight, 1.0, m_nLevelBlur);
		}
		if (0 != m_nLevelSharpen)
		{
			ImageStudio::Transforms::Core::BGRA::BGRA_EffectSharpen(pImage, lWidth, lHeight, 1.0, m_nLevelSharpen);
		}
		if (0 != m_nLevelSaturation)
		{
			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustSaturation(pImage, lWidth, lHeight, 1.0, m_nLevelSaturation);
		}
		//if (0 != m_nLevelPosterize)
		if (0 != m_nLevelEffectPosterize)
		{
			// у нас идет пересчет. ноль - это не ноль, а 32
			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustPosterize(pImage, lWidth, lHeight, 1.0, m_nLevelPosterize);
		}
		//if (0 != m_nLevelGlow)
		if (0 != m_nLevelEffectGlow)
		{
			// у нас идет пересчет. ноль - это не ноль, а 1
			ImageStudio::Transforms::Core::BGRA::BGRA_EffectGlow(pImage, lWidth, lHeight, 1.0, m_nLevelGlow);		
		}
		if (0 != m_nLevelGamma)
		{
			ImageStudio::Transforms::Core::BGRA::BGRA_AdjustGamma(pImage, lWidth, lHeight, 1.0, m_nLevelGamma);
		}
	}

	//void ApplyChanges3(BYTE* pImageSrc1, BYTE* pImageSrc2, BYTE* pImageDst, CHAR* pChanges, LONG lWidthImage, RECT oRect)
	//{
	//	LONG lW = oRect.right - oRect.left;
	//	LONG lH = oRect.bottom - oRect.top;
	//	
	//	LONG lSkipC = lWidthImage * oRect.top + oRect.left;
	//	LONG lSkipTop = 4 * lSkipC;
	//	
	//	BYTE* pDst  = pImageDst  + lSkipTop;
	//	BYTE* pSrc1 = pImageSrc1 + lSkipTop;
	//	BYTE* pSrc2 = pImageSrc2 + lSkipTop;

	//	CHAR* pMask = pChanges + lSkipC;

	//	LONG lPitchChange = lWidthImage - lW;
	//	LONG lPitchImage = 4 * lPitchChange;
	//		
	//	for (LONG j = 0; j < lH; ++j)
	//	{
	//		for (LONG i = 0; i < lW; ++i, pSrc1 += 4, pSrc2 += 4, pDst += 4, ++pMask)
	//		{
	//			Update(pSrc1, pSrc2, pDst, *pMask);
	//		}
	//		
	//		pSrc1 += lPitchImage;
	//		pSrc2 += lPitchImage;
	//		pDst  += lPitchImage;

	//		pMask += lPitchChange;
	//	}
	//}

	void ApplyChanges3(BYTE* pImageDst, const BYTE* pImageSrc, const BYTE* pChanges, LONG lWidthImage, const RECT& oRect)
	{
		LONG lW = oRect.right - oRect.left;
		LONG lH = oRect.bottom - oRect.top;
		
		LONG lSkipC = lWidthImage * oRect.top + oRect.left;
		LONG lSkipTop = 4 * lSkipC;
		
		BYTE* pDst = pImageDst + lSkipTop;
		const BYTE* pSrc = pImageSrc + lSkipTop;
		const BYTE* pMask = pChanges + lSkipC;

		LONG lPitchChange = lWidthImage - lW;
		LONG lPitchImage = 4 * lPitchChange;
			
		for (LONG j = 0; j < lH; ++j, pDst += lPitchImage, pSrc += lPitchImage, pMask += lPitchChange )
		{
			for (LONG i = 0; i < lW; ++i, pSrc += 4, pDst += 4, ++pMask)
			{
				unsigned sa = pMask[0] * 0x8081u;
				unsigned da = 255 * 0x8081u - sa;

				for( int c = 0; c < 3; c++ )
				{
					pDst[c] = (BYTE)((pDst[c] * da + pSrc[c] * sa + 127 * 0x8081u) >> 23);
				}
			}
		}
	}

	//void ApplyChanges4(BYTE* pImageSrc1, BYTE* pImageSrc2, BYTE* pImageDst, CHAR* pChanges, LONG lWidthImage, RECT oRect)
	//{
	//	LONG lW = oRect.right - oRect.left;
	//	LONG lH = oRect.bottom - oRect.top;
	//	
	//	LONG lSkipC = lWidthImage * oRect.top + oRect.left;
	//	LONG lSkipTop = 4 * lSkipC;
	//	
	//	BYTE* pDst  = pImageDst  + lSkipTop;
	//	BYTE* pSrc1 = pImageSrc1 + lSkipTop;
	//	BYTE* pSrc2 = pImageSrc2 + lSkipTop;

	//	CHAR* pMask = pChanges + lSkipC;

	//	LONG lPitchChange = lWidthImage - lW;
	//	LONG lPitchImage = 4 * lPitchChange;
	//		
	//	for (LONG j = 0; j < lH; ++j)
	//	{
	//		for (LONG i = 0; i < lW; ++i, pSrc1 += 4, pSrc2 += 4, pDst += 4, ++pMask)
	//		{
	//			Update4(pSrc1, pSrc2, pDst, *pMask);
	//		}
	//		
	//		pSrc1 += lPitchImage;
	//		pSrc2 += lPitchImage;
	//		pDst  += lPitchImage;

	//		pMask += lPitchChange;
	//	}
	//}

public:
	BOOL SetLastBrushLevel()
	{
		CHistoryBlockLevel oBlockLevel;

		for (LONG lIndex = (LONG)m_lCurrentBlock; lIndex >= 0; --lIndex)
		{
			if (CHistoryBlockBase::htLevel == m_arHistory[lIndex]->m_eType)
			{
				CHistoryBlockLevel* pLevel = static_cast<CHistoryBlockLevel*>(m_arHistory[lIndex]);
				if (NULL != pLevel)
				{
					return CheckEqualLevels(pLevel);
				}
			}
		}
		
		return CheckEqualLevels(&oBlockLevel);
	}

	BOOL CheckEqualLevels(CHistoryBlockLevel* pBlock)
	{
		#define CHECK_EQUAL(TYPE)														\
		if (m_nLevelEffect##TYPE != pBlock->m_nLevelEffect##TYPE)						\
		{																				\
			m_nLevelEffect##TYPE = pBlock->m_nLevelEffect##TYPE;						\
			FireEvent(c_nParamIndexLevel_##TYPE, m_nLevelEffect##TYPE);					\
			bIsRecalc = TRUE;															\
		}
		
		BOOL bIsRecalc = FALSE;
		CHECK_EQUAL(Brightness)
		CHECK_EQUAL(Contrast)
		CHECK_EQUAL(Blur)
		CHECK_EQUAL(Sharpen)
		CHECK_EQUAL(Saturation)
		CHECK_EQUAL(Posterize)
		CHECK_EQUAL(Glow)
		CHECK_EQUAL(Gamma)

		if (bIsRecalc)
		{
			LevelEffectToLevel();
		}
		return bIsRecalc;
	}

public:
	// вспомогательные функции быcтрой интерполяции
	//void CreateMask()
	//{
	//	if (!m_bIsMaskInit)
	//	{
	//		// маска для изменений
	//		BYTE* pBufferMem = m_pMaskChanged;
	//		for (int nNew = 0; nNew < 128; ++nNew)
	//		{
	//			for (int nOld = 0; nOld < 128; ++nOld)
	//			{
	//				*pBufferMem = (BYTE)(min(127, (128 * (double)(abs(nNew - nOld)) / (128 - nOld))));
	//				++pBufferMem;
	//			}
	//		}

	//		// маска для интерполирования
	//		BYTE* pBuffer = m_pMaskInterpolation;
	//		for (int nSrc1 = 0; nSrc1 < 256; ++nSrc1)
	//		{
	//			for (int nSrc2 = 0; nSrc2 < 256; ++nSrc2)
	//			{
	//				for (int nW = 0; nW < 128; ++nW)
	//				{
	//					double dW = (double)nW / 128.0;
	//					BYTE val = (BYTE)((1 - dW) * nSrc1 + dW * nSrc2);
	//					
	//					*pBuffer = val;
	//					++pBuffer;
	//				}
	//			}
	//		}

	//		m_bIsMaskInit = TRUE;
	//	}
	//}

	//void Update(BYTE* pData1, BYTE* pData2, BYTE* pDst, const LONG& lWeight) const
	//{
	//	BYTE nW = m_pMaskChanged[lWeight];

	//	pDst[0] = m_pMaskInterpolation[((pData1[0] << 8 | pData2[0]) << 7 | nW)];
	//	pDst[1] = m_pMaskInterpolation[((pData1[1] << 8 | pData2[1]) << 7 | nW)];
	//	pDst[2] = m_pMaskInterpolation[((pData1[2] << 8 | pData2[2]) << 7 | nW)];
	//}
	//void Update(BYTE* pData1, BYTE* pData2, const BYTE& nNewWeight, const BYTE& lOldWeight) const
	//{
	//	BYTE nW = m_pMaskChanged[nNewWeight << 7 | lOldWeight];

	//	pData1[0] = m_pMaskInterpolation[((pData1[0] << 8 | pData2[0]) << 7 | nW)];
	//	pData1[1] = m_pMaskInterpolation[((pData1[1] << 8 | pData2[1]) << 7 | nW)];
	//	pData1[2] = m_pMaskInterpolation[((pData1[2] << 8 | pData2[2]) << 7 | nW)];
	//}
	//void Update(BYTE* pDst, BYTE* pSrc, const CHAR& nNewWeight, const CHAR& nOldWeight, CHAR& lCurChanged) const
	//{
	//	BYTE nW = m_pMaskChanged[nNewWeight << 7 | nOldWeight];

	//	pDst[0] = m_pMaskInterpolation[((pDst[0] << 8 | pSrc[0]) << 7 | nW)];
	//	pDst[1] = m_pMaskInterpolation[((pDst[1] << 8 | pSrc[1]) << 7 | nW)];
	//	pDst[2] = m_pMaskInterpolation[((pDst[2] << 8 | pSrc[2]) << 7 | nW)];

	//	lCurChanged += (nNewWeight - nOldWeight);
	//}
	//void Update2(BYTE* pDst, BYTE* pSrc, const CHAR& nNewWeight, const CHAR& nOldWeight, CHAR& lCurChanged) const
	//{
	//	BYTE nW = m_pMaskChanged[nNewWeight << 7];

	//	pDst[0] = m_pMaskInterpolation[((pDst[0] << 8 | pSrc[0]) << 7 | nW)];
	//	pDst[1] = m_pMaskInterpolation[((pDst[1] << 8 | pSrc[1]) << 7 | nW)];
	//	pDst[2] = m_pMaskInterpolation[((pDst[2] << 8 | pSrc[2]) << 7 | nW)];

	//	lCurChanged += (nNewWeight - nOldWeight);
	//}
	
	//static void UpdateErase(BYTE* pDst, const BYTE* pSrc1, const BYTE* pSrc2, BYTE nNewWeight)
	//{
	//	// nNewWeight = [0..127]
	//	BYTE nW = m_pMaskChanged[nNewWeight << 7];

	//	pDst[0] = m_pMaskInterpolation[((pSrc1[0] << 8 | pSrc2[0]) << 7 | nW)];
	//	pDst[1] = m_pMaskInterpolation[((pSrc1[1] << 8 | pSrc2[1]) << 7 | nW)];
	//	pDst[2] = m_pMaskInterpolation[((pSrc1[2] << 8 | pSrc2[2]) << 7 | nW)];
	//}
	//static void Update(BYTE* pDst, const BYTE* pSrc, BYTE nWeight)
	//{
	//	// nWeight = [0..127]
	//	BYTE nW = m_pMaskChanged[nNewWeight << 7];

	//	pDst[0] = m_pMaskInterpolation[((pDst[0] << 8 | pSrc[0]) << 7 | nW)];
	//	pDst[1] = m_pMaskInterpolation[((pDst[1] << 8 | pSrc[1]) << 7 | nW)];
	//	pDst[2] = m_pMaskInterpolation[((pDst[2] << 8 | pSrc[2]) << 7 | nW)];
	//}

	//static void Update4(BYTE* pDst, BYTE* pSrc, const BYTE& nWeight)
	//{
	//	BYTE nW = m_pMaskChanged[nWeight << 7];

	//	pDst[0] = m_pMaskInterpolation[((pDst[0] << 8 | pSrc[0]) << 7 | nW)];
	//	pDst[1] = m_pMaskInterpolation[((pDst[1] << 8 | pSrc[1]) << 7 | nW)];
	//	pDst[2] = m_pMaskInterpolation[((pDst[2] << 8 | pSrc[2]) << 7 | nW)];
	//	pDst[3] = pSrc[3];
	//}

	//static void Update4(BYTE* pData1, BYTE* pData2, BYTE* pDst, const LONG& lWeight)
	//{
	//	BYTE nW = m_pMaskChanged[lWeight];

	//	pDst[0] = m_pMaskInterpolation[((pData1[0] << 8 | pData2[0]) << 7 | nW)];
	//	pDst[1] = m_pMaskInterpolation[((pData1[1] << 8 | pData2[1]) << 7 | nW)];
	//	pDst[2] = m_pMaskInterpolation[((pData1[2] << 8 | pData2[2]) << 7 | nW)];
	//	pDst[3] = pData1[3];
	//}
	
	//inline static BYTE Interpolate(BYTE nSource1, BYTE nSource2, double dFrame/*0..1*/)
	//{
	//	return (BYTE)(nSource1 * (1.0 - dFrame) + nSource2 * dFrame);
	//}

	//inline static BYTE Interpolate_(BYTE nSource1, BYTE nSource2, int Frame /*0..256*/)
	//{
	//	return (((nSource2 - nSource1) * Frame + 127 ) >> 8) + nSource1;
	//}

	//inline static void Interpolate3(BYTE* pResult, BYTE* pSource1, BYTE* pSource2, double dFrame/*0..1*/)
	//{
	//	pResult[0] = Interpolate(pSource1[0], pSource2[0], dFrame);
	//	pResult[1] = Interpolate(pSource1[1], pSource2[1], dFrame);
	//	pResult[2] = Interpolate(pSource1[2], pSource2[2], dFrame);
	//}
	
	//inline static void Interpolate4(BYTE* pResult, BYTE* pSource1, BYTE* pSource2, double dFrame/*0..1*/)
	//{
	//	pResult[0] = Interpolate(pSource1[0], pSource2[0], dFrame);
	//	pResult[1] = Interpolate(pSource1[1], pSource2[1], dFrame);
	//	pResult[2] = Interpolate(pSource1[2], pSource2[2], dFrame);
	//	pResult[3] = pSource1[3];
	//}

	//inline static void Interpolate3_(BYTE* pResult, BYTE* pSource1, BYTE* pSource2, int Frame/*0..256*/)
	//{
	//	pResult[0] = Interpolate_(pSource1[0], pSource2[0], Frame);
	//	pResult[1] = Interpolate_(pSource1[1], pSource2[1], Frame);
	//	pResult[2] = Interpolate_(pSource1[2], pSource2[2], Frame);
	//}
	
	//inline static void Interpolate4_(BYTE* pResult, BYTE* pSource1, BYTE* pSource2, int Frame/*0..256*/)
	//{
	//	pResult[0] = Interpolate_(pSource1[0], pSource2[0], Frame);
	//	pResult[1] = Interpolate_(pSource1[1], pSource2[1], Frame);
	//	pResult[2] = Interpolate_(pSource1[2], pSource2[2], Frame);
	//	pResult[3] = pSource1[3];
	//}

	//inline static float GetDistance1(float d1, float d2, float dNorm)
	//{
	//	return 1 - sqrt(d1 * d1 + d2 * d2) / dNorm;
	//}
	//inline static float GetDistance2(float d1, float d2, float dNorm)
	//{
	//	return 1 - (d1 * d1 + d2 * d2) / dNorm;
	//}

	//inline static LONG round_(double dVal)
	//{
	//	return (LONG)(2 * dVal) - (LONG)dVal;
	//}

public:
	void ApplyBrushToSourceImage()
	{
		if ((NULL == m_pImageSLOW) || (NULL == m_pImageBrushSLOW))
			return;

		memcpy(m_pImageBrushSLOW, m_pImageSLOW, 4 * m_lWidth * m_lHeight);
		//ApplyBrushToSourceImage(m_eType, m_nLevel, m_pImageBrushSLOW, m_lWidth, m_lHeight);
		ApplyAllBrushes(m_pImageBrushSLOW, m_lWidth, m_lHeight);
	}
	void ApplyBrushToThumbnailImage(BYTE* pThumbnail)
	{
		if (NULL == pThumbnail)
			return;

		//ApplyBrushToSourceImage(m_eType, m_nLevel, pThumbnail, m_lWidthFAST, m_lHeightFAST);
		ApplyAllBrushes(pThumbnail, m_lWidthFAST, m_lHeightFAST);
	}
	void RecalcSlowCopy()
	{
		if ((NULL == m_pManager) || (NULL == m_pImageSLOW) || (NULL == m_pImageSLOW_COPY))
			return;

		LONG lW = 0;
		LONG lH = 0;
		m_pImageSLOW_COPY = m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW_COPY, lW, lH);

		if ((lW != m_lWidth) || (lH != m_lHeight))
		{
			// падение при резете
			return;
		}
		
		memcpy(m_pImageSLOW_COPY, m_pImageSLOW, 4 * m_lWidth * m_lHeight);
		m_pManager->ApplyBackgroundCommands(m_pImageSLOW_COPY, m_lWidth, m_lHeight);

		if (NULL != m_pImageFirstSLOW_COPY)
		{
			memcpy(m_pImageFirstSLOW_COPY, m_pImageSLOW_COPY, 4 * m_lWidth * m_lHeight);
		}
	}
	void RecalcSlowCopyWithBrush()
	{
		if ((NULL == m_pManager) || (NULL == m_pImageBrushSLOW) || (NULL == m_pImageBrushSLOW_COPY))
			return;

		memcpy(m_pImageBrushSLOW_COPY, m_pImageBrushSLOW, 4 * m_lWidth * m_lHeight);
		m_pManager->ApplyBackgroundCommands(m_pImageBrushSLOW_COPY, m_lWidth, m_lHeight);
	}
	void RecalcSlowCopyWithoutBrush()
	{
		if ((NULL == m_pManager) || (NULL == m_pImageSLOW) || (NULL == m_pImageFirstSLOW_COPY))
			return;

		memcpy(m_pImageFirstSLOW_COPY, m_pImageSLOW, 4 * m_lWidth * m_lHeight);
		m_pManager->ApplyBackgroundCommands(m_pImageFirstSLOW_COPY, m_lWidth, m_lHeight);
	}

	void ApplyChangedTypeOnScan0(RECT oRectChanges)
	{
		if ((NULL == m_pImageSLOW_COPY) || (NULL == m_pImageBrushSLOW_COPY))
			return;

		ApplyChanges3(m_pImageSLOW_COPY, m_pImageBrushSLOW_COPY, m_pChanges, m_lWidth, oRectChanges);
	}
	void ApplyChangedTypeOnSLOW()
	{
		if ((NULL == m_pImageSLOW) || (NULL == m_pImageBrushSLOW_COPY) || (m_oBounds.IsEmpty()))
			return;

		ApplyChanges3(m_pImageSLOW, m_pImageBrushSLOW, m_pChanges, m_lWidth, m_oBounds.m_oBounds);
	}

	void CorrectFAST()
	{
		LONG lW = 0; LONG lH = 0;
		BYTE* pFAST = const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, lW, lH));
		ResizeBYTE4(m_pImageSLOW, pFAST, m_lWidth, m_lHeight, lW, lH);
	}

	void ApplyChangedTypeOnSLOW(RECT oRect)
	{
		if ((NULL == m_pImageSLOW) || (NULL == m_pImageBrushSLOW_COPY) || (oRect.left >= oRect.right) || (oRect.top >= oRect.bottom))
			return;

		ApplyChanges3(m_pImageSLOW, m_pImageBrushSLOW, m_pChanges, m_lWidth, oRect);
	}

	// здесь применение маски. вот эти методы нужно оптимизировать !!!
	void ApplyMask(const BYTE* pMask, LONG lSize, LONG lX, LONG lY, BOOL bIsMouseMove, LONG lMouseOldX, LONG lMouseOldY)
	{
		if (m_bIsEmptyParameters || !pMask)
			return;

		// координаты - в большой картинке (центр)
		LONG left	= lX - lSize;
		LONG top	= lY - lSize;
		LONG right	= lX + lSize;
		LONG bottom = lY + lSize;

		if( right <= 0 || bottom <= 0 || left >= m_lWidth || top >= m_lHeight )
			return;

		if( left   < 0 ) left = 0;
		if( top    < 0 ) top  = 0;
		if( right  > m_lWidth  ) right  = m_lWidth;
		if( bottom > m_lHeight ) bottom = m_lHeight;

		BOOL bIsHistory = FALSE;

		LONG lYCorrect = max(0, lSize - lY);
		LONG lXCorrect = max(0, lSize - lX);

		//if (bIsMouseMove && BRUSH_MOTION_PATH)
		//{
		//	ApplyMaskWeight(m_pImageSLOW_COPY, m_pImageBrushSLOW_COPY, lMouseOldX, lMouseOldY, lX, lY, pMask, nSize);
		//	bIsHistory = true;
		//}
		//else
		{
			pMask += lYCorrect * lSize * 2 + lXCorrect;
			BYTE* pImageDst  = m_pImageSLOW_COPY;
			BYTE* pImageSrc1 = m_pImageFirstSLOW_COPY;
			BYTE* pImageSrc2 = m_pImageBrushSLOW_COPY;
			BYTE* pChanges   = m_pChanges;
			BYTE* pChangedCurrent = m_pChangedCurrent;

			long lStride = m_lWidth - (right - left);
			long lIndex  = top * m_lWidth + left;

			for (LONG j = top; j < bottom; ++j, lIndex += lStride, pMask += lSize * 2 )
			{
				const BYTE* pSrcMask = pMask - lIndex;

				for (LONG i = left; i < right; ++i, ++lIndex )
				{
					BYTE nChange = pChanges[lIndex];
					BYTE nMask   = pSrcMask[lIndex];

					bool bDelta = (!m_bEraseMode) ? (nMask > nChange) : (nMask = ~nMask, nChange > nMask);
					if( bDelta )
					{
						pChanges[lIndex] = nMask;
						pChangedCurrent[lIndex] += nMask - nChange;
						bIsHistory = TRUE;

						unsigned sa = (unsigned)nMask * 0x8081u;
						unsigned da = 255 * 0x8081u - sa;

						for( int c = 0; c < 3; c++ )
						{
							pImageDst[lIndex * 4 + c] = (BYTE)((pImageSrc1[lIndex * 4 + c] * da + pImageSrc2[lIndex * 4 + c] * sa + 127 * 0x8081u) >> 23);
						}
					}
				}
			}
		}

		if (bIsHistory)
		{
			RECT oChangesRect = {left, top, right, bottom };

			m_oHistoryBounds.AddRECT(oChangesRect);
		}
	}

	//void ApplyMaskWeight(BYTE* pDstImage, BYTE* pSrcImage, LONG lPointOldX, LONG lPointOldY, LONG lPointNewX, LONG lPointNewY, BYTE* pMask, int nSize)
	//{
	//	long dx = lPointNewX - lPointOldX;
	//	long dy = lPointNewY - lPointOldY;

	//	if( !dx && !dy )
	//		return;
	//	
	//	double dDistance = sqrt( (double)(dx)*dx + (double)(dy)*dy );

	//	double dCos = dx / dDistance;
	//	double dSin = dy / dDistance;

	//	LONG lDistance = (LONG)dDistance;
	//	LONG lStride = 4 * m_lWidth;

	//	// теперь будем в цикле бежать по направлению вектора смещения
	//	// причем применять можно сразу в двух направлениях (симметрия относительно прямой движения)

	//	double dPointX = lPointOldX + dCos;
	//	double dPointY = lPointOldY + dSin;

	//	int nMaxDistance = ceil( dDistance );
	//	
	//	// сначала отдельно для не симметричного отрезка ------------------------------------------------------------
	//	for (int nIndex = 1; nIndex < nMaxDistance /*dDistance*/; ++nIndex)
	//	{
	//		dPointX += dCos;
	//		dPointY += dSin;
	//		
	//		LONG lPointX = (LONG)round(dPointX);//(LONG)dPointX;
	//		LONG lPointY = (LONG)round(dPointY);//(LONG)dPointY;

	//		if (!IsInImage(lPointX, lPointY))
	//			continue;
	//		
	//		LONG lIndexChange = m_lWidth * lPointY + lPointX;
	//		
	//		CHAR& lChange = m_pChanges[lIndexChange];
	//		if (lChange == 127) // нет измененний
	//			continue;

	//		m_pChangedCurrent[lIndexChange] += (127 - lChange);
	//		lChange = 127;
	//		
	//		// тут максимум - ничего быстрее мемспу - не сделаешь
	//		LONG index = 4 * lIndexChange;
	//		memcpy(pDstImage + index, pSrcImage + index, 3);
	//	}

	//	dPointX = lPointNewX;
	//	dPointY = lPointNewY;
	//	
	//	BYTE* pCurMask = pMask + 2 * nSize * nSize + nSize;
	//	for (int nIndex = 0; nIndex < nSize; ++nIndex)
	//	{
	//		dPointX += dCos;
	//		dPointY += dSin;
	//					
	//		LONG lPointX = (LONG)round(dPointX);//(LONG)dPointX;
	//		LONG lPointY = (LONG)round(dPointY);//(LONG)dPointY;

	//		if (0 == *pCurMask)
	//			break;

	//		if (IsInImage(lPointX, lPointY))
	//		{
	//			LONG lIndex = m_lWidth * lPointY + lPointX;

	//			CHAR& lChange		= m_pChanges[lIndex];
	//			CHAR& lCurChange	= m_pChangedCurrent[lIndex];

	//			if (lChange < *pCurMask)
	//			{
	//				LONG lIndexMem = 4 * lIndex;

	//				Update(pDstImage + lIndexMem, pSrcImage + lIndexMem, *pCurMask, lChange, lCurChange);

	//				lChange = *pCurMask;
	//			}
	//		}

	//		++pCurMask;
	//	}
	//	// ----------------------------------------------------------------------------------------------------------

	//	// теперь для симмертичной части ----------------------------------------------------------------------------
	//	double dPointX1 = lPointOldX; 
	//	double dPointX2 = lPointOldX;
	//	double dPointY1 = lPointOldY;
	//	double dPointY2 = lPointOldY;

	//	for (int nIndex = 1; nIndex < nSize; ++nIndex)
	//	{
	//		dPointX1 += dSin;
	//		dPointX2 -= dSin;

	//		dPointY1 -= dCos;
	//		dPointY2 += dCos;

	//		/*dPointX1 = round(dPointX1 + dSin);
	//		dPointX2 = round(dPointX2 - dSin);

	//		dPointY1 = round(dPointY1 - dCos);
	//		dPointY2 = round(dPointY2 + dCos);*/
	//		
	//		pCurMask = pMask + 2 * nSize * (nSize + nIndex) + nSize;

	//		double dPointX1Mem = dPointX1;
	//		double dPointY1Mem = dPointY1;

	//		double dPointX2Mem = dPointX2;
	//		double dPointY2Mem = dPointY2;

	//		for (int nIndexMem = 1; nIndexMem < dDistance; ++nIndexMem)
	//		{
	//			LONG lX1 = (LONG)round(dPointX1Mem);//(LONG)dPointX1Mem;
	//			LONG lX2 = (LONG)round(dPointX2Mem);//(LONG)dPointX2Mem;

	//			LONG lY1 = (LONG)round(dPointY1Mem);//(LONG)dPointY1Mem;
	//			LONG lY2 = (LONG)round(dPointY2Mem);//(LONG)dPointY2Mem;

	//			if (0 == *pCurMask)
	//				break;
	//			
	//			if (IsInImage(lX1, lY1))
	//			{
	//				LONG lIndex = m_lWidth * lY1 + lX1;
	//				
	//				CHAR& lChange		= m_pChanges[lIndex];
	//				CHAR& lCurChange	= m_pChangedCurrent[lIndex];

	//				if (lChange < *pCurMask)
	//				{
	//					LONG lIndexMem = 4 * lIndex;
	//					
	//					Update(pDstImage + lIndexMem, pSrcImage + lIndexMem, *pCurMask, lChange, lCurChange);

	//					lChange = *pCurMask;
	//				}
	//			}

	//			if (IsInImage(lX2, lY2))
	//			{
	//				LONG lIndex = m_lWidth * lY2 + lX2;
	//				
	//				CHAR& lChange		= m_pChanges[lIndex];
	//				CHAR& lCurChange	= m_pChangedCurrent[lIndex];

	//				if (lChange < *pCurMask)
	//				{
	//					LONG lIndexMem = 4 * lIndex;
	//					
	//					Update(pDstImage + lIndexMem, pSrcImage + lIndexMem, *pCurMask, lChange, lCurChange);

	//					lChange = *pCurMask;
	//				}
	//			}
	//							
	//			dPointX1Mem += dCos;
	//			dPointX2Mem += dCos;

	//			dPointY1Mem += dSin;
	//			dPointY2Mem += dSin;
	//		}
	//		
	//		for (int nIndex = 0; nIndex < nSize; ++nIndex)
	//		{
	//			LONG lX1 = (LONG)round(dPointX1Mem);//(LONG)dPointX1Mem;
	//			LONG lX2 = (LONG)round(dPointX2Mem);//(LONG)dPointX2Mem;

	//			LONG lY1 = (LONG)round(dPointY1Mem);//(LONG)dPointY1Mem;
	//			LONG lY2 = (LONG)round(dPointY2Mem);//(LONG)dPointY2Mem;

	//			if (0 == *pCurMask)
	//				break;
	//			
	//			if (IsInImage(lX1, lY1))
	//			{
	//				LONG lIndex = m_lWidth * lY1 + lX1;
	//				
	//				CHAR& lChange		= m_pChanges[lIndex];
	//				CHAR& lCurChange	= m_pChangedCurrent[lIndex];

	//				if (lChange < *pCurMask)
	//				{
	//					LONG lIndexMem = 4 * lIndex;
	//					
	//					Update(pDstImage + lIndexMem, pSrcImage + lIndexMem, *pCurMask, lChange, lCurChange);

	//					lChange = *pCurMask;
	//				}
	//			}

	//			if (IsInImage(lX2, lY2))
	//			{
	//				LONG lIndex = m_lWidth * lY2 + lX2;
	//				
	//				CHAR& lChange		= m_pChanges[lIndex];
	//				CHAR& lCurChange	= m_pChangedCurrent[lIndex];

	//				if (lChange < *pCurMask)
	//				{
	//					LONG lIndexMem = 4 * lIndex;
	//					
	//					Update(pDstImage + lIndexMem, pSrcImage + lIndexMem, *pCurMask, lChange, lCurChange);

	//					lChange = *pCurMask;
	//				}
	//			}

	//			dPointX1Mem += dCos;
	//			dPointX2Mem += dCos;

	//			dPointY1Mem += dSin;
	//			dPointY2Mem += dSin;

	//			++pCurMask;
	//		}
	//	}
	//	// ----------------------------------------------------------------------------------------------------------
	//}

	//inline BYTE* CreatePath(BYTE** ppData, const double& dSin, const double& dCos, const int& nSize)
	//{
	//	return NULL;
	//}

	// timer
	virtual void OnTimer(void)	
	{
		if (NULL == m_pManager)
			return;
		
		m_pManager->GetTimer()->StopTimer(this);

		RELEASEARRAYOBJECTS(m_pImageBrushFAST);
		ChangeType();
		
		m_bWait = false;

		DumpToHistoryLevel();
		
		m_pManager->GetLayersManager()->GetCurrentLayer()->SetActualImage(ILayer::SLOW_COPY);
		m_pManager->GetView()->InvalidateView(true);

		m_bIsRedrawDataMouseMove = true;
	}

	// changeshandler
	virtual bool BeforeChangesHandler(int changingObject)
	{
		if (NULL == m_pManager)
			return true;
		
		if (m_bIsThumbnailUpdateFlag)
			return true;

		if (ILayer::SLOW == changingObject)
		{
			// изменилась главная картинка...
			// тут нужно сделать ReInit() - в случае, когда изиенение - не из-за фоновой (анду)
			// и просто RecalcSlowcopy, если из фоновой
			m_bWaitSlowCopy = true;
			return true;
		}
		
		if (m_oBounds.IsEmpty())
		{
			return true;
		}
		
		if (ILayer::FAST_COPY == changingObject)
		{
			// опа... тут надо посмотреть, отсылали ли мы уже изменения или нет
			if (m_bIsFoneRecalc)
			{
				// тут нужно сделать следующее: подменить слоу - и все,
				// главное, не забыть потом где лежит слоу...
				memcpy(m_pImageBrushSLOW_COPY, m_pImageSLOW, 4 * m_lWidth * m_lHeight);
				ApplyChanges3(m_pImageSLOW, m_pImageBrushSLOW, m_pChanges, m_lWidth, m_oBounds.m_oBounds);

				LONG lW = 0; LONG lH = 0;
				BYTE* pFAST = const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, lW, lH));
				ResizeBYTE4(m_pImageSLOW, pFAST, m_lWidth, m_lHeight, lW, lH);

				//m_pManager->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::FAST);
				BYTE* pFAST_COPY = const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST_COPY, lW, lH));
				memcpy(pFAST_COPY, pFAST, 4 * lW * lH);
				m_pManager->ApplyBackgroundCommands(pFAST_COPY, lW, lH);

				// чтобы сразу фаст_сопи показался
				//BGRA_TransformResize_(m_pSourceImage, m_lWidth, m_lHeight, pFAST, lW, lH, 0, 0, (double)lW / m_lWidth, 0);
			}
			m_bIsFoneRecalc = false;

			return true;
		}

		return true;
	}

	virtual void ChangesHandler(int changingObject, bool repaint)
	{
		if (NULL == m_pManager)
			return;
		
		if (NULL == m_pImageSLOW)
		{
			return;
		}

		if (changingObject == ILayer::SLOW)
		{
			// тут надо наверное полный рестарт???
			if (m_bWaitSlowCopy)
			{
				Enable(false);
				return;
			}

			Init2();
			return;
		}
		
		if (changingObject == ILayer::FAST_COPY)
		{
			Enable(false);
			m_bWait = true;
		}
		else if (changingObject == ILayer::SLOW_COPY)
		{
			if (m_bWaitSlowCopy)
			{
				// до этого изменился SLOW - 
				// надо посмотреть что делать...
				
				LONG lWidth		= -1;
				LONG lHeight	= -1;
				BYTE* pSLOW = m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, lWidth, lHeight);

				if ((pSLOW != m_pImageSLOW) || (lWidth != m_lWidth) || (lHeight != m_lHeight))
				{
					Init(m_pManager, FALSE);
					m_pManager->GetView()->InvalidateView(true);
				}
				else
				{
					RecalcSlowCopy();
					ApplyBrushToSourceImage();

					RecalcSlowCopyWithBrush();

					ApplyChangedTypeOnScan0(m_oBounds.m_oBounds);

					m_pManager->GetLayersManager()->GetCurrentLayer()->SetActualImage(ILayer::SLOW_COPY);

					UpdateThumbnail(TRUE);
					m_pManager->GetView()->InvalidateView(true);
				}

				m_bWaitSlowCopy = false;

				Enable(true);
				return;
			}

			m_bWait = false;

			// возвращаем слоу на место
			if (!m_bIsFoneRecalc)
			{
				memcpy(m_pImageSLOW, m_pImageBrushSLOW_COPY, 4 * m_lWidth * m_lHeight);
				m_bIsFoneRecalc = true;

				RecalcSlowCopyWithoutBrush();
				RecalcSlowCopyWithBrush();
						
				Enable(true);
				return;
			}

			Enable(true);
			m_pManager->GetLayersManager()->GetCurrentLayer()->SetActualImage(ILayer::SLOW_COPY);
			m_pManager->GetView()->InvalidateView(true);
		}
	}

	// полные пересчеты...
	void UpdateLevel()
	{
		//if (m_oBounds.IsEmpty())
		//{
		//	ApplyBrushToSourceImage();
		//	RecalcSlowCopyWithBrush();

		//	return;
		//}

		// наверное здесь без thumbnail'a и другого потока - не обойтись
		// но по сути - это тоже самое, что и ChangeType
		m_bWait = true;
		m_pManager->GetLayersManager()->GetCurrentLayer()->SetActualImage(ILayer::FAST_COPY);
		
		CreateFASTChanges_LEVEL();
		UpdateThumbnail(FALSE);

		m_pManager->GetTimer()->StartTimer(this, 1000);//1000
		
		if( !m_oBounds.IsEmpty() )
		{
			m_pManager->GetView()->InvalidateView(true);
		}
	}

	void ChangeType(bool bIsRedraw = false)
	{
		Enable(false);
		
		ApplyBrushToSourceImage(); 
		
		RecalcSlowCopy();
		RecalcSlowCopyWithBrush();

		ApplyChangedTypeOnScan0(m_oBounds.m_oBounds);
		
		Enable(true);
		m_bIsRedrawDataMouseMove = true;
		
		if (bIsRedraw)
		{
			m_pManager->GetView()->InvalidateView(true);
		}
	}
	
	void UpdateThumbnail(BOOL bAttack)
	{
		// нужно пересчитать табнейл и гистограмму
		if (bAttack)
		{
			LONG lW = 0; LONG lH = 0;
			BYTE* pFAST_COPY = const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST_COPY, lW, lH));
			BGRA_TransformResize_(m_pImageSLOW_COPY, m_lWidth, m_lHeight, pFAST_COPY, lW, lH, 0, 0, (double)lW / m_lWidth, 0);
		}
		
		m_bIsThumbnailUpdateFlag = true;
		bool bFreeze = m_pManager->GetLayersManager()->GetCurrentLayer()->GetFreezeLayer();

		m_pManager->GetLayersManager()->GetCurrentLayer()->SetFreezeLayer(false);
		m_pManager->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::FAST_COPY, NULL, true, false);
		m_pManager->GetLayersManager()->GetCurrentLayer()->SetFreezeLayer(bFreeze);

		m_bIsThumbnailUpdateFlag = false;

		m_pManager->SendMsg(WM_IEM_UPDATE_THUMBNAIL, NULL);
	}

	void CreateFASTChanges_LEVEL()
	{
		if (m_oBounds.IsEmpty())
			return;

		m_pImageFAST     = m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, m_lWidthFAST, m_lHeightFAST);
		BYTE* pFAST_COPY = m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST_COPY, m_lWidthFAST, m_lHeightFAST);
		
		LONG lSize = 4 * m_lWidthFAST * m_lHeightFAST;
		if (NULL == m_pImageBrushFAST)
		{
			// вообще это нужно не всегда. Надо следить была ли применена фоновая
			ResizeBYTE4(m_pImageSLOW, m_pImageFAST, m_lWidth, m_lHeight, m_lWidthFAST, m_lHeightFAST);
			m_pManager->ApplyBackgroundCommands(m_pImageFAST, m_lWidthFAST, m_lHeightFAST);
			
			m_pImageBrushFAST = new BYTE[lSize];
			ResizeBYTE1(m_pChanges, m_pChangesFAST, m_lWidth, m_lHeight, m_lWidthFAST, m_lHeightFAST);
		}
		memcpy(m_pImageBrushFAST, m_pImageFAST, lSize);
		memcpy(pFAST_COPY, m_pImageFAST, lSize);

		ApplyBrushToThumbnailImage(m_pImageBrushFAST);
		m_pManager->ApplyBackgroundCommands(m_pImageBrushFAST, m_lWidthFAST, m_lHeightFAST);

		double dScaleX = (double)m_lWidthFAST / m_lWidth;
		double dScaleY = (double)m_lHeightFAST / m_lHeight;

		RECT oRectChanges;
		oRectChanges.left   = (LONG)(dScaleX * m_oBounds.m_oBounds.left);
		oRectChanges.top    = (LONG)(dScaleY * m_oBounds.m_oBounds.top);
		oRectChanges.right  = (LONG)(dScaleX * m_oBounds.m_oBounds.right);
		oRectChanges.bottom = (LONG)(dScaleY * m_oBounds.m_oBounds.bottom);

		ApplyChanges3(pFAST_COPY, m_pImageBrushFAST, m_pChangesFAST, m_lWidthFAST, oRectChanges);
	}

	// common
	//inline BOOL IsInImage(LONG lX, LONG lY)
	//{
	//	return (lX >= 0) && (lX < m_lWidth) && (lY >= 0) && (lY < m_lHeight);
	//}

	void ResizeBYTE4(const BYTE* pSource, BYTE* pResult, LONG lSrcW, LONG lSrcH, LONG lDstW, LONG lDstH)
	{
		IppiSize sizeSrc = {lSrcW, lSrcH};
		IppiSize sizeDst = {lDstW, lDstH};
		IppiRect srcRect = {0, 0, lSrcW, lSrcH};
		double dScale = ((double)(lDstW)) / lSrcW;

		ippiResize_8u_AC4R(pSource, sizeSrc, lSrcW*4, 
			srcRect, pResult, 4*lDstW, sizeDst, dScale, dScale, IPPI_INTER_LINEAR);
	}
	void ResizeBYTE1(const BYTE* pSource, BYTE* pResult, LONG lSrcW, LONG lSrcH, LONG lDstW, LONG lDstH)
	{
		IppiSize sizeSrc = {lSrcW, lSrcH};
		IppiSize sizeDst = {lDstW, lDstH};
		IppiRect srcRect = {0, 0, lSrcW, lSrcH};
		double dScale = ((double)(lDstW)) / lSrcW;

		ippiResize_8u_C1R(pSource, sizeSrc, lSrcW, 
			srcRect, pResult, lDstW, sizeDst, dScale, dScale, IPPI_INTER_LINEAR);
	}

	//void ResizeSHORT(SHORT* pSource, SHORT* pResult, LONG lSrcW, LONG lSrcH, LONG lDstW, LONG lDstH)
	//{
	//	double dScaleX = (double)lSrcW / lDstW;
	//	double dScaleY = (double)lSrcH / lDstH;

	//	for (LONG j = 0; j < lDstH; ++j)
	//	{
	//		LONG lStartDstY = j * lDstW;
	//		LONG lStartSrcY = ((LONG)(dScaleY * j)) * lSrcW;
	//		for (LONG i = 0; i < lDstW; ++i)
	//		{
	//			LONG lX = (LONG)(dScaleX * i);
	//			pResult[lStartDstY + i] = pSource[lStartSrcY + lX];
	//		}
	//	}
	//}

	// сброс в историю
	void DumpToHistoryTHIS()
	{
		m_lLastBlock = m_lCurrentBlock;
		
		UpdateThumbnail(TRUE);
		
		m_pManager->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
		m_pManager->GetLayersManager()->GetCurrentLayer()->ChangeManually(this);
		m_pManager->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock(_T("Brush"));
	}

	void DumpToHistoryBlock()
	{
		ClearRedo();

		if (m_oHistoryBounds.IsEmpty())
			return;
		
		m_oBounds.AddRECT(m_oHistoryBounds.m_oBounds);
		m_arHistory.Add(new CHistoryBlockMemory(m_pChangedCurrent, m_oHistoryBounds.m_oBounds, m_lWidth));

		// и обнулим изменения
		int nDstStep = m_lWidth;
		IppiSize size;
		size.width = m_oHistoryBounds.m_oBounds.right - m_oHistoryBounds.m_oBounds.left;
		size.height = m_oHistoryBounds.m_oBounds.bottom - m_oHistoryBounds.m_oBounds.top;

		BYTE* pBuffer = m_pChangedCurrent + m_lWidth * m_oHistoryBounds.m_oBounds.top + m_oHistoryBounds.m_oBounds.left;
		ippiSet_8u_C1R(0, pBuffer, nDstStep, size);
		//memset(m_pChangedCurrent, 0, m_lWidth * m_lHeight);
		
		m_oHistoryBounds.Clear();

		++m_lCurrentBlock;
		DumpToHistoryTHIS();
	}
	void DumpToHistoryLevel()
	{
		ClearRedo();

		CHistoryBlockLevel* pBlock = new CHistoryBlockLevel();
		
		pBlock->m_nLevelEffectBrightness	= m_nLevelEffectBrightness;
		pBlock->m_nLevelEffectContrast		= m_nLevelEffectContrast;
		pBlock->m_nLevelEffectBlur			= m_nLevelEffectBlur;
		pBlock->m_nLevelEffectSharpen		= m_nLevelEffectSharpen;
		pBlock->m_nLevelEffectSaturation	= m_nLevelEffectSaturation;
		pBlock->m_nLevelEffectPosterize		= m_nLevelEffectSaturation;
		pBlock->m_nLevelEffectGlow			= m_nLevelEffectGlow;
		pBlock->m_nLevelEffectGamma			= m_nLevelEffectGamma;
		
		m_arHistory.Add(pBlock);
		++m_lCurrentBlock;
		DumpToHistoryTHIS();
	}
	//void DumpToHistoryBackground()
	//{
	//	ClearRedo();

	//	m_arHistory.Add(new CHistoryBlockBackground());
	//	++m_lCurrentBlock;
	//	DumpToHistoryTHIS();
	//}

	//void DumpToHistoryAll()
	//{
	//	// сначала удалим весь реду
	//	ClearRedo();

	//	if (m_oBounds.IsEmpty())
	//		return;

	//	// счас для быстроты сделаем без путей, а просто
	//	LONG lWidthC = m_oBounds.m_oBounds.right - m_oBounds.m_oBounds.left;
	//	LONG lHeightC = m_oBounds.m_oBounds.bottom - m_oBounds.m_oBounds.top;
	//	LONG lStrideC = 4 * lWidthC;

	//	LONG lSkipTop = 4 * (m_lWidth * m_oBounds.m_oBounds.top + m_oBounds.m_oBounds.left);
	//	LONG lSkipC = m_lWidth * m_oBounds.m_oBounds.top + m_oBounds.m_oBounds.left;
	//	
	//	BYTE* pBuffer = new BYTE[lStrideC * lHeightC];
	//		
	//	for (LONG j = 0; j < lHeightC; ++j)
	//	{
	//		for (LONG i = 0; i < lWidthC; ++i)
	//		{
	//			//m_oBrush.Interpolate4_(
	//			//	&pBuffer[j * lStrideC + 4 * i], &m_pSourceImage[lSkipTop + 4 * i], 
	//			//	&m_pSourceImageWithBrush[lSkipTop + 4 * i], 
	//			//	m_pChanges[lSkipC + i]);
	//			
	//			Update(m_pImageSLOW + lSkipTop + 4 * i, m_pImageBrushSLOW + lSkipTop + 4 * i, pBuffer + j * lStrideC + 4 * i, m_pChanges[lSkipC + i]);
	//		}
	//		lSkipC += m_lWidth;
	//		lSkipTop += 4 * m_lWidth;
	//	}

	//	// и посылаем эти изменения наверх
	//	m_pManager->GetLayersManager()->GetCurrentLayer()->StartHistoryBlock();
	//	m_pManager->GetLayersManager()->GetCurrentLayer()->ChangePartOfLayer(pBuffer, lWidthC, lHeightC, 
	//							m_oBounds.m_oBounds.left, m_oBounds.m_oBounds.top);

	//	RELEASEARRAYOBJECTS(pBuffer);
	//	
	//	m_pManager->GetLayersManager()->GetCurrentLayer()->StopHistoryBlock(_T("Brush"));
	//	m_pManager->GetLayersManager()->GetCurrentLayer()->ChangeImage(ILayer::SLOW, this);		
	//}

	void GoToCommand(LONG lOffset)
	{
		BOOL bChangedLevel = FALSE;
		BOOL bChangedType  = FALSE;

		BOOL bIsRecalc = FALSE;
		
		// вернуть нужно находится ли данное изменение в пределах команды браш
		if (lOffset > 0)
		{
			LONG lOldCurLayer = m_lCurrentBlock;
			m_lCurrentBlock += lOffset;

			bIsRecalc = SetLastBrushLevel();

			for (LONG lIndex = lOldCurLayer + 1; lIndex <= m_lCurrentBlock; ++lIndex)
			{
				if (CHistoryBlockBase::htMemory == m_arHistory[lIndex]->m_eType)
				{
					CHistoryBlockMemory* pBlock = static_cast<CHistoryBlockMemory*>(m_arHistory[lIndex]);
					if (NULL != pBlock)
					{
						if (bIsRecalc)
						{
							pBlock->Redo(m_pChanges, m_lWidth, m_lHeight);
						}
						else
						{
							ApplyToImageRedo(pBlock);
						}					
					}
				}
			}
		}
		else if (lOffset < 0)
		{
			LONG lOldCurLayer = m_lCurrentBlock;
			m_lCurrentBlock += lOffset;

			bIsRecalc = SetLastBrushLevel();

			for (LONG lIndex = lOldCurLayer; lIndex > m_lCurrentBlock; --lIndex)
			{
				if (CHistoryBlockBase::htMemory == m_arHistory[lIndex]->m_eType)
				{
					CHistoryBlockMemory* pBlock = static_cast<CHistoryBlockMemory*>(m_arHistory[lIndex]);
					if (NULL != pBlock)
					{
						pBlock->Undo(m_pChanges, m_lWidth, m_lHeight);
						if (!bIsRecalc)
						{
							//ApplyToImageUndo(pBlock);
							RecalcSlowCopy();
							CBoundManager oManager;
							for (LONG i = 0; i <= m_lCurrentBlock; ++i)
							{
								if (CHistoryBlockBase::htMemory == m_arHistory[i]->m_eType)
								{
									CHistoryBlockMemory* pMem = static_cast<CHistoryBlockMemory*>(m_arHistory[i]);
									if (NULL != pMem)
									{
										oManager.AddRECT(pMem->m_oBounds);
									}
								}
							}
							if (!oManager.IsEmpty())
							{
								ApplyChangedTypeOnScan0(oManager.m_oBounds);
							}
						}
					}
				}
			}
		}

		if (bIsRecalc)
		{
			//LevelEffectToLevel(); - это уже сделано вроде
			ChangeType();
		}
		
		UpdateThumbnail(TRUE);
		m_pManager->GetView()->InvalidateView(true);
	}

	void ApplyToImageRedo(CHistoryBlockMemory* pBlock)
	{
		long left	= pBlock->m_oBounds.left;
		long top	= pBlock->m_oBounds.top;
		long right	= pBlock->m_oBounds.right;
		long bottom = pBlock->m_oBounds.bottom;
		
		BYTE* pNewWeight = pBlock->m_pDataChanges;
		BYTE* pChanges = m_pChanges;
		BYTE* pDst = m_pImageSLOW_COPY;
		BYTE* pSrc1 = m_pImageFirstSLOW_COPY;
		BYTE* pSrc2 = m_pImageBrushSLOW_COPY;

		long lIndex = m_lWidth * top + left;
		long lStride = m_lWidth - (right - left);
		
		for (long j = top; j < bottom; ++j, lIndex += lStride )
		{
			for (long i = left; i < right; ++i, ++lIndex)
			{
				pChanges[lIndex] += *pNewWeight++;

				unsigned sa = pChanges[lIndex] * 0x8081u;
				unsigned da = 255 * 0x8081u - sa;

				for( int c = 0; c < 3; c++ )
				{
					pDst[lIndex * 4 + c] = (BYTE)((pSrc1[lIndex * 4 + c] * da + pSrc2[lIndex * 4 + c] * sa + 127 * 0x8081u) >> 23);
				}
			}
		}
	}
	//void ApplyToImageUndo(CBrush::CHistoryBlockMemory* pBlock)
	//{
	//	long left	= pBlock->m_oBounds.left;
	//	long top	= pBlock->m_oBounds.top;
	//	long right	= pBlock->m_oBounds.right;
	//	long bottom = pBlock->m_oBounds.bottom;

	//	CHAR* pNewWeight	= pBlock->m_pDataChanges;
	//	LONG lIndexChanged = m_lWidth * top + left;

	//	LONG lStrideIndex = m_lWidth - right + left;
	//	LONG lStride = 4 * lStrideIndex;

	//	BYTE* pDst = m_pImageSLOW_COPY		+ 4 * m_lWidth * top + 4 * left;
	//	BYTE* pSrc = m_pImageBrushSLOW_COPY + 4 * m_lWidth * top + 4 * left;
	//	BYTE* pSLOW_COPY_WB = m_pImageFirstSLOW_COPY + 4 * m_lWidth * top + 4 * left;

	//	for (long j = top; j < bottom; ++j)
	//	{
	//		for (long i = left; i < right; ++i, pSrc += 4, pDst += 4, pSLOW_COPY_WB += 4, ++pNewWeight, ++lIndexChanged)
	//		{
	//			//LONG lOld = m_pChanges[lIndexChanged];
	//			m_pChanges[lIndexChanged] -= *pNewWeight;

	//			UpdateErase(pDst, pSLOW_COPY_WB, pSrc, m_pChanges[lIndexChanged]);

	//			//double dChange = ((double)(m_pChanges[lIndexChanged] - lOld) / (128 - lOld)); // он отрицательный!!!
	//			//if (0 < dChange)
	//			//{
	//			//	dChange = 0;
	//			//}
	//			//
	//			//Interpolate3(pDst, pDst, pSrc, dChange);
	//		}

	//		pDst += lStride;
	//		pSrc += lStride;
	//		pSLOW_COPY_WB += lStride;

	//		lIndexChanged += lStrideIndex;
	//	}
	//}

	// функции для конструктора / деструктора команды браш
	void SetCommand(CCommand* pCommand)
	{
		m_pCurrentCommand = pCommand;
	}

	void Init(IAppManager* pManager, BOOL bIsNULLCommand = TRUE)
	{
		m_pManager = pManager;
		
		Destroy(bIsNULLCommand);
		
		ClearLevels();
		LevelEffectToLevel();
		
		m_oBounds.Clear();
		m_oHistoryBounds.Clear();

		m_pChanges			= NULL;
		m_pChangedCurrent	= NULL;
		m_pChangesFAST		= NULL;

		m_pImageSLOW		= const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, m_lWidth, m_lHeight));
		m_pImageSLOW_COPY	= const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW_COPY, m_lWidth, m_lHeight));

		m_pImageFAST		= const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, m_lWidthFAST, m_lHeightFAST));
		
		LONG lBufferSize = 4 * m_lWidth * m_lHeight;

		m_pImageBrushSLOW		= new BYTE[lBufferSize];
		m_pImageBrushSLOW_COPY	= new BYTE[lBufferSize];
		m_pImageFirstSLOW_COPY  = new BYTE[lBufferSize];

		memcpy(m_pImageFirstSLOW_COPY, m_pImageSLOW_COPY, lBufferSize);

		ApplyBrushToSourceImage();
		RecalcSlowCopyWithBrush();
		
		m_pChanges			= new BYTE[m_lWidth * m_lHeight];
		m_pChangedCurrent	= new BYTE[m_lWidth * m_lHeight];
		memset(m_pChangedCurrent, 0, m_lWidth * m_lHeight);
		memset(m_pChanges, 0, m_lWidth * m_lHeight);

		m_pChangesFAST		= new BYTE[m_lWidthFAST * m_lHeightFAST];

		m_bWait					= false;
		m_bIsFoneRecalc			= true;

		m_bIsThumbnailUpdateFlag = false;

		m_pImageBrushFAST = NULL;

		m_lLastBlock = -1;
		m_lCurrentSession = -1; // ??
	}

	void Init2()
	{		
		Destroy(FALSE);
		LevelEffectToLevel();
		
		m_oBounds.Clear();
		m_oHistoryBounds.Clear();

		m_pChanges			= NULL;
		m_pChangedCurrent	= NULL;
		m_pChangesFAST		= NULL;

		m_pImageSLOW		= const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, m_lWidth, m_lHeight));
		m_pImageSLOW_COPY	= const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW_COPY, m_lWidth, m_lHeight));

		m_pImageFAST		= const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::FAST, m_lWidthFAST, m_lHeightFAST));
		
		LONG lBufferSize = 4 * m_lWidth * m_lHeight;

		m_pImageBrushSLOW		= new BYTE[lBufferSize];
		m_pImageBrushSLOW_COPY	= new BYTE[lBufferSize];
		m_pImageFirstSLOW_COPY= new BYTE[lBufferSize];

		memcpy(m_pImageFirstSLOW_COPY, m_pImageSLOW_COPY, lBufferSize);

		ApplyBrushToSourceImage();
		RecalcSlowCopyWithBrush();
		
		m_pChanges			= new BYTE[m_lWidth * m_lHeight];
		m_pChangedCurrent	= new BYTE[m_lWidth * m_lHeight];
		memset(m_pChangedCurrent, 0, m_lWidth * m_lHeight);
		memset(m_pChanges, 0, m_lWidth * m_lHeight);

		m_pChangesFAST		= new BYTE[m_lWidthFAST * m_lHeightFAST];

		m_bWait					= false;
		m_bIsFoneRecalc			= true;

		m_bIsThumbnailUpdateFlag = false;

		m_pImageBrushFAST = NULL;
	}

	void Destroy(BOOL bIsCommandDestroy = TRUE)
	{
		// удаляем все, кроме истории
		RELEASEARRAYOBJECTS(m_pChanges);
		RELEASEARRAYOBJECTS(m_pChangedCurrent);
		RELEASEARRAYOBJECTS(m_pChangesFAST);

		RELEASEARRAYOBJECTS(m_pImageBrushSLOW);
		RELEASEARRAYOBJECTS(m_pImageBrushSLOW_COPY);
		RELEASEARRAYOBJECTS(m_pImageFirstSLOW_COPY);

		RELEASEARRAYOBJECTS(m_pImageBrushFAST);

		m_pImageSLOW		= NULL;
		m_pImageBrushSLOW	= NULL;

		m_oHistoryBounds.Clear();
		m_oBounds.Clear();

		if (bIsCommandDestroy)
		{
			m_pCurrentCommand = NULL;
		}
	}

	BOOL IsBad()
	{
		LONG lWidth		= 0;
		LONG lHeight	= 0;

		BYTE* pSLOW			= const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, lWidth, lHeight));
		BYTE* pSLOW_COPY	= const_cast<BYTE*>(m_pManager->GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW_COPY, lWidth, lHeight));

		if ((pSLOW != m_pImageSLOW)				||
			(pSLOW_COPY != m_pImageSLOW_COPY)	||
			(NULL == m_pImageBrushSLOW_COPY)	||
			(NULL == m_pImageBrushSLOW)			||
			(NULL == m_pChanges)				||
			(NULL == m_pChangedCurrent))
		{
			return TRUE;
		}

		return FALSE;
	}

	void EndCommand()
	{
		m_bIsEnabled = true;
		m_bIsRedrawDataMouseMove = false;
		if (!m_oBounds.IsEmpty() && (-1 != m_lLastBlock))
		{
			CBrushSession* pSession = new CBrushSession();
			
			pSession->m_lFirstBlock = CheckLastBlock();
			pSession->m_lLastBlock	= m_lLastBlock;

			pSession->Create(m_pImageSLOW, m_oBounds.m_oBounds, m_lWidth);
			m_arSessions.Add(pSession);
			
			m_lLastBlock = -1;
		}
	}

	void CheckCurrentSession(BOOL bIsLast = FALSE)
	{
		if (!bIsLast)
		{
			// здесь по m_lCurrentBlock - определим в какой сессии мы находимся
			// просто должно выполняться неравенство    session->firstblock <= m_lCurrentBlock < sessionNext->firstblock 
			// (sessionNext->firstblock = session->lastblock)
			for (LONG nIndex = 0; nIndex < (LONG)m_arSessions.GetCount(); ++nIndex)
			{
				if ((m_lCurrentBlock >= m_arSessions[nIndex]->m_lFirstBlock) &&
					(m_lCurrentBlock < m_arSessions[nIndex]->m_lLastBlock))
				{
					m_lCurrentSession = nIndex;
					return;
				}
			}

			// это значит - что мы находимся в самом последнем браше
			m_lCurrentSession = -1;
		}
		else
		{
			// здесь по m_lCurrentBlock - определим в какой сессии мы находимся
			// просто должно выполняться неравенство    session->firstblock <= m_lCurrentBlock <= sessionNext->firstblock 
			// (sessionNext->firstblock = session->lastblock)
			for (LONG nIndex = 0; nIndex < (LONG)m_arSessions.GetCount(); ++nIndex)
			{
				if ((m_lCurrentBlock >= m_arSessions[nIndex]->m_lFirstBlock) &&
					(m_lCurrentBlock <= m_arSessions[nIndex]->m_lLastBlock))
				{
					m_lCurrentSession = nIndex;
					return;
				}
			}

			// это значит - что мы находимся в самом последнем браше
			m_lCurrentSession = -1;
		}
	}

	LONG CheckLastBlock()
	{
		if (0 >= m_arSessions.GetCount())
			return -1;

		size_t nIndex = m_arSessions.GetCount() - 1;
		return m_arSessions[nIndex]->m_lLastBlock;
	}

	LONG CheckFirstBlock()
	{
		if (-1 == m_lCurrentSession)
		{		
			if (0 >= m_arSessions.GetCount())
				return -1;

			size_t nIndex = m_arSessions.GetCount() - 1;
			return m_arSessions[nIndex]->m_lLastBlock;
		}

		if (m_lCurrentSession >= (LONG)m_arSessions.GetCount())
			return -1;

		return m_arSessions[m_lCurrentSession]->m_lFirstBlock;
	}

	void RemoveRedoSessions()
	{
		// сначала удалим все сессии, у которых m_lFirstBlock >= m_lCurrentBlock
		LONG lCount = (LONG)m_arSessions.GetCount();
		LONG lIndex = lCount - 1;

		for (; lIndex >= 0; --lIndex)
		{
			if (m_arSessions[lIndex]->m_lFirstBlock >= m_lCurrentBlock)
			{
				CBrushSession* pSession = m_arSessions[lIndex];
				m_arSessions.RemoveAt(lIndex);

				RELEASEOBJECT(pSession);
			}
			else
			{
				break;
			}
		}

		if (lIndex < 0)
			return;

		if (NULL != m_pCurrentCommand)
		{
			if (m_arSessions[lIndex]->m_lLastBlock > m_lCurrentBlock)
			{
				// значит произошло применение в текущей сессии.
				// значит она должна быть последней!
				// > - потому что m_lCurrentBlock еще не прибавился
				
				CBrushSession* pSession = m_arSessions[lIndex];
				m_arSessions.RemoveAt(lIndex);

				RELEASEOBJECT(pSession);

				m_lCurrentSession = -1;
			}
			return;
		}

		// теперь поправим если нужно последнюю
		m_arSessions[lIndex]->m_lLastBlock = min(m_arSessions[lIndex]->m_lLastBlock, m_lCurrentBlock);
		if (m_arSessions[lIndex]->m_lFirstBlock >= m_arSessions[lIndex]->m_lLastBlock)
		{
			CBrushSession* pSession = m_arSessions[lIndex];
			m_arSessions.RemoveAt(lIndex);

			RELEASEOBJECT(pSession);
		}
	}

	void RemoveUndoSessions()
	{
		for (size_t nIndex = 0; nIndex < m_arSessions.GetCount(); ++nIndex)
		{
			CBrushSession* pSession = m_arSessions[nIndex];
			RELEASEOBJECT(pSession);
		}
		m_arSessions.RemoveAll();
	}


public:

	void FireEvent(size_t nIndexParam, int nParam)
	{
		if (NULL != m_pCurrentCommand)
		{
			m_pCurrentCommand->FireChangeParameter(nIndexParam, nParam);
		}
	}

	void FireEvent(size_t nIndexParam, float fParam)
	{
		if (NULL != m_pCurrentCommand)
		{
			m_pCurrentCommand->FireChangeParameter(nIndexParam, fParam);
		}
	}

	void CheckEmpty()
	{
		m_bIsEmptyParameters = true;
		
		if ((0 != m_nLevelBrightness) ||
			(0 != m_nLevelContrast) ||
			(0 != m_nLevelBlur) ||
			(0 != m_nLevelSharpen) ||
			(0 != m_nLevelSaturation) ||
			(0 != m_nLevelEffectPosterize) ||
			(0 != m_nLevelEffectGlow) ||
			(0 != m_nLevelGamma))
		{
			m_bIsEmptyParameters = false;
		}
	}

	void LevelEffectToLevel()
	{
		// в приложении - от -100..100
		// тут в зависимости от эффекта пересчитаем
		
		// brightness: // -100..100
		m_nLevelBrightness	= m_nLevelEffectBrightness;

		// contrast:   // -100..100
		m_nLevelContrast	= m_nLevelEffectContrast;

		// blur:	   // 0..50 <- gaussian (из-за тормозов - 0..10)
		m_nLevelBlur		= max(0, m_nLevelEffectBlur / 10);

		// sharpen:	   // 0..100
		m_nLevelSharpen		= max(0, m_nLevelEffectSharpen);

		// saturation: // -100..100
		m_nLevelSaturation	= m_nLevelEffectSaturation;

		// posterize: // 2..32 (наоборот) (у нас левел - интенсивность применения эффекта !!!)
		m_nLevelPosterize	= max(0, m_nLevelEffectPosterize);
		m_nLevelPosterize	= (int)(-0.3 * m_nLevelPosterize) + 32;

		// glow:	  // 1..100	
		m_nLevelGlow	= max(0, m_nLevelEffectGlow);
		m_nLevelGlow	= (int)(0.99 * m_nLevelGlow) + 1;

		// gamma:	  // -99..100
		m_nLevelGamma	= max(m_nLevelEffectGamma, -99);

		CheckEmpty();
	}

	void UpdateBounds(CBoundManager& oManager, LONG lFirst, LONG lLast, BOOL bClear = TRUE)
	{
		if (bClear)
		{
			oManager.Clear();
		}

		for (LONG lIndex = lFirst + 1; lIndex <= lLast; ++lIndex)
		{
			if (CHistoryBlockBase::htMemory == m_arHistory[lIndex]->m_eType)
			{
				CHistoryBlockMemory* pMem = static_cast<CHistoryBlockMemory*>(m_arHistory[lIndex]);
				if (NULL != pMem)
				{
					oManager.AddRECT(pMem->m_oBounds);
				}
			}
		}
	}

	void Enable(bool bEnable)
	{
		m_bIsEnabled = bEnable;		
		if (NULL != m_pCurrentCommand)
		{
			if (bEnable)
			{
				m_pCurrentCommand->SwitchViewToSlow();
			}
			else
			{
				m_pCurrentCommand->SwitchViewToFast();
			}
		}
	}

	void SetEraseMode(bool bValue)
	{
		/*if (bValue == m_bEraseMode)
			return;*/

		m_bEraseMode = bValue;		
	}

private:
	//void FillLine(BYTE* pMask, int nSize, LONG x1, LONG y1, LONG x2, LONG y2, const LONG& lWidth, const LONG& lHeight)
	//{
	//	// тут заполняем по алгоритму брезенхэма
	//	LONG dx = (x2 - x1 >= 0 ? 1 : -1);
	//	LONG dy = (y2 - y1 >= 0 ? 1 : -1);

	//	int lengthX = abs(x2 - x1);
	//	int lengthY = abs(y2 - y1);

	//	int length = max(lengthX, lengthY);

	//	if (length == 0)
	//	{
	//		return;
	//	}

	//	if (lengthY <= lengthX)
	//	{
	//		// Начальные значения
	//		LONG x = x1;
	//		LONG y = y1;
	//		LONG d = -lengthX;

	//		// Основной цикл
	//		length++;
	//		while (length--)
	//		{
	//			  //SetPixel(hdc, x, y, 0);
	//			  x += dx;
	//			  d += 2 * lengthY;
	//			  if (d > 0) 
	//			  {
	//					d -= 2 * lengthX;
	//					y += dy;
	//			  }
	//		}
	//	}
	//	else
	//	{
	//		// Начальные значения
	//		int x = x1;
	//		int y = y1;
	//		int d = - lengthY;

	//		// Основной цикл
	//		length++;
	//		while(length--)
	//		{
	//			  //SetPixel(hdc, x, y, 0);
	//			  y += dy;
	//			  d += 2 * lengthX;
	//			  if (d > 0) 
	//			  {
	//					d -= 2 * lengthY;
	//					x += dx;
	//			  }
	//		}
	//	}
	//}
};

//BOOL CBrush::m_bIsMaskInit = FALSE;
//BYTE CBrush::m_pMaskInterpolation[256 * 256 * 128];
//BYTE CBrush::m_pMaskChanged[128 * 128];

class CBrushCommand : public CCommand
{
public:
	enum
	{
		BRUSH_SIZE_MIN = 5,
		BRUSH_SIZE_MAX = 300,
		BRUSH_SIZE_DEF = 30,
		
		BRUSH_SOFT_MIN = 0,
		BRUSH_SOFT_MAX = 100,
		BRUSH_SOFT_DEF = 50,

		BRUSH_EFFECT_BRIGHTNESS_MIN = -50,
		BRUSH_EFFECT_BRIGHTNESS_MAX = 50,
		BRUSH_EFFECT_BRIGHTNESS_DEF = 0,

		BRUSH_EFFECT_CONTRAST_MIN = -50,
		BRUSH_EFFECT_CONTRAST_MAX = 50,
		BRUSH_EFFECT_CONTRAST_DEF = 0,

		BRUSH_EFFECT_BLUR_MIN = 0,
		BRUSH_EFFECT_BLUR_MAX = 100,
		BRUSH_EFFECT_BLUR_DEF = 0,

		BRUSH_EFFECT_SHARPEN_MIN = 0,
		BRUSH_EFFECT_SHARPEN_MAX = 100,
		BRUSH_EFFECT_SHARPEN_DEF = 0,

		BRUSH_EFFECT_SATURATION_MIN = -100,
		BRUSH_EFFECT_SATURATION_MAX = 100,
		BRUSH_EFFECT_SATURATION_DEF = 0,

		BRUSH_EFFECT_POSTERIZE_MIN = 2,
		BRUSH_EFFECT_POSTERIZE_MAX = 250,
		BRUSH_EFFECT_POSTERIZE_DEF = 250,

		BRUSH_EFFECT_GLOW_MIN = -100,
		BRUSH_EFFECT_GLOW_MAX = 100,
		BRUSH_EFFECT_GLOW_DEF = 0,

		BRUSH_EFFECT_GAMMA_MIN = -100,
		BRUSH_EFFECT_GAMMA_MAX = 100,
		BRUSH_EFFECT_GAMMA_DEF = 0,

		BRUSH_ERASE_MODE_OFF = 0,
		BRUSH_ERASE_MODE_ON = 1,
		BRUSH_ERASE_MODE_DEF = BRUSH_ERASE_MODE_OFF,
	};

public:
	CBrushCommand(IAppManager* app_manager) : CCommand(app_manager),
		m_oBrushMask( BRUSH_SIZE_DEF, BRUSH_SOFT_DEF )
	{
		m_bIsApply = false;

		m_oBrush.Init(app_manager);
		m_oBrush.SetCommand(this);
		
		AddParameter(_T("Size"), _T("Brush Size"), BRUSH_SIZE_DEF, BRUSH_SIZE_MIN, BRUSH_SIZE_MAX);
		AddParameter(_T("Soft"), _T("Soft"), BRUSH_SOFT_DEF, BRUSH_SOFT_MIN, BRUSH_SOFT_MAX);

		AddParameter(_T("Brightness Level")	, _T("Level Value"), m_oBrush.m_nLevelEffectBrightness  , BRUSH_EFFECT_BRIGHTNESS_MIN, BRUSH_EFFECT_BRIGHTNESS_MAX);
		AddParameter(_T("Contrast Level")	, _T("Level Value"), m_oBrush.m_nLevelEffectContrast    , BRUSH_EFFECT_CONTRAST_MIN,   BRUSH_EFFECT_CONTRAST_MAX);
		AddParameter(_T("Blur Level")		, _T("Level Value"), m_oBrush.m_nLevelEffectBlur        , BRUSH_EFFECT_BLUR_MIN,       BRUSH_EFFECT_BLUR_MAX);
		AddParameter(_T("Sharpen Level")	, _T("Level Value"), m_oBrush.m_nLevelEffectSharpen     , BRUSH_EFFECT_SHARPEN_MIN,    BRUSH_EFFECT_SHARPEN_MAX);
		AddParameter(_T("Saturation Level")	, _T("Level Value"), m_oBrush.m_nLevelEffectSaturation  , BRUSH_EFFECT_SATURATION_MIN, BRUSH_EFFECT_SATURATION_MAX);
		AddParameter(_T("Posterize Level")	, _T("Level Value"), m_oBrush.m_nLevelEffectPosterize   , BRUSH_EFFECT_POSTERIZE_MIN,  BRUSH_EFFECT_POSTERIZE_MAX);
		AddParameter(_T("Glow Level")		, _T("Level Value"), m_oBrush.m_nLevelEffectGlow        , BRUSH_EFFECT_GLOW_MIN,       BRUSH_EFFECT_GLOW_MAX);
		AddParameter(_T("Gamma Level")		, _T("Level Value"), m_oBrush.m_nLevelEffectGamma       , BRUSH_EFFECT_GAMMA_MIN,      BRUSH_EFFECT_GAMMA_MAX);
		AddParameter(_T("EraseMode")		, _T("EraseMode"),	 m_oBrush.m_bEraseMode              , BRUSH_ERASE_MODE_OFF,        BRUSH_ERASE_MODE_ON); // было m_nLevelEffectGamma ???


		m_bIsMouseMove = FALSE;

		m_lMouseOldX = 0;
		m_lMouseOldY = 0;

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->AddSubscriber(&m_oBrush);

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->SetActualImage(ILayer::SLOW_COPY);
		GetAppManager()->GetView()->EnableDraw(true);
	}
	virtual ~CBrushCommand() 
	{
		// сначала закончим текущую сессию
		m_oBrush.EndCommand();

		// нужно применить к SLOW - иначе следующая команда начнет работу не с той картинкой
		m_oBrush.ApplyChangedTypeOnSLOW();
		m_oBrush.CorrectFAST();	
		m_oBrush.Destroy();

		GetAppManager()->GetLayersManager()->GetCurrentLayer()->RemoveSubscriber(&m_oBrush);
	}
public:

	virtual void OnMouseDown(LONG nX, LONG nY, LONG nButton)	
	{
		if (!m_oBrush.m_bIsEnabled)
			return;
		
		if (m_oBrush.m_bWait)
		{
			return;
		}
		
		m_bIsApply = TRUE;
		m_oCursor.x = nX;
		m_oCursor.y = nY;

		GetAppManager()->GetCoordinateTransformer()->ScreenPointToImagePoint(nX, nY);
		
		m_lMouseOldX = nX;
		m_lMouseOldY = nY;
		
		//ApplyMask(nX, nY);
		m_oBrush.ApplyMask( GetBrushMask(), GetBrushSize(), nX, nY, m_bIsMouseMove, m_lMouseOldX, m_lMouseOldY);

		m_bIsMouseMove = TRUE;
		SetInvalidateFlag(INVALID_GRAPHICS);
	}
	virtual void OnMouseUp(LONG nX, LONG nY, LONG nButton)	
	{
		if (!m_oBrush.m_bIsEnabled)
			return;
		
		m_bIsMouseMove = FALSE;
		if (!m_bIsApply)
		{
			return;
		}
		
		m_bIsApply = FALSE;
		m_oBrush.DumpToHistoryBlock();

		// а вот это наверное не надо ???
		SetInvalidateFlag(INVALID_GRAPHICS);
	}
	virtual void OnMouseMove(LONG nX, LONG nY, LONG nButton)	
	{
		if (!m_oBrush.m_bIsEnabled)
			return;

		if (m_bIsApply && (0 == (nButton & MK_LBUTTON)))
		{
			m_bIsMouseMove = FALSE;
			m_bIsApply = FALSE;

			SetInvalidateFlag(INVALID_GRAPHICS, m_bIsApply);
		}
		
		m_oCursor.x = nX;
		m_oCursor.y = nY;

		if (m_bIsApply)
		{
			GetAppManager()->GetCoordinateTransformer()->ScreenPointToImagePoint(nX, nY);

			BYTE* pBrushMask = GetBrushMask();
			if( pBrushMask )
			{
				int nBrushSize = GetBrushSize();

				if (FALSE == BRUSH_MOTION_PATH)
				{
					//LONG lOffsetX = (nX - m_lMouseOldX);
					//LONG lOffsetY = (nY - m_lMouseOldY);
					//
					//double dDistance = _hypot(lOffsetX, lOffsetY);
					//LONG lCount = (LONG)(dDistance / m_nSize);

					//lOffsetX = (LONG)(lOffsetX * m_nSize / dDistance);
					//lOffsetY = (LONG)(lOffsetY * m_nSize / dDistance);

					//for (LONG lIndex = 1; lIndex <= lCount; ++lIndex)
					//{
					//	// вот тут след
					//	LONG lX = m_lMouseOldX + lIndex * lOffsetX;
					//	LONG lY = m_lMouseOldY + lIndex * lOffsetY;
					//	
					//	//ApplyMask(lX, lY);
					//	m_oBrush.ApplyMask(m_pMask, m_nSize, lX, lY, m_bIsMouseMove, m_lMouseOldX, m_lMouseOldY);
					//}

					long dX = nX - m_lMouseOldX;
					long dY = nY - m_lMouseOldY;
					
					if( dX || dY )
					{
						double dDistance = sqrt( (double)dX * dX + (double)dY * dY );

						double DELTA = (double)nBrushSize / 3.0;

						double dDeltaX = (dX / dDistance) * DELTA;
						double dDeltaY = (dY / dDistance) * DELTA;

						double x = m_lMouseOldX;
						double y = m_lMouseOldY;

						LONG lCount = (LONG)(dDistance / DELTA);
						for (long i = 0; i < lCount; ++i)
						{
							x += dDeltaX;
							y += dDeltaY;

							m_oBrush.ApplyMask( pBrushMask, nBrushSize, (LONG)x, (LONG)y, FALSE, 0, 0 );
						}

						m_oBrush.ApplyMask( pBrushMask, nBrushSize, nX, nY, FALSE, 0, 0);
					}
				}
			}
			
			//ApplyMask(nX, nY);
			//m_oBrush.ApplyMask(m_pMask, m_nSize, nX, nY, m_bIsMouseMove, m_lMouseOldX, m_lMouseOldY);

			m_lMouseOldX = nX;
			m_lMouseOldY = nY;
		}

		SetInvalidateFlag(INVALID_GRAPHICS, m_bIsApply | m_oBrush.m_bIsRedrawDataMouseMove);
	}

	
	virtual void OnChangeParameter(int paramInd) 
	{
		if (!m_oBrush.m_bIsEnabled)
			return;
		
		if (paramInd == c_nParamIndexLevel_BrushSize && m_params.GetCount() > c_nParamIndexLevel_BrushSize)
		{
			long lNewSize = m_params[c_nParamIndexLevel_BrushSize]->m_value.lVal;
			
			if( lNewSize < BRUSH_SIZE_MIN ) lNewSize = BRUSH_SIZE_MIN;
			if( lNewSize > BRUSH_SIZE_MAX ) lNewSize = BRUSH_SIZE_MAX;
			
			if( m_oBrushMask.SetSize( lNewSize ) )
			{
				SetInvalidateFlag(INVALID_GRAPHICS);
			}
			return;
		}

		BOOL bIsChangeLevel = FALSE;
		
		if (paramInd == c_nParamIndexLevel_BrushSoft && m_params.GetCount() > c_nParamIndexLevel_BrushSoft)
		{
			long lNewSoft = m_params[c_nParamIndexLevel_BrushSoft]->m_value.lVal;

			if( lNewSoft < BRUSH_SOFT_MIN ) lNewSoft = BRUSH_SOFT_MIN;
			if( lNewSoft > BRUSH_SOFT_MAX ) lNewSoft = BRUSH_SOFT_MAX;
			
			if( m_oBrushMask.SetSoft( lNewSoft ) )
			{
			}
			return;
		}

		if (paramInd == c_nParamIndexLevel_Erase && m_params.GetCount() > c_nParamIndexLevel_Erase)
		{
			long lNewMode = m_params[c_nParamIndexLevel_Erase]->m_value.lVal;
			m_oBrush.SetEraseMode( lNewMode != BRUSH_ERASE_MODE_OFF );
			return;
		}
		
		//ANALYZE_PARAM(Brightness)
		if (c_nParamIndexLevel_Brightness == paramInd && m_params.GetCount() > c_nParamIndexLevel_Brightness)
		{
			m_oBrush.m_nLevelEffectBrightness = 2 * max(-100, min(100, m_params[c_nParamIndexLevel_Brightness]->m_value.lVal));

			LONG lOldLevel = m_oBrush.m_nLevelBrightness;
			m_oBrush.LevelEffectToLevel();

			bIsChangeLevel = (lOldLevel != m_oBrush.m_nLevelBrightness) ? TRUE : FALSE;
		}
		//ANALYZE_PARAM(Contrast)
		else if (c_nParamIndexLevel_Contrast == paramInd && m_params.GetCount() > c_nParamIndexLevel_Contrast)
		{
			m_oBrush.m_nLevelEffectContrast = 2 * max(-100, min(100, m_params[c_nParamIndexLevel_Contrast]->m_value.lVal));

			LONG lOldLevel = m_oBrush.m_nLevelContrast;
			m_oBrush.LevelEffectToLevel();

			bIsChangeLevel = (lOldLevel != m_oBrush.m_nLevelContrast) ? TRUE : FALSE;
		}
		//ANALYZE_PARAM(Blur)
		else if (c_nParamIndexLevel_Blur == paramInd && m_params.GetCount() > c_nParamIndexLevel_Blur)
		{
			m_oBrush.m_nLevelEffectBlur = 2 * max(-100, min(100, m_params[c_nParamIndexLevel_Blur]->m_value.lVal));

			LONG lOldLevel = m_oBrush.m_nLevelBlur;
			m_oBrush.LevelEffectToLevel();

			bIsChangeLevel = (lOldLevel != m_oBrush.m_nLevelBlur) ? TRUE : FALSE;
		}
		//ANALYZE_PARAM(Sharpen)
		else if (c_nParamIndexLevel_Sharpen == paramInd && m_params.GetCount() > c_nParamIndexLevel_Sharpen)
		{
			m_oBrush.m_nLevelEffectSharpen = 2 * max(-100, min(100, m_params[c_nParamIndexLevel_Sharpen]->m_value.lVal));

			LONG lOldLevel = m_oBrush.m_nLevelSharpen;
			m_oBrush.LevelEffectToLevel();

			bIsChangeLevel = (lOldLevel != m_oBrush.m_nLevelSharpen) ? TRUE : FALSE;
		}
		//ANALYZE_PARAM(Saturation)
		else if (c_nParamIndexLevel_Saturation == paramInd && m_params.GetCount() > c_nParamIndexLevel_Saturation)
		{
			m_oBrush.m_nLevelEffectSaturation = 2 * max(-100, min(100, m_params[c_nParamIndexLevel_Saturation]->m_value.lVal));

			LONG lOldLevel = m_oBrush.m_nLevelSaturation;
			m_oBrush.LevelEffectToLevel();

			bIsChangeLevel = (lOldLevel != m_oBrush.m_nLevelSaturation) ? TRUE : FALSE;
		}
		//ANALYZE_PARAM(Posterize)
		else if (c_nParamIndexLevel_Posterize == paramInd && m_params.GetCount() > c_nParamIndexLevel_Posterize)
		{
			m_oBrush.m_nLevelEffectPosterize = 2 * max(-100, min(100, m_params[c_nParamIndexLevel_Posterize]->m_value.lVal));

			LONG lOldLevel = m_oBrush.m_nLevelPosterize;
			m_oBrush.LevelEffectToLevel();

			bIsChangeLevel = (lOldLevel != m_oBrush.m_nLevelPosterize) ? TRUE : FALSE;
		}
		//ANALYZE_PARAM(Glow)
		else if (c_nParamIndexLevel_Glow == paramInd && m_params.GetCount() > c_nParamIndexLevel_Glow)
		{
			m_oBrush.m_nLevelEffectGlow = 2 * max(-100, min(100, m_params[c_nParamIndexLevel_Glow]->m_value.lVal));

			LONG lOldLevel = m_oBrush.m_nLevelGlow;
			m_oBrush.LevelEffectToLevel();

			bIsChangeLevel = (lOldLevel != m_oBrush.m_nLevelGlow) ? TRUE : FALSE;
		}
		//ANALYZE_PARAM(Gamma)
		else if (c_nParamIndexLevel_Gamma == paramInd && m_params.GetCount() > c_nParamIndexLevel_Gamma)
		{
			m_oBrush.m_nLevelEffectGamma = 2 * max(-100, min(100, m_params[c_nParamIndexLevel_Gamma]->m_value.lVal));

			LONG lOldLevel = m_oBrush.m_nLevelGamma;
			m_oBrush.LevelEffectToLevel();

			bIsChangeLevel = (lOldLevel != m_oBrush.m_nLevelGamma) ? TRUE : FALSE;
		}

		if (bIsChangeLevel)
		{
			m_oBrush.UpdateLevel();
		}
	}

	
	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{
		m_oBrush.m_bIsRedrawDataMouseMove = false;
		
		long realSz = GetBrushSize();
		GetAppManager()->GetCoordinateTransformer()->ImageDistanceToScreenDistance(realSz);

		Gdiplus::SmoothingMode  savingSM  = pGr->GetSmoothingMode();
		pGr->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
		Gdiplus::Pen pn( Gdiplus::Color::White, 1.0f);
		Gdiplus::Pen  pnBlack( Gdiplus::Color::Black, 1.0f );

		pGr->DrawEllipse(&pn, m_oCursor.x - realSz, m_oCursor.y - realSz, 2 * realSz , 2 * realSz);
		pGr->DrawEllipse(&pnBlack, m_oCursor.x - realSz+1, m_oCursor.y - realSz+1, 2 * realSz-2 , 2 * realSz-2);

		pGr->SetSmoothingMode(savingSM);
	}

	virtual bool DrawOnLayer()	
	{	
		return true;
	}

	//virtual void Undo(int stepsCount)	{ m_oBrush.OnUndo(stepsCount, false); }
	//virtual void Redo(int stepsCount)	{ m_oBrush.OnRedo(stepsCount);	}

	int GetBrushSize() const
	{
		return m_oBrushMask.GetSize();
	}

	int GetBrushSoft() const
	{
		return m_oBrushMask.GetSoft();
	}

	BYTE* GetBrushMask()
	{
		return m_oBrushMask.GetBuffer();
	}

private:
	
	CBrushMask m_oBrushMask;

	POINT m_oCursor;		// курсор мыши - в координатах окна
	
	// флаги - следящие за "быстрым движением" мышки -> формирование следа
	LONG m_lMouseOldX;
	LONG m_lMouseOldY;

	bool m_bIsMouseMove;
	bool m_bIsApply;		// true - когда в режиме применения эффекта

	// -------- для Undo-Redo ---------
	static CBrush m_oBrush;
};

CBrush CBrushCommand::m_oBrush;

#endif