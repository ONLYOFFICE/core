#include "Utils.h"
#include "../../DesktopEditor/common/String.h"
#include "../../DesktopEditor/common/Types.h"
#include "../../Common/DocxFormat/Source/XML/xmlutils.h"
#include "../../DesktopEditor/graphics/IRenderer.h"

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
	  || '.' == (X)\
      || 'e' == (X)\
	  || 'E' == (X))

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
			if (' ' == wChar)
			{
				nPos++;
				continue;
			}
			else
				break;
		}

		return true;
	}
	static inline bool   SkipWhiteSpacesAndCommas(const wchar_t* wsString, int& nPos, const int& nLen)
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
	static inline double GetDouble(const wchar_t* wsString, int& nPos, const int& nLen)
	{
		if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
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
				else if ('e' == wChar || 'E' == wChar)
				{
					nPos++;
					dFloat = (double)nInt;
					goto doExponent;
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

				if ('e' == wChar || 'E' == wChar)
				{
					nPos++;
					goto doExponent;
				}
				else if (!isdigit(wChar))
					break;

				nPos++;
				dFloat = dFloat + dScale * (wChar - '0');
				dScale *= 0.1;
			}

			return (bNegative ? (double)(-dFloat) : (double)dFloat);

		doExponent:

			wChar = GetChar(wsString, nPos);
			bool bNegativeExponent = false;
			int nExp = 0;
			if ('-' == wChar)
			{
				bNegativeExponent = true;
			}
			else if ('+' == wChar)
			{
				bNegativeExponent = false;
			}
			else
			{
				nExp = wChar - '0';
			}

			while (1)
			{
				wChar = LookChar(wsString, nPos);
				if (!isdigit(wChar))
					break;

				nPos++;
				nExp = nExp * 10 + (wChar - '0');
			}

			dFloat = (bNegative ? (double)(-dFloat) : (double)dFloat);
			nExp = max(20, min(0, nExp));

			while (nExp)
			{
				if (bNegativeExponent)
					dFloat /= 10;
				else
					dFloat *= 10;

				nExp--;
			}

			return dFloat;			
		}

		return 0.0;
	}
	static inline int    GetInt   (const wchar_t* wsString, int& nPos, const int& nLen)
	{
		if (!SkipWhiteSpaces(wsString, nPos, nLen))
			return 0;

		wchar_t wChar = GetChar(wsString, nPos);
		if (IsNumber(wChar))
		{
			bool bNegative = false;
			int nInt = 0;

			if ('-' == wChar)
			{
				bNegative = true;
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
				else
				{
					break;
				}
			}

			return (bNegative ? -nInt : nInt);		
		}

		return 0;
	}
	static inline bool   GetBool(const wchar_t* wsString, int& nPos, const int& nLen)
	{
		if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
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

		double rx    = GetDouble(wsString, nPos, nLen);
		double ry    = GetDouble(wsString, nPos, nLen);
		double angle = GetDouble(wsString, nPos, nLen);

		bool isLarge = GetBool(wsString, nPos, nLen);
		bool isCCW	 = !GetBool(wsString, nPos, nLen);

		double x_end = GetDouble(wsString, nPos, nLen);
		double y_end = GetDouble(wsString, nPos, nLen);

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
	double GetDouble(const CWString& wsString)
	{
		return _wtof(wsString.c_str());
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
			if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
				break;

			wChar = GetChar(wsString, nPos);
			switch (wChar)
			{
				case 'F':
				{
					if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
						break;

					bWinding = GetBool(wsString, nPos, nLen);
					bPrevCommandIsCurve = false;
					break;
				}
				case 'm':
				{
					dCurX += GetDouble(wsString, nPos, nLen);
					dCurY += GetDouble(wsString, nPos, nLen);
					pRenderer->PathCommandMoveTo(xpsUnitToMM(dCurX), xpsUnitToMM(dCurY));
					bPrevCommandIsCurve = false;
					break;
				}
				case 'M':
				{
					dCurX = GetDouble(wsString, nPos, nLen);
					dCurY = GetDouble(wsString, nPos, nLen);
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
						if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							switch (wChar)
							{
								case 'L':
								{
									dCurX = GetDouble(wsString, nPos, nLen);
									dCurY = GetDouble(wsString, nPos, nLen);
									break;
								}
								case 'l':
								{
									dCurX = GetDouble(wsString, nPos, nLen);
									dCurY = GetDouble(wsString, nPos, nLen);
									break;
								}
								case 'H':
								{
									dCurX = GetDouble(wsString, nPos, nLen);
									break;
								}
								case 'h':
								{
									dCurX += GetDouble(wsString, nPos, nLen);
									break;
								}
								case 'V':
								{
									dCurY = GetDouble(wsString, nPos, nLen);
									break;
								}
								case 'v':
								{
									dCurY += GetDouble(wsString, nPos, nLen);
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
						if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
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
						if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							double dX1, dY1, dX2, dY2;
							if ('c' == wChar)
							{
								dX1 = dCurX + GetDouble(wsString, nPos, nLen);
								dY1 = dCurY + GetDouble(wsString, nPos, nLen);
								dX2 = dCurX + GetDouble(wsString, nPos, nLen);
								dY2 = dCurY + GetDouble(wsString, nPos, nLen);
								dCurX += GetDouble(wsString, nPos, nLen);
								dCurY += GetDouble(wsString, nPos, nLen);
							}
							else
							{
								dX1 = GetDouble(wsString, nPos, nLen);
								dY1 = GetDouble(wsString, nPos, nLen);
								dX2 = GetDouble(wsString, nPos, nLen);
								dY2 = GetDouble(wsString, nPos, nLen);
								dCurX = GetDouble(wsString, nPos, nLen);
								dCurY = GetDouble(wsString, nPos, nLen);
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
						if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
							break;

						if (IsNumber(LookChar(wsString, nPos)))
						{
							double dX1, dY1;
							if ('q' == wChar)
							{
								dX1 = dCurX + GetDouble(wsString, nPos, nLen);
								dY1 = dCurY + GetDouble(wsString, nPos, nLen);
								dCurX += GetDouble(wsString, nPos, nLen);
								dCurY += GetDouble(wsString, nPos, nLen);
							}
							else
							{
								dX1 = GetDouble(wsString, nPos, nLen);
								dY1 = GetDouble(wsString, nPos, nLen);
								dCurX = GetDouble(wsString, nPos, nLen);
								dCurY = GetDouble(wsString, nPos, nLen);
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
						if (!SkipWhiteSpacesAndCommas(wsString, nPos, nLen))
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
								dX2 = dCurX + GetDouble(wsString, nPos, nLen);
								dY2 = dCurX + GetDouble(wsString, nPos, nLen);
								dCurX += GetDouble(wsString, nPos, nLen);
								dCurY += GetDouble(wsString, nPos, nLen);
							}
							else
							{
								dX2 = GetDouble(wsString, nPos, nLen);
								dY2 = GetDouble(wsString, nPos, nLen);
								dCurX = GetDouble(wsString, nPos, nLen);
								dCurY = GetDouble(wsString, nPos, nLen);
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
	bool GetNextGlyph(const wchar_t* wsIndices, int& nIndicesPos, const int& nIndicesLen, unsigned short* pUtf16, int& nUtf16Pos, const int& nUtf16Len, TIndicesEntry& oEntry)
	{
		oEntry.Reset();
		if (!wsIndices || nIndicesPos >= nIndicesLen)
		{
			if (!pUtf16 || nUtf16Pos >= nUtf16Len)
				return false;

			oEntry.nUnicode = pUtf16[nUtf16Pos++];
			oEntry.bUnicode = true;
			return true;
		}

		if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
			return false;

		wchar_t wChar = LookChar(wsIndices, nIndicesPos);

		int nCodeUnitCount = 1, nGlyphCount = 1;
		if ('(' == wChar)
		{
			nIndicesPos++;
			if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
				return false;

			wChar = LookChar(wsIndices, nIndicesPos);
			if (IsNumber(wChar))
				nCodeUnitCount = GetInt(wsIndices, nIndicesPos, nIndicesLen);
			else
				nCodeUnitCount = 1;

			if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
				return false;

			wChar = LookChar(wsIndices, nIndicesPos);
			if (':' == wChar)
			{
				nIndicesPos++;
				if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
					return false;

				wChar = LookChar(wsIndices, nIndicesPos);
				if (IsNumber(wChar))
					nGlyphCount = GetInt(wsIndices, nIndicesPos, nIndicesLen);
				else
					nGlyphCount = 1;
			}
			else
				return false;

			if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
				return false;

			wChar = LookChar(wsIndices, nIndicesPos);
			if (')' != wChar)
				return false; // Такого не должно быть

			nIndicesPos++;
		}


		if (nCodeUnitCount > 0 && nGlyphCount > 0)
		{
			oEntry.vRemainUnicodes.clear();
			// Нам нужно прочитать сколько реальных юникодных значений лежит в 
			// промежутке [pUnicode + nUnicodePos, pUnicode + nUnicodePos + nCodeUnitCount]
			int nUnicodesCount = 0;
			unsigned int* pUnicodes = NULL;
			nCodeUnitCount = min(nUtf16Len - nUtf16Pos, nCodeUnitCount);			
			if (nCodeUnitCount)
			{
				pUnicodes = new unsigned int[nCodeUnitCount];
				if (!pUnicodes)
					return false;

				unsigned short ushLeading, ushTraling;
				unsigned int unCode;
				int nCodeUnitPos = 0;
				while (nCodeUnitPos < nCodeUnitCount)
				{
					ushLeading = pUtf16[nUtf16Pos + nCodeUnitPos++];
					if (ushLeading < 0xD800 || ushLeading > 0xDFFF)
					{
						pUnicodes[nUnicodesCount++] = (unsigned int)ushLeading;
					}
					else if (ushLeading >= 0xDC00)
					{
						// Такого не должно быть
						continue;
					}
					else
					{
						if (nCodeUnitPos >= nCodeUnitCount)
							break;

						unCode = (ushLeading & 0x3FF) << 10;
						ushTraling =  pUtf16[nUtf16Pos + nCodeUnitPos++];
						if (ushTraling < 0xDC00 || ushTraling > 0xDFFF)
						{
							// Такого не должно быть
							continue;
						}
						else
						{
							pUnicodes[nUnicodesCount++] = (unCode | (ushTraling & 0x3FF) + 0x10000);
						}
					}
				}
			}

			// Равномерно распределяем юникоды по глифам, в идеале их количество должно совпадать.
			// Если юникодов больше, то лишние удаляем, если их меньше, то недостающие заполняем пробелами.
			nUnicodesCount = min(nUnicodesCount, nGlyphCount);
			for (int nIndex = 0; nIndex < nGlyphCount; nIndex++)
			{
				if (nIndex < nUnicodesCount)
					oEntry.vRemainUnicodes.push_back(pUnicodes[nIndex]);
				else
					oEntry.vRemainUnicodes.push_back(0x20);
			}

			if (pUnicodes)
				delete[] pUnicodes;

			nUtf16Pos += nCodeUnitCount;
		}

		if (oEntry.vRemainUnicodes.size() <= 0)
			return false;

		// Теперь мы читаем ровно 1 глиф с возможными метриками
		oEntry.nUnicode = oEntry.vRemainUnicodes.at(0);
		oEntry.vRemainUnicodes.erase(oEntry.vRemainUnicodes.begin());

		wChar = LookChar(wsIndices, nIndicesPos);
		if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
			return false;

		if (IsNumber(wChar))
		{
			oEntry.nGid = GetInt(wsIndices, nIndicesPos, nIndicesLen);
			oEntry.bGid = true;
		}
		else
		{
			oEntry.bGid = false;
		}

		if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
			return true;

		wChar = LookChar(wsIndices, nIndicesPos);
		if (',' == wChar)
		{
			nIndicesPos++;

			if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
				return true;

			wChar = LookChar(wsIndices, nIndicesPos);
			if (IsNumber(wChar))
			{
				oEntry.dAdvance = GetDouble(wsIndices, nIndicesPos, nIndicesLen);
				oEntry.bAdvance = true;
			}
			else
			{
				oEntry.bAdvance = false;
			}

			if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
				return true;

			wChar = LookChar(wsIndices, nIndicesPos);
			if (',' == wChar)
			{
				nIndicesPos++;
				if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
					return true;

				wChar = LookChar(wsIndices, nIndicesPos);
				if (IsNumber(wChar))
				{
					oEntry.dHorOffset = GetDouble(wsIndices, nIndicesPos, nIndicesLen);
					oEntry.bHorOffset = true;
				}
				else
				{
					oEntry.bHorOffset = false;
				}

				if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
					return true;

				wChar = LookChar(wsIndices, nIndicesPos);
				if (',' == wChar)
				{
					nIndicesPos++;
					if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
						return true;

					wChar = LookChar(wsIndices, nIndicesPos);
					if (IsNumber(wChar))
					{
						oEntry.dVerOffset = GetDouble(wsIndices, nIndicesPos, nIndicesLen);
						oEntry.bVerOffset = true;
					}
					else
					{
						oEntry.bVerOffset = false;
					}
				}
			}
		}

		if (!SkipWhiteSpaces(wsIndices, nIndicesPos, nIndicesLen))
			return true;

		wChar = LookChar(wsIndices, nIndicesPos);
		if (';' == wChar)
		{
			nIndicesPos++;
			return true;
		}
		else
			return false; // Такого не должно быть
	}
	void ReadTransform    (XmlUtils::CXmlLiteReader& oReader, CWString& wsTransform, CWString* pwsKey)
	{
		CWString wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			if (wsNodeName == L"MatrixTransform")
			{
				if (oReader.GetAttributesCount() <= 0)
					return;

				if (!oReader.MoveToFirstAttribute())
					return;

				CWString wsAttrName = oReader.GetName();
				while (!wsAttrName.empty())
				{
					if (wsAttrName == L"Matrix")
						wsTransform.create(oReader.GetText(), true);
					else if (wsAttrName == L"x:Key" && pwsKey)
						pwsKey->create(oReader.GetText(), true);

					if (!oReader.MoveToNextAttribute())
						break;

					wsAttrName = oReader.GetName();					
				}

				oReader.MoveToElement();
			}
		}
	}
	void ReadPathGeometry (XmlUtils::CXmlLiteReader& oReader, CWString& wsData, CWString& wsTransform, CWString* pwsKey)
	{
		bool bEvenOdd = true;
		CWString wsAttrName;
		if (oReader.MoveToFirstAttribute())
		{
			wsAttrName = oReader.GetName();
			while (!wsAttrName.empty())
			{
				if (wsAttrName == L"x:Key" && pwsKey)
					pwsKey->create(oReader.GetText(), true);
				else if (wsAttrName == L"Figures")
					wsData.create(oReader.GetText(), true);
				else if (wsAttrName == L"Transform")
					wsTransform.create(oReader.GetText(), true);
				else if (wsAttrName == L"FillRule")
				{
					CWString wsFillingRule = oReader.GetText();
					bEvenOdd = wsFillingRule == L"EvenOdd" ? true : false;
				}

				if (!oReader.MoveToNextAttribute())
					break;

				wsAttrName = oReader.GetName();
			}

			oReader.MoveToElement();
		}

		if (oReader.IsEmptyNode())
			return;

		CWString wsNodeName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			if (wsNodeName == L"PathGeometry.Transform" && wsTransform.empty())
				ReadTransform(oReader, wsTransform);
			else if (wsNodeName == L"PathFigure" && wsData.empty())
				ReadPathFigure(oReader, wsData, bEvenOdd);
		}
	}
	void ReadPathFigure   (XmlUtils::CXmlLiteReader& oReader, CWString& _wsData, bool bEvenOdd)
	{
		// TODO: Улучшить здесь сложение строк и хождение по атрибутам
		std::wstring wsData;

		if (oReader.IsEmptyNode())
			return;

		if (!bEvenOdd)
			wsData = L"F 1";

		std::wstring wsStartPoint;
		ReadAttribute(oReader, L"StartPoint", wsStartPoint);
		wsData += L" M " + wsStartPoint;

		std::wstring wsNodeName;
		std::wstring wsText;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			wsText.empty();
			if (L"PolyLineSegment" == wsNodeName)
			{
				ReadAttribute(oReader, L"Points", wsText);
				wsData += L" L " + wsText;
			}
			else if (L"PolyBezierSegment" == wsNodeName)
			{
				ReadAttribute(oReader, L"Points", wsText);
				wsData += L" C " + wsText;
			}
			else if (L"PolyQuadraticBezierSegment" == wsNodeName)
			{
				ReadAttribute(oReader, L"Points", wsText);
				wsData += L" Q " + wsText;
			}
			else if (L"ArcSegment" == wsNodeName)
			{
				std::wstring wsSize, wsRotationAngle, wsIsLargeArc, wsSweepDirection, wsPoint;
				if (oReader.MoveToFirstAttribute())
				{
					std::wstring wsAttrName = oReader.GetName();
					while (!wsAttrName.empty())
					{
						if (L"Size" == wsAttrName)
							wsSize = oReader.GetText();
						else if (L"RotationAngle" == wsAttrName)
							wsRotationAngle = oReader.GetText();
						else if (L"IsLargeArc" == wsAttrName)
							wsIsLargeArc = oReader.GetText();
						else if (L"SweepDirection" == wsAttrName)
							wsSweepDirection = oReader.GetText();
						else if (L"Point" == wsAttrName)
							wsPoint = oReader.GetText();

						if (!oReader.MoveToNextAttribute())
							break;

						wsAttrName = oReader.GetName();
					}
					oReader.MoveToElement();
				}

				wsData += L" A " + wsSize + L" " + wsRotationAngle + L" ";
				if (GetBool(wsIsLargeArc))
					wsData += L"1 ";
				else
					wsData += L"0 ";

				if (L"Counterclockwise" == wsSweepDirection)
					wsData += L"0 ";
				else
					wsData += L"1 ";

				wsData += wsPoint;
			}
		}

		std::wstring wsClosed;
		ReadAttribute(oReader, L"IsClosed", wsClosed);
		if (GetBool(wsClosed))
			wsData += L" Z ";

		_wsData.create(wsData.c_str(), true);
	}
	void ReadGradientStops(XmlUtils::CXmlLiteReader& oReader, std::vector<LONG>& vColors, std::vector<double>& vPositions, const double& dOpacity)
	{
		if (oReader.IsEmptyNode())
			return;

		CWString wsNodeName, wsAttrName;
		int nCurDepth = oReader.GetDepth();
		while (oReader.ReadNextSiblingNode(nCurDepth))
		{
			wsNodeName = oReader.GetName();
			if (wsNodeName == L"GradientStop")
			{				
				double dPos = 0;
				LONG lColor = 0;
				if (oReader.MoveToFirstAttribute())
				{
					wsAttrName = oReader.GetName();
					while (!wsAttrName.empty())
					{
						if (wsAttrName == L"Color")
						{
							int nBgr, nAlpha;
							ReadSTColor(oReader.GetText(), nBgr, nAlpha);
							nAlpha *= dOpacity;
							lColor = (nAlpha << 24 & 0xFF000000) | (nBgr & 0xFFFFFF);
						}
						else if (wsAttrName == L"Offset")
						{
							ReadSTDouble(oReader.GetText(), dPos);
						}

						if (!oReader.MoveToNextAttribute())
							break;

						wsAttrName = oReader.GetName();
					}

					oReader.MoveToElement();
				}
				vColors.push_back(lColor);
				vPositions.push_back(dPos);
			}
		}
	}

	void ReadSTPoint(const CWString& wsString, double& dX, double& dY)
	{
		int nCommaPos = 0;
		while (nCommaPos < wsString.size())
		{
			if (wsString[nCommaPos] == ',')
				break;

			nCommaPos++;
		}

		if (nCommaPos >= wsString.size())
		{
			CWString wsX = wsString.c_str();
			dX = GetDouble(wsX);
			dY = 0;
		}
		else
		{
			CWString wsX((wchar_t*)wsString.c_str(), false, nCommaPos);
			CWString wsY((wchar_t*)(wsString.c_str() + nCommaPos + 1), false, wsString.size() - nCommaPos - 1);
			dX = GetDouble(wsX);
			dY = GetDouble(wsY);
		}
	}
	void ReadSTColor(const CWString& wsString, int& nBgr, int& nAlpha)
	{
		int nLen = wsString.size();
		if (nLen <= 0)
			return;

		const wchar_t* pBuffer = wsString.c_str();
		if (L'#' == pBuffer[0])
		{
			nLen--;
			pBuffer++;

			if (6 != nLen && 8 != nLen)
				return;

			if (8 == nLen)
			{
				nAlpha = GetDigit(*pBuffer++);
				nAlpha <<= 4;
				nAlpha += GetDigit(*pBuffer++);
			}
			else
			{
				nAlpha = 255;
			}

			nBgr = GetDigit(pBuffer[4]);
			nBgr <<= 4;
			nBgr += GetDigit(pBuffer[5]);
			nBgr <<= 4;
			nBgr += GetDigit(pBuffer[2]);
			nBgr <<= 4;
			nBgr += GetDigit(pBuffer[3]);
			nBgr <<= 4;
			nBgr += GetDigit(pBuffer[0]);
			nBgr <<= 4;
			nBgr += GetDigit(pBuffer[1]);
		}
		else if (nLen >= 3 && L's' == pBuffer[0] && L'c' == pBuffer[1] && L'#' == pBuffer[2])
		{
			int nPos = 3;
			if (nPos >= nLen)
				return;

			CWString wsString2;
			wsString2.create(pBuffer + 3, false);

			std::vector<CWString> vElements = wsString2.split(',');
			if (3 == vElements.size())
			{
				nAlpha = 255;
				nBgr   = (((int)(min(GetDouble(vElements[2]), 1.0) * 255)) << 16) + (((int)(min(GetDouble(vElements[1]), 1.0) * 255)) << 8) + ((int)(min(GetDouble(vElements[0]), 1.0) * 255));
			}
			else if (4 == vElements.size())
			{
				nAlpha = (int)(min(GetDouble(vElements[0]), 1.0) * 255);
				nBgr   = (((int)(min(GetDouble(vElements[3]), 1.0) * 255)) << 16) + (((int)(min(GetDouble(vElements[2]), 1.0) * 255)) << 8) + ((int)(min(GetDouble(vElements[1]), 1.0) * 255));
			}
		}
	}
	void ReadSTColor(const CWString& wsString, LONG& lColor)
	{
		int nBgr, nAlpha;
		ReadSTColor(wsString, nBgr, nAlpha);
		lColor = (nAlpha << 24 & 0xFF000000) | (nBgr & 0xFFFFFF);
	}
	void ReadSTDouble(const CWString& wsString, double& dValue)
	{
		int nPos = 0;
		dValue = GetDouble(wsString.c_str(), nPos, wsString.size());
	}
}