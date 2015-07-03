#include "Utils.h"
#include "../../DesktopEditor/common/String.h"
#include "../../DesktopEditor/common/Types.h"
#include "../../Common/DocxFormat/Source/XML/xmlutils.h"

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
		NSString::ToLower(wsStr);
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
			std::vector<std::wstring> arrElements = NSString::Split(wsStr, L',');

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
	std::wstring NormalizePath(const std::wstring& wsPath)
	{		
		std::wstring wsResult = wsPath;
		NSString::Replace(wsResult, L"/", L"\\");
		while (std::wstring::npos != wsResult.find(L"\\\\"))
		{			
			NSString::Replace(wsResult, L"\\\\", L"\\");
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
		std::vector<std::wstring> arrStrings = NSString::Split(wsString, wDelim1);
		int nCount = arrStrings.size();
		for (int nIndex = 0; nIndex < nCount; nIndex++)
		{
			std::vector<std::wstring> arrStr = NSString::Split(arrStrings[nIndex], wDelim2);
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
}