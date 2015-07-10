#include "Utils.h"
#include "../../DesktopEditor/common/String.h"
#include "../../DesktopEditor/common/Types.h"
#include "../../Common/DocxFormat/Source/XML/xmlutils.h"
#include "../../DesktopEditor/graphics/IRenderer.h"

#define MAX_STRING_LEN 2147483648 

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef xpsUnitToMM
#define xpsUnitToMM(x) ((x) * 25.4 / 96)
#endif

#define IsNumber(X) \
		('0' == (X)\
	  || '1' == (X)\
      || '2' == (X)\
      || '3' == (X)\
      || '4' == (X)\
      || '5' == (X)\
      || '6' == (X)\
      || '7' == (X)\
      || '8' == (X)\
      || '9' == (X)\
      || '-' == (X)\
	  || '.' == (X))

#define GetChar(STRING, POS) STRING[POS++] 
#define LookChar(STRING, POS) STRING[POS] 

namespace XPS
{
	static inline bool   SkipWhiteSpaces(const wchar_t* wsString, int& nPos, const int& nLen)
	{
		while (1)
		{
			if (nPos >= nLen)
				return false;
			wchar_t wChar = LookChar(wsString, nPos);
			if (' ' == wChar || ',' == wChar)
			{
				nPos++;
				continue;
			}
			else
				break;
		}

		return true;
	}
	static inline double GetNumber(const wchar_t* wsString, int& nPos, const int& nLen)
	{
		if (!SkipWhiteSpaces(wsString, nPos, nLen))
			return 0.0;

		wchar_t wChar = GetChar(wsString, nPos);
		if (IsNumber(wChar))
		{
			bool bNegative = false;
			int nInt = 0;
			double dFloat = 0.0, dScale = 0.1;

			if ('-' == wChar)
			{
				bNegative = true;
			}
			else if ('.' == wChar)
			{
				goto doReal;
			}
			else
			{
				nInt = wChar - '0';
			}

			while (1)
			{
				wChar = LookChar(wsString, nPos);
				if (isdigit(wChar))
				{
					nPos++;
					nInt = nInt * 10 + (wChar - '0');
				}
				else if ('.' == wChar)
				{
					nPos++;
					dFloat = (double)nInt;
					goto doReal;
				}
				else
				{
					break;
				}
			}

			return (bNegative ? (double)(-nInt) : (double)nInt);

		doReal:

			while (1)
			{
				wChar = LookChar(wsString, nPos);
				if (!isdigit(wChar))
					break;

				nPos++;
				dFloat = dFloat + dScale * (wChar - '0');
				dScale *= 0.1;
			}

			return (bNegative ? (double)(-dFloat) : (double)dFloat);
		}

		return 0.0;
	}
	static inline bool   GetBool(const wchar_t* wsString, int& nPos, const int& nLen)
	{
		if (!SkipWhiteSpaces(wsString, nPos, nLen))
			return false;

		int nBufPos = 0;
		wchar_t pBuffer[32];
		memset(pBuffer, 0x00, sizeof(wchar_t) * 32);

		wchar_t wChar = LookChar(wsString, nPos);
		while (' ' != wChar)
		{
			pBuffer[nBufPos++] = wChar;
			nPos++;

			if (nPos >= nLen)
				break;

			wChar = LookChar(wsString, nPos);
		}

		CWString wsBool(pBuffer, false, nBufPos);
		if ((wsBool == L"true") || (wsBool == L"t") || (wsBool == L"1") || (wsBool == L"on"))
			return true;

		return false;
	}
	static inline void PageCommandA(const wchar_t* wsString, int& nPos, const int& nLen, double& dCurX, double& dCurY, IRenderer* pRenderer)
	{
		Aggplus::CMatrix tmatx, rmatx, smatx, itmatx, irmatx, ismatx;
		double x_cur_temp = dCurX;
		double y_cur_temp = dCurY;

		double rx    = GetNumber(wsString, nPos, nLen);
		double ry    = GetNumber(wsString, nPos, nLen);
		double angle = GetNumber(wsString, nPos, nLen);

		bool isLarge = GetBool(wsString, nPos, nLen);
		bool isCCW	 = !GetBool(wsString, nPos, nLen);

		double x_end = GetNumber(wsString, nPos, nLen);
		double y_end = GetNumber(wsString, nPos, nLen);

		tmatx.Translate(-dCurX, -dCurY);
		tmatx.TransformPoint(x_cur_temp, y_cur_temp);
		tmatx.TransformPoint(x_end, y_end);

		rmatx.Rotate(-angle);
		rmatx.TransformPoint(x_cur_temp, y_cur_temp);
		rmatx.TransformPoint(x_end, y_end);

		smatx.Scale(ry / rx, 1);
		smatx.TransformPoint(x_cur_temp, y_cur_temp);
		smatx.TransformPoint(x_end, y_end);

		double mid_x = (x_cur_temp + x_end) / 2.0, mid_y = (y_cur_temp + y_end) / 2.0;
		double vect_x = x_end - x_cur_temp, vect_y = y_end - y_cur_temp;
		double length = sqrt(vect_x * vect_x + vect_y * vect_y);
		double halfChord = length / 2.0;
		double rotated_x, rotated_y;

		if (isLarge == isCCW)
		{
			rotated_x = -vect_y;
			rotated_y = vect_x;
		}
		else
		{
			rotated_x = vect_y;
			rotated_y = -vect_x;
		}

		rotated_x = rotated_x / length;
		rotated_y = rotated_y / length;

		double centerDistance = sqrt(max(ry * ry - halfChord * halfChord, 0));
		double center_x = mid_x + centerDistance * rotated_x, center_y = mid_y + centerDistance * rotated_y;

		double angle1 = atan2(y_cur_temp - center_y, x_cur_temp - center_x);
		double angle2 = atan2(y_end - center_y, x_end - center_x);

		if ((!isCCW) && (abs(y_end - center_y) < 0.000001) && (x_end - center_x < 0))
			angle2 = -M_PI;
		if ((isCCW) && (abs(y_cur_temp - center_y) < 0.000001) && (x_cur_temp - center_x < 0))
			angle1 = -M_PI;

		if (isLarge == (abs(angle2 - angle1) < M_PI))
		{
			if (angle1 < angle2)
				angle1 += 2 * M_PI;
			else
				angle2 += 2 * M_PI;
		}
		if (abs(angle2 - angle1) == M_PI)
		{
			if ((angle1 < angle2) && (isCCW) && (angle1 != -M_PI) && (angle1 != 0.0) && (angle1 != M_PI))
				angle1 += 2 * M_PI;
		}

		itmatx.Translate(dCurX, dCurY);
		irmatx.Rotate(angle);
		ismatx.Scale(rx / ry, 1);

		int max = (int)((4 * (rx + ry) * abs(angle2 - angle1) / (2 * M_PI)) / 1/*tolerance*/);

		double x, y;
		for (int counter = 0; counter <= max; counter++)
		{
			double angle_cur = ((max - counter) * angle1 + counter * angle2) / max;
			x = center_x + ry * cos(angle_cur);
			y = center_y + ry * sin(angle_cur);

			// Transform the point back
			ismatx.TransformPoint(x, y);
			irmatx.TransformPoint(x, y);
			itmatx.TransformPoint(x, y);
			pRenderer->PathCommandLineTo(xpsUnitToMM(x), xpsUnitToMM(y));
		}

		dCurX = x;
		dCurY = y;
	}
}

namespace XPS
{	
	class CWStringBuffer
	{
	public:

		CWStringBuffer(const wchar_t* wsString, unsigned int unLen)
		{
			if (unLen)
			{
				m_pBuffer = new wchar_t[unLen + 1];
				m_pBuffer[unLen] = 0x00;
				memcpy(m_pBuffer, wsString, sizeof(wchar_t) * unLen);
			}
			else
			{
				m_pBuffer = NULL;
			}

			m_nRefCount = 1;
		}
		void AddRef()
		{
			m_nRefCount++;
		}
		int  Release()
		{
			return --m_nRefCount;
		}
		void Free()
		{
			RELEASEARRAYOBJECTS(m_pBuffer);
		}
		wchar_t operator[](const unsigned int& unIndex) const
		{
			return m_pBuffer[unIndex];
		}

	private:

		wchar_t* m_pBuffer;
		int      m_nRefCount;

		friend class CWString;
	};
	CWString::CWString()
	{
		m_bOwnBuffer = false;
		m_pBuffer    = NULL;
		m_unLen      = 0;
	}
	CWString::CWString(const wchar_t* wsString)
	{
		m_bOwnBuffer = false;
		m_pBuffer    = NULL;
		m_unLen      = 0;
		create(wsString, false);
	}
	CWString::CWString(wchar_t* wsString, bool bCopy, int nLen)
	{
		m_bOwnBuffer = false;
		m_pBuffer    = NULL;
		m_unLen      = 0;
		create(wsString, bCopy, nLen);
	}
	CWString::CWString(const CWString& wsString)
	{
		m_unLen      = wsString.m_unLen;
		m_bOwnBuffer = wsString.m_bOwnBuffer;
		m_pBuffer    = wsString.m_pBuffer;
		if (m_bOwnBuffer && m_pBuffer)
			((CWStringBuffer*)m_pBuffer)->AddRef();
	}
	CWString::~CWString()
	{
		clear();
	}
	void CWString::create(const wchar_t* wsString, bool bCopy, int nLen)
	{
		clear();
		unsigned int unLen = -1 == nLen ? min(wcslen(wsString), MAX_STRING_LEN) : (unsigned int)nLen;
		m_unLen = unLen;
		if (bCopy)
		{
			if (unLen)
			{
				m_pBuffer    = (void*)(new CWStringBuffer(wsString, m_unLen));
				m_bOwnBuffer = true;
			}
		}
		else
		{
			m_pBuffer    = (void*)wsString;
			m_bOwnBuffer = false;
		}
	}
	void CWString::clear()
	{	
		if (m_bOwnBuffer)
		{
			CWStringBuffer* pWStringBuffer = (CWStringBuffer*)m_pBuffer;
			if (pWStringBuffer && !pWStringBuffer->Release())
				delete pWStringBuffer;
		}

		m_bOwnBuffer = false;
		m_pBuffer    = NULL;
		m_unLen      = 0;
	}
	void CWString::operator=(const wchar_t* wsString)
	{
		clear();
		create(wsString, false);
	}
	void CWString::operator=(const CWString& wsString)
	{
		clear();
		m_unLen      = wsString.m_unLen;
		m_bOwnBuffer = wsString.m_bOwnBuffer;
		m_pBuffer    = wsString.m_pBuffer;
		if (m_bOwnBuffer && m_pBuffer)
			((CWStringBuffer*)m_pBuffer)->AddRef();
	}
	const wchar_t* CWString::c_str() const
	{
		if (m_bOwnBuffer)
		{
			CWStringBuffer* pWStringBuffer = (CWStringBuffer*)m_pBuffer;
			if (pWStringBuffer)
				return pWStringBuffer->m_pBuffer;

			return NULL;
		}

		return (const wchar_t*)m_pBuffer;
	}
	bool CWString::operator<(const CWString& wsString) const
	{
		const wchar_t* wsLeft = this->c_str();
		const wchar_t* wsRight = wsString.c_str();

		unsigned int unLen = min(m_unLen, wsString.m_unLen);
		for (unsigned int unPos = 0; unPos < unLen; unPos++)
		{
			if (wsLeft[unPos] < wsRight[unPos])
				return true;
			else if (wsLeft[unPos] > wsRight[unPos])
				return false;
		}

		return (m_unLen > wsString.m_unLen);
	}
	bool CWString::operator>(const CWString& wsString) const
	{		
		return !operator<(wsString);
	}
	bool CWString::operator==(const CWString& wsString) const
	{
		const wchar_t* wsLeft = this->c_str();
		const wchar_t* wsRight = wsString.c_str();

		if (m_unLen != wsString.m_unLen)
			return false;

		for (unsigned int unPos = 0; unPos < m_unLen; unPos++)
		{
			if (wsLeft[unPos] != wsRight[unPos])
				return false;
		}

		return true;
	}
	bool CWString::operator==(const wchar_t* wsString) const
	{
		const wchar_t* wsLeft = this->c_str();
		unsigned unLen = min(wcslen(wsString), MAX_STRING_LEN);

		if (m_unLen != unLen)
			return false;

		for (unsigned int unPos = 0; unPos < m_unLen; unPos++)
		{
			if (wsLeft[unPos] != wsString[unPos])
				return false;
		}

		return true;
	}
	unsigned int CWString::size() const 
	{ 
		return m_unLen; 
	}
	wchar_t CWString::operator[](const unsigned int& unIndex) const 
	{ 
		return this->c_str()[unIndex]; 
	}
	bool CWString::empty() const
	{
		return 0 == m_unLen;
	}

	int    GetDigit(wchar_t wChar)
	{
		if (wChar >= '0' && wChar <= '9')
			return (int)(wChar - '0');
		if (wChar >= 'a' && wChar <= 'f')
			return 10 + (int)(wChar - 'a');
		if (wChar >= 'A' && wChar <= 'F')
			return 10 + (int)(wChar - 'A');

		return 0;
	}
	bool   IsAlpha(wchar_t wChar)
	{
		return (((wChar >= 'A') && (wChar <= 'Z')) || ((wChar >= 'a') && (wChar <= 'z')));
	}
	double GetDouble(const std::wstring& wsString)
	{
		return _wtof(wsString.c_str());
	}
	int    GetInteger(const std::wstring& wsString)
	{
		return _wtoi(wsString.c_str());
	}
	bool   GetBool(const std::wstring& wsString)
	{
		std::wstring wsStr = wsString;
		NSStringExt::ToLower(wsStr);
		if ((wsStr == L"true") || (wsStr == L"t") || (wsStr == L"1") || (wsStr == L"on"))
			return true;

		return false;
	}
	void   GetBgra(const std::wstring& wsString, int& nBgr, int& nAlpha)
	{
		if (L'#' == wsString[0])
		{
			std::wstring wsStr = wsString.substr(1);

			while (wsStr.length() < 6)
				wsStr = L"0" + wsStr;

			while (wsStr.length() < 8)
				wsStr = L"F" + wsStr;

			nAlpha = GetDigit(wsStr[0]);
			nAlpha = nAlpha << 4;
			nAlpha += GetDigit(wsStr[1]);

			nBgr = GetDigit(wsStr[6]);
			nBgr = nBgr << 4;
			nBgr += GetDigit(wsStr[7]);
			nBgr = nBgr << 4;
			nBgr += GetDigit(wsStr[4]);
			nBgr = nBgr << 4;
			nBgr += GetDigit(wsStr[5]);
			nBgr = nBgr << 4;
			nBgr += GetDigit(wsStr[2]);
			nBgr = nBgr << 4;
			nBgr += GetDigit(wsStr[3]);
		}
		else if (L's' == wsString[0] && L'c' == wsString[1] && L'#' == wsString[2])
		{
			std::wstring wsStr = wsString.substr(3);
			std::vector<std::wstring> arrElements = NSStringExt::Split(wsStr, L',');

			if (3 == arrElements.size())
			{
				nAlpha = 255;
				nBgr = (((int)(min(GetDouble(arrElements[2]), 1.0) * 255)) << 16) + (((int)(min(GetDouble(arrElements[1]), 1.0) * 255)) << 8) + ((int)(min(GetDouble(arrElements[0]), 1.0) * 255));
			}
			else if (4 == arrElements.size())
			{
				nAlpha = GetDouble(arrElements[0]) * 255;
				nBgr = (((int)(min(GetDouble(arrElements[3]), 1.0) * 255)) << 16) + (((int)(min(GetDouble(arrElements[2]), 1.0) * 255)) << 8) + ((int)(min(GetDouble(arrElements[1]), 1.0) * 255));
			}
		}
		else
			return;
	}
	unsigned char GetCapStyle(const wchar_t* wsCapStyle)
	{
		BYTE nCapStyle = Aggplus::LineCapFlat;
		CWString wsDashCap = wsCapStyle;
		if (wsDashCap == L"Flat")
			nCapStyle = Aggplus::LineCapFlat;
		else if (wsDashCap == L"Round")
			nCapStyle = Aggplus::LineCapRound;
		else if (wsDashCap == L"Square")
			nCapStyle = Aggplus::LineCapSquare;
		else if (wsDashCap == L"Triangle")
			nCapStyle = Aggplus::LineCapTriangle;

		return nCapStyle;
	}
	std::wstring NormalizePath(const std::wstring& wsPath)
	{		
		std::wstring wsResult = wsPath;
		NSStringExt::Replace(wsResult, L"/", L"\\");
		while (std::wstring::npos != wsResult.find(L"\\\\"))
		{			
			NSStringExt::Replace(wsResult, L"\\\\", L"\\");
		}
		return wsResult;
	}
	std::wstring GetPath(const std::wstring& wsPath)
	{
		std::wstring wsResult;
		wsResult = wsPath.substr(0, wsPath.find_last_of('/') + 1);
		return wsResult;
	}
	std::wstring GetFileName(const std::wstring& wsPath)
	{
		int nCommaPos = wsPath.find_last_of(L'.');
		int nSlashPos = wsPath.find_last_of(L'/');

		if (std::wstring::npos == nCommaPos)
			nCommaPos = wsPath.length();
		if (std::wstring::npos == nSlashPos)
			nSlashPos = -1;

		if (nCommaPos < nSlashPos)
			return L"";

		std::wstring wsResult = wsPath.substr(nSlashPos + 1, nCommaPos - nSlashPos - 1);
		return wsResult;
	}
	std::wstring GetFileExtension(const std::wstring& wsPath)
	{
		int nCommaPos = wsPath.find_last_of(L'.');

		if (std::wstring::npos == nCommaPos)
			return L"";

		std::wstring wsResult = wsPath.substr(nCommaPos + 1);
		return wsResult;
	}
	std::wstring RemoveNamespace(const std::wstring& wsString)
	{
		std::wstring wsResult;

		int nPos = wsString.find(L":");
		if (std::wstring::npos != nPos)
			wsResult = wsString.substr(nPos + 1);
		else
			wsResult = wsString;

		return wsResult;
	}
	std::vector<std::vector<std::wstring>> Split(const std::wstring& wsString, wchar_t wDelim1, wchar_t wDelim2)
	{
		std::vector<std::vector<std::wstring>> arrResult;
		std::vector<std::wstring> arrStrings = NSStringExt::Split(wsString, wDelim1);
		int nCount = arrStrings.size();
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			std::vector<std::wstring> arrStr = NSStringExt::Split(arrStrings[nIndex], wDelim2);
			arrResult.push_back(arrStr);
		}
		return arrResult;
	}
	void ReadAttribute(XmlUtils::CXmlLiteReader& oReader, const wchar_t* wsAttrName, std::wstring& wsAttr)
	{
		if (oReader.GetAttributesCount() <= 0)
			return;

		if (!oReader.MoveToFirstAttribute())
			return;

		std::wstring wsName = oReader.GetName();

		while (!wsName.empty())
		{
			if (wsAttrName == wsName)
			{
				wsAttr = oReader.GetText();
				break;
			}
			if (!oReader.MoveToNextAttribute())
				break;

			wsName = oReader.GetName();
		}

		oReader.MoveToElement();
	}
	void ReadAttribute(XmlUtils::CXmlLiteReader& oReader, const wchar_t* wsAttrName, CWString& wsAttr)
	{
		if (oReader.GetAttributesCount() <= 0)
			return;

		if (!oReader.MoveToFirstAttribute())
			return;

		CWString wsName = oReader.GetName();
		while (!wsName.empty())
		{
			if (wsName == wsAttrName)
			{
				wsAttr.create(oReader.GetText(), true);
				break;
			}
			if (!oReader.MoveToNextAttribute())
				break;

			wsName = oReader.GetName();
		}

		oReader.MoveToElement();
	}
	bool VmlToRenderer(const wchar_t* wsString, IRenderer* pRenderer)
	{
		bool bWinding = false;

		int nPos = 0;
		int nLen = wcslen(wsString);

		double dCurX = 0.0, dCurY = 0.0;
		double dCpX = 0.0, dCpY = 0.0;

		bool bPrevCommandIsCurve = false;
		wchar_t wChar = 0x00;
		while (nPos < nLen)
		{
			if (!SkipWhiteSpaces(wsString, nPos, nLen))
				break;

			wChar = GetChar(wsString, nPos);
			switch (wChar)
			{
				case 'F':
				{
					if (!SkipWhiteSpaces(wsString, nPos, nLen))
						break;

					bWinding = GetBool(wsString, nPos, nLen);
					bPrevCommandIsCurve = false;
					break;
				}
				case 'm':
				{
					dCurX += GetNumber(wsString, nPos, nLen);
					dCurY += GetNumber(wsString, nPos, nLen);
					pRenderer->PathCommandMoveTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
					bPrevCommandIsCurve = false;
					break;
				}
				case 'M':
				{
					dCurX = GetNumber(wsString, nPos, nLen);
					dCurY = GetNumber(wsString, nPos, nLen);
					pRenderer->PathCommandMoveTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
					bPrevCommandIsCurve = false;
					break;
				}
				case 'L':
				case 'l':
				case 'H':
				case 'h':
				case 'V':
				case 'v':
				{
					while (true)
					{
						if (!SkipWhiteSpaces(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							switch (wChar)
							{
								case 'L':
								{
									dCurX = GetNumber(wsString, nPos, nLen);
									dCurY = GetNumber(wsString, nPos, nLen);
									break;
								}
								case 'l':
								{
									dCurX = GetNumber(wsString, nPos, nLen);
									dCurY = GetNumber(wsString, nPos, nLen);
									break;
								}
								case 'H':
								{
									dCurX = GetNumber(wsString, nPos, nLen); 
									break;
								}
								case 'h':
								{
									dCurX += GetNumber(wsString, nPos, nLen);
									break;
								}
								case 'V':
								{
									dCurY = GetNumber(wsString, nPos, nLen); 
									break;
								}
								case 'v':
								{
									dCurY += GetNumber(wsString, nPos, nLen);
									break;
								}
							}

							pRenderer->PathCommandLineTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
						}
						else
							break;
					}

					bPrevCommandIsCurve = false;
					break;
				}
				case 'A':
				case 'a':
				{
					while (true)
					{
						if (!SkipWhiteSpaces(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
							PageCommandA(wsString, nPos, nLen, dCurX, dCurY, pRenderer);
						else
							break;
					}
					bPrevCommandIsCurve = false;
					break;
				}
				case 'C':
				case 'c':
				{
					while (true)
					{
						if (!SkipWhiteSpaces(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							double dX1, dY1, dX2, dY2;
							if ('c' == wChar)
							{
								dX1 = dCurX + GetNumber(wsString, nPos, nLen);
								dY1 = dCurY + GetNumber(wsString, nPos, nLen);
								dX2 = dCurX + GetNumber(wsString, nPos, nLen);
								dY2 = dCurY + GetNumber(wsString, nPos, nLen);
								dCurX += GetNumber(wsString, nPos, nLen);
								dCurY += GetNumber(wsString, nPos, nLen);
							}
							else
							{
								dX1 = GetNumber(wsString, nPos, nLen);
								dY1 = GetNumber(wsString, nPos, nLen);
								dX2 = GetNumber(wsString, nPos, nLen);
								dY2 = GetNumber(wsString, nPos, nLen);
								dCurX = GetNumber(wsString, nPos, nLen);
								dCurY = GetNumber(wsString, nPos, nLen);
							}
							pRenderer->PathCommandCurveTo(xpsUnitToMM(dX1), xpsUnitToMM(dY1), xpsUnitToMM(dX2), xpsUnitToMM(dY2), xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
							dCpX = dX2;
							dCpY = dY2;
							bPrevCommandIsCurve = true;
						}
						else
							break;
					}
					break;
				}
				case 'Q':
				case 'q':
				{
					while (true)
					{
						if (!SkipWhiteSpaces(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							double dX1, dY1;
							if ('q' == wChar)
							{
								dX1 = dCurX + GetNumber(wsString, nPos, nLen);
								dY1 = dCurY + GetNumber(wsString, nPos, nLen);
								dCurX += GetNumber(wsString, nPos, nLen);
								dCurY += GetNumber(wsString, nPos, nLen);
							}
							else
							{
								dX1 = GetNumber(wsString, nPos, nLen);
								dY1 = GetNumber(wsString, nPos, nLen);
								dCurX = GetNumber(wsString, nPos, nLen);
								dCurY = GetNumber(wsString, nPos, nLen);
							}
							pRenderer->PathCommandCurveTo(xpsUnitToMM(dX1), xpsUnitToMM(dY1), xpsUnitToMM(dX1), xpsUnitToMM(dY1), xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
						}
						else
							break;
					}
					bPrevCommandIsCurve = false;
					break;
				}
				case 'S':
				case 's':
				{
					while (true)
					{
						if (!SkipWhiteSpaces(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							double dX1, dY1, dX2, dY2;
							if (!bPrevCommandIsCurve)
							{
								dX1 = dCurX;
								dY1 = dCurY;
							}
							else
							{
								dX1 = 2 * dCurX - dCpX;
								dY1 = 2 * dCurY - dCpY;
							}

							if ('s' == wChar)
							{
								dX2 = dCurX + GetNumber(wsString, nPos, nLen);
								dY2 = dCurX + GetNumber(wsString, nPos, nLen);
								dCurX += GetNumber(wsString, nPos, nLen);
								dCurY += GetNumber(wsString, nPos, nLen);
							}
							else
							{
								dX2 = GetNumber(wsString, nPos, nLen);
								dY2 = GetNumber(wsString, nPos, nLen);
								dCurX = GetNumber(wsString, nPos, nLen);
								dCurY = GetNumber(wsString, nPos, nLen);
							}

							pRenderer->PathCommandCurveTo(xpsUnitToMM(dX1), xpsUnitToMM(dY1), xpsUnitToMM(dX2), xpsUnitToMM(dY2), xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
							dCpX = dX2;
							dCpY = dY2;
							bPrevCommandIsCurve = true;
						}
						else
							break;
					}
					break;
				}
				case 'Z':
				case 'z':
				{
					pRenderer->PathCommandClose();
					bPrevCommandIsCurve = false;
					break;
				}
			}
		}

		return bWinding;
	}
}