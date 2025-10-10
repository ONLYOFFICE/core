#ifndef SHAKEY_H
#define SHAKEY_H
#include<string>

namespace HashSM
{
	std::wstring HashingAnnotation(std::wstring & wsAnnotation, const bool &bUnicodeConversionString = false);
	bool HashComparison(const std::wstring& wsHashFirst,const std::wstring& wsHashSecond);
	void RemovingSpaces(std::wstring& wsAnnotation);
}

#endif // SHAKEY_H
