#include "shakey.h"
#include "../../../../../DesktopEditor/common/File.h"
#include "../../../../../Common/3dParty/cryptopp/sha.h"
#include "../../../../../Common/3dParty/cryptopp/hex.h"
#include "../../../../../Common/3dParty/cryptopp/base64.h"
#include "../../../../../Common/3dParty/cryptopp/filters.h"
#include "../../../../../Common/3dParty/cryptopp/files.h"

namespace HashSM
{
std::wstring HashingAnnotation(std::wstring &wsAnnotation, const bool& bUnicodeConversionString)
	{
		RemovingSpaces(wsAnnotation);
		std::wstring wsTempAnnotation = L"";
		if(bUnicodeConversionString)
		{
			wsTempAnnotation = wsAnnotation;
			wsAnnotation.clear();
			for(wchar_t wcOneSymbol:wsTempAnnotation)
			{
				if(wcOneSymbol == L'"')
					wsAnnotation += L"&quot;";
				else
					wsAnnotation += wcOneSymbol;
			}
		}
		std::string sAnnotation = U_TO_UTF8(wsAnnotation),sResult;
		std::wstring wsResult;
		CryptoPP::SHA256 hash;
		CryptoPP::StringSource oStringSourse(sAnnotation, true, new CryptoPP::HashFilter(hash,new CryptoPP::HexEncoder(new CryptoPP::StringSink(sResult))));
		wsResult = UTF8_TO_U(sResult);
		if( bUnicodeConversionString && !wsTempAnnotation.empty())
			wsAnnotation = wsTempAnnotation;
		return wsResult;
	}

	bool HashComparison(const std::wstring& wsHashFirst,const std::wstring& wsHashSecond)
	{
		if(wsHashFirst.empty() || wsHashSecond.empty() || wsHashFirst.length() != wsHashSecond.length())
			return false;

		std::string sFirstHash = U_TO_UTF8(wsHashFirst),sSecondHash = U_TO_UTF8(wsHashSecond);

		const char* cFirst = sFirstHash.c_str();
		const char* cSecond = sSecondHash.c_str();

		size_t sizeLen = wsHashFirst.length();
		for(size_t i = 0; i< sizeLen;i++)
		{
			if((cFirst[i]^cSecond[i]) != 0)
				return false;
		}
		return true;
	}

	void RemovingSpaces(std::wstring &wsAnnotation)
	{
		if(!wsAnnotation.empty())
		{
			while(wsAnnotation.back() == L' ')
				wsAnnotation.pop_back();
			while(wsAnnotation[0] == L' ')
			{
				if(wsAnnotation.size() > 1)
				{
					wsAnnotation = wsAnnotation.substr(1,wsAnnotation.size());
				}
			}
		}
	}

}
