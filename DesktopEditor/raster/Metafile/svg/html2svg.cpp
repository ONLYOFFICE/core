#include "html2svg.h"

#include <vector>
#include <cwctype>
#include <cwchar>
#include <cmath>
#include <cstddef>

namespace SVG
{
struct Style
{
	double       dFontSize    = 16.0;
	double       dLineHeight  = 1.2;
	std::wstring wsColor      = L"#000";
	std::wstring wsFontFamily = L"Arial";
	std::wstring wsFontWeight = L"normal";
	std::wstring wsFontStyle  = L"normal";
	wchar_t      wchAlign     = L'l';   // l|c|r

	std::wstring wsClass;
	std::wstring wsStyle;
};

inline wchar_t ToLower(wchar_t c) { return (wchar_t)std::towlower((std::wint_t)c); }

void AppendCp(unsigned cp, std::wstring& wsOut)
{
	if (sizeof(wchar_t) >= 4 || cp <= 0xFFFF)
		wsOut += (wchar_t)cp;

	else
	{
		cp -= 0x10000;
		wsOut += (wchar_t)(0xD800 + (cp >> 10));
		wsOut += (wchar_t)(0xDC00 + (cp & 0x3FF));
	}
}

std::wstring DecodeEntities(const std::wstring& wsValue)
{
	std::wstring wsOut;
	wsOut.reserve(wsValue.size());

	for (std::size_t i = 0; i < wsValue.size();)
	{
		if (wsValue[i] != L'&')
		{
			wsOut += wsValue[i++];
			continue;
		}

		std::size_t unSemi{wsValue.find(L';', i + 1)};

		if (std::wstring::npos == unSemi || unSemi - i > 12)
		{
			wsOut += wsValue[i++];
			continue;
		}

		std::wstring wsEntities{wsValue.substr(i + 1, unSemi - i - 1)};
		bool bResult{true};

		if (!wsEntities.empty() && L'#' == wsEntities[0])
		{
			unsigned cp = 0;
			if (wsEntities.size() > 1 && (L'x' == wsEntities[1] || L'X' == wsEntities[1]))
			{
				for (std::size_t k = 2; k < wsEntities.size(); ++k)
				{
					const wchar_t c = ToLower(wsEntities[k]);
					cp = cp*16 + (unsigned)((c <= L'9') ? (c - L'0') : (c - L'a' + 10));
				}
			}
			else
			{
				for (std::size_t k = 1; k < wsEntities.size(); ++k)
					cp = cp * 10 + (unsigned)(wsEntities[k] - L'0');
			}

			AppendCp(cp ? cp : L'?', wsOut);
		}
		else if (wsEntities == L"amp")
			wsOut += L'&';
		else if (wsEntities == L"lt")
			wsOut += L'<';
		else if (wsEntities == L"gt")
			wsOut += L'>';
		else if (wsEntities == L"quot")
			wsOut += L'"';
		else if (wsEntities == L"apos")
			wsOut += L'\'';
		else if (wsEntities == L"nbsp")
			wsOut += L' ';
		else
			bResult = false;

		if (bResult)
			i = unSemi + 1;
		else
			wsOut += wsValue[i++];
	}

	return wsOut;
}

void XMLEscape(const std::wstring& wsIn, std::wstring& wsOut)
{
	for (std::size_t i = 0; i < wsIn.size(); ++i)
	{
		switch (wsIn[i])
		{
			case L'&': wsOut += L"&amp;";  break;
			case L'<': wsOut += L"&lt;";   break;
			case L'>': wsOut += L"&gt;";   break;
			case L'"': wsOut += L"&quot;"; break;
			default:   wsOut += wsIn[i];     break;
		}
	}
}

double EstimateWidth(const std::wstring& wsValue, const double& dFontSize)
{
	double dUnits = 0.0;
	for (std::size_t i = 0; i < wsValue.size(); ++i)
	{
		const wchar_t c = wsValue[i];
		if (c == L' ')                                  dUnits += 0.2;  // Space
		else if (std::wcschr(L"iIl1.,;:'!|jtf()[]", c)) dUnits += 0.3;  // Narrow
		else if (std::wcschr(L"mMwW@", c))              dUnits += 0.8;  // Wide
		else if (c >= 0x4E00)                           dUnits += 0.9;  // CJK ~ Square
		else                                            dUnits += 0.45; // Medium
	}
	return dUnits * dFontSize;
}

std::wstring CSSProp(const std::wstring& wsCSS, const wchar_t* pKey)
{
	std::size_t unKeyLen{std::wcslen(pKey)}, unPos{0};

	while ((unPos = wsCSS.find(pKey, unPos)) != std::wstring::npos)
	{
		const bool bPermission{(unPos == 0) || wsCSS[unPos-1] == L';' || wsCSS[unPos-1] == L' '};
		std::size_t unFound{wsCSS.find(L':', unPos + unKeyLen)};

		if (bPermission && unFound != std::wstring::npos)
		{
			const std::size_t unEntities{wsCSS.find(L';', unFound)};
			std::size_t unStart{unFound + 1}, unEnd{(unEntities == std::wstring::npos ? wsCSS.size() : unEntities)};

			while (unStart < unEnd && wsCSS[unStart]   == L' ')
				++unStart;
			while (unEnd > unStart && wsCSS[unEnd - 1] == L' ')
				--unEnd;

			return wsCSS.substr(unStart, unEnd - unStart);
		}
		unPos += unKeyLen;
	}

	return std::wstring();
}

//TODO
double ParsePx(const std::wstring& wsValue, double dDefault)
{
	if (wsValue.empty())
		return dDefault;

	try
	{
		return std::stod(wsValue);
	}
	catch (...)
	{
		return dDefault;
	}
}

//TODO
bool Equals(const std::wstring& wsValue, const wchar_t* pValue)
{
	std::size_t unLen{std::wcslen(pValue)};

	if (wsValue.size() != unLen)
		return false;

	for (std::size_t k = 0; k < unLen; ++k)
		if (ToLower(wsValue[k]) != ToLower(pValue[k]))
			return false;

	return true;
}

std::wstring GetAttr(const std::wstring& wsTag, const wchar_t* pName)
{
	std::size_t unLen{std::wcslen(pName)}, unPos = 0;

	while ((unPos = wsTag.find(pName, unPos)) != std::wstring::npos)
	{
		const bool bPermission{(unPos == 0) || wsTag[unPos - 1] == L' '};
		std::size_t unEnd{unPos + unLen};

		while (unEnd < wsTag.size() && wsTag[unEnd] == L' ')
			++unEnd;

		if (bPermission && unEnd < wsTag.size() && wsTag[unEnd] == L'=')
		{
			++unEnd;

			while (unEnd < wsTag.size() && wsTag[unEnd] == L' ')
				++unEnd;

			if (unEnd < wsTag.size() && (wsTag[unEnd] == L'"' || wsTag[unEnd] == L'\''))
			{
				wchar_t quote = wsTag[unEnd];
				std::size_t b = unEnd + 1, e = wsTag.find(quote, b);

				if (e != std::wstring::npos)
					return wsTag.substr(b, e - b);
			}
		}
		unPos += unLen;
	}

	return std::wstring();
}

std::wstring TagName(const std::wstring& wsTag)
{
	std::size_t b = (!wsTag.empty() && wsTag[0] == L'/') ? 1 : 0, n = b;

	while (n < wsTag.size() && (std::iswalnum(wsTag[n]) || wsTag[n] == L'-'))
		++n;

	return wsTag.substr(b, n - b);
}

bool IsClosingTag(const std::wstring& wsTag)
{
	return !wsTag.empty() && wsTag[0] == L'/';
}

bool isBlock(const std::wstring& n)
{
	return Equals(n, L"div") || Equals(n, L"p")  || Equals(n, L"li") || Equals(n, L"section")||
	       Equals(n, L"h1")  || Equals(n, L"h2") || Equals(n, L"h3") || Equals(n, L"h4")||
	       Equals(n, L"ul")  || Equals(n, L"ol") || Equals(n, L"tr") || Equals(n, L"table");
}

std::wstring FmtNum(double dValue)
{
	wchar_t b[32];

	if (dValue == std::floor(dValue))
		std::swprintf(b, 32, L"%lld", (long long)dValue);
	else
		std::swprintf(b, 32, L"%.2f", dValue);

	std::wstring wsResult(b);

	const std::size_t unComma = wsResult.find(L',');
	if (unComma != std::wstring::npos)
		wsResult[unComma] = L'.';

	return wsResult;
}
class Converter
{
public:
	std::wstring Convert(const std::wstring& wsHTML, const double& dContentWidth, const std::wstring& wsRootAttrs)
	{
		dWidth = dContentWidth;
		wsOut.clear();

		wsOut.reserve(wsHTML.size() * 2);

		wsOut += L"<g";

		if (!wsRootAttrs.empty())
		{
			wsOut += L' ';
			wsOut += wsRootAttrs;
		}

		wsOut += L'>';

		std::vector<Style> arStackStyles;
		arStackStyles.push_back(Style());

		dY = arStackStyles.back().dFontSize * arStackStyles.back().dLineHeight / 4.;

		std::size_t unIndex = 0, unHTMLSize = wsHTML.size();

		while (unIndex < unHTMLSize)
		{
			if (wsHTML[unIndex] != L'<')
			{
				const std::size_t unNext = wsHTML.find(L'<', unIndex);
				const std::size_t unEnd = (unNext == std::wstring::npos) ? unHTMLSize : unNext;

				EmitText(wsHTML.substr(unIndex, unEnd - unIndex), arStackStyles.back());
				unIndex = unEnd;

				continue;
			}

			if (wsHTML.compare(unIndex, 4, L"<!--") == 0)
			{
				const std::size_t unEnd{wsHTML.find(L"-->", unIndex + 4)};
				unIndex = (unEnd == std::wstring::npos ? unHTMLSize : unEnd + 3);
				continue;
			}

			if (wsHTML.compare(unIndex, 9, L"<![CDATA[") == 0)
			{
				const std::size_t unEnd{wsHTML.find(L"]]>", unIndex + 9)};
				unIndex = (unEnd == std::wstring::npos ? unHTMLSize : unEnd + 3);
				continue;
			}

			if (unIndex + 1 < unHTMLSize && wsHTML[unIndex + 1] == L'!')
			{
				const std::size_t unEnd{wsHTML.find(L'>', unIndex)};
				unIndex = (unEnd == std::wstring::npos ? unHTMLSize : unEnd + 1);
				continue;
			}

			if (unIndex + 1 < unHTMLSize && wsHTML[unIndex + 1] == L'?')
			{
				const std::size_t unEnd{wsHTML.find(L'>', unIndex)};
				unIndex = (unEnd == std::wstring::npos ? unHTMLSize : unEnd + 1);
				continue;
			}

			const std::size_t unTagEnd{FindTagEnd(wsHTML, unIndex)};

			if (std::wstring::npos == unTagEnd)
				break;

			const std::wstring wsTag{wsHTML.substr(unIndex + 1, unTagEnd - unIndex - 1)};
			const std::wstring wsName{TagName(wsTag)};

			if (!IsClosingTag(wsTag) && (Equals(wsName, L"script") || Equals(wsName, L"style")))
			{
				unIndex = SkipElement(wsHTML, unTagEnd + 1, wsName);
				continue;
			}
			HandleTag(wsTag, wsName, arStackStyles);
			unIndex = unTagEnd + 1;
		}
		wsOut += L"</g>";
		return wsOut;
	}

private:
	std::wstring wsOut;
	double dWidth= 0, dY = 0;

	static std::size_t FindTagEnd(const std::wstring& wsValue, std::size_t unIndex)
	{
		wchar_t q = 0;

		for (std::size_t k = unIndex + 1; k < wsValue.size(); ++k)
		{
			const wchar_t c = wsValue[k];
			if (q)
			{
				if (c == q)
					q = 0;
			}
			else if (c == L'"' || c == L'\'')
				q = c;
			else if (c == L'>')
				return k;
		}

		return std::wstring::npos;
	}

	static std::size_t SkipElement(const std::wstring& wsValue, std::size_t unFrom, const std::wstring& wsName)
	{
		for (std::size_t unIndex = unFrom; unIndex + 1 < wsValue.size(); ++unIndex)
		{
			if (wsValue[unIndex] == L'<' && wsValue[unIndex+1] == L'/' && unIndex + 2 + wsName.size() <= wsValue.size())
			{
				bool match = true;
				for (std::size_t j = 0; j < wsName.size(); ++j)
					if (ToLower(wsValue[unIndex+2+j]) != ToLower(wsName[j]))
					{
						match = false;
						break;
					}
				if (match)
				{
					std::size_t e = wsValue.find(L'>', unIndex);
					return e == std::wstring::npos ? wsValue.size() : e + 1;
				}
			}
		}
		return wsValue.size();
	}

	void HandleTag(const std::wstring& wsTag, const std::wstring& wsName, std::vector<Style>& arStackStyles)
	{
		if (IsClosingTag(wsTag))
		{
			if (isBlock(wsName))
				dY += arStackStyles.back().dLineHeight * arStackStyles.back().dFontSize;

			if (arStackStyles.size() > 1)
				arStackStyles.pop_back();

			return;
		}
		if (Equals(wsName, L"br"))
		{
			dY += arStackStyles.back().dLineHeight * arStackStyles.back().dFontSize;
			return;
		}

		bool selfClose = !wsTag.empty() && wsTag[wsTag.size() - 1] == L'/';

		Style st = arStackStyles.back();
		st.wsClass.clear();
		st.wsStyle.clear();

		ApplyTagStyle(wsName, st);
		st.wsClass = GetAttr(wsTag, L"class");
		st.wsStyle = GetAttr(wsTag, L"style");
		ApplyCSS(st.wsStyle, st);

		if (!selfClose)
			arStackStyles.push_back(st);
	}

	static void ApplyTagStyle(const std::wstring& wsStyle, Style& oStyle)
	{
		if (Equals(wsStyle, L"b") || Equals(wsStyle, L"strong"))
			oStyle.wsFontWeight = L"bold";
		else if (Equals(wsStyle, L"i") || Equals(wsStyle, L"em"))
			oStyle.wsFontStyle = L"italic";
		else if (Equals(wsStyle, L"h1"))
		{
			oStyle.dFontSize *= 2.0;
			oStyle.wsFontWeight = L"bold";
		}
		else if (Equals(wsStyle, L"h2"))
		{
			oStyle.dFontSize *= 1.5;
			oStyle.wsFontWeight = L"bold";
		}
		else if (Equals(wsStyle, L"h3"))
		{
			oStyle.dFontSize *= 1.17;
			oStyle.wsFontWeight = L"bold";
		}
	}

	static double ParseLineHeight(const std::wstring& wsValue, double dCurrent, double dFontSize)
	{
		if (wsValue.empty() || Equals(wsValue, L"normal"))
			return dCurrent;

		try
		{
			std::size_t unIdx = 0;
			const double dNum = std::stod(wsValue, &unIdx);

			std::wstring wsUnit = wsValue.substr(unIdx);
			while (!wsUnit.empty() && wsUnit[0] == L' ')
				wsUnit.erase(0, 1);

			if (!wsUnit.empty() && wsUnit[0] == L'%')
				return dNum / 100.0;
			if (wsUnit.size() >= 2 && wsUnit[0] == L'p' && wsUnit[1] == L'x')
				return (dFontSize > 0) ? dNum / dFontSize : dCurrent;

			return dNum;
		}
		catch (...)
		{
			return dCurrent;
		}
	}

	static void ApplyCSS(const std::wstring& wsCSS, Style& oStyle)
	{
		if (wsCSS.empty())
			return;

		std::wstring wsPropertie;

		if (!(wsPropertie = CSSProp(wsCSS, L"font-size")).empty())
			oStyle.dFontSize = ParsePx(wsPropertie, oStyle.dFontSize);
		if (!(wsPropertie = CSSProp(wsCSS, L"color")).empty())
			oStyle.wsColor = wsPropertie;
		if (!(wsPropertie = CSSProp(wsCSS, L"font-weight")).empty())
			oStyle.wsFontWeight = wsPropertie;
		if (!(wsPropertie = CSSProp(wsCSS, L"font-style")).empty())
			oStyle.wsFontStyle  = wsPropertie;
		if (!(wsPropertie = CSSProp(wsCSS, L"font-family")).empty())
			oStyle.wsFontFamily = wsPropertie;
		if (!(wsPropertie = CSSProp(wsCSS, L"line-height")).empty())
			 oStyle.dLineHeight = ParseLineHeight(wsPropertie, oStyle.dLineHeight, oStyle.dFontSize);
		if (!(wsPropertie = CSSProp(wsCSS, L"text-align")).empty())
			oStyle.wchAlign = (wsPropertie == L"center" ? L'c' : wsPropertie == L"right" ? L'r' : L'l');
	}

	void EmitLineAtTop(const std::wstring& wsLine, const Style& oStyle, double dTop)
	{
		if (wsLine.empty())
			return;

		double dX = 0; wchar_t wchAnchor = L'l';

		if (oStyle.wchAlign == L'c')
		{
			dX = dWidth / 2;
			wchAnchor = L'm';
		}
		else if (oStyle.wchAlign == L'r')
		{
			dX = dWidth;
			wchAnchor = L'e';
		}

		const double dHalfLeading = (oStyle.dLineHeight - 1.0) * oStyle.dFontSize / 2.0;
		const double dAscent      = oStyle.dFontSize * 0.8;
		const double dBaseline    = dTop + dHalfLeading + dAscent;

		EmitLine(wsLine, oStyle, dX, dBaseline, wchAnchor);
	}

	void EmitText(const std::wstring& wsText, const Style& oStyle)
	{
		std::wstring wsDecoded{DecodeEntities(wsText)};
		std::wstring wsNorm; wsNorm.reserve(wsDecoded.size());
		bool bPendingWs{false};

		for (std::size_t k = 0; k < wsDecoded.size(); ++k)
		{
			const wchar_t c = wsDecoded[k];
			const bool ws = (c == L' ' || c == L'\t' || c == L'\n' || c == L'\r');
			if (ws)
			{
				if (!wsNorm.empty())
					bPendingWs = true;
			}
			else
			{
				if (bPendingWs)
				{
					wsNorm += L' ';
					bPendingWs = false;
				}
				wsNorm += c;
			}
		}

		if (wsNorm.empty())
			return;

		const double dMaxWidth = (dWidth > 0) ? dWidth : 1e9;

		std::vector<std::wstring> arLines;
		std::wstring wsLine;
		std::size_t unPos{0};

		while (unPos < wsNorm.size())
		{
			const std::size_t unSpace = wsNorm.find(L' ', unPos);
			const std::wstring wsWord  = wsNorm.substr(unPos, (unSpace == std::wstring::npos ? wsNorm.size() : unSpace) - unPos);
			const std::wstring wsCand  = wsLine.empty() ? wsWord : wsLine + L" " + wsWord;

			if (EstimateWidth(wsCand, oStyle.dFontSize) > dMaxWidth && !wsLine.empty())
			{
				arLines.push_back(wsLine);
				wsLine = wsWord;
			}
			else
				wsLine = wsCand;

			unPos = (unSpace == std::wstring::npos) ? wsNorm.size() : unSpace + 1;
		}

		if (!wsLine.empty())
			arLines.push_back(wsLine);

		if (arLines.empty())
			return;

		const double dStep = oStyle.dLineHeight * oStyle.dFontSize;

		for (std::size_t i = 0; i < arLines.size(); ++i)
			EmitLineAtTop(arLines[i], oStyle, dY + (double)i * dStep);

		dY += (double)(arLines.size() - 1) * dStep;
	}

	void FlushLine(std::wstring& wsLine, const Style& oStyle)
	{
		if (wsLine.empty())
			return;

		double dX = 0; wchar_t wchAnchor = L'l';

		if (oStyle.wchAlign == L'c')
		{
			dX = dWidth / 2;
			wchAnchor = L'm';
		}
		else if (oStyle.wchAlign == L'r')
		{
			dX = dWidth;
			wchAnchor = L'e';
		}

		const double dHalfLeading = (oStyle.dLineHeight - 1.0) * oStyle.dFontSize / 2.0;
		const double dAscent      = oStyle.dFontSize * 0.8;
		const double dBaseline    = dY + dHalfLeading + dAscent;

		EmitLine(wsLine, oStyle, dX, dBaseline, wchAnchor);

		wsLine.clear();
	}

	void EmitLine(const std::wstring& wsLine, const Style& oStyle, const double& dX, const double& dY, wchar_t wchAnchor)
	{
		std::wstring wsStyle{oStyle.wsStyle};

		while (!wsStyle.empty() && (wsStyle[wsStyle.size() - 1] == L' ' || wsStyle[wsStyle.size() - 1] == L';'))
			wsStyle.erase(wsStyle.size() - 1);

		const std::wstring& wsSrc{oStyle.wsStyle};

		auto Add = [&](const wchar_t* k, const std::wstring& wsVal)
		{
			if (!wsStyle.empty())
				wsStyle += L';';
			wsStyle += k;
			wsStyle += L':';
			wsStyle += wsVal;
		};

		if (CSSProp(wsSrc, L"font-family").empty())
			Add(L"font-family", oStyle.wsFontFamily);
		if (CSSProp(wsSrc, L"font-size").empty())
			Add(L"font-size", FmtNum(oStyle.dFontSize) + L"px");
		if (CSSProp(wsSrc, L"fill").empty())
			Add(L"fill", oStyle.wsColor);
		if (oStyle.wsFontWeight != L"normal" && CSSProp(wsSrc, L"font-weight").empty())
			Add(L"font-weight", oStyle.wsFontWeight);
		if (oStyle.wsFontStyle  != L"normal" && CSSProp(wsSrc, L"font-style").empty())
			Add(L"font-style",  oStyle.wsFontStyle);
		if (wchAnchor != L'l' && CSSProp(wsSrc, L"text-anchor").empty())
			Add(L"text-anchor", wchAnchor == L'm' ? L"middle" : L"end");

		wsOut += L"<text x=\"";
		wsOut += FmtNum(dX);
		wsOut += L"\" y=\"";
		wsOut += FmtNum(dY);

		if (!oStyle.wsClass.empty())
		{
			wsOut += L"\" class=\"";
			XMLEscape(oStyle.wsClass, wsOut);
		}

		wsOut += L"\" style=\"";
		XMLEscape(wsStyle, wsOut);
		wsOut += L"\">";
		XMLEscape(wsLine, wsOut);
		wsOut += L"</text>";
	}
};

std::wstring ConvertForeignObjectToSvg(const std::wstring& wsHTML, const double& dWidth, const std::wstring& wsRootAttrs)
{
	return Converter().Convert(wsHTML, dWidth, wsRootAttrs);
}
}

