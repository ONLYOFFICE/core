#include "StaticFunctions.h"

namespace NSCSS
{
namespace NS_STATIC_FUNCTIONS
{
	std::string GetContentAsUTF8(const std::string &sString, const std::wstring &sEncoding)
	{
		const std::string& sEnc = U_TO_UTF8(sEncoding);

		NSUnicodeConverter::CUnicodeConverter oConverter;
		const std::wstring& sUnicodeContent = oConverter.toUnicode(sString, sEnc.c_str());
		return U_TO_UTF8(oConverter.toUnicode(sString, sEnc.c_str()));
	}

	std::string GetContentAsUTF8(const std::wstring& sFileName)
	{
		std::string sContent;
		// читаем файл как есть. utf-8 тут просто название.
		if(!NSFile::CFileBinary::ReadAllTextUtf8A(sFileName, sContent))
			return sContent;

		std::string sEncoding;
		if (true)
		{
			// определяем кодировку
			const std::string::size_type& posCharset = sContent.find("@charset");

			if (std::string::npos != posCharset)
			{
				std::string::size_type pos1 = sContent.find_first_of("\"';", posCharset);
				if (std::string::npos != pos1)
				{
					pos1 += 1;
					std::string::size_type pos2 = sContent.find_first_of("\"';", pos1);
					if (std::string::npos != pos2)
					{
						sEncoding = sContent.substr(pos1, pos2 - pos1);
					}

					// check valid
					if (std::string::npos != sEncoding.find_first_of(" \n\r\t\f\v"))
						sEncoding = "";
				}
			}
		}

		if (sEncoding.empty())
			sEncoding = "utf-8";

		if (!sEncoding.empty() && sEncoding != "utf-8" && sEncoding != "UTF-8")
		{
			NSUnicodeConverter::CUnicodeConverter oConverter;
			sContent = U_TO_UTF8(oConverter.toUnicode(sContent, sEncoding.c_str()));
		}

		return sContent;
	}

	double ReadDouble(const std::wstring& wsValue)
	{
		double dValue;
		std::wistringstream(wsValue) >> dValue;
		return dValue;
	}

	std::vector<double> ReadDoubleValues(const std::wstring& wsValue)
	{
		std::vector<double> arValues;

		std::wregex oPattern(LR"([-+]?(\d+(\.\d*)?|\.\d+)([eE][-+]?\d+)?)");

		std::wsregex_iterator oIter(wsValue.begin(), wsValue.end(), oPattern);
		std::wsregex_iterator oEndIter;

		for (; oIter != oEndIter; ++oIter)
			arValues.push_back(std::stod(oIter->str()));

		return arValues;
	}

	std::vector<std::wstring> ParseCSSPropertie(const std::wstring& wsInput)
	{
		std::vector<std::wstring> arResult;
		std::wstring wsCurrent;
		bool bInQuotes = false;
		bool bInFunction = false;
		int nParenDepth = 0;

		for (wchar_t c : wsInput)
		{
			if (c == ' ' && !bInQuotes && !bInFunction)
			{
				if (!wsCurrent.empty())
				{
					arResult.push_back(wsCurrent);
					wsCurrent.clear();
				}
			}
			else if (c == '"' || c == '\'')
			{
				bInQuotes = !bInQuotes;
				wsCurrent += c;
			}
			else if (c == '(')
			{
				bInFunction = true;
				nParenDepth++;
				wsCurrent += c;
			}
			else if (c == ')')
			{
				nParenDepth--;
				if (nParenDepth == 0)
					bInFunction = false;
				wsCurrent += c;
			} 
			else 
				wsCurrent += c;
		}

		if (!wsCurrent.empty())
			arResult.push_back(wsCurrent);

		return arResult;
	}

	std::vector<std::wstring> GetWordsW(const std::wstring& wsLine, bool bWithSigns, const std::wstring& wsDelimiters)
	{
		if (wsLine.empty())
			return {};

		size_t unEnd = wsLine.find_first_of(wsDelimiters);

		if (std::wstring::npos == unEnd)
			return {wsLine};

		size_t unStart = 0;
		std::vector<std::wstring> arWords;

		while (std::wstring::npos != unEnd)
		{
			if (unStart != unEnd)
				arWords.emplace_back(wsLine.data() + unStart, unEnd - unStart + ((bWithSigns) ? 1 : 0));

			unStart = wsLine.find_first_not_of(wsDelimiters, unEnd);
			unEnd = wsLine.find_first_of(wsDelimiters, unStart);
		}

		if (std::wstring::npos != unStart)
			arWords.emplace_back(wsLine.data() + unStart);

		return arWords;
	}

	std::vector<unsigned short int> GetWeightSelector(const std::wstring& wsSelector)
	{
		if (wsSelector.empty())
			return std::vector<unsigned short int>{0, 0, 0, 0};

		std::vector<unsigned short int> arWeight = {0, 0, 0, 0};

		std::wstring sReverseSelector = wsSelector;
		std::reverse(sReverseSelector.begin(), sReverseSelector.end());

		const std::vector<std::wstring> arSelectors = NS_STATIC_FUNCTIONS::GetWordsW(sReverseSelector, true, L" .#:");

		for (const std::wstring& sSel : arSelectors)
		{
			if (sSel == L"*")
				++arWeight[3];
			else if (sSel.rfind(L'#') != std::wstring::npos)
				++arWeight[0];
			else if (sSel.rfind(L':') != std::wstring::npos)
			{
				std::wstring sTemp(sSel);
				sTemp.erase(std::remove_if(sTemp.begin(), sTemp.end(), [] (const wchar_t& wc) { return !std::iswalpha(wc);}));
//                    std::find(NSConstValues::arPseudoClasses.begin(), NSConstValues::arPseudoClasses.end(), sTemp) != NSConstValues::arPseudoClasses.end() ? ++arWeight[1] : ++arWeight[2];
			}
			else if (sSel.find_last_of(L".[]") != std::wstring::npos)
				++arWeight[1];
			else
				++arWeight[2];
		}

		return arWeight;
	}

	std::map<std::wstring, std::wstring> GetRules(const std::wstring& wsStyles)
	{
		std::wregex oCssPropertyRegex(L"([a-zA-Z-]+)\\s*:\\s*([^;\t\n\r\f\v]+)");
		std::wsmatch oMatch;

		std::wstring::const_iterator oSearchStart(wsStyles.cbegin());

		std::map<std::wstring, std::wstring> mRules;

		while (std::regex_search(oSearchStart, wsStyles.cend(), oMatch, oCssPropertyRegex))
		{
			mRules.insert(std::make_pair<std::wstring, std::wstring>(oMatch[1], oMatch[2]));
			oSearchStart = oMatch.suffix().first;
		}

		return mRules;
	}

	void RemoveSpaces(std::wstring &wsString)
	{
		std::wstring::const_iterator ciStart = std::find_if_not(wsString.begin(), wsString.end(), std::iswspace);

		if (ciStart == wsString.end())
			return wsString.clear();

		std::wstring::const_reverse_iterator criEnd = std::find_if_not(wsString.rbegin(),wsString.rend(), std::iswspace);

		wsString = std::wstring(ciStart, criEnd.base());
	}
	
	double CalculatePersentage(const std::wstring &wsValue, double dRelativeValue)
	{
		double dValue = ReadDouble(wsValue);

		if (std::wstring::npos != wsValue.find(L'%'))
			return dValue / 100. * dRelativeValue;

		return dValue;
	}
	
}
}
