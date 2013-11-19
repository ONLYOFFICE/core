#include "stdafx.h"
#include "Utils.h"

namespace XPS
{
	static const CString vml = _T("FMLHVCQSAZfmlhvcqsaz");

	int GetDigit(TCHAR c)
	{
		if (c >= '0' && c <= '9')
			return (int)(c - '0');
		if (c >= 'a' && c <= 'f')
			return 10 + (int)(c - 'a');
		if (c >= 'A' && c <= 'F')
			return 10 + (int)(c - 'A');

		return 0;
	}

	bool IsAlpha(TCHAR sym)
	{
		return (((sym >= 'A') && (sym <= 'Z')) || ((sym >= 'a') && (sym <= 'z')));
	}

	double GetDouble(const CString& string)
	{
		double d = 0;
		_stscanf(string, _T("%lf"), &d);
		return d;
	}

	int GetInteger(const CString& string)
	{
		int i = 0;
		_stscanf(string, _T("%d"), &i);
		return i;
	}

	int GetIntegerFromHex(const CString& string)
	{
		int i = 0;
		_stscanf(string, _T("%x"), &i);
		return i;
	}

	bool GetBool(const CString& string)
	{
		CString str = string;
		str.MakeLower();
		if((str == _T("true")) || (str == _T("t")) || (str == _T("1")) || (str == _T("on")))
			return true;
		return false;
	}

	void ARGB2BGRA(const CString& source, int& bgr, int& alpha)
	{
		if(source[0] == _T('#'))
		{
			CString str = source.Mid(1);
			while(str.GetLength() < 6)
				str = _T("0") + str;
			while(str.GetLength() < 8)
				str = _T("F") + str;

			alpha = GetDigit(str[0]);
			alpha = alpha << 4;
			alpha += GetDigit(str[1]);

			bgr = GetDigit(str[6]);
			bgr = bgr << 4;
			bgr += GetDigit(str[7]);
			bgr = bgr << 4;
			bgr += GetDigit(str[4]);
			bgr = bgr << 4;
			bgr += GetDigit(str[5]);
			bgr = bgr << 4;
			bgr += GetDigit(str[2]);
			bgr = bgr << 4;
			bgr += GetDigit(str[3]);
		}
		else if((source[0] == _T('s')) && (source[1] == _T('c')) && (source[2] == _T('#')))
		{
			CString str = source.Mid(3);
			std::vector<CString> dest;
			DelimString(str, dest, _T(','));

			if(dest.size() == 3)
			{
				alpha = 255;
				bgr = (((int)(min(GetDouble(dest[2]), 1.0) * 255)) << 16)
					+ (((int)(min(GetDouble(dest[1]), 1.0) * 255)) << 8) + ((int)(min(GetDouble(dest[0]), 1.0) * 255));
			}
			else if(dest.size() == 4)
			{
				alpha = GetDouble(dest[0]) * 255;
				bgr = (((int)(min(GetDouble(dest[3]), 1.0) * 255)) << 16)
					+ (((int)(min(GetDouble(dest[2]), 1.0) * 255)) << 8) + ((int)(min(GetDouble(dest[1]), 1.0) * 255));
			}
		}
		else
			return;
	}

	void DelimString(const CString& source, std::vector<CString>& dest, TCHAR delim)
	{
		CString str = source;
		CString temp;
		int pos = str.Find(delim);
		while(pos != -1)
		{
			temp = str.Mid(0, pos);
			str.Delete(0, pos + 1);
			if(temp != _T(""))
				dest.push_back(temp);
			pos = str.Find(delim);
		}
		if(str != _T(""))
			dest.push_back(str);
	}

	void DelimString(const CString& source, std::vector<CString>& dest, const CString& delim)
	{
		CString str = source;
		CString temp;
		int pos = str.FindOneOf(delim);
		while(pos != -1)
		{
			temp = str.Mid(0, pos);
			str.Delete(0 ,pos + 1);
			if(temp != _T(""))
				dest.push_back(temp);
			pos = str.FindOneOf(delim);
		}
		if(str != _T(""))
			dest.push_back(str);
	}

	void PrepareVML(CString& str)
	{
		CString result;
		int pos = str.FindOneOf(vml);
		while(pos != -1)
		{
			result += str.Mid(0, pos);
			result += _T(" ");
			result += str.Mid(pos, 1);
			result += _T(" ");
			str.Delete(0, pos + 1);
			pos = str.FindOneOf(vml);
		}
		result += str;
		str = result;
	}

	void GetFontKey(const CString& str, unsigned char* keys)
	{
		int k = 0;
		for(int i = str.GetLength() - 1; i >= 0; i--)
		{
			if(str[i] != '-')
			{
				keys[k] = (unsigned char)GetIntegerFromHex(str.Mid(i - 1, 2));
				i--;
				k++;
			}
		}
	}

	CString GetPath(const CString& path)
	{
		CString result;
		result = path.Left(path.ReverseFind(_T('/')) + 1);
		return result;
	}

	CString GetExtension(const CString& path)
	{
		CString result;
		result = path.Right(path.GetLength() - path.ReverseFind(_T('.')) - 1);
		return result;
	}

	CString RemoveNamespace(const CString& str)
	{
		CString result;
		result = str.Right(str.GetLength() - str.Find(_T(':')) - 1);
		return result;
	}

	void DelimStringWithEmptys(const CString& source, std::vector<CString>& dest, TCHAR delim)
	{
		CString str = source;
		CString temp;
		int pos = str.Find(delim);
		while(pos != -1)
		{
			temp = str.Mid(0, pos);
			str.Delete(0, pos + 1);
			dest.push_back(temp);
			pos = str.Find(delim);
		}
		dest.push_back(str);
	}

	void DelimString(const CString& source, std::vector<std::vector<CString> >& dest, TCHAR delim1, TCHAR delim2)
	{
		CString str = source;
		CString temp;
		int pos = str.Find(delim1);
		int count = 0;
		while(pos >= 0)
		{
			temp = str.Mid(0, pos);
			str.Delete(0, pos + 1);
			dest.push_back(std::vector<CString>());
			DelimStringWithEmptys(temp, dest.at(count), delim2);
			count++;
			pos = str.Find(delim1);
		}
		dest.push_back(std::vector<CString>());
		DelimStringWithEmptys(str, dest.at(count), delim2);
	}

}// namespace XPS