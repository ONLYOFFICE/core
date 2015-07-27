#pragma once

#include <algorithm>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
	#include <atlbase.h>
	#include <atlstr.h>
	#include "../../../../Common/atldefine.h"

#else
    #include "../../../../Common/DocxFormat/Source/Base/ASCString.h"
#endif

#include "../../../../Common/DocxFormat/Source/Base/Base.h"
#include "../../../../DesktopEditor/graphics/GraphicsPath.h"

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
		std::vector<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayResults)
			return;

		if (bIsCleared)
			pArrayResults->clear();

		CString resToken;
		int curPos= 0;

		resToken = strSource.Tokenize(strDelimeters, curPos);
		while (resToken != _T(""))
		{
			pArrayResults->push_back(resToken);
			resToken = strSource.Tokenize(strDelimeters, curPos);
		};

	}
	static void ParseString(std::vector<char>* pArrayDelimeters, CString strSource, 
		std::vector<CString>* pArrayResults, bool bIsCleared = true)
	{
		if (NULL == pArrayDelimeters)
			return;

		CString strDelimeters = _T("");
		for (int nIndex = 0; nIndex < pArrayDelimeters->size(); ++nIndex)
			strDelimeters += (*pArrayDelimeters)[nIndex];

		return ParseString(strDelimeters, strSource, pArrayResults, bIsCleared);
	}
	static void ParsePath(CString strSource, std::vector<CString>* pArrayResults, bool bIsCleared = true)
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

	static void ParsePath2(CString strSource, std::vector<CString>* pArrayResults, bool bIsCleared = true)
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
				pArrayResults->push_back(strPath.Mid(nIndexOld));
				//continue;
			}

			TCHAR _c	= strPath[nIndex];
			TCHAR _c1	= strPath[nIndex + 1];

			if (_c1 == ',')
			{
				if (',' == _c)
				{
					pArrayResults->push_back(_T("0"));
				}
				else if (IS_ALPHA(_c))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
					pArrayResults->push_back(_T("0"));
				}
				else if (IS_DIGIT(_c))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
				}
			}
			else if (',' == _c)
			{
				if (IS_ALPHA(_c1))
				{
					pArrayResults->push_back(_T("0"));
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
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
					nIndexOld = nIndex + 1;
				}
				else if (_isD && _isA1)
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));
					nIndexOld = nIndex + 1;
				}
				else if (_isD && ('@' == _c1))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (_isD && ('#' == _c1))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (_isA && ('@' == _c1))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (_isA && ('#' == _c1))
				{
					pArrayResults->push_back(strPath.Mid(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (('x' == _c) && _isA1)
				{
					pArrayResults->push_back(_T("x"));
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

class COOXToVMLGeometry : public IRenderer
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

        AVSINLINE void AddSize(size_t nSize)
		{
			if (NULL == m_pData)
			{
                m_lSize = (std::max)((int)nSize, (int)1000);
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
        AVSINLINE size_t GetCurSize()
		{
			return m_lSizeCur;
		}

        AVSINLINE void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
        AVSINLINE void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

        AVSINLINE CString GetData()
		{
			CString str(m_pData, (int)m_lSizeCur);
			return str;
		}

        AVSINLINE void AddCharNoCheck(const WCHAR& wc)
		{
			*m_pDataCur++ = wc;
			++m_lSizeCur;
		}
        AVSINLINE void AddIntNoCheck(int val)
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

        AVSINLINE void AddStringNoCheck(const wchar_t* pData, const int& len)
		{
			memcpy(m_pDataCur, pData, len << 1);
			m_pDataCur += len;
			m_lSizeCur += len;
		}
        AVSINLINE void AddSpaceNoCheck()
		{
			*m_pDataCur = WCHAR(' ');
			++m_pDataCur;
			++m_lSizeCur;
		}
	};


public:
	COOXToVMLGeometry()
	{
                m_bIsFillPart = false;
                m_bIsStrokePart = false;
		
		m_dScaleX = 1.0;
		m_dScaleY = 1.0;

		m_pSimpleGraphicsConverter = new Aggplus::CGraphicsPathSimpleConverter();
		m_pSimpleGraphicsConverter->SetRenderer(this);

		m_lCountPathCommands = 0;
	}
	~COOXToVMLGeometry()
	{
		RELEASEOBJECT(m_pSimpleGraphicsConverter);
	}
// тип рендерера-----------------------------------------------------------------------------
	virtual HRESULT get_Type(LONG* lType){ return S_OK; }
//-------- Функции для работы со страницей --------------------------------------------------
	virtual HRESULT NewPage(){ return S_OK; }
	virtual HRESULT get_Height(double* dHeight){ return S_OK; }
	virtual HRESULT put_Height(const double& dHeight)
	{
		m_dScaleY = dHeight;
		return S_OK;
	}
	virtual HRESULT get_Width(double* dWidth){ return S_OK; }
	virtual HRESULT put_Width(const double& dWidth)
	{
		m_dScaleX = dWidth;
		return S_OK; 
	}
	virtual HRESULT get_DpiX(double* dDpiX){ return S_OK; }
	virtual HRESULT get_DpiY(double* dDpiY){ return S_OK; }

// pen --------------------------------------------------------------------------------------
	virtual HRESULT get_PenColor(LONG* lColor){ return S_OK; }
	virtual HRESULT put_PenColor(const LONG& lColor){ return S_OK; }
	virtual HRESULT get_PenAlpha(LONG* lAlpha){ return S_OK; }
	virtual HRESULT put_PenAlpha(const LONG& lAlpha)
	{
                m_bIsStrokePart = (0 != lAlpha) ? true : false;
		return S_OK; 
	}
	virtual HRESULT get_PenSize(double* dSize){ return S_OK; }
	virtual HRESULT put_PenSize(const double& dSize){ return S_OK; }
	virtual HRESULT get_PenDashStyle(BYTE* val){ return S_OK; }
	virtual HRESULT put_PenDashStyle(const BYTE& val){ return S_OK; }
	virtual HRESULT get_PenLineStartCap(BYTE* val){ return S_OK; }
	virtual HRESULT put_PenLineStartCap(const BYTE& val){ return S_OK; }
	virtual HRESULT get_PenLineEndCap(BYTE* val){ return S_OK; }
	virtual HRESULT put_PenLineEndCap(const BYTE& val){ return S_OK; }
	virtual HRESULT get_PenLineJoin(BYTE* val){ return S_OK; }
	virtual HRESULT put_PenLineJoin(const BYTE& val){ return S_OK; }
	virtual HRESULT get_PenDashOffset(double* dOffset){ return S_OK; }
	virtual HRESULT put_PenDashOffset(const double& dOffset){ return S_OK; }
	virtual HRESULT get_PenAlign(LONG* lAlign){ return S_OK; }
	virtual HRESULT put_PenAlign(const LONG& lAlign){ return S_OK; }
	virtual HRESULT get_PenMiterLimit(double* dOffset){ return S_OK; }
	virtual HRESULT put_PenMiterLimit(const double& dOffset){ return S_OK; }
	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount){ return S_OK; }

// brush ------------------------------------------------------------------------------------
	virtual HRESULT get_BrushType(LONG* lType){ return S_OK; }
	virtual HRESULT put_BrushType(const LONG& lType){ return S_OK; }
	virtual HRESULT get_BrushColor1(LONG* lColor){ return S_OK; }
	virtual HRESULT put_BrushColor1(const LONG& lColor){ return S_OK; }
	virtual HRESULT get_BrushAlpha1(LONG* lAlpha)
	{
                m_bIsFillPart = (0 != lAlpha) ? true : false;
		return S_OK; 
	}
	virtual HRESULT put_BrushAlpha1(const LONG& lAlpha){ return S_OK; }
	virtual HRESULT get_BrushColor2(LONG* lColor){ return S_OK; }
	virtual HRESULT put_BrushColor2(const LONG& lColor){ return S_OK; }
	virtual HRESULT get_BrushAlpha2(LONG* lAlpha){ return S_OK; }
	virtual HRESULT put_BrushAlpha2(const LONG& lAlpha){ return S_OK; }
	virtual HRESULT get_BrushTexturePath(std::wstring* bsPath){ return S_OK; }
	virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath){ return S_OK; }
	virtual HRESULT get_BrushTextureMode(LONG* lMode){ return S_OK; }
	virtual HRESULT put_BrushTextureMode(const LONG& lMode){ return S_OK; }
	virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha){ return S_OK; }
	virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha){ return S_OK; }
	virtual HRESULT get_BrushLinearAngle(double* dAngle){ return S_OK; }
	virtual HRESULT put_BrushLinearAngle(const double& dAngle){ return S_OK; }
	virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height){ return S_OK; }
	virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height){ return S_OK; }

	virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount){ return S_OK; }

// font -------------------------------------------------------------------------------------
	virtual HRESULT get_FontName(std::wstring* bsName){ return S_OK; }
	virtual HRESULT put_FontName(const std::wstring& bsName){ return S_OK; }
	virtual HRESULT get_FontPath(std::wstring* bsName){ return S_OK; }
	virtual HRESULT put_FontPath(const std::wstring& bsName){ return S_OK; }
	virtual HRESULT get_FontSize(double* dSize){ return S_OK; }
	virtual HRESULT put_FontSize(const double& dSize){ return S_OK; }
	virtual HRESULT get_FontStyle(LONG* lStyle){ return S_OK; }
	virtual HRESULT put_FontStyle(const LONG& lStyle){ return S_OK; }
	virtual HRESULT get_FontStringGID(INT* bGID){ return S_OK; }
	virtual HRESULT put_FontStringGID(const INT& bGID){ return S_OK; }
	virtual HRESULT get_FontCharSpace(double* dSpace){ return S_OK; }
	virtual HRESULT put_FontCharSpace(const double& dSpace){ return S_OK; }
	virtual HRESULT get_FontFaceIndex(int* lFaceIndex){ return S_OK; }
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex){ return S_OK; }

//-------- Функции для вывода текста --------------------------------------------------------
    virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h) { return S_OK; }

    virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h) { return S_OK; }

//-------- Маркеры для команд ---------------------------------------------------------------
	virtual HRESULT BeginCommand(const DWORD& lType)
	{
		m_lCurrentCommandType = lType;
		return S_OK; 
	}
	virtual HRESULT EndCommand(const DWORD& lType)
	{ 
		m_lCurrentCommandType = -1;

		if (lType == c_nPathType)
		{
			m_oWriter.AddSize(20);
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
	virtual HRESULT PathCommandMoveTo(const double& x, const double& y)
	{
		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			MoveTo(x * m_dScaleX, y * m_dScaleY);
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandMoveTo(x, y);
		}
		return S_OK;		
	}
	virtual HRESULT PathCommandLineTo(const double& x, const double& y)
	{ 
		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			LineTo(x * m_dScaleX, y * m_dScaleY);
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandLineTo(x, y);
		}

		return S_OK;
	}
	virtual HRESULT PathCommandLinesTo(double* points, const int& count)
	{ 
		m_pSimpleGraphicsConverter->PathCommandLinesTo(points, count);
		return S_OK;
	}
	virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
	{ 
		if (c_nSimpleGraphicType == m_lCurrentCommandType)
		{
			CurveTo(x1 * m_dScaleX, y1 * m_dScaleY, x2 * m_dScaleX, y2 * m_dScaleY, x3 * m_dScaleX, y3 * m_dScaleY);
		}
		else
		{
			m_pSimpleGraphicsConverter->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
		}

		return S_OK;
	}
	virtual HRESULT PathCommandCurvesTo(double* points, const int& count)
	{ 
		m_pSimpleGraphicsConverter->PathCommandCurvesTo(points, count);
		return S_OK;
	}
	virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle)
	{ 
		m_pSimpleGraphicsConverter->PathCommandArcTo(x, y, w, h, startAngle, sweepAngle);
		return S_OK;
	}
	virtual HRESULT PathCommandClose()
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
	virtual HRESULT PathCommandEnd(){ return S_OK; }
	virtual HRESULT DrawPath(const LONG& nType){ return S_OK; }
	virtual HRESULT PathCommandStart(){ return S_OK; }
	virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y)
	{
		if (NULL != m_pSimpleGraphicsConverter)
			return m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(x, y);
		return S_OK; 
	}

    virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h) { return S_OK; }

    virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    virtual HRESULT PathCommandTextEx(const std::wstring& sText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h) { return S_OK; }

//-------- Функции для вывода изображений ---------------------------------------------------
	virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h){ return S_OK; }
	virtual HRESULT DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255){ return S_OK; }	

// transform --------------------------------------------------------------------------------
	virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6){ return S_OK; }
	virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF){ return S_OK; }
	virtual HRESULT ResetTransform(){ return S_OK; }

// -----------------------------------------------------------------------------------------
	virtual HRESULT get_ClipMode(LONG* plMode){ return S_OK; }
	virtual HRESULT put_ClipMode(const LONG& lMode){ return S_OK; }

// additiaonal params ----------------------------------------------------------------------
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand){ return S_OK; }
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand){ return S_OK; }
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand){ return S_OK; }

// additiaonal params ----------------------------------------------------------------------
	HRESULT NewShape()
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
		m_oWriter.ClearNoAttack();
		return S_OK;
	}
	HRESULT ResultPath(CString* bstrVal)
	{
		*bstrVal = m_oWriter.GetData();
		return S_OK;
	}
private:
	
	Aggplus::CGraphicsPathSimpleConverter*	m_pSimpleGraphicsConverter;
        bool m_bIsFillPart;
        bool m_bIsStrokePart;

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
		m_oWriter.AddSize(90);
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
