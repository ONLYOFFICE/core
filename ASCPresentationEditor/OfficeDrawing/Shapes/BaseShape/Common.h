#pragma once
//todo убрать include
#include <atlbase.h>
#include <atlcom.h>
#include "../../../../Common/atldefine.h"
#include "../../../../ASCImageStudio3/ASCGraphics/Interfaces/ASCRenderer.h"
#import "../../../../Redist/ASCGraphics.dll"				named_guids raw_interfaces_only rename_namespace("ASCGraphics"), exclude("IASCRenderer")

const double ShapeSize		= 43200.0;
const LONG ShapeSizeVML		= 21600;
const double RadKoef = M_PI/10800000.0;

#ifndef		pow2_16
#define		pow2_16			65536
#endif

#ifndef		pow3_16
#define		pow3_16			60000
#endif


namespace NSMath
{
	inline LONG round(double dVal)
	{
		return (LONG)(2 * dVal) - (LONG)(dVal);
	}
}

namespace NSStringUtils
{
	#define IS_ALPHA(c) (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
	#define IS_DIGIT(c) (((c >= '0') && (c <= '9')) || (c == '-'))

	static bool IsDigit(const TCHAR& c)
	{
		return (((c >= '0') && (c <= '9')) || (c == '-'));
	}
	static bool IsAlpha(const TCHAR& c)
	{
		return (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')));
	}
	static bool IsNumber(CString str)
	{
		for (int nIndex = 0; nIndex < str.GetLength(); ++nIndex)
		{
			if (!IsDigit(str[nIndex]))
			{
				return false;
			}
		}
		return true;
	}

	static CString ToString(LONG val)
	{
		CString str = _T("");
		str.Format(_T("%d"), val);
		return str;
	}
	
	static void ParseString(CString strDelimeters, CString strSource, 
		CSimpleArray<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayResults)
			return;

		if (bIsCleared)
			pArrayResults->RemoveAll();

		CString resToken;
		int curPos= 0;

		resToken = strSource.Tokenize(strDelimeters, curPos);
		while (resToken != _T(""))
		{
			pArrayResults->Add(resToken);
			resToken = strSource.Tokenize(strDelimeters, curPos);
		};

	}
	static void ParseString(CSimpleArray<char>* pArrayDelimeters, CString strSource, 
		CSimpleArray<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayDelimeters)
			return;

		CString strDelimeters = _T("");
		for (int nIndex = 0; nIndex < pArrayDelimeters->GetSize(); ++nIndex)
			strDelimeters += (*pArrayDelimeters)[nIndex];

		return ParseString(strDelimeters, strSource, pArrayResults, bIsCleared);
	}
	static void ParsePath(CString strSource, CSimpleArray<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayResults)
			return;
		
		CString strPath = strSource;
		//strPath.Replace(_T(" "), _T(","));
		for (int nIndex = 0; nIndex < strPath.GetLength(); ++nIndex)
		{
			if (nIndex == (strPath.GetLength() - 1))
				continue;

			if (IsAlpha(strPath[nIndex]) && (',' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
				strPath.Insert(nIndex + 1, '0');
				++nIndex;
			}
			else if ((',' == strPath[nIndex]) && (',' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, '0');
				++nIndex;
			}
			else if ((',' == strPath[nIndex]) && IsAlpha(strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, '0');
				++nIndex;
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && IsDigit(strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && IsAlpha(strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('@' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('#' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('@' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('#' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('$' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('?' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('$' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('?' == strPath[nIndex + 1]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
			else if ((IsAlpha(strPath[nIndex]) && IsAlpha(strPath[nIndex + 1])) && ('x' == strPath[nIndex]))
			{
				strPath.Insert(nIndex + 1, ',');
				++nIndex;
			}
		}

		ParseString(_T(","), strPath, pArrayResults, bIsCleared);
		return;
	}

	static void ParsePath2(CString strSource, CSimpleArray<CString>* pArrayResults, bool bIsCleared = true)
    {
        if (NULL == pArrayResults)
            return;
        
        CString strPath = strSource;
		int nLength = strPath.GetLength();
        //strPath.Replace(_T(" "), _T(","));
		if (strPath.Find(TCHAR('h')) != -1)
		{
			TCHAR* pBuff = new TCHAR[nLength + 1];
			int nCur = 0;
			for (int i = 1; i < nLength; ++i)
			{
				TCHAR _c = strPath[i - 1];
				if (_c != TCHAR('h'))
				{
					pBuff[nCur++] = _c;
				}
				else
				{
					TCHAR _c1 = strPath[i];
					if (_c1 == TCHAR('a') ||
						_c1 == TCHAR('b') ||
						_c1 == TCHAR('c') ||
						_c1 == TCHAR('d') ||
						_c1 == TCHAR('e') ||
						_c1 == TCHAR('f') ||
						_c1 == TCHAR('g') ||
						_c1 == TCHAR('h') ||
						_c1 == TCHAR('i'))
					{
						++i;
					}
				}
			}

			if (nLength > 0)
				pBuff[nCur++] = strPath[nLength - 1];

			pBuff[nCur] = 0;

			strPath = CString(pBuff, nCur);
			nLength = strPath.GetLength();

			if (NULL != pBuff)
			{
				delete []pBuff;
				pBuff = NULL;
			}
		}

		if (nLength > 0 && strPath[nLength - 1] == TCHAR(','))
		{
			strPath += _T("0");
			++nLength;
		}

        int nIndexOld = 0;
        for (int nIndex = 0; nIndex < nLength; ++nIndex)
        {
            if (nIndex == (nLength - 1))
			{
				pArrayResults->Add(strPath.Mid(nIndexOld));
				//continue;
			}

			TCHAR _c	= strPath[nIndex];
			TCHAR _c1	= strPath[nIndex + 1];

			if (_c1 == ',')
			{
				if (',' == _c)
				{
					pArrayResults->Add(_T("0"));
				}
				else if (IS_ALPHA(_c))
				{
					pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
					pArrayResults->Add(_T("0"));
				}
				else if (IS_DIGIT(_c))
				{
					pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
				}
			}
			else if (',' == _c)
			{
				if (IS_ALPHA(_c1))
				{
					pArrayResults->Add(_T("0"));
					nIndexOld = nIndex + 1;
				}
				else if (IS_DIGIT(_c1))
				{
					nIndexOld = nIndex + 1;
				}
			}
			else
			{
				bool _isA = IS_ALPHA(_c);
				bool _isD = _isA ? false : IS_DIGIT(_c);

				bool _isA1 = IS_ALPHA(_c1);
				bool _isD1 = _isA1 ? false : IS_DIGIT(_c1);

				if (_isA && _isD1)
				{
					pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
					nIndexOld = nIndex + 1;
				}
				else if (_isD && _isA1)
				{
					pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
					nIndexOld = nIndex + 1;
				}
				else if (_isD && ('@' == _c1))
				{
					pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (_isD && ('#' == _c1))
				{
					pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (_isA && ('@' == _c1))
				{
					pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (_isA && ('#' == _c1))
				{
					pArrayResults->Add(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (('x' == _c) && _isA1)
				{
					pArrayResults->Add(_T("x"));
					nIndexOld = nIndex + 1;
				}				
			}            
        }

        //ParseString(_T(","), strPath, pArrayResults, bIsCleared);
        return;
    }

	static void CheckPathOn_Fill_Stroke(CString& strPath, bool& bFill, bool& bStroke)
	{
		bFill = true;
		bStroke = true;
		int nIndex = strPath.Find(_T("nf"));
		if (-1 != nIndex)
		{
			bFill = false;
			while (-1 != nIndex)
			{
				strPath.Delete(nIndex, 2);
				nIndex = strPath.Find(_T("nf"));
			}
		}
		nIndex = strPath.Find(_T("ns"));
		if (-1 != nIndex)
		{
			bStroke = false;
			while (-1 != nIndex)
			{
				strPath.Delete(nIndex, 2);
				nIndex = strPath.Find(_T("ns"));
			}
		}

		nIndex = strPath.Find(_T("F"));
		if (-1 != nIndex)
		{
			bFill = false;
			while (-1 != nIndex)
			{
				strPath.Delete(nIndex, 2);
				nIndex = strPath.Find(_T("F"));
			}
		}

		nIndex = strPath.Find(_T("S"));
		if (-1 != nIndex)
		{
			bStroke = false;
			while (-1 != nIndex)
			{
				strPath.Delete(nIndex, 2);
				nIndex = strPath.Find(_T("S"));
			}
		}
	}
}

#ifdef AVS_USE_CONVERT_PPTX_TOCUSTOM_VML

#include "../../../../ASCImageStudio3/ASCGraphics/Interfaces/ASCRenderer.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CAVSOOXToVMLGeometry
[ coclass, default(IASCRenderer), threading(apartment), vi_progid("AVSPPTX.VMLShape"), progid("AVSPPTX.VMLShape.1"), version(1.0), uuid("3266F3E4-B6AB-4440-BC58-F38E78BBCCBD") ]
class ATL_NO_VTABLE CAVSOOXToVMLGeometry : public IASCRenderer
{
private:
	class _CStringWriter
	{
	private:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		_CStringWriter()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;
		}
		~_CStringWriter()
		{
			RELEASEMEM(m_pData);
		}

		__forceinline void AddSize(size_t nSize)
		{
			if (NULL == m_pData)
			{
				m_lSize = max(nSize, 1000);				
				m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				
				m_lSizeCur = 0;
				m_pDataCur = m_pData;
				return;
			}

			if ((m_lSizeCur + nSize) > m_lSize)
			{
				while ((m_lSizeCur + nSize) > m_lSize)
				{
					m_lSize *= 2;
				}

				wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
				if (NULL != pRealloc)
				{
					// реаллок сработал
					m_pData		= pRealloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
				else
				{
					wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

					free(m_pData);
					m_pData		= pMalloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
			}
		}

	public:			
		__forceinline size_t GetCurSize()
		{
			return m_lSizeCur;
		}

		__forceinline void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		__forceinline void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

		__forceinline CString GetData()
		{
			CString str(m_pData, (int)m_lSizeCur);
			return str;
		}

		__forceinline void AddCharNoCheck(const WCHAR& wc)
		{
			*m_pDataCur++ = wc;
			++m_lSizeCur;
		}
		__forceinline void AddIntNoCheck(int val)
		{
			if (0 == val)
			{
				*m_pDataCur++ = (WCHAR)'0';
				++m_lSizeCur;
				return;
			}
			if (val < 0)
			{
				val = -val;
				*m_pDataCur++ = (WCHAR)'-';
				++m_lSizeCur;
			}

			int len = 0;
			int oval = val;
			while (oval > 0)
			{
				oval /= 10;
				++len;
			}

			oval = 1;
			while (val > 0)
			{
				m_pDataCur[len - oval] = (WCHAR)('0' + (val % 10));
				++oval;
				val /= 10;
			}

			m_pDataCur += len;
			m_lSizeCur += len;
		}

		__forceinline void AddStringNoCheck(const wchar_t* pData, const int& len)
		{
			memcpy(m_pDataCur, pData, len << 1);
			m_pDataCur += len;
			m_lSizeCur += len;
		}
		__forceinline void AddSpaceNoCheck()
		{
			*m_pDataCur = WCHAR(' ');
			++m_pDataCur;
			++m_lSizeCur;
		}
	};

public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	
	CAVSOOXToVMLGeometry()
	{
	}
	~CAVSOOXToVMLGeometry()
	{
	}

public:
	// ------------------------------------ renderer --------------------------------------------
	STDMETHOD(get_Type)(LONG* lType) { return S_OK; }
	//-------- Функции для работы со страницей --------------------------------------------------
	STDMETHOD(NewPage)() { return S_OK; }
	STDMETHOD(get_Height)(double* dHeight) { return S_OK; }
	STDMETHOD(put_Height)(double dHeight) 
	{
		m_dScaleY = dHeight;
		return S_OK; 
	}
	STDMETHOD(get_Width)(double* dWidth) { return S_OK; }
	STDMETHOD(put_Width)(double dWidth) 
	{
		m_dScaleX = dWidth;
		return S_OK; 
	}

	STDMETHOD(get_DpiX)(double* dDpiX) { return S_OK; }
	STDMETHOD(get_DpiY)(double* dDpiY) { return S_OK; }
// pen --------------------------------------------------------------------------------------
	STDMETHOD(SetPen)(BSTR bsXML) { return S_OK; }
	STDMETHOD(get_PenColor)(LONG* lColor) { return S_OK; }
	STDMETHOD(put_PenColor)(LONG lColor) { return S_OK; }
	STDMETHOD(get_PenAlpha)(LONG* lAlpha) { return S_OK; }
	STDMETHOD(put_PenAlpha)(LONG lAlpha) 
	{
		m_bIsStrokePart = (0 != lAlpha) ? TRUE : FALSE;
		return S_OK; 
	}
	STDMETHOD(get_PenSize)(double* dSize) { return S_OK; }
	STDMETHOD(put_PenSize)(double dSize) { return S_OK; }
	STDMETHOD(get_PenDashStyle)(BYTE* val) { return S_OK; }
	STDMETHOD(put_PenDashStyle)(BYTE val) { return S_OK; }
	STDMETHOD(get_PenLineStartCap)(BYTE* val) { return S_OK; }
	STDMETHOD(put_PenLineStartCap)(BYTE val) { return S_OK; }
	STDMETHOD(get_PenLineEndCap)(BYTE* val) { return S_OK; }
	STDMETHOD(put_PenLineEndCap)(BYTE val) { return S_OK; }
	STDMETHOD(get_PenLineJoin)(BYTE* val) { return S_OK; }
	STDMETHOD(put_PenLineJoin)(BYTE val) { return S_OK; }
	STDMETHOD(get_PenDashOffset)(double* val) { return S_OK; }
	STDMETHOD(put_PenDashOffset)(double val) { return S_OK; }
	STDMETHOD(get_PenAlign)(LONG* val) { return S_OK; }
	STDMETHOD(put_PenAlign)(LONG val) { return S_OK; }
	STDMETHOD(get_PenMiterLimit)(double* val) { return S_OK; }
	STDMETHOD(put_PenMiterLimit)(double val) { return S_OK; }
	STDMETHOD(PenDashPattern)(SAFEARRAY* pPattern) { return S_OK; }
// brush ------------------------------------------------------------------------------------
	STDMETHOD(SetBrush)(BSTR bsXML) { return S_OK; }
	STDMETHOD(get_BrushType)(LONG* lType) { return S_OK; }
	STDMETHOD(put_BrushType)(LONG lType) { return S_OK; }
	STDMETHOD(get_BrushColor1)(LONG* lColor) { return S_OK; }
	STDMETHOD(put_BrushColor1)(LONG lColor) { return S_OK; }
	STDMETHOD(get_BrushAlpha1)(LONG* lAlpha) { return S_OK; }
	STDMETHOD(put_BrushAlpha1)(LONG lAlpha) 
	{
		m_bIsFillPart = (0 != lAlpha) ? TRUE : FALSE;
		return S_OK; 
	}
	STDMETHOD(get_BrushColor2)(LONG* lColor) { return S_OK; }
	STDMETHOD(put_BrushColor2)(LONG lColor) { return S_OK; }
	STDMETHOD(get_BrushAlpha2)(LONG* lAlpha) { return S_OK; }
	STDMETHOD(put_BrushAlpha2)(LONG lAlpha) { return S_OK; }
	STDMETHOD(get_BrushTexturePath)(BSTR* bsPath) { return S_OK; }
	STDMETHOD(put_BrushTexturePath)(BSTR bsPath) { return S_OK; }
	STDMETHOD(get_BrushTextureMode)(LONG* lMode) { return S_OK; }
	STDMETHOD(put_BrushTextureMode)(LONG lMode) { return S_OK; }
	STDMETHOD(get_BrushTextureAlpha)(LONG* lTxAlpha) { return S_OK; }
	STDMETHOD(put_BrushTextureAlpha)(LONG lTxAlpha) { return S_OK; }
	STDMETHOD(get_BrushLinearAngle)(double* dAngle) { return S_OK; }
	STDMETHOD(put_BrushLinearAngle)(double dAngle) { return S_OK; }
	STDMETHOD(BrushRect)(BOOL val, double left, double top, double width, double height) { return S_OK; }
// font -------------------------------------------------------------------------------------
	STDMETHOD(SetFont)(BSTR bsXML) { return S_OK; }
	STDMETHOD(get_FontName)(BSTR* bsName) { return S_OK; }
	STDMETHOD(put_FontName)(BSTR bsName) { return S_OK; }
	STDMETHOD(get_FontPath)(BSTR* bsName) { return S_OK; }
	STDMETHOD(put_FontPath)(BSTR bsName) { return S_OK; }
	STDMETHOD(get_FontSize)(double* dSize) { return S_OK; }
	STDMETHOD(put_FontSize)(double dSize){ return S_OK; }
	STDMETHOD(get_FontStyle)(LONG* lStyle) { return S_OK; }
	STDMETHOD(put_FontStyle)(LONG lStyle) { return S_OK; }
	STDMETHOD(get_FontStringGID)(BOOL* bGID) { return S_OK; }
	STDMETHOD(put_FontStringGID)(BOOL bGID) { return S_OK; }
	STDMETHOD(get_FontCharSpace)(double* dSpace) { return S_OK; }
	STDMETHOD(put_FontCharSpace)(double dSpace) { return S_OK; }
// shadow -----------------------------------------------------------------------------------
	STDMETHOD(SetShadow)(BSTR bsXML) { return S_OK; }
	STDMETHOD(get_ShadowDistanceX)(double* val) { return S_OK; }
	STDMETHOD(put_ShadowDistanceX)(double val) { return S_OK; }
	STDMETHOD(get_ShadowDistanceY)(double* val) { return S_OK; }
	STDMETHOD(put_ShadowDistanceY)(double val) { return S_OK; }
	STDMETHOD(get_ShadowBlurSize)(double* val) { return S_OK; }
	STDMETHOD(put_ShadowBlurSize)(double val) { return S_OK; }
	STDMETHOD(get_ShadowColor)(LONG* val) { return S_OK; }
	STDMETHOD(put_ShadowColor)(LONG val) { return S_OK; }
	STDMETHOD(get_ShadowAlpha)(LONG* val) { return S_OK; }
	STDMETHOD(put_ShadowAlpha)(LONG val) { return S_OK; }
	STDMETHOD(get_ShadowVisible)(BOOL* val) { return S_OK; }
	STDMETHOD(put_ShadowVisible)(BOOL val) { return S_OK; }
// edge -------------------------------------------------------------------------------------
	STDMETHOD(SetEdgeText)(BSTR bsXML) { return S_OK; }
	STDMETHOD(get_EdgeVisible)(LONG* val) { return S_OK; }
	STDMETHOD(put_EdgeVisible)(LONG val) { return S_OK; }
	STDMETHOD(get_EdgeColor)(LONG* val) { return S_OK; }
	STDMETHOD(put_EdgeColor)(LONG val) { return S_OK; }
	STDMETHOD(get_EdgeAlpha)(LONG* val) { return S_OK; }
	STDMETHOD(put_EdgeAlpha)(LONG val) { return S_OK; }
	STDMETHOD(get_EdgeDist)(double* val) { return S_OK; }
	STDMETHOD(put_EdgeDist)(double val) { return S_OK; }
//-------- Функции для вывода текста --------------------------------------------------------
	STDMETHOD(CommandDrawText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset) { return S_OK; }
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags) { return S_OK; }
//-------- Маркеры для команд ---------------------------------------------------------------
	STDMETHOD(BeginCommand)(DWORD lType) 
	{
		m_lCurrentCommandType = lType;
		return S_OK; 
	}
	STDMETHOD(EndCommand)(DWORD lType) 
	{ 
		m_lCurrentCommandType = -1;

		if (lType == c_nPathType)
		{
			if (!m_bIsFillPart)
			{
				m_oWriter.AddCharNoCheck((WCHAR)'n');
				m_oWriter.AddCharNoCheck((WCHAR)'f');
			}
			if (!m_bIsStrokePart)
			{
				m_oWriter.AddCharNoCheck((WCHAR)'n');
				m_oWriter.AddCharNoCheck((WCHAR)'s');
			}
			m_oWriter.AddCharNoCheck((WCHAR)'e');
			m_lCountPathCommands = 0;
		}

		return S_OK; 
	}
//-------- Функции для работы с Graphics Path -----------------------------------------------
	STDMETHOD(PathCommandMoveTo)(double fX, double fY) 
	{
		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			MoveTo(fX * m_dScaleX, fY * m_dScaleY);
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandMoveTo(fX, fY);
		}
		return S_OK;		
	}
	STDMETHOD(PathCommandLineTo)(double fX, double fY) 
	{ 
		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			LineTo(fX * m_dScaleX, fY * m_dScaleY);
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandLineTo(fX, fY);
		}

		return S_OK;
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints) 
	{ 
		m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);
		return S_OK;
	}
	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3) 
	{ 
		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			CurveTo(fX1 * m_dScaleX, fY1 * m_dScaleY, fX2 * m_dScaleX, fY2 * m_dScaleY, fX3 * m_dScaleX, fY3 * m_dScaleY);
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		}

		return S_OK;
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints) 
	{ 
		m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);
		return S_OK;
	}
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle) 
	{ 
		m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
		return S_OK;
	}
	STDMETHOD(PathCommandClose)() 
	{
		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			Close();
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandClose();
		}
		return S_OK;
	}
	STDMETHOD(PathCommandEnd)() { return S_OK; }
	STDMETHOD(DrawPath)(long nType) { return S_OK; }
	STDMETHOD(PathCommandStart)() { return S_OK; }
	STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY) 
	{
		if (NULL != m_pSimpleGraphicsConverter)
			return m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);
		return S_OK; 
	}
	STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset) { return S_OK; }
	STDMETHOD(PathCommandTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags) { return S_OK; }
//-------- Функции для вывода изображений ---------------------------------------------------
	STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight) { return S_OK; }
	STDMETHOD(DrawImageFromFile)(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight) { return S_OK; }
// transform --------------------------------------------------------------------------------
	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags) { return S_OK; }
	STDMETHOD(SetCommandParams)(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags) { return S_OK; }
	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF) { return S_OK; }
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF) { return S_OK; }
	STDMETHOD(ResetTransform)(void) { return S_OK; }
// -----------------------------------------------------------------------------------------
	STDMETHOD(get_ClipMode)(LONG* plMode) { return S_OK; }
	STDMETHOD(put_ClipMode)(LONG lMode) { return S_OK; }
// additiaonal params ----------------------------------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue) 
	{
		CString name = (CString)ParamName;
		if (name == _T("NewShape"))
		{
			m_pSimpleGraphicsConverter->PathCommandEnd();
			m_oWriter.ClearNoAttack();
		}
		return S_OK; 
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue) 
	{ 
		CString name = (CString)ParamName;
		if (name == _T("ResultPath") && NULL != ParamValue)
		{
			BSTR bs = m_oWriter.GetData().AllocSysString();
			ParamValue->vt = VT_BSTR;
			ParamValue->bstrVal = bs;
		}
		return S_OK; 
	}
// --------------------------------------------------------------------------------------------

public:
	HRESULT FinalConstruct()
	{
		m_bIsFillPart = FALSE;
		m_bIsStrokePart = FALSE;
		
		m_dScaleX = 1.0;
		m_dScaleY = 1.0;

		m_pSimpleGraphicsConverter = NULL;
		CoCreateInstance(ASCGraphics::CLSID_CASCGraphicSimpleComverter, NULL, CLSCTX_ALL, 
			ASCGraphics::IID_IASCGraphicSimpleComverter, (void**)&m_pSimpleGraphicsConverter);

		IUnknown* punkRenderer = NULL;
		this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

		m_pSimpleGraphicsConverter->put_Renderer(punkRenderer);
		RELEASEINTERFACE(punkRenderer);

		m_lCountPathCommands = 0;

		return S_OK;
	}
	void FinalRelease() 
	{
		RELEASEINTERFACE(m_pSimpleGraphicsConverter);
	}

private:
	
	ASCGraphics::IASCGraphicSimpleComverter*	m_pSimpleGraphicsConverter;
	BOOL m_bIsFillPart;
	BOOL m_bIsStrokePart;

	double m_dScaleX;
	double m_dScaleY;

	LONG m_lCountPathCommands;

	_CStringWriter m_oWriter;

	LONG m_lCurrentCommandType;	// текущая команда
	
	inline void MoveTo(const double& dX, const double& dY)
	{
		m_oWriter.AddSize(30);
		m_oWriter.AddCharNoCheck((WCHAR)'m');
		m_oWriter.AddIntNoCheck(int(dX + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(dY + 0.5));

		m_lCountPathCommands++;
	}
	inline void LineTo(const double& dX, const double& dY)
	{
		m_oWriter.AddSize(30);
		m_oWriter.AddCharNoCheck((WCHAR)'l');
		m_oWriter.AddIntNoCheck(int(dX + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(dY + 0.5));

		m_lCountPathCommands++;
	}
	inline void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
	{
		m_oWriter.AddSize(80);
		m_oWriter.AddCharNoCheck((WCHAR)'c');
		m_oWriter.AddIntNoCheck(int(x1 + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(y1 + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(x2 + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(y2 + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(x3 + 0.5));
		m_oWriter.AddCharNoCheck((WCHAR)',');
		m_oWriter.AddIntNoCheck(int(y3 + 0.5));

		m_lCountPathCommands++;
	}
	void Start()
	{
		m_lCountPathCommands = 0;
	}
	void End()
	{
		m_lCountPathCommands = 0;
	}
	void Close()
	{
		if (1 >= m_lCountPathCommands)
		{
			// 2007 office имеет проблемы с путями вида moveto-close.
			return;
		}

		m_oWriter.AddCharNoCheck((WCHAR)'x');
		m_lCountPathCommands++;
	}	
};

#endif