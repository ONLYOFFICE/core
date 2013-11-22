#pragma once
#include "..\stdafx.h"

#include "Interfaces.h"
#include "AVSMetafile.h"

[coclass, uuid("41403DF2-5EB1-4310-AF9B-93FD1BEEB866"), threading(apartment), vi_progid("PageCommands"), progid("PageCommands.1"), version(1.0)]
class ATL_NO_VTABLE CPageCommands :	public IAVSPageCommands, public IAVSOfficeRendererTemplate2
{
public:
	class CBufferPage
	{
	public:
		enum CommandType
		{
			ctSetPen						= 0,
			ctSetBrush						= 1,
			ctSetFont						= 2,
			ctSetShadow						= 3,
			ctSetEdgeText					= 4,

			ctDrawText						= 5,
			
			ctPathCommandMoveTo				= 6,
			ctPathCommandLineTo				= 7,
			ctPathCommandLinesTo			= 8,
			ctPathCommandCurveTo			= 9,
			ctPathCommandCurvesTo			= 10,
			ctPathCommandArcTo				= 11,
			ctPathCommandClose				= 12,
			ctPathCommandEnd				= 13,
			ctDrawPath						= 14,
			ctPathCommandStart				= 15,
			ctPathCommandGetCurrentPoint	= 16,	

			ctDrawImage						= 17,
			ctDrawImageFromFile				= 18,

			ctSetParams						= 19,

			ctBeginCommand					= 20,
			ctEndCommand					= 21,

			ctSetTransform					= 22,
			ctResetTransform				= 23,
			ctClipMode						= 24,

			ctPathCommandText				= 25
		};

	private: 
		class CCommandsCache
		{
		private:
			class CLetter
			{
			public:
				CString m_strCommand;
				LONG	m_lIndex;

			public:
				CLetter()
				{
					m_strCommand	= _T("");
					m_lIndex		= -1;
				}
				CLetter& operator=(const CLetter& oSrc)
				{
					m_strCommand	= oSrc.m_strCommand;
					m_lIndex		= m_lIndex;
				}
				CLetter(const CLetter& oSrc)
				{
					*this = oSrc;
				}
			};

		private:
			CAtlArray<CLetter> m_arrPens;
			CAtlArray<CLetter> m_arrBrushes;
			CAtlArray<CLetter> m_arrFonts;
			//CAtlArray<CLetter> m_arrShadows;
			//CAtlArray<CLetter> m_arrEdges;

			size_t m_lCountCache;

		public:

			CCommandsCache() : m_arrPens(), m_arrBrushes(), m_arrFonts()
			{
				m_lCountCache = 5;
			}
			~CCommandsCache()
			{
			}

		public:

			LONG AddPen(const CString& str, CAtlArray<CString>* pArray)
			{
				return GetEqualIndex(&m_arrPens, pArray, str);
			}
			LONG AddBrush(const CString& str, CAtlArray<CString>* pArray)
			{
				return GetEqualIndex(&m_arrBrushes, pArray, str);
			}
			LONG AddFont(const CString& str, CAtlArray<CString>* pArray)
			{
				return GetEqualIndex(&m_arrFonts, pArray, str);
			}

		protected:

			LONG GetEqualIndex(CAtlArray<CLetter>* pArray, CAtlArray<CString>* pArrayGlobal, const CString& str)
			{
				size_t nCount = pArray->GetCount();
				size_t nIndex = 0;

				for (; nIndex < nCount; ++nIndex)
				{
					if (((*pArray)[nIndex]).m_strCommand == str)
					{
						return ((*pArray)[nIndex]).m_lIndex;
					}
				}

				// не нашли...

				LONG lIndexNew = (LONG)pArrayGlobal->GetCount();
				pArrayGlobal->Add(str);
				
				if (nIndex >= m_lCountCache)
				{
					pArray->RemoveAt(0);
					--nCount;
				}

				pArray->Add();
				((*pArray)[nCount]).m_strCommand = str;
				((*pArray)[nCount]).m_lIndex	 = lIndexNew;

				return lIndexNew;
			}
		};

	private:
		BYTE* m_pBuffer;
		BYTE* m_pBufferMem;
		
		size_t m_lPosition;
		size_t m_lSize;

		CAtlArray<CString> m_arStrings;
		CAtlArray<IUnknown*> m_arImages;

		BOOL			m_bIsAdvanced;			
		CBufferPage*	m_pAdvancedCommands;

	public:
		float m_fWidth;
		float m_fHeight;

	private:
		LONG m_lSizeofDouble;
		LONG m_lSizeofFloat;
		LONG m_lSizeofLONG;

		CString m_strOldPen;
		CString m_strOldBrush;
		CString m_strOldFont;
		CString m_strOldShadow;
		CString m_strOldEdge;

		CCommandsCache m_oGraphicsCache;

	public:
		CBufferPage() : m_arImages(), m_arStrings(), m_oGraphicsCache()
		{
			Clear();

			m_fWidth = 210;
			m_fHeight = 190;

			m_lSizeofDouble	= sizeof(double);
			m_lSizeofFloat 	= sizeof(float);
			m_lSizeofLONG	= sizeof(long);

			m_strOldPen		= _T("");
			m_strOldBrush	= _T("");
			m_strOldFont	= _T("");
			m_strOldShadow	= _T("");
			m_strOldEdge	= _T("");

			m_pAdvancedCommands = NULL;
			m_bIsAdvanced		= FALSE;
		}
		~CBufferPage()
		{
			RELEASEARRAYOBJECTS(m_pBuffer);

			m_arStrings.RemoveAll();

			size_t nCount = m_arImages.GetCount();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				IUnknown* punkImage = m_arImages[nIndex];
				RELEASEINTERFACE(punkImage);
			}

			m_arImages.RemoveAll();

			RELEASEOBJECT(m_pAdvancedCommands);
		}

		void ReleaseAdvanced()
		{
			RELEASEOBJECT(m_pAdvancedCommands);
			m_bIsAdvanced = FALSE;
		}

		CBufferPage(const CBufferPage& oSrc)
		{
			*this = oSrc;
		}

		CBufferPage& operator =(const CBufferPage& oSrc)
		{
			m_fWidth	= oSrc.m_fWidth;
			m_fHeight	= oSrc.m_fHeight;

			m_pBuffer = oSrc.m_pBuffer;

			m_arImages.Copy(oSrc.m_arImages);
			m_arStrings.Copy(oSrc.m_arStrings);

			m_lPosition	= oSrc.m_lPosition;
			m_lSize		= oSrc.m_lSize;

			m_pAdvancedCommands = oSrc.m_pAdvancedCommands;
			m_bIsAdvanced		= oSrc.m_bIsAdvanced;
			
			return *this;
		}

		void Clear()
		{
			m_lSize = 1000;
			m_lPosition = 0;

			m_pBuffer = new BYTE[m_lSize];

			m_arImages.RemoveAll();
			m_arStrings.RemoveAll();
		}

		inline LONG ReadLONG(const size_t& pos)
		{
			return *((LONG*)(m_pBuffer + pos));
		}
		inline float ReadFloat(const size_t& pos)
		{
			return *((float*)(m_pBuffer + pos));
		}
		inline BYTE ReadByte(const size_t& pos)
		{
			return *(m_pBuffer + pos);
		}
		inline double ReadDouble(const size_t& pos)
		{
			return *((double*)(m_pBuffer + pos));
		}

		inline LONG ReadLONG()
		{
			LONG l = *((LONG*)m_pBufferMem);
			m_pBufferMem += m_lSizeofLONG;
			return l;
		}
		inline float ReadFloat()
		{
			float l = *((float*)m_pBufferMem);
			m_pBufferMem += m_lSizeofFloat;
			return l;
		}
		inline BYTE ReadByte()
		{
			BYTE l = *m_pBufferMem;
			++m_pBufferMem;
			return l;
		}
		inline double ReadDouble()
		{
			double l = *((double*)m_pBufferMem);
			m_pBufferMem += m_lSizeofDouble;
			return l;
		}

		void CheckBufferSize(size_t lPlus)
		{
			size_t nNewSize = m_lPosition + lPlus;

			if (nNewSize >= m_lSize)
			{
				while (nNewSize >= m_lSize)
				{
					m_lSize *= 2;
				}
				
				BYTE* pNew = new BYTE[m_lSize];
				memcpy(pNew, m_pBuffer, m_lPosition);

				RELEASEARRAYOBJECTS(m_pBuffer);
				m_pBuffer = pNew;
			}
		}

		void Write(LONG lCommand)
		{
			size_t lMem = sizeof(LONG);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= lCommand; m_lPosition += sizeof(LONG);
		}

		void Write(LONG lCommand, LONG lType)
		{
			size_t lMem = 2 * sizeof(LONG);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= lCommand; m_lPosition += sizeof(LONG);
			*((LONG*)(m_pBuffer + m_lPosition))	= lType;	m_lPosition += sizeof(LONG);
		}

		void Write(LONG lCommand, float f1, float f2)
		{
			size_t lMem = sizeof(LONG) + 2 * sizeof(float);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= lCommand; m_lPosition += sizeof(LONG);
			*((float*)(m_pBuffer + m_lPosition))= f1;		m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= f2;		m_lPosition += sizeof(float);
		}

		void Write(LONG lCommand, float f1, float f2, float f3, float f4, float f5, float f6)
		{
			size_t lMem = sizeof(LONG) + 6 * sizeof(float);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= lCommand; m_lPosition += sizeof(LONG);
			*((float*)(m_pBuffer + m_lPosition))= f1;		m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= f2;		m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= f3;		m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= f4;		m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= f5;		m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= f6;		m_lPosition += sizeof(float);
		}

		void Write(LONG lCommand, LONG lCount, float* pData)
		{
			size_t lFloats = lCount * sizeof(float);
			size_t lMem = 2 * sizeof(LONG) + lFloats;

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= lCommand; m_lPosition += sizeof(LONG);
			*((LONG*)(m_pBuffer + m_lPosition))	= lCount;	m_lPosition += sizeof(LONG);

			memcpy(m_pBuffer + m_lPosition, pData, lFloats);
			m_lPosition += lFloats;
		}

		void Write(LONG lCommand, const CString& sString)
		{
			LONG lIndex = -1;
			
			switch (lCommand)
			{
			case ctSetPen:
				{
					if (m_strOldPen == sString)
						return;

					m_strOldPen = sString;

					lIndex = m_oGraphicsCache.AddPen(sString, &m_arStrings);
					break;
				}
			case ctSetBrush:
				{
					if (m_strOldBrush == sString)
						return;

					m_strOldBrush = sString;

					lIndex = m_oGraphicsCache.AddBrush(sString, &m_arStrings);
					break;
				}
			case ctSetFont:
				{
					if (m_strOldFont == sString)
						return;

					m_strOldFont = sString;

					lIndex = m_oGraphicsCache.AddFont(sString, &m_arStrings);
					break;
				}
			case ctSetShadow:
				{
					if (m_strOldShadow == sString)
						return;

					m_strOldShadow = sString;
					break;
				}
			case ctSetEdgeText:
				{
					if (m_strOldEdge == sString)
						return;

					m_strOldEdge = sString;
					break;
				}			
			};
			
			size_t lMem = 2 * sizeof(LONG);

			CheckBufferSize(lMem);

			if (-1 == lIndex)
			{
				*((LONG*)(m_pBuffer + m_lPosition))	= lCommand;						m_lPosition += sizeof(LONG);
				*((LONG*)(m_pBuffer + m_lPosition))	= (LONG)m_arStrings.GetCount();	m_lPosition += sizeof(LONG);

				m_arStrings.Add(sString);
			}
			else
			{
				*((LONG*)(m_pBuffer + m_lPosition))	= lCommand;						m_lPosition += sizeof(LONG);
				*((LONG*)(m_pBuffer + m_lPosition))	= lIndex;						m_lPosition += sizeof(LONG);
			}			
		}

		
		inline void WritePath(const LONG& lType)
		{
			if (m_bIsAdvanced)
			{
				return m_pAdvancedCommands->WritePath(lType);
			}
			
			size_t lMem = 2 * sizeof(LONG);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= ctDrawPath;	m_lPosition += sizeof(LONG);
			*((LONG*)(m_pBuffer + m_lPosition))	= lType;		m_lPosition += sizeof(LONG);
		}

		inline void WriteString(const CString& strText, const float& fX, const float& fY, const float& fWidth, const float& fHeight, const float& fBaseOffset, BOOL bIsPath = FALSE)
		{
			if (m_bIsAdvanced)
			{
				return m_pAdvancedCommands->WriteString(strText, fX, fY, fWidth, fHeight, fBaseOffset, bIsPath);
			}
			
			size_t lMem = 2 * sizeof(LONG) + 5 * sizeof(float);

			CheckBufferSize(lMem);

			LONG lCommandType = (FALSE == bIsPath) ? ctDrawText : ctPathCommandText; 

			*((LONG*)(m_pBuffer + m_lPosition))	= lCommandType;						m_lPosition += sizeof(LONG);
			
			*((LONG*)(m_pBuffer + m_lPosition))	= (LONG)m_arStrings.GetCount();		m_lPosition += sizeof(LONG);

			*((float*)(m_pBuffer + m_lPosition))= fX;								m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= fY;								m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= fWidth;							m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= fHeight;							m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= fBaseOffset;						m_lPosition += sizeof(float);

			m_arStrings.Add(strText);
		}

		inline void WriteImage(IUnknown* punkImage, const float& fX, const float& fY, const float& fWidth, const float& fHeight)
		{
			if (m_bIsAdvanced)
			{
				return m_pAdvancedCommands->WriteImage(punkImage, fX, fY, fWidth, fHeight);
			}

			size_t lMem = 2 * sizeof(LONG) + 4 * sizeof(float);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= ctDrawImage;					m_lPosition += sizeof(LONG);

			*((LONG*)(m_pBuffer + m_lPosition))	= (LONG)m_arImages.GetCount();	m_lPosition += sizeof(LONG);

			*((float*)(m_pBuffer + m_lPosition))= fX;							m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= fY;							m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= fWidth;						m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= fHeight;						m_lPosition += sizeof(float);

			ADDREFINTERFACE(punkImage);
			m_arImages.Add(punkImage);
		}

		inline void WriteImage(CString strPath, const float& fX, const float& fY, const float& fWidth, const float& fHeight)
		{
			if (m_bIsAdvanced)
			{
				return m_pAdvancedCommands->WriteImage(strPath, fX, fY, fWidth, fHeight);
			}
			
			size_t lMem = 2 * sizeof(LONG) + 4 * sizeof(float);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= ctDrawImageFromFile;			m_lPosition += sizeof(LONG);

			*((LONG*)(m_pBuffer + m_lPosition))	= (LONG)m_arStrings.GetCount();	m_lPosition += sizeof(LONG);

			*((float*)(m_pBuffer + m_lPosition))= fX;							m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= fY;							m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= fWidth;						m_lPosition += sizeof(float);
			*((float*)(m_pBuffer + m_lPosition))= fHeight;						m_lPosition += sizeof(float);

			m_arStrings.Add(strPath);
		}

		inline void WriteParams(const double& dAngle, const double& x, const double& y, const double& width, const double& height,	const LONG& lFlags)
		{
			if (m_bIsAdvanced)
			{
				return m_pAdvancedCommands->WriteParams(dAngle, x, y, width, height, lFlags);
			}
			
			size_t lMem = 2 * sizeof(LONG) + 5 * sizeof(double);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= ctSetParams;	m_lPosition += sizeof(LONG);

			*((double*)(m_pBuffer + m_lPosition))= dAngle;		m_lPosition += sizeof(double);
			*((double*)(m_pBuffer + m_lPosition))= x;			m_lPosition += sizeof(double);
			*((double*)(m_pBuffer + m_lPosition))= y;			m_lPosition += sizeof(double);
			*((double*)(m_pBuffer + m_lPosition))= width;		m_lPosition += sizeof(double);
			*((double*)(m_pBuffer + m_lPosition))= height;		m_lPosition += sizeof(double);

			*((LONG*)(m_pBuffer + m_lPosition))	= lFlags;		m_lPosition += sizeof(LONG);
		}

		inline void WriteSetTransform(double d1, double d2, double d3, double d4, double d5, double d6)
		{
			if (m_bIsAdvanced)
			{
				return m_pAdvancedCommands->WriteSetTransform(d1, d2, d3, d4, d5, d6);
			}
			
			size_t lMem = sizeof(LONG) + 6 * sizeof(double);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= ctSetTransform;	m_lPosition += sizeof(LONG);

			*((double*)(m_pBuffer + m_lPosition))= d1;				m_lPosition += sizeof(double);
			*((double*)(m_pBuffer + m_lPosition))= d2;				m_lPosition += sizeof(double);
			*((double*)(m_pBuffer + m_lPosition))= d3;				m_lPosition += sizeof(double);
			*((double*)(m_pBuffer + m_lPosition))= d4;				m_lPosition += sizeof(double);
			*((double*)(m_pBuffer + m_lPosition))= d5;				m_lPosition += sizeof(double);
			*((double*)(m_pBuffer + m_lPosition))= d6;				m_lPosition += sizeof(double);		
		}
		inline void WriteResetTransform()
		{
			if (m_bIsAdvanced)
			{
				return m_pAdvancedCommands->WriteResetTransform();
			}
			
			size_t lMem = sizeof(LONG);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= ctResetTransform;	m_lPosition += sizeof(LONG);
		}

		inline void WriteClipMode(LONG lMode)
		{
			if (m_bIsAdvanced)
			{
				return m_pAdvancedCommands->WriteClipMode(lMode);
			}
			
			size_t lMem = 2 * sizeof(LONG);

			CheckBufferSize(lMem);

			*((LONG*)(m_pBuffer + m_lPosition))	= ctClipMode;	m_lPosition += sizeof(LONG);
			*((LONG*)(m_pBuffer + m_lPosition))	= lMode;		m_lPosition += sizeof(LONG);
		}

		inline void InitAdvancedCommands()
		{
			RELEASEOBJECT(m_pAdvancedCommands);
			m_bIsAdvanced = TRUE;
			m_pAdvancedCommands = new CBufferPage();

			m_pAdvancedCommands->m_fWidth	= m_fWidth;
			m_pAdvancedCommands->m_fHeight	= m_fHeight;
		}

		inline void Draw(IAVSOfficeRendererTemplate* pRenderer, double dDPIX, double dDPIY, BOOL* pBreak)
		{
			if (NULL == pRenderer)
				return;

			// выставляем размеры страницы
			pRenderer->SetWidth(m_fWidth);
			pRenderer->SetHeight(m_fHeight);

			// отрисовываем все объекты...
			size_t lPosMem = 0;

			LONG lSizeofDouble	= sizeof(double);
			LONG lSizeofFloat 	= sizeof(float);
			LONG lSizeofLONG	= sizeof(LONG);

			m_pBufferMem = m_pBuffer;

			//CTimeMeasurer oTimer;
			//oTimer.Reset();

			//while (lPosMem < m_lPosition)
			//{
			//	CommandType eCommand = (CommandType)(*((LONG*)(m_pBuffer + lPosMem)));
			//	lPosMem += lSizeofLONG;

			//	HRESULT hRes = S_OK;

			//	switch (eCommand)
			//	{
			//	case ctSetPen:
			//		{
			//			LONG lIndex = ReadLONG(lPosMem);
			//			lPosMem += lSizeofLONG;

			//			CComBSTR oBSTR(m_arStrings[lIndex]);
			//			hRes = pRenderer->SetPen(oBSTR.m_str);

			//			break;
			//		}
			//	case ctSetBrush:
			//		{
			//			LONG lIndex = ReadLONG(lPosMem);
			//			lPosMem += lSizeofLONG;

			//			CComBSTR oBSTR(m_arStrings[lIndex]);
			//			hRes = pRenderer->SetBrush(oBSTR.m_str);

			//			break;
			//		}
			//	case ctSetFont:
			//		{
			//			LONG lIndex = ReadLONG(lPosMem);
			//			lPosMem += lSizeofLONG;

			//			CComBSTR oBSTR(m_arStrings[lIndex]);
			//			hRes = pRenderer->SetFont(oBSTR.m_str);

			//			break;
			//		}
			//	case ctSetShadow:
			//		{
			//			LONG lIndex = ReadLONG(lPosMem);
			//			lPosMem += lSizeofLONG;

			//			CComBSTR oBSTR(m_arStrings[lIndex]);
			//			hRes = pRenderer->SetShadow(oBSTR.m_str);
			//			
			//			break;
			//		}
			//	case ctSetEdgeText:
			//		{
			//			LONG lIndex = ReadLONG(lPosMem);
			//			lPosMem += lSizeofLONG;

			//			CComBSTR oBSTR(m_arStrings[lIndex]);
			//			hRes = pRenderer->SetEdgeText(oBSTR.m_str);

			//			break;
			//		}

			//	case ctDrawText:
			//	case ctPathCommandText:
			//		{
			//			LONG lIndex			= ReadLONG(lPosMem);	lPosMem += lSizeofLONG;

			//			float fX			= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY			= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fWidth		= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fHeight		= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fBaseOffset	= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;

			//			CComBSTR oBSTR(m_arStrings[lIndex]);
			//			
			//			if (ctDrawText == eCommand)
			//			{
			//				hRes = pRenderer->CommandDrawText(oBSTR.m_str, fX, fY, fWidth, fHeight, fBaseOffset);
			//			}
			//			else
			//			{
			//				IAVSOfficeRendererTemplate2* pTemplate2 = NULL;
			//				pRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate2), (void**)&pTemplate2);

			//				if (NULL != pTemplate2)
			//				{
			//					hRes = pTemplate2->PathCommandText(oBSTR.m_str, fX, fY, fWidth, fHeight, fBaseOffset);
			//					RELEASEINTERFACE(pTemplate2);
			//				}
			//			}

			//			break;
			//		}

			//	case ctPathCommandMoveTo:
			//		{
			//			float fX = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;

			//			hRes = pRenderer->PathCommandMoveTo(fX, fY);
			//			
			//			break;
			//		}
			//	case ctPathCommandLineTo:
			//		{
			//			float fX = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;

			//			hRes = pRenderer->PathCommandLineTo(fX, fY);
			//			
			//			break;
			//		}
			//	case ctPathCommandLinesTo:
			//		{
			//			LONG lCount = ReadLONG(lPosMem);	lPosMem += lSizeofLONG;
			//			ULONG lFloats = lCount * lSizeofFloat;

			//			SAFEARRAYBOUND rgsab;
			//			rgsab.lLbound	= 0;
			//			rgsab.cElements	= lCount;

			//			SAFEARRAY* pArray = SafeArrayCreate(VT_R4, 1, &rgsab);
			//			memcpy(pArray->pvData, m_pBuffer + lPosMem, lFloats);

			//			lPosMem += lFloats;

			//			hRes = pRenderer->PathCommandLinesTo(&pArray);

			//			RELEASEARRAY(pArray);
			//			
			//			break;
			//		}
			//	case ctPathCommandCurveTo:
			//		{
			//			float fX1 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY1 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fX2 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY2 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fX3 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY3 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;

			//			hRes = pRenderer->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
			//			
			//			break;
			//		}
			//	case ctPathCommandCurvesTo:
			//		{
			//			LONG lCount = ReadLONG(lPosMem);	lPosMem += lSizeofLONG;
			//			ULONG lFloats = lCount * lSizeofFloat;

			//			SAFEARRAYBOUND rgsab;
			//			rgsab.lLbound	= 0;
			//			rgsab.cElements	= lCount;

			//			SAFEARRAY* pArray = SafeArrayCreate(VT_R4, 1, &rgsab);
			//			memcpy(pArray->pvData, m_pBuffer + lPosMem, lFloats);

			//			lPosMem += lFloats;

			//			hRes = pRenderer->PathCommandCurvesTo(&pArray);

			//			RELEASEARRAY(pArray);
			//			
			//			break;
			//		}
			//	case ctPathCommandArcTo:
			//		{
			//			float fX1 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY1 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fX2 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY2 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fX3 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY3 = ReadFloat(lPosMem);	lPosMem += lSizeofFloat;

			//			hRes = pRenderer->PathCommandArcTo(fX1, fY1, fX2, fY2, fX3, fY3);
			//			
			//			break;
			//		}
			//	case ctPathCommandClose:
			//		{
			//			hRes = pRenderer->PathCommandClose();
			//			
			//			break;
			//		}
			//	case ctPathCommandEnd:
			//		{
			//			hRes = pRenderer->PathCommandEnd();

			//			break;
			//		}
			//	case ctDrawPath:
			//		{
			//			LONG lType = ReadLONG(lPosMem);	lPosMem += lSizeofLONG;

			//			hRes = pRenderer->DrawPath(lType);

			//			break;
			//		}
			//	case ctPathCommandStart:
			//		{
			//			hRes = pRenderer->PathCommandStart();

			//			break;
			//		}
			//	case ctDrawImage:
			//		{
			//			LONG lIndex			= ReadLONG(lPosMem);	lPosMem += lSizeofLONG;
			//			
			//			float fX			= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY			= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fWidth		= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fHeight		= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;

			//			hRes = pRenderer->DrawImage(&m_arImages[lIndex], fX, fY, fWidth, fHeight);
			//			
			//			break;
			//		}
			//	case ctDrawImageFromFile:
			//		{
			//			LONG lIndex			= ReadLONG(lPosMem);	lPosMem += lSizeofLONG;
			//			
			//			float fX			= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fY			= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fWidth		= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;
			//			float fHeight		= ReadFloat(lPosMem);	lPosMem += lSizeofFloat;

			//			ImageStudio::Serialize::Paint::Common::CDrawImageFromFile oEffect;

			//			oEffect.Left	= (double)fX;
			//			oEffect.Top		= (double)fY;
			//			oEffect.Right	= (double)(fX + fWidth);
			//			oEffect.Bottom  = (double)(fY + fHeight);

			//			oEffect.m_dWidthMM	= (double)m_fWidth;
			//			oEffect.m_dHeightMM	= (double)m_fHeight;

			//			oEffect.FilePath = m_arStrings[lIndex];
			//			hRes = oEffect.Draw(pRenderer, dDPIX, dDPIY) ? S_OK : S_FALSE;

			//			break;
			//		}
			//	case ctSetParams:
			//		{
			//			double angle		= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;
			//			double left			= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;
			//			double top			= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;
			//			double width		= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;
			//			double height		= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;
			//			LONG   flags		= ReadLONG(lPosMem);	lPosMem += lSizeofLONG;

			//			hRes = pRenderer->SetCommandParams(angle, left, top, width, height, flags);

			//			break;
			//		}
			//	case ctBeginCommand:
			//		{
			//			LONG lType			= ReadLONG(lPosMem);	lPosMem += lSizeofLONG;

			//			hRes = pRenderer->BeginCommand(lType);
			//			break;
			//		}
			//	case ctEndCommand:
			//		{
			//			LONG lType			= ReadLONG(lPosMem);	lPosMem += lSizeofLONG;

			//			hRes = pRenderer->EndCommand(lType);

			//			break;
			//		}
			//	case ctSetTransform:
			//		{
			//			double d1		= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;
			//			double d2		= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;
			//			double d3		= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;
			//			double d4		= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;
			//			double d5		= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;
			//			double d6		= ReadDouble(lPosMem);	lPosMem += lSizeofDouble;

			//			IAVSOfficeRendererTemplate2* pTemplate2 = NULL;
			//			pRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate2), (void**)&pTemplate2);

			//			if (NULL != pTemplate2)
			//			{
			//				pTemplate2->SetTransform(d1, d2, d3, d4, d5, d6);
			//				RELEASEINTERFACE(pTemplate2);
			//			}
			//			break;
			//		}
			//	case ctResetTransform:
			//		{
			//			IAVSOfficeRendererTemplate2* pTemplate2 = NULL;
			//			pRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate2), (void**)&pTemplate2);

			//			if (NULL != pTemplate2)
			//			{
			//				pTemplate2->ResetTransform();
			//				RELEASEINTERFACE(pTemplate2);
			//			}
			//			break;
			//		}
			//	case ctClipMode:
			//		{
			//			LONG lMode		= ReadLONG(lPosMem);	lPosMem += lSizeofLONG;
			//			
			//			IAVSOfficeRendererTemplate2* pTemplate2 = NULL;
			//			pRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate2), (void**)&pTemplate2);

			//			if (NULL != pTemplate2)
			//			{
			//				pTemplate2->put_ClipMode(lMode);
			//				RELEASEINTERFACE(pTemplate2);
			//			}
			//			break;
			//		}
			//	default:
			//		{
			//			// мало ли что тут произошло, уж лучше выйдем
			//			return;
			//		}
			//	};

			//	if (S_OK != hRes)
			//		break;
			//}

			HRESULT hRes = S_OK;
			while (m_lPosition > (m_pBufferMem - m_pBuffer))
			{
				LONG eCommand = ReadLONG();
				
				switch (eCommand)
				{
				case ctSetPen:
					{
						LONG lIndex = ReadLONG();

						CComBSTR oBSTR(m_arStrings[lIndex]);
						hRes = pRenderer->SetPen(oBSTR.m_str);

						break;
					}
				case ctSetBrush:
					{
						LONG lIndex = ReadLONG();

						CComBSTR oBSTR(m_arStrings[lIndex]);
						hRes = pRenderer->SetBrush(oBSTR.m_str);

						break;
					}
				case ctSetFont:
					{
						LONG lIndex = ReadLONG();

						CComBSTR oBSTR(m_arStrings[lIndex]);
						hRes = pRenderer->SetFont(oBSTR.m_str);

						break;
					}
				case ctSetShadow:
					{
						LONG lIndex = ReadLONG();

						CComBSTR oBSTR(m_arStrings[lIndex]);
						hRes = pRenderer->SetShadow(oBSTR.m_str);
						
						break;
					}
				case ctSetEdgeText:
					{
						LONG lIndex = ReadLONG();

						CComBSTR oBSTR(m_arStrings[lIndex]);
						hRes = pRenderer->SetEdgeText(oBSTR.m_str);

						break;
					}

				case ctDrawText:
				case ctPathCommandText:
					{
						LONG lIndex			= ReadLONG();

						float fX			= ReadFloat();
						float fY			= ReadFloat();
						float fWidth		= ReadFloat();
						float fHeight		= ReadFloat();
						float fBaseOffset	= ReadFloat();

						CComBSTR oBSTR(m_arStrings[lIndex]);
						
						if (ctDrawText == eCommand)
						{
							hRes = pRenderer->CommandDrawText(oBSTR.m_str, fX, fY, fWidth, fHeight, fBaseOffset);
						}
						else
						{
							IAVSOfficeRendererTemplate2* pTemplate2 = NULL;
							pRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate2), (void**)&pTemplate2);

							if (NULL != pTemplate2)
							{
								hRes = pTemplate2->PathCommandText(oBSTR.m_str, fX, fY, fWidth, fHeight, fBaseOffset);
								RELEASEINTERFACE(pTemplate2);
							}
						}

						break;
					}

				case ctPathCommandMoveTo:
					{
						float fX = ReadFloat();
						float fY = ReadFloat();

						hRes = pRenderer->PathCommandMoveTo(fX, fY);
						
						break;
					}
				case ctPathCommandLineTo:
					{
						float fX = ReadFloat();
						float fY = ReadFloat();

						hRes = pRenderer->PathCommandLineTo(fX, fY);
						
						break;
					}
				case ctPathCommandLinesTo:
					{
						LONG lCount = ReadLONG();
						ULONG lFloats = lCount * lSizeofFloat;

						SAFEARRAYBOUND rgsab;
						rgsab.lLbound	= 0;
						rgsab.cElements	= lCount;

						SAFEARRAY* pArray = SafeArrayCreate(VT_R4, 1, &rgsab);
						memcpy(pArray->pvData, m_pBufferMem, lFloats);

						m_pBufferMem += lFloats;

						hRes = pRenderer->PathCommandLinesTo(&pArray);

						RELEASEARRAY(pArray);
						
						break;
					}
				case ctPathCommandCurveTo:
					{
						float fX1 = ReadFloat();
						float fY1 = ReadFloat();
						float fX2 = ReadFloat();
						float fY2 = ReadFloat();
						float fX3 = ReadFloat();
						float fY3 = ReadFloat();

						hRes = pRenderer->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
						
						break;
					}
				case ctPathCommandCurvesTo:
					{
						LONG lCount = ReadLONG();
						ULONG lFloats = lCount * lSizeofFloat;

						SAFEARRAYBOUND rgsab;
						rgsab.lLbound	= 0;
						rgsab.cElements	= lCount;

						SAFEARRAY* pArray = SafeArrayCreate(VT_R4, 1, &rgsab);
						memcpy(pArray->pvData, m_pBufferMem, lFloats);

						m_pBufferMem += lFloats;

						hRes = pRenderer->PathCommandCurvesTo(&pArray);

						RELEASEARRAY(pArray);
						
						break;
					}
				case ctPathCommandArcTo:
					{
						float fX1 = ReadFloat();
						float fY1 = ReadFloat();
						float fX2 = ReadFloat();
						float fY2 = ReadFloat();
						float fX3 = ReadFloat();
						float fY3 = ReadFloat();

						hRes = pRenderer->PathCommandArcTo(fX1, fY1, fX2, fY2, fX3, fY3);
						
						break;
					}
				case ctPathCommandClose:
					{
						hRes = pRenderer->PathCommandClose();
						
						break;
					}
				case ctPathCommandEnd:
					{
						hRes = pRenderer->PathCommandEnd();

						break;
					}
				case ctDrawPath:
					{
						LONG lType = ReadLONG();

						hRes = pRenderer->DrawPath(lType);

						break;
					}
				case ctPathCommandStart:
					{
						hRes = pRenderer->PathCommandStart();

						break;
					}
				case ctDrawImage:
					{
						LONG lIndex			= ReadLONG();
						
						float fX			= ReadFloat();
						float fY			= ReadFloat();
						float fWidth		= ReadFloat();
						float fHeight		= ReadFloat();

						hRes = pRenderer->DrawImage(&m_arImages[lIndex], fX, fY, fWidth, fHeight);
						
						break;
					}
				case ctDrawImageFromFile:
					{
						LONG lIndex			= ReadLONG();
					
						float fX			= ReadFloat();
						float fY			= ReadFloat();
						float fWidth		= ReadFloat();
						float fHeight		= ReadFloat();

						BSTR bsFile = m_arStrings[lIndex].AllocSysString();

						pRenderer->DrawImageFromFile(bsFile, fX, fY, fWidth, fHeight);

						SysFreeString(bsFile);

						break;
					}
				case ctSetParams:
					{
						double angle		= ReadDouble();
						double left			= ReadDouble();
						double top			= ReadDouble();
						double width		= ReadDouble();
						double height		= ReadDouble();
						LONG   flags		= ReadLONG();

						hRes = pRenderer->SetCommandParams(angle, left, top, width, height, flags);

						break;
					}
				case ctBeginCommand:
					{
						LONG lType			= ReadLONG();

						hRes = pRenderer->BeginCommand(lType);
						break;
					}
				case ctEndCommand:
					{
						LONG lType			= ReadLONG();

						hRes = pRenderer->EndCommand(lType);

						break;
					}
				case ctSetTransform:
					{
						double d1		= ReadDouble();
						double d2		= ReadDouble();
						double d3		= ReadDouble();
						double d4		= ReadDouble();
						double d5		= ReadDouble();
						double d6		= ReadDouble();

						IAVSOfficeRendererTemplate2* pTemplate2 = NULL;
						pRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate2), (void**)&pTemplate2);

						if (NULL != pTemplate2)
						{
							pTemplate2->SetTransform(d1, d2, d3, d4, d5, d6);
							RELEASEINTERFACE(pTemplate2);
						}
						break;
					}
				case ctResetTransform:
					{
						IAVSOfficeRendererTemplate2* pTemplate2 = NULL;
						pRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate2), (void**)&pTemplate2);

						if (NULL != pTemplate2)
						{
							pTemplate2->ResetTransform();
							RELEASEINTERFACE(pTemplate2);
						}
						break;
					}
				case ctClipMode:
					{
						LONG lMode		= ReadLONG();
						
						IAVSOfficeRendererTemplate2* pTemplate2 = NULL;
						pRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate2), (void**)&pTemplate2);

						if (NULL != pTemplate2)
						{
							pTemplate2->put_ClipMode(lMode);
							RELEASEINTERFACE(pTemplate2);
						}
						break;
					}
				default:
					{
						// мало ли что тут произошло, уж лучше выйдем
						return;
					}
				};

				if (S_OK != hRes)
					break;

				if (TRUE == *pBreak)
				{
					// отрисовщик остановлен
					return;
				}
			}

			/*float time = oTimer.GetTimeInterval();
			
			FILE* file = fopen("c:\\test.txt", "a+");
			fprintf(file, "%f\n", time);
			fclose(file);*/

			if (m_bIsAdvanced)
			{
				m_pAdvancedCommands->Draw(pRenderer, dDPIX, dDPIY, pBreak);
			}
		}
	};

protected:

	CBufferPage m_oPage;

public:

	CPageCommands() : m_oPage()
	{
	}

	~CPageCommands()
	{
	}

public:
	STDMETHOD(NewPage)()
	{
		return S_OK;
	}

	STDMETHOD(get_Height)(float* fHeight)
	{
		if (NULL != fHeight)
			*fHeight = m_oPage.m_fHeight;
		return S_OK;
	}
	STDMETHOD(get_Width)(float* fWidth)
	{		
		if (NULL != fWidth)
			*fWidth = m_oPage.m_fWidth;
		return S_OK;
	}
	STDMETHOD(SetHeight)(float fHeight)
	{
		m_oPage.m_fHeight = fHeight;
		return S_OK;
	}
	STDMETHOD(SetWidth)(float fWidth)
	{		
		m_oPage.m_fWidth = fWidth;		
		return S_OK;
	}
	// ---------------------------------------------
	// функции состояния откисовщика ---------------
	STDMETHOD(SetPen)(BSTR bsXML)
	{
		m_oPage.Write(CBufferPage::ctSetPen, (CString)bsXML);		
		return S_OK;
	}
	STDMETHOD(SetBrush)(BSTR bsXML)
	{
		m_oPage.Write(CBufferPage::ctSetBrush, (CString)bsXML);		
		return S_OK;
	}
	STDMETHOD(SetFont)(BSTR bsXML)
	{
		m_oPage.Write(CBufferPage::ctSetFont, (CString)bsXML);		
		return S_OK;
	}
	STDMETHOD(SetShadow)(BSTR bsXML)
	{
		//m_oPage.Write(CBufferPage::ctSetShadow, (CString)bsXML);		
		return S_OK;
	}
	STDMETHOD(SetEdgeText)(BSTR bsXML)
	{
		//m_oPage.Write(CBufferPage::ctSetEdgeText, (CString)bsXML);		
		return S_OK;
	}
	// ---------------------------------------------
	// отрисовка текста
	STDMETHOD(CommandDrawText)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset)
	{
		m_oPage.WriteString((CString)bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		return S_OK;
	}
	// ---------------------------------------------
	// GraphicPath Methods
	STDMETHOD(PathCommandMoveTo)(float fX, float fY)
	{
		m_oPage.Write(CBufferPage::ctPathCommandMoveTo, fX, fY);
		return S_OK;
	}
	STDMETHOD(PathCommandLineTo)(float fX, float fY)
	{
		m_oPage.Write(CBufferPage::ctPathCommandLineTo, fX, fY);
		return S_OK;
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY** ppPoints)
	{
		LONG lCount = (*ppPoints)->rgsabound[0].cElements;
		float* pData = (float*)((*ppPoints)->pvData);

		m_oPage.Write(CBufferPage::ctPathCommandLinesTo, lCount, pData);
		return S_OK;
	}

	STDMETHOD(PathCommandCurveTo)(float fX1, float fY1, float fX2, float fY2, float fX3, float fY3)
	{
		m_oPage.Write(CBufferPage::ctPathCommandCurveTo, fX1, fY1, fX2, fY2, fX3, fY3);
		return S_OK;
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY** ppPoints)
	{
		LONG lCount = (*ppPoints)->rgsabound[0].cElements;
		float* pData = (float*)((*ppPoints)->pvData);

		m_oPage.Write(CBufferPage::ctPathCommandCurvesTo, lCount, pData);
		return S_OK;
	}
	STDMETHOD(PathCommandArcTo)(float fX, float fY, float fWidth, float fHeight, float fStartAngle, float fSweepAngle)
	{
		m_oPage.Write(CBufferPage::ctPathCommandArcTo, 
			fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);

		return S_OK;
	}
	STDMETHOD(PathCommandClose)()
	{
		m_oPage.Write(CBufferPage::ctPathCommandClose);
		return S_OK;
	}
	STDMETHOD(PathCommandEnd)()
	{
		m_oPage.Write(CBufferPage::ctPathCommandEnd);
		return S_OK;
	}

	STDMETHOD(PathCommandStart)()
	{
		m_oPage.Write(CBufferPage::ctPathCommandStart);
		return S_OK;
	}
	STDMETHOD(PathCommandGetCurrentPoint)(float* fX, float* fY)
	{
		return S_OK;
	}
	
	STDMETHOD(DrawPath)(long nType)
	{
		m_oPage.WritePath(nType);
		return S_OK;		
	}
	// ---------------------------------------------
	STDMETHOD(DrawImage)(IUnknown **pInterface, float fX, float fY, float fWidth, float fHeight)
	{
		m_oPage.WriteImage(*pInterface, fX, fY, fWidth, fHeight);
		return S_OK;	
	}
	// ---------------------------------------------
	STDMETHOD(DrawImageFromFile)(BSTR bstrPath, float fX, float fY, float fWidth, float fHeight)
	{
		m_oPage.WriteImage((CString)bstrPath, fX, fY, fWidth, fHeight);
		return S_OK;	
	}
	//----------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	
	// отрисовка страницы
	STDMETHOD(Draw)(IUnknown* punkRenderer, double dDPIX, double dDPIY, BOOL* pBreak)
	{
		IAVSOfficeRendererTemplate* pRenderer = NULL;
		punkRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate), (void**)&pRenderer);

		if (NULL == pRenderer)
			return S_FALSE;

		m_oPage.Draw(pRenderer, dDPIX, dDPIY, pBreak);

		RELEASEINTERFACE(pRenderer);

		return S_OK;
	}
	STDMETHOD(Draw2)(IUnknown* punkRenderer, double dDPIX, double dDPIY, BOOL* pBreak)
	{
		IAVSOfficeRendererTemplate* pRenderer = NULL;
		punkRenderer->QueryInterface(__uuidof(IAVSOfficeRendererTemplate), (void**)&pRenderer);

		if (NULL == pRenderer)
			return S_FALSE;

		m_oPage.Draw(pRenderer, dDPIX, dDPIY, pBreak);

		RELEASEINTERFACE(pRenderer);

		return S_OK;
	}

	STDMETHOD(BeginCommand)(DWORD lType)
	{
		m_oPage.Write(CBufferPage::ctBeginCommand, (LONG)lType);
		return S_OK;	
	}
	STDMETHOD(EndCommand)(DWORD lType)
	{
		m_oPage.Write(CBufferPage::ctEndCommand, lType);
		return S_OK;
	}
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset, DWORD lFlags, BSTR sParams)
	{
		return CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		return S_OK;
	}
	STDMETHOD(SetCommandParams)(double dAngle,  double dLeft,  double dTop,  double dWidth,  double dHeight,  DWORD lFlags)
	{
		m_oPage.WriteParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
		return S_OK;
	}

	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		m_oPage.WriteSetTransform(dA, dB, dC, dD, dE, dF);
		return S_OK;
	}
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		return S_FALSE;
	}
	STDMETHOD(ResetTransform)(void)
	{
		m_oPage.WriteResetTransform();
		return S_OK;
	}

	STDMETHOD(get_ClipMode)(LONG* plMode)
	{
		return S_OK;
	}
	STDMETHOD(put_ClipMode)(LONG lMode)
	{
		m_oPage.WriteClipMode(lMode);
		return S_OK;
	}

	STDMETHOD(PathCommandText)(BSTR bsText, float fX, float fY, float fWidth, float fHeight, float fBaseLineOffset)
	{
		m_oPage.WriteString((CString)bsText, fX, fY, fWidth, fHeight, fBaseLineOffset, TRUE);
		return S_OK;
	}

	STDMETHOD(InitAdvancedCommands)()
	{
		m_oPage.InitAdvancedCommands();
		return S_OK;
	}
	STDMETHOD(DestroyAdvancedCommands)()
	{
		m_oPage.ReleaseAdvanced();
		return S_OK;
	}
};


