#include "../../../../../DesktopEditor/common/File.h"
#include "../../../../../Common/3dParty/cryptopp/sha.h"
#include "../../../../../Common/3dParty/cryptopp/hex.h"
#include "../../../../../Common/3dParty/cryptopp/base64.h"
#include "../../../../../Common/3dParty/cryptopp/filters.h"
#include "../../../../../Common/3dParty/cryptopp/files.h"

namespace HashSM
{
	std::wstring HashingAnnotation(const std::wstring & wsAnnotation)
	{
		std::string sAnnotation = U_TO_UTF8(wsAnnotation),sResult;
		std::wstring wsResult;
		CryptoPP::SHA256 hash;
		CryptoPP::StringSource oStringSourse(sAnnotation,true, new CryptoPP::HashFilter(hash,new CryptoPP::HexEncoder(new CryptoPP::StringSink(sResult))));
		wsResult = UTF8_TO_U(sResult);
		return wsResult;
	}

	bool HashComparison(const std::wstring& wsHashFirst,const std::wstring& wsHashSecond)
	{
		if(wsHashFirst.length() != wsHashSecond.length())
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
}
