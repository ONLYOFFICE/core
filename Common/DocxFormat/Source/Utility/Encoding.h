#pragma once
#ifndef UTILITY_ENCODING_INCLUDE_H_
#define UTILITY_ENCODING_INCLUDE_H_

#include "../Base/Base.h"

class Encoding
{
public:

	static __forceinline const CStringA ansi2utf8   (const CStringA &sLine);
	static __forceinline const CStringW ansi2unicode(const CStringA &sLine);
	static __forceinline const CStringA utf82ansi   (const CStringA &sLine);
	static __forceinline const CStringW utf82unicode(const CStringA &sLine);
	static __forceinline const CStringA unicode2ansi(const CStringW &sLine);
	static __forceinline const CStringA unicode2utf8(const CStringW &sLine);

private:
	static __forceinline const CStringA wstring2string(const CStringW &sLine, const unsigned int unCodePage);
	static __forceinline const CStringW string2wstring(const CStringA &sLine, const unsigned int unCodePage);

};	

#endif // UTILITY_ENCODING_INCLUDE_H_